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

#include "config.h"
#include "httpbuffer.h"
#include "httpinput.h"
#include "facontext.h"
#include "log.h"

#ifndef WIN32
#define closesocket(s) close(s)
#endif

const int iHttpPort = 80;
const int iMaxHostNameLen = 64;
const int iGetHostNameBuffer = 1024;
const int iBufferUpInterval = 3;
const int iInitialBufferSize = 1024;
const int iHeaderSize = 1024;
const int iICY_OK = 200;
const int iTransmitTimeout = 60;

#define DB printf("%s:%d\n", __FILE__, __LINE__);

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

HttpBuffer::HttpBuffer(size_t iBufferSize, size_t iOverFlowSize, 
                       size_t iWriteTriggerSize, char *szFile, 
                       HttpInput *pHttp, FAContext *context) :
          StreamBuffer(iBufferSize, iOverFlowSize, iWriteTriggerSize, context)
{
    m_hHandle = -1;
    m_bLoop = false;
    m_pBufferThread = NULL;
    m_pID3Tag = NULL;
	 m_szError = new char[iMaxErrorLen];
    m_pHttp = pHttp;
    m_fpSave = NULL;

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

    if (m_fpSave)
       fclose(m_fpSave);

    if (m_pID3Tag)
       delete m_pID3Tag;

    delete m_szError;
}

void HttpBuffer::LogError(char *szErrorMsg)
{
#ifdef WIN32
	char *lpMessageBuffer;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
                  WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMessageBuffer, 0, NULL );

    sprintf(m_szError, "%s: %s", szErrorMsg, lpMessageBuffer);
	LocalFree(lpMessageBuffer);

#else
    sprintf(m_szError, "%s: %s", szErrorMsg, strerror(errno));
#endif
    m_pHttp->ReportError(m_szError);
    m_context->log->Error("%s\n", m_szError);
}

// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
// The function gethostbyname_r() differs greatly from system to system
// and on linux it seems to behave quite erratically. I've elected to
// use gethostbyname() (the non-reentrant version) because it works,
// and I don't see two threads conflicting each other during a gethostbyname
// lookup in FreeAmp.
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
Error GetHostByName(char *szHostName, struct hostent *pResult)
{
    struct hostent *pTemp;

    pTemp = gethostbyname(szHostName);
    if (pTemp == NULL)
        return kError_NoDataAvail;

    memcpy(pResult, pTemp, sizeof(struct hostent));

    return kError_NoErr;
}

Error HttpBuffer::Open(void)
{
    char                szHostName[iMaxHostNameLen+1], *szFile, *szQuery;
    char                szLocalName[iMaxHostNameLen+1];
    char               *pInitialBuffer, szStreamName[255];
    unsigned            iPort;
    int                 iRet, iRead, iConnect;
    struct sockaddr_in  sAddr;
    struct hostent      sHost;
    Error               eRet;
    void               *pPtr;
    fd_set              sSet; 
    struct timeval      sTv;

    szStreamName[0] = 0;

    iRet = sscanf(m_szUrl, "http://%[^:/]:%d", szHostName, &iPort);
    if (iRet < 1)
    {
        m_pHttp->ReportError("Bad URL format. URL format: http:<host name>"
                             ":[port][/path]");
        m_context->log->Error("Badly formatted URL: %s\n", m_szUrl);
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
       m_context->log->Error("Cannot find host %s\n", szHostName);
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
    iConnect = connect(m_hHandle,(const sockaddr *)&sAddr,sizeof(sAddr));
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

    gethostname(szLocalName, iMaxHostNameLen);    
    szQuery = new char[iMaxUrlLen];

    if (szFile)
        sprintf(szQuery, "GET %s HTTP/1.0\n\n", szFile);
    else
        sprintf(szQuery, "GET / HTTP/1.0\nHost %s\nAccept: */*\n\n", 
                szLocalName);

    iRet = send(m_hHandle, szQuery, strlen(szQuery), 0);
    if (iRet != (int)strlen(szQuery))
    {
		delete szQuery;
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

    if (sscanf(pInitialBuffer, " ICY %d %255[^\n\r]", &iRet, m_szError) ||
        sscanf(pInitialBuffer, " HTTP/1.0 %d %255[^\n\r]", &iRet, m_szError))
    {
        char *pHeaderData, *pPtr;
        int   iHeaderBytes = 0, iCurHeaderSize = iHeaderSize;

		  if (iRet != iICY_OK)
		  {
            char szErr[255];

            sprintf(szErr, "This stream is not available: %s\n", m_szError);
            m_context->log->Error(szErr);
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

            if (strstr(pHeaderData, "\r\n\r\n") ||
                strstr(pHeaderData, "\n\n"))
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
            sscanf(pPtr, "%254[^\r\n]", szStreamName);
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
    }

    delete pInitialBuffer;

    bool bSave;
    unsigned  size = 255;
    m_context->prefs->GetPrefBoolean(kHTTPStreamSave, &bSave);
    if (bSave)
    {
        char szPath[255], szFile[255];

        for(int i = 0; i < strlen(szStreamName); i++)
           if (strchr("\\/?*{}[]()*|<>\"'", szStreamName[i]))
               szStreamName[i] = '-';

        if (szStreamName[0] == 0)
           sprintf(szStreamName, "%s:%d", szHostName, iPort);

        if (m_context->prefs->GetPrefString(kHTTPStreamSaveDir, szPath, &size) == 
            kError_NoPrefValue)
           strcpy(szPath, ".");
        if (szPath[strlen(szPath) - 1] == '/' ||
            szPath[strlen(szPath) - 1] == '\\')
            szPath[strlen(szPath) - 1]  = 0;

        for(int i = 0;; i++)
        {
            if (!i)
                sprintf(szFile, "%s/%s.mp3", szPath, szStreamName);
            else
                sprintf(szFile, "%s/%s-%d.mp3", szPath, szStreamName, i);

            if (access(szFile, F_OK))
                break;
        }

        m_fpSave = fopen(szFile, "wb");
    }

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

          if (m_fpSave)
          {
             iRet = fwrite(pBuffer, sizeof(char), iRead, m_fpSave);
             if (iRet != iRead)
             {
                 m_pHttp->ReportError("Cannot save http stream to disk. Disk full?");
                 break;
             }
          }

          //for(int i = 0; i<20; i++)
          //   printf("%02X ", ((char*)pBuffer)[i] & 0xFF);
          //printf(" (%d - %d)\n", iToCopy, iReadSize);

          eError = EndWrite(iRead);
          if (IsError(eError))
          {
              m_context->log->Error("http: EndWrite returned: %d\n", eError);
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
