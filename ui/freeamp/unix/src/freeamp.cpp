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


#include <stdio.h>

#include "config.h"
#include "event.h"
#include "freeamp-x11.h"

#include "windowhash.h"
#include "graphics.h"

void FreeAmpUI::SetArgs(int argc, char **argv) {
    m_argc = argc;
    m_argv = argv;
}
extern "C" {

UserInterface *Initialize() {
    return new FreeAmpUI();
}

	   }

FreeAmpUI::FreeAmpUI() {
    m_windowHash = new WindowHash();
}
FreeAmpUI::~FreeAmpUI() {
    if (m_windowHash) {
	delete m_windowHash;
	m_windowHash = NULL;
    }
    XFreeGC(m_display, m_gc);
    XCloseDisplay(m_display);

}
int32 FreeAmpUI::AcceptEvent(Event *e) {
    switch (e->Type()) {
	case CMD_Cleanup:
	    m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));
	    break;
	default:
	    break;
    }
    return 0;
}
void FreeAmpUI::SetPlayListManager(PlayListManager *plm) {
    m_plm = plm;
}

void FreeAmpUI::Init()
{
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
    cerr << endl<<endl<<"Screen Depth: " << display_depth << endl;
    
//    m_mainWindow = XCreateSimpleWindow(m_display, RootWindow(m_display,m_screenNum), 
//			      0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, 0,
//			      BlackPixel(m_display,m_screenNum), WhitePixel(m_display,m_screenNum));

    m_mainWindow = new FAMainWindow(m_display,m_screenNum,m_gc,RootWindow(m_display,m_screenNum),0,0,TOTAL_WIDTH,TOTAL_HEIGHT);
    XGCValues values;
    int valuemask = 0;
    m_gc = XCreateGC(m_display, m_mainWindow->GetWindow(), valuemask, &values);

    m_mainWindow->SetGC(m_gc);

    m_playButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PLAY_BUTTON_X,PLAY_BUTTON_Y,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT*3);
    m_playButton->SetPartialHeight(PLAY_BUTTON_HEIGHT);
    
    m_pauseButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PAUSE_BUTTON_X,PAUSE_BUTTON_Y,PAUSE_BUTTON_WIDTH,PAUSE_BUTTON_HEIGHT*3);
    m_pauseButton->SetPartialHeight(PAUSE_BUTTON_HEIGHT);

    m_prevButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),PREV_BUTTON_X,PREV_BUTTON_Y,PREV_BUTTON_WIDTH,PREV_BUTTON_HEIGHT*3);
    m_prevButton->SetPartialHeight(PREV_BUTTON_HEIGHT);

    m_nextButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),NEXT_BUTTON_X,NEXT_BUTTON_Y,NEXT_BUTTON_WIDTH,NEXT_BUTTON_HEIGHT*3);
    m_nextButton->SetPartialHeight(NEXT_BUTTON_HEIGHT);

    m_doubleBufferPixmap = XCreatePixmap(m_display,m_mainWindow->GetWindow(),TOTAL_WIDTH,TOTAL_HEIGHT,display_depth);

    m_switchModeButton = new FATriStateWindow(m_display,m_screenNum,m_gc,m_mainWindow->GetWindow(),SWITCH_MODE_BUTTON_X,SWITCH_MODE_BUTTON_Y,SWITCH_MODE_BUTTON_WIDTH,SWITCH_MODE_BUTTON_HEIGHT*3);
    m_switchModeButton->SetPartialHeight(SWITCH_MODE_BUTTON_HEIGHT);

    switch (display_depth) {
	case 16:
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),logo,&m_iconPixmap,NULL,NULL);	    
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),leftside,&left_side_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),dials,&dials_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd,&lcd_pixmap,NULL,NULL);
	    
	    player_full_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);

	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),rightside,&right_side_pixmap,NULL,NULL);

	    major_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)major_button_mask_bits,MAJOR_BUTTON_MASK_WIDTH,MAJOR_BUTTON_MASK_HEIGHT);
	    minor_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)minor_button_mask_bits,MINOR_BUTTON_MASK_WIDTH,MINOR_BUTTON_MASK_HEIGHT);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),play_buttons,&play_buttons_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),pause_buttons,&pause_buttons_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),last_buttons,&prev_buttons_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),next_buttons,&next_buttons_pixmap,NULL,NULL);

	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),mode_buttons,&mode_buttons_pixmap,NULL,NULL);
	    break;
	case 8:
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),logo256,&m_iconPixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),leftside256,&left_side_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),dials256,&dials_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),lcd256,&lcd_pixmap,NULL,NULL);
	    
	    player_full_mask_pixmap =XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);

	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),rightside256,&right_side_pixmap,NULL,NULL);

	    major_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)major_button_mask_bits,MAJOR_BUTTON_MASK_WIDTH,MAJOR_BUTTON_MASK_HEIGHT);
	    minor_button_mask_pixmap = XCreateBitmapFromData(m_display,m_mainWindow->GetWindow(),(char *)minor_button_mask_bits,MINOR_BUTTON_MASK_WIDTH,MINOR_BUTTON_MASK_HEIGHT);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),play_buttons256,&play_buttons_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),pause_buttons256,&pause_buttons_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),last_buttons256,&prev_buttons_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),next_buttons256,&next_buttons_pixmap,NULL,NULL);

	    XpmCreatePixmapFromData(m_display,m_mainWindow->GetWindow(),mode_buttons256,&mode_buttons_pixmap,NULL,NULL);
	    break;
	default:
	    cerr << "Only know how to deal with bit depths of 8 or 16 (256 or 65535 colors)!!!" << endl;
	    m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
	    return;
    }

    int x=0;
    XCopyArea(m_display,left_side_pixmap,m_doubleBufferPixmap,m_gc,0,0,LEFT_SIDE_WIDTH,LEFT_SIDE_HEIGHT,x,0);
    x+=LEFT_SIDE_WIDTH;
    XCopyArea(m_display,dials_pixmap,m_doubleBufferPixmap,m_gc,0,0,SINGLE_DIAL_WIDTH,DIALS_HEIGHT,x,0);
    x+=SINGLE_DIAL_WIDTH;
    XCopyArea(m_display,lcd_pixmap,m_doubleBufferPixmap,m_gc,0,0,LCD_WIDTH,LCD_HEIGHT,x,0);
    x+=LCD_WIDTH;
    XCopyArea(m_display,dials_pixmap,m_doubleBufferPixmap,m_gc,0,0,SINGLE_DIAL_WIDTH,DIALS_HEIGHT,x,0);
    x+=SINGLE_DIAL_WIDTH;
    XCopyArea(m_display,right_side_pixmap,m_doubleBufferPixmap,m_gc,0,0,RIGHT_SIDE_WIDTH,RIGHT_SIDE_HEIGHT,x,0);
    x+=RIGHT_SIDE_WIDTH;
    m_mainWindow->SetPixmap(m_doubleBufferPixmap);

    m_playButton->SetPixmap(play_buttons_pixmap);
    m_pauseButton->SetPixmap(pause_buttons_pixmap);
    m_prevButton->SetPixmap(prev_buttons_pixmap);
    m_nextButton->SetPixmap(next_buttons_pixmap);
    m_switchModeButton->SetPixmap(mode_buttons_pixmap);

    size_hints->flags = PPosition | PSize | PMinSize;
    size_hints->min_width = TOTAL_WIDTH;
    size_hints->min_height = TOTAL_HEIGHT;
    
    /* These calls store window_name and icon_name into
     * XTextProperty structures and set their other 
     * fields properly. */

    char *window_name = "FreeAmp v1.0.0";
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
    m_mainWindow->SelectInput(ExposureMask | KeyPressMask | ButtonReleaseMask | KeyReleaseMask | PointerMotionMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);
    
    m_playButton->SelectInput(ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);

    m_pauseButton->SelectInput(ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);

    m_prevButton->SelectInput(ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);

    m_nextButton->SelectInput(ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);

    m_switchModeButton->SelectInput(ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);

    
    // set up shape
    XSetForeground(m_display,m_gc,BlackPixel(m_display,m_screenNum));
    XFillRectangle(m_display,m_mainWindow->GetWindow(),m_gc,0,0,TOTAL_WIDTH,TOTAL_HEIGHT);
    m_mainWindow->SetMask(player_full_mask_pixmap);

    m_playButton->SetMask(major_button_mask_pixmap);
    m_pauseButton->SetMask(major_button_mask_pixmap);
    m_prevButton->SetMask(major_button_mask_pixmap);
    m_nextButton->SetMask(major_button_mask_pixmap);
    m_switchModeButton->SetMask(minor_button_mask_pixmap);

    gtkListenThread = Thread::CreateThread();
    gtkListenThread->Create(FreeAmpUI::x11ServiceFunction,this);
    

}

void FreeAmpUI::x11ServiceFunction(void *p) {
    ((FreeAmpUI *)p)->X11EventService();
}

void FreeAmpUI::X11EventService() {
    int32 button_click_spot_x = 0;
    int32 button_click_spot_y = 0;
    XEvent report;

    m_windowHash->Insert(m_mainWindow->GetWindow(),(FAWindow *)m_mainWindow);
    m_windowHash->Insert(m_playButton->GetWindow(),(FAWindow *)m_playButton);
    m_windowHash->Insert(m_pauseButton->GetWindow(),(FAWindow *)m_pauseButton);
    m_windowHash->Insert(m_prevButton->GetWindow(),(FAWindow *)m_prevButton);
    m_windowHash->Insert(m_nextButton->GetWindow(),(FAWindow *)m_nextButton);
    m_windowHash->Insert(m_switchModeButton->GetWindow(),(FAWindow *)m_switchModeButton);

    /* Display window */
    m_mainWindow->MapWindow();
    m_playButton->MapWindow();
    m_pauseButton->MapWindow();
    m_nextButton->MapWindow();
    m_prevButton->MapWindow();
    m_switchModeButton->MapWindow();
    
    fprintf(stderr,"Main window ID: %x\n",m_mainWindow->GetWindow());
    fprintf(stderr,"Play Button ID: %x\n",m_playButton->GetWindow());
    fprintf(stderr,"PauseButton ID: %x\n",m_pauseButton->GetWindow());
    fprintf(stderr,"Prev Button ID: %x\n",m_prevButton->GetWindow());
    fprintf(stderr,"Next Button ID: %x\n",m_nextButton->GetWindow());


    /* get events, use first to display text and graphics */
    while (1)  {
	XNextEvent(m_display, &report);

	//fprintf(stderr, "window ID: %x\n", report.xany.window);
        FAWindow *w = m_windowHash->Value(report.xany.window);
	
	if (w) {
	    //fprintf(stderr, "handling by class\n");
	    w->DoEvent(report);
	}
    } /* end while */
}
