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
// FreeAmpUI.h


#ifndef _FreeAmpUI_H_
#define _FreeAmpUI_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <X11/xpm.h>

#include <X11/extensions/shape.h>

#include "ui.h"
#include "event.h"
#include "thread.h"
#include "playlist.h"

enum {
    kPlayButton = 0,
    kUpperBound

};
enum {
    kPressed = 0,
    kLit,
    kNotLit
};
class ButtonState {
 public:
    int32 m_state;
};
class FreeAmpUI : public UserInterface {
 public:
    FreeAmpUI();
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int argc, char **argv);
    virtual void SetTarget(EventQueue *eqr) { m_playerEQ = eqr; }
    virtual void Init();
    virtual void SetPlayListManager(PlayListManager *);
    static void x11ServiceFunction(void *);
    virtual ~FreeAmpUI();

    EventQueue *m_playerEQ;
 private:
    ButtonState *m_buttonStates;
    Display *m_display;
    int32 m_screenNum;
    Pixmap m_doubleBufferPixmap;
    Window m_mainWindow;
    Pixmap m_iconPixmap;
    GC m_gc;
    void draw_graphics(Window win, GC gc);
    void X11EventService();

    int32 m_argc;
    char **m_argv;
    void processSwitch(char *);
    Thread *gtkListenThread;
    PlayListManager *m_plm;
};


#endif // _FreeAmpUI_H_


