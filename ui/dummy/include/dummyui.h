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

#ifndef _DUMMY_UI_H_
#define _DUMMY_UI_H_

#include "ui.h"
#include "semaphore.h"

class DummyUI : public UserInterface {
 public:
    virtual void SetTarget(EventQueue* eq);
    virtual int32 AcceptEvent(Event *);
    virtual void  SetArgs(int32 i, char **c) { return; }

    static int32 AcceptEventStub(UIRef pUI, Event *pe);
    static void Cleanup(UIRef pUI);

    DummyUI(Semaphore*);
    virtual ~DummyUI();

 private:
    Semaphore*      m_termSemaphore;
    EventQueueRef   m_target;

};

#endif // _DUMMY_COO_H_
