
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


// player.h

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "config.h"
#include "thread.h"
#include "event.h"
#include "queue.h"
#include "ctrlobj.h"
#include "vector.h"
#include "mutex.h"
#include "playlist.h"
#include "semaphore.h"


#include "lmc.h"
#include "xinglmc.h"

typedef enum {
    STATE_Paused = 1,
    STATE_Playing,
    STATE_Stopped,
} PlayerState;

class Player : public EventQueue {

 public:
    //Player();
    static Player *GetPlayer();
    ~Player();

    virtual int32 AcceptEvent(Event *);
    int32 RegisterCOO(COO *);
    int32 RegisterCIO(CIO *);
    void testQueue();
    static void EventServiceThreadFunc(void *);
 private:
    Semaphore *event_sem;
    PlayerState playerState;
    static Player *thePlayer;
    Player();
    Queue<Event *> *event_queue;
    Thread *event_service_thread;
    int32 ServiceEvent(Event *);
    int32 quitWaitingFor;  // keeps track of how many CIO's and COO's haven't sent in their "Ready To Die" infos.
    int32 imQuitting;
    Vector<COO *> *coo_vector;
    Vector<CIO *> *cio_vector;
    Vector<COO *> *coo_death_vector;
    Vector<CIO *> *cio_death_vector;
    void GetCOManipLock();
    void ReleaseCOManipLock();
    Mutex *coManipLock;
    PlayList *myPlayList;
    void SendToCIOCOO(Event *);
    void SendToCOO(Event *);
    bool SetState(PlayerState);
    LMC *myLMC;
};

#endif // _PLAYER_H_



