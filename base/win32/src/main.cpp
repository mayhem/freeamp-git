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
#include <winsock.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>

/* Project Includes */
#include "player.h"
#include "event.h"
#include "registrar.h"
#include "log.h"
#include "facontext.h"
#include "win32prefs.h"

static
BOOL
CALLBACK
EnumThreadWndProc(  HWND hwnd,
                    LPARAM lParam )
{
    BOOL    result = TRUE;
    char    windowTitle[256];
    char    freeampTitle[] = "FreeAmp";
    int32   count;

    count = GetWindowText(hwnd, windowTitle, sizeof(windowTitle));

    // If we found our main window, stop the enumeration
    if (!strncmp(windowTitle, freeampTitle, strlen(freeampTitle)))
    {
        HWND* phwnd = (HWND*)lParam;
        *phwnd = hwnd;
        result =  FALSE;
    }

    return result;
}

int APIENTRY WinMain(	HINSTANCE hInstance, 
						HINSTANCE hPrevInstance,
		 				LPSTR lpszCmdLine, 
						int cmdShow)
{
    HANDLE runOnceMutex;

    runOnceMutex = CreateMutex(	NULL,
							    TRUE,
							    "FreeAmp Should Only Run One Time!");

    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        HWND hwnd = NULL;
        // find currently running FreeAmp player
        EnumWindows(EnumThreadWndProc, (LPARAM)&hwnd);

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

            SetForegroundWindow(hwnd);
        }
        
        CloseHandle(runOnceMutex);
        return 0;
    }

    WSADATA sGawdIHateMicrosoft;
    WSAStartup(0x0002,  &sGawdIHateMicrosoft);

    FAContext *context = new FAContext;
    Win32Prefs* prefs = new Win32Prefs();
    //prefs->Initialize();

    context->prefs = prefs;
    context->log = new LogFile("freeamp.log");

    

    // find all the plug-ins we use
    Registrar* registrar;
    LMCRegistry* lmc;
    PMIRegistry* pmi;
    PMORegistry* pmo;
    UIRegistry*  ui;

    registrar = new Registrar;

    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.lmc");
    lmc = new LMCRegistry;
    registrar->InitializeRegistry(lmc, context->prefs);

    registrar->SetSearchString("*.pmi");
    pmi = new PMIRegistry;
    registrar->InitializeRegistry(pmi, context->prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new PMORegistry;
    registrar->InitializeRegistry(pmo, context->prefs);

    registrar->SetSearchString("*.ui");
    ui = new UIRegistry;
    registrar->InitializeRegistry(ui, context->prefs);

    delete registrar;

    // create the player
	Player *player = Player::GetPlayer(context);

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

    CloseHandle(runOnceMutex);

	WSACleanup();

	return 0;
}