
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


#ifndef _FAWindow_H_
#define _FAWindow_H_
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>

#include <X11/extensions/shape.h>

#include "config.h"
#include "event.h"



class FAWindow {
 protected:
    bool m_mapped;
    Display *m_display;
    int32 m_screenNum;
    GC m_gc;
    Window m_parent;
    Window m_me;
    int32 m_xParent,m_yParent;
    int32 m_width,m_height;
    Pixmap m_pixmap;
    FAWindow();
    EventQueue *m_playerEQ;
 public:
    virtual ~FAWindow();
    Window GetParent();
    Window GetWindow();
    GC GetGC();
    void SetTarget(EventQueue *);
    void SetGC(GC);
    void SetPixmap(Pixmap);
    void SetMask(Pixmap);
    void MapWindow();
    void UnMapWindow();
    void SelectInput(int32);
    bool IsMapped();
    virtual void DoEvent(XEvent) = 0;
    virtual void Draw(int32) = 0;
};

class FAMainWindow : public FAWindow {
 private:
    int32 m_buttonClickSpotX,m_buttonClickSpotY;
 public:
    FAMainWindow(Display *, int32,GC, Window,int32,int32,int32,int32);
    virtual ~FAMainWindow();
    virtual void DoEvent(XEvent);
    virtual void Draw(int32);
};

class FADumbWindow : public FAWindow {
 private:
 public:
    FADumbWindow(Display *, int32,GC, Window,int32,int32,int32,int32);
    virtual ~FADumbWindow();
    virtual void DoEvent(XEvent);
    virtual void Draw(int32);
};


typedef void (*action_function)(void *);


class FATriStateWindow : public FAWindow {
 private:
    int32 m_state;
    bool m_activated;
    bool m_pressed;
    bool m_insideButton;
    int32 m_partialHeight;
    void *m_cookie;
    action_function m_clickFunction;
 public:
    FATriStateWindow(Display *,int32,GC,Window,int32,int32,int32,int32);
    virtual ~FATriStateWindow();
    void SetPartialHeight(int32);
    void SetActivated();
    void ClearActivated();
    bool IsActivated();
    void SetClickAction(action_function,void *);
    void SetCurrentTime(int32 h, int32 m, int32 s);
    void SetTotalTime(int32 h, int32 m, int32 s);
    virtual void DoEvent(XEvent);
    virtual void Draw(int32);
};

class FALcdWindow : public FAWindow {
 private:
    enum {
	LargeFont = 0,
	SmallFont
    };
    char *m_text;
    int *m_smallFontWidth;
    Pixmap m_smallFontPixmap;
    int *m_largeFontWidth;
    Pixmap m_largeFontPixmap;
    Pixmap m_doubleBufferPixmap;
    int32 m_displayState;
    int32 m_volume;
    Pixmap m_mainTextMask;
    Pixmap m_iconMask;
    Pixmap m_shufflePixmap;
    Pixmap m_repeatPixmap;
    Pixmap m_repeatAllPixmap;
    XRectangle m_shuffleRect;
    XRectangle m_repeatRect;
    XRectangle m_repeatAllRect;
    GC m_timeGC;
    GC m_iconGC;

    bool m_insideDisplay;

    int32 m_totalHours, m_totalMinutes, m_totalSeconds;
    int32 m_currHours, m_currMinutes, m_currSeconds;

    void BlitText(Drawable d, GC gc, int32 x, int32 y, const char *text, int32 font);
    void SetState(int32);
    void DrawIntroState(int32);
    void DrawVolumeState(int32);
    void DrawCurrentTimeState(int32);
    void DrawSeekTimeState(int32);
    void DrawRemainingTimeState(int32);
    void DrawTotalTimeState(int32);
    bool m_iconStates[3];
    void BlitIcons(Drawable);
  public:
    enum {
	SHUFFLE = 0,
	REPEAT,
	REPEAT_ALL,
	MAX
    };
    enum {
	IntroState,
	VolumeState,
	CurrentTimeState,
	SeekTimeState,
	RemainingTimeState,
	TotalTimeState,
	MaxState
    };
    enum {
	FullRedraw,
	TimeOnly,
	IconsOnly
    };
    FALcdWindow(Display *,int32,GC,Window,int32,int32,int32,int32);
    virtual ~FALcdWindow();
    virtual void DoEvent(XEvent);
    void SetShufflePixmap(Pixmap p) { m_shufflePixmap = p; }
    void SetShuffleRect(XRectangle r) { m_shuffleRect = r; }
    void SetRepeatPixmap(Pixmap p) { m_repeatPixmap = p; }
    void SetRepeatRect(XRectangle r) { m_repeatRect = r; }
    void SetRepeatAllPixmap(Pixmap p) { m_repeatAllPixmap = p; }
    void SetRepeatAllRect(XRectangle r) { m_repeatAllRect = r; }
    void SetSmallFontPixmap(Pixmap);
    void SetSmallFontWidth(int *);
    void SetLargeFontPixmap(Pixmap);
    void SetLargeFontWidth(int *);
    void SetMainText(const char *);
    void SetCurrentTime(int32 h, int32 m, int32 s);
    void SetTotalTime(int32 h, int32 m, int32 s);
    void SetIcon(int32);
    void ClearIcon(int32);
    void SetDisplayState(int32);
    void SetVolume(int32);
    int32 GetDisplayState() { return m_displayState; }
    virtual void Draw(int32);
};

typedef void (*dial_motion_function)(void *,int32);

class FADialWindow : public FAWindow {
 private:
    int32 m_buttonClickSpotX, m_buttonClickSpotY,m_prevY;
    int32 m_currentDial;
    dial_motion_function m_func;
    void *m_cookie;
 public:
    FADialWindow(Display *, int32,GC, Window,int32,int32,int32,int32);
    virtual ~FADialWindow();
    void SetMotionFunction(dial_motion_function,void *);
    virtual void DoEvent(XEvent);
    virtual void Draw(int32);

};

#endif // _WindowHash_H_


















