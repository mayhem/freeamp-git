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

#include "aps.h"

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

    m_MB = new MusicBrainz;

    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       assert(m_pBufferThread);
       m_pBufferThread->Create(SignaturePMO::StartWorkerThread, this);
    }

    m_collID = context->aps->GetCollectionId();
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
  if (m_MB)
  {
     delete m_MB;
     m_MB = NULL;
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
   m_data_size = info->max_buffer_size;

   m_MB->SetPCMDataInfo(info->samples_per_second, info->number_of_channels,
                        info->bits_per_sample);

   m_strGUID = "";
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
    bool    bDone = false;
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
            if (pEvent->Type() == PMO_Error)
            {
                AudioSignatureFailedEvent *asfe;
                asfe = new AudioSignatureFailedEvent(m_url, this);
                m_pTarget->AcceptEvent(asfe);
                delete pEvent;
                return;
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
       
            if (bDone || bGotPMOQuit) {
                AudioSignatureGeneratedEvent *asge = 
                             new AudioSignatureGeneratedEvent(m_url, m_strGUID,
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

                    m_MB->GenerateSignatureNow(m_strGUID, m_collID);
                    continue;
                }
                if (pEvent->Type() == PMO_Error)
                {
                    AudioSignatureFailedEvent *asfe;
                    asfe = new AudioSignatureFailedEvent(m_url, this);
                    m_pTarget->AcceptEvent(asfe);
                    delete pEvent;
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
        {
            m_pInputBuffer->EndRead(0);
            continue;
        }

        if (m_MB->GenerateSignature((char *)pBuffer, m_data_size, m_strGUID, 
                                    m_collID)) 
            bDone = true;

        m_pInputBuffer->EndRead(m_data_size);
     
        m_pLmc->Wake();
    }
}

