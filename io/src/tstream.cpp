/*____________________________________________________________________________
   
   FreeAmp - The Free MP3 Player

   Copyright (C) 1998 GoodNoise
   Copyright (C) 1999 Alexander Haväng <eel@musiknet.se>

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
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif  

/* project headers */
#include <config.h>
#include "tstream.h"
#include "facontext.h"
#include "log.h" 
#include "eventdata.h" 

#ifndef WIN32
#define closesocket(s) close(s)
#endif  

#define DB printf("%s:%d\n", __FILE__, __LINE__); 

TitleStreamServer::TitleStreamServer(FAContext *context,
                                     EventQueue *target)
{
   m_pTarget = target;
   m_pContext = context;

   m_bExit = false;
   m_bUseMulticast = false;
   m_pBufferThread = NULL;
   m_pSin = NULL;
}

TitleStreamServer::~TitleStreamServer()
{
   m_bExit = true;

   if (m_pBufferThread)
   {
       m_pBufferThread->Join();
       delete m_pBufferThread;
   }    

   if (m_pSin)
       free(m_pSin);
}

Error TitleStreamServer::Init(int &iPort)
{
   struct sockaddr_in sin;
   socklen_t sinlen = sizeof(struct sockaddr_in);
   int       port, startport = 10000;

   if ((m_hHandle = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      return kError_UnknownErr;
   }
   else
   {
      memset(&sin, 0, sinlen);
      sin.sin_family = AF_INET;
      sin.sin_addr.s_addr = htonl(INADDR_ANY);

      for (port = startport; port < 32767; port++)
      {
         sin.sin_port = htons(port);

         if (bind(m_hHandle, (struct sockaddr *) &sin, sinlen) < 0)
            continue;
         else
         {
            iPort = port;
            return kError_NoErr;
         }
      }

      close(m_hHandle);
      return kError_UnknownErr;
   }
}

Error TitleStreamServer::MulticastInit(char *szAddr, int iPort)
{
    int    iRet;
    struct ip_mreq sMreq;
    int    iReuse=0;
    char   szSourceAddr[100];

    m_bUseMulticast = true;

    m_hHandle = socket( AF_INET, SOCK_DGRAM, 0 );
    if (m_hHandle < 0)
    {
       return kError_CantCreateSocket;
    }

    m_pSin = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    assert(m_pSin);

    iReuse = 1;
    m_pSin->sin_family = AF_INET;
    m_pSin->sin_port = htons(iPort);
    m_pSin->sin_addr.s_addr = inet_addr(szAddr);

    iRet = setsockopt(m_hHandle, SOL_SOCKET, SO_REUSEADDR, 
                      (const char *)&iReuse, sizeof(int));
    if (iRet < 0)
    {
       close(m_hHandle);
       m_hHandle= -1;
       return kError_CannotSetSocketOpts;
    }

    iRet = bind(m_hHandle, (struct sockaddr *)m_pSin, 
                sizeof(struct sockaddr_in));
    if (iRet < 0)
    {
       close(m_hHandle);
       m_hHandle= -1;
       return kError_CannotBind;
    }

    sMreq.imr_multiaddr.s_addr = inet_addr(szAddr);
    sMreq.imr_interface.s_addr = htonl(INADDR_ANY);
    iRet = setsockopt(m_hHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                   (char *)&sMreq, sizeof(sMreq));
    if (iRet < 0)
    {
       close(m_hHandle);
       m_hHandle= -1;
       return kError_CannotSetSocketOpts;
    }

    return kError_NoErr;
}

Error TitleStreamServer::Run(in_addr &sAddr, int iPort)
{
    struct sockaddr_in sin;
    
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr = sAddr;
    sin.sin_port = htons(iPort);
   
    if (connect(m_hHandle, (struct sockaddr *) &sin, sizeof(sin)) < 0)
    {
        close(m_hHandle);
        return kError_ConnectFailed;
    }

    return Run();
}

Error TitleStreamServer::Run(void)
{
    if (!m_pBufferThread)
    {
       m_pBufferThread = Thread::CreateThread();
       if (!m_pBufferThread)
       {
           return (Error)kError_CreateThreadFailed;
       }
       m_pBufferThread->Create(TitleStreamServer::StartWorkerThread, this);
    }

    return kError_NoErr; 
}

void TitleStreamServer::StartWorkerThread(void *pVoidBuffer)
{
   ((TitleStreamServer*)pVoidBuffer)->WorkerThread();
} 

void TitleStreamServer::WorkerThread(void)
{
   char            buf[256];
   char           *ptr, *szTitle = NULL, *szURL = NULL;
   fd_set          sSet; 
   struct timeval  sTv; 
   int             iRet, iStructSize;

   for(; !m_bExit; ) 
   {
      sTv.tv_sec = 0; sTv.tv_usec = 0;
      FD_ZERO(&sSet); FD_SET(m_hHandle, &sSet);
      iRet = select(m_hHandle + 1, &sSet, NULL, NULL, &sTv);
      if (!iRet)
      {
         usleep(10000);
         continue;
      } 

      if (m_bUseMulticast)
      {
          iStructSize = sizeof(struct sockaddr_in);
          iRet = recvfrom(m_hHandle, buf, 255, 0, (struct sockaddr *)m_pSin, 
                          (socklen_t *)&iStructSize);
      }
      else
          iRet = recv(m_hHandle, buf, 255, 0);
      if (iRet > 0)
      {
         buf[iRet] = 0;
         if (strstr(buf, "x-audiocast-streamtitle") != NULL)
         {
            ptr = strchr(buf, ':');
            if (!ptr)
               continue;

            szTitle = strdup(ptr + 2);
            szTitle[strlen(szTitle) - 1] = 0;
         }
         if (strstr(buf, "x-audiocast-streamurl") != NULL)
         {
            ptr = strchr(buf, ':');
            if (!ptr)
               continue;

            szURL = strdup(ptr + 2);
            szURL[strlen(szURL) - 1] = 0;
         }

         if (szURL && szTitle)
         {
             m_pTarget->AcceptEvent(new StreamInfoEvent(szTitle, szURL));
             free(szTitle);
             free(szURL);
             szTitle = NULL;
             szURL = NULL;
         }
      }
   }

   free(szTitle);
   free(szURL);

   shutdown(m_hHandle, 2);
   closesocket(m_hHandle);
   m_hHandle = -1; 
}


