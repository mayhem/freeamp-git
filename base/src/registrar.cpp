/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1998 GoodNoise

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

/* System Includes */
#define STRICT
#ifdef WIN32
#include <windows.h>
#else
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#endif // WIN32

#include <stdio.h>
#include <string.h>
#include <iostream.h>

/* Project Includes */
#include "config.h"
#include "registrar.h"
#include "errors.h"
#include "hashtable.h"

Error 
Registrar::
InitializeRegistry(Registry* registry, Preferences* prefs)
{
    Error error = kError_NoErr;
    char dir[MAX_PATH];
    HashTable<int32 *> *pHT = new HashTable<int32 *>();

    if(registry == NULL)
        error = kError_InvalidParam;

    if(m_search == NULL)
        error = kError_InvalidParam;

    if(m_subdir == NULL)
        error = kError_InvalidParam;

    uint32 len = sizeof(dir);
    uint32 totalFilesFound = 0;
    HANDLE libDirHandle = (HANDLE)NULL;
    if(IsntError(error))
    {
#ifdef WIN32
        prefs->GetInstallDirectory(dir, &len);
#else
	libDirHandle = prefs->GetFirstLibDir(dir, &len);
#endif
    }

#ifdef WIN32
    if(IsntError(error))
#else
    while(libDirHandle && (error != kError_NoMoreLibDirs))
#endif
    {
#ifndef WIN32
        if (dir[0] == '~') {
	    if (dir[1] != '/') {
		// a ~ then a users name
		char newdir[MAX_PATH];
		char *tmp = strchr(dir,'/');
		char tmpChar = 0x00;
		if (tmp) { tmpChar = *tmp; *tmp = '\0'; }
		struct passwd *pwd = getpwnam(&(dir[1]));
		if (pwd) {
		    strcpy(newdir, pwd->pw_dir);
		    if (tmpChar) { *tmp = tmpChar; }
		    if (tmp) { strcat(newdir,tmp); }
		    memcpy(dir,newdir,MAX_PATH);
		} else {
		    cerr << "Couldn't get user's identity: " << &(dir[1]) << endl;
		}
	    } else {
		// just a ~
		char newdir[MAX_PATH];
		struct passwd *pwd = getpwuid(getuid());
		if (pwd) {
		    strcpy(newdir,pwd->pw_dir);
		    char *pSlash = strchr(dir,'/');
		    if (pSlash) {
			strcat(newdir,pSlash);
		    }
		    memcpy(dir,newdir,MAX_PATH);
		} else {
		    cerr << "Couldn't get user's identity: " << getuid() << endl;
		}
	    }
	}
#endif
	len = sizeof(dir);
        WIN32_FIND_DATA find;
        HANDLE handle;
        char search[MAX_PATH];

        if(*m_subdir != DIR_MARKER)
            strcat(dir, DIR_MARKER_STR);

        strcat(dir, m_subdir);

        strcpy(search, dir);

        if(*m_search != DIR_MARKER)
            strcat(search, DIR_MARKER_STR);

        strcat(search, m_search);

        handle = FindFirstFile(search, &find);

	//cerr << "Searching: " << search << endl;

        if(handle != INVALID_HANDLE_VALUE)
        {
            do
            {
#ifndef WIN32
		if (pHT) {
		    if (pHT->Value(find.cFileName)) {
			// only put things with same Name into registry once.
			continue;
		    }
		}
#endif
                char file[MAX_PATH];

		//cerr << "Found file: " << find.cFileName << endl;

                sprintf(file, "%s%s%s", dir, DIR_MARKER_STR, find.cFileName);

		//cerr << "Trying to load: " << file << endl;

                RegistryItem* item = new RegistryItem;

                item->SetPath(file);
                item->SetName(find.cFileName);
                item->SetDescription(find.cFileName);

                HMODULE module = NULL;
                error = kError_LoadLibFailed;

                /*LEAK*/module = LoadLibrary(file);
            
                if(module)
                {
                    InitializeFunction init = NULL;
                    error = kError_FindFuncFailed;

                    item->SetModule(module);

                    init = (InitializeFunction)GetProcAddress(module, "Initialize");

                    if(init)
                    {
                        error = kError_NoErr;
                        item->SetInitFunction(init);
			totalFilesFound++;
#ifndef WIN32
			int32 *pInt = new int32;
			*pInt = 1;
			pHT->Insert(find.cFileName,pInt);
#endif
                    }
                }
                
                if(IsntError(error))
                    registry->Add(item);
                else
                {
                    if(module)
                        FreeLibrary(module);

                    delete item;
                }
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
#ifndef WIN32
	    error = prefs->GetNextLibDir(libDirHandle,dir,&len);
#endif
    }

#ifndef WIN32
    if (libDirHandle) 
        prefs->GetLibDirClose(libDirHandle);
#endif
    if (pHT) {
	delete pHT;
	pHT = NULL;
    }
    if (totalFilesFound == 0) 
	    error = kError_NoFiles;

    return error;
}

Error 
Registrar::
CleanupRegistry(Registry* registry)
{
    Error           error   = kError_NoErr;
    RegistryItem*   item    = NULL;
    int32           index   = 0;

    while(item = registry->GetItem(index++))
    {
        HMODULE module = NULL;

        module = (HMODULE)item->Module();

        if(module)
            FreeLibrary(module);
    }
 
    return error;
}
