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

#define DB Debug_v("%s:%d", __FILE__, __LINE__);

const int iDefaultBufferSize = 512 * 1024;
const int iOrigBufferSize = 64 * 1024;
const int iOverflowSize = 0;
const int iWriteTriggerSize = 4192;

Mutex *g_pHeaderMutex;

extern    "C"
{
   PhysicalMediaOutput *Initialize(FAContext *context)
   {
      return new SoundCardPMO(context);
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

         g_pHeaderMutex->Acquire();
         lpWaveHdr->dwUser = -((int)lpWaveHdr->dwUser);
         g_pHeaderMutex->Release();

         break;
      }
   }
}

SoundCardPMO::SoundCardPMO(FAContext *context) :
              EventBuffer(iOrigBufferSize, iOverflowSize, 
                          iWriteTriggerSize, context)
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
   m_pPauseMutex = new Mutex(); 
   g_pHeaderMutex = new Mutex(); 
   m_iTotalBytesWritten = 0;
   m_iHead = 0;
   m_iTail = 0;
   m_iOffset = 0;
   m_iLastFrame = -1;
   m_bPaused = false;

   m_context = context;
   m_prefs = m_context->prefs;

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

   m_pPauseMutex->Release();

   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
   }

   if (m_initialized)
   {
      waveOutReset(m_hwo);

      while (waveOutClose(m_hwo) == WAVERR_STILLPLAYING)
      {
         Sleep(SLEEPTIME);
      }

      // Unprepare and free the header memory.
      for (uint32 j = 0; j < m_num_headers; j++)
      {
         delete    m_wavehdr_array[j];
      }

      delete []m_wavehdr_array;
      delete m_wfex;
   }

   if (m_pPauseMutex)
   {
      delete m_pPauseMutex;
      m_pPauseMutex = NULL;
   }
   if (g_pHeaderMutex)
   {
      delete g_pHeaderMutex;
      g_pHeaderMutex = NULL;
   }
}

VolumeManager *SoundCardPMO::GetVolumeManager()
{
   return new Win32VolumeManager();
}

Error SoundCardPMO::Init(OutputInfo * info)
{
   Error     result = kError_UnknownErr;
   MMRESULT  mmresult = 0;
   int32     iNewSize = iDefaultBufferSize;

   m_channels = info->number_of_channels;
   m_samples_per_second = info->samples_per_second;
   m_data_size = info->max_buffer_size;

   m_context->prefs->GetOutputBufferSize(&iNewSize);
   iNewSize *= 1024;

   iNewSize -= iNewSize % m_data_size;
   result = Resize(iNewSize, 0, m_data_size);
   if (IsError(result))
   {
       ReportError("Internal buffer sizing error occurred.");
       m_context->log->Error("Resize output buffer failed.");

       return result;
   }

   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

   m_num_headers = 32;
   m_hdr_size = sizeof(WAVEHDR);
   m_wavehdr_array = new LPWAVEHDR[m_num_headers];

   m_fillup = 0;
   m_user_stop = false;

   m_wfex = new WAVEFORMATEX;

   m_wfex->wBitsPerSample = info->bits_per_sample;
   m_wfex->wFormatTag = WAVE_FORMAT_PCM;
   m_wfex->nChannels = (WORD) m_channels;
   m_wfex->nSamplesPerSec = info->samples_per_second;
   m_wfex->nBlockAlign = (info->bits_per_sample / 8) * m_channels;
   m_wfex->nAvgBytesPerSec = info->samples_per_second * m_wfex->nBlockAlign;
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
         break;
      }

      temp->dwBufferLength = m_data_size;
      temp->dwBytesRecorded = 0;
      temp->dwUser = 0;         // If played, dwUser = 1

      temp->dwLoops = 0;
      temp->dwFlags = NULL;
   }

   for (i = 0; i < m_channels; i++)
   {
      m_buffer[i] = i * m_channels;
   }

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

Error SoundCardPMO::Break()
{
   PullBuffer::BreakBlocks();

   return kError_NoErr;
}


Error SoundCardPMO::Pause()
{
   if (m_bPaused)
      return kError_NoErr;

   m_pPauseMutex->Acquire();
   waveOutPause(m_hwo);
   m_bPaused = true;

   return kError_NoErr;
}

Error SoundCardPMO::Resume()
{
   if (!m_bPaused)
      return kError_NoErr;

   m_pPauseMutex->Release();
   waveOutRestart(m_hwo);
   m_bPaused = false;

   return kError_NoErr;
}

Error SoundCardPMO::SetPropManager(Properties * p)
{
   m_propManager = p;

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
   MediaTimeInfoEvent *pmtpi;
   int32               hours, minutes, seconds;
   int                 iTotalTime = 0;
   MMTIME              sTime;

   if (pEvent->GetFrameNumber() != m_iLastFrame + 1)
   {
//       m_iTotalBytesWritten = 1152 * pEvent->GetFrameNumber() *
//                              m_iBytesPerSample;
              
       m_iTotalBytesWritten += 1152 * (pEvent->GetFrameNumber() - m_iLastFrame) *
                              m_iBytesPerSample;
   }
   m_iLastFrame = pEvent->GetFrameNumber();

   if (m_samples_per_second <= 0)
      return;

   sTime.wType = TIME_BYTES;
   if (waveOutGetPosition(m_hwo, &sTime, sizeof(sTime)) != MMSYSERR_NOERROR)
       return;

   iTotalTime = (m_iTotalBytesWritten + sTime.u.cb) /
                (m_iBytesPerSample * m_samples_per_second);
   hours = iTotalTime / 3600;
   minutes = (iTotalTime - hours) / 60;
   seconds = iTotalTime - hours * 3600 - minutes * 60;

   if (minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
      return;

   pmtpi = new MediaTimeInfoEvent(hours, minutes, seconds, 0,
                                  (float)iTotalTime, 0);
   m_target->AcceptEvent(pmtpi);
}

Error SoundCardPMO::Write(void *pBuffer)
{
   Error    result = kError_NoErr;
   WAVEHDR *wavhdr = NULL;

   wavhdr = NextHeader();
   if (!wavhdr)
   {
      return kError_Interrupt;
   }

   wavhdr->dwBufferLength = m_data_size;
   wavhdr->lpData = (char *)pBuffer;

   // Prepare & write newest header
   waveOutPrepareHeader(m_hwo, wavhdr, m_hdr_size);

   m_pPauseMutex->Acquire(WAIT_FOREVER);

   waveOutWrite(m_hwo, wavhdr, m_hdr_size);

   m_pPauseMutex->Release();

   return result;
}

Error SoundCardPMO::AllocHeader(void *&pBuffer)
{
    Error    eRet;
    unsigned iRead;

    iRead = m_iOffset + m_data_size;
    eRet = BeginRead(pBuffer, iRead);
    if (eRet != kError_NoErr)
       return eRet;

    eRet = EndRead(0);
    if (eRet != kError_NoErr)
       return eRet;

    pBuffer = (char *)pBuffer + m_iOffset;
    WrapPointer(pBuffer);

    m_iOffset += m_data_size;

    return kError_NoErr;
}

Error SoundCardPMO::FreeHeader()
{
    Error     eRet;
    void     *pBuffer;
    unsigned  iRead;

    iRead = m_data_size;
    eRet = BeginRead(pBuffer, iRead);
    if (eRet != kError_NoErr)
       return eRet;

//  assert(iRead == m_data_size);

    eRet = EndRead(iRead);
    if (eRet != kError_NoErr)
       return eRet;

    m_iOffset -= m_data_size;

    return kError_NoErr;
}

WAVEHDR *SoundCardPMO::NextHeader()
{
   WAVEHDR  *result = NULL;
   unsigned  iLoop;
   Error     eRet;

   assert(m_initialized);

   for(; !m_bExit;)
   {
       g_pHeaderMutex->Acquire();
       for(iLoop = 0; iLoop < m_num_headers; iLoop++)
       {
           if ((int)m_wavehdr_array[iLoop]->dwUser < 0 &&
              (-(int)m_wavehdr_array[iLoop]->dwUser) == m_iTail + 1)
          {
              waveOutUnprepareHeader(m_hwo, m_wavehdr_array[iLoop], sizeof(WAVEHDR));

              eRet = FreeHeader();
              if (IsError(eRet))
              {
                 g_pHeaderMutex->Release();
                 return NULL;
              }

              m_wavehdr_array[iLoop]->dwUser = 0;
              m_iTail++;
          }
          if (!m_wavehdr_array[iLoop]->dwUser)
          {
             result = m_wavehdr_array[iLoop];
             result->dwUser = ++m_iHead;

             g_pHeaderMutex->Release();

             return result;
          }
       }
       g_pHeaderMutex->Release();

       usleep(10000);
   }

   return NULL;
}

void SoundCardPMO::StartWorkerThread(void *pVoidBuffer)
{
   ((SoundCardPMO*)pVoidBuffer)->WorkerThread();
}
 
void SoundCardPMO::WorkerThread(void)
{
   void       *pBuffer;
   Error       eErr;
   Event      *pEvent;
   int         iValue;

   m_prefs->GetDecoderThreadPriority(&iValue);

   m_pBufferThread->SetPriority(iValue);


   m_bPause = false;
   for(; !m_bExit;)
   {
      if (!m_initialized)
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
                  delete (PMOInitEvent *)pEvent;
                  break;
              }
              delete (PMOInitEvent *)pEvent;
          }
		  else
    		  delete pEvent;

          continue;
      }

      eErr = AllocHeader(pBuffer);
      if (eErr == kError_InputUnsuccessful || eErr == kError_NoDataAvail)
      {
          m_pReadSem->Wait();
          continue;
      }

      if (eErr == kError_EventPending)
      {
          pEvent = GetEvent();

          if (pEvent->Type() == PMO_Init)
		  {
              Init(((PMOInitEvent *)pEvent)->GetInfo());
			  delete (PMOInitEvent *)pEvent;
		  }
		  else
          if (pEvent->Type() == PMO_Reset)
		  {
              Reset(false);
			  delete (PMOResetEvent *)pEvent;
		  }
		  else
          if (pEvent->Type() == PMO_Info)
		  {
              HandleTimeInfoEvent((PMOTimeInfoEvent *)pEvent);
			  delete (PMOTimeInfoEvent *)pEvent;
		  } 
		  else
          if (pEvent->Type() == PMO_Quit)
          {
              Reset(false);
              delete (PMOQuitEvent *)pEvent;
              break;
          }

          continue;
      }

      if (IsError(eErr))
      {
          ReportError("Internal error occured.");
          m_context->log->Error("Cannot read from buffer in PMO worker tread: %d\n",
              eErr);
          break;
      }

      Write(pBuffer);
   }
   m_context->log->Log(LogDecode, "PMO: Soundcard thread exiting\n");
}    

