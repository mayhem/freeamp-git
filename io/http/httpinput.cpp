/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998-1999 EMusic.com

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
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <iostream.h>
#include <errno.h>
#include <assert.h>
#ifdef WIN32
#include <winsock.h>
#include <time.h>
#else
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#ifndef __BEOS__
#include <arpa/inet.h> 
#endif
#include <netdb.h>
#include <fcntl.h>
#endif 

#include "config.h"

#if HAVE_UNISTD_H
#include <unistd.h>
#elif HAVE_IO_H
#include <io.h>
#else
#error Must have unistd.h or io.h!
#endif // HAVE_UNISTD_H

/* project headers */
#include "httpinput.h"
#include "facontext.h"
#include "log.h"
#include "tstream.h"
#include "debug.h"

const int iBufferSize = 8192;
const int iOverflowSize = 1536;
const int iTriggerSize = 1024;
const char *szDefaultStreamTitle = "SHOUTcast Stream";

#if !defined(WIN32) && !defined(__BEOS__)
#define closesocket(s) close(s)
#endif

#ifndef F_OK
#define F_OK 0
#endif

const int iHttpPort = 80;
const int iMaxHostNameLen = 64;
const int iGetHostNameBuffer = 1024;
const int iBufferUpInterval = 3;
const int iInitialBufferSize = 1024;
const int iHeaderSize = 1024;
const int iICY_OK = 200;
const int iTransmitTimeout = 60;

#ifdef WIN32
const char cDirSepChar = '\\';
#else
const char cDirSepChar = '/';
#endif

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif 

extern    "C"
{
   PhysicalMediaInput *Initialize(FAContext *context)
   {
      return new HttpInput(context);
   }
}

HttpInput::HttpInput(FAContext *context):
           PhysicalMediaInput(context)
{
    uint32 len;

    m_path = NULL;
    m_hHandle = -1;
    m_bLoop = false;
    m_bDiscarded = false;
    m_bIsStreaming = true;
    m_pBufferThread = NULL;
    m_fpSave = NULL;
    m_szError = new char[iMaxErrorLen];
    m_pTitleStream = NULL;
    m_bUseBufferReduction = true;

    m_pContext->prefs->GetPrefBoolean(kUseProxyPref, &m_bUseProxy);
    if (m_bUseProxy)
    {
        m_pContext->prefs->GetPrefString(kProxyHostPref, m_szProxyHost, &len);
        if ( len == 0 )
            m_pContext->log->Error("useProxy is true but ProxyHost "
                                  "has no value ?!");
    }   
}

HttpInput::~HttpInput()
{
    m_bExit = true;
    m_pSleepSem->Signal();
    m_pPauseSem->Signal();

    if (m_pTitleStream)
       delete m_pTitleStream;

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

    delete m_szError; 
}

bool HttpInput::CanHandle(const char *szUrl, char *szTitle)
{
    bool bRet;

    bRet = strncmp(szUrl, "http://", 7) == 0;
 
    if (szTitle && bRet)
       strcpy(szTitle, szDefaultStreamTitle);

    return bRet;
}

Error HttpInput::Prepare(PullBuffer *&pBuffer)
{
    int32 iBufferSize = iDefaultBufferSize;
    Error result;

    if (m_pOutputBuffer)
    {
       delete m_pOutputBuffer;
       m_pOutputBuffer = NULL;
    }

    if (!IsError(m_pContext->prefs->GetInputBufferSize(&iBufferSize)))
       iBufferSize *= 1024;

    m_pOutputBuffer = new PullBuffer(iBufferSize, iDefaultOverflowSize,
                                     m_pContext);
    assert(m_pOutputBuffer);

    pBuffer = m_pOutputBuffer;

    result = Run();
    if (IsError(result))
    {
        ReportError("Could not initialize http streaming plugin.");
        return result;
    }

    return kError_NoErr;
} 

Error HttpInput::Close(void)
{
    delete m_pOutputBuffer;
    m_pOutputBuffer = NULL;
 
    if (m_hHandle >= 0)
    {
       shutdown(m_hHandle, 2);
       closesocket(m_hHandle);
    }

    if (m_fpSave)
       fclose(m_fpSave);  

   return kError_NoErr;
}

Error HttpInput::Run(void)
{
    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       if (!m_pBufferThread)
       {
           return (Error)kError_CreateThreadFailed;
       }
       m_pBufferThread->Create(HttpInput::StartWorkerThread, this);
    }

    return kError_NoErr;
}

bool HttpInput::PauseLoop(bool bLoop)
{
   bool bRet;

   m_bLoop = bLoop;
   bRet = m_bDiscarded;
   m_bDiscarded = false;

   return bRet;
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
    struct hostent TempHostent;
    static unsigned long IP_Adr;
    static char *AdrPtrs[2] = {(char *) &IP_Adr, NULL };

    pTemp = gethostbyname(szHostName);
    if (pTemp == NULL) 
    {
        // That didn't work.  On some stacks a numeric IP address
        // will not parse with gethostbyname.  Try to convert it as a
        // numeric address before giving up.
        if((IP_Adr = inet_addr(szHostName)) < 0) 
            return kError_NoDataAvail;

        TempHostent.h_length = sizeof(uint32);
        TempHostent.h_addrtype = AF_INET;
        TempHostent.h_addr_list = (char **) &AdrPtrs;
        pTemp = &TempHostent;
    }

    memcpy(pResult, pTemp, sizeof(struct hostent));

    return kError_NoErr;
}

Error HttpInput::Open(void)
{
    char                szHostName[iMaxHostNameLen+1], *szFile, *szQuery;
    char                szLocalName[iMaxHostNameLen+1], *pEnd;
    char               *pInitialBuffer, szSourceAddr[100];
    char               *szStreamName, *szStreamUrl;
    unsigned            iPort;
    int                 iRet, iRead = 0, iConnect;
    struct sockaddr_in  sAddr, sSourceAddr;
    struct hostent      sHost;
    Error               eRet;
    char               *pHeaderData = NULL, *pPtr;
    fd_set              sSet; 
    struct timeval      sTv;
    bool                bUseTitleStreaming = true, bUseAltNIC = false;

    szStreamName = NULL;
    szStreamUrl = NULL;
    if (!m_bUseProxy)
    {
        iRet = sscanf(m_path, "http://%[^:/]:%d", szHostName, &iPort);
        if (iRet < 1)
        {
           ReportError("Bad URL format. URL format: http:<host name>"
                       ":[port][/path]. Please check the URL and try again.");
           return (Error)httpError_BadUrl;
        }
        szFile = strchr(m_path + 7, '/');
     }
     else
     {
        iRet = sscanf(m_szProxyHost, "http://%[^:/]:%d", szHostName, &iPort);
        if (iRet < 1)
        {
           ReportError("Bad Proxy URL format. URL format: http:"
                       "<host name>:[port]. Please check your proxy settings "
                       "in the Options.");
           return (Error)httpError_BadUrl;
        }
        szFile = m_path;
     }

     if (iRet < 2)
        iPort = iHttpPort;

     memset(&sAddr, 0, sizeof(struct sockaddr_in));

     ReportStatus("Looking up host %s...", szHostName);

     eRet = GetHostByName(szHostName, &sHost);
     if (eRet != kError_NoErr)
     {
          sprintf(m_szError, "Cannot find host %s\n", szHostName);
          ReportError(m_szError);
          return (Error)httpError_CustomError;
     }

     memcpy((char *)&sAddr.sin_addr,sHost.h_addr, sHost.h_length);
     sAddr.sin_family= sHost.h_addrtype;
     sAddr.sin_port= htons((unsigned short)iPort); 

     ReportStatus("Contacting host %s...", szHostName);
     m_hHandle = socket(sHost.h_addrtype,SOCK_STREAM,0);
     if (m_hHandle < 0)
     {
          ReportError("Cannot create socket. Is TCP/IP networking properly installed?");
          return (Error)httpError_CannotOpenSocket;
     }    

     m_pContext->prefs->GetPrefBoolean(kUseAlternateNICPref, &bUseAltNIC);
     if (bUseAltNIC)
     {
         uint32 len = 100;

         m_pContext->prefs->GetPrefString(kAlternateNICAddressPref, szSourceAddr, &len);
         if ( len == 0 )
             m_pContext->log->Error("UseAlternateNIC is true but AlternateNIC "
                                    "has no value ?!");

         sSourceAddr.sin_family= AF_INET;
         sSourceAddr.sin_port = 0; 
         sSourceAddr.sin_addr.s_addr = inet_addr(szSourceAddr);
         iRet = bind(m_hHandle, (struct sockaddr *)&sSourceAddr, 
                     sizeof(struct sockaddr_in));
         if (iRet < 0)
         {
             close(m_hHandle);
             m_hHandle= -1;
             ReportError("Cannot bind the socket. Please make sure that your TCP/IP networking is correctly configured.");
             return kError_CannotBind;
         }  
     }   

#if defined(WIN32)
	unsigned long lMicrosoftSucksBalls = 1;
	ioctlsocket(m_hHandle, FIONBIO, &lMicrosoftSucksBalls);
#elif defined(__BEOS__)
//	int on = 1;
//	setsockopt( m_hHandle, SOL_SOCKET, SO_NONBLOCK, &on, sizeof( on ) );
#else
    fcntl(m_hHandle, F_SETFL, fcntl(m_hHandle, F_GETFL) | O_NONBLOCK);
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
           ReportError("Cannot connect to host: %s", szHostName);
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
        sprintf(szQuery, "GET %s HTTP/1.0\n"
                         "Host: %s\n"
                         "Accept: */*\n" 
                         "User-Agent: FreeAmp/%s\n", 
                         szFile, szLocalName, FREEAMP_VERSION);
    else
        sprintf(szQuery, "GET / HTTP/1.0\n"
                         "Host: %s\n"
                         "Accept: */*\n" 
                         "User-Agent: FreeAmp/%s\n", 
                         szLocalName, FREEAMP_VERSION);

    m_pContext->prefs->GetPrefBoolean(kUseTitleStreamingPref, &bUseTitleStreaming);
    if (bUseTitleStreaming)
    {
        int   iPort;
        Error eRet;

        m_pTitleStream = new TitleStreamServer(m_pContext, m_pTarget);

        eRet = m_pTitleStream->Init(iPort);
        if (IsntError(eRet))
        {
            sprintf(szQuery + strlen(szQuery), "x-audiocast-udpport: %d\n", 
                    iPort); 
        }
        else
        {
            delete m_pTitleStream;
            m_pTitleStream = NULL;
        }
    }

    strcat(szQuery, "\n");

    ReportStatus("Requesting stream...");

    iRet = send(m_hHandle, szQuery, strlen(szQuery), 0);
    if (iRet != (int)strlen(szQuery))
    {
        delete szQuery;
        ReportError("Cannot send data to host: %s", szHostName);
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
            ReportError("Cannot receive data from host: %s", szHostName);
            closesocket(m_hHandle);
            return (Error)httpError_SocketRead;
        }
        break;
    }
    if (m_bExit)
        return (Error)kError_Interrupt;

    if (sscanf(pInitialBuffer, " %*s %d %255[^\n\r]", &iRet, m_szError))
    {
        void *pData;
        int   iHeaderBytes = 0, iCurHeaderSize = iHeaderSize;

        if (iRet != iICY_OK)
        {
            ReportStatus("");
            ReportError("This stream is not available: %s\n", m_szError);
            
            delete pInitialBuffer;
            closesocket(m_hHandle);
            return (Error)httpError_CustomError;
        }

        pHeaderData = new char[iHeaderSize];
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

            pEnd = strstr(pHeaderData, "\r\n\r\n");
            if (pEnd)
            {    
                *(pEnd + 3) = 0;
                break;
            }    
            pEnd = strstr(pHeaderData, "\n\n");
            if (pEnd)
            {    
                *(pEnd + 1) = 0;
                break;
            }    
               
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
                    ReportError("Cannot receive data from host: %s", szHostName);
                    closesocket(m_hHandle);
                    return (Error)httpError_SocketRead;
                }
                break;
            }
            if (m_bExit)
                return (Error)kError_Interrupt;
        }

        pPtr = strstr(pHeaderData, "icy-name");
        if (pPtr)
        {
            pPtr += strlen("icy-name:");
            szStreamName = new char[strlen(pPtr) + 1];
            sscanf(pPtr, " %[^\r\n]", szStreamName);
        }

        pPtr = strstr(pHeaderData, "icy-url");
        if (pPtr)
        {
            pPtr += strlen("icy-url:");
            szStreamUrl = new char[strlen(pPtr) + 1];
            sscanf(pPtr, " %[^\r\n]", szStreamUrl);
        }
        
        // If this is a stream from a web server and not a shout/ice
        // server we don't want to use buffer reduction when the
        // input buffers fill up
        if (strstr(pHeaderData, "Server:") &&
            strstr(pHeaderData, "Date:"))
            m_bUseBufferReduction = false;
        
        if (szStreamName && strlen(szStreamName))
        {
           StreamInfoEvent *e;
           
           e = new StreamInfoEvent(szStreamName ? szStreamName : (char *)"", 
                                   szStreamUrl ? szStreamUrl : (char *)"");
           m_pTarget->AcceptEvent(e);
           delete szStreamUrl;
        }   

        pPtr = strstr(pHeaderData, "x-audiocast-udpport:");
        if (pPtr)
        {
             if (m_pTitleStream)
                m_pTitleStream->Run(sAddr.sin_addr, atoi(pPtr + 20));
        }

        // Let's save the bytes we've read into the pullbuffer.
        iRead = iHeaderBytes - strlen(pHeaderData) - 1;
        if (iRead > 0)
        {
            m_pOutputBuffer->BeginWrite(pData, iRead);
            memcpy(pData, (char *)pHeaderData + strlen(pHeaderData) + 1, iRead);
            m_pOutputBuffer->EndWrite(iRead);
        }    
    }

    delete pInitialBuffer;

    bool bSave;
    uint32  size = 255;
    m_pContext->prefs->GetPrefBoolean(kSaveStreamsPref, &bSave);
    if (bSave || (m_pContext->argFlags & FAC_ARGFLAGS_SAVE_STREAMS))
    {
        char szPath[255], szFile[255];
        unsigned i;
					
        if (szStreamName == NULL)
        {
           szStreamName = new char[255];
           sprintf(szStreamName, "%s:%d", szHostName, iPort);
        }   

        for(i = 0; i < strlen(szStreamName); i++)
           if (strchr("\\/?*{}[]()*|:<>\"'", szStreamName[i]))
               szStreamName[i] = '-';

        if (m_pContext->prefs->GetPrefString(kSaveStreamsDirPref, szPath, &size) == 
            kError_NoPrefValue)
           strcpy(szPath, ".");
        if (szPath[strlen(szPath) - 1] == cDirSepChar)
            szPath[strlen(szPath) - 1]  = 0;

        for(i = 0;; i++)
        {
            if (!i)
                sprintf(szFile, "%s%c%s.mp3", szPath, cDirSepChar, szStreamName);
            else
                sprintf(szFile, "%s%c%s-%d.mp3", szPath, cDirSepChar, szStreamName, i);

            if (access(szFile, F_OK))
                break;
        }

        m_fpSave = fopen(szFile, "wb");
        
        if (pHeaderData)
        {
            iRet = fwrite((char *)pHeaderData + strlen(pHeaderData) + 1, 
                          sizeof(char), iRead, m_fpSave);
            if (iRet != iRead)
            {
               delete pHeaderData;
               ReportError("Cannot save http stream to disk. Disk full?");
               return kError_WriteFile;
            }
        }    
    }
    delete pHeaderData;
    delete szStreamName;

    ReportStatus("");

    return kError_NoErr;
}

void HttpInput::StartWorkerThread(void *pVoidBuffer)
{
   ((HttpInput*)pVoidBuffer)->WorkerThread();
}

void HttpInput::WorkerThread(void)
{
   int             iRead, iRet, iReadSize = 1024;
   void           *pBuffer;
   Error           eError;
   fd_set          sSet;
   struct timeval  sTv;

   static int      iSize = 0;

   eError = Open();
   if (IsError(eError) || m_bExit)
   {
      return;
   }   

   m_pSleepSem->Wait(); 

   for(; !m_bExit;)
   {
      if (m_pOutputBuffer->IsEndOfStream())
      {
          m_pSleepSem->Wait();
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
        
      eError = m_pOutputBuffer->BeginWrite(pBuffer, iReadSize);
      if (eError == kError_NoErr)
      {
          iRead = recv(m_hHandle, (char *)pBuffer, iReadSize, 0);
          if (iRead <= 0)
          {
             m_pOutputBuffer->SetEndOfStream(true);
             m_pOutputBuffer->EndWrite(0);
             break;
          }
          iSize += iRead;

          if (m_fpSave)
          {
             iRet = fwrite(pBuffer, sizeof(char), iRead, m_fpSave);
             if (iRet != iRead)
             {
                 ReportError("Cannot save http stream to disk. Disk full?");
                 break;
             }
          }
          
          eError = m_pOutputBuffer->EndWrite(iRead);
          if (IsError(eError))
          {
              m_pContext->log->Error("http: EndWrite returned: %d\n", eError);
              break;
          }
      }
      if (eError == kError_BufferTooSmall)
      {
          if (m_bLoop)
          {
             m_pOutputBuffer->DiscardBytes();
             m_bDiscarded = true;
          }
          else
          {
             iSize = 0;
             m_pSleepSem->Wait();
          }
          continue;
      }
   }

   shutdown(m_hHandle, 2);
   closesocket(m_hHandle);
   m_hHandle = -1;
}
