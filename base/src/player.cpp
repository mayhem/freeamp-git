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



Player *Player::m_thePlayer = NULL;

Player *Player::GetPlayer() {
    if (m_thePlayer == NULL) {
        m_thePlayer = new Player();
    }
    return m_thePlayer;
}

Player::Player() {
    //cout << "Creating player..." << endl;
    m_eventSem = new Semaphore();
    m_eventQueue = new Queue<Event *>();
    //cout << "Created queue" << endl;
    m_eventServiceThread = Thread::CreateThread();
    m_eventServiceThread->Create(Player::EventServiceThreadFunc,this);
    //cout << "Started event thread" << endl;
    m_uiVector = new Vector<UIRef>();
    //cout << "Created vectors" << endl;
    m_uiManipLock = new Mutex();
    m_lmcMutex = new Mutex();
    m_pmiMutex = new Mutex();
    m_pmoMutex = new Mutex();
    m_uiMutex = new Mutex();
    //cout << "Created mutex" << endl;
    m_imQuitting = 0;
    m_quitWaitingFor = 0;
    m_myPlayList = NULL;
    m_playerState = STATE_Stopped;

    m_lmcRegistry = NULL;
    m_pmiRegistry = NULL;
    m_pmoRegistry = NULL;
    m_uiRegistry = NULL;

    m_lmcRef = NULL;
    m_uiRef = NULL;

    m_argUI = NULL;

    m_argc  = 0;
    m_argv  = NULL;

    //m_autoplay = false;
}

Player::~Player() {
    //cout << "waiting for service thread to end..." << endl;
    m_eventServiceThread->Join();
    //cout << "serivce thread done.." << endl;
    if (m_lmcRef) {
        m_lmcRef->Stop(m_lmcRef);
        m_lmcRef->Cleanup(m_lmcRef);
        delete m_lmcRef;
        m_lmcRef = NULL;
    }

    //cout << "done deleting myLMC" << endl;
    if (m_eventSem) {
        delete m_eventSem;
        m_eventSem = NULL;
    }
    //cout << "Player: deleting event service thread" << endl;
    if (m_eventServiceThread) {
        delete m_eventServiceThread;
        m_eventServiceThread = NULL;
    }
    //cout << "Player: deleting event queue" << endl;
    if (m_eventQueue) {
        delete m_eventQueue;
        m_eventQueue = NULL;
    }
    // Delete PlayList
    //cout << "Player: deleting PlayList" << endl;
    if (m_myPlayList) {
        delete m_myPlayList;
        m_myPlayList = NULL;
    }

    // Delete CIOs
    //cout << "Player: deleting CIOs" << endl;
    if (m_uiVector) {
	for(int i=0;i<m_uiVector->NumElements();i++) {
	    m_uiVector->ElementAt(i)->Cleanup(m_uiVector->ElementAt(i));
	}
        m_uiVector->DeleteAll();
        delete m_uiVector;
        m_uiVector = NULL;
    }


    //cout << "Player: deleting CIO/COO manipulation lock" << endl;
    if (m_uiManipLock) {
        delete m_uiManipLock;
        m_uiManipLock = NULL;
    }

    if (m_lmcMutex) {
        delete m_lmcMutex;
        m_lmcMutex = NULL;
    }

    if (m_pmiMutex) {
        delete m_pmiMutex;
        m_pmiMutex = NULL;
    }

    if (m_pmoMutex) {
        delete m_pmoMutex;
        m_pmoMutex = NULL;
    }

    if (m_uiMutex) {
        delete m_uiMutex;
        m_uiMutex = NULL;
    }

    if(m_lmcRegistry){
        delete m_lmcRegistry;
        m_lmcRegistry = NULL;
    }

    if(m_pmiRegistry){
        delete m_pmiRegistry;
        m_pmiRegistry = NULL;
    }

    if(m_pmoRegistry){
        delete m_pmoRegistry;
        m_pmoRegistry = NULL;
    }

    if(m_uiRegistry){
        delete m_uiRegistry;
        m_uiRegistry = NULL;
    }

    if(m_argUI)
    {
        delete [] m_argUI;
        m_argUI = NULL;
    }
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
    if ((p1[i]=='-') && (p2[j]=='.')) {
	return 0;
    } else {
	return 1;
    }
#endif
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
		UIRef ui = NULL;
        int32 i = 0;
	int32 uisActivated = 0;
        while(item = m_uiRegistry->GetItem(i++))
        {
            if(!CompareNames(item->Name(),name))
            {
                m_uiRef = new UI;
			    item->InitFunction()(m_uiRef);

                EventQueueRef eq = new EventQueue;
                eq->ref = this;
                eq->AcceptEvent = AcceptEventStub;

		    	m_uiRef->SetTarget(m_uiRef, eq);

                m_uiRef->SetArgs(m_uiRef, m_argc, m_argv);

                RegisterActiveUI(m_uiRef);
		uisActivated++;
            }
        }
	if (!uisActivated) {
	    cerr << "No UI's to initialize!!!" << endl;
	    Event *e = new Event(CMD_QuitPlayer);
	    AcceptEvent(e);
	}
    }

    delete [] name;
}

int32 Player::AcceptEventStub(EventQueueRef ref, Event* e){
    Player *pP = (Player*)ref->ref;

    return pP->AcceptEvent(e);
}

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
int32 Player::RegisterActiveUI(UIRef ui) {
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

int32 Player::ServiceEvent(Event *pC) {
    if (pC) {
	//cout << "Player: serviceEvent: servicing Event: " << pC->GetEvent() << endl;
	switch (pC->GetEvent()) {
	    case INFO_DoneOutputting: {  // LMC or PMO sends this when its done outputting whatever.  Now, go on to next piece in playlist
                if (SetState(STATE_Stopped)) {
		    SEND_NORMAL_EVENT(INFO_Stopped);
                }
                m_myPlayList->SetNext();
                Event *e = new Event(CMD_Play);
                AcceptEvent(e);
                return 0;
                break; 
            }
	    
	    case CMD_Stop: {
                if (m_lmcRef) {
		    m_lmcRef->Stop(m_lmcRef);
		    m_lmcRef->Cleanup(m_lmcRef);
		    delete m_lmcRef;
		    m_lmcRef = NULL;
                }
                if (SetState(STATE_Stopped)) {
		    SEND_NORMAL_EVENT(INFO_Stopped);
                }
                return 0;
		    break;
	    }
	    
        case CMD_ChangePosition: {
            m_lmcRef->ChangePosition(m_lmcRef, (int32) pC->GetArgument());        
            return 0;
		    break;
	    }

	    case CMD_Play: {
		PlayListItem *pc = m_myPlayList->GetCurrent();
        Error error = kError_NoErr;

		if (pc) {
		    if (m_lmcRef) {
			    m_lmcRef->Stop(m_lmcRef);
			    m_lmcRef->Cleanup(m_lmcRef);
			    delete m_lmcRef;
			    m_lmcRef = NULL;
		    }
		    
		    RegistryItem* item = NULL;
		    PMORef pmo = NULL;
		    PMIRef pmi = NULL;
		    
		    item = m_pmiRegistry->GetItem(0);
		    
		    if(item) {
			    pmi = new PMI;
			    item->InitFunction()(pmi);
			    error = pmi->SetTo(pmi, pc->m_url);

                if(IsError(error))
                {
                    pmi->Cleanup(pmi);
                    delete pmi;
                    break;
                }
		    }

		    item = m_pmoRegistry->GetItem(0);
		    
		    if(item) {
			    pmo = new PMO;
			    item->InitFunction()(pmo);
		    }
		    
		    item = m_lmcRegistry->GetItem(0);

		    if(item) {
			    m_lmcRef = new LMC;
			    item->InitFunction()(m_lmcRef);

                EventQueueRef eq = new EventQueue;
                eq->ref = this;
                eq->AcceptEvent = AcceptEventStub;

			    m_lmcRef->SetTarget(m_lmcRef, eq);
			    m_lmcRef->SetPMI(m_lmcRef, pmi);
			    m_lmcRef->SetPMO(m_lmcRef, pmo);
		    }
		    
		    m_lmcRef->InitDecoder(m_lmcRef);
		    if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
		    }
		    m_lmcRef->ChangePosition(m_lmcRef, m_myPlayList->GetSkip());
		    m_lmcRef->Decode(m_lmcRef);
		} else {
            m_myPlayList->SetFirst();
		    //cout << "no more in playlist..." << endl;
		    if (m_lmcRef) {
			    m_lmcRef->Stop(m_lmcRef);
			    m_lmcRef->Cleanup(m_lmcRef);
			    delete m_lmcRef;
			    m_lmcRef = NULL;
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
		    m_myPlayList->SetNext();
            AcceptEvent(new Event(CMD_Stop));
            AcceptEvent(new Event(CMD_Play));
		    return 0;
	    break;
	    
        case CMD_PrevMediaPiece:
            m_myPlayList->SetPrev();
            AcceptEvent(new Event(CMD_Stop));
            AcceptEvent(new Event(CMD_Play));
            return 0;
            break;
		
	    case CMD_Pause: {
		if (m_lmcRef) {
		    m_lmcRef->Pause(m_lmcRef);
		    if (SetState(STATE_Paused)) {
			SEND_NORMAL_EVENT(INFO_Paused);
		    }
		}
		return 0;
		break;
	    }
	    
	    case CMD_UnPause: {
                if (m_lmcRef) {
		    m_lmcRef->Resume(m_lmcRef);
		    if (SetState(STATE_Playing)) {
			SEND_NORMAL_EVENT(INFO_Playing);
		    }
                }
                return 0;
                break;
	    }
	    case CMD_TogglePause: {
                if (m_lmcRef) {
		    if (m_playerState == STATE_Playing) {
                        m_lmcRef->Pause(m_lmcRef);
                        if (SetState(STATE_Paused)) {
			    SEND_NORMAL_EVENT(INFO_Paused);
                        }
		    } else if (m_playerState == STATE_Paused) {
                        m_lmcRef->Resume(m_lmcRef);
                        if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
                        }
		    }
                }
                return 0;
                break;
	    }
	    
	    case CMD_SetPlaylist:

            if (m_myPlayList) {
                delete m_myPlayList;
                m_myPlayList = NULL;
            }

		    m_myPlayList = (PlayList *)pC->GetArgument();
            m_myPlayList->SetFirst();

		    return 0;
	    	break;
		
	    case CMD_QuitPlayer: {
		Event *e = new Event(CMD_Stop);
		AcceptEvent(e);
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
		
		if (m_quitWaitingFor) 
		    return 0;
		
		GetUIManipLock();
		Event* pe = new Event(CMD_Terminate);
		SendToUI(pe);
		delete pe;
		return 1;
		break; 
	    }
	    
	    case INFO_MediaVitalStats: {
		    GetUIManipLock();

            MediaVitalInfo *pmvi = (MediaVitalInfo*)pC->GetArgument();
            pmvi->indexOfSong = m_myPlayList->Current() + 1; // zero based
            pmvi->totalSongs = m_myPlayList->Total();
        
		    SendToUI(pC);

		    ReleaseUIManipLock();
		    delete pmvi;
		    return 0;
		    break; 
        }
	    
	    case INFO_MediaTimePosition: {
		GetUIManipLock();
		SendToUI(pC);
		ReleaseUIManipLock();
		delete ((MediaTimePositionInfo *)pC->GetArgument());
		return 0;
		break; 
	    }
	    
	    default:
		cout << "serviceEvent: Unknown event (i.e. I don't do anything with it): " << pC->GetEvent() << "  Passing..." << endl;
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
	m_uiVector->ElementAt(i)->AcceptEvent(m_uiVector->ElementAt(i), pe);
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
