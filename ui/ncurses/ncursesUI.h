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

----------------------------------------------------------------------------
ncurses user interface by stephan auerhahn (palpatine@midi.net)

this is a hacked up version of cmdlineUI.h
____________________________________________________________________________*/


// ncursesUI.h


#ifndef _NCURSESUI_H_
#define _NCURSESUI_H_

#include "ui.h"
#include "event.h"
#include "thread.h"
#include "playlist.h"

class FAContext;

class ncursesUI : public UserInterface {
 public:
    ncursesUI(FAContext *context);
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int argc, char **argv);
    virtual void SetTarget(EventQueue *eqr) { m_playerEQ = eqr; }
    virtual Error Init(int32);
    virtual void SetPlayListManager(PlayListManager *);
    static void keyboardServiceFunction(void *);
    virtual ~ncursesUI();
   virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }

 protected:
   FAContext *m_context;

 private:
   Properties *m_propManager;
   void ProcessArgs();
   void showInfo();
   int32 m_startupLevel;
   int32 m_argc;
   char **m_argv;
   EventQueue *m_playerEQ;
   void processSwitch(char *);
   Thread *keyboardListenThread;
   PlayListManager *m_plm;
   int32 m_lastIndexPlayed;
   bool m_id3InfoPrinted;
   float totalTime;
   int totalFrames;
   MpegInfoEvent m_mpegInfo;
   bool m_mpegInfo_set;
   MediaInfoEvent *m_mediaInfo;
   bool m_mediaInfo_set;
   float lastSeconds;
   int titleStart;
   int titleDir;
   char title[1024];
   int counter;
};


#endif // _COMMANDLINECIO_H_

