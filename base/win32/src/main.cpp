/*____________________________________________________________________________
	
	FreeAMP - The Free MP3 Player
	Portions copyright (C) 1998-1999 EMusic.com

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
#include "player.h"
#include "event.h"
#include "registrar.h"
#include "registry.h"
#include "log.h"
#include "facontext.h"
#include "win32prefs.h"
#include "thread.h"
#include "utility.h"

void CreateHiddenWindow(void* arg);
void SendCommandLineToHiddenWindow(void);
void ReclaimFileTypes(const char* path, bool askBeforeReclaiming);

const char* kHiddenWindow = "FreeAmp Hidden Window";
HINSTANCE g_hinst = NULL;
static const char *themeExtension = "fat";

int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
		 			 LPSTR lpszCmdLine, 
					 int cmdShow)
{
    HANDLE runOnceMutex;
    
    g_hinst = hInstance;

    runOnceMutex = CreateMutex(	NULL,
							    TRUE,
							    BRANDING " Should Only Run One Time!");

    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        SendCommandLineToHiddenWindow();
        
        CloseHandle(runOnceMutex);
        return 0;
    }

    WSADATA sGawdIHateMicrosoft;
    WSAStartup(0x0002,  &sGawdIHateMicrosoft);

    FAContext *context = new FAContext;

    context->prefs = new Win32Prefs();
    context->log = new LogFile("freeamp.log");

    // find all the plug-ins we use
    Registrar* registrar;
    Registry* lmc;
    Registry* pmi;
    Registry* pmo;
    Registry*  ui;

    registrar = new Registrar;

    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.lmc");
    lmc = new Registry;
    registrar->InitializeRegistry(lmc, context->prefs);

    registrar->SetSearchString("*.pmi");
    pmi = new Registry;
    registrar->InitializeRegistry(pmi, context->prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new Registry;
    registrar->InitializeRegistry(pmo, context->prefs);

    registrar->SetSearchString("*.ui");
    ui = new Registry;
    registrar->InitializeRegistry(ui, context->prefs);

    delete registrar;

    bool reclaimFileTypes, askBeforeReclaiming;
    char path[MAX_PATH];
    uint32 length = sizeof(path);
    context->prefs->GetReclaimFiletypes(&reclaimFileTypes);
    context->prefs->GetAskToReclaimFiletypes(&askBeforeReclaiming);
    context->prefs->GetInstallDirectory(path, &length);
    strcat(path, "\\freeamp.exe");

    if(reclaimFileTypes)
        ReclaimFileTypes(path, askBeforeReclaiming);

    // create the player
	Player *player = Player::GetPlayer(context);

    // we are the first instance so create our hidden window
    Thread* thread = Thread::CreateThread();

    thread->Create(CreateHiddenWindow, context);

    // need a way to signal main thread to quit...
    Semaphore *termination = new Semaphore();
    
    // register items... we give up ownership here
    player->SetTerminationSemaphore(termination);
    player->RegisterLMCs(lmc);
    player->RegisterPMIs(pmi);
    player->RegisterPMOs(pmo);
    player->RegisterUIs(ui);

    // Let the player know if there are special requests from the user
    // __argc and __argv are magical variables provided for us
    // in MS's STDLIB.H file. 
    player->SetArgs(__argc, __argv);

    // kick things off... player is now in charge!
    player->Run();

    // sit around and twiddle our thumbs
    termination->Wait();

    // clean up our act
    delete player;
	delete context;
    delete thread;

    CloseHandle(runOnceMutex);

	WSACleanup();

	return 0;
}

static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
    BOOL    result = TRUE;
    char    windowTitle[256];
    int32   count;

    count = GetWindowText(hwnd, windowTitle, sizeof(windowTitle));

    // If we found our main window, stop the enumeration
    if (!strncmp(windowTitle, kHiddenWindow, strlen(kHiddenWindow)))
    {
        HWND* phwnd = (HWND*)lParam;
        *phwnd = hwnd;
        result =  FALSE;
    }

    return result;
}

void SendCommandLineToHiddenWindow()
{
    HWND hwnd = NULL;

    // we give it five attempts and then give up
    for(uint32 count = 0; count < 5; count++)
    {
        // find currently running FreeAmp player
        EnumWindows(EnumWndProc, (LPARAM)&hwnd);

        if(hwnd)
        {
            COPYDATASTRUCT data;

            if(__argc > 1)
            {
                char* filelist = NULL;
                size_t length = 0;

                for(int32 i = 1; i < __argc; i++)
                {
                    int32 argLength = strlen(__argv[i]) + 1;
                
                    filelist = (char*)realloc(filelist, length + argLength);

                    strcpy(filelist + length, __argv[i]);

                    length += argLength;
                }

                data.dwData = __argc - 1;
                data.cbData = length;
                data.lpData = filelist;

                SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&data);

                free(filelist);
            }

            break;
        }

        Sleep(500);
    }
}

static LRESULT WINAPI HiddenWndProc(HWND hwnd, 
                                    UINT msg, 
                                    WPARAM wParam, 
                                    LPARAM lParam )
{
    LRESULT result = 0;
    FAContext* context = (FAContext*)GetWindowLong(hwnd, GWL_USERDATA);

    switch (msg)
    {
        case WM_CREATE:
        {
            context = (FAContext*)((LPCREATESTRUCT)lParam)->lpCreateParams;

            assert(context != NULL);
          
            result = SetWindowLong(hwnd, GWL_USERDATA, (LONG)context);
                        
            break;
        }

        case WM_COPYDATA:
        {
            COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
            int32 count = pcds->dwData;
            char* array = (char*)pcds->lpData;
            char path[MAX_PATH];
            char url[MAX_PATH + 7];
            uint32 length = sizeof(url);
            int offset = 0;


            for(int32 i = 0; i < count; i++)
            {
                strcpy(path, array + offset);
                offset += strlen(path) + 1;

                // is this a URL we know how to handle
                if( !strncasecmp(path, "http://", 7) ||
                    !strncasecmp(path, "rtp://", 6))
                {
                    context->plm->AddItem(path);
                    continue;
                }

                HANDLE handle;
                WIN32_FIND_DATA data;

                handle = FindFirstFile(path, &data);

                if(handle != INVALID_HANDLE_VALUE)
                {
                    char* cp = NULL;

                    cp = strrchr(path, '\\');

                    if(cp)
                    {
                        strcpy(cp + 1, data.cFileName);
                    }

                    FindClose(handle);
                }

                FilePathToURL(path, url, &length);

                // who needs to get this, plm or dlm?
                bool giveToDLM = false;
                bool giveToTheme = false;
                char* extension = NULL;
                PlaylistManager* plm = context->plm;
                DownloadManager* dlm = context->downloadManager;

                extension = strrchr(url, '.');
                if(extension)
                {
                    DownloadFormatInfo dlfi;
                    uint32 i = 0;

                    extension++;

                    while(IsntError(dlm->GetSupportedDownloadFormats(&dlfi, i++)))
                    {
                        if(!strcasecmp(extension, dlfi.GetExtension()))
                        {
                            giveToDLM = true;
                            break;
                        }
                    }
                    if (strcasecmp(extension, themeExtension) == 0)
                        giveToTheme = true; 
                }

                if(giveToDLM)
                    dlm->ReadDownloadFile(url);
                else if(giveToTheme)
                    context->target->AcceptEvent(new LoadThemeEvent(url, ""));
                else
                    plm->AddItem(url);
            }
            
            break;
        }

        default:
            result = DefWindowProc(hwnd, msg, wParam, lParam);
            break;

    }

    return result;
}


void CreateHiddenWindow(void* arg)
{
    FAContext* context = (FAContext*)arg;
    WNDCLASS wc;
    MSG      msg;
    HWND     hwnd;

    memset(&wc, 0x00, sizeof(WNDCLASS));

    wc.style = CS_OWNDC|CS_DBLCLKS;
    wc.lpfnWndProc = HiddenWndProc;
    wc.hInstance = g_hinst;
    wc.hCursor = NULL;
    wc.hIcon = NULL;
    wc.hbrBackground = NULL;
    wc.lpszClassName = kHiddenWindow;

    RegisterClass(&wc);
         
    hwnd = CreateWindow(kHiddenWindow, 
                        kHiddenWindow,
                        WS_POPUP, 
                        0, 
                        0, 
                        100, 
                        100,
                        NULL, 
                        NULL, 
                        g_hinst, 
                        context);

    if(hwnd)
    {
        while(GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

const char* kFileTypes[][2] = {
    {".mp1", "MP1AudioFile"},
    {".mp2", "MP2AudioFile"},
    {".mp3", "MP3AudioFile"},
    {".m3u", "M3UPlaylistFile"},
    {".pls", "PLSPlaylistFile"},
    {NULL, NULL}
};

const char* kOpenCommand = "\\shell\\open\\command";
const char* kNotifyStolen = "File types normally associated with " BRANDING "\r\n"
                            "have been associated with another application.\r\n"
                            "Do you want to reclaim these file types?";

void ReclaimFileTypes(const char* path, bool askBeforeReclaiming)
{
    LONG    result; 
    DWORD   index;
    HKEY    typeKey;
    HKEY    appKey;
    DWORD   type;
    char    buf[MAX_PATH];
    DWORD   len = sizeof(buf);
    bool    permission = false;

    if(!askBeforeReclaiming)
        permission = true;

    // reclaim the windows filetypes
    for(index = 0; ; index++)
    {
        if(kFileTypes[index][1] == NULL)
            break;

        result = RegOpenKeyEx(HKEY_CLASSES_ROOT,
							  kFileTypes[index][1],
							  0, 
                              KEY_ALL_ACCESS,
                              &typeKey);
        if(typeKey)
	    {
            len = sizeof(buf);
		    result = RegQueryValueEx(typeKey,
                                     NULL, 
                                     NULL, 
                                     &type, 
                                     (LPBYTE)buf, 
                                     &len);

            if(result == ERROR_SUCCESS)
            {
                //MessageBox(NULL, buf, "value", MB_OK);
                if(strcmp(buf, kFileTypes[index][2]))
                {
                    if(!permission)
                    {
                        int ret;
                        ret = MessageBox(NULL, 
                                   kNotifyStolen,
                                   "Reclaim File Types?", 
                                   MB_OKCANCEL|MB_ICONQUESTION);

                        if(ret == IDOK)
                            permission = true;
                        else
                        {
                            RegCloseKey(typeKey);
                            return;
                        }

                    }

                    RegSetValueEx(typeKey,
                                  NULL, 
                                  NULL, 
                                  REG_SZ, 
                                  (LPBYTE)kFileTypes[index][2], 
                                  strlen(kFileTypes[index][2]) + 1);
                }
            }

            RegCloseKey(typeKey);
        }

        wsprintf(buf, "%s%s", kFileTypes[index][2], kOpenCommand);

        result = RegOpenKeyEx(	HKEY_CLASSES_ROOT,
							    buf,
							    0, 
							    KEY_ALL_ACCESS,
							    &appKey);
        if(appKey)
	    {
            len = sizeof(buf);
		    result = RegQueryValueEx(appKey,
                                     NULL, 
                                     NULL, 
                                     &type, 
                                     (LPBYTE)buf, 
                                     &len);

            if(result == ERROR_SUCCESS)
            {
                //MessageBox(NULL, buf, "value", MB_OK);
                if(strcmp(buf, path))
                {
                    RegSetValueEx(appKey,
                                  NULL, 
                                  NULL, 
                                  REG_SZ, 
                                  (LPBYTE)path, 
                                  strlen(path) + 1);
                }
            }

            RegCloseKey(appKey);
        }
    }

    // reclaim netscape filetypes
}
