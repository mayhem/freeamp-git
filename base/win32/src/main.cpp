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
#include "player.h"
#include "event.h"
#include "registrar.h"
#include "dummyui.h"
#include "preferences.h"
 
int APIENTRY WinMain(	HINSTANCE hInstance, 
						HINSTANCE hPrevInstance,
		 				LPSTR lpszCmdLine, 
						int cmdShow)
{
    // Initialize the preferences in case the user moved the app
   Preferences* prefs;

   prefs = new Preferences;
   prefs->Initialize();

    // find all the plug-ins we use
    Registrar* registrar;
    LMCRegistry* lmc;
    PMIRegistry* pmi;
    PMORegistry* pmo;
    UIRegistry*  ui;

    registrar = new Registrar;

    registrar->SetSubDir("lmc");
    registrar->SetSearchString("*.lmc");
    lmc = new LMCRegistry;
    registrar->InitializeRegistry(lmc, prefs);

    registrar->SetSubDir("io");
    registrar->SetSearchString("*.pmi");
    pmi = new PMIRegistry;
    registrar->InitializeRegistry(pmi, prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new PMORegistry;
    registrar->InitializeRegistry(pmo, prefs);


    registrar->SetSubDir("ui");
    registrar->SetSearchString("*.ui");
    ui = new UIRegistry;
    registrar->InitializeRegistry(ui, prefs);

    delete registrar;
    delete prefs;

    // create the player
	Player *player = Player::GetPlayer();

    // need a way to signal main thread to quit...
    Semaphore *termination = new Semaphore();

    EventQueueRef eq = new EventQueue;
    eq->ref = player;
    eq->AcceptEvent = Player::AcceptEventStub;

    DummyUI *dummy = new DummyUI(termination);
    dummy->SetTarget(eq);

    UIRef dummyRef = new UI;
    dummyRef->ref = dummy;
    dummyRef->AcceptEvent = dummy->AcceptEventStub;
    dummyRef->Cleanup = dummy->Cleanup;

    // register items... we give up ownership here
    player->RegisterActiveUI(dummyRef);
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

    delete termination;

	return 0;
}

