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
#include "fawindow.h"
#include "windowhash.h"

class FreeAmpUI : public UserInterface {
 public:
    FreeAmpUI();
    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int argc, char **argv);
    virtual void SetTarget(EventQueue *eqr) { m_playerEQ = eqr; }
    virtual Error Init(int32);
    virtual void SetPlayListManager(PlayListManager *);
    static void x11ServiceFunction(void *);
    static void TimerEventFunction(void *);
    virtual ~FreeAmpUI();

    EventQueue *m_playerEQ;
   virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }
 private:
   Properties *m_propManager;
    int32 m_startupType;
    void Usage();
    bool m_noStartUp;

    bool m_initialized;
    int32 m_oldLcdState;
    int32 m_volume;
    int32 m_mixerFd;
    bool m_timerContinue;
    bool m_needsWiggling;
    Thread *m_timerThread;

    int32 m_totalSeconds;
    int32 m_currSeconds;
    int32 m_seekSeconds;
    float m_secondsPerFrame;

    FAMainWindow *m_mainWindow;
    FADialWindow *m_volumeWindow;
    static void VolumeDialFunction(void *,int32,int32,int32);
    FADialWindow *m_seekWindow;
    static void SeekDialFunction(void *,int32,int32,int32);
    void SeekJogAction(); // called by timer thread
    int32 m_seekSlice;
    int32 m_seekDelta;
    FADumbWindow *m_lcdUpperWindow;
    FALcdWindow *m_lcdWindow;
    FADumbWindow *m_playListDrawerWindow;

    FATriStateWindow *m_playButton;
    static void playFunction(void *);
    FATriStateWindow *m_stopButton;
    static void stopFunction(void *);
    FATriStateWindow *m_pauseButton;
    static void pauseFunction(void *);
    FATriStateWindow *m_prevButton;
    static void prevFunction(void *);
    FATriStateWindow *m_nextButton;
    static void nextFunction(void *);

    FATriStateWindow *m_switchModeButton;
    static void switchModeFunction(void *);
    FATriStateWindow *m_minimizeButton;
    static void minimizeFunction(void *);
    FATriStateWindow *m_closeButton;
    static void closeFunction(void *);
    FATriStateWindow *m_repeatButton;
    static void repeatFunction(void *);
    FATriStateWindow *m_shuffleButton;
    static void shuffleFunction(void *);
    FATriStateWindow *m_openButton;
    static void openFunction(void *);

    WindowHash *m_windowHash;
    
    bool m_done;
    
    Display *m_display;
    int32 m_screenNum;
    Pixmap m_doubleBufferPixmap;
    //Window m_mainWindow;
    Pixmap m_iconPixmap;
    GC m_gc;
    void X11EventService();

    int32 m_argc;
    char **m_argv;
    void processSwitch(char *);
    void ParseArgs();
    Thread *gtkListenThread;
    PlayListManager *m_plm;
};


#endif // _FreeAmpUI_H_




