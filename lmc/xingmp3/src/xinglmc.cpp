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

/* system headers */
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>
#include <assert.h>

/* project headers */
#include "config.h"
#include "errors.h"
#include "xinglmc.h"
#include "event.h"
#include "eventdata.h"
#include "mutex.h"
#include "semaphore.h"
#include "lmc.h"

#define DB printf("%s:%d\n", __FILE__, __LINE__);

extern    "C"
{
   int       actually_decode;
}

extern    "C"
{
   LogicalMediaConverter *Initialize()
   {
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

static char *g_ErrorArray[9] =
{
   "Invalid Error Code",
   "head_info2 return 0 bytes/frame",
   "audio.decode_init failed",
   "bs_fill() failed",
   "creation of decoder thread failed",
   "output didn't return the same number of bytes we wrote",
   "decoder method didn't return anything",
   "seeking and reading ID3v1(.1) tag failed",
   "output initialization failed"
};

static int sample_rate_table[8] =
{
    22050L, 24000L, 16000L, 1L, 44100L, 48000L, 32000L, 1L
};

const int ID3_TAG_SIZE = 128;
const int iMaxDecodeRetries = 6;     // one partial frame and five full frames
const int iInitialBufferSize = 1100; // just larger than the largest frame size
const int iStreamingBufferSize = 5;  // in seconds

#define ENSURE_INITIALIZED if (!m_properlyInitialized) return kError_PluginNotInitialized;

Error     XingLMC::
SetTarget(EventQueue * eq)
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

Error     XingLMC::
SetPMI(PhysicalMediaInput * i)
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

Error     XingLMC::
SetPMO(PhysicalMediaOutput * o)
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

bool XingLMC::
IsStreaming()
{
   if (!m_input)
	   return false;

   return m_input->IsStreaming();
}

const char *XingLMC::
GetErrorString(int32 error)
{
   if ((error <= lmcError_MinimumError) || (error >= lmcError_MaximumError))
   {
      return g_ErrorArray[0];
   }
   return g_ErrorArray[error - lmcError_MinimumError];
}

Error XingLMC::AdvanceBufferToNextFrame()
{
   int            iNumBytes;
	void          *pBufferBase;
	unsigned char *pBuffer;
   int32          iCount;
	Error          Err;

   iNumBytes = iInitialBufferSize;
   Err = BeginRead(pBufferBase, iNumBytes);
   if (Err != kError_NoErr)
   {
	    return Err;
   }

   for(;;)
   {
		 pBuffer = ((unsigned char *)pBufferBase) + 1;
	    
       for(iCount = 0; iCount < iNumBytes - 1 &&
           !(*pBuffer == 0xFF && (*(pBuffer+1) & 0xF0) == 0xF0); 
           pBuffer++, iCount++)
               ; // <=== Empty body!
  
       m_input->EndRead(iCount + 1);

       if (iCount != 0 && iCount < iNumBytes - 1)
       {
          break;
       }

       iNumBytes = iInitialBufferSize;
       BeginRead(pBufferBase, iNumBytes);
       if (Err != kError_NoErr)
       {
	        return Err;
       }
   }

	return kError_NoErr;
}

Error XingLMC::GetHeadInfo()
{
   int          iNumBytes, iLoop;
	unsigned int iForward;
	bool         bRet;
	void        *pBuffer;
	Error        Err;

   for(iLoop = 0; iLoop < iMaxDecodeRetries; iLoop++)
   {
       iNumBytes = iInitialBufferSize;
       Err = BeginRead(pBuffer, iNumBytes);
       if (Err == kError_NoErr)
       {
           m_frameBytes = head_info3((unsigned char *)pBuffer,
			                            iNumBytes, &m_sMpegHead, 
                                     &m_iBitRate, &iForward);
           if (m_frameBytes > 0 || m_frameBytes < 1050)
           {
              MPEG_HEAD sHead;
              int       iFrameBytes, iBitRate;
             
              iFrameBytes = head_info3(((unsigned char *)pBuffer) + 
                                       m_frameBytes - 1,
                                       iNumBytes - (m_frameBytes - 1), &sHead,
                                       &iBitRate, &iForward);
              m_input->EndRead(0);

              // Did the decoder find a bad sync marker?
              if (iFrameBytes == m_frameBytes && iBitRate == m_iBitRate)
              {
			         return kError_NoErr;
              }
           }
           else
              m_input->EndRead(0);

           AdvanceBufferToNextFrame();
       }
		 else
		 {
		     return Err;
       }
   }

   return (Error)lmcError_HeadInfoReturnedZero;
}

bool      XingLMC::
CanDecode()
{
   Error  Err;
   int32  dummy, iLoop;
   void  *pBuffer;
   bool   bRet = false;
   size_t iNumBytes;

   if (!m_input)
      return false;

   if (!m_input->IsStreaming())
       m_input->Seek(dummy, 0, SEEK_FROM_START);

   Err = GetHeadInfo();
   if (Err != kError_NoErr)
   {
       return false;
   }

   if (m_input->CachePMI())
       m_input->SetBufferSize((m_iBitRate * iStreamingBufferSize * 1000) / 8192);

   return true;
}

Error     XingLMC::
ExtractMediaInfo(MediaInfoEvent ** pMIE)
{
   size_t         iNumBytes;
   int32          totalFrames = 0;
   int32          bytesPerFrame;
   size_t         end;
   unsigned char *pID3Tag; 
   Error          Err;
   float          totalSeconds;

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

   *pMIE = new MediaInfoEvent(m_input->Url(), totalSeconds);

   if (!(*pMIE))
      return kError_OutOfMemory;

   if (tag_info.m_containsInfo)
   {

       ID3TagEvent *ite = new ID3TagEvent(tag_info);

       if (ite)
       {
           (*pMIE)->AddChildEvent((Event *) ite);
           ite = NULL;
       }
       else
       {
           return kError_OutOfMemory;
       }
   }

   delete pID3Tag;

   MpegInfoEvent *mie = new MpegInfoEvent(totalFrames,
                       milliseconds_per_frame / 1000, m_frameBytes,
                       m_iBitRate, samprate, layer,
                       (m_sMpegHead.sync == 2) ? 3 : (m_sMpegHead.id) ? 1 : 2,
                       (m_sMpegHead.mode == 0x3 ? 1 : 2), 
							  m_sMpegHead.original, m_sMpegHead.prot,
                       m_sMpegHead.emphasis, m_sMpegHead.mode, 
							  (float) m_sMpegHead.mode_ext);

   if (mie)
   {
       (*pMIE)->AddChildEvent((Event *) mie);
       mie = NULL;
   }
   else
   {
       return kError_OutOfMemory;
   }

   return kError_NoErr;
}

Error     XingLMC::
InitDecoder()
{
   void          *pBuffer;
   int32          bitrate;
   unsigned int   dummy;
   size_t         iNumBytes;
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

   if (!m_input->CachePMI())
       m_input->SetBufferSize((m_iBitRate * iStreamingBufferSize * 1000) / 8192);

   // select decoder
   m_audioMethods = audio_table[0][0];       // not integer, non 8 bit mode

   if (m_audioMethods.decode_init(&m_sMpegHead,
                                  m_frameBytes,
                                  0 /* reduction code */ ,
                                  0 /* transform code (ignored) */ ,
                                  0 /* convert code */ ,
                                  24000 /* freq limit */ ))
   {
         DEC_INFO  decinfo;

         m_audioMethods.decode_info(&decinfo);

#if __BYTE_ORDER != __LITTLE_ENDIAN
         cvt_to_wave_init(decinfo.bits);
#endif

         OutputInfo info;

         info.bits_per_sample = decinfo.bits;
         info.number_of_channels = decinfo.channels;
         info.samples_per_second = decinfo.samprate;
         info.max_buffer_size = (info.number_of_channels * 2 * 1152) << 2;
         Error     error = m_output->Init(&info);

         if (error != kError_NoErr)
         {
            return (Error) lmcError_OutputInitializeFailed;
         }

         m_pcmBuffer = new unsigned char[info.max_buffer_size];

         if (!m_pcmBuffer)
            return kError_OutOfMemory;
         m_pcmTrigger = info.max_buffer_size - 2500 * sizeof(short);
#define _VISUAL_ENABLE_
#ifdef  _VISUAL_ENABLE_
                m_pcmTrigger = info.max_buffer_size - 
					           (3250 * info.number_of_channels * sizeof(short)); 

#endif  //_VISUAL_ENABLE_
#undef  _VISUAL_ENABLE_

         m_pcmBufBytes = 0;
   }
   else
   {
      return (Error) lmcError_AudioDecodeInitFailed;
   }

   m_properlyInitialized = true;
   return (Error) kError_NoErr;
}

XingLMC::
XingLMC()
{
   m_input = NULL;
   m_output = NULL;
   m_properlyInitialized = false;
   m_target = NULL;
   m_decoderThread = NULL;
   m_xcqueue = new Queue < XingCommand * >(false);
   m_seekMutex = new Mutex();
   m_pauseSemaphore = new Semaphore();
	m_bBufferingUp = false;
	m_iBufferUpdate = 0;
	m_iBitRate = 0;
	m_frameBytes = -1;

   m_pcmBuffer = NULL;
   m_frameCounter = 0;
   m_frameWaitTill = 0;
   actually_decode = 0;
}

XingLMC::~XingLMC()
{
   Stop();
   if (m_xcqueue)
   {
      delete    m_xcqueue;

      m_xcqueue = NULL;
   }
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
   if (m_pcmBuffer)
   {
      delete    m_pcmBuffer;

      m_pcmBuffer = NULL;
   }

}

Error     XingLMC::
Stop()
{
   ENSURE_INITIALIZED;

   if (m_decoderThread)
   {
      XingCommand *xc = new XingCommand[1];

      if (!xc)
         return kError_OutOfMemory;

      xc[0] = XING_Stop;
      m_xcqueue->Write(xc);
      m_pauseSemaphore->Signal();
      m_decoderThread->Join();  // wait for thread to exit

      delete    m_decoderThread;

      m_decoderThread = NULL;
      m_output->Reset(true);
   }
   return kError_NoErr;
}

Error     XingLMC::
Decode()
{
   ENSURE_INITIALIZED;

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

void      XingLMC::
DecodeWorkerThreadFunc(void *pxlmc)
{
   if (pxlmc)
   {
      XingLMC  *xlmc = (XingLMC *) pxlmc;

      xlmc->DecodeWork();
   }
}

void      XingLMC::
DecodeWork()
{
   bool           result = true;
   int32          in_bytes, out_bytes;
   int32          nwrite;
   size_t         iBytesNeeded;
   void          *pBuffer;
   Error          Err;
   int            iLoop = 0;
   IN_OUT         x = {0, 0};

   in_bytes = out_bytes = 0;

	m_input->Resume();
   for (m_frameCounter = 0;;)
   {
      if (m_frameCounter >= m_frameWaitTill)
      {
         actually_decode = 1;
      }
      if (actually_decode)
      {
         double    tpf = (double) 1152 / (double) 44100;
         float     totalTime = (float) ((double) m_frameCounter * (double) tpf);
         int32     hours = (int32) (totalTime / 3600);
         int32     minutes = ((int32) totalTime - hours) / 60;
         int32     seconds = (int32) totalTime - hours * 3600 - minutes * 60;
         MediaTimeInfoEvent *pmtpi = new MediaTimeInfoEvent(hours, 
                       minutes, seconds, 0, totalTime, m_frameCounter);

         if (m_target)
         {
            m_target->AcceptEvent(pmtpi);
         }
      }

      while (!m_xcqueue->IsEmpty())
      {
         XingCommand *xc = m_xcqueue->Read();

         switch (*xc)
         {
         case XING_Stop:
            return;
         case XING_Pause:
            m_output->Pause();
            m_input->Pause();
            m_pauseSemaphore->Wait();
            m_input->Resume();
            break;
         default:
            break;
         }
      }

      m_seekMutex->Acquire(WAIT_FOREVER);

      for(iLoop = 0; iLoop < iMaxDecodeRetries; iLoop++)
		{
          iBytesNeeded = m_frameBytes;
          Err = BeginRead(pBuffer, iBytesNeeded);
          if (Err != kError_NoErr)
          {
              m_seekMutex->Release();
              if (m_target)
                 m_target->AcceptEvent(new LMCErrorEvent(this,Err));

              return;
          }
         
          x = m_audioMethods.decode((unsigned char *)pBuffer, 
                  (short *)(m_pcmBuffer + m_pcmBufBytes));
          if (x.in_bytes == 0)
			 {
             m_input->EndRead(x.in_bytes);

             m_audioMethods.decode_init(&m_sMpegHead, m_frameBytes,
				                           0, 0, 0, 24000);

			    Err = AdvanceBufferToNextFrame();
             if (Err != kError_NoErr)
             {
                 m_seekMutex->Release();
                 if (m_target)
                     m_target->AcceptEvent(new LMCErrorEvent(this,Err));

                 return;
             }
			 }
			 else
          {
			    break;
          }
      }
      m_input->EndRead(x.in_bytes);

      if (iLoop == iMaxDecodeRetries)
      {
         m_seekMutex->Release();
         if (m_target)
            m_target->AcceptEvent(new LMCErrorEvent(this, (Error) 
                        lmcError_DecodeDidntDecode));
         return;
      }

      m_pcmBufBytes += x.out_bytes;
      m_frameCounter++;

      int32     nwrite = 0;

      if (m_pcmBufBytes > m_pcmTrigger)
      {
#if __BYTE_ORDER != __LITTLE_ENDIAN
         m_pcmBufBytes = cvt_to_wave(m_pcmBuffer, m_pcmBufBytes);
#endif
         if (actually_decode)
         {
            Error     error = m_output->Write(nwrite, 
                                  m_pcmBuffer, m_pcmBufBytes);

            if (error != kError_NoErr)
            {
               m_seekMutex->Release();
               if (m_target)
                  m_target->AcceptEvent(new LMCErrorEvent(this, 
                           (Error) lmcError_OutputWriteFailed));
               return;
            }

#define _VISUAL_ENABLE_
#ifdef  _VISUAL_ENABLE_
                if (m_target)
                        m_target->AcceptEvent(new SendVisBufEvent(nwrite,m_pcmBuffer,m_pcmBufBytes));
#endif  //_VISUAL_ENABLE_
#undef  _VISUAL_ENABLE_

         }
         out_bytes += m_pcmBufBytes;
         m_pcmBufBytes = 0;
      }

      in_bytes += x.in_bytes;
      m_seekMutex->Release();
   }

   if (m_pcmBufBytes > 0)
   {                            // Write out last bit
#if __BYTE_ORDER != __LITTLE_ENDIAN
      m_pcmBufBytes = cvt_to_wave(m_pcmBuffer, m_pcmBufBytes);
#endif
      Error     error = m_output->Write(nwrite, m_pcmBuffer, m_pcmBufBytes);

      if (error != kError_NoErr)
      {
         if (m_target)
            m_target->AcceptEvent(new LMCErrorEvent(this, (Error) 
                         lmcError_OutputWriteFailed));
         return;
      }
#define _VISUAL_ENABLE_
#ifdef  _VISUAL_ENABLE_
                if (m_target)
                        m_target->AcceptEvent(new SendVisBufEvent(nwrite,m_pcmBuffer,m_pcmBufBytes));
#endif  //_VISUAL_ENABLE_
#undef  _VISUAL_ENABLE_
      out_bytes += m_pcmBufBytes;
      m_pcmBufBytes = 0;
   }

   m_output->Reset(false);

   if (m_target)
      m_target->AcceptEvent(new Event(INFO_DoneOutputting));

   return;
}

// This function encapsulates all the buffering event management
Error XingLMC::
BeginRead(void *&pBuffer, unsigned int iBytesNeeded)
{
   time_t iNow;
	Error  Err;
	bool   bBuffering = false;
	int32  iPercent;

   if (!m_input->IsStreaming())
	    return m_input->BeginRead(pBuffer, iBytesNeeded);

   for(;;)
	{
	    iPercent = m_input->GetBufferPercentage();

       time(&iNow);
	    if (iNow != m_iBufferUpdate)
	    {
	        //if (m_target)
		     //    m_target->AcceptEvent(new StreamBufferEvent(bBuffering, 
           //    iPercent));

	        printf("Buffer: %d%%  \r", m_input->GetBufferPercentage());
		     fflush(stdout);
           m_iBufferUpdate = iNow;
	    }

	    Err = m_input->BeginRead(pBuffer, iBytesNeeded);
	    if (Err == kError_BufferingUp)
	    {
		     if (!bBuffering)
			  {
			     printf("Buffering up.\n");
				  //if (m_target)
		        //    m_target->AcceptEvent(new StreamBufferEvent(true, iPercent));
           }

		     bBuffering = true;

		     sleep(1);
			  continue;
	    }
		 break;
	}

	if (bBuffering)
	{
	    //if (m_target)
		 //    m_target->AcceptEvent(new StreamBufferEvent(false, iPercent));

       printf("Done buffering up.\n");
	}

	return Err;
}

Error     XingLMC::
Pause()
{
   ENSURE_INITIALIZED;
   XingCommand *xc = new XingCommand[1];

   xc[0] = XING_Pause;
   m_xcqueue->Write(xc);
   return kError_NoErr;
}

Error     XingLMC::
Resume()
{
   ENSURE_INITIALIZED;
   m_pauseSemaphore->Signal();
   m_output->Resume();
   return kError_NoErr;
}

Error     XingLMC::
Reset()
{
   ENSURE_INITIALIZED;
   return kError_NoErr;
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

   m_seekMutex->Acquire(WAIT_FOREVER);

   int32     dummy;

   error = m_input->Seek(dummy, position * m_frameBytes, SEEK_FROM_START);
	m_frameCounter = position;

   m_seekMutex->Release();

   return error;
}

