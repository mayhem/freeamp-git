/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com 

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include <be/app/Application.h>

#include "config.h"
#include "player.h"
#include "event.h"
#include "ui.h"
#include "list.h"
#include "hashtable.h"
#include "semaphore.h"
#include "registrar.h"
#include "log.h"
#include "facontext.h"
#include "beosprefs.h"

int main(int argc, char **argv) {

	// *** BeOS specific section begin ***

	// Add the current directory to the ADDON_PATH environment var.
	char*	old_addon_path = getenv( "ADDON_PATH" );
	char*	new_addon_path = new char[ strlen( old_addon_path ) + 20 ];
	sprintf( new_addon_path, "ADDON_PATH=%s:.", old_addon_path );
	if ( putenv( new_addon_path ) != 0 )
	{
		cerr << "couldn't add . to the environment variable ADDON_PATH" << endl;
	}

	// BSoundPlayer needs BApplication.
	BApplication*	app = new BApplication( "application/x-vnd.freeamp-freeamp" );

	// *** BeOS specific section end ***

    FAContext *context = new FAContext;
    context->prefs = new BeOSPrefs();
    context->log = new LogFile("freeamp.log");
    Registrar *registrar= new Registrar();
    Registry *lmc;
    Registry *pmi;
    Registry *pmo;
    Registry* ui;
    
    lmc = new Registry();

//    registrar->SetSubDir("lmc");
    registrar->SetSubDir("");
    registrar->SetSearchString("*.lmc");
    registrar->InitializeRegistry(lmc,context->prefs);

//    registrar->SetSubDir("io");
    registrar->SetSubDir("");
    registrar->SetSearchString("*.pmi");
    pmi = new Registry;
    registrar->InitializeRegistry(pmi,context->prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new Registry;
    registrar->InitializeRegistry(pmo,context->prefs);


//    registrar->SetSubDir("ui");
    registrar->SetSubDir("");
    registrar->SetSearchString("*.ui");
    ui = new Registry;
    registrar->InitializeRegistry(ui,context->prefs);

    delete registrar;

    Semaphore *termSemaphore;
    termSemaphore = new Semaphore();
    Player *pP = Player::GetPlayer(context);

    pP->RegisterLMCs(lmc);
    pP->RegisterPMIs(pmi);
    pP->RegisterPMOs(pmo);
    pP->RegisterUIs(ui);

    if (pP->SetArgs(argc,argv)) {
		pP->SetTerminationSemaphore(termSemaphore);
		pP->Run();
	
		termSemaphore->Wait();
    }
    
    delete pP;
    delete context;
    delete app;
    return 0;
}


