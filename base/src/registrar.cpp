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
#endif // WIN32

#include <stdio.h>
#include <string.h>
#include <iostream.h>

/* Project Includes */
#include "config.h"
#include "registrar.h"
#include "utility.h"
#include "error.h"

Error 
Registrar::
InitializeRegistry(Registry* registry)
{
    Error error = kError_NoErr;
    char dir[MAX_PATH];

    if(registry == NULL)
        error = kError_InvalidParam;

    if(m_search == NULL)
        error = kError_InvalidParam;

    if(m_subdir == NULL)
        error = kError_InvalidParam;

    if(IsntError(error))
        error = GetInstallDirectory(dir, sizeof(dir));

    if(IsntError(error))
    {
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

        if(handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                char file[MAX_PATH];
#ifdef WIN32
                sprintf(file, "%s\\%s", dir, find.cFileName);
#else
		sprintf(file, "%s/%s",dir, find.cFileName);
#endif

                RegistryItem* item = new RegistryItem;

                item->SetPath(file);
                item->SetName(find.cFileName);
                item->SetDescription(find.cFileName);

                HMODULE module = NULL;
                error = kError_LoadLibFailed;

                module = LoadLibrary(file);
            
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

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
        else
        {
            error = kError_NoFiles;
        }
    }

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
/*
Error InitializeLMCRegistry(LMCRegistry* registry)
{
    Error error = kError_NoErr;
    char dir[MAX_PATH];

    if(registry == NULL)
        error = kError_InvalidParam;

    if(IsntError(error))
        error = GetInstallDirectory(dir, sizeof(dir));

    if(IsntError(error))
    {
        WIN32_FIND_DATA find;
        HANDLE handle;
        char search[MAX_PATH];

        strcpy(search, dir);
        strcat(search, "\\lmc\\*.lmc");

        handle = FindFirstFile(search, &find);

        if(handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                char file[MAX_PATH];

                sprintf(file, "%s\\%s", dir, find.cFileName);

                LMCItem* item = new LMCItem;

                item->SetPath(file);
                item->SetName(find.cFileName);
                item->SetDescription(find.cFileName);

                HMODULE module = NULL;
                error = kError_LoadLibFailed;

                module = LoadLibrary(file);
            
                if(module)
                {
                    InitializeFunction init = NULL;
                    error = kError_FindFuncFailed;

                    init = (InitializeFunction)GetProcAddress(module, "Initialize");

                    if(init)
                        error = kError_NoErr;
                }
                
                if(IsntError(error))
                    registry->Add(item);
                else
                    delete item;

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

Error InitializePMORegistry(PMORegistry* registry)
{
    Error error = kError_NoErr;
    char dir[MAX_PATH];

    if(registry == NULL)
        error = kError_InvalidParam;

    if(IsntError(error))
        error = GetInstallDirectory(dir, sizeof(dir));

    if(IsntError(error))
    {
        WIN32_FIND_DATA find;
        HANDLE handle;
        char search[MAX_PATH];

        strcpy(search, dir);
        strcat(search, "\\io\\*.pmo");

        handle = FindFirstFile(search, &find);

        if(handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                char file[MAX_PATH];

                sprintf(file, "%s\\%s", dir, find.cFileName);

                PMOItem* item = new PMOItem;

                item->SetPath(file);
                item->SetName(find.cFileName);
                item->SetDescription(find.cFileName);

                registry->Add(item);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

Error InitializePMIRegistry(PMIRegistry* registry)
{
    Error error = kError_NoErr;
    char dir[MAX_PATH];

    if(registry == NULL)
        error = kError_InvalidParam;

    if(IsntError(error))
        error = GetInstallDirectory(dir, sizeof(dir));

    if(IsntError(error))
    {
        WIN32_FIND_DATA find;
        HANDLE handle;
        char search[MAX_PATH];

        strcpy(search, dir);
        strcat(search, "\\io\\*.pmi");

        handle = FindFirstFile(search, &find);

        if(handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                char file[MAX_PATH];

                sprintf(file, "%s\\%s", dir, find.cFileName);

                PMIItem* item = new PMIItem;

                item->SetPath(file);
                item->SetName(find.cFileName);
                item->SetDescription(find.cFileName);

                registry->Add(item);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

Error InitializeUIRegistry(UIRegistry* registry)
{
    Error error = kError_NoErr;
    char dir[MAX_PATH];

    if(registry == NULL)
        error = kError_InvalidParam;

    if(IsntError(error))
        error = GetInstallDirectory(dir, sizeof(dir));

    if(IsntError(error))
    {
        WIN32_FIND_DATA find;
        HANDLE handle;
        char search[MAX_PATH];

        strcpy(search, dir);
        strcat(search, "\\ui\\*.ui");

        handle = FindFirstFile(search, &find);

        if(handle != INVALID_HANDLE_VALUE)
        {
            do
            {
                char file[MAX_PATH];

                sprintf(file, "%s\\%s", dir, find.cFileName);

                UIItem* item = new UIItem;

                item->SetPath(file);
                item->SetName(find.cFileName);
                item->SetDescription(find.cFileName);

                registry->Add(item);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

*/
