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
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define closesocket(x) close(x)
#define O_BINARY 0
#endif
#ifdef unix
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
//#include <iterators>

#include "config.h"
#include "facontext.h"

#include "errors.h"
#include "downloadmanager.h"
#include "registrar.h"
#include "utility.h"
#include "event.h"
#include "eventdata.h"
#include "musicbrowser.h"


DownloadManager::DownloadManager(FAContext* context)
{
    m_context = context;
    m_current = 0;

    Registrar registrar;

    registrar.SetSubDir("plugins");
    registrar.SetSearchString("*.dlf");
    registrar.InitializeRegistry(&m_formatRegistry, context->prefs);

    const RegistryItem* module = NULL;
    DownloadFormat* dlf = NULL;
    int32 i = 0;

    while((module = m_formatRegistry.GetItem(i++)))
    {
        dlf = (DownloadFormat*) module->InitFunction()(m_context);

        if(dlf)
        {
            DownloadFormatInfo dlfi;

            uint32 index = 0;

            // error != kError_NoMoreFormats
            while(IsntError(dlf->GetSupportedFormats(&dlfi, index++)))
            {
                dlfi.SetRef(dlf);
                m_formats.push_back(new DownloadFormatInfo(dlfi));
            }
        }
    }

    LoadResumableDownloadItems();

    /*MetaData md;

    md.SetArtist("The Crystal Method");
    md.SetAlbum("Vegas");
    md.SetGenre("Electronica");
    md.SetTitle("Trip Like I Do");

    DownloadItem* dli = new DownloadItem("http://www.blah.com","a.mp3", &md);
    dli->SetTotalBytes(2221568);
    dli->SetBytesReceived(54432);
    dli->SetState(kDownloadItemState_Paused);

    m_itemList.push_back(dli);*/

    m_runDownloadThread = true;

    m_downloadThread = Thread::CreateThread();

    if(m_downloadThread)
    {
        m_downloadThread->Create(download_thread_function, this);
    }
}

DownloadManager::~DownloadManager()
{
    uint32 index = 0;
    uint32 size = 0;
    DownloadItem* item = NULL;

    m_runDownloadThread = false;
    m_queueSemaphore.Signal();

    m_quitMutex.Acquire();

    SaveResumableDownloadItems();

    size = m_itemList.size();

    for(index = 0; index < size; index++)
    {
        item = m_itemList[index];

        if(item)
        {
            if(item->GetState() == kDownloadItemState_Cancelled)
            {
                CleanUpDownload(item);
            }
            delete item;
        }
    }

    size = m_formats.size();

    for(index = 0; index < size; index++)
    {
        delete m_formats[index]->GetRef();
        delete m_formats[index];
    }

    if(m_downloadThread)
    {
        delete m_downloadThread;
    }
}

// Functions for adding items to Download Manager
// Adding an item implicitly queues it for
// downloading.
Error DownloadManager::AddItem(const char* url, const char* filename)
{
    Error result = kError_InvalidParam;

    assert(url);

    if(url)
    {
        result = kError_OutOfMemory;

        if(!filename)
        {
            filename = strrchr(url, '/');

            if(filename)
                filename++;
            else
                filename = url;
        }

        DownloadItem* item = new DownloadItem(url, filename);

        if(item)
        {
            result = AddItem(item);
        }
    }

    return result;
}

Error DownloadManager::AddItem(DownloadItem* item)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(item);

    if(item)
    {
        m_itemList.push_back(item);
        SendItemAddedMessage(item);
        QueueDownload(item);

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error DownloadManager::AddItems(vector<DownloadItem*>* list)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(list);

    if(list)
    {
        m_itemList.insert( m_itemList.end(),
                            list->begin(), 
                            list->end());

        uint32 count = list->size();

        for(uint32 i = 0; i < count; i++)
        {
            SendItemAddedMessage((*list)[i]);
            QueueDownload((*list)[i]);
        }

        result = kError_NoErr;
    }

    m_mutex.Release();

    return result;
}

// Changes item state to queued if it is cancelled or error.
// This will indicate to the download thread that it should
// attempt to retrieve this item. Has no effect if the item's
// state is Done, or Downloading.
Error DownloadManager::QueueDownload(DownloadItem* item)
{
    Error result = kError_InvalidParam;

    assert(item);

    if(item)
    {
        //cout << "Queue item: " << item->SourceURL() << endl;

        if(item->GetState() != kDownloadItemState_Downloading &&
           item->GetState() != kDownloadItemState_Done &&
           item->GetState() != kDownloadItemState_Queued)
        {
            item->SetState(kDownloadItemState_Queued);
            SendStateChangedMessage(item);

            m_queueList.push_back(item);

            m_queueSemaphore.Signal();

            result = kError_NoErr;
        }
    }

    return result;
}

Error DownloadManager::QueueDownload(uint32 index)
{
    return QueueDownload(ItemAt(index));
}


// Changes item state to cancelled if it is queued or downloading.
// If allowResume is true then data is retained for later download.
// Has no effect if the item's state is Done, Cancelled, or Error.
Error DownloadManager::CancelDownload(DownloadItem* item, bool allowResume)
{
    Error result = kError_InvalidParam;

    assert(item);

    if(item)
    {
        if(item->GetState() == kDownloadItemState_Downloading ||
           item->GetState() == kDownloadItemState_Queued ||
           item->GetState() == kDownloadItemState_Paused)
        {
            m_queueMutex.Acquire();

            // remove from download queue
            if(item->GetState() == kDownloadItemState_Queued)
            {
                deque<DownloadItem*>::iterator position;
                
                if((position = find(m_queueList.begin(), 
                                    m_queueList.end(), 
                                    item)) != m_queueList.end())
                {
                    m_queueList.erase(position); 
                }
            }

            m_queueMutex.Release();

            if(!allowResume)
            {
                item->SetState(kDownloadItemState_Cancelled);
            }
            else
            {
                item->SetState(kDownloadItemState_Paused);
            }

            SendStateChangedMessage(item);

            result = kError_NoErr;
        }
    }

    return result;
}

Error DownloadManager::CancelDownload(uint32 index, bool allowResume)
{
    return CancelDownload(ItemAt(index), allowResume);
}


// File Format support
Error DownloadManager::GetSupportedDownloadFormats(DownloadFormatInfo* format,
                                                   uint32 index)
{
    Error result = kError_InvalidParam;

    assert(format);

    if(format)
    {
        result = kError_NoMoreFormats;

        uint32 numFormats = m_formats.size();

        if(index < numFormats)
        {
            result = kError_NoErr;

            *format = *m_formats[index];
        }
    }

    return result;
}

Error DownloadManager::ReadDownloadFile(char* url, 
                                        vector<DownloadItem*>* items)
{
    Error result = kError_InvalidParam;

    assert(url);

    if(url)
    {
        // find a suitable plugin
        result = kError_FormatNotSupported;
        char* extension = strrchr(url, '.');

        if(extension)
        {
            extension++;

            uint32 numFormats = m_formats.size();

            for(uint32 index = 0; index < numFormats; index++)
            {
                DownloadFormatInfo* format = m_formats[index];
                
                if(!strcmp(extension, format->GetExtension()))
                {
                    bool addToInternalList = false;

                    if(!items)
                    {
                        items = new vector<DownloadItem*>;
                        addToInternalList = true;
                    }

                    result = format->GetRef()->ReadDownloadFile(url, 
                                                                items);

                    if(addToInternalList)
                    {
                        AddItems(items);
                        delete items;
                    }

                    break;
                }
            }
        }
    }

    return result;
}

// Utility Functions
bool DownloadManager::IsEmpty()
{
    bool result;
    m_mutex.Acquire();

    result = m_itemList.empty();

    m_mutex.Release();
    return result;
}

uint32 DownloadManager::CountItems()
{
    uint32 result;
    m_mutex.Acquire();

    result = m_itemList.size();

    m_mutex.Release();
    return result;
}

DownloadItem* DownloadManager::ItemAt(uint32 index)
{
    DownloadItem* result = NULL;
    m_mutex.Acquire();
    
    index = CheckIndex(index);

    if(index != kInvalidIndex)
    {
        result = m_itemList[index];
    }
    
    m_mutex.Release();
    return result;
}

uint32 DownloadManager::IndexOf(DownloadItem* item)
{
    return InternalIndexOf(&m_itemList, item);
}

uint32 DownloadManager::InternalIndexOf(vector<DownloadItem*>* list,
                                        DownloadItem* item)
{
    uint32 result = kInvalidIndex;
    uint32 index = 0;
    uint32 size = 0;

    assert(list);
    assert(item);

    if(list && item)
    {
        size = list->size();

        for(index = 0; index < size; index++)
        {
            if(item == (*list)[index])
            {
                result = index;
                break;
            }
        }
    }
    
    return result;
}

bool DownloadManager::HasItem(DownloadItem* item)
{
    return (IndexOf(item) != kInvalidIndex);
}

// Internal functions

inline uint32 DownloadManager::CheckIndex(uint32 index)
{
	// If we're dealing with a bogus index then set it to -1.
	if(index >= CountItems())
    {
		index = kInvalidIndex;
    }

	return index;
}

DownloadItem* DownloadManager::GetNextQueuedItem()
{
    DownloadItem* result = NULL;

    m_queueMutex.Acquire();

    if(m_queueList.size())
    {
        result = m_queueList[0];

        m_queueList.pop_front();

        cout << "Queue count: " << m_queueList.size() << endl;
    }

    m_queueMutex.Release();

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

const uint8 kHttpPort = 80;
const uint32 kMaxHostNameLen = 64;

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

Error DownloadManager::Download(DownloadItem* item)
{
    Error result = kError_InvalidParam;

    assert(item);

    if(item)
    {
        char hostname[kMaxHostNameLen + 1];
        char localname[kMaxHostNameLen + 1];
        char proxyname[kMaxHostNameLen + 1];
        uint8  port;
        struct sockaddr_in  addr;
        struct hostent      host;
        SOCKET s = -1;
        char* file = NULL;
        char* destPath = NULL;
        bool useProxy;

        destPath = new char[_MAX_PATH];
        uint32 length = _MAX_PATH;

        m_context->prefs->GetPrefString(kSaveMusicDirPref, destPath, &length);

        strcat(destPath, DIR_MARKER_STR);
        strcat(destPath, item->DestinationFile().c_str());

        result = kError_ProtocolNotSupported;

        // where should we connect to?
        if(!strncasecmp(item->SourceURL().c_str(), "http://", 7))
        {
            int32 numFields;
            uint32 length;

            result = kError_NoErr;  

            m_context->prefs->GetUseProxyServer(&useProxy);

            length = sizeof(proxyname);
            m_context->prefs->GetProxyServerAddress(proxyname, &length);

            if(useProxy)
            {
                numFields = sscanf(proxyname, 
                                   "http://%[^:/]:%u", hostname, &port);

                strcpy(proxyname, item->SourceURL().c_str());
                file = proxyname;
            }
            else
            {
                numFields = sscanf(item->SourceURL().c_str(), 
                               "http://%[^:/]:%u", hostname, &port);

                file = strchr(item->SourceURL().c_str() + 7, '/');
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

        if(item->GetState() == kDownloadItemState_Cancelled ||
           item->GetState() == kDownloadItemState_Paused)
            result = kError_UserCancel;

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

            if(item->GetState() == kDownloadItemState_Cancelled ||
               item->GetState() == kDownloadItemState_Paused)
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

            if(item->GetState() == kDownloadItemState_Cancelled ||
               item->GetState() == kDownloadItemState_Paused)
                result = kError_UserCancel;
        }

        // connect and send request
        if(IsntError(result))
        {
            if(connect(s,(const struct sockaddr*)&addr, sizeof(struct sockaddr)))
                result = kError_CannotBind;

            if(item->GetState() == kDownloadItemState_Cancelled ||
               item->GetState() == kDownloadItemState_Paused)
                result = kError_UserCancel;

            if(IsntError(result))
            {
                gethostname(localname, kMaxHostNameLen);    

                const char* kHTTPQuery = "GET %s HTTP/1.1\n"
                                         "Host: %s\n"
                                         "Accept: */*\n" 
                                         "User-Agent: "BRANDING"/%s\n";

                const char* kRange = "Range: %lu-\n"
                                     "If-Range: %s\n";

                const char* kCookie = "Cookie: %s\n";

                // the magic 58 is enough for fixed length time in
                // HTTP time format + 2 terabyte length range numbers.
                // the 2 extra bytes on the end is an extra \n and 0x00 byte
                char* query = new char[ strlen(kHTTPQuery) + 
                                        strlen(file) +
                                        strlen(localname) +
                                        strlen(FREEAMP_VERSION)+
                                        (item->GetBytesReceived() ? (strlen(kRange) + 58): 0 ) +
                                        (item->SourceCookie().size() ? (item->SourceCookie().size() + strlen(kCookie)): 0) +
                                        2];
            
                sprintf(query, kHTTPQuery, file, localname, FREEAMP_VERSION);

                // do we need to request a range?
                if(item->GetBytesReceived())
                { 
                    struct stat st;

                    if(-1 != stat(destPath, &st))
                    {
                        char* range = new char[strlen(kRange) + 58 + 1];
                        char time[32];

                        RFC822GMTTimeString(gmtime(&st.st_mtime), time);

                        sprintf(range, kRange, item->GetBytesReceived(), time);

                        strcat(query, range);

                        delete [] range;
                    }
                    else
                    {
                        item->SetBytesReceived(0);
                    }
                }

                if(item->SourceCookie().size())
                {
                    char* cookie = new char[strlen(kCookie) + item->SourceCookie().size() + 1];

                    sprintf(cookie, kCookie, item->SourceCookie().c_str());

                    strcat(query, cookie);

                    delete [] cookie;
                }
            
                strcat(query, "\n");

                //cout << query << endl;

                int count;

                count = send(s, query, strlen(query), 0);

                if(count != (int)strlen(query))
                {
                    result = kError_IOError;
                }

                if(item->GetState() == kDownloadItemState_Cancelled ||
                   item->GetState() == kDownloadItemState_Paused)
                    result = kError_UserCancel;

                delete [] query;
            }
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

                    if(item->GetState() == kDownloadItemState_Cancelled ||
                       item->GetState() == kDownloadItemState_Paused)
                        result = kError_UserCancel;


                }while(!IsHTTPHeaderComplete(buffer, total) && IsntError(result));
            }

            // parse header
            if(IsntError(result))
            {
                uint32 returnCode = atoi(buffer+9);
                buffer[total] = 0x00;
                //cout << buffer << endl;

                cout << returnCode << endl;

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

                        int32 fileSize = GetContentLengthFromHeader(buffer);

                        if(fileSize > 0)
                            item->SetTotalBytes(fileSize);

                        cout << destPath << endl;

                        int openFlags = O_BINARY|O_CREAT|O_RDWR|O_APPEND;

                        if(returnCode == 200) // always whole file
                        {
                            item->SetBytesReceived(0);
                            openFlags |= O_TRUNC;
                        }

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
                                    item->SetBytesReceived(total - (cp - buffer) + item->GetBytesReceived());
                                    SendProgressMessage(item);
                                }
                            }

                            do
                            {
                                count = recv(s, buffer, bufferSize, 0);

                                if(count > 0)
                                {
                                    write(fd, buffer, count);
                                    item->SetBytesReceived(count + item->GetBytesReceived());
                                    SendProgressMessage(item);
                                }

                                if(count < 0)
                                    result = kError_IOError;
                                
                                if(item->GetState() == kDownloadItemState_Cancelled ||
                                   item->GetState() == kDownloadItemState_Paused)
                                    result = kError_UserCancel;

                                //cout << "bytes recvd:" << count << endl;

                            }while(count > 0 && IsntError(result));

                            close(fd);                           
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
                                                       strlen(item->SourceURL().c_str()) + 1];

                                sprintf(proxy, "%s%s", cp, item->SourceURL().c_str());

                                item->SetSourceURL(proxy);

                                delete [] proxy;
                            }
                            else // redirect of some type
                            {
                                item->SetSourceURL(cp);
                            }

                            result = Download(item);
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
                                result = kError_HTTPFileNotFound;
                                break;

                            case 416:
                                // try to grab the whole thing...
                                item->SetBytesReceived(0);
                                result = Download(item);
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
    }

    return result;
}

void DownloadManager::CleanUpDownload(DownloadItem* item)
{
    cout << "Cleaning item: " << item->SourceURL() << endl;

    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    m_context->prefs->GetPrefString(kSaveMusicDirPref, path, &length);

    strcat(path, DIR_MARKER_STR);
    strcat(path, item->DestinationFile().c_str());

    remove(path);

    item->SetBytesReceived(0);
}

Error DownloadManager::SubmitToDatabase(DownloadItem* item)
{
    Error result = kError_InvalidParam;

    assert(item);

    if(item)
    {
        cout << "Submitting item: " << item->SourceURL() << endl;

        char path[_MAX_PATH];
        uint32 length = sizeof(path);

        m_context->prefs->GetPrefString(kSaveMusicDirPref, path, &length);

        strcat(path, DIR_MARKER_STR);
        strcat(path, item->DestinationFile().c_str());

        uint32 urlLength = strlen(path) + 10;
        char *url = new char[urlLength];
        
        if (IsntError(FilePathToURL(path, url, &urlLength)))
        {
            m_context->browser->WriteMetaDataToDatabase(url, 
                                                        item->GetMetaData());
            m_context->browser->m_catalog->AddSong(url);
        }

        delete [] url;
    }

    return result;
}

void DownloadManager::DownloadThreadFunction()
{
    m_quitMutex.Acquire();

    while(m_runDownloadThread)
    {
        DownloadItem* item = GetNextQueuedItem();
        Error result;

        if(item)
        {
            item->SetState(kDownloadItemState_Downloading);
            SendStateChangedMessage(item);

            result = Download(item);

            if(IsntError(result))
            {
                item->SetState(kDownloadItemState_Done);

                result = SubmitToDatabase(item);
            }
            else if(result == kError_UserCancel)
            {
                if(item->GetState() == kDownloadItemState_Cancelled)
                {
                    CleanUpDownload(item);
                }
            }
            else
            {
                item->SetState(kDownloadItemState_Error);
                CleanUpDownload(item);
            }

            item->SetDownloadError(result);

            SendStateChangedMessage(item);
        }

        m_queueSemaphore.Wait();
    }

    m_quitMutex.Release();
}

void DownloadManager::download_thread_function(void* arg)
{
    DownloadManager* dlm = (DownloadManager*)arg;

    dlm->DownloadThreadFunction();
}

// Messaging functions

void DownloadManager::SendItemAddedMessage(DownloadItem* item)
{    
    m_context->target->AcceptEvent(new DownloadItemAddedEvent(item));
}

void DownloadManager::SendItemRemovedMessage(DownloadItem* item)
{    
    m_context->target->AcceptEvent(new DownloadItemRemovedEvent(item));
}

void DownloadManager::SendStateChangedMessage(DownloadItem* item)
{    
    m_context->target->AcceptEvent(new DownloadItemNewStateEvent(item));
}

void DownloadManager::SendProgressMessage(DownloadItem* item)
{    
    m_context->target->AcceptEvent(new DownloadItemProgressEvent(item));
}

void DownloadManager::SaveResumableDownloadItems()
{
    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    m_context->prefs->GetPrefString(kDatabaseDirPref, path, &length);
    
    if(DoesDBDirExist(path))
    {
        strcat(path, DIR_MARKER_STR);
        strcat(path, "ResumeDownloadDB");

        Database database(path);  

        if(database.Working())
        {
            uint32 size = 0;
            DownloadItem* item = NULL;

            size = m_itemList.size();

            for(uint32 index = 0; index < size; index++)
            {
                item = m_itemList[index];

                if(item && item->GetState() == kDownloadItemState_Paused)
                {
                    ostringstream ost;
                    char num[256];
                    const char* kDatabaseDelimiter = " ";
                    MetaData metadata = item->GetMetaData();

                    // write out the number of elements we have
                    ost << 15 << kDatabaseDelimiter;
                    // next record the length of each element
                    ost << item->SourceURL().size() << kDatabaseDelimiter;
                    ost << item->SourceCookie().size() << kDatabaseDelimiter;
                    ost << item->DestinationFile().size() << kDatabaseDelimiter;
                    ost << item->PlaylistName().size() << kDatabaseDelimiter;

                    sprintf(num, "%d", item->GetTotalBytes());
                    ost << strlen(num) << kDatabaseDelimiter;
                    sprintf(num, "%d", item->GetBytesReceived());
                    ost << strlen(num) << kDatabaseDelimiter;

                    // metadata lengths
                    ost << metadata.Artist().size() << kDatabaseDelimiter;
                    ost << metadata.Album().size() << kDatabaseDelimiter;
                    ost << metadata.Title().size() << kDatabaseDelimiter;
                    ost << metadata.Comment().size() << kDatabaseDelimiter;
                    ost << metadata.Genre().size() << kDatabaseDelimiter;

                    sprintf(num, "%d", metadata.Year());
                    ost << strlen(num) << kDatabaseDelimiter;
                    sprintf(num, "%d", metadata.Track());
                    ost << strlen(num) << kDatabaseDelimiter;
                    sprintf(num, "%d", metadata.Time());
                    ost << strlen(num) << kDatabaseDelimiter;
                    sprintf(num, "%d", metadata.Size());
                    ost << strlen(num) << kDatabaseDelimiter;

                    // now stuff all the data in there
                    ost << item->SourceURL();
                    ost << item->SourceCookie();
                    ost << item->DestinationFile();
                    ost << item->PlaylistName();
                    ost << item->GetTotalBytes();
                    ost << item->GetBytesReceived();
                    ost << metadata.Artist();
                    ost << metadata.Album();
                    ost << metadata.Title();
                    ost << metadata.Comment();
                    ost << metadata.Genre();
                    ost << metadata.Year();
                    ost << metadata.Track();
                    ost << metadata.Time();
                    ost << metadata.Size();
                    ost << '\0';     
          
#ifdef WIN32 
                    database.Insert(path, (char*)ost.str().c_str());  
#else
                    database.Insert(path, (char*)ost.str());
#endif
                }
            }
        }
    }
}

void DownloadManager::LoadResumableDownloadItems()
{
    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    m_context->prefs->GetPrefString(kDatabaseDirPref, path, &length);

    if(DoesDBDirExist(path))
    {
        strcat(path, DIR_MARKER_STR);
        strcat(path, "ResumeDownloadDB");

        Database database(path);  

        if(database.Working())
        {
            char *key = NULL;

            while((key = database.NextKey(key))) 
            {
                char* value = database.Value(key);

                if(!value)
                    continue;

                uint32 numFields, offset = 0;
 
                sscanf(value, "%d%n", &numFields, &offset);
                uint32* fieldLength =  new uint32[numFields];
 
                for(uint32 i = 0; i < numFields; i++)
                {
                   uint32 temp;
 
                   sscanf(value + offset, " %d %n", &fieldLength[i], &temp);
                   printf("field %d: %d\n", i, fieldLength[i]);
                   if (i == numFields - 1) {
                       char intholder[10];
                       sprintf(intholder, "%d", fieldLength[i]);
                       offset += strlen(intholder) + 1;
                   }
                   else
                       offset += temp;
                }

                string data = value;
                data.erase(0, offset);

                DownloadItem* item = new DownloadItem();
                MetaData metadata;
                uint32 count = 0;

                for(uint32 j = 0; j < numFields; j++)
                {
                    switch(j)
                    {
                        case 0:
                            item->SetSourceURL(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 1:
                            item->SetSourceCookie(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 2:
                            item->SetDestinationFile(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 3:
                            item->SetPlaylistName(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 4:
                            item->SetTotalBytes(atoi(data.substr(count, fieldLength[j]).c_str()));
                            break;
                        case 5:
                            item->SetBytesReceived(atoi(data.substr(count, fieldLength[j]).c_str()));
                            break;
                        case 6:
                            metadata.SetArtist(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 7:
                            metadata.SetAlbum(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 8:
                            metadata.SetTitle(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 9:
                            metadata.SetComment(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 10:
                            metadata.SetGenre(data.substr(count, fieldLength[j]).c_str());
                            break;
                        case 11:
                            metadata.SetYear(atoi(data.substr(count, fieldLength[j]).c_str()));
                            break;
                        case 12:
                            metadata.SetTrack(atoi(data.substr(count, fieldLength[j]).c_str()));
                            break;
                        case 13:
                            metadata.SetTime(atoi(data.substr(count, fieldLength[j]).c_str()));
                            break;
                        case 14:
                            metadata.SetSize(atoi(data.substr(count, fieldLength[j]).c_str()));
                            break;
                        default:
                            break;

                    }

                    count += fieldLength[j];
                }

                delete [] fieldLength;

                fieldLength = NULL;

                item->SetMetaData(&metadata);
                item->SetState(kDownloadItemState_Paused);

                m_itemList.push_back(item);
                SendItemAddedMessage(item);

                database.Remove(key);
            }
        }
    }
}

bool DownloadManager::DoesDBDirExist(char* path)
{
    bool result = false;
    struct stat st;

    if(-1 != stat(path, &st))
    {
        if(st.st_mode & S_IFDIR)
            result = true;
    }

    return result;
}

