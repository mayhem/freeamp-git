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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "httpbuffer.h"

const int iHttpPort = 80;
const int iMaxHostNameLen = 64;
const int iGetHostNameBuffer = 1024;
const int iBufferUpInterval = 3;
const int iInitialBufferSize = 64;
const int iHeaderSize = 1024;

#define DB printf("%s:%d\n", __FILE__, __LINE__);

static char *g_ErrorArray[9] =
{
   "Invalid URL. URL format: http://<host>[:port][/path]",
   "Cannot find host.",
   "Cannot open socket.",
   "Cannot connect to host.",
   "Failed to read data from socket."
};

HttpBuffer::HttpBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                       size_t iWriteTriggerSize, char *szFile) :
            PullBuffer(iBufferSize, iOverFlowSize, iWriteTriggerSize)
{
    m_hHandle = -1;
    m_bLoop = false;
    m_bExit = false;
    m_pBufferThread = NULL;
    m_pID3Tag = NULL;

    strcpy(m_szUrl, szFile);
}

HttpBuffer::~HttpBuffer(void)
{
    m_bExit = true;
    m_pWriteSem->Signal();

    if (m_pBufferThread)
    {
       m_pBufferThread->Join();
       delete m_pBufferThread;
    }

    if (m_hHandle >= 0)
       close(m_hHandle);

    if (m_pID3Tag)
       delete m_pID3Tag;
}

Error HttpBuffer::Open(void)
{
    char                szHostName[iMaxHostNameLen+1], *szFile, *szQuery;
    char               *pDummyBuf, *pInitialBuffer;
    unsigned            iPort;
    int                 iRet, iErrno, iDummy, iRead, iLoop;
    struct sockaddr_in  sAddr;
    struct hostent      sHost, *pRes;
    Error               eRet;
    void               *pPtr;
    size_t              iSize;

    iRet = sscanf(m_szUrl, "http://%[^:/]:%d", szHostName, &iPort);
    if (iRet < 1)
        return (Error)httpError_BadUrl;

    if (iRet < 2)
       iPort = iHttpPort;

    szFile = strchr(m_szUrl + 7, '/');
    memset(&sAddr, 0, sizeof(struct sockaddr_in));

#ifndef USING_GDB

    // Does anyone know how to use gethostbyname_r properly?
    // What is the proper size for dummybuf?
    pDummyBuf = new char[iGetHostNameBuffer]; 
    iRet = gethostbyname_r(szHostName, &sHost, pDummyBuf, 
                           iGetHostNameBuffer, &pRes, &iErrno);
    if (iRet < 0)
    {
       // For some reason error reporting on the pmi does not work.
       // When it does work, remove the following print
       printf("Cannot find host %s\n", szHostName);
       return (Error)httpError_GetHostByNameFailed;
    }

    memcpy((char *)&sAddr.sin_addr,sHost.h_addr, sHost.h_length);
    sAddr.sin_family= sHost.h_addrtype;
    sAddr.sin_port= htons((unsigned short)iPort);

    delete pDummyBuf;

    m_hHandle = socket(sHost.h_addrtype,SOCK_STREAM,0);

#else

    // That IP resolves to moon.eorbit.net for testing purposes
    int dum = 0x2D4BBECE;
    sAddr.sin_family= AF_INET;
    memcpy((char *)&sAddr.sin_addr,&dum, sizeof(dum));
    sAddr.sin_port= htons((unsigned short)iPort);

    m_hHandle = socket(AF_INET,SOCK_STREAM,0);
#endif

    if (m_hHandle < 0)
    {
       return (Error)httpError_CannotOpenSocket;
    }

    if (connect(m_hHandle,(const sockaddr *)&sAddr,sizeof sAddr) < 0)
    { 
       close(m_hHandle);
       return (Error)httpError_CannotConnect;
    }

    gethostname(szHostName, iMaxHostNameLen);    
    szQuery = new char[iMaxUrlLen];

    if (szFile)
        sprintf(szQuery, "GET %s HTTP/1.0\n\n", szFile);
    else
        sprintf(szQuery, "GET / HTTP/1.0\nHost %s\nAccept: */*\n\n\n", 
                szHostName);

    send(m_hHandle, szQuery, strlen(szQuery), 0);
    delete szQuery;

    pInitialBuffer = new char[iInitialBufferSize + 1];
    
    iRead = read(m_hHandle, pInitialBuffer, iInitialBufferSize);
    if (iRead < 0)
    {
       close(m_hHandle);
       return (Error)httpError_SocketRead;
    }

    if (sscanf(pInitialBuffer, " ICY %d", &iRet))
    {
        char *pStreamBegin, *pHeaderData, *pPtr;
        int   iHeaderBytes = 0, iCurHeaderSize = iHeaderSize;

        pHeaderData = new char[iHeaderSize];

        // This is a SHOUTcast stream
        for(;;)
        {
            if (iHeaderBytes + iRead > iCurHeaderSize)
            {
                char *pNew;

                iCurHeaderSize += iHeaderSize;
                pNew = new char[iCurHeaderSize];
                memcpy(pNew, pHeaderData, iHeaderBytes);
                delete pHeaderData;
                pHeaderData = pNew;
            }

            memcpy(pHeaderData + iHeaderBytes, pInitialBuffer, iRead);
            iHeaderBytes += iRead;

            for(iLoop = 0, pStreamBegin = pHeaderData; 
                iLoop < iHeaderBytes; pStreamBegin++, iLoop++)
            {
                if (strncmp(pStreamBegin, "\r\n\r\n", 4) == 0)
                {
                    break;
                }
            }
 
            if (iLoop < iHeaderBytes)
               break;
               
            iRead = read(m_hHandle, pInitialBuffer, iInitialBufferSize);
            if (iRead < 0)
            {
               close(m_hHandle);
               return (Error)httpError_SocketRead;
            }
        }

        // Let's make up a ficticous ID3 tag.
        if (m_pID3Tag == NULL)
            m_pID3Tag = new ID3Tag();

        memset(m_pID3Tag, 0, sizeof(ID3Tag));
        memcpy(m_pID3Tag->szTag, "TAG", 3);
       
        pPtr = strstr(pHeaderData, "icy-name");
        if (pPtr)
        {
            pPtr += strlen("icy-name:");
            sscanf(pPtr, "%30[^\r\n]", m_pID3Tag->szTitle);
        }

        pPtr = strstr(pHeaderData, "icy-url");
        if (pPtr)
        {
            pPtr += strlen("icy-url:");
            sscanf(pPtr, "%30[^\r\n]", m_pID3Tag->szArtist);
        }

        delete pHeaderData;

        // Don't bother saving the beginning of the frame -- the
        // data in a shoutcast stream does not necessarily start on
        // a frame boundary. Adding extra logic for the case that
        // the stream does start on a boundary doesn't make sense.
    }
    else
    {
        // Its a regular HTTP download. Let's save the bytes we've
        // read into the pullbuffer.

        BeginWrite(pPtr, iSize);
        assert(iSize > iInitialBufferSize);
        memcpy(pPtr, pInitialBuffer, iInitialBufferSize);
        EndWrite(iInitialBufferSize);
    }

    delete pInitialBuffer;

    return kError_NoErr;
}

bool HttpBuffer::GetID3v1Tag(unsigned char *pTag)
{
    if (m_pID3Tag)
    {
        memcpy(pTag, m_pID3Tag, sizeof(ID3Tag));
        return true;
    }

    return false;
}

Error HttpBuffer::Run(void)
{
    int iRet;

    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       if (!m_pBufferThread)
       {
           return (Error)kError_CreateThreadFailed;
       }
       m_pBufferThread->Create(HttpBuffer::StartWorkerThread, this);
    }

    return kError_NoErr;
}

void HttpBuffer::StartWorkerThread(void *pVoidBuffer)
{
   ((HttpBuffer*)pVoidBuffer)->WorkerThread();
}

void HttpBuffer::WorkerThread(void)
{
   size_t  iToCopy, iRead; 
   void   *pBuffer;
   Error   eError;

   for(; !m_bExit;)
   {
      if (IsEndOfStream())
      {
          m_pWriteSem->Wait();
          continue;
      }
          
      eError = BeginWrite(pBuffer, iToCopy);
      if (eError == kError_NoErr)
      {
          iRead = recv(m_hHandle, (unsigned char *)pBuffer, iToCopy, 0);
          if (iRead <= 0)
          {
             SetEndOfStream(true);
             EndWrite(0);
             break;
          }

          EndWrite(iRead);
      }
      if (eError == kError_BufferTooSmall)
      {
          m_pWriteSem->Wait();
      }
   }

   close(m_hHandle);
   m_hHandle = -1;
}


Error HttpBuffer::BeginRead(void *&pBuffer, size_t &iBytesNeeded)
{
   if (IsEndOfStream() && GetNumBytesInBuffer_i() < iBytesNeeded)
       return kError_InputUnsuccessful;

   if (GetNumBytesInBuffer_i() < iBytesNeeded)
   {
      printf("Buffering up, please wait...\r");
      fflush(stdout);
      while(GetNumBytesInBuffer_i() < (GetBufferSize_i() >> 1) && !m_bExit)
      {
          sleep(1);
      }
      printf("                            \r");
      fflush(stdout);
   }
   if (m_bExit)
       return kError_InputUnsuccessful;

   return PullBuffer::BeginRead(pBuffer, iBytesNeeded);
}

const char *HttpBuffer::
GetErrorString(int32 error)
{
   if ((error <= httpError_MinimumError) || (error >= httpError_MaximumError))
   {
      return g_ErrorArray[0];
   }

   return g_ErrorArray[error - httpError_MinimumError];
}
