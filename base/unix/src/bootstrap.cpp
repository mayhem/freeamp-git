/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 EMusic.com

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

#include <iostream.h>
#include <stdio.h>

#include "config.h"
#include "player.h"
#include "event.h"
#include "ui.h"
#include "list.h"
#include "hashtable.h"
#include "semaphore.h"
#include "registrar.h"
#include "facontext.h"
#include "log.h"
#include "unixprefs.h"

#if MP3_PROF
extern "C" {
  Initialize();
}
#endif

#if 0
bool CompareName(const char *p1, const char *p2) {
    //cout << "Comparing " << p1 << " to " << p2 << endl;
    int32 i=0; int32 j=0;
    if(!p1 ||!p2) return false;
    while ((p1[i] == p2[j]) && p1[i] && p2[j]) {
	i++; j++;
    }
    if (!p1[i] && (p2[j]=='-')) {
	return true;
    } else {
	return false;
    }
}
#endif

int main(int argc, char **argv) {
    //exit(1);

    FAContext *context = new FAContext;
    UnixPrefs *unixPrefs = new UnixPrefs;

    int errLine = unixPrefs->GetErrorLineNumber();
    if (errLine)
    	cerr << "ERROR parsing line " << errLine << " of ~/.freeamp/preferences\n";

    context->prefs = unixPrefs;
    context->log = new LogFile("freeamp.log");

    Registrar *registrar= new Registrar();
    Registry *lmc;
    Registry *pmi;
    Registry *pmo;
    Registry* ui;
    
    lmc = new Registry();
#if MP3_PROF
    {
	RegistryItem* item = new RegistryItem;
	item->SetPath("[builtin]");
	item->SetName("xingmp3-linux.lmc");
	item->SetDescription("xingmp3-linux.lmc");
	item->SetInitFunction((InitializeFunction)Initialize);
	lmc->Add(item);
    }
#else
//    registrar->SetSubDir("lmc");
    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.lmc");
    registrar->InitializeRegistry(lmc,context->prefs);
#endif


//    registrar->SetSubDir("io");
    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.pmi");
    pmi = new Registry;
    registrar->InitializeRegistry(pmi,context->prefs);

    registrar->SetSearchString("*.pmo");
    pmo = new Registry;
    registrar->InitializeRegistry(pmo,context->prefs);


//    registrar->SetSubDir("ui");
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




    if (pP->SetArgs(argc,argv)) {
	pP->SetTerminationSemaphore(termSemaphore);
	pP->Run();
	
	termSemaphore->Wait();
    }
    delete pP;
    delete context;
    return 0;
}
