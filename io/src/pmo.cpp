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
#include <fcntl.h>
//#include <sys/soundcard.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/* project headers */
#include <config.h>
#include "pipeline.h"
#include "pullbuffer.h"
#include "eventbuffer.h"
#include "eventdata.h"
#include "facontext.h"
#include "log.h"

#define DB printf("%s:%d\n", __FILE__, __LINE__);  

PhysicalMediaOutput::PhysicalMediaOutput(FAContext *context) :
                     PipelineUnit(context)
{
    m_pPmi = NULL;
    m_pLmc = NULL;
}

PhysicalMediaOutput::~PhysicalMediaOutput()
{
    m_bExit = true;

    if (m_pLmc)
        m_pLmc->Pause();
    if (m_pPmi)
        m_pPmi->Pause();

    m_pPauseSem->Signal();
    m_pSleepSem->Signal();

    delete m_pLmc;
    delete m_pPmi;
}

Error PhysicalMediaOutput::SetTo(char *url)
{
    Error       eRet;
    PullBuffer *pBuffer;

    m_pMutex->Acquire();

    assert(m_pPmi != NULL);
    assert(m_pLmc != NULL);

    m_pPmi->SetTo(url);
    eRet = m_pPmi->Prepare(pBuffer);
    if (!IsError(eRet))
         eRet = m_pLmc->Prepare(pBuffer, m_pInputBuffer);

    m_pMutex->Release();

    return eRet;
}

void PhysicalMediaOutput::SetLMC(LogicalMediaConverter *pLMC)
{
    m_pMutex->Acquire();

    m_pLmc = pLMC;

    m_pMutex->Release();
}

void PhysicalMediaOutput::SetPMI(PhysicalMediaInput *pPMI)
{
    m_pMutex->Acquire();

    m_pPmi = pPMI;

    m_pMutex->Release();
}

void PhysicalMediaOutput::Pause(void)
{
    PipelineUnit::Pause();

    m_pPmi->PauseLoop(true);

    Reset(true);
}

void PhysicalMediaOutput::Resume(void)
{
    if (m_pPmi->PauseLoop(false))
    {
        m_pLmc->Pause();
        m_pPmi->Pause();
        m_pLmc->Clear();
        m_pPmi->Resume();
        m_pLmc->Resume();
    }

    PipelineUnit::Resume();
}

bool PhysicalMediaOutput::WasteTime()
{
    usleep(10000);

    return m_bExit || m_bPause;
}       

Error PhysicalMediaOutput::ChangePosition(int32 position)
{
   Error     error = kError_NoErr;

   if (m_pPmi->IsStreaming())
      return kError_FileSeekNotSupported;

   m_pMutex->Acquire();

   Pause();
   m_pLmc->Pause();
   m_pPmi->Pause();

   m_pLmc->Clear();
   m_pPmi->Clear();

   m_pLmc->ChangePosition(position);

   m_pPmi->Resume();
   m_pLmc->Resume();
   Resume();

   m_pMutex->Release();

   return error;
}  
