/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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
#define STRICT
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>

/* project headers */
#include "config.h"
#include "resource.h"
#include "utility.h"
#include "preferences.h"

const char* kSaveToRio = "<<< Save To Rio >>>";

HRGN
DetermineRegion(DIB* bitmap, 
			    Color* color)
{
	HRGN result = NULL;
    HRGN scanline = NULL;
    uint32 regionColor = color->Pack();
    int32 width, height;

    width = bitmap->Width(); 
    height = bitmap->Height();

    // empty region
    result = CreateRectRgn(0,0,0,0);

    for(int32 y = 0; y < height; y++)
    {
        bool scanning = false;
        int32 start;

        for(int32 x = 0; x < width; x++)
        {
            uint32 pixel = bitmap->Pixel(x, y);

            if(pixel == regionColor)
            {
                if(scanning)
                    continue;
                else
                {
                    scanning = true;
                    start = x;
                }
            }
            else if(scanning)
            {
                scanning = false;
                // regions are non-inclusive of their bottom & right edges
                // so need to add one to values
                scanline = CreateRectRgn(start, y, x, y + 1);

                CombineRgn( result,
                            result,
                            scanline,
                            RGN_OR);

                DeleteObject(scanline);

            }

        }

        // last pixel in scanline is included in region...
        if(scanning)
        {
            scanline = CreateRectRgn(start, y, x, y + 1);

            CombineRgn( result,
                        result,
                        scanline,
                        RGN_OR);

            DeleteObject(scanline);
        }
    }

	return result;
}

void
DetermineControlRegions(DIB* bitmap, 
                        HRGN* controlRegions,
			            Color* controlColors,
                        int32 numControls)
{
    HRGN scanline = NULL;
    uint32* regionColors;// = RGB(color->r, color->g, color->b);
    int32 width, height;
    int32 i;

    for(i = 0; i < numControls; i++)
    {
        // empty region
        /*LEAK-2*/controlRegions[i] = CreateRectRgn(0,0,0,0);
    }

    regionColors = new uint32[numControls];

    width = bitmap->Width(); 
    height = bitmap->Height();

    for(i = 0; i < numControls; i++)
    {
        regionColors[i] = controlColors[i].Pack();
    }

    uint32 black = RGB(0, 0, 0);
    uint32 white = RGB(255, 255, 255);

    for(int32 y = 0; y < height; y++)
    {
        bool scanning = false;
        int32 scanIndex;
        int32 start;

        for(int32 x = 0; x < width; x++)
        {
            uint32 pixel = bitmap->Pixel(x,y);

            if(scanning )
            {
                if(pixel == regionColors[scanIndex])
                    continue;
                else
                {
                    scanning = false;
                    // regions are non-inclusive of their bottom
                    // & right edges so need to add one
                    scanline = CreateRectRgn(start, y, x, y + 1);

                    CombineRgn( controlRegions[scanIndex],
                                controlRegions[scanIndex],
                                scanline,
                                RGN_OR);

                    DeleteObject(scanline);
                }

                if(pixel != black && pixel != white)
                {
                    for(i = 0; i < numControls; i++)
                    {
                        if(pixel == regionColors[i])
                        {
                            scanIndex = i;
                            scanning = true;
                            start = x;
                            break;
                        }
                    }
                }
            }
            else
            {
                if(pixel != black && pixel != white)
                {
                    for(i = 0; i < numControls; i++)
                    {
                        if(pixel == regionColors[i])
                        {
                            scanIndex = i;
                            scanning = true;
                            start = x;
                            break;
                        }
                    }
                }
            }
        }

        // last pixel in scanline is included in region...
        if(scanning)
        {
            scanline = CreateRectRgn(start, y, x, y + 1);

            CombineRgn( controlRegions[scanIndex],
                        controlRegions[scanIndex],
                        scanline,
                        RGN_OR);

            DeleteObject(scanline);
        }
    }

    delete [] regionColors;
}

                        
static
UINT 
APIENTRY 
OpenFileHookProc(   HWND hwnd, 
                    UINT msg, 
                    WPARAM wParam, 
                    LPARAM lParam)
{
    UINT result = 0;
    static OPENFILENAME* ofn = NULL;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            ofn = (OPENFILENAME*)lParam;

            break;
        }

        case WM_COMMAND:
        {
            switch(wParam)
            {
                case IDC_OPEN_URL:
                {
                    char* url = new char[ofn->nMaxFile + 1];

                    if(GetDlgItemText(  hwnd,
                                        IDC_URL,
                                        url,
                                        ofn->nMaxFile))
                    {
                        *ofn->lpstrFile = 0x00;

                        if(!strstr(url, "://"))
                        {
                            strcpy(ofn->lpstrFile, "http://");
                        }

                        strcat(ofn->lpstrFile, url);

                        PostMessage(GetParent(hwnd), 
                                    WM_COMMAND, 
                                    IDCANCEL,
                                    0);

                        ofn->lCustData = 1;
                    }

                    delete url;
                    
                    result = 1;
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            OFNOTIFY* notify = (OFNOTIFY*)lParam;

            switch(notify->hdr.code)
            {
                case CDN_FILEOK:
                {
                    
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool 
FileOpenDialog(HWND hwnd, 
               const char* title,
               const char* filter,
               List<char*>* fileList,
               Preferences* prefs)
{
    bool result = false;
    OPENFILENAME ofn;
    char szInitialDir[MAX_PATH + 1] = {0x00};
    uint32 initialDirSize = sizeof(szInitialDir);
    const int32 kBufferSize = MAX_PATH * 128;
    char* fileBuffer = new char[kBufferSize];

    *fileBuffer = 0x00;

    if(prefs)
    {
        prefs->GetOpenSaveDirectory( szInitialDir, &initialDirSize);
    }

    // Setup open file dialog box structure
    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
    ofn.hInstance         = (HINSTANCE)GetWindowLong(hwnd, 
                                                     GWL_HINSTANCE);
    ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = fileBuffer;
    ofn.nMaxFile          = kBufferSize;
    ofn.lpstrFileTitle    = NULL;
    ofn.nMaxFileTitle     = 0;
    ofn.lpstrInitialDir   = szInitialDir;
    ofn.lpstrTitle        = title;
    ofn.Flags             = OFN_FILEMUSTEXIST | 
					        OFN_PATHMUSTEXIST |
  	     			        OFN_HIDEREADONLY | 
					        OFN_ALLOWMULTISELECT |
					        OFN_EXPLORER |
                            OFN_ENABLEHOOK |
                            OFN_ENABLETEMPLATE;
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = "MP3";
    ofn.lCustData         = 0;
    ofn.lpfnHook          = OpenFileHookProc;
    ofn.lpTemplateName    = MAKEINTRESOURCE(IDD_OPENURL);

    if(GetOpenFileName(&ofn) || ofn.lCustData)
    {
        char file[MAX_PATH + 1];
        char* cp = NULL;

        if(ofn.lCustData) // URL
        {
            char* foo = new char[strlen(fileBuffer) + 1];

            strcpy(foo, fileBuffer);

            fileList->AddItem(foo);
        }
        else // potential list of files
        {
            strncpy(file, fileBuffer, ofn.nFileOffset);

            if(*(fileBuffer + ofn.nFileOffset - 1) != DIR_MARKER)
                strcat(file, DIR_MARKER_STR);

            cp = fileBuffer + ofn.nFileOffset;

            while(*cp)
            {
	            strcpy(file + ofn.nFileOffset, cp);

                char* foo = new char[strlen(file) + 1];

                strcpy(foo, file);

                fileList->AddItem(foo);

	            cp += strlen(cp) + 1;
            }

            *(fileBuffer + ofn.nFileOffset - 1) = 0x00;

            if(prefs)
            {
                prefs->SetOpenSaveDirectory(fileBuffer);
            }
        }

        result = true;
    }

    delete [] fileBuffer;

    return result;
}

static
UINT 
APIENTRY 
SaveFileHookProc(   HWND hwnd, 
                    UINT msg, 
                    WPARAM wParam, 
                    LPARAM lParam)
{
    UINT result = 0;
    static OPENFILENAME* ofn = NULL;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            ofn = (OPENFILENAME*)lParam;

            break;
        }

        case WM_COMMAND:
        {
            switch(wParam)
            {
                case IDC_SAVE_RIO:
                {
                    PostMessage(GetParent(hwnd), 
                                WM_COMMAND, 
                                IDCANCEL,
                                0);

                    ofn->lCustData = 1;
                    
                    result = 1;
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            OFNOTIFY* notify = (OFNOTIFY*)lParam;

            switch(notify->hdr.code)
            {
                case CDN_FILEOK:
                {
                    
                    break;
                }
            }

            break;
        }
    }

    return result;
}

bool 
FileSaveDialog( HWND hwnd, 
                const char* title,
                const char* filter,
                char* path,
                uint32* pathLength,
                Preferences* prefs)
{
    bool result = false;
    OPENFILENAME ofn;
    char szInitialDir[MAX_PATH + 1] = {0x00};
    char szFile[MAX_PATH + 1] = {0x00};
    uint32 initialDirSize = sizeof(szInitialDir);
  
    *path = 0x00;

    if(prefs)
    {
        prefs->GetOpenSaveDirectory( szInitialDir, &initialDirSize);
    }

    // Setup open file dialog box structure
    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
    ofn.hInstance         = (HINSTANCE)GetWindowLong(hwnd, 
                                                     GWL_HINSTANCE);
    ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = szFile;
    ofn.nMaxFile          = sizeof(szFile);
    ofn.lpstrFileTitle    = NULL;
    ofn.nMaxFileTitle     = 0;
    ofn.lpstrInitialDir   = szInitialDir;
    ofn.lpstrTitle        = title;
    ofn.Flags             = OFN_HIDEREADONLY | 
                            OFN_OVERWRITEPROMPT |
					        OFN_EXPLORER|
                            OFN_ENABLEHOOK |
                            OFN_ENABLETEMPLATE;
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = "M3U";
    ofn.lCustData         = 0;
    ofn.lpfnHook          = SaveFileHookProc;
    ofn.lpTemplateName    = MAKEINTRESOURCE(IDD_SAVERIO);;

    if(GetSaveFileName(&ofn) || ofn.lCustData)
    {
        if(ofn.lCustData)
        {
            strcpy(path, kSaveToRio);
        }
        else
        {
            if(*pathLength > strlen(szFile))
            {
                strcpy(path, szFile);
                result = true;
            }
            else
            {
                *pathLength = strlen(szFile) + 1;
            }

            *(szFile + ofn.nFileOffset - 1) = 0x00;

            if(prefs)
            {
                prefs->SetOpenSaveDirectory(szFile);
            }
        }

        result = true;
    }
    else
    {
        int32 error = CommDlgExtendedError();
    }

    return result;
}

static 
bool
InstallDriver(  SC_HANDLE  scManager,
                const char* driverName,
                const char* driverPath)

{
    bool result = false;
    SC_HANDLE  service;

    service = CreateService(scManager,              // SCManager database
                            driverName,             // name of service
                            driverName,             // name to display
                            SERVICE_ALL_ACCESS,     // desired access
                            SERVICE_KERNEL_DRIVER,  // service type
                            SERVICE_DEMAND_START,   // start type
                            SERVICE_ERROR_NORMAL,   // error control type
                            driverPath,             // service's binary
                            NULL,                   // no load ordering group
                            NULL,                   // no tag identifier
                            NULL,                   // no dependencies
                            NULL,                   // LocalSystem account
                            NULL);                  // no password


                                
    if(service || ERROR_SERVICE_EXISTS == GetLastError())
    {
        result = true;
    }
    /*else
    {
        LPVOID lpMessageBuffer;
        int32 error = GetLastError();

		FormatMessage(
		  FORMAT_MESSAGE_ALLOCATE_BUFFER |
		  FORMAT_MESSAGE_FROM_SYSTEM,
		  NULL,
		  error,
		  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		  (LPTSTR) &lpMessageBuffer,
		  0,
		  NULL );

		// now display this string
 		MessageBox(NULL, (char*)lpMessageBuffer, 0, MB_OK);

		// Free the buffer allocated by the system
		LocalFree( lpMessageBuffer );

    }*/
    
    CloseServiceHandle(service);

    return result;
}

static
bool
StartDriver(SC_HANDLE scManager,
            const char* driverName)
{
    bool result = false;
    SC_HANDLE  service;

    service = OpenService(  scManager,
                            driverName,
                            SERVICE_ALL_ACCESS);

    if(service)
    {
        BOOL error;

        error = StartService(service, // service identifier
                             0,       // number of arguments
                             NULL);   // pointer to arguments

        result = (error > 0);

        CloseServiceHandle(service);    
    }
    
    return result;
}

static
bool
OpenDevice(const char* driverName)
{
    bool    result = false;
    char    path[MAX_PATH];
    HANDLE  handle;

    sprintf(path, "\\\\.\\%s", driverName);

    handle = CreateFile(path,
                        GENERIC_READ | GENERIC_WRITE,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

    if(handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        result = true;
    }
   
    return result;
}


bool 
LoadDriver(const char* driverName, 
           const char* driverPath)
{

    bool result = false;
    SC_HANDLE scManager;

    scManager = OpenSCManager(NULL,
                              NULL,
                              SC_MANAGER_ALL_ACCESS);

    if(scManager)
    {
        result = InstallDriver( scManager,
                                driverName,
                                driverPath);
        if(result)
        {
            result = StartDriver(scManager,
                                 driverName);

            if(result)
            {
                result = OpenDevice(driverName);
            }
        }

        CloseServiceHandle(scManager);
    }

    return result;
}

static
bool
StopDriver( SC_HANDLE scManager,
            const char* driverName)
{
    bool result = false;
    SC_HANDLE service;
    SERVICE_STATUS  serviceStatus;

    service = OpenService(  scManager,
                            driverName,
                            SERVICE_ALL_ACCESS);

    if(service)
    {
        BOOL error;

        error = ControlService( service,
                                SERVICE_CONTROL_STOP,
                                &serviceStatus);

        result = (error > 0);

        CloseServiceHandle(service);
    }

    return result;
}

static
bool
RemoveDriver(   SC_HANDLE scManager,
                const char* driverName)
{
    bool result = false;
    SC_HANDLE  service;

    service = OpenService(  scManager,
                            driverName,
                            SERVICE_ALL_ACCESS);
    if(service)
    {
        BOOL error;

        error = DeleteService(service);

        result = (error > 0);

        CloseServiceHandle(service);
    }

    return result;
}

bool 
UnloadDriver(const char* driverName)
{
    bool result = false;
    SC_HANDLE scManager;

    scManager = OpenSCManager(NULL,
                              NULL,
                              SC_MANAGER_ALL_ACCESS);

    if(scManager)
    {
        result = StopDriver(scManager,
                            driverName);

        if(result)
        {
            result = RemoveDriver(  scManager, 
                                    driverName);
        }

        CloseServiceHandle(scManager);
    }

    return result;
}

