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

#define DB //printf("%s:%d\n", __FILE__, __LINE__);

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

const char *XingLMC::
GetErrorString(int32 error)
{
   if ((error <= lmcError_MinimumError) || (error >= lmcError_MaximumError))
   {
      return g_ErrorArray[0];
   }
   return g_ErrorArray[error - lmcError_MinimumError];
}

bool      XingLMC::
CanDecode()
{
   Error  Err;
   int32  dummy;
   void  *pBuffer;
   bool   bRet = false;
   size_t iNumBytes;

   if (!m_input)
      return false;

   Err = m_input->Seek(dummy, 0, SEEK_FROM_START);
   m_bCannotSeek = (Err == kError_FileSeekNotSupported);

   iNumBytes = 1024;
   Err = m_input->BeginRead(pBuffer, iNumBytes);
   if (Err == kError_NoErr)
   {
       MPEG_HEAD      head;
       int32          bitrate;
       uint32         searchAhead;
       int32          framebytes;

       bRet = (head_info3((unsigned char *)pBuffer,iNumBytes, &head, 
                          &bitrate, &searchAhead) != 0);
   }
   m_input->EndRead(0);

   return bRet;
}

Error     XingLMC::
ExtractMediaInfo(MediaInfoEvent ** pMIE)
{
   MPEG_HEAD      head;
   int32          bitrate;
   uint32         searchAhead;
   int32          dummy;
   size_t         iNumBytes;
   void          *pBuffer;
   int32          framebytes;
   int32          totalFrames = 0;
   int32          bytesPerFrame;
   size_t         end;
   unsigned char *pID3Tag; 
   Error          Err;
   float          totalSeconds;

   if (!m_input)
      return kError_NullValueInvalid;
 
   iNumBytes = 1024;
   Err = m_input->BeginRead(pBuffer, iNumBytes);
   if (Err != kError_NoErr)
       return (Error) lmcError_ID3ReadFailed;

   framebytes = head_info3((unsigned char *)pBuffer, iNumBytes, 
                           &head, &bitrate, &searchAhead);
   m_input->EndRead(0);
   if (framebytes == 0)
   {
       return (Error) lmcError_ID3ReadFailed;
   }

   if (m_input->GetLength(end) == kError_FileSeekNotSupported)
      end = 0;

   pID3Tag = new unsigned char[ID3_TAG_SIZE];
   if (!m_input->GetID3v1Tag(pID3Tag))
   {
      memset(pID3Tag, 0, ID3_TAG_SIZE);
   }

   Id3TagInfo tag_info((char *)pID3Tag);


   int32     sampRateIndex = 4 * head.id + head.sr_index;
   int32     samprate = sample_rate_table[sampRateIndex];

   if ((head.sync & 1) == 0)
        samprate = samprate / 2;    // mpeg25

   double    milliseconds_per_frame = 0;
   static int32 l[4] = {25, 3, 2, 1};

   int32     layer = l[head.option];
   static double ms_p_f_table[3][3] =
   {
      {8.707483f, 8.0f, 12.0f},
      {26.12245f, 24.0f, 36.0f},
      {26.12245f, 24.0f, 36.0f}
   };

   milliseconds_per_frame = ms_p_f_table[layer - 1][head.sr_index];

   if (end > 0)
   {
       bytesPerFrame = framebytes;
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
                       bitrate, samprate, layer,
                       (head.sync == 2) ? 3 : (head.id) ? 1 : 2,
                       (head.mode == 0x3 ? 1 : 2), head.original, head.prot,
                       head.emphasis, head.mode, (float) head.mode_ext);

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
   MPEG_HEAD      head;
   int32          bitrate;
   unsigned int   dummy;
   size_t         iNumBytes;
   Error          Err;

   if (!m_target || !m_input || !m_output)
   {
      return kError_NullValueInvalid;
   }
   m_properlyInitialized = false;

   iNumBytes = 1024;
   Err = m_input->BeginRead(pBuffer, iNumBytes);
   if (Err != kError_NoErr)
       return (Error) lmcError_ID3ReadFailed;

   // parse MPEG header
   m_frameBytes = head_info3((unsigned char *)pBuffer, iNumBytes, &head, 
                             &bitrate, &dummy);
   m_input->EndRead(0);

   if (m_frameBytes == 0)
   {
      return (Error) lmcError_HeadInfoReturnedZero;
   }

   // select decoder
   m_audioMethods = audio_table[0][0];       // not integer, non 8 bit mode

   if (m_audioMethods.decode_init(&head,
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
                m_pcmTrigger = info.max_buffer_size - 6500 * sizeof(short); 
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
   int            iTossed = 0;

   in_bytes = out_bytes = 0;

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
            m_pauseSemaphore->Wait();
            break;
         default:
            break;
         }
      }

      m_seekMutex->Acquire(WAIT_FOREVER);

      iBytesNeeded = m_frameBytes;
      Err = m_input->BeginRead(pBuffer, iBytesNeeded);
      if (Err != kError_NoErr)
      {
          m_seekMutex->Release();
          break;
      }

      // If the data is not frame aligned, search to the next frame
      if (*((unsigned char *)pBuffer) != 0xFF || 
          (*(((unsigned char *)pBuffer)+1) & 0xE0) != 0xE0)
      {
          unsigned char *pPtr;
          int32          iCount;

          for(;;)
          {
              for(pPtr = (unsigned char *)pBuffer, iCount = 0; 
                  iCount < m_frameBytes &&
                  !(*pPtr == 0xFF && (*(pPtr+1) & 0xE0) == 0xE0); 
                  pPtr++, iCount++)
                      ; // <=== Empty body!
   
              iTossed += iCount;
              m_input->EndRead(iCount);
              m_input->BeginRead(pBuffer, m_frameBytes);

              if (iCount == m_frameBytes)
                 continue;
              else
                 break;
          }
      }

      IN_OUT x = {0, 0};

      assert(*((unsigned char *)pBuffer) == 0xFF && 
             (*(((unsigned char *)pBuffer)+1) & 0xE0) == 0xE0);
            
      x = m_audioMethods.decode((unsigned char *)pBuffer, 
              (short *)(m_pcmBuffer + m_pcmBufBytes));
      if (x.in_bytes <= 0)
      {
         m_seekMutex->Release();
         if (m_target)
            m_target->AcceptEvent(new LMCErrorEvent(this, (Error) 
                        lmcError_DecodeDidntDecode));
         return;
      }
      m_input->EndRead(x.in_bytes);

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

Error XingLMC::SetEQData(float *arrayEQ) {
    ENSURE_INITIALIZED;
    Error error = kError_NoErr;
        for(int i=0; i<32; i++)
                m_equalizer[i] = arrayEQ[i];
        return error;
}

Error XingLMC::SetEQData(bool enable) {
    ENSURE_INITIALIZED;
    Error error = kError_NoErr;
        m_enableEQ = enable;
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

   error = m_input->Seek(dummy, 0, SEEK_FROM_START);
   m_frameCounter = 0;
   m_frameWaitTill = position;
   actually_decode = 0;

   m_seekMutex->Release();

   return error;
}

