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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <X11/xpm.h>

#include <X11/extensions/shape.h>

#include <linux/soundcard.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <unistd.h>

#include "config.h"
#include "event.h"
#include "freeamp-x11.h"

#include "windowhash.h"
#include "graphics.h"
#include "queue.h"

#define DEFINE_FONT_WIDTHS
#include "font_width.h"
#undef DEFINE_FONT_WIDTHS

extern "C" {

UserInterface *Initialize() {
    return new FreeAmpUI();
}

	   }

FreeAmpUI::FreeAmpUI() {
    int32 foo = 0;
    if (!(foo = XInitThreads())) {
	fprintf(stderr, "FreeAmpUI: XInitThreads returned %d: Unexplained X errors may arise!\n",foo);
    }
    m_windowHash = new WindowHash();
    m_done = false;
    m_initialized = false;
    m_needsWiggling = true;
    m_noStartUp = false;
    m_timerThread = NULL;
    m_seekDelta = 0;
    m_seekSlice = 0;
    m_currSeconds = 0;
    m_totalSeconds = 0;
    m_seekSeconds = 0;
}
FreeAmpUI::~FreeAmpUI() {
    if (m_windowHash) {
	delete m_windowHash;
	m_windowHash = NULL;
    }
    if (m_timerThread) {
	m_timerContinue = false;
	m_timerThread->Join();
	delete m_timerThread;
	m_timerThread = NULL;
    }
    if (!m_noStartUp) {
	XFreeGC(m_display, m_gc);
	XCloseDisplay(m_display);
    }

}

void FreeAmpUI::SetPlayListManager(PlayListManager *plm) {
    m_plm = plm;
}

Error FreeAmpUI::Init(int32 startup_type)
{
    m_startupType = startup_type;
    if (m_noStartUp) {
	return kError_InitFailedSafely;
    }

    XSizeHints *size_hints;
    XIconSize *size_list;
    XWMHints *wm_hints;
    XClassHint *class_hints;
    int32 display_depth;

    if (!(size_hints = XAllocSizeHints())) {
	fprintf(stderr, "%s: failure allocating memory\n", m_argv[0]);
    }
    if (!(wm_hints = XAllocWMHints())) {
	fprintf(stderr, "%s: failure allocating memory\n", m_argv[0]);
    }
    if (!(class_hints = XAllocClassHint())) {
	fprintf(stderr, "%s: failure allocating memory\n", m_argv[0]);
    }
    
    /* connect to X server */
    if ( (m_display=XOpenDisplay(NULL/*display_name*/)) == NULL )
    {
	(void) fprintf( stderr, "%s: cannot connect to X server %s\n", 
			m_argv[0], XDisplayName(NULL/*display_name*/));
	exit( -1 );
    }
    
    int foo,bar;
    if (!XShapeQueryExtension(m_display,&foo,&bar)) {
	fprintf( stderr, "%s: No Shape Extension on server %s\n",m_argv[0], XDisplayName(NULL/*display_name*/));
    }

    /* get screen size from display structure macro */
    m_screenNum = DefaultScreen(m_display);
    display_depth = DefaultDepth(m_display,m_screenNum);


//    cerr << endl<<endl<<"Screen Depth: " << display_depth << endl;
    
//    m_mainWindow = XCreateSimpleWindow(m_display, RootWindow(m_display,m_screenNum), 
//			      0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, 0,
//			      BlackPixel(m_display,m_screenNum), WhitePixel(m_display,m_screenNum));

    m_mainWindow = new FAMainWindow(m_display,m_screenNum,m_gc,RootWindow(m_display,m_screenNum),0,0,TOTAL_WIDTH,TOTAL_HEIGHT);
    XGCValues values;
    int valuemask = 0;
    m_gc = XCreateGC(m_display, m_mainWindow->GetWindow(), valuemask, &values);

    m_mainWindow->SetGC(m_gc);
    m_mainWindow->SetTarget(m_playerEQ);

    m_playButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PLAY_BUTTON_X,PLAY_BUTTON_Y,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT*3);
    m_playButton->SetPartialHeight(PLAY_BUTTON_HEIGHT);

    m_stopButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),STOP_BUTTON_X,STOP_BUTTON_Y,STOP_BUTTON_WIDTH,STOP_BUTTON_HEIGHT*3);
    m_stopButton->SetPartialHeight(STOP_BUTTON_HEIGHT);
    
    m_pauseButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PAUSE_BUTTON_X,PAUSE_BUTTON_Y,PAUSE_BUTTON_WIDTH,PAUSE_BUTTON_HEIGHT*3);
    m_pauseButton->SetPartialHeight(PAUSE_BUTTON_HEIGHT);

    m_prevButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PREV_BUTTON_X,PREV_BUTTON_Y,PREV_BUTTON_WIDTH,PREV_BUTTON_HEIGHT*3);
    m_prevButton->SetPartialHeight(PREV_BUTTON_HEIGHT);

    m_nextButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),NEXT_BUTTON_X,NEXT_BUTTON_Y,NEXT_BUTTON_WIDTH,NEXT_BUTTON_HEIGHT*3);
    m_nextButton->SetPartialHeight(NEXT_BUTTON_HEIGHT);

    m_doubleBufferPixmap = XCreatePixmap(m_display,m_mainWindow->GetWindow(),TOTAL_WIDTH,TOTAL_HEIGHT,display_depth);

    m_switchModeButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),SWITCH_MODE_BUTTON_X,SWITCH_MODE_BUTTON_Y,SWITCH_MODE_BUTTON_WIDTH,SWITCH_MODE_BUTTON_HEIGHT*3);
    m_switchModeButton->SetPartialHeight(SWITCH_MODE_BUTTON_HEIGHT);

    m_minimizeButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),MINIMIZE_BUTTON_X,MINIMIZE_BUTTON_Y,MINIMIZE_BUTTON_WIDTH,MINIMIZE_BUTTON_HEIGHT * 3);
    m_minimizeButton->SetPartialHeight(MINIMIZE_BUTTON_HEIGHT);

    m_closeButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),CLOSE_BUTTON_X,CLOSE_BUTTON_Y,CLOSE_BUTTON_WIDTH,CLOSE_BUTTON_HEIGHT * 3);
    m_closeButton->SetPartialHeight(CLOSE_BUTTON_HEIGHT);

    m_repeatButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),REPEAT_BUTTON_X,REPEAT_BUTTON_Y,REPEAT_BUTTON_WIDTH,REPEAT_BUTTON_HEIGHT * 3);
    m_repeatButton->SetPartialHeight(REPEAT_BUTTON_HEIGHT);

    m_shuffleButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),SHUFFLE_BUTTON_X,SHUFFLE_BUTTON_Y,SHUFFLE_BUTTON_WIDTH,SHUFFLE_BUTTON_HEIGHT * 3);
    m_shuffleButton->SetPartialHeight(SHUFFLE_BUTTON_HEIGHT);

    m_openButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),OPEN_BUTTON_X,OPEN_BUTTON_Y,OPEN_BUTTON_WIDTH,OPEN_BUTTON_HEIGHT * 3);
    m_openButton->SetPartialHeight(OPEN_BUTTON_HEIGHT);


//    m_lcdWindow = new FALcdWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),LCD_X,LCD_Y,LCD_WIDTH,LCD_HEIGHT);
    m_lcdUpperWindow = new FADumbWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),LCD_UPPER_X,LCD_UPPER_Y,LCD_UPPER_WIDTH,LCD_UPPER_HEIGHT);
    m_playListDrawerWindow = new FADumbWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PLAYLIST_DRAWER_X,PLAYLIST_DRAWER_Y,PLAYLIST_DRAWER_WIDTH,PLAYLIST_DRAWER_HEIGHT);
    m_lcdWindow = new FALcdWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),LCD_DISPLAY_X,LCD_DISPLAY_Y,LCD_DISPLAY_WIDTH,LCD_DISPLAY_HEIGHT);

    m_volumeWindow = new FADialWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(), VOLUME_DIAL_X,VOLUME_DIAL_Y,SINGLE_DIAL_WIDTH,DIALS_HEIGHT);
    m_volumeWindow->SetMotionFunction(VolumeDialFunction,this);
    m_seekWindow = new FADialWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(), SEEK_DIAL_X,SEEK_DIAL_Y,SINGLE_DIAL_WIDTH,DIALS_HEIGHT);
    m_seekWindow->SetMotionFunction(SeekDialFunction,this);


    XpmAttributes xpma;
    xpma.closeness = 0xFFFF;
    xpma.valuemask = XpmCloseness;

    if (display_depth >= 15) {
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),logo,&m_iconPixmap,NULL,&xpma);	    
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),leftside,&left_side_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),dials,&dials_pixmap,NULL,&xpma);

	//XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd,&lcd_pixmap,NULL,&xpma);  // just for building total image
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd_upper_frame,&lcd_upper_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd_display,&lcd_display_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),playlist_drawer,&playlist_drawer_pixmap,NULL,&xpma);
	lcd_upper_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)lcd_upper_frame_mask_bits,lcd_upper_frame_mask_width,lcd_upper_frame_mask_height);
	playlist_drawer_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)playlist_drawer_mask_bits,playlist_drawer_mask_width,playlist_drawer_mask_height);
	
	player_full_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),rightside,&right_side_pixmap,NULL,&xpma);
	
	major_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)major_button_mask_bits,MAJOR_BUTTON_MASK_WIDTH,MAJOR_BUTTON_MASK_HEIGHT);
	minor_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)minor_button_mask_bits,MINOR_BUTTON_MASK_WIDTH,MINOR_BUTTON_MASK_HEIGHT);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),play_buttons,&play_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),stop_buttons,&stop_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),pause_buttons,&pause_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),last_buttons,&prev_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),next_buttons,&next_buttons_pixmap,NULL,&xpma);
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),mode_buttons,&mode_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),minimize_buttons,&minimize_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),close_buttons,&close_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),repeat_buttons,&repeat_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),shuffle_buttons,&shuffle_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),open_buttons,&open_buttons_pixmap,NULL,&xpma);
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),small_font,&small_font_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),big_font,&big_font_pixmap,NULL,&xpma);

	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),shuffled_icon,&shuffled_icon_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),repeat_icon,&repeat_icon_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),all_icon,&all_icon_pixmap,NULL,&xpma);
    } else { //if (display_depth >= 8) {
	if (display_depth < 8) {
	    fprintf(stderr, "FreeAmpUI: FreeAmp has only been tested with screen depths of 8 bits and greater\n");
	}
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),logo256,&m_iconPixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),leftside256,&left_side_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),dials256,&dials_pixmap,NULL,&xpma);
	//XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd256,&lcd_pixmap,NULL,&xpma); // just for building total image
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd_upper_frame256,&lcd_upper_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd_display256,&lcd_display_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),playlist_drawer256,&playlist_drawer_pixmap,NULL,&xpma);
	lcd_upper_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)lcd_upper_frame_mask_bits,lcd_upper_frame_mask_width,lcd_upper_frame_mask_height);
	playlist_drawer_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)playlist_drawer_mask_bits,playlist_drawer_mask_width,playlist_drawer_mask_height);

	player_full_mask_pixmap =XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),rightside256,&right_side_pixmap,NULL,&xpma);
	
	major_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)major_button_mask_bits,MAJOR_BUTTON_MASK_WIDTH,MAJOR_BUTTON_MASK_HEIGHT);
	minor_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)minor_button_mask_bits,MINOR_BUTTON_MASK_WIDTH,MINOR_BUTTON_MASK_HEIGHT);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),play_buttons256,&play_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),stop_buttons256,&stop_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),pause_buttons256,&pause_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),last_buttons256,&prev_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),next_buttons256,&next_buttons_pixmap,NULL,&xpma);
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),mode_buttons256,&mode_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),minimize_buttons256,&minimize_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),close_buttons256,&close_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),repeat_buttons256,&repeat_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),shuffle_buttons256,&shuffle_buttons_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),open_buttons256,&open_buttons_pixmap,NULL,&xpma);
	
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),small_font256,&small_font_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),big_font256,&big_font_pixmap,NULL,&xpma);

	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),shuffled_icon256,&shuffled_icon_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),repeat_icon256,&repeat_icon_pixmap,NULL,&xpma);
	XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),all_icon256,&all_icon_pixmap,NULL,&xpma);

    }

    int x=0;
    XCopyArea(m_display,left_side_pixmap,m_doubleBufferPixmap,m_gc,0,0,LEFT_SIDE_WIDTH,LEFT_SIDE_HEIGHT,x,0);
    x+=LEFT_SIDE_WIDTH;
    x+=SINGLE_DIAL_WIDTH;
    x+=LCD_UPPER_WIDTH;
    x+=SINGLE_DIAL_WIDTH;
    XCopyArea(m_display,right_side_pixmap,m_doubleBufferPixmap,m_gc,0,0,RIGHT_SIDE_WIDTH,RIGHT_SIDE_HEIGHT,x,0);
    x+=RIGHT_SIDE_WIDTH;
    m_mainWindow->SetPixmap(m_doubleBufferPixmap);

    m_playButton->SetPixmap(play_buttons_pixmap);
    m_stopButton->SetPixmap(stop_buttons_pixmap);
    m_pauseButton->SetPixmap(pause_buttons_pixmap);
    m_prevButton->SetPixmap(prev_buttons_pixmap);
    m_nextButton->SetPixmap(next_buttons_pixmap);
    m_switchModeButton->SetPixmap(mode_buttons_pixmap);
    m_minimizeButton->SetPixmap(minimize_buttons_pixmap);
    m_closeButton->SetPixmap(close_buttons_pixmap);
    m_repeatButton->SetPixmap(repeat_buttons_pixmap);
    m_shuffleButton->SetPixmap(shuffle_buttons_pixmap);
    m_openButton->SetPixmap(open_buttons_pixmap);

    m_volumeWindow->SetPixmap(dials_pixmap);
    m_seekWindow->SetPixmap(dials_pixmap);

    m_lcdUpperWindow->SetPixmap(lcd_upper_pixmap);
    m_lcdUpperWindow->SetMask(lcd_upper_mask_pixmap);
    m_lcdWindow->SetPixmap(lcd_display_pixmap);
    m_lcdWindow->SetSmallFontPixmap(small_font_pixmap);
    m_lcdWindow->SetSmallFontWidth(smallFontWidth);
    m_lcdWindow->SetLargeFontPixmap(big_font_pixmap);
    m_lcdWindow->SetLargeFontWidth(largeFontWidth);
    XRectangle r;
    m_lcdWindow->SetShufflePixmap(shuffled_icon_pixmap);
    r.x = SHUFFLED_ICON_X; r.y = SHUFFLED_ICON_Y; r.width = SHUFFLED_ICON_WIDTH; r.height = SHUFFLED_ICON_HEIGHT;
    m_lcdWindow->SetShuffleRect(r);
    m_lcdWindow->SetRepeatPixmap(repeat_icon_pixmap);
    r.x = REPEAT_ICON_X; r.y = REPEAT_ICON_Y; r.width = REPEAT_ICON_WIDTH; r.height = REPEAT_ICON_HEIGHT;
    m_lcdWindow->SetRepeatRect(r);
    m_lcdWindow->SetRepeatAllPixmap(all_icon_pixmap);
    r.x = REPEAT_ALL_ICON_X; r.y = REPEAT_ALL_ICON_Y; r.width = REPEAT_ALL_ICON_WIDTH; r.height = REPEAT_ALL_ICON_HEIGHT;
    m_lcdWindow->SetRepeatAllRect(r);

    m_lcdWindow->SetCurrentTime(0,0,0);
    m_lcdWindow->SetSeekTime(0,0,0);
    m_lcdWindow->SetTotalTime(0,0,0);

    m_playListDrawerWindow->SetPixmap(playlist_drawer_pixmap);
    m_playListDrawerWindow->SetMask(playlist_drawer_mask_pixmap);

    size_hints->flags = PPosition | PSize | PMinSize;
    size_hints->min_width = TOTAL_WIDTH;
    size_hints->min_height = TOTAL_HEIGHT;
    
    /* These calls store window_name and icon_name into
     * XTextProperty structures and set their other 
     * fields properly. */

    char *window_name = "FreeAmp v1.1.0";
    char *icon_name = "FreeAmp";
    XTextProperty windowName, iconName;

    if (XStringListToTextProperty(&window_name, 1, &windowName) == 0) {
	(void) fprintf( stderr, "%s: structure allocation for windowName failed.\n", 
			m_argv[0]);
	exit(-1);
    }
    
    if (XStringListToTextProperty(&icon_name, 1, &iconName) == 0) {
	(void) fprintf( stderr, "%s: structure allocation for iconName failed.\n", 
			m_argv[0]);
	exit(-1);
    }
    
    wm_hints->initial_state = NormalState;
    wm_hints->input = True;
    wm_hints->icon_pixmap = m_iconPixmap;
    wm_hints->flags = StateHint | IconPixmapHint | InputHint;
    
    class_hints->res_name = m_argv[0];
    class_hints->res_class = "Basicwin";
    
    XSetWMProperties(m_display, m_mainWindow->GetWindow(), &windowName, &iconName, 
		     m_argv, m_argc, size_hints, wm_hints, 
		     class_hints);
    
    /* Select event types wanted */
    int32 tmpMask = ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask;

    m_mainWindow->SelectInput(ExposureMask | KeyPressMask | ButtonReleaseMask | KeyReleaseMask | PointerMotionMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);
    
    m_playButton->SelectInput(tmpMask);
    m_stopButton->SelectInput(tmpMask);
    m_pauseButton->SelectInput(tmpMask);
    m_prevButton->SelectInput(tmpMask);
    m_nextButton->SelectInput(tmpMask);
    m_switchModeButton->SelectInput(tmpMask);
    m_minimizeButton->SelectInput(tmpMask);
    m_closeButton->SelectInput(tmpMask);
    m_repeatButton->SelectInput(tmpMask);
    m_shuffleButton->SelectInput(tmpMask);
    m_openButton->SelectInput(tmpMask);

    m_volumeWindow->SelectInput(tmpMask | PointerMotionMask);
    m_seekWindow->SelectInput(tmpMask | PointerMotionMask);

    m_lcdWindow->SelectInput(tmpMask);

    m_lcdUpperWindow->SelectInput(ExposureMask);
    m_playListDrawerWindow->SelectInput(ExposureMask);

    // set up shape
    XSetForeground(m_display,m_gc,BlackPixel(m_display,m_screenNum));
    XFillRectangle(m_display,m_mainWindow->GetWindow(),m_gc,0,0,TOTAL_WIDTH,TOTAL_HEIGHT);
    m_mainWindow->SetMask(player_full_mask_pixmap);

    m_playButton->SetMask(major_button_mask_pixmap);
    m_stopButton->SetMask(major_button_mask_pixmap);
    m_pauseButton->SetMask(major_button_mask_pixmap);
    m_prevButton->SetMask(major_button_mask_pixmap);
    m_nextButton->SetMask(major_button_mask_pixmap);
    m_switchModeButton->SetMask(minor_button_mask_pixmap);
    m_minimizeButton->SetMask(minor_button_mask_pixmap);
    m_closeButton->SetMask(minor_button_mask_pixmap);
    m_repeatButton->SetMask(minor_button_mask_pixmap);
    m_shuffleButton->SetMask(minor_button_mask_pixmap);
    m_openButton->SetMask(minor_button_mask_pixmap);

    m_playButton->SetClickAction(playFunction,this);
    m_stopButton->SetClickAction(stopFunction,this);
    m_pauseButton->SetClickAction(pauseFunction,this);
    m_prevButton->SetClickAction(prevFunction,this);
    m_nextButton->SetClickAction(nextFunction,this);
    m_switchModeButton->SetClickAction(switchModeFunction,this);
    m_minimizeButton->SetClickAction(minimizeFunction,this);
    m_closeButton->SetClickAction(closeFunction,this);
    m_repeatButton->SetClickAction(repeatFunction,this);
    m_shuffleButton->SetClickAction(shuffleFunction,this);
    m_openButton->SetClickAction(openFunction,this);

    m_windowHash->Insert(m_mainWindow->GetWindow(),(FAWindow *)m_mainWindow);
    m_windowHash->Insert(m_playButton->GetWindow(),(FAWindow *)m_playButton);
    m_windowHash->Insert(m_stopButton->GetWindow(),(FAWindow *)m_stopButton);
    m_windowHash->Insert(m_pauseButton->GetWindow(),(FAWindow *)m_pauseButton);
    m_windowHash->Insert(m_prevButton->GetWindow(),(FAWindow *)m_prevButton);
    m_windowHash->Insert(m_nextButton->GetWindow(),(FAWindow *)m_nextButton);
    m_windowHash->Insert(m_switchModeButton->GetWindow(),(FAWindow *)m_switchModeButton);
    m_windowHash->Insert(m_minimizeButton->GetWindow(),(FAWindow *)m_minimizeButton);
    m_windowHash->Insert(m_closeButton->GetWindow(),(FAWindow *)m_closeButton);
    m_windowHash->Insert(m_repeatButton->GetWindow(),(FAWindow *)m_repeatButton);
    m_windowHash->Insert(m_shuffleButton->GetWindow(),(FAWindow *)m_shuffleButton);
    m_windowHash->Insert(m_openButton->GetWindow(),(FAWindow *)m_openButton);
    m_windowHash->Insert(m_lcdUpperWindow->GetWindow(),(FAWindow *)m_lcdUpperWindow);
    m_windowHash->Insert(m_lcdWindow->GetWindow(),(FAWindow *)m_lcdWindow);
    m_windowHash->Insert(m_playListDrawerWindow->GetWindow(),(FAWindow *)m_playListDrawerWindow);
    m_windowHash->Insert(m_volumeWindow->GetWindow(),(FAWindow *)m_volumeWindow);
    m_windowHash->Insert(m_seekWindow->GetWindow(),(FAWindow *)m_seekWindow);

    m_initialized = true;




    gtkListenThread = Thread::CreateThread();
    gtkListenThread->Create(FreeAmpUI::x11ServiceFunction,this);

    m_timerContinue = true;
    m_timerThread = Thread::CreateThread();
    m_timerThread->Create(FreeAmpUI::TimerEventFunction,this);
    
    return kError_NoErr;

}

void FreeAmpUI::ParseArgs() {
    char *arg = NULL;
    bool shuffle = false;
    bool autoplay = false;

    for(int32 i = 1;i < m_argc; i++) {
	arg = m_argv[i];
	
	if (arg[0] == '-') {
	    switch (arg[1]) {
		case 's':
                    shuffle = true;
		    break;
                case 'p':
                    autoplay = true;
		    break;
            }
        } else {
	    if (m_startupType == PRIMARY_UI) {
		m_plm->AddItem(arg,0);
	    }
	}
    }
    if (m_startupType == PRIMARY_UI) {
	m_plm->SetFirst();
    }
    if (m_startupType == PRIMARY_UI) {
	if(shuffle) 
	    m_plm->SetShuffle(SHUFFLE_RANDOM);
	
	if(autoplay)
	    m_playerEQ->AcceptEvent(new Event(CMD_Play));
    }
}


void FreeAmpUI::x11ServiceFunction(void *p) {
    ((FreeAmpUI *)p)->X11EventService();
}

void FreeAmpUI::X11EventService() {
    XEvent report;

    /* Display window */
    m_mainWindow->MapWindow();
    m_playButton->MapWindow();
    m_pauseButton->MapWindow();
    m_nextButton->MapWindow();
    m_prevButton->MapWindow();
    m_switchModeButton->MapWindow();
    m_minimizeButton->MapWindow();
    m_closeButton->MapWindow();
    m_repeatButton->MapWindow();
    m_shuffleButton->MapWindow();
    m_openButton->MapWindow();
    m_lcdUpperWindow->MapWindow();
    m_lcdWindow->MapWindow();
    m_playListDrawerWindow->MapWindow();
    m_volumeWindow->MapWindow();
    m_seekWindow->MapWindow();


    ParseArgs();

//    fprintf(stderr,"Main window ID: %x\n",m_mainWindow->GetWindow());
//    fprintf(stderr,"Play Button ID: %x\n",m_playButton->GetWindow());
//    fprintf(stderr,"PauseButton ID: %x\n",m_pauseButton->GetWindow());
//    fprintf(stderr,"Prev Button ID: %x\n",m_prevButton->GetWindow());
//    fprintf(stderr,"Next Button ID: %x\n",m_nextButton->GetWindow());
//    fprintf(stderr,"MinimizeButtonID: %x\n",m_minimizeButton->GetWindow());

    /* get events, use first to display text and graphics */
    while (!m_done)  {
	XNextEvent(m_display, &report);
        FAWindow *w = m_windowHash->Value(report.xany.window);
	
	if (w) {
	    //fprintf(stderr, "handling by class\n");
	    XLockDisplay(m_display);
	    w->DoEvent(report);
	    XUnlockDisplay(m_display);
	}
    } /* end while */
    if (m_startupType == PRIMARY_UI) {
	m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
    }
}

int32 FreeAmpUI::AcceptEvent(Event *e) {
    //cout << "Got Event " << e->Type() << endl;
    switch (e->Type()) {
	case CMD_Cleanup: {
	    m_done = true;
	    XEvent ev;
	    ev.xclient.format = 32;
	    XSendEvent(m_display,InputFocus,true,0,&ev);
	    gtkListenThread->Join();
	    m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));
	    break; }
	case INFO_Playing:
	    m_stopButton->MapWindow();
	    m_playButton->UnMapWindow();
	    m_pauseButton->ClearActivated();
	    XLockDisplay(m_display);
	    m_pauseButton->Draw();
	    XUnlockDisplay(m_display);
	    break;
	case INFO_Paused:
	    m_pauseButton->SetActivated();
	    m_stopButton->MapWindow();
	    m_playButton->UnMapWindow();
	    XLockDisplay(m_display);
	    m_pauseButton->Draw();
	    XUnlockDisplay(m_display);
	    break;
	case INFO_Stopped:
	    m_lcdWindow->SetCurrentTime(0,0,0);
	    m_currSeconds = 0;
	    m_pauseButton->ClearActivated();
	    XLockDisplay(m_display);
	    m_pauseButton->Draw();
	    m_lcdWindow->Draw(FALcdWindow::TimeOnly);
	    XUnlockDisplay(m_display);
	    m_playButton->MapWindow();
	    m_stopButton->UnMapWindow();
	    break;
	case INFO_MediaInfo: {
	    MediaInfoEvent *info = (MediaInfoEvent *)e;
	    char *pFoo = strrchr(info->m_filename,'/');
	    pFoo = (pFoo ? ++pFoo : info->m_filename);
	    m_lcdWindow->SetMainText(pFoo);
	    m_needsWiggling = true;
	    m_totalSeconds = (int32)info->m_totalSeconds;
	    int32 s = (int32)info->m_totalSeconds;
	    int32 h = s / 3600;
	    int32 m = (s % 3600) / 60;
	    s = ((s % 3600) % 60);
	    m_lcdWindow->SetTotalTime(h,m,s);
	    m_lcdWindow->SetCurrentTime(0,0,0);
	    XLockDisplay(m_display);
	    m_lcdWindow->Draw(FALcdWindow::FullRedraw);
	    //cout << "Full redraw..." << endl;
	    XUnlockDisplay(m_display);
	    break;
	}
	case INFO_MediaTimeInfo: {
	    MediaTimeInfoEvent *info = (MediaTimeInfoEvent *)e;
	    m_lcdWindow->SetCurrentTime(info->m_hours,info->m_minutes,info->m_seconds);
	    m_currSeconds = (info->m_hours * 3600) + (info->m_minutes * 60) + info->m_seconds;
	    XLockDisplay(m_display);
	    m_lcdWindow->Draw(FALcdWindow::TimeOnly);
	    XUnlockDisplay(m_display);
	    break;
	}
	case INFO_MPEGInfo: {
	    MpegInfoEvent *info = (MpegInfoEvent *)e;
	    m_secondsPerFrame = info->GetSecondsPerFrame();
	    break;
	}
	case INFO_ID3TagInfo: {
	    ID3TagEvent *info = (ID3TagEvent *)e;
	    Id3TagInfo p = info->GetId3Tag();
	    char *pEnd = &((p.m_songName)[strlen(p.m_songName)]);
	    while ( (pEnd != p.m_songName) && (*(pEnd - sizeof(char *)) == ' ')) pEnd--;
	    char end = *pEnd;
	    *pEnd = '\0';
	    m_lcdWindow->SetMainText(p.m_songName);
	    *pEnd = end;
	    XLockDisplay(m_display);
	    m_lcdWindow->Draw(FALcdWindow::FullRedraw);
	    XUnlockDisplay(m_display);
	    break;
	}
	case INFO_PlayListRepeat: {
	    PlayListRepeatEvent *plre = (PlayListRepeatEvent *)e;
	    switch (plre->GetRepeatMode()) 
	    {
		case REPEAT_CURRENT:
		    m_lcdWindow->SetIcon(FALcdWindow::REPEAT);
		    m_lcdWindow->ClearIcon(FALcdWindow::REPEAT_ALL);
		    break;
		case REPEAT_ALL:
		    m_lcdWindow->SetIcon(FALcdWindow::REPEAT);
		    m_lcdWindow->SetIcon(FALcdWindow::REPEAT_ALL);
		    break;
		case REPEAT_NOT:
		default:
		    m_lcdWindow->ClearIcon(FALcdWindow::REPEAT);
		    m_lcdWindow->ClearIcon(FALcdWindow::REPEAT_ALL);
		    break;
	    }
	    XLockDisplay(m_display);
	    m_lcdWindow->Draw(FALcdWindow::IconsOnly);
	    XUnlockDisplay(m_display);
	    break;
	}
	case INFO_PlayListShuffle:
	{
	    PlayListShuffleEvent *plse = (PlayListShuffleEvent *)e;
	    
	    switch (plse->GetShuffleMode()) 
	    {
		case SHUFFLE_NOT_SHUFFLED:
		    m_lcdWindow->ClearIcon(FALcdWindow::SHUFFLE);
		    break;
		case SHUFFLE_RANDOM:
		    m_lcdWindow->SetIcon(FALcdWindow::SHUFFLE);
		    break;
		default:
		    m_lcdWindow->ClearIcon(FALcdWindow::SHUFFLE);
	    };
	    XLockDisplay(m_display);
	    m_lcdWindow->Draw(FALcdWindow::IconsOnly);
	    XUnlockDisplay(m_display);
	    break;
	}
	default:
	    break;
    }
    return 0;
}

void FreeAmpUI::SetArgs(int argc, char **argv) {
    m_argc = argc;
    m_argv = argv;
    // delay arg parsing till later, except for looking for -h and --help
    // if found, set m_noStartUp to true and send the QuitPlayer message...
    for (int i = 0;i < m_argc;i++) {
	if (m_argv[i][0] == '-') {
	    if ((m_argv[i][1] == 'h') ||
		(m_argv[i][1] == 'H') ||
		!strcmp(&(m_argv[i][1]),"-help")) {
		m_noStartUp = true;
		Usage();
		m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
	    }
	    
	}
    }

}

void FreeAmpUI::Usage() {
    cout << "FreeAmp X11 UI PlugIn usage:" << endl;
    cout << "   [any global FreeAmp option] <file1> [file2] ..." << endl;
    cout << endl;
}


void FreeAmpUI::playFunction(void *p) {
    //cerr << "play" << endl;
    //((FreeAmpUI *)p)->m_stopButton->MapWindow();
    //((FreeAmpUI *)p)->m_playButton->UnMapWindow();
    ((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_Play));
    ((FreeAmpUI *)p)->m_lcdWindow->SetDisplayState(FALcdWindow::CurrentTimeState);
}

void FreeAmpUI::stopFunction(void *p) {
    //cerr << "stop" << endl;
    //((FreeAmpUI *)p)->m_playButton->MapWindow();
    //((FreeAmpUI *)p)->m_stopButton->UnMapWindow();
    ((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_Stop));
}

void FreeAmpUI::pauseFunction(void *p) {
    //cerr << "pause" << endl;
    if (((FreeAmpUI *)p)->m_playButton->IsMapped()) {
	((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_PlayPaused));
	((FreeAmpUI *)p)->m_lcdWindow->SetDisplayState(FALcdWindow::CurrentTimeState);
    } else {
	((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_TogglePause));
    }
}

void FreeAmpUI::prevFunction(void *p) {
    //cerr << "prev" << endl;
    if ( ((FreeAmpUI *)p)->m_currSeconds < 2) {
	((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_PrevMediaPiece));
    } else {
	((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_Play));
    }
}

void FreeAmpUI::nextFunction(void *p) {
    //cerr << "next" << endl;
    ((FreeAmpUI *)p)->m_playerEQ->AcceptEvent(new Event(CMD_NextMediaPiece));
}

void FreeAmpUI::switchModeFunction(void *p) {
    //cerr << "switch mode" << endl;
}

void FreeAmpUI::minimizeFunction(void *p) {
    //cerr << "minimize" << endl;
    XIconifyWindow(((FreeAmpUI *)p)->m_display,
		   ((FreeAmpUI *)p)->m_mainWindow->GetWindow(),
		   ((FreeAmpUI *)p)->m_screenNum);
}

void FreeAmpUI::closeFunction(void *p) {
    //cerr << "close" << endl;
    ((FreeAmpUI *)p)->m_done = true;
}

void FreeAmpUI::repeatFunction(void *p) {
    //cerr << "repeat" << endl;
    ((FreeAmpUI *)p)->m_plm->ToggleRepeat();
}

void FreeAmpUI::shuffleFunction(void *p) {
    //cerr << "shuffle" << endl;
    ((FreeAmpUI *)p)->m_plm->ToggleShuffle();
}

void FreeAmpUI::openFunction(void *p) {
    //cerr << "open" << endl;
}


#define VOL_MAX 100
// 0 = off
// 1 = on
// 2 = up
// 3 = down
// 5 = no change
void FreeAmpUI::VolumeDialFunction(void *p, int32 c,int32 x, int32 y) {
    //cout << "volume y: " << c << endl;
    switch (c) {
	case 0:
	    ((FreeAmpUI *)p)->m_lcdWindow->SetDisplayState( ((FreeAmpUI *)p)->m_oldLcdState);
	    if ( ((FreeAmpUI *)p)->m_mixerFd != -1 )
		close( ((FreeAmpUI *)p)->m_mixerFd);
	    break;
	case 1:
	    ((FreeAmpUI *)p)->m_oldLcdState = ((FreeAmpUI *)p)->m_lcdWindow->GetDisplayState();

	    ((FreeAmpUI *)p)->m_mixerFd = open("/dev/mixer",O_RDWR);
	    ioctl( ((FreeAmpUI *)p)->m_mixerFd, SOUND_MIXER_READ_VOLUME, &( ((FreeAmpUI *)p)->m_volume ));
	    ((FreeAmpUI *)p)->m_volume &= 0xFF;
	    ((FreeAmpUI *)p)->m_lcdWindow->SetVolume( ((FreeAmpUI *)p)->m_volume );
    
	    ((FreeAmpUI *)p)->m_lcdWindow->SetDisplayState(FALcdWindow::VolumeState);
    
	    break;
	case 2: {
	    int foo = ((FreeAmpUI *)p)->m_volume;
	    foo++;
	    if ( foo > VOL_MAX) {
		foo = VOL_MAX;
	    }
	    ((FreeAmpUI *)p)->m_lcdWindow->SetVolume( (foo*100)/VOL_MAX );
	    ((FreeAmpUI *)p)->m_lcdWindow->Draw(FALcdWindow::TimeOnly);
	    ((FreeAmpUI *)p)->m_volume = foo;
	    
	    foo |= (foo << 8);

	    ioctl( ((FreeAmpUI *)p)->m_mixerFd, SOUND_MIXER_WRITE_VOLUME, &foo);

	    break; }
	case 3: {
	    int foo = ((FreeAmpUI *)p)->m_volume;
	    foo--;
	    if ( foo < 0) {
		foo= 0;
	    }
	    ((FreeAmpUI *)p)->m_lcdWindow->SetVolume( (foo*100)/VOL_MAX );
	    ((FreeAmpUI *)p)->m_lcdWindow->Draw(FALcdWindow::TimeOnly);
	    ((FreeAmpUI *)p)->m_volume = foo;

	    foo |= (foo << 8);

	    ioctl( ((FreeAmpUI *)p)->m_mixerFd, SOUND_MIXER_WRITE_VOLUME, &foo);
	    
	    break; }
    }
}

#define PIXELS_PER_DELTA 8
#define SEEK_SLICE 6
void FreeAmpUI::SeekDialFunction(void *p, int32 c,int32 x,int32 y) {
    FreeAmpUI *pMe = (FreeAmpUI *)p;
    //cout << "seek c: " << c << endl;
    switch (c) {
	case 0: {
	    pMe->m_seekDelta = 0;
	    pMe->m_currSeconds = pMe->m_seekSeconds;
	    int32 s = pMe->m_seekSeconds;
	    int32 h = s / 3600;
	    int32 m = (s % 3600) / 60;
	    s = ((s % 3600) % 60);
	    pMe->m_lcdWindow->SetCurrentTime(h,m,s);
	    pMe->m_lcdWindow->SetDisplayState( pMe->m_oldLcdState);
	    pMe->m_playerEQ->AcceptEvent(new ChangePositionEvent( pMe->m_seekSeconds / pMe->m_secondsPerFrame ));
	    //cout << "Changed position to" << pMe->m_seekSeconds / pMe->m_secondsPerFrame << endl;
	    break; }
	case 1: {
	    pMe->m_seekDelta = 0;
	    pMe->m_oldLcdState = pMe->m_lcdWindow->GetDisplayState();
	    pMe->m_seekSeconds = pMe->m_currSeconds;
	    int32 s = pMe->m_seekSeconds;
	    int32 h = s / 3600;
	    int32 m = (s % 3600) / 60;
	    s = ((s % 3600) % 60);
	    pMe->m_lcdWindow->SetSeekTime(h,m,s);
	    pMe->m_lcdWindow->SetDisplayState(FALcdWindow::SeekTimeState);
	    break; }
	case 2: {
	    pMe->m_seekDelta = (y * -1) / PIXELS_PER_DELTA;
#if 0
	    pMe->m_seekSeconds++;
	    if (pMe->m_seekSeconds > pMe->m_totalSeconds) {
		pMe->m_seekSeconds = pMe->m_totalSeconds;
	    }
	    int32 s = pMe->m_seekSeconds;
	    int32 h = s / 3600;
	    int32 m = (s % 3600) / 60;
	    s = ((s % 3600) % 60);
	    pMe->m_lcdWindow->SetSeekTime(h,m,s);
	    pMe->m_lcdWindow->Draw(FALcdWindow::TimeOnly);
#endif
	    break; }
	case 3: {
	    pMe->m_seekDelta = (y * -1) / PIXELS_PER_DELTA;
#if 0
	    pMe->m_seekSeconds--;
	    if (pMe->m_seekSeconds < 0) {
		pMe->m_seekSeconds = 0;
	    }
	    int32 s = pMe->m_seekSeconds;
	    int32 h = s / 3600;
	    int32 m = (s % 3600) / 60;
	    s = ((s % 3600) % 60);
	    pMe->m_lcdWindow->SetSeekTime(h,m,s);
	    pMe->m_lcdWindow->Draw(FALcdWindow::TimeOnly);
#endif
	    break; }
    }
}

void FreeAmpUI::SeekJogAction() {
    //cout << "seek delta: " << m_seekDelta << endl;
    m_seekSeconds += m_seekDelta;
    if (m_seekSeconds < 0) m_seekSeconds = 0;
    if (m_seekSeconds > m_totalSeconds) m_seekSeconds = m_totalSeconds;
    int32 s = m_seekSeconds;
    int32 h = s / 3600;
    int32 m = (s % 3600) / 60;
    s = ((s % 3600) % 60);
    m_lcdWindow->SetSeekTime(h,m,s);
    if (m_seekDelta) {
	XLockDisplay(m_display);
	m_lcdWindow->Draw(FALcdWindow::TimeOnly);
	XUnlockDisplay(m_display);
    }
}

void FreeAmpUI::TimerEventFunction(void *p) {
    FreeAmpUI *pMe = (FreeAmpUI *)p;

    while (pMe->m_timerContinue) {
	if (pMe->m_needsWiggling) {
	    XLockDisplay(pMe->m_display);
	    pMe->m_needsWiggling = pMe->m_lcdWindow->WiggleMainText();
	    XUnlockDisplay(pMe->m_display);
	}
	if (pMe->m_seekSlice) {
	    pMe->SeekJogAction();
	}
	pMe->m_seekSlice = ++(pMe->m_seekSlice) % SEEK_SLICE;
	usleep(170000);
    }
}










