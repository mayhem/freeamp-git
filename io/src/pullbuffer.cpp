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
#include <string.h>
#include <assert.h>

#include "pullbuffer.h"

#define DB //printf("%s:%d\n", __FILE__, __LINE__);

PullBuffer::PullBuffer(size_t iBufferSize,
                       size_t iOverflowSize,
                       size_t iWriteTriggerSize)
{
   m_bEOS = false;
   m_bReadOpPending = false;
   m_bWriteOpPending = false;
   m_iReadIndex =  m_iWriteIndex = 0;
   m_iBytesInBuffer = 0;

   m_iOverflowSize = iOverflowSize;
   m_iBufferSize = iBufferSize;
   m_iWriteTriggerSize = iWriteTriggerSize;

   m_pPullBuffer = new unsigned char[m_iBufferSize + iOverflowSize];
   assert(m_pPullBuffer != NULL);

   m_pWriteSem = new Semaphore();
   m_pReadSem = new Semaphore();
   m_pMutex = new Mutex();
}

PullBuffer::~PullBuffer(void)
{
   // In case anyone is blocked on a BeginRead 
   m_bEOS = true;
   m_pReadSem->Signal();

   delete m_pPullBuffer;
   delete m_pWriteSem;
   delete m_pReadSem;
   delete m_pMutex;
}

void PullBuffer::Clear(void)
{
   for(;;)
   {
       m_pMutex->Acquire();

       if (m_bReadOpPending || m_bWriteOpPending)
       {
           m_pMutex->Release();
           continue;
       }
       break;
   }
  
   m_bEOS = false;
   m_iReadIndex =  m_iWriteIndex = 0;
   m_iBytesInBuffer = 0;

   m_pWriteSem->Signal();

   m_pMutex->Release();
}

bool PullBuffer::IsEndOfStream(void)
{
   bool bRet;

   m_pMutex->Acquire();
   bRet = m_bEOS; 
   m_pMutex->Release();

   return bRet;
}

void PullBuffer::SetEndOfStream(bool bEOS)
{
   m_pMutex->Acquire();
   m_bEOS = bEOS;
   m_pMutex->Release();
}

Error PullBuffer::BeginWrite(void *&pBuffer, size_t &iBytesToWrite)
{
   Error eError = kError_NoErr;

   assert(m_pPullBuffer != NULL);
   assert(m_iReadIndex >= 0 && m_iReadIndex < m_iBufferSize);
   assert(m_iWriteIndex >= 0 && m_iWriteIndex < m_iBufferSize);

   m_pMutex->Acquire();

   pBuffer = m_pPullBuffer + m_iWriteIndex;

   if (m_iWriteIndex > m_iReadIndex)
   {
       iBytesToWrite = m_iBufferSize - m_iWriteIndex;
       if (iBytesToWrite > m_iWriteTriggerSize)
          iBytesToWrite = m_iWriteTriggerSize;
   }
   else
   {
      if (m_iWriteIndex == m_iReadIndex)
      {
          if (m_iBytesInBuffer == 0)
              iBytesToWrite = m_iWriteTriggerSize; 
          else
              iBytesToWrite = 0;
      }
      else
          iBytesToWrite = m_iReadIndex - m_iWriteIndex;

      if (iBytesToWrite < m_iWriteTriggerSize)
         eError = kError_BufferTooSmall;
      else
      if (iBytesToWrite > m_iWriteTriggerSize)
         iBytesToWrite = m_iWriteTriggerSize;
   }

   assert(m_iWriteIndex + iBytesToWrite <= m_iBufferSize);

   if (eError == kError_NoErr)
      m_bWriteOpPending = true;

   m_pMutex->Release();
  
   return eError;
}

Error PullBuffer::EndWrite(size_t iBytesWritten)
{
   int32 iBlock;
   Error eError;

   assert(m_pPullBuffer != NULL);

   m_pMutex->Acquire();

   m_iWriteIndex = (m_iWriteIndex + iBytesWritten) % m_iBufferSize;
   m_iBytesInBuffer += iBytesWritten;
   assert(m_iBytesInBuffer <= m_iBufferSize);

   m_pReadSem->Signal();

   if (m_iBufferSize - m_iBytesInBuffer >= m_iWriteTriggerSize && !m_bEOS)
      m_pWriteSem->Signal();

   m_bWriteOpPending = false;

   m_pMutex->Release();

   return kError_NoErr;
}

Error PullBuffer::BeginRead(void *&pBuffer, size_t &iBytesNeeded)
{
   assert(m_pPullBuffer != NULL);
   Error  eError = kError_UnknownErr;
   size_t iOverflow;

   for(;;)
   {
      m_pMutex->Acquire();

      if (m_bEOS && iBytesNeeded > m_iBytesInBuffer)
      {
          iBytesNeeded = m_iBytesInBuffer;
          eError = kError_InputUnsuccessful;
          break;
      }

      if (iBytesNeeded > m_iBytesInBuffer)
      {
          m_pMutex->Release();
          m_pReadSem->Wait();
          continue;
      }

      pBuffer = m_pPullBuffer + m_iReadIndex;
      if (m_iWriteIndex > m_iReadIndex ||
          m_iBufferSize - m_iReadIndex >= iBytesNeeded)
      {
          eError = kError_NoErr;
          break;
      }

      // We need to copy the beginning part of the buffer into the
      // overflow.
      iOverflow = (m_iReadIndex + iBytesNeeded) - m_iBufferSize; 
      if (iOverflow > m_iOverflowSize)
      {
          printf("Overflow buffer is too small!! \n");
          printf("Needed %d but had only %d for overflow.\n",
               iOverflow, m_iOverflowSize);

          eError = kError_InvalidParam;
          break;
      } 
      
      memcpy(m_pPullBuffer + m_iBufferSize, m_pPullBuffer, iOverflow);
      eError = kError_NoErr;

      break;
   }   

   if (eError == kError_NoErr)
      m_bReadOpPending = true;

   m_pMutex->Release();

   return eError;
}

Error PullBuffer::EndRead(size_t iBytesUsed)
{
   int32 iBlock;
   Error eError;

   assert(m_pPullBuffer != NULL);

   m_pMutex->Acquire();

   m_iReadIndex = (m_iReadIndex + iBytesUsed) % m_iBufferSize;

   m_iBytesInBuffer -= iBytesUsed;
   assert(m_iBytesInBuffer <= m_iBufferSize);

   if (m_iBufferSize - m_iBytesInBuffer >= m_iWriteTriggerSize && !m_bEOS)
       m_pWriteSem->Signal();

   m_bReadOpPending = false;

   m_pMutex->Release();

   return kError_NoErr;
}

void PullBuffer::DiscardBytes()
{
   m_pMutex->Acquire();

   m_iReadIndex = (m_iReadIndex + m_iWriteTriggerSize) % m_iBufferSize;
	m_iBytesInBuffer -= m_iWriteTriggerSize;

   m_pMutex->Release();
}
