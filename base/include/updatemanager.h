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

#ifndef INCLUDED_UPDATE_MANAGER_H_
#define INCLUDED_UPDATE_MANAGER_H_

#include <assert.h>
#include <string>
#include <vector>
#include <deque>
#include <functional>

using namespace std;

#include "config.h"
#include "facontext.h"

#include "errors.h"
#include "mutex.h"
#include "semaphore.h"
#include "thread.h"
#include "Parse.h"

#define kInvalidIndex 0xFFFFFFFF

typedef enum {
    kUMEvent_Status,
    kUMEvent_Progress,
    kUMEvent_Error,
    kUMEvent_Done

} UMEventType;

#define kUMProgressInfinity -1

class UpdateItem;

typedef struct UMEventProgressData {
    uint32 position;
    uint32 total;
    UpdateItem* item;
} UMEventProgressData;

typedef struct UMEventErrorData {
    uint32 errorCode;
} UMEventErrorData;

typedef union UMEventData {
    UMEventProgressData progressData;
    UMEventErrorData    errorData;
} UMEventData;

typedef struct UMEvent {
    UMEventType type;
    UMEventData data;
    string eventString;
} UMEvent;

typedef bool (*UMCallBackFunction)(UMEvent* event, void* userData);

class UpdateItem {
 public:
    virtual ~UpdateItem() { }
    
    void SetLocalFileName(const string& name) { m_localFileName = name; }
    const string& GetLocalFileName() const { return m_localFileName; }

    void SetLocalFilePath(const string& path) { m_localFilePath = path; }
    const string& GetLocalFilePath() const { return m_localFilePath; }

    void SetLocalFileVersion(const string& version) { m_localVersion = version; }
    const string& GetLocalFileVersion() const { return m_localVersion; }

    void SetCurrentFileLocation(const string& location) { m_currentFileLocation = location; }
    const string& GetCurrentFileLocation() const { return m_currentFileLocation; }

    void SetCurrentFileURL(const string& url) { m_currentFileURL = url; }
    const string& GetCurrentFileURL() const { return m_currentFileURL; }

    void SetCurrentFileVersion(const string& version) { m_currentVersion = version; }
    const string& GetCurrentFileVersion() const { return m_currentVersion; }

    void SetFileDescription(const string& desc) { m_description = desc; }
    const string& GetFileDescription() const { return m_description; }

 protected:

 private:
    string m_localFileName;
    string m_localFilePath;
    string m_localVersion;

    string m_currentVersion;
    string m_currentFileLocation;
    string m_currentFileURL;
    string m_description;
};

class UpdateManager : public Parse {

 public:
    UpdateManager(FAContext* context);
    virtual ~UpdateManager();
    
    void SetPlatform(const string& platform) { m_currentPlatform = platform; }
    const string&  GetPlatform() { return m_currentPlatform; }
    virtual Error DetermineLocalVersions() { return kError_FeatureNotSupported; }
    Error RetrieveLatestVersionInfo(UMCallBackFunction function = NULL,
                                    void* cookie = NULL);
    Error UpdateComponents(UMCallBackFunction function = NULL,
                           void* cookie = NULL);

    // Utility Functions
    bool            IsEmpty();
    uint32          CountItems();
    UpdateItem*     ItemAt(uint32 index);
	uint32          IndexOf(UpdateItem* item);
    bool            HasItem(UpdateItem* item);

 protected:
    Error AddItem(UpdateItem* item);
    inline uint32 CheckIndex(uint32 index);

    Error DownloadInfo( string& info, 
                        UMCallBackFunction function,
                        void* cookie);

    Error DownloadItem( UpdateItem* item, 
                        UMCallBackFunction function,
                        void* cookie);
    
    Error ParseInfo(string& info);

    virtual Error BeginElement(string &element, AttrMap &attrMap);
    virtual Error EndElement(string &element);
    virtual Error PCData(string &data);

    Error InternalRetrieveLatestVersionInfo(
                                    UMCallBackFunction function = NULL,
                                    void* cookie = NULL);

 private:

    FAContext* m_context;

    Mutex m_mutex;

    vector<UpdateItem*> m_itemList;
   
    volatile bool m_runUpdateThread;
    Mutex m_quitMutex;

    string m_path;
    string m_versionPlatform;
    string m_currentPlatform;

};

#endif // INCLUDED_UPDATE_MANAGER_H_










