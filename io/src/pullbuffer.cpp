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
#include "log.h"

extern LogFile *g_Log;

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#define DB printf("%x %s:%d\n", pthread_self(), __FILE__, __LINE__);

PullBuffer::PullBuffer(size_t iBufferSize,
                       size_t iOverflowSize,
                       size_t iWriteTriggerSize)
{
   m_bEOS = false;
   m_bExit = false;
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
   m_bExit = true;
   m_pReadSem->Signal();
   m_pWriteSem->Signal();

   delete m_pPullBuffer;
   delete m_pWriteSem;
   delete m_pReadSem;
   delete m_pMutex;
}

void PullBuffer::BreakBlocks(void)
{
   m_bExit = true;
   m_pReadSem->Signal();
   m_pWriteSem->Signal();
}

Error PullBuffer::Clear(void)
{
   for(;;)
   {
       if (m_bExit)
          return kError_Interrupt;

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

   return kError_NoErr;
}

int PullBuffer::GetReadIndex(void)
{
   int iRet;

   for(;;)
   {
       if (m_bExit)
          return kError_Interrupt;

       m_pMutex->Acquire();

       if (m_bReadOpPending)
       {
           m_pMutex->Release();
           continue;
       }
       break;
   }

   iRet = m_iReadIndex;

   m_pMutex->Release();
 
   return iRet;
}

int PullBuffer::GetWriteIndex(void)
{
   int iRet;

   for(;;)
   {
       if (m_bExit)
          return kError_Interrupt;

       m_pMutex->Acquire();

       if (m_bWriteOpPending)
       {
           m_pMutex->Release();
           continue;
       }
       break;
   }

   iRet = m_iWriteIndex;

   m_pMutex->Release();
 
   return iRet;
}

void PullBuffer::WrapPointer(void *&pBuffer)
{
   char *pPtr;

   pPtr = (char *)pBuffer;

   if (pPtr >= (char *)((int)m_pPullBuffer + m_iBufferSize))
	   pPtr -= m_iBufferSize;

   pBuffer = (void *)pPtr;
}


Error PullBuffer::Resize(size_t iNewSize, 
                         size_t iNewOverflowSize, 
                         size_t iNewWriteTriggerSize)
{
   unsigned char *pNew;

   if (iNewSize < m_iBytesInBuffer)
   {
       g_Log->Error("Pullbuffer: Not resized. Too many bytes in buffer.\n");
       return kError_BufferTooSmall;
   }

   // Nothing should've been read from the buffer yet!
   if (m_iReadIndex != 0)
   {
       g_Log->Error("Pullbuffer: Not resized. The buffer has been read.\n");
       return kError_InvalidError;
   }

   for(;;)
   {
       if (m_bExit)
          return kError_Interrupt;

       m_pMutex->Acquire();

       if (m_bReadOpPending || m_bWriteOpPending)
       {
           m_pMutex->Release();
		   usleep(10000);
           continue;
       }
       break;
   }

   pNew = new unsigned char[iNewSize + iNewOverflowSize];
   memcpy(pNew, m_pPullBuffer, m_iBytesInBuffer);

   delete m_pPullBuffer;
   m_pPullBuffer = pNew;
   m_iBufferSize = iNewSize;
   m_iOverflowSize = iNewOverflowSize;
   m_iWriteTriggerSize = iNewWriteTriggerSize;
   m_iWriteIndex = m_iBytesInBuffer;

   m_pWriteSem->Signal();

   m_pMutex->Release();

   return kError_NoErr;
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
   assert(m_iReadIndex >= 0 && (uint32)m_iReadIndex < m_iBufferSize);
   assert(m_iWriteIndex >= 0 && (uint32)m_iWriteIndex < m_iBufferSize);
   assert(m_bWriteOpPending == false);

   m_pMutex->Acquire();

   if (m_bExit)
   {
       m_pMutex->Release();
       return kError_Interrupt;
   }

   pBuffer = m_pPullBuffer + m_iWriteIndex;

   //printf("%08X: BeginWrite: ReadIndex: %d WriteIndex %d Size: %d Bytes: %d\n", 
   //    pthread_self(), m_iReadIndex, m_iWriteIndex, m_iBufferSize, m_iBytesInBuffer);
   if (m_iWriteIndex > m_iReadIndex)
   {
       iBytesToWrite = m_iBufferSize - m_iWriteIndex + 
                            min((uint32)m_iReadIndex, m_iOverflowSize);

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
      {
          iBytesToWrite = m_iReadIndex - m_iWriteIndex;
      }

      if (iBytesToWrite < m_iWriteTriggerSize)
	  {
         eError = kError_BufferTooSmall;
	  }
      else
      if (iBytesToWrite > m_iWriteTriggerSize)
         iBytesToWrite = m_iWriteTriggerSize;
   }
   //printf("%08X: iBytesToWrite: %d\n", pthread_self(), iBytesToWrite);

   assert(m_iWriteIndex + iBytesToWrite <= m_iBufferSize + m_iOverflowSize);
   assert(m_iBytesInBuffer + iBytesToWrite <= m_iBufferSize);

   if (eError == kError_NoErr)
   {
      m_bWriteOpPending = true;
   }

   m_pMutex->Release();
  
   return eError;
}

Error PullBuffer::EndWrite(size_t iBytesWritten)
{
   assert(m_pPullBuffer != NULL);

   m_pMutex->Acquire();

   if (m_iBytesInBuffer + iBytesWritten > m_iBufferSize)
   {
       m_pMutex->Release();

       return kError_YouScrewedUp;
   }

   m_iWriteIndex = m_iWriteIndex + iBytesWritten;
   if ((uint32)m_iWriteIndex > m_iBufferSize)
      memmove(m_pPullBuffer, m_pPullBuffer + m_iBufferSize,
              m_iWriteIndex - m_iBufferSize);

   m_iWriteIndex %= m_iBufferSize;
   assert(m_iWriteIndex >= 0 && (uint32)m_iWriteIndex < m_iBufferSize);
   m_iBytesInBuffer += iBytesWritten;

   //printf("%08X: bytesinbuffer: %d byteswritten %d buffsize: %d\n", 
   //   pthread_self(), m_iBytesInBuffer, iBytesWritten, m_iBufferSize);

   
   assert(m_iBytesInBuffer <= m_iBufferSize);

   m_pReadSem->Signal();

   if (m_iBufferSize - m_iBytesInBuffer >= m_iWriteTriggerSize && !m_bEOS)
      m_pWriteSem->Signal();

   m_bWriteOpPending = false;

   g_Log->Log(LogInput, "EndWrite: ReadIndex: %d WriteIndex %d\n", m_iReadIndex, m_iWriteIndex);
   m_pMutex->Release();

   return kError_NoErr;
}

Error PullBuffer::BeginRead(void *&pBuffer, size_t &iBytesNeeded, bool bBlock)
{
   assert(m_pPullBuffer != NULL);
   assert(m_bReadOpPending == false);
   Error  eError = kError_UnknownErr;
   size_t iOverflow;

   for(;;)
   {
      if (m_bExit)
      {
          return kError_Interrupt;
      }
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

          if (bBlock)
          {
             m_pReadSem->Wait();
             continue;
          }

          return kError_NoDataAvail;
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
          g_Log->Error("Overflow buffer is too small!! \n"
                       "Needed %d but had only %d for overflow.\n",
                       iOverflow, m_iOverflowSize);

          eError = kError_InvalidParam;
          break;
      } 
      
      memcpy(m_pPullBuffer + m_iBufferSize, m_pPullBuffer, iOverflow);
      eError = kError_NoErr;

      break;
   }   

   if (eError == kError_NoErr)
   {
      m_bReadOpPending = true;
   }

   m_pMutex->Release();

   return eError;
}

Error PullBuffer::EndRead(size_t iBytesUsed)
{
   assert(m_pPullBuffer != NULL);
   assert(iBytesUsed >= 0);

   m_pMutex->Acquire();

   m_iReadIndex = (m_iReadIndex + iBytesUsed) % m_iBufferSize;

   m_iBytesInBuffer -= iBytesUsed;
   assert(m_iBytesInBuffer <= m_iBufferSize);

   m_bReadOpPending = false;

   g_Log->Log(LogInput, "EndRead: ReadIndex: %d WriteIndex %d\n", m_iReadIndex, m_iWriteIndex);

   if (m_iBufferSize - m_iBytesInBuffer >= m_iWriteTriggerSize && !m_bEOS)
       m_pWriteSem->Signal();

   m_pMutex->Release();

   return kError_NoErr;
}

Error PullBuffer::DiscardBytes()
{

   for(;;)
   {
        if (m_bExit)
           return kError_Interrupt;

        m_pMutex->Acquire();
        if (m_bReadOpPending || m_bWriteOpPending)
        {
            m_pMutex->Release();
            continue;
        }
        break;
   }

   if (m_iBytesInBuffer < m_iWriteTriggerSize)
   {
      m_iReadIndex = (m_iReadIndex + m_iWriteTriggerSize) % m_iBufferSize;
    	m_iBytesInBuffer -= m_iWriteTriggerSize;
      assert(m_iBytesInBuffer <= m_iBufferSize);
      g_Log->Log(LogInput, "Discarding %d bytes.\n", m_iWriteTriggerSize);
   }

   m_pMutex->Release();

   return kError_NoErr;
}
