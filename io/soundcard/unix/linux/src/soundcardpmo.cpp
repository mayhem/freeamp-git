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
#include <assert.h>

/* project headers */
#include <config.h>
#include "soundcardpmo.h"
#include "eventdata.h"
#include "log.h"

LogFile  *g_Log;

#define PIECES 50
#define DB printf("%s:%d\n", __FILE__, __LINE__);

const int iBufferSize = 256 * 1024;
const int iOverflowSize = 24 * 1024;
const int iWriteTriggerSize = 18432;
const int iWriteToCard = 8 * 1024;

extern    "C"
{
   PhysicalMediaOutput *Initialize(LogFile * pLog)
   {
      g_Log = pLog;
      return new SoundCardPMO();
   }
}

static char *g_ErrorArray[8] =
{
   "Unknown Error Code",
   "Cannot open the sound device (/dev/dsp).",
   "fcntl F_GETFL on /dev/dsp failed",
   "fcntl F_SETFL on /dev/dsp failed"
   "ioctl reset failed",
   "config of samplesize failed",
   "config of stereo failed",
   "config of speed failed"
};

const char *SoundCardPMO::
GetErrorString(int32 error)
{
   if ((error <= pmoError_MinimumError) || (error >= pmoError_MaximumError))
   {
      return g_ErrorArray[0];
   }
   return g_ErrorArray[error - pmoError_MinimumError];
}

SoundCardPMO::SoundCardPMO() :
              EventBuffer(iBufferSize, iOverflowSize, iWriteTriggerSize)
{
   m_properlyInitialized = false;

   myInfo = new OutputInfo();
   m_bPause = false;
   m_pBufferThread = NULL;

   m_pPauseSem = new Semaphore();
   m_iOutputBufferSize = 0;
   m_iTotalBytesWritten = 0;
   m_iBytesPerSample = 0;
   m_iLastFrame = -1;

   if (!m_pBufferThread)
   {
      m_pBufferThread = Thread::CreateThread();
      assert(m_pBufferThread);
      m_pBufferThread->Create(SoundCardPMO::StartWorkerThread, this);
   }
}

SoundCardPMO::~SoundCardPMO()
{
   m_bExit = true;
   m_pWriteSem->Signal();
   m_pReadSem->Signal();
   m_pPauseSem->Signal();

   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
   }

   if (m_pPauseSem)
   {
      delete m_pPauseSem;
      m_pPauseSem = NULL;
   }

   close(audio_fd);

   if (myInfo)
   {
      delete    myInfo;

      myInfo = NULL;
   }
}

void SoundCardPMO::WaitToQuit()
{
   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
      m_pBufferThread = NULL;
   }
}

Error SoundCardPMO::SetPropManager(Properties * p)
{
   PropValue *pProp;
   int        iNewSize;

   m_propManager = p;
   m_propManager->GetProperty("OutputBuffer", &pProp);
   if (pProp)
   {
       iNewSize = atoi(((StringPropValue *)pProp)->GetString()) * 1024;
       if (iNewSize > iBufferSize)
           Resize(iNewSize, iOverflowSize, iWriteTriggerSize);
   }

   return kError_NoErr;
}

int SoundCardPMO::GetBufferPercentage()
{
   return PullBuffer::GetBufferPercentage();
}

int SoundCardPMO::audio_fd = -1;

Error SoundCardPMO::Pause()
{
   m_bPause = true;
}

Error SoundCardPMO::Resume()
{
   m_pPauseSem->Signal();

   return kError_NoErr;
}

Error SoundCardPMO::Init(OutputInfo * info)
{
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
            g_Log->Error("Audio device busy!\n");
            return (Error) pmoError_DeviceOpenFailed;
         }
         else
         {
            g_Log->Error("Cannot open audio device!\n");
            return (Error) pmoError_DeviceOpenFailed;
         }
      }
   }

   int       fd = audio_fd;
   int       flags;

   if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
   {
      return (Error) pmoError_IOCTL_F_GETFL;
   }

   flags &= O_NDELAY;           // SYNC;

   if (fcntl(fd, F_SETFL, flags) < 0)
   {
      return (Error) pmoError_IOCTL_F_SETFL;
   }

   audio_fd = fd;

   channels = info->number_of_channels;

   for (int i = 0; i < info->number_of_channels; ++i)
      bufferp[i] = buffer + i;

   // configure the device:
   int       play_precision = 16;
   int       play_stereo = channels - 1;
   int       play_sample_rate = info->samples_per_second;

   int       junkvar = 0;

   if (ioctl(audio_fd, SNDCTL_DSP_RESET, &junkvar) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_RESET;
   }

   if (ioctl(audio_fd, SNDCTL_DSP_SAMPLESIZE, &play_precision) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE;
   }

   if (ioctl(audio_fd, SNDCTL_DSP_STEREO, &play_stereo) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_STEREO;
   }
   if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &play_sample_rate) == -1)
   {
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

   if (user_stop)
   {
      if (ioctl(audio_fd, SNDCTL_DSP_RESET, &a) == -1)
      {
         return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
      }
      Init(NULL);
   }
   else
   {
      if (ioctl(audio_fd, SNDCTL_DSP_SYNC, &a) == -1)
      {
         return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
      }
   }
   return kError_NoErr;
}

Error SoundCardPMO::BeginWrite(void *&pBuffer, size_t &iBytesToWrite)
{
   return EventBuffer::BeginWrite(pBuffer, iBytesToWrite);
}

Error SoundCardPMO::EndWrite(size_t iNumBytesWritten)
{
   return EventBuffer::EndWrite(iNumBytesWritten);
}

Error SoundCardPMO::AcceptEvent(Event *pEvent)
{
   return EventBuffer::AcceptEvent(pEvent);
}


Error SoundCardPMO::Clear()
{
   return EventBuffer::Clear();
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

   ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info);

   iTotalTime = (m_iTotalBytesWritten - (info.fragments * info.fragsize)) /
                (m_iBytesPerSample * myInfo->samples_per_second);
   hours = iTotalTime / 3600;
   minutes = (iTotalTime - hours) / 60;
   seconds = iTotalTime - hours * 3600 - minutes * 60;

   if (minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
      return;

   pmtpi = new MediaTimeInfoEvent(hours, minutes, seconds, 0, 
                                  iTotalTime, 0);
   m_target->AcceptEvent(pmtpi);
}

void SoundCardPMO::StartWorkerThread(void *pVoidBuffer)
{
   ((SoundCardPMO*)pVoidBuffer)->WorkerThread();
}

void SoundCardPMO::WorkerThread(void)
{
   void       *pBuffer;
   size_t      iToCopy, iCopied;
   Error       eErr;
   size_t      iRet;
   Event      *pEvent;
   OutputInfo *pInfo;

   for(; !m_bExit;)
   {
      iToCopy = iWriteToCard;
      eErr = BeginRead(pBuffer, iToCopy);
      if (eErr == kError_InputUnsuccessful || eErr == kError_NoDataAvail)
      {
          m_pReadSem->Wait();
          continue;
      }
          
      if (eErr == kError_EventPending)
      {
          pEvent = GetEvent();

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
              break;
          }
 
          delete pEvent;

          continue;
      }
          
      if (IsError(eErr))
      {
          g_Log->Error("Cannot read from buffer in PMO worker tread: %d\n",
              eErr);
          break;
      }

      iCopied = 0;
      do
      {
          iRet = write(audio_fd, pBuffer, iToCopy);
          if (iRet > 0)
          {
              pBuffer = ((char *)pBuffer + iRet);
              iCopied += iRet;
              m_iTotalBytesWritten += iRet;
          }
      }
      while (!m_bPause && iCopied < iToCopy && (errno == EINTR || errno == 0));

      if (m_bPause)
      {
         Reset(true);
         EndRead(iCopied);
         m_pPauseSem->Wait();
         m_bPause = false;
         continue;
      }

      if (iCopied < iToCopy)
      {
         EndRead(0);
         g_Log->Error("Failed to write to the soundcard: %s\n", strerror(errno))
;
         break;
      }
      EndRead(iCopied);
   }
}

