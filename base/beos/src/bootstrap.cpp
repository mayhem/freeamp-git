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

#include <be/app/Roster.h>
#include <be/storage/Entry.h>
#include <be/storage/Path.h>
#define DEBUG 1
#include <be/support/Debug.h>

#include "config.h"
#include "freeampapp.h"
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
#include "thread.h"

void
termination_thread( void* arg )
{
    PRINT(( "waiting for termination semaphore\n" ));
    Semaphore* termSem = (Semaphore*)arg;
    termSem->Wait();

    be_app->PostMessage( B_QUIT_REQUESTED );
    PRINT(( "Termination thread exiting\n" ));
}

int main(int argc, char **argv)
{
    FAContext *context = new FAContext;
    context->prefs = new BeOSPrefs();
    context->log = new LogFile("freeamp.log");

    FreeAmpApp app( "application/x-vnd.freeamp.freeamp", context );

    app_info appInfo;
    be_app->GetAppInfo( &appInfo );
    BEntry entry( &appInfo.ref );
    BPath path;
    entry.GetPath( &path );
    path.GetParent( &path );

    const char* kUserAddOnPath = "/boot/home/config/add-ons/freeamp";
    char* new_freeamp_path = new char[ strlen( FREEAMP_PATH_ENV ) +
                                       strlen( kUserAddOnPath ) +
                                       strlen( path.Path() ) + 40 ];
    sprintf( new_freeamp_path, "%s=%s:%s", FREEAMP_PATH_ENV, path.Path(),
                                           kUserAddOnPath );
    if ( putenv( new_freeamp_path ) != 0 )
    {
        cerr << "couldn't add app path to the environment variable "
                "FREEAMP_PATH_ENV" << endl;
    }
    delete[] new_freeamp_path;

    context->prefs->SetPrefString(kInstallDirPref,  path.Path() );

#if 0
    // Add the current directory to the ADDON_PATH environment var.
    char* old_addon_path = getenv( "ADDON_PATH" );
    char* new_addon_path = new char[ strlen( old_addon_path ) +
                                     strlen( path.Path() ) + 20 ];
    sprintf( new_addon_path, "ADDON_PATH=%s:%s:.", old_addon_path, path.Path() );
    cout << "addon path = " << new_addon_path << endl;
    if ( putenv( new_addon_path ) != 0 )
    {
        cerr << "couldn't add . to the environment variable ADDON_PATH" << endl;
    }
    delete[] new_addon_path;
#endif

    Registrar *registrar= new Registrar();
    Registry *lmc;
    Registry *pmi;
    Registry *pmo;
    Registry* ui;
    
    lmc = new Registry();

    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.lmc");
    registrar->InitializeRegistry(lmc,context->prefs);

    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.pmi");
    pmi = new Registry;
    registrar->InitializeRegistry(pmi,context->prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new Registry;
    registrar->InitializeRegistry(pmo,context->prefs);

    registrar->SetSubDir("plugins");
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

    if (pP->SetArgs(argc,argv))
    {
        pP->SetTerminationSemaphore(termSemaphore);
        pP->Run();

        Thread* termThread = Thread::CreateThread();
        termThread->Create( termination_thread, termSemaphore );

        app.Run();
        delete termThread;
    }
    
    delete pP;
    delete context;
    return 0;
}

// vi: set ts=4:
