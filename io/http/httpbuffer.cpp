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
#include <errno.h>
#ifdef WIN32
#include <winsock.h>
#include <time.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#endif

#include "httpbuffer.h"
#include "httpinput.h"
#include "log.h"

#ifndef WIN32
#define closesocket(s) close(s)
#endif

extern LogFile *g_Log;

const int iHttpPort = 80;
const int iMaxHostNameLen = 64;
const int iGetHostNameBuffer = 1024;
const int iBufferUpInterval = 3;
const int iInitialBufferSize = 128;
const int iHeaderSize = 1024;
const int iICY_OK = 200;
const int iTransmitTimeout = 60;

#define DB printf("%s:%d\n", __FILE__, __LINE__);

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

HttpBuffer::HttpBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                       size_t iWriteTriggerSize, char *szFile, 
                       HttpInput *pHttp) :
          StreamBuffer(iBufferSize, iOverFlowSize, iWriteTriggerSize)
{
    m_hHandle = -1;
    m_bLoop = false;
    m_pBufferThread = NULL;
    m_pID3Tag = NULL;
	 m_szError = new char[iMaxErrorLen];
    m_pHttp = pHttp;

    strcpy(m_szUrl, szFile);
}

HttpBuffer::~HttpBuffer(void)
{
    m_bExit = true;
    m_pWriteSem->Signal();
    m_pReadSem->Signal();

    if (m_pBufferThread)
    {
       m_pBufferThread->Join();
       delete m_pBufferThread;
    }

    if (m_hHandle >= 0)
    {
       shutdown(m_hHandle, 2);
       closesocket(m_hHandle);
    }

    if (m_pID3Tag)
       delete m_pID3Tag;

    delete m_szError;
}

void HttpBuffer::LogError(char *szErrorMsg)
{
#ifdef WIN32
    sprintf(m_szError, "%s: %s", szErrorMsg, WSAGetLastError());
#else
    sprintf(m_szError, "%s: %s", szErrorMsg, strerror(errno));
#endif
    m_pHttp->ReportError(m_szError);
    g_Log->Error("%s\n", m_szError);
}

// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
// As far as I can tell there are many different versions of gethostbyname_r
// out there. If this code does not compile for your platform, please
// fix one of the following sections so that it will work on your
// system. (without breaking the existing systems, please!)
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
Error GetHostByName(char *szHostName, struct hostent *pResult)
{
#ifdef WIN32
    struct hostent *pTemp;

    pTemp = gethostbyname(szHostName);
    if (pTemp == NULL)
        return kError_NoDataAvail;

    memcpy(pResult, pTemp, sizeof(struct hostent));

    return kError_NoErr;
#endif

#ifdef __linux__
    char               *pDummyBuf;
    struct hostent     *pRes;
    int                 iErrno, iRet;

    pDummyBuf = new char[iGetHostNameBuffer]; 
    iRet = gethostbyname_r(szHostName, pResult, pDummyBuf, 
                           iGetHostNameBuffer, &pRes, &iErrno);
    delete pDummyBuf;

    return (iRet < 0) ? kError_NoDataAvail : kError_NoErr;
#endif

#ifdef __solaris__
    char               *pDummyBuf;
    struct hostent     *pRes;
    int                 iErrno, iRet;

    pDummyBuf = new char[iGetHostNameBuffer]; 
    pRes = gethostbyname_r(szHostName, pResult, pDummyBuf, 
                           iGetHostNameBuffer, &iErrno);
    delete pDummyBuf;

    return (pRes == NULL) ? kError_NoDataAvail : kError_NoErr;
#endif
}
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
// Please read the note on top of this function
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE

Error HttpBuffer::Open(void)
{
    char                szHostName[iMaxHostNameLen+1], *szFile, *szQuery;
    char               *pInitialBuffer;
    unsigned            iPort;
    int                 iRet, iRead, iLoop, iConnect;
    struct sockaddr_in  sAddr;
    struct hostent      sHost;
    Error               eRet;
    void               *pPtr;
    fd_set              sSet; 
    struct timeval      sTv;

    iRet = sscanf(m_szUrl, "http://%[^:/]:%d", szHostName, &iPort);
    if (iRet < 1)
    {
        m_pHttp->ReportError("Bad URL format. URL format: http:<host name>"
                             ":[port][/path]");
        g_Log->Error("Badly formatted URL: %s\n", m_szUrl);
        return (Error)httpError_BadUrl;
    }

    if (iRet < 2)
       iPort = iHttpPort;

    szFile = strchr(m_szUrl + 7, '/');
    memset(&sAddr, 0, sizeof(struct sockaddr_in));

    eRet = GetHostByName(szHostName, &sHost);
    if (eRet != kError_NoErr)
    {
       sprintf(m_szError, "Cannot find host %s\n", szHostName);
       m_pHttp->ReportError(m_szError);
       g_Log->Error("Cannot find host %s\n", szHostName);
       return (Error)httpError_CustomError;
    }

    memcpy((char *)&sAddr.sin_addr,sHost.h_addr, sHost.h_length);
    sAddr.sin_family= sHost.h_addrtype;
    sAddr.sin_port= htons((unsigned short)iPort);

    m_hHandle = socket(sHost.h_addrtype,SOCK_STREAM,0);
    if (m_hHandle < 0)
    {
       LogError("Cannot create socket");
       return (Error)httpError_CannotOpenSocket;
    }

#ifndef WIN32
    fcntl(m_hHandle, F_SETFL, fcntl(m_hHandle, F_GETFL) | O_NONBLOCK);
#else
	unsigned long lMicrosoftSucksBalls = 1;
	ioctlsocket(m_hHandle, FIONBIO, &lMicrosoftSucksBalls);
#endif
    iConnect = connect(m_hHandle,(const sockaddr *)&sAddr,sizeof sAddr);
    for(; iConnect && !m_bExit;)
    {
        sTv.tv_sec = 0; sTv.tv_usec = 0;
        FD_ZERO(&sSet); FD_SET(m_hHandle, &sSet);
        iRet = select(m_hHandle + 1, NULL, &sSet, NULL, &sTv);
        if (!iRet)
        {
		   usleep(100000);
           continue;
        }
        if (iRet < 0)
        { 
           LogError("Cannot connect socket");
           closesocket(m_hHandle);
           return (Error)httpError_CannotConnect;
        }
        break;
    }
    if (m_bExit)
        return (Error)kError_Interrupt;

    gethostname(szHostName, iMaxHostNameLen);    
    szQuery = new char[iMaxUrlLen];

    if (szFile)
        sprintf(szQuery, "GET %s HTTP/1.0\n\n", szFile);
    else
        sprintf(szQuery, "GET / HTTP/1.0\nHost %s\nAccept: */*\n\n\n", 
                szHostName);

    iRet = send(m_hHandle, szQuery, strlen(szQuery), 0);
    if (iRet != strlen(szQuery))
    {
        LogError("Cannot write to socket");
        closesocket(m_hHandle);
        return (Error)httpError_SocketWrite;
    }

    delete szQuery;

    pInitialBuffer = new char[iInitialBufferSize + 1];

    for(;!m_bExit;)
    {
        sTv.tv_sec = 0; sTv.tv_usec = 0;
        FD_ZERO(&sSet); FD_SET(m_hHandle, &sSet);
        iRet = select(m_hHandle + 1, &sSet, NULL, NULL, &sTv);
        if (!iRet)
        {
		   usleep(10000);
           continue;
        }
        iRead = recv(m_hHandle, pInitialBuffer, iInitialBufferSize, 0);
        if (iRead < 0)
        {
            LogError("Cannot read from socket");
            closesocket(m_hHandle);
            return (Error)httpError_SocketRead;
        }
        break;
    }
    if (m_bExit)
        return (Error)kError_Interrupt;

    if (sscanf(pInitialBuffer, " ICY %d %255[^\n\r]", &iRet, m_szError))
    {
        char *pStreamBegin, *pHeaderData, *pPtr;
        int   iHeaderBytes = 0, iCurHeaderSize = iHeaderSize;

		  if (iRet != iICY_OK)
		  {
            char szErr[255];

            sprintf(szErr, "This stream is not available: %s\n", m_szError);
            g_Log->Error(szErr);
            m_pHttp->ReportError(szErr);

		      delete pInitialBuffer;
		      closesocket(m_hHandle);
				return (Error)httpError_CustomError;
		  }

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
                    pHeaderData[iLoop + 2] = 0; 
                    break;
                }
            }
 
            if (iLoop < iHeaderBytes)
               break;
               
            for(;!m_bExit;)
            {
                sTv.tv_sec = 0; sTv.tv_usec = 0;
                FD_ZERO(&sSet); FD_SET(m_hHandle, &sSet);
                iRet = select(m_hHandle + 1, &sSet, NULL, NULL, &sTv);
                if (!iRet)
                {
				   usleep(10000);
                   continue;
                }
                iRead = recv(m_hHandle, pInitialBuffer, iInitialBufferSize, 0);
                if (iRead < 0)
                {
                    LogError("Cannot read from socket");
                    closesocket(m_hHandle);
                    return (Error)httpError_SocketRead;
                }
                break;
            }
            if (m_bExit)
                return (Error)kError_Interrupt;
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
        unsigned int iSize;

        // Its a regular HTTP download. Let's save the bytes we've
        // read into the pullbuffer.

        // Set the pull buffer to not keep streaming while we're
		  // not playing. Otherwise portions (if not all the) MP3
		  // file will be consumed before the user can hit play.
        Resume();

        iSize = iRead;
        BeginWrite(pPtr, iSize);
        memcpy(pPtr, pInitialBuffer, iRead);
        EndWrite(iRead);

        //for(int i = 0; i < iRead; i++)
        //   printf("%02X ", pInitialBuffer[i] & 0xFF);
        //printf("\n");
        //printf("%s\n", pInitialBuffer);
    }

    delete pInitialBuffer;

    return kError_NoErr;
}

Error HttpBuffer::GetID3v1Tag(unsigned char *pTag)
{
    if (m_pID3Tag)
    {
        memcpy(pTag, m_pID3Tag, sizeof(ID3Tag));
        return kError_NoErr;
    }

    return kError_NoDataAvail;
}

Error HttpBuffer::Run(void)
{
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
   size_t          iToCopy; 
   int             iRead, iRet, iReadSize = 1024;
   void           *pBuffer;
   Error           eError;
   fd_set          sSet;
   struct timeval  sTv;

   FD_ZERO(&sSet);

   for(; !m_bExit;)
   {
      if (IsEndOfStream())
      {
          m_pWriteSem->Wait();
          continue;
      }

      sTv.tv_sec = 0; sTv.tv_usec = 0;
      FD_ZERO(&sSet); FD_SET(m_hHandle, &sSet);
      iRet = select(m_hHandle + 1, &sSet, NULL, NULL, &sTv);
      if (!iRet)
      {
		 usleep(10000);
         continue;
      }
        
      iToCopy = iReadSize;
      eError = BeginWrite(pBuffer, iToCopy);
      if (eError == kError_NoErr)
      {
          iRead = recv(m_hHandle, (char *)pBuffer, iToCopy, 0);
          if (iRead <= 0)
          {
             SetEndOfStream(true);
             EndWrite(0);
             break;
          }

          //for(int i = 0; i<20; i++)
          //   printf("%02X ", ((char*)pBuffer)[i] & 0xFF);
          //printf(" (%d - %d)\n", iToCopy, iReadSize);

          eError = EndWrite(iRead);
          if (IsError(eError))
          {
              g_Log->Error("http: EndWrite returned: %d\n", eError);
              break;
          }
      }
      if (eError == kError_BufferTooSmall)
      {
          m_pWriteSem->Wait();
      }
   }

   shutdown(m_hHandle, 2);
   closesocket(m_hHandle);
   m_hHandle = -1;
}
