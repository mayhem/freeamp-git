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

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "config.h"
#include "thread.h"
#include "event.h"
#include "queue.h"
#include "ui.h"
#include "vector.h"
#include "mutex.h"
#include "playlist.h"
#include "semaphore.h"
#include "lmcregistry.h"
#include "pmiregistry.h"
#include "pmoregistry.h"
#include "uiregistry.h"
#include "preferences.h"

#include "lmc.h"

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

    int32 RegisterActiveUI(UserInterface *ui);
    int32 RegisterLMCs(LMCRegistry* registry);
    int32 RegisterPMIs(PMIRegistry* registry);
    int32 RegisterPMOs(PMORegistry* registry);
    int32 RegisterUIs(UIRegistry* registry);

    void Run();
    bool SetArgs(int32 argc, char** argv);
    void SetPreferences(Preferences *);
    void SetTerminationSemaphore(Semaphore *);
    void testQueue();
    static void EventServiceThreadFunc(void *);
//    static int32 AcceptEventStub(EventQueueRef ref, Event* e);
    virtual int32 AcceptEvent(Event *);

 protected:
    Player();
    void GetUIManipLock();
    void ReleaseUIManipLock();
    int32 CompareNames(const char *,const char *);
    void SendToUI(Event *);
    void Usage(const char *);
    bool SetState(PlayerState);
    int32 ServiceEvent(Event *);


 private:
    bool                    m_didUsage;
    Preferences*            m_prefs;
    Semaphore*              m_pTermSem;
    static Player*          m_thePlayer;
    Semaphore*              m_eventSem;
    PlayerState             m_playerState;
    Queue<Event *>*         m_eventQueue;
    Thread*                 m_eventServiceThread;
    int32                   m_quitWaitingFor;// keeps track of how many CIO's 
                                             // and COO's haven't sent in 
                                             // their "Ready To Die" infos.
    int32                   m_imQuitting;
    Vector<UserInterface *>*          m_uiVector;
    
    Mutex*                  m_uiManipLock;
    Mutex*                  m_lmcMutex;
    Mutex*                  m_pmiMutex;
    Mutex*                  m_pmoMutex;
    Mutex*                  m_uiMutex;
    PlayListManager*               m_plm;
    
    LogicalMediaConverter * m_lmc;
    UserInterface *         m_ui;

    LMCRegistry*            m_lmcRegistry;
    PMIRegistry*            m_pmiRegistry;
    PMORegistry*            m_pmoRegistry;
    UIRegistry*             m_uiRegistry;

    Vector<char *>*                   m_argUIvector;

    int32                   m_argc;          
    char**                  m_argv;

};

#endif // _PLAYER_H_



