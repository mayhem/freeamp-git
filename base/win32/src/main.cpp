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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

/* System Includes */
#include <windows.h>
#include <winsock.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>

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
bool SendCommandLineToRealJukebox(void);
void ReclaimFileTypes(const char* path, bool askBeforeReclaiming);
bool IsWinNT(void);
bool IsMultiProcessor(void);

const char* kHiddenWindow = "FreeAmp Hidden Window";
HINSTANCE g_hinst = NULL;
static const char *themeExtension = "fat";
const char* kReboot = The_BRANDING" needs for you to restart your computer "
                      "in order to complete the update process.\r\nPlease "
                      "restart your computer before attempting to use "
                      the_BRANDING".";



int APIENTRY WinMain(HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance,
		 			 LPSTR lpszCmdLine, 
					 int cmdShow)
{
    HANDLE runOnceMutex;

    g_hinst = hInstance;

    // should we allow FreeAmp to run?
    struct stat st;
    char path[MAX_PATH];

    getcwd(path, sizeof(path));
    strcat(path, "\\NeedToRebootForUpdate");
    if(!stat(path, &st))
    {
        MessageBox(NULL, kReboot, "You Need To Reboot...", 
                            MB_OK|MB_ICONINFORMATION|MB_SETFOREGROUND);
        return 0;
    }

	if(SendCommandLineToRealJukebox())
	{
		return 0;
	}

    runOnceMutex = CreateMutex(	NULL,
							    TRUE,
							    The_BRANDING" Should Only Run One Time!");

    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        SendCommandLineToHiddenWindow();
        
        CloseHandle(runOnceMutex);
        return 0;
    }

    // This causes a dynamic link error on some non NT systems, and
    // it didn't seem to help on multiprocessor NT systems, so I'm
    // commenting it.
    //if(IsWinNT() && IsMultiProcessor())
    //    SetProcessAffinityMask(GetCurrentProcess(), 0);

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


bool SendCommandLineToRealJukebox()
{
	bool result = false;

	Registrar registrar;
	Registry formatRegistry;
	Win32Prefs prefs;
	vector<DownloadFormatInfo*> formats;

	// init
    registrar.SetSubDir("plugins");
    registrar.SetSearchString("*.dlf");
    registrar.InitializeRegistry(&formatRegistry, &prefs);

    const RegistryItem* module = NULL;
    DownloadFormat* dlf = NULL;
    int32 i = 0;

    while((module = formatRegistry.GetItem(i++)))
    {
        dlf = (DownloadFormat*) module->InitFunction()(NULL);

        if(dlf)
        {
            DownloadFormatInfo dlfi;

            uint32 index = 0;

            // error != kError_NoMoreFormats
            while(IsntError(dlf->GetSupportedFormats(&dlfi, index++)))
            {
                dlfi.SetRef(dlf);
                formats.push_back(new DownloadFormatInfo(dlfi));
            }
        }
    }

	vector<DownloadFormatInfo*>::iterator dlfIter;

	if(__argc > 1)
    {
        char* extension;

		extension = strrchr(__argv[1], '.');

		if(extension)
        {
			extension++;

			for(dlfIter = formats.begin(); 
				dlfIter != formats.end(); 
				dlfIter++)
			{
				if(!strcasecmp(extension, (*dlfIter)->GetExtension()))
                {
					vector<DownloadItem*> items;
					char url[MAX_PATH + 7];
					uint32 size = sizeof(url);
					Error err;

					err = FilePathToURL(__argv[1], url, &size);

					if(IsntError(err))
					{
						(*dlfIter)->GetRef()->ReadDownloadFile(url, 
															   &items);

						vector<DownloadItem*>::iterator dliIter;

						for(dliIter = items.begin(); 
							dliIter != items.end(); 
							dliIter++)
						{
							MetaData metadata = (*dliIter)->GetMetaData();

							if(	strcasecmp("mp3", metadata.FormatExtension().c_str()) &&
								strcasecmp("mp2", metadata.FormatExtension().c_str()) &&
								strcasecmp("mp1", metadata.FormatExtension().c_str()) &&
								strcasecmp("m3u", metadata.FormatExtension().c_str()) &&
								strcasecmp("pls", metadata.FormatExtension().c_str()) )
							{
                                bool rjFound = false;

								result = true;

								LONG regErr;
								HKEY key;

                                regErr = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
													  "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\realjbox.exe",
													  0, 
													  KEY_ALL_ACCESS,
													  &key);

                                if(regErr == ERROR_SUCCESS)
                                {
                                    char buf[MAX_PATH*2];
                                    DWORD len = sizeof(buf);
                                    DWORD type;

                                    regErr = RegQueryValueEx(key,
                                                             NULL, 
                                                             NULL, 
                                                             &type, 
                                                             (LPBYTE)buf, 
                                                             &len);

                                    if(regErr == ERROR_SUCCESS)
                                    {
                                        rjFound = true;

                                        strcat(buf, " /m application/vnd.rn-rn_music_package ");
                                        strcat(buf, __argv[1]);

                                        WinExec(buf, SW_NORMAL);
                                    }
                                }

                                if(!rjFound)
                                {
                                    MessageBox(NULL, The_BRANDING" does not support the formats "
                                                     "contained in this Music Package.  The download "
                                                     "will be aborted.", "Unsupported Formats", MB_OK);

                                    DeleteFile(__argv[1]);
                                }
							}
						}
					}

                    break;
                }
			}

		}
    }

	// clean up
	for(dlfIter = formats.begin(); 
		dlfIter != formats.end(); 
		dlfIter++)
	{
		delete (*dlfIter)->GetRef();
		delete (*dlfIter);
	}

	return result;
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
    static bool replaceCurrentQueue = true;

    switch (msg)
    {
        case WM_CREATE:
        {
            context = (FAContext*)((LPCREATESTRUCT)lParam)->lpCreateParams;

            assert(context != NULL);
          
            result = SetWindowLong(hwnd, GWL_USERDATA, (LONG)context);
                        
            break;
        }

        case WM_TIMER:
        {
            KillTimer(hwnd, 1);
            replaceCurrentQueue = true;
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

            KillTimer(hwnd, 1);

            bool playNow = false;

            context->prefs->GetPlayImmediately(&playNow);
            
            // If a single theme or rpm file gets passed, don't affect 
            // the play queue
            if (strcasecmp("fat", array + strlen(array) - 3) == 0 ||
                strcasecmp("rmp", array + strlen(array) - 3) == 0)
                playNow = false;

            if(playNow)
            {
                if(replaceCurrentQueue)
                {
                    context->target->AcceptEvent(new Event(CMD_Stop));
                    context->plm->RemoveAll();

                    replaceCurrentQueue = false;
                }
            }

            for(int32 i = 0; i < count; i++)
            {
                strcpy(path, array + offset);
                offset += strlen(path) + 1;

                // is this a URL we know how to handle
                if( !strncasecmp(path, "http://", 7) ||
                    !strncasecmp(path, "rtp://", 6))
                {
                    context->plm->AddItem(path);

                    if(playNow)
                    {
                        SetTimer(hwnd, 1, 1000, NULL);
                    }

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
                    context->player->AddTheme(url);
                else
                    plm->AddItem(url);

                if(playNow)
                {
                    SetTimer(hwnd, 1, 1000, NULL);
                }
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

    wc.lpfnWndProc = HiddenWndProc;
    wc.hInstance = g_hinst;
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

const char* kFileTypes[][5] = {
    {".mp1", "MPEGAudioFile", "audio/x-mpeg", "MPEG Audio File", "13"},
    {".mp2", "MPEGAudioFile", "audio/x-mpeg", "MPEG Audio File", "13"},
    {".mp3", "MPEGAudioFile", "audio/x-mpeg", "MPEG Audio File", "13"},
    {".m3u", "M3UPlaylistFile", "audio/x-mpegurl", "M3U Playlist File", "16"},
    {".pls", "PLSPlaylistFile", "audio/x-scpls", "PLS Playlist File", "17"},
    {".rmp", "RealMusicPackage", "application/vnd.rn-rn_music_package", "Real Music Package", "4"},
    {".fat", "FreeAmpTheme", "application/x-freeamp-theme", "FreeAmp Theme File", "19"},
    {NULL, NULL}
};

const char* kMimeTypes[] = {
    "audio/x-mpeg",
    "audio/x-mp3",
    "audio/x-mpegurl",
    "audio/x-scpls",
    "audio/mpeg",
    "audio/mp3",
    "audio/mpegurl",
    "audio/scpls",
    "application/vnd.rn-rn_music_package",
    "application/x-freeamp-theme",
    NULL
};

const char* kOpenCommand = "\\shell\\open\\command";
const char* kNotifyStolen = "Music files normally associated with "the_BRANDING "\r\n"
                            "have been associated with another application.\r\n"
                            "Do you want to reclaim these music files?";

void ReclaimFileTypes(const char* path, bool askBeforeReclaiming)
{
    LONG    result; 
    DWORD   index;
    HKEY    typeKey;
    HKEY    appKey;
    DWORD   type;
    char    buf[MAX_PATH];
    char    openString[MAX_PATH + 8];
    DWORD   len = sizeof(buf);
    bool    permission = false;

    //"C:\Program Files\FreeAmp\freeamp.exe" "%1"

    wsprintf(openString, "\"%s\" \"%%1\"", path);

    if(!askBeforeReclaiming)
        permission = true;

    // reclaim the windows filetypes
    for(index = 0; ; index++)
    {
        if(kFileTypes[index][0] == NULL)
            break;

        result = RegOpenKeyEx(HKEY_CLASSES_ROOT,
							  kFileTypes[index][0],
							  0, 
                              KEY_ALL_ACCESS,
                              &typeKey);

        if(result == ERROR_SUCCESS)
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
                if(stricmp(buf, kFileTypes[index][1]))
                {
                    if(!permission)
                    {
                        int ret;
                        ret = MessageBox(NULL, 
                                   kNotifyStolen,
                                   "Reclaim File Types?", 
                                   MB_YESNO|MB_ICONQUESTION);

                        if(ret == IDYES)
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
                                  (LPBYTE)kFileTypes[index][1], 
                                  strlen(kFileTypes[index][1]) + 1);
                }
            }

            RegCloseKey(typeKey);
        }
        else // create it
        {
            RegCreateKey(HKEY_CLASSES_ROOT,
                         kFileTypes[index][0],
                         &typeKey);

            RegSetValueEx(typeKey,
                          NULL, 
                          NULL, 
                          REG_SZ, 
                          (LPBYTE)kFileTypes[index][1], 
                          strlen(kFileTypes[index][1]) + 1);

            RegSetValueEx(typeKey,
                          "Content Type", 
                          NULL, 
                          REG_SZ, 
                          (LPBYTE)kFileTypes[index][2], 
                          strlen(kFileTypes[index][2]) + 1);

            RegCloseKey(typeKey);
        }

        wsprintf(buf, "%s%s", kFileTypes[index][1], kOpenCommand);

        result = RegOpenKeyEx(	HKEY_CLASSES_ROOT,
							    buf,
							    0, 
							    KEY_ALL_ACCESS,
							    &appKey);

        if(result == ERROR_SUCCESS)
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
                if(stricmp(buf, openString))
                {
                    if(!permission)
                    {
                        int ret;
                        ret = MessageBox(NULL, 
                                   kNotifyStolen,
                                   "Reclaim File Types?", 
                                   MB_YESNO|MB_ICONQUESTION);

                        if(ret == IDYES)
                            permission = true;
                        else
                        {
                            RegCloseKey(appKey);
                            return;
                        }
                    }

                    RegSetValueEx(appKey,
                                  NULL, 
                                  NULL, 
                                  REG_SZ, 
                                  (LPBYTE)openString, 
                                  strlen(openString) + 1);
                }
            }

            RegCloseKey(appKey);
        }
        else // create it
        {
            HKEY iconKey;
            char iconPath[MAX_PATH];

            RegCreateKey(HKEY_CLASSES_ROOT,
                         kFileTypes[index][1],
                         &appKey);

            RegSetValueEx(appKey,
                          NULL, 
                          NULL, 
                          REG_SZ, 
                          (LPBYTE)kFileTypes[index][3], 
                          strlen(kFileTypes[index][3]) + 1);
            
            
            sprintf(iconPath, "%s,%s", path,  kFileTypes[index][4]);

            RegCreateKey(appKey,
                         "DefaultIcon",
                         &iconKey);

            RegSetValueEx(iconKey,
                          NULL, 
                          NULL, 
                          REG_SZ, 
                          (LPBYTE)iconPath, 
                          strlen(iconPath) + 1);

            
            RegCloseKey(iconKey);
            RegCloseKey(appKey);

            RegCreateKey(HKEY_CLASSES_ROOT,
                         buf,
                         &appKey);

            RegSetValueEx(appKey,
                          NULL, 
                          NULL, 
                          REG_SZ, 
                          (LPBYTE)openString, 
                          strlen(openString) + 1);

            
            RegCloseKey(appKey);
        }
    }


    // reclaim netscape filetypes

    result = RegOpenKeyEx(HKEY_CURRENT_USER,
                          "Software\\Netscape\\Netscape Navigator\\Viewers",
                          0, 
                          KEY_ALL_ACCESS,
                          &appKey);

    if(result == ERROR_SUCCESS)
    {
        for(index = 0; ; index++)
        {
            if(kMimeTypes[index] == NULL)
                break;
        
            len = sizeof(buf);
		    result = RegQueryValueEx(appKey,
                                     kMimeTypes[index], 
                                     NULL, 
                                     &type, 
                                     (LPBYTE)buf, 
                                     &len);

            if(result == ERROR_SUCCESS)
            {
                //MessageBox(NULL, buf, "value", MB_OK);
                if(stricmp(buf, path))
                {
                    if(!permission)
                    {
                        int ret;
                        ret = MessageBox(NULL, 
                                   kNotifyStolen,
                                   "Reclaim File Types?", 
                                   MB_YESNO|MB_ICONQUESTION);

                        if(ret == IDYES)
                            permission = true;
                        else
                        {
                            RegCloseKey(appKey);
                            return;
                        }
                    }

                    RegSetValueEx(appKey,
                                  kMimeTypes[index], 
                                  NULL, 
                                  REG_SZ, 
                                  (LPBYTE)path, 
                                  strlen(path) + 1);
                }
            }
        }

        RegCloseKey(appKey);
    }
}

bool IsWinNT(void)
{
   OSVERSIONINFO osv;
   osv.dwOSVersionInfoSize=sizeof(osv);
   GetVersionEx(&osv);
   return osv.dwPlatformId==VER_PLATFORM_WIN32_NT;
}

bool IsMultiProcessor(void)
{
   SYSTEM_INFO sInfo;
   
   GetSystemInfo(&sInfo);
   return sInfo.dwNumberOfProcessors > 1;
}

