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
// CommandLineCIO.h


#ifndef _COMMANDLINECIO_H_
#define _COMMANDLINECIO_H_

#include "ctrlobj.h"
#include "event.h"
#include "thread.h"
#include "playlist.h"

class cmdlineUI : public UserInterface {
 public:
    cmdlineUI();
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int argc, char **argv);
    virtual void SetRef(UIRef uir) { return; }
    virtual void SetTarget(EventQueueRef eqr) { m_playerEQ = eqr; }
    static void keyboardServiceFunction(void *);
    virtual ~cmdlineUI();
 private:
    EventQueueRef m_playerEQ;
    void processSwitch(char *);
    Thread *keyboardListenThread;
    PlayList *mypl;
};


#endif // _COMMANDLINECIO_H_
