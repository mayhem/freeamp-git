/*____________________________________________________________________________
        
   FreeAmp - The Free MP3 Player

   Copyright (C) 2000 EMusic.com

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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>    

#ifdef WIN32
#include <io.h>
#else
#undef socklen_t
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif

#if defined(unix) || defined(__BEOS__)
#define SOCKET int
#endif

#if defined(unix)
#include <arpa/inet.h>
#define closesocket(x) close(x)
#define O_BINARY 0
#endif  

#include "Headlines.h"
#include "eventdata.h"
#include "event.h"
#include "debug.h"

#define DB printf("%s:%d\n", __FILE__, __LINE__);

HeadlineInfo::HeadlineInfo(const string &oUrl,
                           const string &oXMLPath, 
                           const string &oXMLURLPath, 
                           int iDownloadInterval,
                           int iHeadlineChangeInterval)
{
    m_oUrl = oUrl;
    m_oXMLPath = oXMLPath;
    m_oXMLURLPath = oXMLURLPath;
    m_iDownloadInterval = iDownloadInterval;
    m_iHeadlineChangeInterval = iHeadlineChangeInterval;
}

HeadlineInfo::~HeadlineInfo(void)
{

}

void HeadlineInfo::operator=(const HeadlineInfo &oOther)
{
    m_oUrl = oOther.m_oUrl;
    m_oXMLPath = oOther.m_oXMLPath;
    m_oXMLURLPath = oOther.m_oXMLURLPath;
    m_iDownloadInterval = oOther.m_iDownloadInterval;
    m_iHeadlineChangeInterval = oOther.m_iHeadlineChangeInterval;
}

Headlines::Headlines(FAContext * context)
{
    m_pContext = context;
    m_pWakeSem = new Semaphore();
    m_bPause = false;
    m_lLastDownload = 0;
    m_iIndex = 0;

    m_pThread = Thread::CreateThread();
    m_pThread->Create(worker_thread, this);
}

Headlines::~Headlines()
{

    delete m_pThread;
    delete m_pWakeSem;
}

void Headlines::SetInfo(HeadlineInfo &oInfo)
{
    m_oInfo = oInfo;
    m_lLastDownload = 0;
    m_iIndex = 0;
}

void Headlines::Pause(void)
{
    m_bPause = true;    
}

void Headlines::Resume(void)
{
    m_bPause = false;    
    m_pWakeSem->Signal();
}

void Headlines::worker_thread(void* arg)
{
    ((Headlines *)arg)->WorkerThread();
}

void Headlines::WorkerThread(void)
{
    time_t t;

    m_pWakeSem->Wait();

    for(;;)
    {
        if (m_bPause)
           m_pWakeSem->Wait();

        time(&t);
        if (m_lLastDownload + m_oInfo.m_iDownloadInterval < t || m_oHeadlines.size() == 0)
        {
            if (IsntError(Download()))
                m_lLastDownload = t;
        }

        if (m_oHeadlines.size())
        {
            m_iIndex = m_iIndex % m_oHeadlines.size();
            m_pContext->target->AcceptEvent(new HeadlineMessageEvent(
                    m_oHeadlines[m_iIndex].c_str(),
                    (m_iIndex >= (int)m_oHeadlineURLs.size()) ? "" : 
                     m_oHeadlineURLs[m_iIndex].c_str()));
                                       
            m_iIndex++;
        }
        usleep(m_oInfo.m_iHeadlineChangeInterval * 1000000);
    }
}

Error Headlines::Download(void)
{
    string oPage;
    Error  eRet;

    m_pContext->target->AcceptEvent(new StatusMessageEvent("Downloading headlines..."));
    eRet = DownloadHeadlines(m_oInfo.m_oUrl, oPage);
    if (eRet != kError_NoErr)
    {
       m_pContext->target->AcceptEvent(new StatusMessageEvent("Headline download failed."));
       return eRet;
    }   

    m_pContext->target->AcceptEvent(new StatusMessageEvent(""));
    m_oHeadlines.clear(); 
    
    eRet = ParseString(oPage);
    if (IsntError(eRet))
    {
        if (m_oHeadlines.size() != m_oHeadlineURLs.size())
            m_oHeadlineURLs.clear();
    }
    
    return eRet;
}

Error Headlines::BeginElement(string &oElement, AttrMap &oAttrMap)
{
    m_oPath += string("/") + oElement;

    return kError_NoErr;
}

Error Headlines::PCData(string &oData)
{
    if (m_oPath == m_oInfo.m_oXMLPath)
    {
        m_oHeadlines.push_back(oData);
        return kError_NoErr;
    }
    if (m_oPath == m_oInfo.m_oXMLURLPath)
    {
        m_oHeadlineURLs.push_back(oData);
        return kError_NoErr;
    }
    return kError_NoErr;
} 

Error Headlines::EndElement(string &oElement)
{
    char *pPtr;
    int   iOffset;

    pPtr = strrchr(m_oPath.c_str(), '/');
    if (pPtr == NULL)
       return kError_NoErr;

    iOffset = pPtr - m_oPath.c_str();
    m_oPath.erase(iOffset, m_oPath.length() - iOffset);

    return kError_NoErr;
}

const uint8 kHttpPort = 80;
const uint32 kMaxHostNameLen = 64;

Error Headlines::DownloadHeadlines(string &oUrl, string &oPage)
{
    Error result = kError_InvalidParam;

    char hostname[kMaxHostNameLen + 1];
    char localname[kMaxHostNameLen + 1];
    char proxyname[kMaxHostNameLen + 1];
    unsigned short port;
    struct sockaddr_in  addr;
    struct hostent      host;
    SOCKET s = -1;
    bool useProxy;
    char *buffer = NULL, *file;

    int32 numFields;
    uint32 length;

    result = kError_NoErr;  

    m_pContext->prefs->GetUseProxyServer(&useProxy);

    length = sizeof(proxyname);
    m_pContext->prefs->GetProxyServerAddress(proxyname, &length);

    if(useProxy)
    {
         numFields = sscanf(proxyname, "http://%[^:/]:%hu", hostname, &port);

         strcpy(proxyname, oUrl.c_str());
         file = proxyname;
    }
    else
    {
         numFields = sscanf(oUrl.c_str(), "http://%[^:/]:%hu", hostname, &port);
         file = strchr(oUrl.c_str() + 7, '/');
    }

    if(numFields < 1)
    {
         result = kError_InvalidURL;     
    }

    if(numFields < 2)
    {
         port = kHttpPort;
    }            

    // get hostname
    if(IsntError(result))
    {
        struct hostent* hostByName;
        struct hostent  hostByIP;

        //*m_debug << "gethostbyname: " << hostname << endl;
        hostByName = gethostbyname(hostname);

        // On some stacks a numeric IP address
        // will not parse with gethostbyname.  
        // If that didn't work try to convert it as a
        // numeric address before giving up.
        if(!hostByName)
        {
            static unsigned long ip;
            static char *addr_ptr[2] = {(char*)&ip, NULL};

            if((ip = inet_addr(hostname)) < 0) 
                result =  kError_CantFindHost;
            else
            {
                hostByIP.h_length = sizeof(uint32);
                hostByIP.h_addrtype = AF_INET;
                hostByIP.h_addr_list = (char**)&addr_ptr;
                hostByName = &hostByIP;
            }
        }

        if(IsntError(result))
        {
            memcpy(&host, hostByName, sizeof(struct hostent));
        }
    }

    // open socket
    if(IsntError(result))
    {
        memset(&addr, 0x00, sizeof(struct sockaddr_in));
        memcpy(&addr.sin_addr, host.h_addr, host.h_length);
        addr.sin_family= host.h_addrtype;
        addr.sin_port= htons(port); 

        s = socket(host.h_addrtype, SOCK_STREAM, 0);

        if(s < 0)
            result = kError_CantCreateSocket;
    }

    // connect and send request
    if(IsntError(result))
    {
        if(connect(s,(const struct sockaddr*)&addr, sizeof(struct sockaddr)))
            result = kError_CannotBind;

        if(IsntError(result))
        {
            gethostname(localname, kMaxHostNameLen);    

            const char* kHTTPQuery = "GET %s HTTP/1.1\n"
                                     "Host: %s\n"
                                     "Accept: */*\n" 
                                     "User-Agent: FreeAmp/%s\n\n";

            // the magic 58 is enough for fixed length time in
            // HTTP time format + 2 terabyte length range numbers.
            // the 2 extra bytes on the end is an extra \n and 0x00 byte
            char* query = new char[ strlen(kHTTPQuery) +
                                    strlen(file) +
                                    strlen(localname) +
                                        strlen(FREEAMP_VERSION)+ 2];

            sprintf(query, kHTTPQuery, file, localname, FREEAMP_VERSION);  

            int count;

            count = send(s, query, strlen(query), 0);
            if(count != (int)strlen(query))
            {
                result = kError_IOError;
            }

            delete [] query;
        }
    }

    // receive response
    if(IsntError(result))
    {
        uint32 bufferSize = 4096;
        int count;
        uint32 total = 0;

        buffer = (char*)malloc(bufferSize);

        result = kError_OutOfMemory;

        if(buffer)
        {
            result = kError_NoErr;

            for(;;)
            {
                if(total >= bufferSize - 1)
                {
                    bufferSize *= 2;

                    buffer = (char*) realloc(buffer, bufferSize);
                    if(!buffer)
                    {
                        result = kError_OutOfMemory;
                        break;
                    }
                }

                count = recv(s, buffer + total, bufferSize - total - 1, 0);
                if(count > 0)
                    total += count;
                else
                    break;
            }

            if (IsntError(result))
            {
                buffer[total] = 0;

                if (strstr(buffer, "\r\n\r\n"))
                   oPage = string(strstr(buffer, "\r\n\r\n") + 2);
                else
                   oPage = string(buffer);
            }
        }
    }

    // cleanup
    if(s > 0)
        closesocket(s);

    free(buffer);

    return result;
}

