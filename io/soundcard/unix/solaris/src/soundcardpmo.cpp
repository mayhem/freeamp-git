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
#include <sys/audioio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

/* project headers */
#include <config.h>
#include "soundcardpmo.h"
#include "eventdata.h"
#include "facontext.h"
#include "log.h"

#define PIECES 50
#define DB printf("%s:%d\n", __FILE__, __LINE__);

const int iInitialBufferSize = 64 * 1024;
const int iBufferSize = 512 * 1024;
const int iOverflowSize = 24 * 1024;
const int iWriteTriggerSize = 8 * 1024;
const int iWriteToCard = 8 * 1024;

extern    "C"
{
   PhysicalMediaOutput *Initialize(FAContext *context)
   {
      return new SoundCardPMO(context);
   }
}

SoundCardPMO::SoundCardPMO(context) :
              EventBuffer(iInitialBufferSize, iOverflowSize,
			  iWriteTriggerSize, context)
{
   //printf("PMO ctor\n");
   m_properlyInitialized = false;

   myInfo = new OutputInfo();
   memset(myInfo, 0, sizeof(OutputInfo));

   m_pBufferThread = NULL;

   m_pPauseMutex = new Mutex();
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
   //printf("PMO dtor\n");
   m_bExit = true;
   m_pWriteSem->Signal();
   m_pReadSem->Signal();
   m_pPauseMutex->Release();

   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
   }

   if (m_pPauseMutex)
   {
      delete m_pPauseMutex;
      m_pPauseMutex = NULL;
   }

   close(audio_fd);
   audio_fd = -1;

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
       if (iNewSize > iInitialBufferSize)
           Resize(iNewSize, iOverflowSize, iWriteTriggerSize);
   }
   else
      Resize(iBufferSize, iOverflowSize, iWriteTriggerSize);

   return kError_NoErr;
}

int SoundCardPMO::GetVolume()
{
   int mixFd = open("/dev/audioctl",O_RDWR);
   struct audio_prinfo audinf;

   if (mixFd != -1) 
   {
      ioctl(mixFd, AUDIO_GETINFO, &audinf);
      close(mixFd);
   }
   return audinf.gain;   
}

void SoundCardPMO::SetVolume(int32 iVolume)
{
   int mixFd;
   struct audio_prinfo audinf;

   mixFd = open("/dev/audioctl",O_RDWR);
   if (mixFd >= 0) 
   {
      ioctl(mixFd, AUDIO_GETINFO, &audinf);
      /*iVolume |= (iVolume << 8);*/
      audinf.gain=iVolume;
      ioctl(mixFd, AUDIO_SETINFO, &audinf);
      close(mixFd);
   }    
}

int SoundCardPMO::GetBufferPercentage()
{
   return PullBuffer::GetBufferPercentage();
}

int SoundCardPMO::audio_fd = -1;

Error SoundCardPMO::Pause()
{
   //printf("Got pause\n");
   m_pPauseMutex->Acquire();
   //printf("Got pause mutex\n");

   if (m_properlyInitialized)
       Reset(true);
}

Error SoundCardPMO::Resume()
{
   m_pPauseMutex->Release();

   return kError_NoErr;
}

Error SoundCardPMO::Break()
{
   m_bExit = true;
   PullBuffer::BreakBlocks();

   return kError_NoErr;
}

Error SoundCardPMO::Init(OutputInfo * info)
{
   struct audio_prinfo audinf;
   struct audio_prinfo sInfo;
   m_properlyInitialized = false;
   if (!info)
   {
      info = myInfo;
   }
   else
   {
      // got info, so this is the beginning...
      if ((audio_fd = open("/dev/audio", O_WRONLY)) < 0)
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
   }

   int       ctlfd = -1;

   if ((ctlfd=open("/dev/audioctl",O_RDWR)) <0)
   {
	ReportError("Cannot open audiocontrol device. Please make sure "
		"that the audio device is properly configured.");
	return (Error) pmoError_DeviceOpenFailed;
   } 
   if (ioctl(ctlfd, AUDIO_GETINFO, &audinf) < 0)
   {
      ReportError("Cannot get the flags on the audio device.");
      return (Error) pmoError_IOCTL_AUDIO_GETINFO;
   }

   audinf.buffer_size = 0;           // SYNC;

   if (ioctl(ctlfd, AUDIO_SETINFO, &audinf) < 0)
   {
      ReportError("Cannot set the no delay flag on the audio device.");
      return (Error) pmoError_IOCTL_AUDIO_SETINFO;
   }

   channels = info->number_of_channels;

   for (int i = 0; i < info->number_of_channels; ++i)
      bufferp[i] = buffer + i;

   // configure the device:
   int       play_precision = 16;
//   int       play_stereo = channels - 1;
   int       play_sample_rate = info->samples_per_second;

   int       junkvar = 0;

   if (ioctl(audio_fd, AUDIO_DRAIN, &audinf) == -1)
   {
      ReportError("Cannot reset the soundcard.");
      return (Error) pmoError_IOCTL_AUDIO_DRAIN;
   }

   audinf.precision = play_precision;
   audinf.channels = channels;
   audinf.sample_rate = play_sample_rate;

   if (ioctl(ctlfd, AUDIO_SETINFO, &audinf) == -1)
   {
      ReportError("Cannot set the soundcard's options.");
      return (Error) pmoError_IOCTL_AUDIO_SETINFO;
   }

   myInfo->bits_per_sample = info->bits_per_sample;
   myInfo->number_of_channels = info->number_of_channels;
   myInfo->samples_per_second = info->samples_per_second;
   myInfo->max_buffer_size = info->max_buffer_size;
   m_properlyInitialized = true;

   ioctl(audio_fd, AUDIO_GETINFO, &sInfo);
   m_iOutputBufferSize = sInfo.buffer_size;
   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

   close(ctlfd);

   return kError_NoErr;
}

Error SoundCardPMO::Reset(bool user_stop)
{
   int a;

   if (audio_fd <= 0)
      return kError_NoErr;

   if (user_stop)
   {
      if (ioctl(audio_fd, AUDIO_DRAIN, &a) == -1)
      {
         ReportError("Cannot reset the soundcard.");
         return (Error)pmoError_IOCTL_AUDIO_DRAIN;
      }
      Init(NULL);
   }
   else
   {
      if (ioctl(audio_fd, AUDIO_DRAIN, &a) == -1)
      {
         ReportError("Cannot reset the soundcard.");
         return (Error)pmoError_IOCTL_AUDIO_DRAIN;
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
   audio_prinfo      info;

   if (pEvent->GetFrameNumber() != m_iLastFrame + 1)
   {
       m_iTotalBytesWritten = 1152 * pEvent->GetFrameNumber() * 
                              m_iBytesPerSample; 
   }
   m_iLastFrame = pEvent->GetFrameNumber();

   if (myInfo->samples_per_second <= 0 || pEvent->GetFrameNumber() < 3)
      return;

   ioctl(audio_fd, AUDIO_GETINFO, &info);

   iTotalTime = (m_iTotalBytesWritten - info.buffer_size) /
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
   audio_prinfo info;

   for(; !m_bExit;)
   {
      iToCopy = iWriteToCard;

      //printf("before pause mutex acquire\n");
      m_pPauseMutex->Acquire();
      //printf("after pause mutex acquire\n");

      eErr = BeginRead(pBuffer, iToCopy);
      //printf("after begin read\n");
      if (eErr == kError_InputUnsuccessful || eErr == kError_NoDataAvail)
      {
          m_pPauseMutex->Release();
          m_pReadSem->Wait();
          continue;
      }
      if (eErr == kError_Interrupt)
      {
          m_pPauseMutex->Release();
          break;
      }
          
      if (eErr == kError_EventPending)
      {
          m_pPauseMutex->Release();

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
          m_pPauseMutex->Release();

          ReportError("Internal error occured.");
          m_context->log->Error("Cannot read from buffer in PMO worker thread: %d\n",
              eErr);
          break;
      }

      iCopied = 0;
      do
      {
          ioctl(audio_fd, AUDIO_GETINFO, &info);
          if (info.buffer_size < iToCopy)
          {
              EndRead(0);
              m_pPauseMutex->Release();

              usleep(10000);

              for(;!m_bExit;)
                 if (m_pPauseMutex->Acquire(10000))
                     break;
              if (m_bExit)
                 iToCopy = 0;

              // If beginread returns an error, break out...
              if (BeginRead(pBuffer, iToCopy) != kError_NoErr)
                 iToCopy = 0;

              continue;
          }
          iRet = write(audio_fd, pBuffer, iToCopy);
          if (iRet > 0)
          {
              pBuffer = ((char *)pBuffer + iRet);
              iCopied += iRet;
              m_iTotalBytesWritten += iRet;
          }
      }
      while (iCopied < iToCopy && (errno == EINTR || errno == 0));

      // Was iToCopy set to zero? If so, we got cleared and should
      // start from the top
      if (iToCopy == 0)
      {
         m_pPauseMutex->Release();
         continue;
      }

      if (iCopied < iToCopy)
      {
         EndRead(0);
         m_pPauseMutex->Release();
         ReportError("Could not write sound data to the soundcard.");
         m_context->log->Error("Failed to write to the soundcard: %s\n", strerror(errno))
;
         break;
      }
      EndRead(iCopied);

      m_pPauseMutex->Release();
   }
   //printf("output buffer thread exit\n");
}

