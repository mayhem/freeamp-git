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

#ifndef _FREEAMP_UI_H_
#define _FREEAMP_UI_H_

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
#include "playlist.h"
#include "control.h"
#include "controlinfo.h"
#include "view.h"
#include "dib.h"
#include "linkedlist.h"

#include "bitmapview.h"
#include "buttonview.h"
#include "dialview.h"
#include "statusview.h"
#include "textview.h"
#include "timeview.h"
#include "volumeview.h"
#include "listview.h"
#include "scrollview.h"

enum { STATE_Stopped = 1, STATE_Playing, STATE_Paused };


class FreeAmpUI : public UserInterface {
 public:
    FreeAmpUI();
    ~FreeAmpUI();

    virtual void Init() {}
    virtual void SetTarget(EventQueue* eq){m_target = eq;}
    virtual int32 AcceptEvent(Event*);
    virtual void SetArgs(int32,char**);
	virtual void SetPlayListManager(PlayListManager*);

    void SetWindowHandle(HWND hwnd){m_hwnd = hwnd;}
    HCURSOR SetCursor(HCURSOR cursor)
    {HCURSOR result = m_cursor; m_cursor = cursor; return result;}

    HCURSOR Cursor(void) const { return m_cursor;}

    /* handle window messages */
    void Create();
    void Destroy();
    int32 Paint();
    int32 HitTest(int32 xPos, int32 yPos);
    bool MouseMove(int32 xPos, int32 yPos, int32 modifiers);
    bool LeftButtonDown(int32 xPos, int32 yPos, int32 modifiers);
    bool LeftButtonUp(int32 xPos, int32 yPos, int32 modifiers);
    void Command(int32 command, View* source);
    void Notify(int32 command, LPNMHDR notifyMsgHdr);


 protected:
    static void ui_thread_function(void*);
    void UIThreadFunction();

    void LoadBitmaps();
    void DeleteBitmaps();
    void InitializeRegions();
    void DeleteRegions();

    void CreateControls();

 public:
    Semaphore*          m_uiSemaphore;

    bool                m_scrolling;

    EventQueue*         m_target;
	int32			    m_state;
	PlayListManager*    m_plm;
    float			    m_secondsPerFrame;

 private:

    Thread*             m_uiThread;
    HWND                m_hwnd;
    HCURSOR             m_cursor;

    LinkedList<View*>*  m_viewList;

    HRGN                m_windowRegion;
    HRGN                m_playerRegion;
    HRGN                m_playListRegion;
    HRGN                m_playlistBodyRegion;

    HRGN                m_controlRegions[kNumControls];

    BITMAPINFO          m_bufferBitmapInfo;
    void*               m_bufferBits;

    DIB*                m_playerCanvas;
    DIB*                m_bodyBitmap;
    DIB*                m_playlistBodyBitmap;
    DIB*                m_playBitmap;
    DIB*                m_stopBitmap;
    DIB*                m_pauseBitmap;
    DIB*                m_nextBitmap;
    DIB*                m_lastBitmap;
    DIB*                m_modeBitmap;
    DIB*                m_minimizeBitmap;
    DIB*                m_closeBitmap;
    DIB*                m_repeatBitmap;
    DIB*                m_shuffleBitmap;
    DIB*                m_openBitmap;
    DIB*                m_dialBitmap;
    DIB*                m_shuffleIconBitmap;
    DIB*                m_repeatIconBitmap;
    DIB*                m_repeatAllIconBitmap;

    DIB*                m_smallFontBitmap;
    DIB*                m_largeFontBitmap;
    DIB*                m_timeBackgroundBitmap;

    DIB*                m_panelBackingBitmap;
    DIB*                m_panelBackingMaskBitmap;

    DIB*                m_drawerBitmap;
    DIB*                m_drawerMaskBitmap;

    DIB*                m_scrollbarBitmap;


	int32			    m_width;
	int32			    m_height;

    View*               m_captureView;

    BitmapView*         m_backgroundView;
    BitmapView*         m_playlistBackView;

    ButtonView*         m_playView;
    ButtonView*         m_stopView;
    ButtonView*         m_pauseView;
    ButtonView*         m_nextView;
    ButtonView*         m_lastView;

    ButtonView*         m_modeView;
    ButtonView*         m_minimizeView;
    ButtonView*         m_closeView;
    ButtonView*         m_repeatView;
    ButtonView*         m_shuffleView;
    ButtonView*         m_openView;

    DialView*           m_volumeView;
    DialView*           m_seekView;

    StatusView*         m_shuffleIconView;
    StatusView*         m_repeatIconView;
    StatusView*         m_repeatAllIconView;

    TextView*           m_songTitleView;

    TimeView*           m_timeView;
    VolumeView*         m_volumeInfoView;

    BitmapView*         m_drawerView;
    BitmapView*         m_panelBackingView;

    ListView*           m_playlistView;

    ScrollView*         m_scrollbarView;


};


#endif /* _FREEAMP_UI_H_ */
