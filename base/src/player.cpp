/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, Write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	$Id$
____________________________________________________________________________*/

#include <iostream.h>

#include "event.h"
#include "player.h"
#include "thread.h"
#include "debug.h"
#include "vector.h"
#include "ui.h"
#include "queue.h"
#include "semaphore.h"
#include "eventdata.h"
#include "registrar.h"
#include "preferences.h"
#include "lmc.h"


Player *Player::m_thePlayer = NULL;

Player *Player::GetPlayer() {
    if (m_thePlayer == NULL) {
        m_thePlayer = new Player();
    }
    return m_thePlayer;
}

Player::
Player():
EventQueue() {
    //cout << "Creating player..." << endl;
    m_eventSem = new Semaphore();
    m_eventQueue = new Queue<Event *>();
    //cout << "Created queue" << endl;
    m_eventServiceThread = NULL;
    //cout << "Started event thread" << endl;
    m_uiVector = new Vector<UserInterface *>();
    //cout << "Created vectors" << endl;
    m_uiManipLock = new Mutex();
    m_lmcMutex = new Mutex();
    m_pmiMutex = new Mutex();
    m_pmoMutex = new Mutex();
    m_uiMutex = new Mutex();
    //cout << "Created mutex" << endl;
    m_imQuitting = 0;
    m_quitWaitingFor = 0;
    m_plm = new PlayListManager((EventQueue *)this);
    m_playerState = STATE_Stopped;

    m_lmcRegistry = NULL;
    m_pmiRegistry = NULL;
    m_pmoRegistry = NULL;
    m_uiRegistry = NULL;

    m_lmc = NULL;
    m_ui = NULL;

    m_argUI = NULL;

    m_argc  = 0;
    m_argv  = NULL;
    m_prefs = NULL;
    m_pTermSem = NULL;

    m_didUsage = false;
    //m_autoplay = false;
}

#define TYPICAL_DELETE(x) /*printf("deleting...\n");*/ if (x) { delete x; x = NULL; }

Player::~Player() {

    TYPICAL_DELETE(m_pTermSem);
    
    //cout << "waiting for service thread to end..." << endl;
    if (m_eventServiceThread) {
	m_eventServiceThread->Join();
	delete m_eventServiceThread;
	m_eventServiceThread = NULL;
    }

    //cout << "serivce thread done.." << endl;
    if (m_lmc) {
        m_lmc->Stop();
        delete m_lmc;
        m_lmc = NULL;
    }

    //cout << "done deleting myLMC" << endl;
	TYPICAL_DELETE(m_eventSem);

    //cout << "Player: deleting event queue" << endl;
	TYPICAL_DELETE(m_eventQueue);


    // Delete CIOs
    //cout << "Player: deleting CIOs" << endl;
    if (m_uiVector) {
        m_uiVector->DeleteAll();
        delete m_uiVector;
        m_uiVector = NULL;
    }

    //cout << "Player: deleting PlayList" << endl;
	TYPICAL_DELETE(m_plm);

    //cout << "Player: deleting CIO/COO manipulation lock" << endl;
	TYPICAL_DELETE(m_uiManipLock);

	TYPICAL_DELETE(m_lmcMutex);

	TYPICAL_DELETE(m_pmiMutex);

	TYPICAL_DELETE(m_pmoMutex);

	TYPICAL_DELETE(m_uiMutex);

	TYPICAL_DELETE(m_lmcRegistry);

	TYPICAL_DELETE(m_pmiRegistry);

	TYPICAL_DELETE(m_pmoRegistry);

	TYPICAL_DELETE(m_uiRegistry);

	TYPICAL_DELETE(m_prefs);

	TYPICAL_DELETE(m_argUI);
}

void Player::SetTerminationSemaphore(Semaphore *pSem) {
    m_pTermSem = pSem;
}

/* return true if parsing was successful, false otherwise. */

typedef char *pchar;

bool Player::SetArgs(int32 argc, char** argv){
    Vector<char *> argVector;

    char *arg = NULL;
#ifndef WIN32
    // grab the UI name from how we are invoked.
    m_argUI = new char[strlen(argv[0]) + 1 + 3];
    char *pBegin = strrchr(argv[0],'/');
    if (pBegin) {
	pBegin++;
    } else {
	pBegin = argv[0];
    }
    sprintf(m_argUI,"%s",pBegin);
#endif
    argVector.Insert(argv[0]);
    for(int32 i = 1;i < argc; i++) 
    {
	arg = argv[i];
	
	if (arg[0] == '-') 
	{
	    switch (arg[1]) 
	    {
		case '-':
		    if (!strcmp(&(arg[2]),"help")) {
			Usage(argv[0]);
			argVector.Insert(argv[i]);
		    }
		    break;
		case 'u':
                case 'U':
                {
                    if( arg[2] == 'i' ||
                        arg[2] == 'I')
                    {
                        i++;
			if (i >= argc) {
			    Usage(argv[0]);
			    AcceptEvent(new Event(CMD_QuitPlayer));
			    return false;
			}
			arg = argv[i];
			if (m_argUI) delete m_argUI;
                        m_argUI = new char[strlen(arg) + 1];
                        strcpy(m_argUI, arg);
                    }
		    break;
		}
		case 'h':
		case 'H':
		    Usage(argv[0]);
		    argVector.Insert(argv[i]);
		    break;
		default:
		    argVector.Insert(argv[i]);
            }
        } else {
	    argVector.Insert(argv[i]);
	}
    }
    m_argc = argVector.NumElements();
    if (m_argc) {
	m_argv = new pchar[m_argc];
	for(int f = 0;f < m_argc;f++) {
	    m_argv[f] = argVector.ElementAt(f);
	    //cerr << "Adding argument (" << f << "): " << m_argv[f] << endl;
	}
    }
    return true;
}

void Player::Usage(const char *progname) {
    if (m_didUsage) return;
    cout << "freeamp global usage:" << endl;
    cout << "   " << progname << " [-ui <UI plugin name>]..." << endl;
    cout << endl;
    m_didUsage = true;
}

#ifdef __linux__
#define ARCH_NAME "linux"
#else
#define ARCH_NAME "unknown"
#endif

int32 Player::CompareNames(const char *p1, const char *p2) {
// windows plugins and unix plugins are named differently...
#ifdef WIN32
    return strcmp(p1,p2);
#else
    //ut << "Comparing: " << p1 << " to " << p2 << endl;
    if (strcmp(p1,p2)) {
	// no direct match, try w/ .ui appended...
	char foo[512];
	sprintf(foo,"%s.ui",p2);
	//ut << "Comparing: " << p1 << " to " << foo << endl;
	if (strcmp(p1,foo)) {
	    // no plugin.ui match, try  plugin-arch.ui
	    char foo[512];
	    sprintf(foo,"%s-%s.ui",p2,ARCH_NAME);
	    //cout << "Comparing: " << p1 << " to " << foo << endl;
	    if (strcmp(p1,foo)) {
		// no match
		return 1;
	    } else {
		return 0;
	    }
	} else {
	    return 0;
	}
    } else {
	return 0;
    }
#endif
}

void Player::SetPreferences(Preferences * pP) {
    m_prefs = pP;
}

void Player::Run(){
    Preferences* prefs;
    char* name = NULL;
    uint32 len = 256;
    Error error = kError_NoErr;

    m_eventServiceThread = Thread::CreateThread();
    m_eventServiceThread->Create(Player::EventServiceThreadFunc,this);

    // which ui should we instantiate first??
    if(m_argUI == NULL)
    {
        name = new char[len];
        prefs = new Preferences;

        while((error = prefs->GetDefaultUI(name, &len)) == 
                kError_BufferTooSmall)
        {
            delete [] name;
            len++;

            name = new char[len];
        }

        delete prefs;
    }
    else
    {
        name = new char[strlen(m_argUI) + 1]; 
        strcpy(name, m_argUI);
    }

    if(IsntError(error))
    {
        RegistryItem* item = NULL;
	UserInterface *ui = NULL;
        int32 i = 0;
	    int32 uisActivated = 0;
	
        while(item = m_uiRegistry->GetItem(i++))
        {
            if(!CompareNames(item->Name(),name))
            {
		        m_ui = (UserInterface *)item->InitFunction()();
		
		        m_ui->SetTarget((EventQueue *)this);
 		        m_ui->SetPlayListManager(m_plm);
			m_ui->SetArgs(m_argc, m_argv);
		        Error er = m_ui->Init();
			if (er == kError_NoErr) {
			    RegisterActiveUI(m_ui);
			} else {
			    delete m_ui;
			    m_ui = NULL;
			}
			uisActivated++;
                break;
            }
        }
	
	if (!uisActivated) {
#ifdef WIN32
	    char foo[1024];
	    char bar[MAX_PATH];
		uint32 size = MAX_PATH - 1;
		m_prefs->GetInstallDirectory(bar,&size);
	    sprintf(foo,"No UI plugin matched 'plugins\\%s' or 'plugins\\%s.ui' in '%s'.  FreeAmp will quit.",name,name,bar	);
	    MessageBox(NULL,foo,"FreeAmp Error",MB_OK);
#else
	    const char *thePath = getenv(FREEAMP_PATH_ENV);
	    if (thePath == NULL) thePath = Preferences::GetLibDirs();
	    cerr << "No UI plugin in '" << thePath << "' matched 'plugins/" << name << "' or 'plugins/" << name << ".ui'" << endl;
	    cerr << "FreeAmp will quit." << endl;
#endif
	    Event *e = new Event(CMD_QuitPlayer);
	    AcceptEvent(e);
	    e = new Event(INFO_ReadyToDieUI);
	    AcceptEvent(e);
	}
    }

    delete [] name;
}

//int32 Player::AcceptEventStub(EventQueueRef ref, Event* e){
//    Player *pP = (Player*)ref->ref;
//
//    return pP->AcceptEvent(e);
//}

void Player::EventServiceThreadFunc(void *pPlayer) {
    //cout << "Beginning event service" << endl;
    Player *pP = (Player *)pPlayer;
    Event *pC;
    int32 rtnVal = 0x0;
    while (rtnVal == 0) {   // serviceEvent will return 1 if error or time to quit.
        //cout << "About to Read queue..." << endl;
        pP->m_eventSem->Wait();
        pC = pP->m_eventQueue->Read();
        //cout << "Read queue..." << endl;
        if (pC) {
	    rtnVal = pP->ServiceEvent(pC);
	    //delete pC; //only on _some_ events now...
        }
        //if (pP->m_eventQueue->IsEmpty()) usleep(50000);
    }
//    cout << "EventServiceThreadFunc: quitting on " << rtnVal << endl;
    // Time to quit!!!
}
int32 Player::RegisterActiveUI(UserInterface *ui) {
    GetUIManipLock();
    if (m_uiVector && ui) {
        m_uiVector->Insert(ui);
        ReleaseUIManipLock();
        return 0;
    } else {
        ReleaseUIManipLock();
        return 255;
    }
}

int32 Player::RegisterLMCs(LMCRegistry* registry){
    int32 result = 0;
    m_lmcMutex->Acquire();

    if (m_lmcRegistry) {
        Registrar::CleanupRegistry(m_lmcRegistry);
        delete m_lmcRegistry;
    }

    m_lmcRegistry = registry;

    m_lmcMutex->Release();
    
    return result;
}
int32 Player::RegisterPMIs(PMIRegistry* registry){
    int32 result = 0;
    m_pmiMutex->Acquire();

    if (m_pmiRegistry) {
        Registrar::CleanupRegistry(m_pmiRegistry);
        delete m_pmiRegistry;
    }

    m_pmiRegistry = registry;

    m_pmiMutex->Release();
     
    return result;
}

int32 Player::RegisterPMOs(PMORegistry* registry){
    int32 result = 0;  
    m_pmoMutex->Acquire();

    if (m_pmoRegistry) {
        Registrar::CleanupRegistry(m_pmoRegistry);
        delete m_pmoRegistry;
    } 

    m_pmoRegistry = registry;

    m_pmoMutex->Release();

    return result;
}

int32 Player::RegisterUIs(UIRegistry* registry){
    int32 result = 0;
    m_uiMutex->Acquire();

    if (m_uiRegistry) {
        Registrar::CleanupRegistry(m_uiRegistry);
        delete m_uiRegistry;
    } 

    m_uiRegistry = registry;

    m_uiMutex->Release();

    return result;
}

void Player::GetUIManipLock() {
    m_uiManipLock->Acquire(WAIT_FOREVER);
}

void Player::ReleaseUIManipLock() {
    m_uiManipLock->Release();
}

int32 Player::AcceptEvent(Event *e) {
    m_eventQueue->Write(e);
    m_eventSem->Signal();
    return 0;
}

bool Player::SetState(PlayerState ps) {
    //cout << "Changing from " << m_playerState << " to " << ps << endl;
    if (ps == m_playerState) 
        return false;
    m_playerState = ps;
    return true;
}


#define SEND_NORMAL_EVENT(e)  { Event *ev = new Event(e); GetUIManipLock(); SendToUI(ev); ReleaseUIManipLock(); delete ev;}

#ifndef WIN32
#define DISPLAY_ERROR(s,e) { if (e & 0xFFFF0000) { cout << s->GetErrorString(e) << endl;} else { cout << "Error Number " << e << endl; } }
#else
#define DISPLAY_ERROR(s,e) { if (e & 0xFFFF0000) { MessageBox(NULL,s->GetErrorString(e),NULL,MB_OK);} else { char foo[1024];sprintf(foo,"ErrorNumber %d",e); MessageBox(NULL,foo,NULL,MB_OK); } }
#endif
int32 Player::ServiceEvent(Event *pC) {
    if (pC) {
	//cout << "Player: serviceEvent: servicing Event: " << pC->Type() << endl;
	switch (pC->Type()) {
	    case INFO_DoneOutputting: {  // LMC or PMO sends this when its done outputting whatever.  Now, go on to next piece in playlist
                if (SetState(STATE_Stopped)) {
		    SEND_NORMAL_EVENT(INFO_Stopped);
                }
                m_plm->SetNext();
                Event *e = new Event(CMD_Play);
                AcceptEvent(e);
		delete pC;
                return 0;
                break; 
            }
	    
	    case CMD_Stop: {
                if (m_lmc) {
		    m_lmc->Stop();
		    delete m_lmc;
		    m_lmc = NULL;
                }
                if (SetState(STATE_Stopped)) {
		    SEND_NORMAL_EVENT(INFO_Stopped);
                }
		delete pC;
                return 0;
		break;
	    }
	    
	    case CMD_ChangePosition: {
		if(m_lmc)
		    m_lmc->ChangePosition(((ChangePositionEvent *)pC)->GetPosition());        
		delete pC;
		return 0;
		break;
	    }

	    case CMD_PLMGetMediaInfo: {
		PLMGetMediaInfoEvent *gmi = (PLMGetMediaInfoEvent *)pC;
		PLMSetMediaInfoEvent *smi = new PLMSetMediaInfoEvent();
		smi->SetPlayListItem(gmi->GetPlayListItem());
		RegistryItem *pmi_item = m_pmiRegistry->GetItem(0);
		RegistryItem *lmc_item = NULL;
		Error error = kError_UnknownErr;
		if (pmi_item) {
		    PhysicalMediaInput *pmi = (PhysicalMediaInput *)pmi_item->InitFunction()();
		    if (pmi) {
			error = pmi->SetTo(gmi->GetPlayListItem()->m_url);
			//cout << "trying: " << gmi->GetPlayListItem()->m_url << endl;
			if (IsntError(error)) {
			    lmc_item = m_lmcRegistry->GetItem(0);
			    if (lmc_item) {
				LogicalMediaConverter *lmc = (LogicalMediaConverter *)lmc_item->InitFunction()();
				if (lmc) {
				    error = lmc->SetPMI(pmi);
				    if (IsntError(error)) {
					if (lmc->CanDecode()) {
					    //cout << "yep" << endl;
					    MediaInfoEvent *mie = NULL;
					    error = lmc->ExtractMediaInfo(&mie);
					    if (IsntError(error)) {
						smi->SetPMIRegistryItem(pmi_item);
						smi->SetLMCRegistryItem(lmc_item);
						smi->SetMediaInfo(mie);
						smi->SetComplete();
						delete lmc;
					    } else {
						delete lmc;
					    }
					} else {
					    //cout << "nope" << endl;
					    delete lmc;
					}
				    } else {
					delete lmc;
				    }
				}
			    }
			} else {
			    delete pmi;
			}
		    }
		}
		m_plm->AcceptEvent(smi);
		delete pC;
		return 0;
		break;
	    }
	    case CMD_PlayPaused:
	    case CMD_Play: {
		PlayListItem *pc = m_plm->GetCurrent();
		Error error = kError_NoErr;
		
		if (pc) {
		    if (m_lmc) {
			m_lmc->Stop();
			delete m_lmc;
			m_lmc = NULL;
		    }
		    
		    PhysicalMediaOutput *pmo = NULL;
		    PhysicalMediaInput *pmi = NULL;
		    RegistryItem *pmi_item = pc->GetPMIRegistryItem();
		    RegistryItem *lmc_item = pc->GetLMCRegistryItem();
		    
		    if (!pmi_item || !lmc_item) {
#ifdef WIN32
			//char foo[512];
			//sprintf(foo,"Sorry, FreeAmp currently does not support the file %s\n\nClick Next then Play to go to the next song",pc->m_url);
			//MessageBox(NULL,foo,"FreeAmp Error",MB_OK | MB_TASKMODAL);
#endif
			if (SetState(STATE_Stopped)) {
			    SEND_NORMAL_EVENT(INFO_Stopped);
			}
			if (!m_plm->NextIsSame()) {
			    AcceptEvent(new Event(CMD_NextMediaPiece));
			    if (m_playerState == STATE_Paused || (pC->Type() == CMD_PlayPaused)) {
				AcceptEvent(new Event(CMD_PlayPaused));
			    } else {
				AcceptEvent(new Event(CMD_Play));
			    }
			} else {
			    if (SetState(STATE_Stopped)) {
				SEND_NORMAL_EVENT(INFO_Stopped);
			    }
			    
			}
			delete pC;
			return 0;
		    }
		    
		    if(pmi_item) {
			pmi = (PhysicalMediaInput *)pmi_item->InitFunction()();
			error = pmi->SetTo(pc->m_url);
			
			if(IsError(error))
			{
			    delete pmi;
			    break;
			}
		    }
		    
		    RegistryItem *item = m_pmoRegistry->GetItem(0);
		    
		    if(item) {
			pmo = (PhysicalMediaOutput *)item->InitFunction()();
		    }
		    
		    error = kError_NoErr;
		    if(lmc_item) {
			m_lmc = (LogicalMediaConverter *)lmc_item->InitFunction()();
			
			if ((error = m_lmc->SetTarget((EventQueue *)this)) != kError_NoErr) {
			    DISPLAY_ERROR(m_lmc,error);
			    return 0;
			}
			if ((error = m_lmc->SetPMI(pmi)) != kError_NoErr) {
			    DISPLAY_ERROR(m_lmc,error);
			    return 0;
			}
			if ((error = m_lmc->SetPMO(pmo)) != kError_NoErr) {
			    DISPLAY_ERROR(m_lmc,error);
			    return 0;
			}
		    }
		    
		    if ((error = m_lmc->InitDecoder()) != kError_NoErr) {
			DISPLAY_ERROR(m_lmc,error);
			delete pC;
			return 0;
		    }
		    
		    if ((error = m_lmc->ChangePosition(m_plm->GetSkip())) != kError_NoErr) {
			DISPLAY_ERROR(m_lmc,error);
			delete pC;
			return 0;
		    }
		    if ((m_playerState == STATE_Paused) || (pC->Type() == CMD_PlayPaused)) {
			if ((error = m_lmc->Pause()) != kError_NoErr) {
			    DISPLAY_ERROR(m_lmc,error);
			    delete pC;
			    return 0;
			}
			if ((error = m_lmc->Decode()) != kError_NoErr) {
			    DISPLAY_ERROR(m_lmc,error);
			    delete pC;
			    return 0;
			}
			if (SetState(STATE_Paused)) {
			    SEND_NORMAL_EVENT(INFO_Paused);
			}
		    } else {
			if ((error = m_lmc->Decode()) != kError_NoErr) {
			    DISPLAY_ERROR(m_lmc,error);
			    delete pC;
			    return 0;
			}
			if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
			}
		    }
		} else {
		    m_plm->SetFirst();
		    //cout << "no more in playlist..." << endl;
		    if (m_lmc) {
			m_lmc->Stop();
			delete m_lmc;
			m_lmc = NULL;
		    }
		    if (SetState(STATE_Stopped)) {
			SEND_NORMAL_EVENT(INFO_Stopped);
		    }
		    //cout << "killed lmc" << endl;
		    GetUIManipLock();
		    //cout << "got lock" << endl;
		    
		    Event *e = new Event(INFO_PlayListDonePlay);
		    SendToUI(e);
		    //cout << "sent playlist done playing" << endl;
		    ReleaseUIManipLock();
		    //cout << "released lock" << endl;
		    delete e;
		    
		    //e = new Event(CMD_QuitPlayer);
	            //Player::GetPlayer()->AcceptEvent(e);
		    //delete e;
		    
		}
		delete pC;
		return 0;
		break; 
	    }
	    
	    case CMD_NextMediaPiece:
		m_plm->SetNext();
		if (m_playerState != STATE_Stopped) {
			AcceptEvent(new Event(CMD_Play));
		}
		delete pC;
		return 0;
		break;
	    
	    case CMD_PrevMediaPiece:
		m_plm->SetPrev();
		if (m_playerState != STATE_Stopped) {
			AcceptEvent(new Event(CMD_Play));
		}
		delete pC;
		return 0;
		break;
	    
	    case CMD_Pause: {
		if (m_lmc) {
		    m_lmc->Pause();
		    if (SetState(STATE_Paused)) {
			SEND_NORMAL_EVENT(INFO_Paused);
		    }
		}
		delete pC;
		return 0;
		break;
	    }
	    
	    case CMD_UnPause: {
                if (m_lmc) {
		    m_lmc->Resume();
		    if (SetState(STATE_Playing)) {
			SEND_NORMAL_EVENT(INFO_Playing);
		    }
                }
		delete pC;
                return 0;
                break;
	    }
	    case CMD_TogglePause: {
                if (m_lmc) {
		    if (m_playerState == STATE_Playing) {
                        m_lmc->Pause();
                        if (SetState(STATE_Paused)) {
			    SEND_NORMAL_EVENT(INFO_Paused);
                        }
		    } else if (m_playerState == STATE_Paused) {
                        m_lmc->Resume();
                        if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
                        }
		    }
                }
		delete pC;
                return 0;
                break;
	    }
	    
	    case CMD_QuitPlayer: {
		AcceptEvent(new Event(CMD_Stop));
		// 1) Set "I'm already quitting flag" (or exit if its already Set)
		m_imQuitting = 1;
		// 2) Get CIO/COO manipulation lock
		GetUIManipLock();
		// 3) Count CIO/COO, put into m_quitWaitingFor.
		m_quitWaitingFor = m_uiVector->NumElements();
		// 4) Send CMD_Cleanup event to all CIO/COOs
		
		Event *pe = new Event(CMD_Cleanup);
		SendToUI(pe);
		delete pe;

		delete pC;
		if (m_quitWaitingFor == 0) {
		    if (m_pTermSem) {
			m_pTermSem->Signal();
		    }
		    return 1;
		} else {
		    ReleaseUIManipLock();
		    return 0;
		}
		break; 
            }
	    
	    case INFO_ReadyToDieUI: {
		if (!m_imQuitting) {
		    delete pC;
		    return 0;
		}
		
		m_quitWaitingFor--;
		if (m_quitWaitingFor > 0) {
		    delete pC;
		    return 0;
		}
		
		GetUIManipLock();
		if (m_pTermSem) {
		    m_pTermSem->Signal();
		}
		//Event* pe = new Event(CMD_Terminate);
		//SendToUI(pe);
		//delete pe;
		delete pC;
		return 1;
		break; 
	    }
	    
	    case INFO_MediaInfo: {
		    GetUIManipLock();
		    
		    MediaInfoEvent *pmvi = (MediaInfoEvent *)pC;
		    pmvi->m_indexOfSong = m_plm->Current() + 1; // zero based
		    pmvi->m_totalSongs = m_plm->Total();
		    
		    SendToUI(pC);
		    Event *pe = NULL;
		    for (int foobar = 0;foobar < pmvi->m_childEvents->NumElements();foobar++) {
			pe = pmvi->m_childEvents->ElementAt(foobar);
			SendToUI(pe);
		    }
		    ReleaseUIManipLock();
		    return 0;
		    break; 
	    }
	    case INFO_MediaTimeInfo: {
		if(m_playerState == STATE_Playing)
		{
		    GetUIManipLock();
		    SendToUI(pC);
		    ReleaseUIManipLock();
		}
		delete pC;
		return 0;
		break; 
	    }
		case INFO_PlayListShuffle:
		case INFO_PlayListRepeat: 
		case INFO_PlayListUpdated:
			{
				GetUIManipLock();
				SendToUI(pC);
				ReleaseUIManipLock();
				delete pC;
				return 0;
				break;
			}

	    case INFO_LMCError: {
		LMCErrorEvent *e = (LMCErrorEvent *)pC;
		DISPLAY_ERROR(m_lmc,(e->GetError()));
		AcceptEvent(new Event(CMD_NextMediaPiece));
		delete pC;
		return 0;
		break;
	    }
	    default:
		cout << "serviceEvent: Unknown event (i.e. I don't do anything with it): " << pC->Type() << "  Passing..." << endl;
		delete pC;
		return 0;
		break;
		
        }
	//cout << "Done servicing event..." << endl;
    } 
    else {
	cout << "serviceEvent: passed NULL event!!!" << endl;
	return 255;
    }
}

void Player::SendToUI(Event *pe) {
    int32 i;
    for (i = 0;i<m_uiVector->NumElements();i++) {
	m_uiVector->ElementAt(i)->AcceptEvent(pe);
    }
}

/*
void Player::testQueue() {
    Event *pC;
    
    pC = m_eventQueue->Read();
    if (pC) {
	    cout << "testQueue: First failed!!" << endl;
    } else {
	    cout << "testQueue: First succeded!!" << endl;
    }
    cout << "testQueue: IsEmpty(): " << m_eventQueue->IsEmpty() << endl;


    pC = new Event(CMD_Play);
    AcceptEvent(pC);
    pC = new Event(CMD_Play);
    AcceptEvent(pC);
    pC = new Event(CMD_NextMediaPiece);
    AcceptEvent(pC);

    pC = m_eventQueue->Read();
    cout << "testQueue: " << pC->GetEvent() << endl;
    delete pC;
    pC = m_eventQueue->Read();
    cout << "testQueue: " << pC->GetEvent() << endl;
    delete pC;
    cout << "testQueue: IsEmpty(): " << m_eventQueue->IsEmpty() << endl;
    pC = m_eventQueue->Read();
    cout << "testQueue: " << pC->GetEvent() << endl;
    delete pC;
    pC = m_eventQueue->Read();
    if (pC) {
	    cout << "testQueue: Failed!!!" << endl;
    } else {
	    cout << "testQueue: Final Succeeded!!" << endl;
    }
    cout << "testQueue: IsEmpty(): " << m_eventQueue->IsEmpty() << endl;
}*/
