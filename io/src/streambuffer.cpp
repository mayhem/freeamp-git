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

#include "streambuffer.h"

StreamBuffer::StreamBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                           FAContext *context) : 
     PullBuffer(iBufferSize, iOverFlowSize, context)
{
   m_bBufferingUp = true;
   m_bPause = true;
	m_pStreamMutex = new Mutex();
   m_bDiscardedBytes = false;
}

StreamBuffer::~StreamBuffer(void)
{
   delete m_pStreamMutex;
}

bool StreamBuffer::IsBufferingUp(int32 iBytesNeeded)
{
   return GetNumBytesInBuffer() < (uint32)iBytesNeeded;
}

bool StreamBuffer::DidDiscardBytes()
{
   bool bRet = m_bDiscardedBytes;

   m_bDiscardedBytes = false;

   return bRet;
}

Error StreamBuffer::BeginWrite(void *&pBuffer, size_t iBytesNeeded)
{
   Error eRet;

   eRet = PullBuffer::BeginWrite(pBuffer, iBytesNeeded);
   m_pStreamMutex->Acquire();
  
	if (m_bPause && eRet == kError_BufferTooSmall)
	{
	    eRet = DiscardBytes();
       m_bDiscardedBytes = true;
  
	    m_pStreamMutex->Release();

       if (IsError(eRet))
          return eRet;

       return PullBuffer::BeginWrite(pBuffer, iBytesNeeded);
   }

	m_pStreamMutex->Release();

	return eRet;
}
