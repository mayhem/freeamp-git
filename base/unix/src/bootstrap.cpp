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

#include "config.h"

#include <iostream.h>
#include <stdio.h>
#include <unistd.h>

#ifdef DEBUG_MUTEXES
#include <signal.h>
#endif

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

#ifdef DEBUG_MUTEXES
static void PrintMutexDebugInfo(int sig)
{
    Mutex newmutex;
    newmutex.DumpAllMutexes();
}
#endif

int main(int argc, char **argv) 
{
    FAContext *context = new FAContext;
    UnixPrefs *unixPrefs = new UnixPrefs;

    int errLine;
    if ((errLine = unixPrefs->GetErrorLineNumber()))
    	cerr << "ERROR parsing line " << errLine 
             << " of ~/.freeamp/preferences\n";

#ifdef DEBUG_MUTEXES
    struct sigaction sigact;
    sigact.sa_handler = PrintMutexDebugInfo;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1, &sigact, 0);

    cout << The_BRANDING << " has been compiled with mutex debugging enabled.\n"
         << "To dump mutex info: kill -SIGUSR1 " << getpid() << endl;
#endif
    
    context->prefs = unixPrefs;
    context->log = new LogFile("freeamp.log");

    Registrar *registrar= new Registrar();
    Registry *lmc;
    Registry *pmi;
    Registry *pmo;
    Registry* ui;
    
    lmc = new Registry();
#if MP3_PROF
    RegistryItem* item = new RegistryItem;
    item->SetPath("[builtin]");
    item->SetName("xingmp3.lmc");
    item->SetDescription("xingmp3.lmc");
    item->SetInitFunction((InitializeFunction)Initialize);
    lmc->Add(item);
#else
    registrar->SetSubDir("plugins");
    registrar->SetSearchString("*.lmc");
    registrar->InitializeRegistry(lmc,context->prefs);
#endif

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

    if (pP->SetArgs(argc,argv)) {
	pP->SetTerminationSemaphore(termSemaphore);
	pP->Run();
	
	termSemaphore->Wait();
    }

    delete pP;
    delete context;
    return 0;
}
