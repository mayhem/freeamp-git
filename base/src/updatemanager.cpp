/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com

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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif


#include <assert.h>

#ifdef WIN32
#include <io.h>
#else
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

#if !defined(WIN32)
#include <strstream>
typedef ostrstream ostringstream;
#else
#include <sstream>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <algorithm>

#include "config.h"
#include "facontext.h"

#include "errors.h"
#include "updatemanager.h"
#include "utility.h"
#include "event.h"
#include "eventdata.h"
#include "zlib.h"
#include "mutex.h"
#include "semaphore.h"


const char* kUpdateServer = BRANDING_UPDATE_SERVER;
const char* kUpdatePath = BRANDING_UPDATE_PATH;
const char* kUpdateFile = BRANDING_UPDATE_PATH"/version_info.xml";
const char* kUpdateRequest = "GET %s HTTP/1.0\r\n"
                             "Host: %s\r\n"
                             "User-Agent: FreeAmp/%s\r\n" // we do not want to change this for branding
                             "\n";
const uint8 kUpdatePort = 80;


UpdateManager::UpdateManager(FAContext* context)
{
    m_runUpdateThread = true;
    m_context = context;

    m_currentPlatform = "UNKNOWN";
    m_currentArchitecture = "UNKNOWN";
}

UpdateManager::~UpdateManager()
{
    uint32 index = 0;
    uint32 size = 0;
    UpdateItem* item = NULL;

    m_runUpdateThread = false;

    m_quitMutex.Acquire();

    size = m_itemList.size();

    for(index = 0; index < size; index++)
    {
        item = m_itemList[index];

        if(item)
        {
            delete item;
        }
    }
}

// Functions for adding items to Update Manager
Error UpdateManager::AddItem(UpdateItem* item)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(item);

    if(item)
    {
        m_itemList.push_back(item);
        //SendItemAddedMessage(item);
        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error UpdateManager::RetrieveLatestVersionInfo(UMCallBackFunction function,
                                               void* cookie)
{
    Error result = kError_AlreadyUpdating;

    if(m_mutex.Acquire(0))
    {
        result = InternalRetrieveLatestVersionInfo(function, cookie);

        if(IsntError(result))
        {
            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Done;
                event.eventString = "Received latest version info from server.";
                function(&event, cookie);
            }
        }
        else if(result != kError_UserCancel)
        {
            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Error;
                event.data.errorData.errorCode = result;
                function(&event, cookie);
            }

        }

        m_mutex.Release();
    }

    
    return result;
}

Error UpdateManager::InternalRetrieveLatestVersionInfo(
                                               UMCallBackFunction function,
                                               void* cookie)
{
    Error result;
    string info;

    if(function)
    {
        UMEvent event;

        memset(&event, 0x00, sizeof(UMEvent));
        event.type = kUMEvent_Status;
        event.eventString = "Retrieving latest version info from server...";
        function(&event, cookie);
    }

    result = DownloadInfo(info, function, cookie);

    if(IsntError(result))
    {
        result = ParseString(info);
    }
    
    return result;
}

bool UpdateManager::IsUpdateAvailable()
{
    bool result = false;
    
    if(m_mutex.Acquire(0))
    {
        UpdateItem* item;

        vector<UpdateItem*>::iterator i = m_itemList.begin();

        for (; i != m_itemList.end(); i++)
        {
            uint32 localMajorVersion, currentMajorVersion;
            uint32 localMinorVersion, currentMinorVersion;
            uint32 localRevisionVersion, currentRevisionVersion;
            uint32 localFileVersion, currentFileVersion; 
            int32 numFields;

            item = *i;

            if(item->GetLocalFileTime().size())
            {
                time_t localFileTime, currentFileTime;
                uint32 month, day, year;
                
                numFields = sscanf(item->GetLocalFileTime().c_str(),
                       "%lu-%lu-%lu",&year,&month,&day);

                struct tm fileTime;

                memset(&fileTime, 0x00, sizeof(struct tm));

                fileTime.tm_mon = month;
                fileTime.tm_mday = day;
                fileTime.tm_year = year - 1900;

                localFileTime = mktime(&fileTime);

                numFields = sscanf(item->GetCurrentFileTime().c_str(),
                       "%lu-%lu-%lu",&year,&month,&day);

                fileTime.tm_mon = month;
                fileTime.tm_mday = day;
                fileTime.tm_year = year - 1900;

                currentFileTime = mktime(&fileTime);

                // is the version on the server more recent?
                if(currentFileTime > localFileTime)
                {
                    result = true;
                    break;
                }
            }
            else
            {
                numFields = sscanf(item->GetLocalFileVersion().c_str(),
                       "%lu.%lu.%lu.%lu", 
                       &localMajorVersion,&localMinorVersion,
                       &localRevisionVersion, &localFileVersion);

                if(numFields < 4)
                    localFileVersion = 0;

                if(numFields < 3)
                    localRevisionVersion = 0;

                if(numFields < 2)
                    localMinorVersion = 0;

                if(numFields < 1)
                    localMajorVersion = 0;
            
                numFields = sscanf(item->GetCurrentFileVersion().c_str(),
                       "%lu.%lu.%lu.%lu", 
                       &currentMajorVersion,&currentMinorVersion,
                       &currentRevisionVersion, &currentFileVersion);

                if(numFields < 4)
                    currentFileVersion = 0;

                if(numFields < 3)
                    currentRevisionVersion = 0;

                if(numFields < 2)
                    currentMinorVersion = 0;

                if(numFields < 1)
                    currentMajorVersion = 0;

                // is the version on the server more recent?
                if( (currentMajorVersion > localMajorVersion) ||
                    (currentMajorVersion == localMajorVersion && 
                     currentMinorVersion > localMinorVersion) ||
                    (currentMajorVersion == localMajorVersion && 
                     currentMinorVersion == localMinorVersion &&
                     currentRevisionVersion > localRevisionVersion) ||
                    (currentMajorVersion == localMajorVersion && 
                     currentMinorVersion == localMinorVersion &&
                     currentRevisionVersion == localRevisionVersion &&
                     currentFileVersion > localFileVersion))
                {
                    result = true;
                    break;
                }
            }            
        }
    }

    return result;
}

Error UpdateManager::UpdateComponents(UMCallBackFunction function,
                                      void* cookie)
{
    Error result = kError_AlreadyUpdating;
    
    if(m_mutex.Acquire(0))
    {
        uint32 downloadCount = 0;

        // make sure we have latest info
        //result = InternalRetrieveLatestVersionInfo(function, cookie);
        result = kError_NoErr;

        if(IsntError(result))
        {
            UpdateItem* item;

            vector<UpdateItem*>::iterator i = m_itemList.begin();

            // go thru list of components and see if 
            // there are newer versions
            for (; i != m_itemList.end(); i++)
            {
                bool currentVersionMoreRecent = false;
                uint32 localMajorVersion, currentMajorVersion;
                uint32 localMinorVersion, currentMinorVersion;
                uint32 localRevisionVersion, currentRevisionVersion;
                uint32 localFileVersion, currentFileVersion; 
                int32 numFields;

                item = *i;

                if(item->GetLocalFileTime().size())
                {
                    time_t localFileTime, currentFileTime;
                    uint32 month, day, year;
                
                    numFields = sscanf(item->GetLocalFileTime().c_str(),
                           "%lu-%lu-%lu",&year,&month,&day);

                    struct tm fileTime;

                    memset(&fileTime, 0x00, sizeof(struct tm));

                    fileTime.tm_mon = month;
                    fileTime.tm_mday = day;
                    fileTime.tm_year = year - 1900;

                    localFileTime = mktime(&fileTime);

                    numFields = sscanf(item->GetCurrentFileTime().c_str(),
                           "%lu-%lu-%lu",&year,&month,&day);

                    fileTime.tm_mon = month;
                    fileTime.tm_mday = day;
                    fileTime.tm_year = year - 1900;

                    currentFileTime = mktime(&fileTime);

                    // is the version on the server more recent?
                    if(currentFileTime > localFileTime)
                    {
                        currentVersionMoreRecent = true;
                    }
                }
                else
                {
                    numFields = sscanf(item->GetLocalFileVersion().c_str(),
                           "%lu.%lu.%lu.%lu", 
                           &localMajorVersion,&localMinorVersion,
                           &localRevisionVersion, &localFileVersion);

                    if(numFields < 4)
                        localFileVersion = 0;

                    if(numFields < 3)
                        localRevisionVersion = 0;

                    if(numFields < 2)
                        localMinorVersion = 0;

                    if(numFields < 1)
                        localMajorVersion = 0;
            
                    numFields = sscanf(item->GetCurrentFileVersion().c_str(),
                           "%lu.%lu.%lu.%lu", 
                           &currentMajorVersion,&currentMinorVersion,
                           &currentRevisionVersion, &currentFileVersion);

                    if(numFields < 4)
                        currentFileVersion = 0;

                    if(numFields < 3)
                        currentRevisionVersion = 0;

                    if(numFields < 2)
                        currentMinorVersion = 0;

                    if(numFields < 1)
                        currentMajorVersion = 0;

                    // is the version on the server more recent?
                    if( (currentMajorVersion > localMajorVersion) ||
                        (currentMajorVersion == localMajorVersion && 
                         currentMinorVersion > localMinorVersion) ||
                        (currentMajorVersion == localMajorVersion && 
                         currentMinorVersion == localMinorVersion &&
                         currentRevisionVersion > localRevisionVersion) ||
                        (currentMajorVersion == localMajorVersion && 
                         currentMinorVersion == localMinorVersion &&
                         currentRevisionVersion == localRevisionVersion &&
                         currentFileVersion > localFileVersion))
                    {
                        currentVersionMoreRecent = true;
                    }
                }
                

                if(currentVersionMoreRecent)
                {
                    result = DownloadItem(item, function, cookie);

                    if(IsntError(result))
                    {
                        downloadCount++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }

        if(IsntError(result))
        {
            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Done;

                if(downloadCount)
                    event.eventString = "Finished retrieving latest components.";
                else
                    event.eventString = "There are no new versions available at this time.";
                function(&event, cookie);
            }
        }
        else if(result != kError_UserCancel)
        {
            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Error;
                event.data.errorData.errorCode = result;
                function(&event, cookie);
            }

        }


        if(!downloadCount)
            result = kError_NoItemsUpdated;

        m_mutex.Release();
    }

    return result;
}

const uint8 kHttpPort = 80;
const uint32 kMaxHostNameLen = 64;

Error UpdateManager::DownloadInfo(string& info, 
                                  UMCallBackFunction function,
                                  void* cookie)
{
    Error result = kError_NoErr;
    char hostname[kMaxHostNameLen + 1];
    char localname[kMaxHostNameLen + 1];
    char proxyname[kMaxHostNameLen + 1];
    unsigned short  port;
    struct sockaddr_in  addr;
    struct hostent      host;
    SOCKET s = -1;
    bool useProxy;
    const char* file = NULL;
    uint32 length;

    m_context->prefs->GetPrefBoolean(kUseProxyPref, &useProxy);

    length = sizeof(proxyname);
    m_context->prefs->GetPrefString(kProxyHostPref, proxyname, &length);

    if(useProxy)
    {
        int32 numFields;

        numFields = sscanf(proxyname, 
                           "http://%[^:/]:%hu", hostname, &port);

        sprintf(proxyname, "http://%s%s", kUpdateServer, kUpdateFile);
        file = proxyname;

        if(numFields < 1)
            result = kError_InvalidURL;     

        if(numFields < 2)
            port = kHttpPort;
    }
    else
    {
        strcpy(hostname, kUpdateServer);
        port = kUpdatePort;
        file = kUpdateFile;
    }

    if(function)
    {
        UMEvent event;

        memset(&event, 0x00, sizeof(UMEvent));
        event.type = kUMEvent_Status;
        event.eventString = "Looking up server address...";
        bool ok = function(&event, cookie);

        if(!ok)
            result = kError_UserCancel;
    }

    // get hostname
    if(IsntError(result))
    {
        struct hostent* hostByName;
        struct hostent  hostByIP;
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

    if(function)
    {
        UMEvent event;

        memset(&event, 0x00, sizeof(UMEvent));
        event.type = kUMEvent_Status;
        event.eventString = "Found server address...";
        bool ok = function(&event, cookie);

        if(!ok)
            result = kError_UserCancel;
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

    if(function)
    {
        UMEvent event;

        memset(&event, 0x00, sizeof(UMEvent));
        event.type = kUMEvent_Status;
        event.eventString = "Connecting to the server...";
        bool ok = function(&event, cookie);

        if(!ok)
            result = kError_UserCancel;
    }

    // connect and send request
    if(IsntError(result))
    {
        if(connect(s,(const struct sockaddr*)&addr, sizeof(struct sockaddr)))
            result = kError_CannotBind;

        if(function)
        {
            UMEvent event;

            memset(&event, 0x00, sizeof(UMEvent));
            event.type = kUMEvent_Status;
            event.eventString = "Requesting latest version info...";
            bool ok = function(&event, cookie);

            if(!ok)
                result = kError_UserCancel;
        }

        if(IsntError(result))
        {
            gethostname(localname, kMaxHostNameLen);    

            char* query = new char[ strlen(kUpdateRequest) + 
                                    strlen(file) +
                                    strlen(localname) +
                                    strlen(FREEAMP_VERSION) + 1];
        
            sprintf(query, kUpdateRequest, file, localname, FREEAMP_VERSION);

            int count;

            count = send(s, query, strlen(query), 0);

            if(count != (int)strlen(query))
            {
                result = kError_IOError;
            }

            delete [] query;
        }
    }

    if(function)
    {
        UMEvent event;

        memset(&event, 0x00, sizeof(UMEvent));
        event.type = kUMEvent_Status;
        event.eventString = "Receiving latest version info...";
        bool ok = function(&event, cookie);

        if(!ok)
            result = kError_UserCancel;
    }

    // receive response
    if(IsntError(result))
    {
        uint32 bufferSize = 2048;
        char* buffer = NULL;
        int count;
        uint32 total = 0;

        buffer = (char*)malloc(bufferSize + 1);

        result = kError_OutOfMemory;

        if(buffer)
        {
            result = kError_NoErr;

            do
            {
                count = recv(s, buffer, bufferSize, 0);

                if(count > 0)
                {
                    buffer[count] = 0x00;
                    info += buffer;
                    total += count;
                }

                if(count < 0)
                    result = kError_IOError;
                
                if(function)
                {
                    UMEvent event;

                    memset(&event, 0x00, sizeof(UMEvent));
                    event.type = kUMEvent_Status;
                    event.eventString = "Receiving latest version info...";
                    bool ok = function(&event, cookie);

                    if(!ok)
                        result = kError_UserCancel;
                }

            }while(count > 0 && IsntError(result));

            free(buffer);
        }
    }

    if(s > 0)
        closesocket(s);


    return result;
}

static bool IsHTTPHeaderComplete(char* buffer, uint32 length)
{
    bool result = false;

    //if(length >= 4)
    //{
        //if( (buffer[0] == 'H' && buffer[1] == 'T' 
        //       && buffer[2] == 'T' && buffer[3] == 'P'))
        //{
            //cout << "buffer is HTTP" << endl;

            for(char* cp = buffer; cp < buffer + length; cp++)
            {
                if(!strncmp(cp, "\n\n", 2) || !strncmp(cp, "\r\n\r\n", 4))
                {
                    result = true;
                    break;
                }
            }
        //}
    //}

    return result;
}

static int32 GetContentLengthFromHeader(const char* buffer)
{
    int32 result = -1;

    char* cp = strstr(buffer, "Content-Length:");

    if(cp)
    {
        cp += strlen("Content-Length:") + 1;

        result = atoi(cp);
    }

    return result;
}

Error UpdateManager::DownloadItem(UpdateItem* item, 
                                  UMCallBackFunction function,
                                  void* cookie)
{
    Error result = kError_InvalidParam;
    
    assert(item);

    if(item)
    {   
        //cout << "Downloading " << item->GetLocalFileName() << endl;

        char hostname[kMaxHostNameLen + 1];
        char localname[kMaxHostNameLen + 1];
        char proxyname[kMaxHostNameLen + 1];
        unsigned short port;
        struct sockaddr_in  addr;
        struct hostent      host;
        SOCKET s = -1;
        char* file = NULL;
        char* destPath = NULL;
        char* srcPath = NULL;
        bool useProxy;
             
        // make sure there is a place to put it on this machine
        destPath = new char[_MAX_PATH];
        srcPath = new char[_MAX_PATH];
        uint32 length = _MAX_PATH;

        m_context->prefs->GetPrefString(kInstallDirPref, destPath, &length);
        strcat(destPath, "\\update");

        // move past platform dir
        char* cp = strchr(item->GetCurrentFileLocation().c_str(), '/');

        // move past arch dir
        if(cp)
            cp = strchr(cp + 1, '/');

        if(cp)
        {
            strcat(destPath, "\\");
            strcat(destPath, cp + 1);
        }

        // make sure all the directory separators are correct
        for (int32 index = strlen(destPath) - 1; index >=0; index--)
        {
            if(destPath[index] == '\\' && DIR_MARKER == '/')
                destPath[index] = DIR_MARKER;
            else if(destPath[index] == '/' && DIR_MARKER == '\\')
                destPath[index] = DIR_MARKER;
        }

        cp = strrchr(destPath, DIR_MARKER);

        if(cp)
            *cp = 0x00;

        CreateDirectoryPath(destPath);

        if(cp)
            *cp = DIR_MARKER;

        result = kError_ProtocolNotSupported;

        // where should we connect to?
        if(!strncasecmp(item->GetCurrentFileURL().c_str(), "http://", 7))
        {
            int32 numFields;
            uint32 length;

            result = kError_NoErr;  

            m_context->prefs->GetPrefBoolean(kUseProxyPref, &useProxy);

            length = sizeof(proxyname);
            m_context->prefs->GetPrefString(kProxyHostPref, proxyname, &length);

            if(useProxy)
            {
                numFields = sscanf(proxyname, 
                                   "http://%[^:/]:%hu", hostname, &port);

                strcpy(proxyname, item->GetCurrentFileURL().c_str());
                file = proxyname;
            }
            else
            {
                numFields = sscanf(item->GetCurrentFileURL().c_str(), 
                               "http://%[^:/]:%hu", hostname, &port);

                file = strchr(item->GetCurrentFileURL().c_str() + 7, '/');
            }

            if(numFields < 1)
            {
                result = kError_InvalidURL;     
            }

            if(numFields < 2)
            {
                port = kHttpPort;
            }            
        }

        if(function)
        {
            UMEvent event;

            memset(&event, 0x00, sizeof(UMEvent));
            event.type = kUMEvent_Status;
            event.eventString = "Downloading ";
            event.eventString += item->GetLocalFileName();
            bool ok = function(&event, cookie);

            if(!ok)
                result = kError_UserCancel;
        }

        // get hostname
        if(IsntError(result))
        {
            struct hostent* hostByName;
            struct hostent  hostByIP;
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

            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Status;
                event.eventString = "Downloading ";
                event.eventString += item->GetLocalFileName();
                bool ok = function(&event, cookie);

                if(!ok)
                    result = kError_UserCancel;
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

            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Status;
                event.eventString = "Downloading ";
                event.eventString += item->GetLocalFileName();
                bool ok = function(&event, cookie);

                if(!ok)
                    result = kError_UserCancel;
            }
        }

        // connect and send request
        if(IsntError(result))
        {
            if(connect(s,(const struct sockaddr*)&addr, sizeof(struct sockaddr)))
                result = kError_CannotBind;

            if(function)
            {
                UMEvent event;

                memset(&event, 0x00, sizeof(UMEvent));
                event.type = kUMEvent_Status;
                event.eventString = "Downloading ";
                event.eventString += item->GetLocalFileName();
                bool ok = function(&event, cookie);

                if(!ok)
                    result = kError_UserCancel;
            }

            if(IsntError(result))
            {
                gethostname(localname, kMaxHostNameLen);    

                const char* kHTTPQuery = "GET %s HTTP/1.0\r\n"
                                         "Host: %s\r\n"
                                         "Accept: */*\r\n" 
                                         "User-Agent: FreeAmp/%s\r\n"
                                         "\r\n";
                                             
                char* query = new char[ strlen(kHTTPQuery) + 
                                        strlen(file) +
                                        strlen(localname) +
                                        strlen(FREEAMP_VERSION) + 1];
            
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

        if(function)
        {
            UMEvent event;

            memset(&event, 0x00, sizeof(UMEvent));
            event.type = kUMEvent_Status;
            event.eventString = "Downloading ";
            event.eventString += item->GetLocalFileName();
            bool ok = function(&event, cookie);

            if(!ok)
                result = kError_UserCancel;
        }

        // receive response
        if(IsntError(result))
        {
            uint32 bufferSize = 2048;
            char* buffer = NULL;
            int count;
            uint32 total = 0;

            buffer = (char*)malloc(bufferSize);

            result = kError_OutOfMemory;

            if(buffer)
            {
                result = kError_NoErr;

                do
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
                    {
                        result = kError_IOError;
                    }

                    if(function)
                    {
                        UMEvent event;

                        memset(&event, 0x00, sizeof(UMEvent));
                        event.type = kUMEvent_Status;
                        event.eventString = "Downloading ";
                        event.eventString += item->GetLocalFileName();
                        bool ok = function(&event, cookie);

                        if(!ok)
                            result = kError_UserCancel;
                    }


                }while(!IsHTTPHeaderComplete(buffer, total) && IsntError(result));
            }

            // parse header
            if(IsntError(result))
            {
                uint32 returnCode = atoi(buffer+9);
                buffer[total] = 0x00;
                //cout << buffer << endl;

                //cout << returnCode << endl;

                switch(buffer[9])
                {
                    // 1xx: Informational - Request received, continuing process
                    case '1':
                    {
                        // not sure what to do here... continue receiving???
                    }    

                    // 2xx: Success - The action was successfully received,
                    // understood, and accepted
                    case '2':
                    {
                        result = kError_UnknownErr;

                        //cout << destPath << endl;

                        int32 fileSize = GetContentLengthFromHeader(buffer);

                        int openFlags = O_BINARY|O_CREAT|O_RDWR|O_TRUNC;

                        int fd = open(destPath, openFlags, S_IREAD | S_IWRITE);

                        if(fd >= 0)
                        {
                            result = kError_NoErr;

                            char* cp = strstr(buffer, "\n\n");

                            if(cp)
                                cp += 2;
                            else
                            {
                                cp = strstr(buffer, "\r\n\r\n");

                                if(cp)
                                    cp += 4;
                            }

                            if(cp)
                            {
                                if(cp - buffer < (int)total)
                                {
                                    write(fd, cp, total - (cp - buffer));
                                    total -= (cp - buffer);

                                    if(function)
                                    {
                                        UMEvent event;

                                        memset(&event, 0x00, sizeof(UMEvent));
                                        event.type = kUMEvent_Progress;
                                        event.data.progressData.item = item;
                                        event.data.progressData.position = total;
                                        event.data.progressData.total = fileSize;

                                        bool ok = function(&event, cookie);

                                        if(!ok)
                                            result = kError_UserCancel;
                                    }
                                }
                            }

                            do
                            {
                                count = recv(s, buffer, bufferSize, 0);

                                if(count > 0)
                                {
                                    write(fd, buffer, count);
                                    total += count;
                                }

                                if(count < 0)
                                    result = kError_IOError;
                                
                                if(function)
                                {
                                    UMEvent event;

                                    memset(&event, 0x00, sizeof(UMEvent));
                                    event.type = kUMEvent_Progress;
                                    event.data.progressData.item = item;
                                    event.data.progressData.position = total;
                                    event.data.progressData.total = fileSize;

                                    bool ok = function(&event, cookie);

                                    if(!ok)
                                        result = kError_UserCancel;
                                }

                                //cout << "bytes recvd:" << count << endl;

                            }while(count > 0 && IsntError(result));
                            

                            close(fd);  
                            
                            if(IsntError(result))
                            {

                                // now that we have it we need to uncompress it
                                // old dest is new src...
                                strcpy(srcPath, destPath); 
                                char* cp = strrchr(destPath, '.');

                                if(cp)
                                    *cp = 0x00;
                                                          
                                gzFile gzfd = gzopen(srcPath, "rb");

                                if (gzfd == NULL)
                                    result = kError_FileNotFound;
                                else if((fd = open(destPath, openFlags, S_IREAD | S_IWRITE)) >=0)
                                {
                                    do
                                    {
                                        count = gzread(gzfd, buffer, bufferSize);

                                        if(count > 0)
                                        {
                                            write(fd, buffer, count);
                                        }

                                    }while(count > 0);

                                    close(fd);
                                }

                                if(gzfd != NULL)
                                    gzclose(gzfd);

                                remove(srcPath);

#ifdef WIN32
                                // when the file is unzipped the modification
                                // date is fubar'd. we need to set it so it
                                // is correct if this is a DATE file.
                                if(item->GetCurrentFileTime().size())
                                {
                                    uint32 month, day, year;

                                    sscanf(item->GetCurrentFileTime().c_str(),
                                                       "%lu-%lu-%lu",&year,&month,&day);

                                    HANDLE fileHandle;
                                    
                                    fileHandle = CreateFile(destPath, 
                                                            GENERIC_READ|GENERIC_WRITE,
                                                            0,
                                                            NULL,
                                                            OPEN_EXISTING,
                                                            FILE_ATTRIBUTE_NORMAL,
                                                            NULL);

                                    if(fileHandle != INVALID_HANDLE_VALUE)
                                    {
                                        SYSTEMTIME sysTime;
                                        FILETIME fileTime, modTime;

                                        memset(&sysTime, 0x00, sizeof(SYSTEMTIME));

                                        sysTime.wDay = day;
                                        sysTime.wMonth = month;
                                        sysTime.wYear = year;
    
                                        SystemTimeToFileTime(&sysTime, &fileTime);
                                        LocalFileTimeToFileTime(&fileTime, &modTime);

                                        SetFileTime(fileHandle, NULL, NULL, &modTime);

                                        CloseHandle(fileHandle);
                                    }

                                }
#endif
                            }
                        }
                        else
                        {
                            switch(errno)
                            {
                                case EEXIST:
                                    result = kError_FileExists;
                                    break;

                                case EACCES:
                                    result = kError_FileNoAccess;
                                    break;

                                case ENOENT:
                                    result = kError_FileNotFound;
                                    break;

                                case EMFILE:
                                    result = kError_FileNoHandles;
                                    break;

                                case EINVAL:
                                    result = kError_FileInvalidArg;
                                    break;
                
                            }
                        }
                        
                        break;
                    }

                    // 3xx: Redirection - Further action must be taken in order to
                    // complete the request
                    case '3':
                    {
                        char* cp = strstr(buffer, "Location:");
                        //int32 length;

                        if(cp)
                        {
                            cp += 9;

                            if(*cp == 0x20)
                                cp++;

                            char *end;
                            for(end = cp; end < buffer + total; end++)
                                if(*end=='\r' || *end == '\n') break;

                            *end = 0x00;

                            //cout << cp << endl;

                            if(305 == returnCode) // proxy
                            {
                                char* proxy = new char[strlen(cp) + 
                                                       strlen(item->GetCurrentFileURL().c_str()) + 1];

                                sprintf(proxy, "%s%s", cp, item->GetCurrentFileURL().c_str());

                                item->SetCurrentFileURL(proxy);

                                delete [] proxy;
                            }
                            else // redirect of some type
                            {
                                item->SetCurrentFileURL(cp);
                            }

                            result = DownloadItem(item, function, cookie);
                        }
                        
                        break;
                    }

                    // 4xx: Client Error - The request contains bad syntax or cannot
                    // be fulfilled
                    case '4':
                    {
                        switch(returnCode)
                        {
                            case 400:
                                result = kError_BadHTTPRequest;
                                break;

                            case 401:
                                result = kError_AccessNotAuthorized;
                                break;                           

                            case 403:
                                result = kError_AccessForbidden;
                                break;

                            case 404:
                                result = kError_FileNotFound;
                                break;

                            default:
                                result = kError_UnknownErr;
                                break;
                        }

                        break;
                    }

                    // 5xx: Server Error - The server failed to fulfill an apparently
                    // valid request
                    case '5':
                    {
                        result = kError_UnknownServerError;
                        break;
                    }
                }

            }

            // cleanup
            if(buffer)
                free(buffer);            
        }

        // cleanup
        if(s > 0)
            closesocket(s);

        if(destPath)
            delete [] destPath;

        if(srcPath)
            delete [] srcPath;
    }

    return result;

}

// Utility Functions
bool UpdateManager::IsEmpty()
{
    bool result;
    //m_mutex.Acquire();

    result = m_itemList.empty();

    //m_mutex.Release();
    return result;
}

uint32 UpdateManager::CountItems()
{
    uint32 result;
    //m_mutex.Acquire();

    result = m_itemList.size();

    //m_mutex.Release();
    return result;
}

UpdateItem* UpdateManager::ItemAt(uint32 index)
{
    UpdateItem* result = NULL;
    //m_mutex.Acquire();
    
    index = CheckIndex(index);

    if(index != kInvalidIndex)
    {
        result = m_itemList[index];
    }
    
    //m_mutex.Release();
    return result;
}

uint32 UpdateManager::IndexOf(UpdateItem* item)
{
    uint32 result = kInvalidIndex;
    uint32 index = 0;
    uint32 size = 0;

    assert(item);

    if(item)
    {
        size = m_itemList.size();

        for(index = 0; index < size; index++)
        {
            if(item == m_itemList[index])
            {
                result = index;
                break;
            }
        }
    }
    
    return result;
}

bool UpdateManager::HasItem(UpdateItem* item)
{
    return (IndexOf(item) != kInvalidIndex);
}

// Internal functions
inline uint32 UpdateManager::CheckIndex(uint32 index)
{
	// If we're dealing with a bogus index then set it to -1.
	if(index >= CountItems())
    {
		index = kInvalidIndex;
    }

	return index;
}

// parsing code
Error UpdateManager::BeginElement(string &element, AttrMap &attrMap)
{
	m_path += string("/") + element;

	if(m_path == "/VERSIONINFO/PLATFORM")
	{
        m_versionPlatform = attrMap["NAME"];
        m_versionArchitecture = attrMap["ARCHITECTURE"];
    }

    if(m_path == "/VERSIONINFO/PLATFORM/COMPONENT" &&
       m_versionPlatform == m_currentPlatform &&
       m_versionArchitecture == m_currentArchitecture)
    {
        UpdateItem* item;
        bool foundComponent = false;

        vector<UpdateItem*>::iterator i = m_itemList.begin();

        // is there a matching component on this machine
        for (; i != m_itemList.end(); i++)
        {
            item = *i;

            if(!strcasecmp(attrMap["NAME"].c_str(), 
                           item->GetLocalFileName().c_str()))
            {
                foundComponent = true;
                break;
            }
        }

        // need to add a new component to the list
        if(!foundComponent)
        {
            item = new UpdateItem();   
            m_itemList.push_back(item);
            item->SetLocalFileName(attrMap["NAME"]);
        }

        item->SetCurrentFileLocation(attrMap["LOCATION"]);

        string url = "http://";

        // where is the latest version located?
        url += kUpdateServer;
        url += kUpdatePath;
        url += attrMap["LOCATION"];

        item->SetCurrentFileURL(url);

        item->SetCurrentFileVersion(attrMap["VERSION"]);
        item->SetFileDescription(attrMap["DESCRIPTION"]);
        item->SetCurrentFileTime(attrMap["DATE"]);

        /*if(attrMap.find("NAME") != attrMap.end())
            cout << "Name: " << attrMap["NAME"] << endl;

        if(attrMap.find("DESCRIPTION") != attrMap.end())
            cout << "Description: " << attrMap["DESCRIPTION"] << endl;

        if(attrMap.find("VERSION") != attrMap.end())
            cout << "Version: " << attrMap["VERSION"] << endl;

        if(attrMap.find("LOCATION") != attrMap.end())
            cout << "Location: " << attrMap["LOCATION"] << endl;*/
    }

    return kError_NoErr;
}

Error UpdateManager::PCData(string &data)
{
    //cout << "PCData: " << data << endl;

	return kError_NoErr;
}


Error UpdateManager::EndElement(string &element)
{
    //cout << "EndElement: " << element << endl;

	char *ptr;
    int   offset;
    
    ptr = strrchr(m_path.c_str(), '/');
    if (ptr == NULL)
       return kError_NoErr;
       
    offset = ptr - m_path.c_str();
    m_path.erase(offset, m_path.length() - offset);
     
	return kError_NoErr;
}
