/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1999 EMusic.com

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
#include <winsock.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

/* Project Includes */
#include "win32prefs.h"
#include "updatemanager.h"

const char* kMessage = "An instance of "BRANDING" is currently running. Please "
                       "close the application before continuing the update.";

void MoveFiles(const char* src, const char* dest);

int APIENTRY WinMain(	HINSTANCE hInstance, 
						HINSTANCE hPrevInstance,
		 				LPSTR lpszCmdLine, 
						int cmdShow)
{
    HANDLE runOnceMutex;

    runOnceMutex = CreateMutex(	NULL,
							    TRUE,
							    BRANDING" Should Only Run One Time!");

    // make sure FreeAmp is not running while this happens...
    while(WAIT_TIMEOUT == WaitForSingleObject(runOnceMutex, 0))
    {
        int32 result;

        result = MessageBox(NULL, kMessage, "Updating "BRANDING, MB_RETRYCANCEL);

        if(result == IDCANCEL)
        {
            CloseHandle(runOnceMutex);
            return 0;
        }
    }
    
    // get our handy dandy directories
    Win32Prefs prefs;
    char appPath[MAX_PATH];
    char updatePath[MAX_PATH];
    uint32 length = sizeof(appPath);

    prefs.GetPrefString(kInstallDirPref, appPath, &length);

    strcpy(updatePath, appPath);
    strcat(updatePath, "\\update");

    // Move all those new files
    MoveFiles(updatePath, appPath);

    CloseHandle(runOnceMutex);

	return 0;
}

void MoveFiles(const char* src, const char* dest)
{
    // iterate over the files in the update directory
    HANDLE findFileHandle = NULL;
    WIN32_FIND_DATA findData;
    char srcPath[MAX_PATH];
    char destPath[MAX_PATH];
    char* dp;
    char* sp;
    
    strcpy(srcPath, src);
    strcat(srcPath, "\\*.*");
    sp = strrchr(srcPath, '\\') + 1;

    strcpy(destPath, dest);
    strcat(destPath, "\\");
    dp = strrchr(destPath, '\\') + 1;

    findFileHandle = FindFirstFile(srcPath, &findData);

    if(findFileHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            strcpy(sp, findData.cFileName);
            strcpy(dp, findData.cFileName);

            // skip these two special entries
            if( strcmp(findData.cFileName, ".") && 
                strcmp(findData.cFileName, ".."))
            {
                if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
                {
                    // call ourselves on that directory
                    MoveFiles(srcPath, destPath);      
                }
                else 
                {
                    // actually move the file
                    if(!MoveFileEx(srcPath, destPath, MOVEFILE_REPLACE_EXISTING))
                    {
                        LPVOID lpMessageBuffer;

		                FormatMessage(
		                  FORMAT_MESSAGE_ALLOCATE_BUFFER |
		                  FORMAT_MESSAGE_FROM_SYSTEM,
		                  NULL,
		                  GetLastError(),
		                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		                  (LPTSTR) &lpMessageBuffer,
		                  0,
		                  NULL );

		                // now display this string
 		                MessageBox(NULL, (char*)lpMessageBuffer, 0, MB_OK);

		                // Free the buffer allocated by the system
		                LocalFree( lpMessageBuffer );
                    }
                }
            }

        }while(FindNextFile(findFileHandle, &findData));

        FindClose(findFileHandle);
    }
}
