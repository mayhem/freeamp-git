/*____________________________________________________________________________

  FreeAmp - The Free MP3 Player

  Portions Copyright (C) 2000 Relatable

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
#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/* project headers */
#include "config.h"
#include "signaturepmo.h"
#include "eventdata.h"
#include "preferences.h"
#include "facontext.h"
#include "log.h"
#include "debug.h"

#define DB Debug_v("%s:%d", __FILE__, __LINE__);

extern "C"
{
  PhysicalMediaOutput* Initialize(FAContext *context)
  {
      return new SignaturePMO(context);
  }
}

SignaturePMO::
SignaturePMO(FAContext *context):
     PhysicalMediaOutput(context)
{
    m_pBufferThread       = NULL;
    m_iTotalBytesWritten  = 0;
    m_iFinishedFFTs       = 0;
    m_iZeroCrossings      = 0;
    m_dEnergySum          = 0.0;
    m_bLastNeg            = false;
    m_pFFT                = NULL;
    m_downmixBuffer       = NULL;

    for (int i = 0; i < iFFTPoints; i++)
        m_iSpectrum[i] = 0;

    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       assert(m_pBufferThread);
       m_pBufferThread->Create(SignaturePMO::StartWorkerThread, this);
    }

    m_initialized = false;
}

SignaturePMO::
~SignaturePMO()
{
  m_bExit = true;
  m_pSleepSem->Signal();
  m_pPauseSem->Signal();

  if (m_pBufferThread)
  {
     m_pBufferThread->Join();
     delete m_pBufferThread;
     m_pBufferThread = NULL;
  }
  if (m_pFFT)
  {
     delete m_pFFT;
     m_pFFT = NULL;
  }
  if (m_downmixBuffer)
  {
     delete m_downmixBuffer;
     m_downmixBuffer = NULL;
  }
}

void
SignaturePMO::
GetVolume(int32 &left, int32 &right)
{
    left = right = -1;
}

void
SignaturePMO::
SetVolume(int32 left, int32 right)
{
}

Error
SignaturePMO::
Init(OutputInfo* info)
{
   m_samples_per_second = info->samples_per_second;
   m_data_size          = info->max_buffer_size;
   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample >> 3);
   m_number_of_channels = info->number_of_channels;
   m_bits_per_sample = info->bits_per_sample;

   m_downmix_size = m_data_size;
   if (m_samples_per_second != 11025)
       m_downmix_size = m_downmix_size * 11025 / m_samples_per_second;

   if (info->bits_per_sample != 8)
       m_downmix_size /= 2;

   if (info->number_of_channels != 1)
       m_downmix_size /= 2;

   if (m_downmix_size % iFFTPoints != 0)
       m_maxloop = iFFTPoints / (m_downmix_size % iFFTPoints);
   else
       m_maxloop = 1;

   m_downmix_size *= m_maxloop;

   m_downmixBuffer = new unsigned char[m_downmix_size];
   m_loopcount = 0;

   m_initialized = true;
   return kError_NoErr;
}

void 
SignaturePMO::
Pause()
{
   PhysicalMediaOutput::Pause();
}

void
SignaturePMO::
Resume()
{
   PhysicalMediaOutput::Resume();

   m_url = m_pPmi->Url();
}

Error
SignaturePMO::
Reset(bool user_stop)
{
   return kError_NoErr;
}

void 
SignaturePMO::
Clear()
{
   PhysicalMediaOutput::Clear();
}

void
SignaturePMO::
Quit()
{
}

void
SignaturePMO::
StartWorkerThread(void *pVoidBuffer)
{
  ((SignaturePMO*)pVoidBuffer)->WorkerThread();
}

void
SignaturePMO::
WorkerThread(void)
{
    void*   pBuffer;
    Error   eErr;
    Event*  pEvent;
    bool    bSetLastNeg = false;
    bool    bGotPMOQuit = false;

    // Don't do anything until resume is called.
    m_pPauseSem->Wait();

    for (; !m_bExit;)
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
       
            if ((m_iFinishedFFTs) >= iMaxFFTs || (bGotPMOQuit)) {
                int iNumSamples = m_iTotalBytesWritten;
                float fLength = iNumSamples / (float)11025;
                float fAverageZeroCrossing = m_iZeroCrossings / fLength;
                float fEnergy = m_dEnergySum / (float)iNumSamples;

                for (int i = 0; i < iFFTPoints; i++)
                    m_iSpectrum[i] = m_iSpectrum[i] / m_iFinishedFFTs;

                AudioSignatureGeneratedEvent *asge;
                asge = new AudioSignatureGeneratedEvent(m_url, fEnergy,
                                                        fAverageZeroCrossing,
                                                        fLength, m_iSpectrum,
                                                        this);
                Quit();
                m_pTarget->AcceptEvent(asge);
                return;
            }

            eErr = ((EventBuffer *)m_pInputBuffer)->BeginRead(pBuffer, 
                                                              m_data_size);
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
   
                if (pEvent->Type() == PMO_Quit) {
                    bGotPMOQuit = true;
                    continue;
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
        {
            m_pInputBuffer->EndRead(0);
            continue;
        }

        int writeoffset = (m_downmix_size / m_maxloop) * m_loopcount;
        int maxwrite = m_downmix_size / m_maxloop;
        int readpos = 0;
        int writepos = 0;
        int rate_change = m_samples_per_second / 11025;
        signed short lsample, rsample;
        unsigned char ls, rs;

        if (m_bits_per_sample == 16) {
            if (m_number_of_channels == 2) {
                while (writepos < maxwrite) {
                    readpos  = writepos * rate_change;
                    readpos *= 2;

                    lsample = ((signed short *)pBuffer)[readpos++];
                    rsample = ((signed short *)pBuffer)[readpos++];

                    lsample /= 256; lsample += 127;
                    rsample /= 256; rsample += 127;
 
                    m_downmixBuffer[writeoffset + writepos++] = 
                                                      (lsample + rsample) / 2;
                }
            }
            else {
                while (writepos < maxwrite) {
                    readpos = writepos * rate_change;
           
                    lsample = ((signed short *)pBuffer)[readpos];
                    lsample /= 256; lsample += 127;

                    m_downmixBuffer[writeoffset + writepos++] = lsample;
                }
            }
        }
        else {
            if (m_number_of_channels == 2) {
                while (writepos < maxwrite) {
                    readpos  = writepos * rate_change;
                    readpos /= 2;

                    ls = ((unsigned char *)pBuffer)[readpos++];
                    rs = ((unsigned char *)pBuffer)[readpos++];

                    m_downmixBuffer[writeoffset + writepos++] = (ls + rs) / 2;
                }
            }
            else {
                while (writepos < maxwrite) {
                    readpos = writepos * rate_change;
                    ls = ((unsigned char *)pBuffer)[readpos];
                    m_downmixBuffer[writeoffset + writepos++] = ls;
                }
            }
        }

        m_loopcount++;

        if (m_loopcount == m_maxloop) {
            m_loopcount = 0;
            if (!bSetLastNeg) {
                if (*(char *)m_downmixBuffer <= 0)
                    m_bLastNeg = true;
                bSetLastNeg = true;
            }

            if (!m_pFFT) {
                m_pFFT = new FFT(iFFTPoints, 11025);
                m_pFFT->CopyIn((char *)m_downmixBuffer, iFFTPoints);
                m_pFFT->Transform();
            }

            char *pCurrent = (char *)m_downmixBuffer;
            char *pBegin = pCurrent;
            int  iFFTs = m_downmix_size / iFFTPoints;
            int  j, k;
     
            for (j = 0; j < iFFTs; j++, m_iFinishedFFTs++) {

                m_pFFT->CopyIn(pCurrent, iFFTPoints);
                m_pFFT->Transform();

                for (k = 0; k < iFFTPoints; k++)
                    m_iSpectrum[k] += (int)m_pFFT->GetIntensity(k);

                while (pCurrent < pBegin + iFFTPoints) 
                {
                    m_dEnergySum += ((*pCurrent) * (*pCurrent));
                    if (m_bLastNeg && (*pCurrent > 0)) 
                    {
                        m_bLastNeg = false;
                        m_iZeroCrossings++;
                    }
                    else if (!m_bLastNeg && (*pCurrent <= 0))
                        m_bLastNeg = true;
                    pCurrent++;
                }
                pBegin = pCurrent;
            }

            m_iTotalBytesWritten += m_downmix_size;
        }

        m_pInputBuffer->EndRead(m_data_size);
     
        m_pLmc->Wake();
    }
}

