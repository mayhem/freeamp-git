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
#include <sys/time.h>
#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <fcntl.h>
#endif

#include "obsbuffer.h"
#include "log.h"

const int iMaxHostNameLen = 64;
const int iGetHostNameBuffer = 1024;
const int iInitialBufferSize = 64;

const int iReadTimeout       = 10;       // in secs

#define DB printf("%s:%d\n", __FILE__, __LINE__);

static char *g_ErrorArray[5] =
{
   "Invalid URL. URL format: http://<host>[:port][/path]",
   "Cannot set socket options.",
   "Cannot create socket.",
   "Cannot bind multicast socket.",
   "Failed to read data from socket."
};

ObsBuffer::ObsBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                     size_t iWriteTriggerSize, char *szFile) :
        StreamBuffer(iBufferSize, iOverFlowSize, iWriteTriggerSize)
{
    m_hHandle = -1;
    m_pBufferThread = NULL;
    m_pID3Tag = NULL;
    m_szError = new char[iMaxErrorLen];

    strcpy(m_szUrl, szFile);
}

ObsBuffer::~ObsBuffer(void)
{
    m_bExit = true;
    g_Log->Log(LogInput, "obsbuffer dtor sleep\n");
    m_pWriteSem->Signal();
    m_pReadSem->Signal();
    g_Log->Log(LogInput, "obsbuffer dtor wake\n");

    if (m_pBufferThread)
    {
       g_Log->Log(LogInput, "obsbuffer dtor join\n");
       m_pBufferThread->Join();
       delete m_pBufferThread;
    }

    if (m_hHandle >= 0)
       close(m_hHandle);

    if (m_pID3Tag)
       delete m_pID3Tag;

    delete m_szError;
}

Error ObsBuffer::Open(void)
{
    int    iRet, iPort;
    struct ip_mreq sMreq;
    int    iReuse=0;
    char   szAddr[100];

    iRet = sscanf(m_szUrl, "obs://%[^:]:%d", szAddr, &iPort);
    if (iRet < 2)
        return (Error)obsError_BadUrl;

    m_hHandle = socket( AF_INET, SOCK_DGRAM, 0 );
    if (m_hHandle < 0)
       return (Error)obsError_CannotCreateSocket;

    m_pSin = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    assert(m_pSin);

    iReuse = 1;
    m_pSin->sin_family = AF_INET;
    m_pSin->sin_port = htons(iPort);
    m_pSin->sin_addr.s_addr = htonl(INADDR_ANY);

    iRet = setsockopt(m_hHandle, SOL_SOCKET, SO_REUSEADDR, 
                      &iReuse, sizeof(int));
    if (iRet < 0)
    {
       close(m_hHandle);
       m_hHandle= -1;
       return (Error)obsError_CannotSetSocketOpts;
    }

    iRet = bind(m_hHandle, (struct sockaddr *)m_pSin, 
                sizeof(struct sockaddr_in));
    if (iRet < 0)
    {
       close(m_hHandle);
       m_hHandle= -1;
       return (Error)obsError_CannotBind;
    }

    sMreq.imr_multiaddr.s_addr = inet_addr(szAddr);
    sMreq.imr_interface.s_addr = htonl(INADDR_ANY);
    iRet = setsockopt(m_hHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (char *)&sMreq, sizeof(sMreq));
    if (iRet < 0)
    {
       close(m_hHandle);
       m_hHandle= -1;
       return (Error)obsError_CannotSetSocketOpts;
    }

    // Let's make up a ficticous ID3 tag.
    if (m_pID3Tag == NULL)
        m_pID3Tag = new ID3Tag();

    memset(m_pID3Tag, 0, sizeof(ID3Tag));
    memcpy(m_pID3Tag->szTag, "TAG", 3);

    strcpy(m_pID3Tag->szTitle, "Obsequieum");

    fcntl(m_hHandle, F_SETFL, fcntl(m_hHandle, F_GETFL) | O_NONBLOCK);

    return kError_NoErr;
}

Error ObsBuffer::GetID3v1Tag(unsigned char *pTag)
{
    if (m_pID3Tag)
    {
        memcpy(pTag, m_pID3Tag, sizeof(ID3Tag));
        return kError_NoErr;
    }

    return kError_NoDataAvail;
}

Error ObsBuffer::Run(void)
{
    int iRet;

    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       if (!m_pBufferThread)
       {
           return (Error)kError_CreateThreadFailed;
       }
       m_pBufferThread->Create(ObsBuffer::StartWorkerThread, this);
    }

    return kError_NoErr;
}

void ObsBuffer::StartWorkerThread(void *pVoidBuffer)
{
   ((ObsBuffer*)pVoidBuffer)->WorkerThread();
}

#define min(a,b) ((a) < (b) ? (a) : (b))

void ObsBuffer::WorkerThread(void)
{
   size_t          iToCopy, iStructSize, iActual; 
   int             iRead, iPacketNum = -1, iCurrNum, iRet;
   RTPHeader      *pHeader;
   void           *pBuffer;
   unsigned        char *pTemp, *pCopy;
   Error           eError;
   fd_set          sSet;
   struct timeval  sTv;

   pTemp = new unsigned char[iMAX_PACKET_SIZE];
   pHeader = (RTPHeader *)pTemp;
   for(; !m_bExit;)
   {
      if (IsEndOfStream())
      {
          g_Log->Log(LogInput, "Sleeping on EOS");
          m_pWriteSem->Wait();
          continue;
      }

      sTv.tv_sec = 0;
      sTv.tv_usec = 100000;  // .1 second
      FD_ZERO(&sSet);
      FD_SET(m_hHandle, &sSet);
      iRet = select(m_hHandle + 1, &sSet, NULL, NULL, &sTv);  
      if (!iRet)
      {
         continue;
      }

      iStructSize = sizeof(struct sockaddr_in);
      iRead = recvfrom(m_hHandle, pTemp, iMAX_PACKET_SIZE, 0,
                       (struct sockaddr *)m_pSin, &iStructSize);
      if (iRead <= 0)
      {
         g_Log->Log(LogInput, "iRead is less than 0");
         SetEndOfStream(true);
         break;
      }

      for(;;)
      {
          eError = BeginWrite(pBuffer, iToCopy);
          if (eError == kError_BufferTooSmall)
          {
              m_pWriteSem->Wait();
              continue;
          }
          break;
      }
      if (eError != kError_NoErr)
         break; 

      iCurrNum = ntohs(pHeader->iFlags & 0xFFFF);
      if (iPacketNum != -1 && iPacketNum != iCurrNum - 1)
      {
          g_Log->Log(LogInput, "Lost packet (%d, %d)\n", iPacketNum, iCurrNum); 
      }
      iPacketNum = iCurrNum;

      iRead -= sizeof(RTPHeader);
      for(pCopy = pTemp + sizeof(RTPHeader); iRead > 0;)
      {
          iActual = min(iToCopy, iRead);
          memcpy(pBuffer, pCopy, iActual);
          EndWrite(iActual);

          pCopy += iActual;
          iRead -= iActual;

          if (iRead != 0)
          {
              for(; !m_bExit;)
              {   
                  eError = BeginWrite(pBuffer, iToCopy);      
                  if (eError == kError_BufferTooSmall)
                     m_pWriteSem->Wait();
                  else
                     break;
              }
          }
      }

   }

   delete pTemp;
   close(m_hHandle);
   m_hHandle = -1;
   g_Log->Log(LogInput, "Worker thread done");
}

const char *ObsBuffer::
GetErrorString(int32 error)
{
   if ((error <= obsError_MinimumError) || (error >= obsError_MaximumError))
   {
      return g_ErrorArray[0];
   }

   return g_ErrorArray[error - obsError_MinimumError];
}
