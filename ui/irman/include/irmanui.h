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

#ifndef _IRManUI_H_
#define _IRManUI_H_

#include "ui.h"
#include "event.h"
#include "thread.h"
#include "playlist.h"
#include "hashtable.h"

#define DEFAULT_DEVICE "/dev/irman"

class IRManUI : public UserInterface {
 public:
    IRManUI();
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int argc, char **argv);
    virtual void SetTarget(EventQueue *eqr) { m_playerEQ = eqr; }
    virtual Error Init(int32);
    virtual void SetPlayListManager(PlayListManager *);
    static void irServiceFunction(void *);
    virtual ~IRManUI();
    virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }
 private:
    HashTable<int32 *> m_commands;
    bool m_quitIRListen;
    Properties *m_propManager;
    int32 m_startupType;
    int32 m_argc;
    char **m_argv;
    void ProcessArgs();
    EventQueue *m_playerEQ;
    void processSwitch(char *);
    Thread *irListenThread;
    PlayListManager *m_plm;
};


#endif // _IRManUI_H_


