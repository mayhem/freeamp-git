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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

#include "eventbuffer.h"

#define DB printf("%x: %s:%d\n", pthread_self(), __FILE__, __LINE__);

EventBuffer::EventBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                         size_t iWriteTriggerSize, FAContext *context) : 
				 PullBuffer(iBufferSize, iOverFlowSize, iWriteTriggerSize, context)
{
    m_pQueue = new Queue < BufferEvent * >();
}

EventBuffer::~EventBuffer(void)
{
    delete m_pQueue;
}

Error EventBuffer::BeginRead(void *&pBuffer, size_t &iBytesWanted)
{
   BufferEvent   *pEvent;
   int   iReadIndex, iMaxBytes;

   pEvent = m_pQueue->Peek();
   iReadIndex = GetReadIndex();

   if (pEvent && pEvent->iIndex == iReadIndex)
   {
      return kError_EventPending;
   }

   if (!pEvent)
   {
       return PullBuffer::BeginRead(pBuffer, iBytesWanted);
   }

   if (pEvent->iIndex > iReadIndex)
      iMaxBytes = pEvent->iIndex - iReadIndex;
   else
      iMaxBytes = (GetBufferSize() - iReadIndex) + pEvent->iIndex;

   if (iBytesWanted > iMaxBytes)
      iBytesWanted = iMaxBytes;

   return PullBuffer::BeginRead(pBuffer, iBytesWanted);
}

Error EventBuffer::AcceptEvent(Event *pPMOEvent)
{
   BufferEvent *pEvent;

   pEvent = new BufferEvent;
   pEvent->iIndex = GetWriteIndex(); 
   pEvent->pEvent = pPMOEvent;

   m_pQueue->Write(pEvent);

	return kError_NoErr;
}

Event *EventBuffer::GetEvent()
{
   BufferEvent *pEvent;
   Event       *pPMOEvent;

   pEvent = m_pQueue->Read();
   if (pEvent == NULL)
	  return NULL;

   pPMOEvent = pEvent->pEvent;
   delete pEvent;

   return pPMOEvent;
}

Event *EventBuffer::PeekEvent()
{
   BufferEvent *pEvent;

   pEvent = m_pQueue->Peek();
   return pEvent->pEvent;
}

Error EventBuffer::Clear()
{
   m_pQueue->Clear();

   return PullBuffer::Clear();
}
