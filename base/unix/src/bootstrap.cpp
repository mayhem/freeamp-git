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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> 
#include <sys/shm.h> 

#include <sys/types.h>
#include <signal.h>

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

const int iSemKey = 0xFA000000;
const int iMemKey = 0xFA000001;
const int iSharedMemSize = 4096;

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

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
#else
union semun 
{
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};
#endif

#if defined(solaris)

#include <poll.h>

extern "C" {
int usleep(unsigned int usec)
{
    return poll(NULL, 0, usec/1000+1);
}
}
#endif

int main(int argc, char **argv) 
{
    FAContext *context = new FAContext;
    UnixPrefs *unixPrefs = new UnixPrefs;
    key_t      tSemKey = iSemKey;
    key_t      tMemKey = iMemKey;
    int        iCmdSem = -1, iCmdMem = -1;
    int        iProcess, i;
    char      *pCmdLine = NULL, *pPtr;

    union semun unsem;
    unsem.val = 0;
    
    context->prefs = unixPrefs;
    context->log = new LogFile("freeamp.log");

    int errLine;
    if ((errLine = unixPrefs->GetErrorLineNumber()))
        cerr << "ERROR parsing line " << errLine
             << " of ~/.freeamp/preferences\n";

    bool allow_mult = false;
    context->prefs->GetPrefBoolean(kAllowMultipleInstancesPref, &allow_mult);

#if defined(solaris)
    allow_mult = true;
#endif

    if (!allow_mult) {
        iCmdSem = semget(tSemKey, 1, IPC_CREAT | 0666);
        if (iCmdSem < 0)
        {
           printf("Cannot create/open a semaphore. Is SYS V IPC installed?\n");
           exit(0);
        }

        // Check to see if the process that created that semaphore still
        // exists
        iProcess = semctl(iCmdSem, 0, GETVAL, unsem);
        if (iProcess > 0 && !allow_mult)
        {
            if (kill(iProcess, 0) >= 0)
            {
                iCmdMem = shmget(tMemKey, iSharedMemSize, 0666);
                pCmdLine = (char *)shmat(iCmdMem, NULL, 0); 
                for(i = 1, pPtr = pCmdLine; i < argc; i++)
                {
                    char *path = new char[_MAX_PATH];
                    strcpy(path, argv[i]);
                    if (strncasecmp(argv[i], "http://", 7) &&
                        strncasecmp(argv[i], "rtp://", 6))
                        ResolvePath(&path);

                    strcpy(pPtr, path);
                    pPtr += strlen(pPtr) + 1;

                    delete [] path;
                }
                *pPtr = 0;

                // Now wait for the main freeamp to parse the args and then exit
                while(*pCmdLine != 0)
                {
                    if (kill(iProcess, 0) < 0)
                        break;

                    sleep(1);
                }

                shmdt(pCmdLine);

                exit(0);
            }
        }

        // Set the current pid into the semaphore
        unsem.val = getpid();
        semctl(iCmdSem, 0, SETVAL, unsem);

        // Create the shared memory segment
        iCmdMem = shmget(tMemKey, iSharedMemSize, IPC_CREAT | 0666);
        if (iCmdMem != -1)
        {
            pCmdLine = (char *)shmat(iCmdMem, NULL, 0); 
            pCmdLine[0] = 0;
        }
    }

#ifdef DEBUG_MUTEXES
    struct sigaction sigact;
    sigact.sa_handler = PrintMutexDebugInfo;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGUSR1, &sigact, 0);

    cout << The_BRANDING << " has been compiled with mutex debugging enabled.\n"
         << "To dump mutex info: kill -SIGUSR1 " << getpid() << endl;
#endif
    
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

    if (pP->SetArgs(argc,argv)) 
    {
        pP->SetTerminationSemaphore(termSemaphore);
        pP->Run();

        for(;;)
        {
            if (!termSemaphore->TimedWait(1000))
            {
                if (pCmdLine && strlen(pCmdLine) > 0 && !allow_mult)
                {
                    int iItems = context->plm->CountItems();
                    bool bPlay;

                    context->prefs->GetPrefBoolean(kPlayImmediatelyPref, &bPlay);
                    for(i = 0, pPtr = pCmdLine; *pPtr; i++)
                    {
                        if (i == 0)
                        {
                            if (!strcasecmp("fat", pPtr + strlen(pPtr) - 3) || 
                                !strcasecmp("rmp", pPtr + strlen(pPtr) - 3)) 
                               bPlay = false;

                            if (bPlay) 
                            {
                                context->target->AcceptEvent(new Event(CMD_Stop));
                                context->plm->RemoveAll();
                            }    
                        }

                        pP->HandleSingleArg(pPtr);
                        pPtr += strlen(pPtr) + 1;
                    }
                    pCmdLine[0] = 0;

                    if (iItems == 0 || bPlay)
                        context->target->AcceptEvent(new Event(CMD_Play));
                }
            }
            else
                break;
        }
    }

    if (!allow_mult) {
        if (pCmdLine)
            shmdt(pCmdLine);
        unsem.val = 0; 
        semctl (iCmdSem, 0, IPC_RMID, unsem);
        shmctl (iCmdMem, IPC_RMID, 0);
    }

    delete pP;
    delete context;

    return 0;
}
