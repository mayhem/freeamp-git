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
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	$Id$
____________________________________________________________________________*/

#include <iostream.h>

#include "event.h"
#include "player.h"
#include "thread.h"
#include "debug.h"
#include "vector.h"
#include "ctrlobj.h"
#include "queue.h"
#ifdef __linux__
#include "localfileinput.h"
#include "soundcardpmo.h"
#include "xinglmc.h"
#endif
#include "semaphore.h"
#include "eventdata.h"
#include "registrar.h"



Player *Player::thePlayer = NULL;

Player *Player::GetPlayer() {
    if (thePlayer == NULL) {
        thePlayer = new Player();
    }
    return thePlayer;
}

Player::Player() {
    event_sem = new Semaphore();
    event_queue = new Queue<Event *>();
    //cout << "Created queue" << endl;
    event_service_thread = Thread::CreateThread();
    event_service_thread->Create(Player::EventServiceThreadFunc,this);
    //cout << "Started event thread" << endl;
    cio_vector = new Vector<CIO *>();
    coo_vector = new Vector<COO *>();
    cio_death_vector = new Vector<CIO *>();
    coo_death_vector = new Vector<COO *>();
    //cout << "Created vectors" << endl;
    coManipLock = new Mutex();
    m_lmcMutex = new Mutex();
    m_pmiMutex = new Mutex();
    m_pmoMutex = new Mutex();
    m_uiMutex = new Mutex();
    //cout << "Created mutex" << endl;
    imQuitting = 0;
    quitWaitingFor = 0;
    myPlayList = NULL;
    myLMC = NULL;
    playerState = STATE_Stopped;

    m_lmcRegistry = NULL;
    m_pmiRegistry = NULL;
    m_pmoRegistry = NULL;
    m_uiRegistry = NULL;

    m_lmcRef = NULL;
}

Player::~Player() {
    //cout << "waiting for service thread to end..." << endl;
    event_service_thread->Join();
    //cout << "serivce thread done.." << endl;
    if (m_lmcRef) {
        m_lmcRef->Stop(m_lmcRef);
        m_lmcRef->Cleanup(m_lmcRef);
        delete m_lmcRef;
        m_lmcRef = NULL;
    }
    //cout << "done deleting myLMC" << endl;
    if (event_sem) {
        delete event_sem;
        event_sem = NULL;
    }
    //cout << "Player: deleting event service thread" << endl;
    if (event_service_thread) {
        delete event_service_thread;
        event_service_thread = NULL;
    }
    //cout << "Player: deleting event queue" << endl;
    if (event_queue) {
        delete event_queue;
        event_queue = NULL;
    }
    // Delete PlayList
    //cout << "Player: deleting PlayList" << endl;
    if (myPlayList) {
        delete myPlayList;
        myPlayList = NULL;
    }

    // Delete CIOs
    //cout << "Player: deleting CIOs" << endl;
    if (cio_vector) {
        //cio_vector->DeleteAll();
        delete cio_vector;
        cio_vector = NULL;
    }
    // Delete COOs
    //cout << "Player: deleting COOs" << endl;
    if (coo_vector) {
        //coo_vector->DeleteAll();
        delete coo_vector;
        coo_vector = NULL;
    }
    //cout << "Actaully deleting registered deletions" << endl;
    if (cio_death_vector) {
        cio_death_vector->DeleteAll();
        delete cio_death_vector;
        cio_death_vector = NULL;
    }
    //cout << "Actually deleting registered deletions" << endl;
    if (coo_death_vector) {
        coo_death_vector->DeleteAll();
        delete coo_death_vector;
        coo_death_vector = NULL;
    }
    //cout << "Player: deleting CIO/COO manipulation lock" << endl;
    if (coManipLock) {
        delete coManipLock;
        coManipLock = NULL;
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
}
void Player::EventServiceThreadFunc(void *pPlayer) {
    //cout << "Beginning event service" << endl;
    Player *pP = (Player *)pPlayer;
    Event *pC;
    int32 rtnVal = 0x0;
    while (rtnVal == 0) {   // serviceEvent will return 1 if error or time to quit.
        //cout << "About to read queue..." << endl;
        pP->event_sem->Wait();
        pC = pP->event_queue->read();
        //cout << "Read queue..." << endl;
        if (pC) {
	        rtnVal = pP->ServiceEvent(pC);
	        delete pC;
        }
        //if (pP->event_queue->isEmpty()) usleep(50000);
    }
//    cout << "EventServiceThreadFunc: quitting on " << rtnVal << endl;
    // Time to quit!!!
}

int32 Player::RegisterCOO(COO *ed) {
    //cout << "Registering a COO..." << endl;
    GetCOManipLock();
//    cout << "Got comaniplock" << endl;
    if (coo_vector && ed) {
        //cout << "about to insert..>" << endl;
        coo_vector->insert(ed);
        //cout <<"inserted" << endl;
        ReleaseCOManipLock();
        //cout <<"released" << endl;
        return 0;
    } else {
        ReleaseCOManipLock();
        return 255;
    }
}

int32 Player::RegisterCIO(CIO *ed) {
    GetCOManipLock();
    if (cio_vector) {
        cio_vector->insert(ed);
        ReleaseCOManipLock();
        return 0;
    } else {
        ReleaseCOManipLock();
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

void Player::GetCOManipLock() {
    coManipLock->Acquire(WAIT_FOREVER);
}

void Player::ReleaseCOManipLock() {
    coManipLock->Release();
}

int32 Player::AcceptEvent(Event *e) {
    Event* copy = new Event(*e);
    event_queue->write(copy);
    event_sem->Signal();
    return 0;
}

bool Player::SetState(PlayerState ps) {
    //cout << "Changing from " << playerState << " to " << ps << endl;
    if (ps == playerState) 
        return false;
    playerState = ps;
    return true;
}


#define SEND_NORMAL_EVENT(e)  { Event *ev = new Event(e); GetCOManipLock(); SendToCOO(ev); ReleaseCOManipLock(); delete ev;}

int32 Player::ServiceEvent(Event *pC) {
    if (pC) {
	//cout << "Player: serviceEvent: servicing Event: " << pC->GetEvent() << endl;
	switch (pC->getEvent()) {
	    case INFO_DoneOutputting: {  // LMC or PMO sends this when its done outputting whatever.  Now, go on to next piece in playlist
                if (SetState(STATE_Stopped)) {
		    SEND_NORMAL_EVENT(INFO_Stopped);
                }
                myPlayList->SetNext();
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
	    
	    case CMD_Play: {
            PlayListItem *pc = myPlayList->GetCurrent();
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

                if(item)
                {
                    pmi = new PMI;
                    item->InitFunction()(pmi);
                    pmi->SetTo(pmi, pc->url);
                }

                item = m_pmoRegistry->GetItem(0);

                if(item)
                {
                    pmo = new PMO;
                    item->InitFunction()(pmo);
                }
                
                item = m_lmcRegistry->GetItem(0);

                if(item)
                {
                    m_lmcRef = new LMC;
                    item->InitFunction()(m_lmcRef);
                    m_lmcRef->SetInfoEventQueue(m_lmcRef, (EventQueue *)this);
		            m_lmcRef->SetPMI(m_lmcRef, pmi);
		            m_lmcRef->SetPMO(m_lmcRef, pmo);
                }

                m_lmcRef->Init(m_lmcRef);
		        if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
		        }
		        m_lmcRef->ChangePosition(m_lmcRef, myPlayList->GetSkip());
		        m_lmcRef->Decode(m_lmcRef);

#if 0
                
		        PhysicalMediaInput* pmi = NULL;
		        //cout << "Done deleting myLMC" << endl;
		        if(pc->type == 0)
			    pmi = new LocalFileInput(pc->url);
		        
		        //cout << "New PMI..." << endl;
		        SoundCardPMO *scPMO = new SoundCardPMO();
		        //cout << "New scPMO..." << endl;
		        myLMC = new XingLMC();
		        //cout << "Created xing..." << endl;
		        myLMC->SetInfoEventQueue((EventQueue *)this);
		        myLMC->SetPMI(pmi);
		        myLMC->SetPMO(scPMO);

		        myLMC->Init();
		        if (SetState(STATE_Playing)) {
			    SEND_NORMAL_EVENT(INFO_Playing);
		        }
		        myLMC->ChangePosition(myPlayList->GetSkip());
		        myLMC->Decode();
		        //cout << "Kicked off decoder..." << endl;
#endif		
            } else {
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
		        GetCOManipLock();
		        //cout << "got lock" << endl;
		        
		        Event *e = new Event(INFO_PlayListDonePlay);
		        SendToCIOCOO(e);
		        //cout << "sent playlist done playing" << endl;
		        ReleaseCOManipLock();
		        //cout << "released lock" << endl;
		        delete e;

                e = new Event(CMD_QuitPlayer);
	            Player::GetPlayer()->AcceptEvent(e);
                delete e;

            }
            return 0;
		    break; 
        }
	    
	    case CMD_NextMediaPiece:
                myPlayList->SetNext();
                return 0;
                break;
		
            case CMD_PrevMediaPiece:
                myPlayList->SetPrev();
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
		    if (playerState == STATE_Playing) {
                        m_lmcRef->Pause(m_lmcRef);
                        if (SetState(STATE_Paused)) {
			    SEND_NORMAL_EVENT(INFO_Paused);
                        }
		    } else if (playerState == STATE_Paused) {
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
		myPlayList = (PlayList *)pC->getArgument();
		//myPlayList->SetFirst();  // Should be done by object creating the playlist
		return 0;
		break;
		
	    case CMD_QuitPlayer: {
		Event *e = new Event(CMD_Stop);
		AcceptEvent(e);
		// 1) Set "I'm already quitting flag" (or exit if its already Set)
		imQuitting = 1;
		// 2) Get CIO/COO manipulation lock
		GetCOManipLock();
		// 3) Count CIO/COO, put into quitWaitingFor.
		quitWaitingFor = cio_vector->numElements() + coo_vector->numElements();
		// 4) Send CMD_Cleanup event to all CIO/COOs
		
		Event *pe = new Event(CMD_Cleanup);
		SendToCIOCOO(pe);
		delete pe;
		// 5) Release CIO/COO manipulation lock
		ReleaseCOManipLock();
		return 0;
		break; 
            }
	    
	    case INFO_ReadyToDieCOO: {
		if (!imQuitting) 
                    return 0;
		
                if (pC->getArgument()) {
		    COO *pCOO = (COO *)(pC->getArgument());
		    //printf("having %x killed(COO)\n",pCOO);
		    coo_death_vector->insert(pCOO);
                }
		
		quitWaitingFor--;
		
		if (quitWaitingFor) 
                    return 0;
		
                GetCOManipLock();
                //cout << "got comaniplock " << endl;
                Event* pe = new Event(CMD_Terminate);
                //cout << "sending to coo's" << endl;
                SendToCOO(pe);
                //cout << "done sending to coo's" << endl;
                delete pe;
                //cout << "ending inforeadytodieCOO" << endl;
                return 1;
		break; 
            }
	    
	    case INFO_ReadyToDieCIO: {
                if (!imQuitting) return 0;
                if (pC->getArgument()) {
		    CIO *pCIO = (CIO *)(pC->getArgument());
		    //printf("having %x killed (CIO)\n",pCIO);
		    cio_death_vector->insert(pCIO);
                }
		
		quitWaitingFor--;
		if (quitWaitingFor) 
                    return 0;
		
		GetCOManipLock();
		//cout << "got COManipLock" << endl;
		Event* pe = new Event(CMD_Terminate);
		SendToCOO(pe);
		delete pe;
		//cout << "ending InfoReadyToDie..." << endl;
		return 1;
		break; 
            }
	    
	    case INFO_MediaVitalStats: {
		// decoder doesn't yet grab song title...
		//cout << "Servicing media vital stats..." << endl;
		GetCOManipLock();
		//cout << "got lock" << endl;
		SendToCOO(pC);
		//cout << "sent to all" << endl;
		ReleaseCOManipLock();
		//cout << "Released manip lock..." << endl;
		delete ((MediaVitalInfo *)pC->getArgument());
		//cout << "Done servicing mediavitalstats event" << endl;
		return 0;
		break; 
            }
	    
	    case INFO_MediaTimePosition: {
		GetCOManipLock();
		SendToCOO(pC);
		ReleaseCOManipLock();
		delete ((MediaTimePositionInfo *)pC->getArgument());
		return 0;
		break; 
            }
	    
            default:
		cout << "serviceEvent: Unknown event (i.e. I don't do anything with it): " << pC->getEvent() << "  Passing..." << endl;
		return 0;
		break;
		
        }
    } 
    else {
	cout << "serviceEvent: passed NULL event!!!" << endl;
	return 255;
    }
}
void Player::SendToCIOCOO(Event *pe) {
    int32 i;
    
    for (i = 0;i<cio_vector->numElements();i++) {
	cio_vector->elementAt(i)->acceptCIOEvent(pe);
    }
    
    for (i = 0;i<coo_vector->numElements();i++) {
	coo_vector->elementAt(i)->acceptCOOEvent(pe);
    }
}

void Player::SendToCOO(Event *pe) {
    //cout << "Sending a " << pe->getEvent() << endl;
    for (int32 i=0;i<coo_vector->numElements();i++) {
	    //cout << "sending to " << i << endl;
	    coo_vector->elementAt(i)->acceptCOOEvent(pe);
	    //cout << "done from " << i << endl;
    }
}
void Player::testQueue() {
    Event *pC;
    
    pC = event_queue->read();
    if (pC) {
	    cout << "testQueue: First failed!!" << endl;
    } else {
	    cout << "testQueue: First succeded!!" << endl;
    }
    cout << "testQueue: isEmpty(): " << event_queue->isEmpty() << endl;


    pC = new Event(CMD_Play);
    AcceptEvent(pC);
    pC = new Event(CMD_Play);
    AcceptEvent(pC);
    pC = new Event(CMD_NextMediaPiece);
    AcceptEvent(pC);

    pC = event_queue->read();
    cout << "testQueue: " << pC->getEvent() << endl;
    delete pC;
    pC = event_queue->read();
    cout << "testQueue: " << pC->getEvent() << endl;
    delete pC;
    cout << "testQueue: isEmpty(): " << event_queue->isEmpty() << endl;
    pC = event_queue->read();
    cout << "testQueue: " << pC->getEvent() << endl;
    delete pC;
    pC = event_queue->read();
    if (pC) {
	    cout << "testQueue: Failed!!!" << endl;
    } else {
	    cout << "testQueue: Final Succeeded!!" << endl;
    }
    cout << "testQueue: isEmpty(): " << event_queue->isEmpty() << endl;
}
