/*____________________________________________________________________________
	
	FreeAMP - The Free MP3 Player
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
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>

/* Project Includes */
#include "registrar.h"
#include "utility.h"

Error RegisterLMCs(LMCRegistry* registry)
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

                LMCInfo* info = new LMCInfo;

                info->SetPath(file);
                info->SetDescription(find.cFileName);

                registry->Add(info);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

Error RegisterPMOs(PMORegistry* registry)
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

                PMOInfo* info = new PMOInfo;

                info->SetPath(file);
                info->SetDescription(find.cFileName);

                registry->Add(info);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

Error RegisterPMIs(PMIRegistry* registry)
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

                PMIInfo* info = new PMIInfo;

                info->SetPath(file);
                info->SetDescription(find.cFileName);

                registry->Add(info);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}

Error RegisterUIs(UIRegistry* registry)
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

                UIInfo* info = new UIInfo;

                info->SetPath(file);
                info->SetDescription(find.cFileName);

                registry->Add(info);

                //MessageBox(NULL, file, "Found File", MB_OK);
                
            }while(FindNextFile(handle, &find));

            FindClose(handle);
        }
    }

    return error;
}