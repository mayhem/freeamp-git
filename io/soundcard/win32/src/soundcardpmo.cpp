/*____________________________________________________________________________
   
   FreeAmp - The Free MP3 Player

   Portions Copyright (C) 1998 GoodNoise
   Portions Copyright (C) 1997 Jeff Tsay (ctsay@pasteur.eecs.berkeley.edu)

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
#include <windows.h>
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "SoundCardPMO.h"
#include "eventdata.h"
#include "log.h"

LogFile  *g_Log;

const int iBufferSize = 256 * 1024;
const int iOverflowSize = 24 * 1024;
const int iWriteTriggerSize = 18432;
const int iWriteToCard = 8 * 1024;

HANDLE    MCISemaphore;

extern    "C"
{
   PhysicalMediaOutput *Initialize(LogFile * pLog)
   {
      g_Log = pLog;
      return new SoundCardPMO();
   }
}

static void CALLBACK
MCICallBack(HWAVEOUT hwo, UINT msg, DWORD dwInstance,
            DWORD dwParam1, DWORD dwParam2)
{
   switch (msg)
   {
   case WOM_DONE:
      {
         LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwParam1;

         waveOutUnprepareHeader(hwo, lpWaveHdr, sizeof(WAVEHDR));
         lpWaveHdr->dwUser = 0;
         ReleaseSemaphore(MCISemaphore, 1, NULL);
         break;
      }
   }
}

SoundCardPMO::SoundCardPMO() :
              EventBuffer(iBufferSize, iOverflowSize, iWriteTriggerSize)
{
   m_wfex = NULL;
   m_wavehdr_array = NULL;
   m_hwo = NULL;

   m_index = 0;
   m_channels = 0;
   m_samples_per_second = 0;
   m_buffer_count = 0;
   m_hdr_size = 0;
   m_fillup = 0;
   m_data_size = 0;
   m_user_stop = false;
   m_initialized = false;
   m_pBufferThread = NULL;
   m_bExit = false;
   m_bPause = false;

   m_pPauseSem = new Semaphore(); 

   if (!m_pBufferThread)
   {
      m_pBufferThread = Thread::CreateThread();
      assert(m_pBufferThread);
      m_pBufferThread->Create(SoundCardPMO::StartWorkerThread, this);
   }
}

SoundCardPMO::~SoundCardPMO()
{
   if (m_initialized)
   {
      char     *buf = new char[m_data_size];
      int32     wrote;

      memset(buf, 0x00, m_data_size);

      Write(wrote, buf, m_data_size);

      delete[]buf;

      while (waveOutClose(m_hwo) == WAVERR_STILLPLAYING)
      {
         Sleep(SLEEPTIME);
      }

      CloseHandle(MCISemaphore);

      // Unprepare and free the header memory.
      for (uint32 j = 0; j < m_num_headers; j++)
      {
         delete[]m_wavehdr_array[j]->lpData;
         delete    m_wavehdr_array[j];
      }

      delete[]m_wavehdr_array;
      delete    m_wfex;
   }

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
}

Error SoundCardPMO::Init(OutputInfo * info)
{
   Error     result = kError_UnknownErr;
   MMRESULT  mmresult = 0;

   m_channels = info->number_of_channels;
   m_samples_per_second = info->samples_per_second;
   m_data_size = info->max_buffer_size;

   m_num_headers = 4;
   m_hdr_size = sizeof(WAVEHDR);
   m_wavehdr_array = new LPWAVEHDR[m_num_headers];

   m_fillup = 0;
   m_user_stop = false;

   m_wfex = new WAVEFORMATEX;

   m_wfex->wBitsPerSample = info->bits_per_sample;
   m_wfex->wFormatTag = WAVE_FORMAT_PCM;
   m_wfex->nChannels = (WORD) m_channels;
   m_wfex->nSamplesPerSec = info->samples_per_second;
   m_wfex->nAvgBytesPerSec = m_channels * info->samples_per_second << 1;
   m_wfex->nBlockAlign = (WORD) (m_channels << 1);
   m_wfex->cbSize = 0;

   mmresult = waveOutOpen(&m_hwo,
                          WAVE_MAPPER,
                          m_wfex,
                          (DWORD) MCICallBack,
                          NULL,
                          WAVE_ALLOWSYNC | CALLBACK_FUNCTION);

   if (mmresult == MMSYSERR_NOERROR)
   {
      result = kError_NoErr;
   }

   uint32    i;

   for (i = 0; i < m_num_headers; i++)
   {
      m_wavehdr_array[i] = new WAVEHDR;

      LPWAVEHDR temp = m_wavehdr_array[i];

      if (!temp)
      {
         result = kError_OutOfMemory;
         // cerr << "error allocating WAVEHDR" << endl;
         break;
      }

      temp->lpData = new char[m_data_size];

      if (!temp->lpData)
      {
         result = kError_OutOfMemory;
         break;
      }

      temp->dwBufferLength = m_data_size;
      temp->dwBytesRecorded = 0;
      temp->dwUser = 0;         // If played, dwUser = 1

      temp->dwLoops = 0;
      temp->dwFlags = NULL;

      // cerr << i << ": dwUser = " << temp->dwUser << endl;
   }

   for (i = 0; i < m_channels; i++)
   {
      m_buffer[i] = i * m_channels;
   }

   MCISemaphore = CreateSemaphore(NULL, m_num_headers, m_num_headers, "MCISemaphore");

   m_initialized = true;

   return result;
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

int SoundCardPMO::GetBufferPercentage()
{
   return PullBuffer::GetBufferPercentage();
}

Error SoundCardPMO::Pause()
{
   //Debug_v("Got pause message");
   m_bPause = true;

   return kError_NoErr;
}

Error SoundCardPMO::Resume()
{
   m_pPauseSem->Signal();

   return kError_NoErr;
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

Error SoundCardPMO::Reset(bool user_stop)
{
   Error     result = kError_NoErr;

   if (user_stop)
   {
      m_user_stop = user_stop;

      waveOutReset(m_hwo);
   }

   return result;
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
#if 0
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

   if (myInfo->samples_per_second <= 0)
      return;

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
#endif
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
   int         iRet;
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
          ReportError("Internal error occured.");
          g_Log->Error("Cannot read from buffer in PMO worker tread: %d\n",
              eErr);
          break;
      }

	  //Debug_v("write % bytes\n", iToCopy);
      Write(iRet, pBuffer, iToCopy);

//      if (m_bPause)
//      {
//         Reset(true);
//         EndRead(iCopied);
//         m_pPauseSem->Wait();
//         m_bPause = false;
//         continue;
//      }

#if 0
      if (iCopied < iToCopy)
      {
         EndRead(0);
         ReportError("Could not write sound data to the soundcard.");
         g_Log->Error("Failed to write to the soundcard: %s\n", strerror(errno))
;
         break;
      }
#endif
      EndRead(iToCopy);
   }
   //Debug_v("Thread exit");
}    

Error SoundCardPMO::Write(int32 & wrote, void *pBuffer, int32 length)
{
   Error    result = kError_NoErr;
   WAVEHDR *wavhdr = NULL;

   WaitForSingleObject(MCISemaphore, 10000);

   wavhdr = NextHeader();

   if (length > (int32) m_data_size)
   {
      length = m_data_size;
      result = kError_UnknownErr;
   }

   wavhdr->dwBufferLength = length;
   memcpy(wavhdr->lpData, pBuffer, length);

   // Prepare & write newest header
   waveOutPrepareHeader(m_hwo, wavhdr, m_hdr_size);
//   m_pauseMutex->Acquire(WAIT_FOREVER);
   waveOutWrite(m_hwo, wavhdr, m_hdr_size);
//   m_pauseMutex->Release();
   wrote = length;

   return result;
}

WAVEHDR *SoundCardPMO::NextHeader()
{
   WAVEHDR  *result = NULL;

   do
   {
      if (m_index >= m_num_headers)
         m_index = 0;

      if (!m_wavehdr_array[m_index]->dwUser)
      {
         result = m_wavehdr_array[m_index++];
         result->dwUser = m_index;
         break;
      }

   }
   while (m_wavehdr_array[m_index++]->dwUser);

   return result;
}

