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
    m_eventServiceThread = Thread::CreateThread();
    m_eventServiceThread->Create(Player::EventServiceThreadFunc,this);
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
    m_plm = NULL;
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

    m_pTermSem = NULL;

    //m_autoplay = false;
}

#define TYPICAL_DELETE(x) if (x) { delete x; x = NULL; }

Player::~Player() {

	TYPICAL_DELETE(m_pTermSem);

    //cout << "waiting for service thread to end..." << endl;
    m_eventServiceThread->Join();

    //cout << "serivce thread done.." << endl;
    if (m_lmc) {
        m_lmc->Stop();
        delete m_lmc;
        m_lmc = NULL;
    }

    //cout << "done deleting myLMC" << endl;
	TYPICAL_DELETE(m_eventSem);

    //cout << "Player: deleting event service thread" << endl;
	TYPICAL_DELETE(m_eventServiceThread);

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


void Player::SetArgs(int32 argc, char** argv){
    m_argc = argc;
    m_argv = argv;

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
    sprintf(m_argUI,"%s.ui",pBegin);
#endif
    for(int32 i = 1;i < m_argc; i++) 
    {
	    arg = m_argv[i];

	    if (arg[0] == '-') 
        {
	        switch (arg[1]) 
            {
                case 'u':
                case 'U':
                {
                    if( arg[2] == 'i' ||
                        arg[2] == 'I')
                    {
                        i++;
			if (i >= m_argc) {
			    cerr << "FreeAmp: -ui switch requires an argument" << endl;
			    cerr << "FreeAmp will exit." << endl;
			    exit(1);
			}
			arg = m_argv[i];
			if (m_argUI) delete m_argUI;
                        m_argUI = new char[strlen(arg) + 1];
                        strcpy(m_argUI, arg);
                    }
		    break;
		} 
            }
        }
    }
}

int32 Player::CompareNames(const char *p1, const char *p2) {
// windows plugins and unix plugins are named differently...
#ifdef WIN32
    return strcmp(p1,p2);
#else
//    cout << "Comparing: " << p1 << " to " << p2 << endl;
    int32 i=0; int32 j=0;
    if(!p1 ||!p2) return 1;
    while ((p1[i] == p2[j]) && p1[i] && p2[j]) {
	i++; j++;
    }
    if ((p1[i]=='-') && ((p2[j]=='.') || (p2[j]=='\0'))) {
	return 0;
    } else {
	return 1;
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
		m_ui->Init();
                RegisterActiveUI(m_ui);
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
	    delete pC;
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
                return 0;
		break;
	    }
	    
	    case CMD_ChangePosition: {
        if(m_lmc)
		    m_lmc->ChangePosition(((ChangePositionEvent *)pC)->GetPosition());        
		return 0;
		break;
	    }
	    
	    case CMD_Play: {
		PlayListItem *pc = m_plm->GetCurrent();
		Error error = kError_NoErr;

		if (pc) {
		    if (m_lmc) {
			m_lmc->Stop();
			delete m_lmc;
			m_lmc = NULL;
		    }
		    
		    RegistryItem* item = NULL;
		    PhysicalMediaOutput *pmo = NULL;
		    PhysicalMediaInput  *pmi = NULL;
		    
		    item = m_pmiRegistry->GetItem(0);
		    
		    if(item) {
			pmi = (PhysicalMediaInput *)item->InitFunction()();
			error = pmi->SetTo(pc->m_url);
			    
			if(IsError(error))
			{
			    delete pmi;
			    break;
			}
		    }
		    
		    item = m_pmoRegistry->GetItem(0);
		    
		    if(item) {
			pmo = (PhysicalMediaOutput *)item->InitFunction()();
		    }
		    
		    item = m_lmcRegistry->GetItem(0);
		    Error error = kError_NoErr;
		    if(item) {
			m_lmc = (LogicalMediaConverter *)item->InitFunction()();
			
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
			return 0;
		    }
		    

		    if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
		    }
		    if ((error = m_lmc->ChangePosition(m_plm->GetSkip())) != kError_NoErr) {
			DISPLAY_ERROR(m_lmc,error);
			return 0;
		    }
		    if ((error = m_lmc->Decode()) != kError_NoErr) {
			DISPLAY_ERROR(m_lmc,error);
			return 0;
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
		return 0;
		break; 
	    }
	    
	    case CMD_NextMediaPiece:
		    m_plm->SetNext();
		    AcceptEvent(new Event(CMD_Stop));
		    AcceptEvent(new Event(CMD_Play));
		    return 0;
	    break;
	    
	    case CMD_PrevMediaPiece:
		m_plm->SetPrev();
		AcceptEvent(new Event(CMD_Stop));
		AcceptEvent(new Event(CMD_Play));
		return 0;
		break;
	    
	    case CMD_Pause: {
		if (m_lmc) {
		    m_lmc->Pause();
		    if (SetState(STATE_Paused)) {
			SEND_NORMAL_EVENT(INFO_Paused);
		    }
		}
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
		// 5) Release CIO/COO manipulation lock
		ReleaseUIManipLock();
		return 0;
		break; 
            }
	    
	    case INFO_ReadyToDieUI: {
		if (!m_imQuitting) 
		    return 0;
		
		m_quitWaitingFor--;
		if (m_quitWaitingFor > 0) 
		    return 0;
		
		GetUIManipLock();
		m_pTermSem->Signal();
		//Event* pe = new Event(CMD_Terminate);
		//SendToUI(pe);
		//delete pe;
		return 1;
		break; 
	    }
	    
	    case INFO_MediaInfo: {
		    GetUIManipLock();
		    
		    MediaInfoEvent *pmvi = (MediaInfoEvent *)pC;
		    pmvi->m_indexOfSong = m_plm->Current() + 1; // zero based
		    pmvi->m_totalSongs = m_plm->Total();
		    
		    SendToUI(pC);
		    
		    ReleaseUIManipLock();

            return 0;
		    break; 
	    }
		case INFO_ID3TagInfo:
		case INFO_MPEGInfo:
	    case INFO_MediaTimeInfo: {
            if(m_playerState == STATE_Playing)
            {
		        GetUIManipLock();
		        SendToUI(pC);
		        ReleaseUIManipLock();
            }
		    return 0;
		    break; 
	    }
	    case INFO_LMCError: {
		LMCErrorEvent *e = (LMCErrorEvent *)pC;
		DISPLAY_ERROR(m_lmc,(e->GetError()));
		AcceptEvent(new Event(CMD_NextMediaPiece));
		return 0;
		break;
	    }
	    default:
		cout << "serviceEvent: Unknown event (i.e. I don't do anything with it): " << pC->Type() << "  Passing..." << endl;
		return 0;
		break;
		
        }
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
