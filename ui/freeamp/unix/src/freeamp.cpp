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
	
	$$
____________________________________________________________________________*/

extern "C" {

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <X11/xpm.h>

#include <X11/extensions/shape.h>


#include <stdio.h>
	   }

#include "config.h"
#include "event.h"

#include "freeamp-x11.h"

#define BITMAPDEPTH 1
#define TOO_SMALL 0
#define BIG_ENOUGH 1

Display *display;
int screen_num;
Pixmap double_buffer_pixmap;

#include "logo.xpm"
#include "logo256.xpm"

#define RIGHT_SIDE_WIDTH 83
#define RIGHT_SIDE_HEIGHT 51
#include "rightside.xpm"
#include "rightside256.xpm"
Pixmap right_side_pixmap;

#define LEFT_SIDE_WIDTH 138
#define LEFT_SIDE_HEIGHT 51
#include "leftside.xpm"
#include "leftside256.xpm"
Pixmap left_side_pixmap;

#define LCD_WIDTH 190
#define LCD_HEIGHT 51
#include "lcd.xpm"
#include "lcd256.xpm"
Pixmap lcd_pixmap;

#define DIALS_WIDTH 66
#define DIALS_HEIGHT 51
#define SINGLE_DIAL_WIDTH 11
#include "dials.xpm"
#include "dials256.xpm"
Pixmap dials_pixmap;

#define TOTAL_WIDTH (LEFT_SIDE_WIDTH + SINGLE_DIAL_WIDTH + LCD_WIDTH + SINGLE_DIAL_WIDTH + RIGHT_SIDE_WIDTH)
#define TOTAL_HEIGHT 51

#include "player_full_mask.xbm"

#define PLAYER_FULL_MASK_WIDTH player_full_mask_width
#define PLAYER_FULL_MASK_HEIGHT player_full_mask_height
Pixmap player_full_mask_pixmap;


#define PLAY_BUTTON_X 29
#define PLAY_BUTTON_Y 15
#define PLAY_BUTTON_WIDTH 20
#define PLAY_BUTTON_HEIGHT 20
#include "play_buttons.xpm"
#include "play_buttons256.xpm"
Pixmap play_buttons_pixmap;
Window play_button_window;

#define MAJOR_BUTTON_MASK_WIDTH 20
#define MAJOR_BUTTON_MASK_HEIGHT 20
#include "major_button_mask.xbm"
Pixmap major_button_mask_pixmap;

#define MINOR_BUTTON_MASK_WIDTH 20
#define MINOR_BUTTON_MASK_HEIGHT 20
#include "minor_button_mask.xbm"
Pixmap minor_button_mask_pixmap;

static char *progname; /* name this program was invoked by */

void draw_graphics(Window win, GC gc, uint32 window_width, uint32 window_height);

void FreeAmpUI::SetArgs(int argc, char **argv) {
    m_argc = argc;
    m_argv = argv;
}
extern "C" {

UserInterface *Initialize() {
    return new FreeAmpUI();
}

	   }

FreeAmpUI::FreeAmpUI() {}
FreeAmpUI::~FreeAmpUI() {}
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

int foo,bar,baz,biz;
Window win;
unsigned int width, height;	/* window size */
int x, y; 	/* window position */
unsigned int display_width, display_height;
unsigned int icon_width, icon_height;
char *window_name = "FreeAmp v1.0.0";
char *icon_name = "FreeAmp";
Pixmap icon_pixmap;
GC gc;
int count;
XSizeHints *size_hints;
XIconSize *size_list;
XWMHints *wm_hints;
XClassHint *class_hints;
XTextProperty windowName, iconName;
char *display_name = (char *)NULL;
int window_size = BIG_ENOUGH;	/* or TOO_SMALL to display contents */
int32 display_depth;

void FreeAmpUI::Init()
{

    if (!(size_hints = XAllocSizeHints())) {
	fprintf(stderr, "%s: failure allocating memory\n", progname);
        exit(0);
    }
    if (!(wm_hints = XAllocWMHints())) {
	fprintf(stderr, "%s: failure allocating memory\n", progname);
        exit(0);
    }
    if (!(class_hints = XAllocClassHint())) {
	fprintf(stderr, "%s: failure allocating memory\n", progname);
        exit(0);
    }
    
    progname = m_argv[0];
    
    /* connect to X server */
    if ( (display=XOpenDisplay(display_name)) == NULL )
    {
	(void) fprintf( stderr, "%s: cannot connect to X server %s\n", 
			progname, XDisplayName(display_name));
	exit( -1 );
    }
    

    if (!XShapeQueryExtension(display,&foo,&bar)) {
	fprintf( stderr, "%s: No Shape Extension on server %s\n",progname, XDisplayName(display_name));
    }

    /* get screen size from display structure macro */
    screen_num = DefaultScreen(display);
    display_width = DisplayWidth(display, screen_num);
    display_height = DisplayHeight(display, screen_num);
    display_depth = DefaultDepth(display,screen_num);
    cerr << endl<<endl<<"Screen Depth: " << display_depth << endl;
    
    win = XCreateSimpleWindow(display, RootWindow(display,screen_num), 
			      0, 0, TOTAL_WIDTH, TOTAL_HEIGHT, 0,
			      BlackPixel(display,screen_num), WhitePixel(display,screen_num));

    play_button_window = XCreateSimpleWindow(display,win,PLAY_BUTTON_X,PLAY_BUTTON_Y,
					     PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT,0,BlackPixel(display,screen_num),
					     WhitePixel(display,screen_num));

    double_buffer_pixmap = XCreatePixmap(display,win,TOTAL_WIDTH,TOTAL_HEIGHT,display_depth);

    /* Get available icon sizes from Window manager */
    
//    if (XGetIconSizes(display, RootWindow(display,screen_num), 
//		      &size_list, &count) == 0) {
//	;
//    } else {
//	;
//    }
    
    switch (display_depth) {
	case 16:
	    XpmCreatePixmapFromData(display,win,logo,&icon_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(display,win,leftside,&left_side_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(display,win,dials,&dials_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(display,win,lcd,&lcd_pixmap,NULL,NULL);
	    
	    player_full_mask_pixmap =XCreateBitmapFromData(display,win,(char *)player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);

	    XpmCreatePixmapFromData(display,win,rightside,&right_side_pixmap,NULL,NULL);

	    major_button_mask_pixmap = XCreateBitmapFromData(display,win,(char *)major_button_mask_bits,MAJOR_BUTTON_MASK_WIDTH,MAJOR_BUTTON_MASK_HEIGHT);
	    minor_button_mask_pixmap = XCreateBitmapFromData(display,win,(char *)minor_button_mask_bits,MINOR_BUTTON_MASK_WIDTH,MINOR_BUTTON_MASK_HEIGHT);
	    XpmCreatePixmapFromData(display,win,play_buttons,&play_buttons_pixmap,NULL,NULL);
	    break;
	case 8:
	    XpmCreatePixmapFromData(display,win,logo256,&icon_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(display,win,leftside256,&left_side_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(display,win,dials256,&dials_pixmap,NULL,NULL);
	    XpmCreatePixmapFromData(display,win,lcd256,&lcd_pixmap,NULL,NULL);
	    
	    player_full_mask_pixmap =XCreateBitmapFromData(display,win,(char *)player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);

	    XpmCreatePixmapFromData(display,win,rightside256,&right_side_pixmap,NULL,NULL);

	    major_button_mask_pixmap = XCreateBitmapFromData(display,win,(char *)major_button_mask_bits,MAJOR_BUTTON_MASK_WIDTH,MAJOR_BUTTON_MASK_HEIGHT);
	    minor_button_mask_pixmap = XCreateBitmapFromData(display,win,(char *)minor_button_mask_bits,MINOR_BUTTON_MASK_WIDTH,MINOR_BUTTON_MASK_HEIGHT);
	    XpmCreatePixmapFromData(display,win,play_buttons256,&play_buttons_pixmap,NULL,NULL);
	    break;
	default:
	    cerr << "Only know how to deal with bit depths of 8 or 16 (256 or 65535 colors)!!!" << endl;
	    m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
    }

    size_hints->flags = PPosition | PSize | PMinSize;
    size_hints->min_width = width;
    size_hints->min_height = height;
    
    /* These calls store window_name and icon_name into
     * XTextProperty structures and set their other 
     * fields properly. */
    if (XStringListToTextProperty(&window_name, 1, &windowName) == 0) {
	(void) fprintf( stderr, "%s: structure allocation for windowName failed.\n", 
			progname);
	exit(-1);
    }
    
    if (XStringListToTextProperty(&icon_name, 1, &iconName) == 0) {
	(void) fprintf( stderr, "%s: structure allocation for iconName failed.\n", 
			progname);
	exit(-1);
    }
    
    wm_hints->initial_state = NormalState;
    wm_hints->input = True;
    wm_hints->icon_pixmap = icon_pixmap;
    wm_hints->flags = StateHint | IconPixmapHint | InputHint;
    
    class_hints->res_name = progname;
    class_hints->res_class = "Basicwin";
    
    XSetWMProperties(display, win, &windowName, &iconName, 
		     m_argv, m_argc, size_hints, wm_hints, 
		     class_hints);
    
    /* Select event types wanted */
    XSelectInput(display, win, ExposureMask | KeyPressMask | ButtonReleaseMask | KeyReleaseMask | PointerMotionMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);
    
    XSelectInput(display, play_button_window, ExposureMask | ButtonReleaseMask |
		 ButtonPressMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | SubstructureNotifyMask);
    
    
    /* create GC for text and drawing */
    {
	XGCValues values;
	int valuemask = 0;
	gc = XCreateGC(display, win, valuemask, &values);

    }

    // set up shape
    XSetForeground(display,gc,BlackPixel(display,screen_num));
    XFillRectangle(display,win,gc,0,0,TOTAL_WIDTH,TOTAL_HEIGHT);
    XShapeCombineMask(display,win,ShapeBounding,0,0,player_full_mask_pixmap,ShapeSet);

    XShapeCombineMask(display,play_button_window,ShapeBounding,0,0,major_button_mask_pixmap,ShapeSet);

    gtkListenThread = Thread::CreateThread();
    gtkListenThread->Create(FreeAmpUI::x11ServiceFunction,this);
    

}

void FreeAmpUI::x11ServiceFunction(void *p) {
    int32 button_click_spot_x = 0;
    int32 button_click_spot_y = 0;
    XEvent report;
    FreeAmpUI *pMe = (FreeAmpUI*)p;

    /* Display window */
    XMapWindow(display, win);
    XMapWindow(display,play_button_window);
    
    fprintf(stderr,"Main window ID: %x\n",win);
    fprintf(stderr,"Play Button ID: %x\n",play_button_window);

    /* get events, use first to display text and graphics */
    while (1)  {
	XNextEvent(display, &report);

	//fprintf(stderr, "window ID: %x\n", report.xany.window);
	switch  (report.type) {
	    case MotionNotify:
		//fprintf(stderr, "%s: motion: %d  %d\n",progname, report.xmotion.x_root,report.xmotion.y_root);
		if (report.xmotion.state & Button1Mask) {
		    //fprintf(stderr, "%s: trying %d  %d\n",progname,report.xmotion.x_root-report.xmotion.x,report.xmotion.y_root-report.xmotion.y);
		    //fprintf(stderr, "%s: from   %d  %d  and %d  %d\n",progname,report.xmotion.x_root,report.xmotion.y_root,report.xmotion.x,report.xmotion.y);
		    //XMoveWindow(display,win,report.xmotion.x_root-report.xmotion.x,report.xmotion.y_root-report.xmotion.y);
		    XMoveWindow(display,win,report.xmotion.x_root-button_click_spot_x,report.xmotion.y_root-button_click_spot_y);
//		    XMoveWindow(display,win,report.xmotion.x_root,report.xmotion.y_root);
		}
		break;
	    case Expose:
		//fprintf(stderr, "%s: expose\n",progname);
		/* unless this is the last contiguous expose,
		 * don't draw the window */
		if (report.xexpose.count != 0)
		    break;
				/* place graphics in window, */
		draw_graphics(report.xexpose.window, gc, width, height);
		break;
	    case ConfigureNotify:
		/* window has been resized, change width and
		 * height to send to draw_text and draw_graphics
		 * in next Expose */
		//fprintf(stderr,"%s: ConfigureNotify %d %d\n",progname,report.xconfigure.x,report.xconfigure.y);
		width = report.xconfigure.width;
		height = report.xconfigure.height;
		if ((width < size_hints->min_width) || 
		    (height < size_hints->min_height))
		    window_size = TOO_SMALL;
		else
		    window_size = BIG_ENOUGH;
		break;
	    case ButtonPress:
		//fprintf(stderr, "%s: button press\n",progname);
		button_click_spot_x = report.xbutton.x;
		button_click_spot_y = report.xbutton.y;
		if (report.xbutton.window == play_button_window) {
		    XCopyArea(display,play_buttons_pixmap,play_button_window,gc,0,2*PLAY_BUTTON_HEIGHT,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT,0,0);
		}
		break;
	    case ButtonRelease:
		//fprintf(stderr, "%s: button release\n",progname);
		if (report.xbutton.window == play_button_window) {
		    XCopyArea(display,play_buttons_pixmap,play_button_window,gc,0,0,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT,0,0);
		    pMe->m_plm->RemoveAll();
		    pMe->m_plm->Add("/home/jdw/mp3s/old/incomplete-Aqua - Happy Boys and Girls.mp3",0);
		    pMe->m_plm->SetFirst();
		    pMe->m_playerEQ->AcceptEvent(new Event(CMD_Play));
		}
		break;
	    case EnterNotify:
		//fprintf(stderr, "%s: entered window\n",progname);
		if (report.xcrossing.window == play_button_window) {
		    XCopyArea(display,play_buttons_pixmap,play_button_window,gc,0,PLAY_BUTTON_HEIGHT,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT,0,0);
		}
		break;
	    case LeaveNotify:
		//fprintf(stderr,"%s: left window\n",progname);
		if (report.xcrossing.window == play_button_window) {
		    XCopyArea(display,play_buttons_pixmap,play_button_window,gc,0,0,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT,0,0);
		}
		break;
	    case KeyPress: {
		break;
	    }
	    case KeyRelease: {
		char foo[32];
		fprintf(stderr,"%s: key released\n",progname);
		XLookupString((XKeyEvent *)&report,foo,sizeof(foo),NULL,NULL);
		//fprintf(stderr, "%s: keypress %s\n",progname,foo);
		if (foo[0] == 'q') {
		    pMe->m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
		    XFreeGC(display, gc);
		    XCloseDisplay(display);
		    return;
		}
		break; }
	    case DestroyNotify:
		fprintf(stderr, "%s: destroy notify\n",progname);
		pMe->m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
		XFreeGC(display, gc);
		XCloseDisplay(display);
		return;
	    default:
		/* all events selected by StructureNotifyMask
		 * except ConfigureNotify are thrown away here,
		 * since nothing is done with them */
		break;
	} /* end switch */
    } /* end while */
    fprintf(stderr, "%s: done!\n",progname);
}

void draw_graphics(Window win, GC gc, uint32 window_width, uint32 window_height)
{
    int x = 0;
    if (win == ::win) {
	XCopyArea(display,left_side_pixmap,double_buffer_pixmap,gc,0,0,LEFT_SIDE_WIDTH,LEFT_SIDE_HEIGHT,x,0);
	x+=LEFT_SIDE_WIDTH;
	XCopyArea(display,dials_pixmap,double_buffer_pixmap,gc,0,0,SINGLE_DIAL_WIDTH,DIALS_HEIGHT,x,0);
	x+=SINGLE_DIAL_WIDTH;
	XCopyArea(display,lcd_pixmap,double_buffer_pixmap,gc,0,0,LCD_WIDTH,LCD_HEIGHT,x,0);
	x+=LCD_WIDTH;
	XCopyArea(display,dials_pixmap,double_buffer_pixmap,gc,0,0,SINGLE_DIAL_WIDTH,DIALS_HEIGHT,x,0);
	x+=SINGLE_DIAL_WIDTH;
	XCopyArea(display,right_side_pixmap,double_buffer_pixmap,gc,0,0,RIGHT_SIDE_WIDTH,RIGHT_SIDE_HEIGHT,x,0);
	x+=RIGHT_SIDE_WIDTH;
	XCopyArea(display,double_buffer_pixmap,win,gc,0,0,TOTAL_WIDTH,TOTAL_HEIGHT,0,0);
    } else if (win == play_button_window) {
	XCopyArea(display,play_buttons_pixmap,play_button_window,gc,0,0,PLAY_BUTTON_WIDTH,PLAY_BUTTON_HEIGHT,0,0);
    }
}









