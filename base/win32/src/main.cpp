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
#include "dummycoo.h"
#include "preferences.h"
 
int APIENTRY WinMain(	HINSTANCE hInstance, 
						HINSTANCE hPrevInstance,
		 				LPSTR lpszCmdLine, 
						int cmdShow)
{
    // Initialize the preferences
   Preferences* prefs;

   prefs = new Preferences;

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

    // need a way to signal main thread to quit...
    Semaphore *termination = new Semaphore();
    DummyCOO *dummy = new DummyCOO(termination);

	// create the player
	Player *player = Player::GetPlayer();

    // register items... we give up ownership here
    player->RegisterActiveUI(dummy);
    player->RegisterLMCs(lmc);
    player->RegisterPMIs(pmi);
    player->RegisterPMOs(pmo);
    player->RegisterUIs(ui);

    //CIO* defaultCIO;
    //COO* defaultCOO;

    //CreateDefaultUI(UIRegistry, &defaultCIO, &defaultCOO);

    //player->RegisterCOO(defaultCOO);
    //player->RegisterCIO(defaultCIO);

    PlayList* playlist = new PlayList();

	playlist->Add("..\\..\\..\\test\\jules.mp3",0);
	
    playlist->SetFirst();
    Player::GetPlayer()->AcceptEvent(new Event(CMD_SetPlaylist,playlist));
    Player::GetPlayer()->AcceptEvent(new Event(CMD_Play));

    //Event *e = new Event(CMD_QuitPlayer);
	//Player::GetPlayer()->AcceptEvent(e);

    // sit around and twiddle our thumbs
    termination->Wait();

    // clean up our act
    delete player;

    delete termination;

	return 0;
}

