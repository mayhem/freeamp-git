
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


//#include "localfileinput.h"
//#include "soundcardpmo.h"
//#include "xinglmc.h"
#include "semaphore.h"

#include "eventdata.h"



Player *Player::thePlayer = NULL;

Player *Player::getPlayer() {
    if (thePlayer == NULL) {
        thePlayer = new Player();
    }
    return thePlayer;
}

Player::Player() {
    event_sem = new Semaphore();
    event_queue = new Queue<Event *>();
    //cout << "Created queue" << endl;
    event_service_thread = new Thread();
    event_service_thread->Create(Player::EventServiceThreadFunc,this);
    //cout << "Started event thread" << endl;
    cio_vector = new Vector<CIO *>();
    coo_vector = new Vector<COO *>();
    cio_death_vector = new Vector<CIO *>();
    coo_death_vector = new Vector<COO *>();
    //cout << "Created vectors" << endl;
    coManipLock = new Mutex();
    //cout << "Created mutex" << endl;
    imQuitting = 0;
    quitWaitingFor = 0;
    myPlayList = NULL;
    myLMC = NULL;
    playerState = STATE_Stopped;
}

Player::~Player() {
    //cout << "waiting for service thread to end..." << endl;
    event_service_thread->Join();
    //cout << "serivce thread done.." << endl;
    if (myLMC) {
        myLMC->Stop();
        delete myLMC;
        myLMC = NULL;
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
}

THREAD_RETURN THREAD_LINKAGE Player::EventServiceThreadFunc(void *pPlayer) {
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
	        rtnVal = pP->serviceEvent(pC);
	        delete pC;
        }
        //if (pP->event_queue->isEmpty()) usleep(50000);
    }
//    cout << "EventServiceThreadFunc: quitting on " << rtnVal << endl;
    // Time to quit!!!

	return 0;
}

int32 Player::registerCOO(COO *ed) {
    //cout << "Registering a COO..." << endl;
    getCOManipLock();
//    cout << "Got comaniplock" << endl;
    if (coo_vector && ed) {
        //cout << "about to insert..>" << endl;
        coo_vector->insert(ed);
        //cout <<"inserted" << endl;
        releaseCOManipLock();
        //cout <<"released" << endl;
        return 0;
    } else {
        releaseCOManipLock();
        return 255;
    }
}

int32 Player::registerCIO(CIO *ed) {
    getCOManipLock();
    if (cio_vector) {
        cio_vector->insert(ed);
        releaseCOManipLock();
        return 0;
    } else {
        releaseCOManipLock();
        return 255;
    }
}

void Player::getCOManipLock() {
    coManipLock->Acquire(WAIT_FOREVER);
}

void Player::releaseCOManipLock() {
    coManipLock->Release();
}

int32 Player::acceptEvent(Event c) {
    Event *newEvent = new Event(c);
    event_queue->write(newEvent);
    event_sem->Signal();
    return 0;
}

bool Player::setState(PlayerState ps) {
    //cout << "Changing from " << playerState << " to " << ps << endl;
    if (ps == playerState) 
        return false;
    playerState = ps;
    return true;
}


#define SEND_NORMAL_EVENT(e)  { Event *ev = new Event(e); getCOManipLock(); sendToCOO(ev); releaseCOManipLock(); delete ev; }

int32 Player::serviceEvent(Event *pC) {
    if (pC) {
	//cout << "Player: serviceEvent: servicing Event: " << pC->getEvent() << endl;
	switch (pC->getEvent()) {
	    case INFO_DoneOutputting: {  // LMC or PMO sends this when its done outputting whatever.  Now, go on to next piece in playlist
            if (setState(STATE_Stopped)) {
	            SEND_NORMAL_EVENT(INFO_Stopped);
            }
            myPlayList->setNext();
            Event *e = new Event(CMD_Play);
            acceptEvent(*e);
            delete e;
            return 0;
            break; 
        }

	    case CMD_Stop: {
            if (myLMC) {
	            myLMC->Stop();
	            delete myLMC;
	            myLMC = NULL;
            }
            if (setState(STATE_Stopped)) {
	            SEND_NORMAL_EVENT(INFO_Stopped);
            }
            return 0;
		break;
	    }

	    case CMD_Play: {
            PlayListItem *pc = myPlayList->getCurrent();
            if (pc) {
	            // MP3 from disk to soundcard proof of concept steps:
	            // 1) Create a LocalFileInput with pc
	            // 2) Create a SoundCardOutput
	            // 3) Create a XingLMC(LocalFilePMI,SoundCardPMO)
	            // 4) go nuts
                if (myLMC) {
                    myLMC->Stop();
	                delete myLMC;
	                myLMC = NULL;
	            }

	            PhysicalMediaInput* pmi = NULL;
	            //cout << "Done deleting myLMC" << endl;
	            //if(pc->type == 0)
	            //	pmi = new LocalFileInput(pc->url);

	            //cout << "New PMI..." << endl;
	            //SoundCardPMO *scPMO = new SoundCardPMO();
	            //cout << "New scPMO..." << endl;
	            //myLMC = new MAPlayLMC(lfPMI,scPMO);
	            //myLMC = new XingLMC(pmi,scPMO);
	            //cout << "Created xing..." << endl;
	            if (setState(STATE_Playing)) {
	                SEND_NORMAL_EVENT(INFO_Playing);
	            }
	            myLMC->ChangePosition(myPlayList->getSkip());
	            myLMC->Decode();
	            //cout << "Kicked off decoder..." << endl;
	            
            } else {
	            //cout << "no more in playlist..." << endl;
	            if (myLMC) {
	                myLMC->Stop();
	                delete myLMC;
	                myLMC = NULL;
	            }
	            if (setState(STATE_Stopped)) {
	                SEND_NORMAL_EVENT(INFO_Stopped);
	            }
	            //cout << "killed lmc" << endl;
	            getCOManipLock();
	            //cout << "got lock" << endl;
	            
	            Event *e = new Event(INFO_PlayListDonePlay);
	            sendToCIOCOO(e);
	            //cout << "sent playlist done playing" << endl;
	            releaseCOManipLock();
	            //cout << "released lock" << endl;
	            delete e;
            }
            return 0;
		    break; 
        }

	    case CMD_NextMediaPiece:
            myPlayList->setNext();
            return 0;
            break;

        case CMD_PrevMediaPiece:
            myPlayList->setPrev();
            return 0;
            break;

	    case CMD_Pause: {
		    if (myLMC) {
		        myLMC->Pause();
		        if (setState(STATE_Paused)) {
			    SEND_NORMAL_EVENT(INFO_Paused);
		        }
		    }
		    return 0;
		    break;
	    }

	    case CMD_UnPause: {
            if (myLMC) {
	            myLMC->Resume();
	            if (setState(STATE_Playing)) {
	                SEND_NORMAL_EVENT(INFO_Playing);
	            }
            }
            return 0;
            break;
	    }
	    case CMD_TogglePause: {
            if (myLMC) {
	            if (playerState == STATE_Playing) {
                    myLMC->Pause();
                    if (setState(STATE_Paused)) {
	                    SEND_NORMAL_EVENT(INFO_Paused);
                    }
	            } else if (playerState == STATE_Paused) {
                    myLMC->Resume();
                    if (setState(STATE_Playing)) {
	                    SEND_NORMAL_EVENT(INFO_Playing);
                    }
	            }
            }
            return 0;
            break;
	    }
	    case CMD_SetPlaylist:
		myPlayList = (PlayList *)pC->getArgument();
		//myPlayList->setFirst();  // Should be done by object creating the playlist
		return 0;
		break;
	    case CMD_QuitPlayer: {
		Event *e = new Event(CMD_Stop);
		acceptEvent(*e);
		delete e;
		// 1) Set "I'm already quitting flag" (or exit if its already set)
		imQuitting = 1;
		// 2) Get CIO/COO manipulation lock
		getCOManipLock();
		// 3) Count CIO/COO, put into quitWaitingFor.
		quitWaitingFor = cio_vector->numElements() + coo_vector->numElements();
		// 4) Send CMD_Cleanup event to all CIO/COOs
		
		Event *pe = new Event(CMD_Cleanup);
		sendToCIOCOO(pe);
		delete pe;
		// 5) Release CIO/COO manipulation lock
		releaseCOManipLock();
		return 0;
		break; }
	    case INFO_ReadyToDieCOO: {
		if (!imQuitting) return 0;
		if (pC->getArgument()) {
		    COO *pCOO = (COO *)(pC->getArgument());
		    //printf("having %x killed(COO)\n",pCOO);
		    coo_death_vector->insert(pCOO);
		}
		quitWaitingFor--;
		if (quitWaitingFor) return 0;
		getCOManipLock();
		//cout << "got comaniplock " << endl;
		Event* pe = new Event(CMD_Terminate);
		//cout << "sending to coo's" << endl;
		sendToCOO(pe);
		//cout << "done sending to coo's" << endl;
		delete pe;
		//cout << "ending inforeadytodieCOO" << endl;
		return 1;
		break; }
	    case INFO_ReadyToDieCIO: {
		if (!imQuitting) return 0;
		if (pC->getArgument()) {
		    CIO *pCIO = (CIO *)(pC->getArgument());
		    //printf("having %x killed (CIO)\n",pCIO);
		    cio_death_vector->insert(pCIO);
		}
		quitWaitingFor--;
		if (quitWaitingFor) return 0;
		getCOManipLock();
		//cout << "got COManipLock" << endl;
		Event* pe = new Event(CMD_Terminate);
		sendToCOO(pe);
		delete pe;
		//cout << "ending InfoReadyToDie..." << endl;
		return 1;
		break; }
	    case INFO_MediaVitalStats: {
		// decoder doesn't yet grab song title...
		//cout << "Servicing media vital stats..." << endl;
		getCOManipLock();
		//cout << "got lock" << endl;
		sendToCOO(pC);
		//cout << "sent to all" << endl;
		releaseCOManipLock();
		//cout << "Released manip lock..." << endl;
		delete ((MediaVitalInfo *)pC->getArgument());
		//cout << "Done servicing mediavitalstats event" << endl;
		return 0;
		break; }
	    case INFO_MediaTimePosition: {
		getCOManipLock();
		sendToCOO(pC);
		releaseCOManipLock();
		delete ((MediaTimePositionInfo *)pC->getArgument());
		return 0;
		break; }
	    default:
		cout << "serviceEvent: Unknown event (i.e. I don't do anything with it): " << pC->getEvent() << "  Passing..." << endl;
		return 0;
		break;
	}
    } else {
	cout << "serviceEvent: passed NULL event!!!" << endl;
	return 255;
    }
}
void Player::sendToCIOCOO(Event *pe) {
	int32 i;

    for (i = 0;i<cio_vector->numElements();i++) {
	cio_vector->elementAt(i)->acceptCIOEvent(pe);
    }
    for (i = 0;i<coo_vector->numElements();i++) {
	coo_vector->elementAt(i)->acceptCOOEvent(pe);
    }
}

void Player::sendToCOO(Event *pe) {
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
    acceptEvent(*pC);
    delete pC;
    pC = new Event(CMD_Play);
    acceptEvent(*pC);
    delete pC;
    pC = new Event(CMD_NextMediaPiece);
    acceptEvent(*pC);
    delete pC;

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
