/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998 GoodNoise

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/

/* system headers */
#include <stdlib.h>
#include <iostream.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/* project headers */
#include <config.h>
#include "soundcardpmo.h"
#include "lmc.h"
#include "eventdata.h"
#include "facontext.h"
#include "log.h"

#define DB printf("%s:%d\n", __FILE__, __LINE__);

extern    "C"
{
   PhysicalMediaOutput *Initialize(FAContext *context)
   {
      return new SoundCardPMO(context);
   }
}

SoundCardPMO::SoundCardPMO(FAContext *context) :
       PhysicalMediaOutput(context)
{
   printf("SoundPMO: ctor\n");
   m_properlyInitialized = false;

   myInfo = new OutputInfo();
   memset(myInfo, 0, sizeof(OutputInfo));

   m_pBufferThread = NULL;

   m_iOutputBufferSize = 0;
   m_iTotalBytesWritten = 0;
   m_iBytesPerSample = 0;
   m_iLastFrame = -1;
   m_iDataSize = 0;
   audio_fd = -1;

   if (!m_pBufferThread)
   {
      m_pBufferThread = Thread::CreateThread();
      assert(m_pBufferThread);
      m_pBufferThread->Create(SoundCardPMO::StartWorkerThread, this);
   }
}

SoundCardPMO::~SoundCardPMO()
{
   printf("SoundPMO: dtor\n");
   m_bExit = true;
   m_pSleepSem->Signal();
   m_pPauseSem->Signal();

   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
   }

   close(audio_fd);

   if (myInfo)
   {
      delete    myInfo;

      myInfo = NULL;
   }
}

VolumeManager *SoundCardPMO::GetVolumeManager()
{
   return new OSSVolumeManager();
}

int SoundCardPMO::audio_fd = -1;

Error SoundCardPMO::Init(OutputInfo * info)
{
   int        iNewSize;
   Error      result;

   m_properlyInitialized = false;

   if (!info)
   {
      info = myInfo;
   }
   else
   {
      // got info, so this is the beginning...
      if ((audio_fd = open("/dev/dsp", O_WRONLY | O_SYNC, 0)) < 0)
      {
         if (errno == EBUSY)
         {
            ReportError("Audio device is busy. Please make sure that "
                        "another program is not using the device.");
            return (Error) pmoError_DeviceOpenFailed;
         }
         else
         {
            ReportError("Cannot open audio device. Please make sure that "
                        "the audio device is properly configured.");
            return (Error) pmoError_DeviceOpenFailed;
         }
      }

      m_iDataSize = info->max_buffer_size;

      m_pContext->prefs->GetOutputBufferSize(&iNewSize);
      iNewSize *= 1024;

      iNewSize -= iNewSize % m_iDataSize;
      result = m_pOutputBuffer->Resize(iNewSize, 0, m_iDataSize);
      if (IsError(result))
      {
         ReportError("Internal buffer sizing error occurred.");
         m_pContext->log->Error("Resize output buffer failed.");
         return result;
      }
   }

   int       fd = audio_fd;
   int       flags;

   if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
   {
      ReportError("Cannot get the flags on the audio device.");
      return (Error) pmoError_IOCTL_F_GETFL;
   }

   flags &= O_NDELAY;           // SYNC;

   if (fcntl(fd, F_SETFL, flags) < 0)
   {
      ReportError("Cannot set the no delay flag on the audio device.");
      return (Error) pmoError_IOCTL_F_SETFL;
   }

   audio_fd = fd;

   channels = info->number_of_channels;

   for (unsigned int i = 0; i < info->number_of_channels; ++i)
      bufferp[i] = buffer + i;

   // configure the device:
   int       play_precision = 16;
   int       play_stereo = channels - 1;
   int       play_sample_rate = info->samples_per_second;

   int       junkvar = 0;

   if (ioctl(audio_fd, SNDCTL_DSP_RESET, &junkvar) == -1)
   {
      ReportError("Cannot reset the soundcard.");
      return (Error) pmoError_IOCTL_SNDCTL_DSP_RESET;
   }

   if (ioctl(audio_fd, SNDCTL_DSP_SAMPLESIZE, &play_precision) == -1)
   {
      ReportError("Cannot set the soundcard's sample size.");
      return (Error) pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE;
   }

   if (ioctl(audio_fd, SNDCTL_DSP_STEREO, &play_stereo) == -1)
   {
      ReportError("Cannot set the soundcard to stereo.");
      return (Error) pmoError_IOCTL_SNDCTL_DSP_STEREO;
   }
   if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &play_sample_rate) == -1)
   {
      ReportError("Cannot set the soundcard's sampling speed.");
      return (Error) pmoError_IOCTL_SNDCTL_DSP_SPEED;
   }
   myInfo->bits_per_sample = info->bits_per_sample;
   myInfo->number_of_channels = info->number_of_channels;
   myInfo->samples_per_second = info->samples_per_second;
   myInfo->max_buffer_size = info->max_buffer_size;
   m_properlyInitialized = true;

   audio_buf_info sInfo;
   ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &sInfo);
   m_iOutputBufferSize = sInfo.fragsize * sInfo.fragstotal;
   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

   return kError_NoErr;
}

Error SoundCardPMO::Reset(bool user_stop)
{
   int a;

   if (audio_fd <= 0)
      return kError_NoErr;

   if (user_stop)
   {
      if (ioctl(audio_fd, SNDCTL_DSP_RESET, &a) == -1)
      {
         ReportError("Cannot reset the soundcard.");
         return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
      }
      Init(NULL);
   }
   else
   {
      if (ioctl(audio_fd, SNDCTL_DSP_SYNC, &a) == -1)
      {
         ReportError("Cannot reset the soundcard.");
         return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
      }
   }
   return kError_NoErr;
}

void SoundCardPMO::HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent)
{
   MediaTimeInfoEvent *pmtpi;
   int32               hours, minutes, seconds;
   int                 iTotalTime = 0;
   audio_buf_info      info;

   if (pEvent->GetFrameNumber() != m_iLastFrame + 1)
   {
       m_iTotalBytesWritten = 1152 * pEvent->GetFrameNumber() * 
                              m_iBytesPerSample; 
   }
   m_iLastFrame = pEvent->GetFrameNumber();

   if (myInfo->samples_per_second <= 0 || pEvent->GetFrameNumber() < 3)
      return;

   ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info);

   iTotalTime = (m_iTotalBytesWritten - (info.fragments * info.fragsize)) /
                (m_iBytesPerSample * myInfo->samples_per_second);

   hours = iTotalTime / 3600;
   minutes = (iTotalTime / 60) % 60;
   seconds = iTotalTime % 60;

   if (hours < 0 || hours > 23 ||
       minutes < 0 || minutes > 59 || 
       seconds < 0 || seconds > 59)
      return;

   pmtpi = new MediaTimeInfoEvent(hours, minutes, seconds, 0, 
                                  iTotalTime, 0);
   m_pTarget->AcceptEvent(pmtpi);
}

void SoundCardPMO::StartWorkerThread(void *pVoidBuffer)
{
   ((SoundCardPMO*)pVoidBuffer)->WorkerThread();
}

void SoundCardPMO::WorkerThread(void)
{
   void       *pBuffer;
   size_t      iToCopy;
   Error       eErr;
   size_t      iRet;
   Event      *pEvent;
   audio_buf_info info;
   bool        bPerfWarn = false;

   // Don't do anything until resume is called.
   m_pPauseSem->Wait();

   // The following should be abstracted out into the general thread
   // classes:
#ifdef __linux__
   struct sched_param sParam;

   sParam.sched_priority = sched_get_priority_max(SCHED_OTHER);
   pthread_setschedparam(pthread_self(), SCHED_OTHER, &sParam);
#endif

   for(; !m_bExit;)
   {
      // Loop until we get an Init event from the LMC
      if (!m_properlyInitialized)
      {
          pEvent = ((EventBuffer *)m_pOutputBuffer)->GetEvent();

          if (pEvent == NULL)
          {
              m_pLmc->Wake();
              continue;
          }

          if (pEvent->Type() == PMO_Init)
          {
              if (IsError(Init(((PMOInitEvent *)pEvent)->GetInfo())))
              {
                  delete pEvent;
                  break;
              }
          }
          delete pEvent;

          continue;
      }

      // Set up reading a block from the buffer. If not enough bytes are
      // available, sleep for a little while and try again.
      for(;;)
      {
          iToCopy = m_iDataSize;
          eErr = m_pOutputBuffer->BeginRead(pBuffer, iToCopy);

          // Enough bytes available?
          if (eErr == kError_InputUnsuccessful || 
              eErr == kError_NoDataAvail ||
              iToCopy < m_iDataSize)
          {
              m_pLmc->Wake();

              if (!bPerfWarn)
              {
                  time_t t;
    
                  time(&t);
                  m_pContext->log->Log(LogPerf, "Output buffer underflow: %s", 
                             ctime(&t));
                  bPerfWarn = true;
              }
    
              m_pOutputBuffer->EndRead(0);
    
              if (WasteTime())
                 return;
    
              continue;
          }
          if (eErr == kError_Interrupt)
          {
              return;
          }
    
          // Is there an event pending that we need to take care of
          // before we play this block of samples?
          if (eErr == kError_EventPending)
          {
              pEvent = ((EventBuffer *)m_pOutputBuffer)->GetEvent();

              if (pEvent->Type() == PMO_Init)
                  Init(((PMOInitEvent *)pEvent)->GetInfo());
    
              if (pEvent->Type() == PMO_Reset)
                  Reset(false);
    
              if (pEvent->Type() == PMO_Info) 
                  HandleTimeInfoEvent((PMOTimeInfoEvent *)pEvent);
    
              if (pEvent->Type() == PMO_Quit) 
              {
                  Reset(false);
                  delete pEvent;
                  m_pTarget->AcceptEvent(new Event(INFO_DoneOutputting));
                  break;
              }
 
              delete pEvent;
    
              return;
          }
          
          if (IsError(eErr))
          {
              ReportError("Internal error occured.");
              m_pContext->log->Error("Cannot read from buffer in PMO "
                                    "worker tread: %d\n", eErr);
              break;
          }
          bPerfWarn = false;
      }

      // Now write the block to the audio device. If the block doesn't
      // all fit, pause and loop until the entire block has been played.
      // This loop could be written using non-blocking io...
      for(;;)
      {
          ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info);
          if ((unsigned)(info.fragments * info.fragsize) < iToCopy)      
          {
              if (WasteTime())
                 return;
          }
      }        
      iRet = write(audio_fd, pBuffer, iToCopy);
      if (iRet < 0)
      {
         m_pOutputBuffer->EndRead(0);
         ReportError("Could not write sound data to the soundcard.");
         m_pContext->log->Error("Failed to write to the soundcard: %s\n", 
                               strerror(errno));
         break;
      }

      m_iTotalBytesWritten += iRet;
      m_pOutputBuffer->EndRead(iRet);
      m_pLmc->Wake();
   }
}

