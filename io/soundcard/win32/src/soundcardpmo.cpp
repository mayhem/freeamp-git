/*____________________________________________________________________________
   
   FreeAmp - The Free MP3 Player

   Portions Copyright (C) 1998-1999 EMusic.com
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
#include <winbase.h>
#include <stdlib.h>
#include <string.h>

/* project headers */
//#include "config.h"
#include "SoundCardPMO.h"
#include "eventdata.h"
#include "log.h"
#include "debug.h"

#define DB Debug_v("%s:%d", __FILE__, __LINE__);

#define MAXINT32 0x7FFFFFFF

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
        PhysicalMediaOutput(context)
{
   m_wfex = NULL;
   m_wavehdr_array = NULL;
   m_hwo = NULL;

   m_channels = 0;
   m_samples_per_second = 0;
   m_samples_per_frame = 0;
   m_hdr_size = 0;
   m_data_size = 0;
   m_initialized = false;
   m_pBufferThread = NULL;
   g_pHeaderMutex = new Mutex(); 
   m_iHead = 0;
   m_iTail = 0;
   m_iOffset = 0;
   m_iBaseTime = MAXINT32;
   m_iBytesPerSample = 0;
   m_num_headers = 0;
   
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
   m_pSleepSem->Signal();
   m_pPauseSem->Signal();

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
         usleep(100000);
      }

      // Unprepare and free the header memory.
      for (uint32 j = 0; j < m_num_headers; j++)
      {
         delete    m_wavehdr_array[j];
      }

      delete []m_wavehdr_array;
      delete m_wfex;
   }
   if (g_pHeaderMutex)
   {
      delete g_pHeaderMutex;
      g_pHeaderMutex = NULL;
   }
}

void SoundCardPMO::SetVolume(int32 volume) 
{
    Int32PropValue *pProp = NULL;
    
    waveOutSetVolume((HWAVEOUT)WAVE_MAPPER, 
                     MAKELPARAM( 0xFFFF*volume/100,  
                                 0xFFFF*volume/100));
                                 
    pProp = new Int32PropValue(volume);
    m_pContext->props->SetProperty("CurrentVolume", pProp);
}

int32 SoundCardPMO::GetVolume() 
{
    int32 volume = 0;

    waveOutGetVolume((HWAVEOUT)WAVE_MAPPER, (DWORD*)&volume);
    volume = (int32)(100 * ((float)LOWORD(volume)/(float)0xffff));

    return volume;
} 

Error SoundCardPMO::Init(OutputInfo * info)
{
   Error           result = kError_UnknownErr;
   MMRESULT        mmresult = 0;
   Int32PropValue *pProp = NULL;

   m_channels = info->number_of_channels;
   m_samples_per_second = info->samples_per_second;
   m_samples_per_frame = info->samples_per_frame;
   m_data_size = info->max_buffer_size;

   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

   m_num_headers = 32;
   m_hdr_size = sizeof(WAVEHDR);
   m_wavehdr_array = new LPWAVEHDR[m_num_headers];

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
   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

   if (IsntError(m_pContext->props->GetProperty("CurrentVolume", 
                (PropValue **)&pProp)))
   {
      SetVolume(pProp->GetInt32());
   }   

   m_initialized = true;

   return result;
}

void SoundCardPMO::HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent)
{
   MediaTimeInfoEvent *pmtpi;
   int32               hours, minutes, seconds;
   int                 iTotalTime = 0;
   MMTIME              sTime;

   if (m_iBaseTime == MAXINT32)
   {
       m_iBaseTime = (pEvent->GetFrameNumber() * m_samples_per_frame) / 
                      m_samples_per_second;

       sTime.wType = TIME_BYTES;
       if (waveOutGetPosition(m_hwo, &sTime, sizeof(sTime)) != MMSYSERR_NOERROR)
           return;
   
       m_iBaseTime -= (sTime.u.cb / (m_samples_per_second * m_iBytesPerSample));
   }

   if (m_samples_per_second <= 0)
      return;

   sTime.wType = TIME_BYTES;
   if (waveOutGetPosition(m_hwo, &sTime, sizeof(sTime)) != MMSYSERR_NOERROR)
       return;
   
   iTotalTime = (sTime.u.cb / (m_samples_per_second * m_iBytesPerSample)) +
                m_iBaseTime;
      
   hours = iTotalTime / 3600;
   minutes = (iTotalTime - 
                (hours * 3600)) / 60;
   seconds = iTotalTime - 
			    (hours * 3600) - 
			    (minutes * 60);

   if (minutes < 0 || minutes > 59 || seconds < 0 || seconds > 59)
      return;

   pmtpi = new MediaTimeInfoEvent(hours, minutes, seconds, 0,
                                  (float)iTotalTime, 0);
   m_pTarget->AcceptEvent(pmtpi);
}

bool SoundCardPMO::WaitForDrain(void)
{
   unsigned iLoop;

   for(; !m_bExit && !m_bPause; )
   {   
       g_pHeaderMutex->Acquire();

	   for(iLoop = 0; iLoop < m_num_headers; iLoop++)
       {
           if ((int)m_wavehdr_array[iLoop]->dwUser >= 0)
			   break;
	   }
	   g_pHeaderMutex->Release();

	   if (iLoop == m_num_headers)
	   {
		  return true;
	   }
	   WasteTime();
   }
 
   return false;
}

void SoundCardPMO::Pause(void)
{
    m_iBaseTime = MAXINT32;

    PhysicalMediaOutput::Pause();
}

void SoundCardPMO::Resume(void)
{
	if (m_initialized)
	   waveOutRestart(m_hwo);
	
	PhysicalMediaOutput::Resume();
}

void SoundCardPMO::Clear(void)
{
	if (m_initialized)
	   waveOutReset(m_hwo);
	
	PhysicalMediaOutput::Clear();
}

Error SoundCardPMO::Reset(bool user_stop)
{
   waveOutPause(m_hwo);

   return kError_NoErr;
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
   waveOutWrite(m_hwo, wavhdr, m_hdr_size);

   return result;
}

Error SoundCardPMO::AllocHeader(void *&pBuffer)
{
    Error    eRet;
    unsigned iRead;
	static char *pSaved = NULL;

    iRead = m_iOffset + m_data_size;
    eRet = ((EventBuffer *)m_pInputBuffer)->BeginRead(pBuffer, iRead);
    if (eRet != kError_NoErr)
       return eRet;

    eRet = ((EventBuffer *)m_pInputBuffer)->EndRead(0);
    if (eRet != kError_NoErr)
       return eRet;

    if (pSaved == NULL)
	   pSaved = (char *)pBuffer;
	   
    pBuffer = (char *)pBuffer + m_iOffset;
    ((EventBuffer *)m_pInputBuffer)->WrapPointer(pBuffer);

    m_iOffset += m_data_size;
    ((EventBuffer *)m_pInputBuffer)->SetBytesInUse(m_iOffset);

    return kError_NoErr;
}

Error SoundCardPMO::FreeHeader()
{
    Error     eRet;
    void     *pBuffer;
    unsigned  iRead;

    iRead = m_data_size;
    eRet = ((EventBuffer *)m_pInputBuffer)->BeginRead(pBuffer, iRead);
    if (eRet != kError_NoErr)
       return eRet;

    eRet = ((EventBuffer *)m_pInputBuffer)->EndRead(iRead);
    if (eRet != kError_NoErr)
       return eRet;

    m_iOffset -= m_data_size;
    ((EventBuffer *)m_pInputBuffer)->SetBytesInUse(m_iOffset);

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

   // Don't do anything until resume is called.
   m_pPauseSem->Wait();

   // Wait for prebuffer period
   PreBuffer();

   m_pContext->prefs->GetDecoderThreadPriority(&iValue);
   m_pBufferThread->SetPriority(iValue);

   for(; !m_bExit;)
   {
      if (m_bPause)
      {
          m_pPauseSem->Wait();
          continue;
      }

      // Loop until we get an Init event from the LMC
      if (!m_initialized)
      {
          pEvent = ((EventBuffer *)m_pInputBuffer)->GetEvent();

          if (pEvent == NULL)
          {
              m_pLmc->Wake();
              WasteTime();

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
		  if (m_bPause || m_bExit)
			  break;
	      
          eErr = AllocHeader(pBuffer);
		  if (eErr == kError_EndOfStream || eErr == kError_Interrupt)
             break;

          if (eErr == kError_NoDataAvail)
          {
              m_pLmc->Wake();
    
              WasteTime();
              continue;
          }

          // Is there an event pending that we need to take care of
          // before we play this block of samples?
          if (eErr == kError_EventPending)
          {
              pEvent = ((EventBuffer *)m_pInputBuffer)->GetEvent();
			  if (pEvent == NULL)
				  continue;

              if (pEvent->Type() == PMO_Init)
                  Init(((PMOInitEvent *)pEvent)->GetInfo());
    
              if (pEvent->Type() == PMO_Reset)
                  Reset(true);
    
              if (pEvent->Type() == PMO_Info) 
                  HandleTimeInfoEvent((PMOTimeInfoEvent *)pEvent);
    
              if (pEvent->Type() == PMO_Quit) 
              {
                  delete pEvent;
                  if (WaitForDrain())
				  {
                     m_pTarget->AcceptEvent(new Event(INFO_DoneOutputting));
				  }

                  return;
              }
 
              delete pEvent;
    
              continue;
          }
          
          if (IsError(eErr))
          {
              ReportError("Internal error occured.");
              m_pContext->log->Error("Cannot read from buffer in PMO "
                                    "worker tread: %d\n", eErr);
              return;
          }
          break;
      }
	  if (m_bPause || m_bExit)
		 continue;

      Write(pBuffer);
	  m_pLmc->Wake();

      UpdateBufferStatus();
   }
   m_pContext->log->Log(LogDecode, "PMO: Soundcard thread exiting\n");
}    


