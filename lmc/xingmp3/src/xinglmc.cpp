/*____________________________________________________________________________
   
   FreeAmp - The Free MP3 Player

        MP3 Decoder originally Copyright (C) 1995-1997 Xing Technology
        Corp.  http://www.xingtech.com

   Portions Copyright (C) 1998 GoodNoise
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
#define sleep(x) Sleep(1000*x)
#endif

/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <assert.h>


#include "config.h"
#include "errors.h"
#include "xinglmc.h"
#include "pmoevent.h"
#include "event.h"
#include "eventdata.h"
#include "mutex.h"
#include "semaphore.h"
#include "lmc.h"
#include "log.h"

#if MP3_PROF
extern LogFile *g_Log;
#else
LogFile *g_Log = NULL;
#endif

#define DB printf("%08x: %s:%d\n", pthread_self(), __FILE__, __LINE__);

extern    "C"
{
   int       actually_decode;
}

extern    "C"
{
   LogicalMediaConverter *Initialize(LogFile *pLogFile)
   {
#if !MP3_PROF
      g_Log = pLogFile;
#endif

      return new XingLMC();
   }
}

static AUDIO audio_table[2][2] =
{
   {                            // [0][]
   // non iteger mode
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

/* Error codes:

   LMC        1000
   HTTP   pmi 2000
   OBS    omi 3000
   Souund pmo 4000
*/
static char *g_ErrorArray[9] =
{
   "Unknown error code",
   "The decoder cannot decode this MP3.",
   "The decoder could not be initialized.",
   "The LMC could not create the decoder thread.",
   "A PMI error occurred."
   "A PMO error occurred."
};

static int sample_rate_table[8] =
{
    22050L, 24000L, 16000L, 1L, 44100L, 48000L, 32000L, 1L
};

const int ID3_TAG_SIZE = 128;
const int iMaxDecodeRetries = 32;
const int iStreamingBufferSize = 64;  // in kbytes 
const int iDefaultBufferUpInterval = 3;
const int iMaxFrameSize = 1046;

const char *szFailRead = "Cannot read MP3 data from input plugin.";
const char *szFailWrite = "Cannot write audio data to output buffer.";

#define ENSURE_INITIALIZED if (!m_properlyInitialized) return kError_PluginNotInitialized;

XingLMC::XingLMC()
{
   m_input = NULL;
   m_output = NULL;
   m_properlyInitialized = false;
   m_target = NULL;
   m_decoderThread = NULL;
   m_seekMutex = new Mutex();
   m_pauseSemaphore = new Semaphore();
   m_bBufferingUp = false;
   m_iBufferUpdate = 0;
   m_iBitRate = 0;
   m_frameBytes = -1;
   m_szUrl = NULL;
   m_szError = NULL;
   m_iMaxWriteSize = 0;
   m_bExit = false;
   m_iBufferUpInterval = iDefaultBufferUpInterval;
   m_frameCounter = 0;
   m_frameWaitTill = 0;
   m_iBufferSize = iStreamingBufferSize * 1024;
   actually_decode = 0;
}

XingLMC::~XingLMC()
{
   //Debug_v("lmc dtor");
   Stop();
   //Debug_v("lmc stop done");
   if (m_output)
   {
      delete    m_output;

      m_output = NULL;
   }
   if (m_input)
   {
      delete    m_input;

      m_input = NULL;
   }
   if (m_pauseSemaphore)
   {
      delete    m_pauseSemaphore;

      m_pauseSemaphore = NULL;
   }
   if (m_seekMutex)
   {
      delete    m_seekMutex;

      m_seekMutex = NULL;
   }
   if (m_szUrl)
   {
      free(m_szUrl);
      m_szUrl = NULL;
   }
   //Debug_v("lmc dtor done");
}

Error     XingLMC::
Stop()
{
   if (m_decoderThread)
   {
      m_bExit = true;
      m_pauseSemaphore->Signal();

	  //Debug_v("output break");
      m_output->Break();
	  //Debug_v("output pause");
      m_output->Pause();
	  //Debug_v("input break");
      m_input->Break();

	  //Debug_v("decode join");
      m_decoderThread->Join();  // wait for thread to exit
	  //Debug_v("decode joined");
      g_Log->Log(LogDecode, "LMC: Decoder thread exited.\n");

	  //Debug_v("delete input");
      delete m_input;
      m_input = NULL;

	  //Debug_v("delete output");
      delete m_output;
      m_output = NULL;

	  //Debug_v("delete thread");
      delete    m_decoderThread;
	  //Debug_v("stop done");

      m_decoderThread = NULL;
   }

   return kError_NoErr;
}

Error XingLMC::Pause()
{
   m_output->Pause();
   m_input->Pause();

   return kError_NoErr;
}

Error XingLMC::Resume()
{
   if (m_input->Resume())
      m_output->Clear();

   m_output->Resume();

   return kError_NoErr;
}

Error XingLMC::Reset()
{
   ENSURE_INITIALIZED;
   return kError_NoErr;
}


Error XingLMC::SetTarget(EventQueue * eq)
{
   if (m_target = eq)
   {
      return kError_NoErr;
   }
   else
   {
      return kError_NullValueInvalid;
   }
}

Error XingLMC::SetTo(char *szUrl)
{
   m_szUrl = strdup(szUrl);

   return kError_NoErr;
}

Error XingLMC::SetPMI(PhysicalMediaInput * i)
{
   if (m_input = i)
   {
      return kError_NoErr;
   }
   else
   {
      return kError_NullValueInvalid;
   }
}

Error XingLMC::SetPMO(PhysicalMediaOutput * o)
{
   if (m_output = o)
   {
      return kError_NoErr;
   }
   else
   {
      return kError_NullValueInvalid;
   }
}

void XingLMC::SetVolume(int32 iVolume)
{
   if (m_output)
      m_output->SetVolume(iVolume);
}

int32 XingLMC::GetVolume(void)
{
   if (m_output)
      return m_output->GetVolume();
   else
      return -1;
}

bool XingLMC::IsStreaming()
{
   if (!m_input)
	   return false;

   return m_input->IsStreaming();
}

Error XingLMC::AdvanceBufferToNextFrame()
{
   int            iNumBytes;
	void          *pBufferBase;
	unsigned char *pBuffer;
   int32          iCount;
	Error          Err;

   iNumBytes = iMaxFrameSize;
   Err = BeginRead(pBufferBase, iNumBytes);
   if (Err != kError_NoErr)
   {
      ReportError(szFailRead);
      return Err;
   }

   for(;;)
   {
		 pBuffer = ((unsigned char *)pBufferBase) + 1;

       for(iCount = 0; iCount < iNumBytes - 1 &&
           !(*pBuffer == 0xFF && (*(pBuffer+1) & 0xF0) == 0xF0); 
           pBuffer++, iCount++)
               ; // <=== Empty body!
 
       g_Log->Log(LogDecode, "Skipped %d bytes in advance frame.\n", iCount + 1);
       if (m_input)
           m_input->EndRead(iCount + 1);

       if (iCount != 0 && iCount < iNumBytes - 1)
       {
          break;
       }

       iNumBytes = iMaxFrameSize;
       Err = BeginRead(pBufferBase, iNumBytes);
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
   int          iNumBytes, iLoop;
   unsigned int iForward;
   void        *pBuffer;
   Error        Err;

   for(iLoop = 0; iLoop < iMaxDecodeRetries; iLoop++)
   {
       iNumBytes = iMaxFrameSize;
       Err = BeginRead(pBuffer, iNumBytes, false);
       if (Err == kError_NoErr)
       {
           m_frameBytes = head_info3((unsigned char *)pBuffer,
			                            iNumBytes, &m_sMpegHead, 
                                     &m_iBitRate, &iForward);
           if (m_frameBytes > 0 && m_frameBytes < 1050 && 
               m_sMpegHead.option == 1)
           {
              MPEG_HEAD sHead;
              int       iFrameBytes, iBitRate;
            
              iFrameBytes = head_info3(((unsigned char *)pBuffer) + 
                                    m_frameBytes + iForward + m_sMpegHead.pad,
                                    iNumBytes - (m_frameBytes + iForward), 
                                    &sHead, &iBitRate, &iForward);
              if (m_input)
                 m_input->EndRead(0);

              if (iFrameBytes > 0 && iFrameBytes < 1050 &&
                  m_sMpegHead.option == 1)
                 return kError_NoErr;
           }
           else
              if (m_input)
                  m_input->EndRead(0);

           AdvanceBufferToNextFrame();
       }
		 else
		 {
           ReportError(szFailRead);
           return Err;
       }
   }

   return (Error)lmcError_DecodeFailed;
}

bool XingLMC::CanDecode()
{
   Error      Err;
   int32      dummy;
   bool       bRet = false;
   PropValue *pProp;

   if (!m_input)
   {
      g_Log->Error("CanDecode() called, with no PMI set.\n");
      return false;
   }

   m_propManager->GetProperty("InputBuffer", &pProp);
   if (pProp)
       m_iBufferSize = atoi(((StringPropValue *)pProp)->GetString()) * 1024;

   m_input->SetBufferSize(m_iBufferSize);

   if (!m_input->IsStreaming())
       m_input->Seek(dummy, 0, SEEK_FROM_START);

   Err = GetHeadInfo();
   if (Err != kError_NoErr)
   {
       g_Log->Log(LogDecode, "GetHeadInfo() in CanDecode() could not find the sync marker.\n");
       return false;
   }

   m_propManager->GetProperty("BufferUpInterval", &pProp);
   if (pProp)
   {
       m_iBufferUpInterval = atoi(((StringPropValue *)pProp)->GetString());
       if (m_iBufferUpInterval <= 0)
           m_iBufferUpInterval = iDefaultBufferUpInterval;
   }

   return true;
}

Error XingLMC::ExtractMediaInfo()
{
   int32           totalFrames = 0;
   int32           bytesPerFrame;
   size_t          end;
   unsigned char  *pID3Tag; 
   Error           Err;
   float           totalSeconds;
   MediaInfoEvent *pMIE;

   if (!m_input)
      return kError_NullValueInvalid;
 
   if (m_frameBytes < 0)
   {
       Err = GetHeadInfo();
		 if (Err != kError_NoErr)
		    return Err;
   }

   if (m_input->GetLength(end) == kError_FileSeekNotSupported)
      end = 0;

   pID3Tag = new unsigned char[ID3_TAG_SIZE];
   if (m_input->GetID3v1Tag(pID3Tag) != kError_NoErr)
   {
      memset(pID3Tag, 0, ID3_TAG_SIZE);
   }

   Id3TagInfo tag_info((char *)pID3Tag);

   int32     sampRateIndex = 4 * m_sMpegHead.id + m_sMpegHead.sr_index;
   int32     samprate = sample_rate_table[sampRateIndex];

   if ((m_sMpegHead.sync & 1) == 0)
        samprate = samprate / 2;    // mpeg25

   double    milliseconds_per_frame = 0;
   static int32 l[4] = {25, 3, 2, 1};

   int32     layer = l[m_sMpegHead.option];
   static double ms_p_f_table[3][3] =
   {
      {8.707483f, 8.0f, 12.0f},
      {26.12245f, 24.0f, 36.0f},
      {26.12245f, 24.0f, 36.0f}
   };

   milliseconds_per_frame = ms_p_f_table[layer - 1][m_sMpegHead.sr_index];

   if (end > 0)
   {
       bytesPerFrame = m_frameBytes;
       totalFrames = end / bytesPerFrame;
       totalSeconds = (float) ((double) totalFrames * 
                      (double) milliseconds_per_frame / 1000);
   }
   else
   {
       totalFrames = -1;
       totalSeconds = -1;
   }

   pMIE = new MediaInfoEvent(m_input->Url(), totalSeconds);
   if (!pMIE)
      return kError_OutOfMemory;

   if (tag_info.m_containsInfo)
   {

       ID3TagEvent *ite = new ID3TagEvent(tag_info);

       if (ite)
       {
           pMIE->AddChildEvent((Event *) ite);
           ite = NULL;
       }
       else
       {
           return kError_OutOfMemory;
       }
   }

   delete pID3Tag;

   /*LEAK*/MpegInfoEvent *mie = new MpegInfoEvent(totalFrames,
                       (float)(milliseconds_per_frame / 1000), m_frameBytes,
                       m_iBitRate, samprate, layer,
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

   if (m_target)
      m_target->AcceptEvent(pMIE);

   return kError_NoErr;
}

Error XingLMC::InitDecoder()
{
   Error          Err;

   if (!m_target || !m_input || !m_output)
   {
      return kError_NullValueInvalid;
   }
   m_properlyInitialized = false;

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
             m_iMaxWriteSize = (info->number_of_channels * 
	    		               (decinfo.bits / 8) * 1152);
		   else
             m_iMaxWriteSize = (info->number_of_channels * 
	    		               (decinfo.bits / 8) * 576);

         info->max_buffer_size = m_iMaxWriteSize;

         m_output->AcceptEvent(new PMOInitEvent(info));
   }
   else
   {
      g_Log->Log(LogDecode, "Audio decode init failed.\n");
      return (Error) lmcError_AudioDecodeInitFailed;
   }

   m_properlyInitialized = true;
   return (Error) kError_NoErr;
}

Error XingLMC::Decode(int32 iSkipNumFrames)
{   
   m_frameWaitTill = iSkipNumFrames;

   if (!m_decoderThread)
   {
      m_decoderThread = Thread::CreateThread();
      if (!m_decoderThread)
      {
         return (Error) lmcError_DecoderThreadFailed;
      }
      m_decoderThread->Create(XingLMC::DecodeWorkerThreadFunc, this);
   }

   return kError_NoErr;
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
   bool           result = true, bRet;
   int32          in_bytes, out_bytes;
   size_t         iBytesNeeded;
   size_t         iOutBytesNeeded;
   void          *pBuffer, *pOutBuffer;
   Error          Err;
   int            iLoop = 0;
   IN_OUT         x = {0, 0};

   in_bytes = out_bytes = 0;

   if (!m_input || !m_target || !m_output)
      return;

   Err = m_input->SetTo(m_szUrl);
   if (Err == kError_Interrupt)
      return;

   if (IsError(Err))
   {
       g_Log->Error("PMI initialization failed: %s\n", m_szError);
       ReportError("Cannot initialize the input plugin.\n");

       return;
   }

   bRet = CanDecode();
   if (!bRet)
   {
       g_Log->Error("CanDecode returned false.\n");
       ReportError("This LMC cannot decode this media\n");
       return;
   }

   Err = ExtractMediaInfo();
   if (Err == kError_Interrupt)
      return;

   if (IsError(Err))
   {
       g_Log->Error("ExtractMediaInfo failed: %d\n", Err);
       ReportError("This LMC cannot decode this media\n");

       return;
   }

   Err = InitDecoder();
   if (Err == kError_Interrupt)
      return;

   if (IsError(Err))
   {
       g_Log->Error("Initializing the decoder failed: %d\n", Err);
       ReportError("Initializing the decoder failed.");

       return;
   }

   for (m_frameCounter = 0; !m_bExit;)
   {
      if (m_frameCounter >= m_frameWaitTill)
      {
         actually_decode = 1;
      }

      m_seekMutex->Acquire(WAIT_FOREVER);


      m_output->AcceptEvent(new PMOTimeInfoEvent(m_frameCounter));

      for(iLoop = 0; !m_bExit && iLoop < iMaxDecodeRetries; iLoop++)
		{
          x.in_bytes = 0;
          iOutBytesNeeded = m_iMaxWriteSize;
          Err = m_output->BeginWrite(pOutBuffer, iOutBytesNeeded);
          if (Err == kError_Interrupt)
             break;

          if (Err != kError_NoErr)
          {
              m_seekMutex->Release();
              ReportError(szFailWrite);
              g_Log->Error("LMC: Cannot write to eventbuffer: %s (%d)\n", 
                        m_szError, Err); 
              return;
          }

          iBytesNeeded = iMaxFrameSize;
          Err = BeginRead(pBuffer, iBytesNeeded);
          if (Err == kError_Interrupt || Err == kError_InputUnsuccessful)
             break;

          if (Err != kError_NoErr)
          {
              m_seekMutex->Release();
              ReportError(szFailRead);
              g_Log->Error("LMC: Cannot read from pullbuffer: %s\n", m_szError); 
              return;
          }

          x = m_audioMethods.decode((unsigned char *)pBuffer, 
                                    (short *)pOutBuffer);
          if (x.in_bytes == 0)
		    {
             if (m_input)
                 m_input->EndRead(x.in_bytes);
             if (m_output)
             {
                 Err = m_output->EndWrite(x.in_bytes);
                 if (Err == kError_Interrupt)
                    break;

                 if (IsError(Err))
                 {
                     m_seekMutex->Release();
                     g_Log->Error("lmc: EndWrite returned %d\n", Err);
                     ReportError(szFailWrite);
                     
                     return;
                 }
             }
             g_Log->Log(LogDecode, "Audio decode failed. Resetting the decoder.\n");
             m_audioMethods.decode_init(&m_sMpegHead, m_frameBytes,
				                           0, 0, 0, 24000);

             Err = AdvanceBufferToNextFrame();
             if (Err == kError_Interrupt)
                 break;

             if (Err != kError_NoErr)
             {
                 m_seekMutex->Release();
                 g_Log->Error("LMC: Cannot advance to next frame: %d\n", Err);
                 ReportError("Cannot advance to next frame. Possible media corruption?");

                 return;
             }
			 }
			 else
          {
			    break;
          }
      }
      if (m_input)
         m_input->EndRead(x.in_bytes);
      if (m_output)
      {
#if __BYTE_ORDER != __LITTLE_ENDIAN
         x.out_bytes = cvt_to_wave(pOutBuffer, x.out_bytes);
#endif

#define _VISUAL_ENABLE_
#ifdef  _VISUAL_ENABLE_
         if (m_target)
             m_target->AcceptEvent(new SendVisBufEvent(x.out_bytes,
                           pOutBuffer,x.out_bytes));
#endif  //_VISUAL_ENABLE_
#undef  _VISUAL_ENABLE_


		 //Debug_v("writing %d bytes", x.out_bytes);
         Err = m_output->EndWrite(x.out_bytes);
         if (Err == kError_Interrupt || Err == kError_YouScrewedUp)
            break;

         if (IsError(Err))
         {
             m_seekMutex->Release();
             g_Log->Error("lmc: EndWrite returned %d\n", Err);
             ReportError(szFailWrite);
                     
             return;
         }
      }

      if (iLoop == iMaxDecodeRetries)
      {
         m_seekMutex->Release();
         g_Log->Error("LMC: Maximum number of retries reached"
                      " while trying to decode.\n");
         ReportError("Cannot decode this MP3. Is the MP3 corrupted?");

         return;
      }

      // EOF?
      if (x.in_bytes == 0)
         break;

      m_frameCounter++;

      in_bytes += x.in_bytes;
      m_seekMutex->Release();
   }
   if (m_bExit)
	  return;


   m_output->AcceptEvent(new PMOQuitEvent());
   m_output->WaitToQuit();

   m_target->AcceptEvent(new Event(INFO_DoneOutputting));

   return;
}

// This function encapsulates all the buffering event management
Error XingLMC::BeginRead(void *&pBuffer, unsigned int iBytesNeeded,
                         bool bBufferUp)
{
   time_t iNow;
   bool   bBuffering = false;
   int32  iInPercent, iOutPercent;

   if (m_input && (!m_input->IsStreaming() || m_iBitRate <= 0))
	    return m_input->BeginRead(pBuffer, iBytesNeeded);

   iInPercent = m_input->GetBufferPercentage();
   iOutPercent = m_output->GetBufferPercentage();

   time(&iNow);
  	if (iNow != m_iBufferUpdate)
  	{
  	    printf("Input: %3d%% Output: %3d%%\r", iInPercent, iOutPercent);
  	    //Debug_v("Input: %3d%% Output: %3d%%", iInPercent, iOutPercent);
  	    fflush(stdout);
       m_iBufferUpdate = iNow;
  	}

   if (bBufferUp && iInPercent < 5 && iOutPercent < 5)
   {
       int iBufferUpBytes;

       assert(m_iBufferSize > 0);
       assert(m_iBufferUpInterval > 0);
       assert(m_iBitRate > 0);

       iBufferUpBytes = (m_iBitRate * m_iBufferUpInterval * 1000) / 8192;
       if (iBufferUpBytes > m_iBufferSize)
          iBufferUpBytes = (m_iBufferSize * 10) / 8;
       //printf("iBufferUpBytes: %d Interval: %d bitrate: %d\n",
       //   iBufferUpBytes, m_iBufferUpInterval, m_iBitRate);

       printf("Buffering up...           \n");
       for(; !m_bExit;)
       {
           sleep(1);
           iInPercent = m_input->GetBufferPercentage();
           iOutPercent = m_output->GetBufferPercentage();
  	        printf("Input: %3d%% Output: %3d%%\r", iInPercent, iOutPercent);
           fflush(stdout);
           
           //printf("In: %d Needed: %d               \n", 
           //   m_input->GetNumBytesInBuffer(), iBufferUpBytes);
           if (m_input->GetNumBytesInBuffer() >= iBufferUpBytes)
              break;
           if (m_input->GetBufferPercentage() > 90)
           {
               printf("iBufferUpBytes: %d Interval: %d bitrate: %d\n",
                     iBufferUpBytes, m_iBufferUpInterval, m_iBitRate);
               printf("iBufferPercentage: %d\n",
                      m_input->GetBufferPercentage());
               break;
           }
       }
       printf("Done buffering up.              \n");
   }

	return m_input->BeginRead(pBuffer, iBytesNeeded);
}
		
	//if (m_target)
	//    m_target->AcceptEvent(new StreamBufferEvent(bBuffering, 
   //    iPercent));
	//if (m_target)
	//    m_target->AcceptEvent(new StreamBufferEvent(true, iPercent));
	//if (m_target)
	//    m_target->AcceptEvent(new StreamBufferEvent(false, iPercent));

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
    ENSURE_INITIALIZED;
    Error error = kError_NoErr;
        for(int i=0; i<32; i++)
                equalizer[i] = arrayEQ[i];
        return error;
}

Error XingLMC::SetEQData(bool enable) {
    ENSURE_INITIALIZED;
    Error error = kError_NoErr;
        enableEQ = enable;
        return error;
}
#endif  //_EQUALIZER_ENABLE_
#undef  _EQUALIZER_ENABLE_



Error     XingLMC::
ChangePosition(int32 position)
{
   ENSURE_INITIALIZED;
   Error     error = kError_NoErr;


   if (IsStreaming())
      return kError_FileSeekNotSupported;

   m_seekMutex->Acquire(WAIT_FOREVER);

   int32     dummy;

   m_output->Pause();

   m_output->Clear();

   m_input->Seek(dummy, position * m_frameBytes, SEEK_FROM_START);
   m_frameCounter = position;

   m_output->Resume();

   m_seekMutex->Release();

   return error;
}
