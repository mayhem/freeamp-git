/*____________________________________________________________________________
   
   FreeAmp - The Free MP3 Player

        MP3 Decoder originally Copyright (C) 1995-1997 Xing Technology
        Corp.  http://www.xingtech.com

   Portions Copyright (C) 1998-1999 EMusic.com
   Portions Copyright (C) 1998 "Michael Bruun Petersen" <mbp@image.dk>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, Write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
   
   $Id$
____________________________________________________________________________*/

#ifdef WIN32
#include <windows.h>
#endif

/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <assert.h>
#include <string.h>

#include "config.h"
#include "errors.h"
#include "xinglmc.h"
#include "pmoevent.h"
#include "eventbuffer.h"
#include "event.h"
#include "eventdata.h"
#include "mutex.h"
#include "semaphore.h"
#include "preferences.h"
#include "lmc.h"
#include "facontext.h"
#include "log.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n",  __FILE__, __LINE__);

const int iInitialOutputBufferSize = 64512;

extern    "C"
{
   LogicalMediaConverter *Initialize(FAContext *context)
   {
      return new XingLMC(context);
   }
}

static AUDIO audio_table[2][2] =
{
   {                            // [0][]
   // non integer mode
      {audio_decode_init, audio_decode_info, audio_decode},
      {audio_decode8_init, audio_decode8_info, audio_decode8},  // 8 bit
                        // methods
   },
   {                            // [1][]
   // integer mode
      {i_audio_decode_init, i_audio_decode_info, i_audio_decode},
      {audio_decode8_init, audio_decode8_info, audio_decode8},  // 8 bit
                        // methods
   }
};

static int sample_rate_table[8] =
{
    22050L, 24000L, 16000L, 1L, 44100L, 48000L, 32000L, 1L
};

const int iMaxDecodeRetries = 32;
const int iStreamingBufferSize = 64;  // in kbytes 
const int iDefaultBufferUpInterval = 3;

// TODO: Sometimes after a seek the decoder will think that it is
// parsing a wrong sized stream and think max frame size if greater than
// 1046. The after seek sych code should be improved to detect
// crap changes like this -- it also sometimes causes static output
const int iMaxFrameSize = 1441;
const int iNumSanityCheckFrames = 3;
const int iInitialFrameSize = iMaxFrameSize * iNumSanityCheckFrames;

const char *szFailRead = "Cannot read MP3 data from input plugin.";
const char *szFailWrite = "Cannot write audio data to output buffer.";
const char *szCannotDecode = BRANDING" cannot play this file/stream. This file/stream may be corrupted.";

XingLMC::XingLMC(FAContext *context) :
         LogicalMediaConverter(context)
{
   m_pContext = context;

   m_decoderThread = NULL;
   m_bBufferingUp = false;
   m_iBufferUpdate = 0;
   m_iBitRate = 0;
   m_frameBytes = -1;
   m_szUrl = NULL;
   m_szError = NULL;
   m_iMaxWriteSize = 0;
   m_iBufferUpInterval = iDefaultBufferUpInterval;
   m_frameCounter = 0;
   m_iBufferSize = iStreamingBufferSize * 1024;
   m_pPmi = NULL;
   m_pPmo = NULL;
   m_fpFile = NULL;
   m_pLocalReadBuffer = NULL;
   m_pXingHeader = NULL;
}

XingLMC::~XingLMC()
{
   if (m_decoderThread)
   {
      m_bExit = true;
      m_pPauseSem->Signal();
      m_pSleepSem->Signal();

      m_decoderThread->Join();
      m_pContext->log->Log(LogDecode, "LMC: Decoder thread exited.\n");

      delete m_decoderThread;
      m_decoderThread = NULL;
   }
   if (m_pXingHeader)
   {
      delete m_pXingHeader->toc;
      delete m_pXingHeader;
   }
}

Error XingLMC::Prepare(PullBuffer *pInputBuffer, PullBuffer *&pOutBuffer)
{
   m_pInputBuffer = pInputBuffer;


   m_pOutputBuffer = new EventBuffer(iInitialOutputBufferSize, 0, 
                                    m_pContext);

   if (!m_decoderThread)
   {
      m_decoderThread = Thread::CreateThread();
      if (!m_decoderThread)
      {
         return (Error) lmcError_DecoderThreadFailed;
      }
      m_decoderThread->Create(XingLMC::DecodeWorkerThreadFunc, this);
   }

   pOutBuffer =  m_pOutputBuffer;

   return kError_NoErr;
}

void XingLMC::Clear()
{
   if (m_pOutputBuffer)
      ((EventBuffer *)m_pOutputBuffer)->Clear();
}

Error XingLMC::AdvanceBufferToNextFrame()
{
	void          *pBufferBase;
	unsigned char *pBuffer;
   int32          iCount;
	Error          Err;

   Err = BeginRead(pBufferBase, iMaxFrameSize);
   if (Err == kError_EndOfStream)
      return Err;

   if (Err != kError_NoErr)
   {
      ReportError(szFailRead);
      return Err;
   }

   for(;;)
   {
		 pBuffer = ((unsigned char *)pBufferBase) + 1;

       for(iCount = 0; iCount < iMaxFrameSize - 1 &&
           !(*pBuffer == 0xFF && ((*(pBuffer+1) & 0xF0) == 0xF0 || 
                                  (*(pBuffer+1) & 0xF0) == 0xE0)); 
           pBuffer++, iCount++)
               ; // <=== Empty body!

       m_pContext->log->Log(LogDecode, "Skipped %d bytes in advance frame.\n", 
                           iCount + 1);
       EndRead(iCount + 1);

       if (iCount != 0 && iCount < iMaxFrameSize - 1)
       {
          break;
       }

       Err = BeginRead(pBufferBase, iMaxFrameSize);
       if (Err == kError_EndOfStream)
           return Err;

       if (Err != kError_NoErr)
       {
           ReportError(szFailRead);
	       return Err;
       }
   }

	return kError_NoErr;
}

Error XingLMC::GetHeadInfo()
{
   int          iLoop, iFrame, iOffset;
   int          iLastSRIndex = -1, iLastOption = -1;
   int          iLastId = -1, iLastMode = -1;
   unsigned int iForward;
   void        *pBuffer;
   Error        Err;

   for(iLoop = 0; iLoop < iMaxDecodeRetries; iLoop++)
   {
       Err = BeginRead(pBuffer, iInitialFrameSize, false);
       if (IsError(Err))
       {
          if (Err != kError_EndOfStream && Err != kError_Interrupt)
          {
              ReportError(szFailRead);
          }
          return Err;
       }

       for(iFrame = 0, iOffset = 0; iFrame < iNumSanityCheckFrames; iFrame++)
       {
           m_frameBytes = head_info3(((unsigned char *)pBuffer) + iOffset,
			                         iInitialFrameSize - iOffset, &m_sMpegHead, 
                                     (int*)&m_iBitRate, &iForward);

           if (m_frameBytes > 0 && iFrame == 0)
           {
               iOffset += m_frameBytes + iForward + m_sMpegHead.pad;
               iLastSRIndex = m_sMpegHead.sr_index;
               iLastOption = m_sMpegHead.option;
               iLastId = m_sMpegHead.id;
               iLastMode = m_sMpegHead.mode;
               continue;
           }
               
           if (m_frameBytes > 0 && m_frameBytes < iMaxFrameSize && 
              (m_sMpegHead.option == 1 || m_sMpegHead.option == 2) &&
              iLastSRIndex == m_sMpegHead.sr_index &&
              iLastOption == m_sMpegHead.option && 
              iLastId == m_sMpegHead.id && 
              iLastMode == m_sMpegHead.mode)
           {
              iOffset += m_frameBytes + iForward + m_sMpegHead.pad;
              iLastSRIndex = m_sMpegHead.sr_index;
              iLastOption = m_sMpegHead.option;
              iLastId = m_sMpegHead.id;
              iLastMode = m_sMpegHead.mode;

              if (iFrame < iNumSanityCheckFrames - 1)
                 continue;

              if (m_pXingHeader)
              {
                 delete m_pXingHeader->toc;
                 delete m_pXingHeader;
              }   

              m_pXingHeader = new XHEADDATA;
              m_pXingHeader->toc = new unsigned char[100];
              if (!GetXingHeader(m_pXingHeader, (unsigned char *)pBuffer))
              {
                 delete m_pXingHeader->toc;
                 delete m_pXingHeader;
                 m_pXingHeader = NULL;
              }    
              continue;
           }
           else
              EndRead(0);

           Err = AdvanceBufferToNextFrame();
           if (Err != kError_NoErr)
              return Err;

           break;
       }
       if (iFrame == iNumSanityCheckFrames)
       {
           EndRead(0);
           return kError_NoErr;
       }
   }

   return (Error)lmcError_DecodeFailed;
}

vector<char *> *XingLMC::GetExtensions(void)
{
   vector<char *> *extList = new vector<char *>;

   extList->push_back("MP3");
   extList->push_back("MP2");
   extList->push_back("MP1");

   return extList;
}

Error XingLMC::CanDecode()
{
   Error      Err;

   if (!m_pInputBuffer)
   {
      m_pContext->log->Error("CanDecode() called, with no PMI set.\n");
      return kError_PluginNotInitialized;
   }

   Err = GetHeadInfo();
   if (Err == kError_Interrupt)
      return Err;

   if (Err != kError_NoErr)
   {
       m_pContext->log->Log(LogDecode, "GetHeadInfo() in CanDecode() could not find the sync marker.\n");
       return Err;
   }

   if (IsError(m_pContext->prefs->
	       GetStreamBufferInterval(&m_iBufferUpInterval)))
      m_iBufferUpInterval = iDefaultBufferUpInterval;

   return kError_NoErr;
}

Error XingLMC::ExtractMediaInfo()
{
   int32           totalFrames = 0, samprate, layer;
   Error           eRet;
   float           totalSeconds, fMsPerFrame;
   MediaInfoEvent *pMIE;

   eRet = GetBitstreamStats(totalSeconds, fMsPerFrame,
                            totalFrames, samprate, layer);
   if (IsError(eRet))
      return eRet; 

   pMIE = new MediaInfoEvent(m_pPmi->Url(), totalSeconds);
   if (!pMIE)
      return kError_OutOfMemory;

   /*LEAK*/MpegInfoEvent *mie = new MpegInfoEvent(totalFrames,
                  (float)(fMsPerFrame / 1000), m_frameBytes,
                  (m_pXingHeader) ? 0 : m_iBitRate, samprate, layer,
                  (m_sMpegHead.sync == 2) ? 3 : (m_sMpegHead.id) ? 1 : 2,
                  (m_sMpegHead.mode == 0x3 ? 1 : 2), 
					   m_sMpegHead.original, m_sMpegHead.prot,
                  m_sMpegHead.emphasis, m_sMpegHead.mode, 
					   m_sMpegHead.mode_ext);

   if (mie)
   {
       pMIE->AddChildEvent((Event *) mie);
       mie = NULL;
   }
   else
   {
       return kError_OutOfMemory;
   }

   if (m_pTarget)
      m_pTarget->AcceptEvent(pMIE);

   return kError_NoErr;
}

Error XingLMC::GetBitstreamStats(float &fTotalSeconds, float &fMsPerFrame,
                                 int32 &iTotalFrames, int32 &iSampleRate, 
                                 int32 &iLayer)
{
   Error        Err;
   static int32 l[4] = {25, 3, 2, 1};
   int32        sampRateIndex;

   fTotalSeconds = fMsPerFrame = 0.0;
   iTotalFrames = iSampleRate = iLayer = 0;

   if (!m_pPmi && !m_fpFile)
      return kError_NullValueInvalid;
 
   if (m_frameBytes < 0)
   {
       Err = GetHeadInfo();
       if (Err != kError_NoErr)
		    return Err;
   }

   if (m_fpFile)
   {
      fseek(m_fpFile, 0, SEEK_END);
      m_lFileSize = ftell(m_fpFile);
      fseek(m_fpFile, 0, SEEK_SET);
   }
   else   
      if (m_pPmi->GetLength(m_lFileSize) == kError_FileSeekNotSupported)
          m_lFileSize = 0;

   sampRateIndex = 4 * m_sMpegHead.id + m_sMpegHead.sr_index;
   iSampleRate = sample_rate_table[sampRateIndex];

   if ((m_sMpegHead.sync & 1) == 0)
        iSampleRate = iSampleRate / 2;    // mpeg25

   iLayer = l[m_sMpegHead.option];
   if (m_sMpegHead.id == 1)
      fMsPerFrame = (double)(1152 * 1000) / (double)iSampleRate;
   else   
      fMsPerFrame = (double)(576 * 1000) / (double)iSampleRate;

   if (m_lFileSize > 0)
   {
       if (m_pXingHeader)
       {
          iTotalFrames = m_pXingHeader->frames;
          fTotalSeconds = (float) ((double) iTotalFrames * 
                                   (double) fMsPerFrame / 1000);
       }                            
       else    
       {
          iTotalFrames = m_lFileSize / m_frameBytes;
          fTotalSeconds = (float)((double) iTotalFrames * 
                                  (double) fMsPerFrame / 1000);
          fTotalSeconds -= 1;                        
       }    
   }
   else
   {
       iTotalFrames = -1;
       fTotalSeconds = -1;
   }
   
   return kError_NoErr;
}

uint32 XingLMC::CalculateSongLength(const char *szUrl)
{
    char   path[_MAX_PATH];
    uint32 len = _MAX_PATH;
    float  fTotalSeconds, fMsPerFrame;
    int32    iTotalFrames, iSampleRate, iLayer;
    Error  eRet;

    URLToFilePath(szUrl, path, &len);
    m_fpFile = fopen(path, "rb");
    if (m_fpFile == NULL)
       return 0;

    eRet = GetBitstreamStats(fTotalSeconds, fMsPerFrame, iTotalFrames, 
                              iSampleRate, iLayer);
    fclose(m_fpFile);
    m_fpFile = NULL;
                              
    if (!IsError(eRet))   
    {
        if (fTotalSeconds < 0)
           return 0;

        return (int)fTotalSeconds;
    }
    
    return 0;
}

int XingLMC::ExtractI4(unsigned char *buf)
{
    int x;
    // big endian extract
    
    x = buf[0];
    x <<= 8;
    x |= buf[1];
    x <<= 8;
    x |= buf[2];
    x <<= 8;
    x |= buf[3];
    
    return x;
}

int XingLMC::GetXingHeader(XHEADDATA *X,  unsigned char *buf)
{
    int i, head_flags;
    int h_id, h_mode, h_sr_index;
    static int sr_table[4] = { 44100, 48000, 32000, 99999 };
    
    // get Xing header data
    X->flags = 0;     // clear to null incase fail
    
    // get selected MPEG header data
    h_id       = (buf[1] >> 3) & 1;
    h_sr_index = (buf[2] >> 2) & 3;
    h_mode     = (buf[3] >> 6) & 3;
    
    
    // determine offset of header
    if( h_id ) {        // mpeg1
        if( h_mode != 3 ) buf+=(32+4);
        else              buf+=(17+4);
    }
    else {      // mpeg2
        if( h_mode != 3 ) buf+=(17+4);
        else              buf+=(9+4);
    }
    
    if( buf[0] != 'X' ) return 0;    // fail
    if( buf[1] != 'i' ) return 0;    // header not found
    if( buf[2] != 'n' ) return 0;
    if( buf[3] != 'g' ) return 0;
    buf+=4;
    
    X->h_id = h_id;
    X->samprate = sr_table[h_sr_index];
    if( h_id == 0 ) X->samprate >>= 1;
    
    head_flags = X->flags = ExtractI4(buf); buf+=4;      // get flags
    
    if( head_flags & FRAMES_FLAG ) {X->frames   = ExtractI4(buf); buf+=4;}
    if( head_flags & BYTES_FLAG )  {X->bytes = ExtractI4(buf); buf+=4;}
    
    if( head_flags & TOC_FLAG ) {
        if( X->toc != NULL ) {
            for(i=0;i<100;i++) X->toc[i] = buf[i];
        }
        buf+=100;
    }
    
    X->vbr_scale = -1;
    if( head_flags & VBR_SCALE_FLAG )  {X->vbr_scale = ExtractI4(buf); buf+=4;}
    
    return 1;       // success
}

int XingLMC::SeekPoint(unsigned char TOC[100], int file_bytes, float percent)
{
    // interpolate in TOC to get file seek point in bytes
    int a, seekpoint;
    float fa, fb, fx;
    
    if( percent < 0.0f )   percent = 0.0f;
    if( percent > 100.0f ) percent = 100.0f;
    
    a = (int)percent;
    if( a > 99 ) a = 99;
    fa = TOC[a];
    if( a < 99 ) {
        fb = TOC[a+1];
    }
    else {
        fb = 256.0f;
    }
    
    fx = fa + (fb-fa)*(percent-a);
    
    seekpoint = (int)((1.0f/256.0f)*fx*file_bytes); 
    
    return seekpoint;
}

Error XingLMC::InitDecoder()
{
   Error          Err;

   if (!m_pTarget || !m_pPmi || !m_pPmo || !m_pInputBuffer || !m_pOutputBuffer)
   {
      return kError_NullValueInvalid;
   }

   if (m_frameBytes < 0)
   {
       Err = GetHeadInfo();
		 if (Err != kError_NoErr)
		    return Err;
   }

   // select decoder
   m_audioMethods = audio_table[0][0];       // not integer, non 8 bit mode

   if (m_audioMethods.decode_init(&m_sMpegHead,
                                  m_frameBytes,
                                  0 /* reduction code */ ,
                                  0 /* transform code (ignored) */ ,
                                  0 /* convert code */ ,
                                  24000 /* freq limit */ ))
   {
         DEC_INFO      decinfo;
         int32         iNewSize;
         Error         result;

         m_audioMethods.decode_info(&decinfo);

#if __BYTE_ORDER != __LITTLE_ENDIAN
         cvt_to_wave_init(decinfo.bits);
#endif

         OutputInfo *info;

         info = new OutputInfo;

         info->bits_per_sample = decinfo.bits;
         info->number_of_channels = decinfo.channels;
         info->samples_per_second = decinfo.samprate;

		   if (m_sMpegHead.id)
           {
             m_iMaxWriteSize = (info->number_of_channels * 
	    		               (decinfo.bits / 8) * 1152);
             info->samples_per_frame = 1152;
           }                    
		   else
           {
             m_iMaxWriteSize = (info->number_of_channels * 
	    		               (decinfo.bits / 8) * 576);
             info->samples_per_frame = 576;
           }                    

         info->max_buffer_size = m_iMaxWriteSize;

         m_pContext->prefs->GetOutputBufferSize(&iNewSize);
         iNewSize *= 1024;
   
         iNewSize -= iNewSize % m_iMaxWriteSize;
         result = m_pOutputBuffer->Resize(iNewSize, 0);
         if (IsError(result))
         {
            ReportError("Internal buffer sizing error occurred.");
            m_pContext->log->Error("Resize output buffer failed.");
            return result;
         } 

         ((EventBuffer *)m_pOutputBuffer)->AcceptEvent(new PMOInitEvent(info));
   }
   else
   {
      m_pContext->log->Log(LogDecode, "Audio decode init failed.\n");
      return (Error) lmcError_AudioDecodeInitFailed;
   }

   return (Error) kError_NoErr;
}


#if MP3_PROF
extern "C"
{
   etext();
   monstartup();
   _mcleanup();
}
#endif

void XingLMC::DecodeWorkerThreadFunc(void *pxlmc)
{
   if (pxlmc)
   {
      XingLMC  *xlmc = (XingLMC *) pxlmc;

#if MP3_PROF
      monstartup(0x08040000, etext);
#endif

      xlmc->DecodeWork();

#if MP3_PROF
      _mcleanup();
#endif
   }
}

void XingLMC::DecodeWork()
{
   void          *pBuffer, *pOutBuffer;
   Error          Err;
   int32          iLoop = 0, iValue;
   IN_OUT         x = {0, 0};

   assert(m_pPmi);
   assert(m_pPmo);

   m_pSleepSem->Wait();

   m_pPmi->Wake();

   Err = CanDecode();
   if (Err == kError_Interrupt)
      return;
   if (Err != kError_NoErr)
   {
       m_pContext->log->Error("CanDecode returned false.\n");
       ReportError(szCannotDecode);
       return;
   }

   Err = ExtractMediaInfo();
   if (Err == kError_Interrupt)
      return;

   if (IsError(Err))
   {
       m_pContext->log->Error("ExtractMediaInfo failed: %d\n", Err);
       ReportError(szCannotDecode);

       return;
   }

   Err = InitDecoder();
   if (Err == kError_Interrupt)
      return;

   if (IsError(Err))
   {
       m_pContext->log->Error("Initializing the decoder failed: %d\n", Err);
       ReportError("Initializing the decoder failed.");

       return;
   }

   m_pContext->prefs->GetDecoderThreadPriority(&iValue);
   m_decoderThread->SetPriority(iValue);

   for (m_frameCounter = 0; !m_bExit;)
   {
      ((EventBuffer *)m_pOutputBuffer)->AcceptEvent(
             new PMOTimeInfoEvent(m_frameCounter));

      // TODO: This loop needs to be terminated after 64k worth of data..
      for(; !m_bExit; )
		{
          if (m_bPause)
          {
              m_pPauseSem->Wait();
              if (m_bExit)
                  break;
          }

          x.in_bytes = 0;
          Err = m_pOutputBuffer->BeginWrite(pOutBuffer, m_iMaxWriteSize);
          if (Err == kError_Interrupt)
          {
              break;
          }
          if (Err == kError_BufferTooSmall)
          {
              if (Sleep())
                  break;

              continue;
          }
          if (Err != kError_NoErr)
          {
              ReportError(szFailWrite);
              m_pContext->log->Error("LMC: Cannot write to eventbuffer: %s (%d)\n", 
                        m_szError, Err); 
              return;
          }

          Err = BeginRead(pBuffer, iMaxFrameSize);
          if (Err == kError_Interrupt)
          {
              m_pOutputBuffer->EndWrite(0);
              break;
          }

          if (Err == kError_EndOfStream)
          {
              m_pOutputBuffer->EndWrite(0);
              ((EventBuffer *)m_pOutputBuffer)->AcceptEvent(new PMOQuitEvent());
              return;
          }
          if (Err == kError_NoDataAvail)
          {
              m_pOutputBuffer->EndWrite(0);
              if (Sleep())
                  break;

              continue;
          }

          if (Err != kError_NoErr)
          {
              ReportError(szFailRead);
              m_pContext->log->Error("LMC: Cannot read from pullbuffer: %s\n", m_szError); 
              return;
          }

          x = m_audioMethods.decode((unsigned char *)pBuffer, 
                                    (short *)pOutBuffer);
          if (x.in_bytes == 0)
		    {
             EndRead(x.in_bytes);
             m_pOutputBuffer->EndWrite(x.in_bytes);
             m_pContext->log->Log(LogDecode, "Audio decode failed. "
                                             "Resetting the decoder.\n");

             Err = AdvanceBufferToNextFrame();
             if (Err == kError_Interrupt)
                 break;

             if (Err == kError_EndOfStream)
             {
                 ((EventBuffer *)m_pOutputBuffer)->AcceptEvent(new PMOQuitEvent());
                 return;
             }

             if (Err != kError_NoErr)
             {
                 m_pContext->log->Error("LMC: Cannot advance to next frame: %d\n", Err);
                 ReportError("Cannot advance to next frame. Possible media corruption?");
                 return;
             }
			 m_audioMethods.decode_init(&m_sMpegHead, m_frameBytes, 0, 0, 0, 24000);
		  }
			 else
          {
			    break;
          }
      }
      if (m_bExit || Err == kError_Interrupt)
      {
          return;
      }
      EndRead(x.in_bytes);
      m_pPmi->Wake();

      if (m_pOutputBuffer)
      {
#if __BYTE_ORDER != __LITTLE_ENDIAN
         x.out_bytes = cvt_to_wave(pOutBuffer, x.out_bytes);
#endif

#define _VISUAL_ENABLE_
#ifdef  _VISUAL_ENABLE_
//         if (m_pTarget)
//         {
//             SendVisBufEvent *e = new SendVisBufEvent(x.out_bytes,
//                                    pOutBuffer,x.out_bytes);
//             m_pTarget->AcceptEvent(e);
//             m_pTarget->AcceptEvent(new SendVisBufEvent(x.out_bytes,
//                                    pOutBuffer,x.out_bytes));
//         }
#endif  //_VISUAL_ENABLE_
#undef  _VISUAL_ENABLE_

         Err = m_pOutputBuffer->EndWrite(x.out_bytes);
         if (Err == kError_Interrupt)
            break;

         if (IsError(Err))
         {
             m_pContext->log->Error("lmc: EndWrite returned %d\n", Err);
             ReportError(szFailWrite);
             return;
         }
      }

      if (iLoop == iMaxDecodeRetries)
      {
         m_pContext->log->Error("LMC: Maximum number of retries reached"
                      " while trying to decode.\n");
         ReportError("Cannot decode this MP3. Is the MP3 corrupted?");
         return;
      }

      m_frameCounter++;
   }

   return;
}

// This function encapsulates all the buffering event management
Error XingLMC::BeginRead(void *&pBuffer, unsigned int iBytesNeeded,
                         bool bBufferUp)
{
   int32  iInPercent, iOutPercent;
   unsigned int iBufferUpBytes;

   if (m_fpFile)
   {
       int iRead;
       
       pBuffer = new char[iBytesNeeded];
       m_pLocalReadBuffer = (char*)pBuffer;
       
       iRead = fread(pBuffer, sizeof(char), iBytesNeeded, m_fpFile);
       if (iRead != (int)iBytesNeeded)
          return kError_EndOfStream;
          
       return kError_NoErr;
   }

   if (m_pPmi && m_pInputBuffer && (!m_pPmi->IsStreaming() || m_iBitRate <= 0))
   {
	   return BlockingBeginRead(pBuffer, iBytesNeeded);
   }

   iInPercent = m_pInputBuffer->GetBufferPercentage();
   iOutPercent = m_pOutputBuffer->GetBufferPercentage();

   // If the input buffer is getting too full, discard some bytes.
   // This could be caused by a soundcard with slow playback or 
   // a host that is sending data too quicky. Clock-drift-a-moo!
   if (iOutPercent > 90 && iInPercent > 90 && m_pPmi->UseBufferReduction())
   {
       m_pInputBuffer->DiscardBytes();
   }

   iBufferUpBytes = (m_iBitRate * m_iBufferUpInterval * 1000) / 8192;
   if (iBufferUpBytes > (unsigned)m_iBufferSize)
       iBufferUpBytes = m_iBufferSize / 2;

   if (bBufferUp && iOutPercent < 1 &&
       m_pInputBuffer->GetNumBytesInBuffer() < iBufferUpBytes / 2)
   {
       assert(m_iBufferSize > 0);
       assert(m_iBufferUpInterval > 0);
       assert(m_iBitRate > 0);

       ReportStatus("Buffering up...");
       m_pTarget->AcceptEvent(new StreamBufferEvent(true, iInPercent, 
                                                    iOutPercent));
       for(; !m_bExit;)
       {
           usleep(1000000);
           iInPercent = m_pInputBuffer->GetBufferPercentage();
           iOutPercent = m_pOutputBuffer->GetBufferPercentage();

           m_pTarget->AcceptEvent(new StreamBufferEvent(true, iInPercent, 
                                                    iOutPercent));

           if (m_pInputBuffer->GetNumBytesInBuffer() >= iBufferUpBytes)
           {
               break;
           }    
           if (m_pInputBuffer->GetBufferPercentage() > 90)
           {
               break;
           }
       }
       m_pTarget->AcceptEvent(new StreamBufferEvent(false, iInPercent, 
                                                    iOutPercent));
       ReportStatus("Playing stream...");
   }

	return BlockingBeginRead(pBuffer, iBytesNeeded);
}

Error XingLMC::BlockingBeginRead(void *&pBuffer, unsigned int iBytesNeeded)
{
   Error eRet;

   for(;;)
   {
       eRet = m_pInputBuffer->BeginRead(pBuffer, iBytesNeeded);
       if (eRet == kError_NoDataAvail)
       {
           m_pPmi->Wake();
           if (Sleep())
              return kError_Interrupt;
           continue;
       }
       break;
   }

   return eRet;
}

Error XingLMC::EndRead(size_t iBytesUsed)
{
   if (m_fpFile)
   {
      delete m_pLocalReadBuffer;
      m_pLocalReadBuffer = NULL;
      return kError_NoErr;
   }
       
   return m_pInputBuffer->EndRead(iBytesUsed);
}

		
#define _EQUALIZER_ENABLE_
#ifdef  _EQUALIZER_ENABLE_

extern "C" {
float equalizer[32] = {
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
};
int enableEQ = false;
	   }

Error XingLMC::SetEQData(float *arrayEQ) {
    Error error = kError_NoErr;
        for(int i=0; i<32; i++)
                equalizer[i] = arrayEQ[i];
        return error;
}

Error XingLMC::SetEQData(bool enable) {
    Error error = kError_NoErr;
        enableEQ = enable;
        return error;
}
#endif  //_EQUALIZER_ENABLE_
#undef  _EQUALIZER_ENABLE_

Error XingLMC::ChangePosition(int32 position)
{
   int32   dummy;
   uint32  lSeekTo;

   m_frameCounter = position;
   if (m_pXingHeader)
      lSeekTo = XingLMC::SeekPoint(m_pXingHeader->toc, m_lFileSize, 
                                   (float)position * 100/
                                   (float)m_pXingHeader->frames);
   else
      lSeekTo = position * m_frameBytes;
      
   m_pPmi->Seek(dummy, lSeekTo, SEEK_FROM_START);

   return kError_NoErr;
}

