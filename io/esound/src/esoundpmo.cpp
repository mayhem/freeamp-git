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
#include <errno.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <esd.h>

/* project headers */
#include <config.h>
#include "esoundpmo.h"
#include "eventdata.h"
#include "facontext.h"
#include "log.h"

#define PIECES 50
#define DB //printf("%s:%d\n", __FILE__, __LINE__);

const int iDefaultBufferSize = 512 * 1024;
const int iOrigBufferSize = 64 * 1024;
const int iOverflowSize = 0;
const int iWriteTriggerSize = 8 * 1024;

extern    "C"
{
   PhysicalMediaOutput *Initialize(FAContext *context)
   {
      return new EsounDPMO(context);
   }
}

EsounDPMO::EsounDPMO(FAContext *context) :
              EventBuffer(iOrigBufferSize, iOverflowSize,
			  iWriteTriggerSize, context)
{
   m_properlyInitialized = false;

   myInfo = new OutputInfo();
   memset(myInfo, 0, sizeof(OutputInfo));

   m_pBufferThread = NULL;

   m_pPauseMutex = new Mutex();
   m_iOutputBufferSize = 0;
   m_iTotalBytesWritten = 0;
   m_iBytesPerSample = 0;
   m_iLastFrame = -1;
   m_iDataSize = 0;

   if (!m_pBufferThread)
   {
      m_pBufferThread = Thread::CreateThread();
      assert(m_pBufferThread);
      m_pBufferThread->Create(EsounDPMO::StartWorkerThread, this);
   }
}

EsounDPMO::~EsounDPMO()
{
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

   esd_close(audio_fd);
   audio_fd = -1;

   if (myInfo)
   {
      delete    myInfo;

      myInfo = NULL;
   }
}

VolumeManager *EsounDPMO::GetVolumeManager()
{
   return new ESDVolumeManager();
}

void EsounDPMO::WaitToQuit()
{
   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
      m_pBufferThread = NULL;
   }
}

Error EsounDPMO::SetPropManager(Properties * p)
{
   m_propManager = p;
   return kError_NoErr;
}

int EsounDPMO::GetBufferPercentage()
{
   return PullBuffer::GetBufferPercentage();
}

int EsounDPMO::audio_fd = -1;

Error EsounDPMO::Pause()
{
   //printf("Got pause\n");
   m_pPauseMutex->Acquire();
   //printf("Got pause mutex\n");

   if (m_properlyInitialized)
       Reset(true);
}

Error EsounDPMO::Resume()
{
   m_pPauseMutex->Release();

   return kError_NoErr;
}

Error EsounDPMO::Break()
{
   m_bExit = true;
   PullBuffer::BreakBlocks();

   return kError_NoErr;
}

Error EsounDPMO::Init(OutputInfo * info)
{
   PropValue *pProp;
   int        iNewSize = iDefaultBufferSize;
   Error      result;

   m_properlyInitialized = false;

   if (!info)
   {
      info = myInfo;
   }
   else
   {
      // got info, so this is the beginning...

      m_iDataSize = info->max_buffer_size;

      m_context->prefs->GetOutputBufferSize(&iNewSize);
      iNewSize *= 1024;

      iNewSize -= iNewSize % m_iDataSize;
      result = Resize(iNewSize, 0, m_iDataSize);
      if (IsError(result))
      {
         ReportError("Internal buffer sizing error occurred.");
         m_context->log->Error("Resize output buffer failed.");
         return result;
      }
   }

   int       fd = audio_fd;
   int       flags;

   channels = info->number_of_channels;

   for (int i = 0; i < info->number_of_channels; ++i)
      bufferp[i] = buffer + i;

   // configure the device:
   int       play_sample_rate = info->samples_per_second;

   int       esd_format = 0;
   esd_format |= ESD_BITS16;
   if (info->number_of_channels == 2)
      esd_format |= ESD_STEREO;
   else if (info->number_of_channels == 1)
      esd_format |= ESD_MONO;
   else
   {
      ReportError("Unsupported number of channels.");
      return (Error) pmoError_DeviceOpenFailed;
   }

   audio_fd = esd_play_stream( esd_format, info->samples_per_second,
                               NULL, "FreeAMP" );
   if (audio_fd < 0)
   {
      ReportError("Unable to connect to EsounD server.");
      return (Error) pmoError_DeviceOpenFailed;
   }

   myInfo->bits_per_sample = info->bits_per_sample;
   myInfo->number_of_channels = info->number_of_channels;
   myInfo->samples_per_second = info->samples_per_second;
   myInfo->max_buffer_size = info->max_buffer_size;
   m_properlyInitialized = true;

   m_iOutputBufferSize = iDefaultBufferSize; // arbitrary
   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

   return kError_NoErr;
}

Error EsounDPMO::Reset(bool user_stop)
{
   return kError_NoErr;
}

Error EsounDPMO::BeginWrite(void *&pBuffer, size_t &iBytesToWrite)
{
   return EventBuffer::BeginWrite(pBuffer, iBytesToWrite);
}

Error EsounDPMO::EndWrite(size_t iNumBytesWritten)
{
   return EventBuffer::EndWrite(iNumBytesWritten);
}

Error EsounDPMO::AcceptEvent(Event *pEvent)
{
   return EventBuffer::AcceptEvent(pEvent);
}

Error EsounDPMO::Clear()
{
   return EventBuffer::Clear();
}

void EsounDPMO::HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent)
{
   MediaTimeInfoEvent *pmtpi;
   int32               hours, minutes, seconds;
   int                 iTotalTime = 0;

   if (pEvent->GetFrameNumber() != m_iLastFrame + 1)
   {
       m_iTotalBytesWritten = 1152 * pEvent->GetFrameNumber() * 
                              m_iBytesPerSample; 
   }
   m_iLastFrame = pEvent->GetFrameNumber();

   if (myInfo->samples_per_second <= 0 || pEvent->GetFrameNumber() < 3)
      return;

    // should take into account what is in the esd buffers
   iTotalTime = m_iTotalBytesWritten / (m_iBytesPerSample * myInfo->samples_per_second);

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

void EsounDPMO::StartWorkerThread(void *pVoidBuffer)
{
   ((EsounDPMO*)pVoidBuffer)->WorkerThread();
}

void EsounDPMO::WorkerThread(void)
{
   void       *pBuffer;
   size_t      iToCopy, iCopied;
   Error       eErr;
   size_t      iRet;
   Event      *pEvent;
   OutputInfo *pInfo;
   bool           bPerfWarn = false;

   // The following should be abstracted out into the general thread
   // classes:
#ifdef __linux__
   struct sched_param sParam;

   sParam.sched_priority = sched_get_priority_max(SCHED_OTHER);
   pthread_setschedparam(pthread_self(), SCHED_OTHER, &sParam);
#endif

   for(; !m_bExit;)
   {
      if (!m_properlyInitialized)
      {
          pEvent = GetEvent();

          if (pEvent == NULL)
          {
              m_pReadSem->Wait();
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
 
      iToCopy = m_iDataSize;

      m_pPauseMutex->Acquire();

      eErr = BeginRead(pBuffer, iToCopy);
      if (eErr == kError_InputUnsuccessful || eErr == kError_NoDataAvail ||
          iToCopy < m_iDataSize)
      {
          if (!bPerfWarn)
          {
              time_t t;

              time(&t);
              m_context->log->Log(LogPerf, "Output buffer underflow: %s", 
                         ctime(&t));
              bPerfWarn = true;
          }

          EndRead(0);
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
          m_context->log->Error("Cannot read from buffer in PMO worker tread: %d\n",
              eErr);
          break;
      }
      bPerfWarn = false;

      iCopied = 0;
      do
      {
/*          ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info);
          if (info.fragments * info.fragsize < iToCopy)
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
*/          
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
}

