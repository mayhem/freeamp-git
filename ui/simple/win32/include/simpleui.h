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

#ifndef _SIMPLE_UI_H_
#define _SIMPLE_UI_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "ui.h"
#include "semaphore.h"
#include "event.h"
#include "thread.h"
#include "mutex.h"
#include "queue.h"


class SimpleUI : public UserInterface {
 public:
    SimpleUI();
    ~SimpleUI();

    virtual Error Init(int32 startup_type) { return kError_NoErr;}
    virtual void SetTarget(EventQueue*);
    virtual int32 AcceptEvent(Event*);
    virtual void SetArgs(int32,char**);
    virtual void SetPlayListManager(PlayListManager*);
    virtual Error SetPropManager(Properties* p) 
    { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }
    void CreateUI();

    static BOOL CALLBACK MainProc(	HWND hwnd, 
						            UINT msg, 
						            WPARAM wParam, 
						            LPARAM lParam );
    
    void SetHwnd(HWND hwnd);

    Semaphore*      m_uiSemaphore;

    bool            m_scrolling;


 protected:
      static void UIThreadFunc(void *);


 private:
    PlayListManager*    m_plm;
    Properties*         m_propManager;
    int32			    m_totalSeconds;
    float			    m_secondsPerFrame;
    Thread*             m_uiThread;
    EventQueue*         m_target;

    HWND            m_hwnd;
    HWND            m_hwndPlay;
	HWND            m_hwndStop;
	HWND            m_hwndPause;
	HWND            m_hwndNext;
	HWND            m_hwndLast;
	HWND            m_hwndSlider;
    HWND            m_hwndCurrent;
    HWND            m_hwndTotal;
	HWND            m_hwndStatus;

    
};


#endif // _SIMPLE_UI_H_
