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
	
	$ $
____________________________________________________________________________*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <X11/xpm.h>

#include <X11/extensions/shape.h>


#include <stdio.h>

#include "config.h"

#include "freeamp-x11.h"

#define BITMAPDEPTH 1
#define TOO_SMALL 0
#define BIG_ENOUGH 1

Display *display;
int screen_num;
Pixmap double_buffer_pixmap;

#include "logo.xpm"

#define RIGHT_SIDE_WIDTH 83
#define RIGHT_SIDE_HEIGHT 51
#include "rightside.xpm"
Pixmap right_side_pixmap;

#define LEFT_SIDE_WIDTH 138
#define LEFT_SIDE_HEIGHT 51
#include "leftside.xpm"
Pixmap left_side_pixmap;

#define LCD_WIDTH 190
#define LCD_HEIGHT 51
#include "lcd.xpm"
Pixmap lcd_pixmap;

#define DIALS_WIDTH 66
#define DIALS_HEIGHT 51
#define SINGLE_DIAL_WIDTH 11
#include "dials.xpm"
Pixmap dials_pixmap;

#define TOTAL_WIDTH (LEFT_SIDE_WIDTH + SINGLE_DIAL_WIDTH + LCD_WIDTH + SINGLE_DIAL_WIDTH + RIGHT_SIDE_WIDTH)
#define TOTAL_HEIGHT 51

#include "player_full_mask.xbm"

#define PLAYER_FULL_MASK_WIDTH player_full_mask_width
#define PLAYER_FULL_MASK_HEIGHT player_full_mask_height
Pixmap player_full_mask_pixmap;

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
    return 0;
}
void FreeAmpUI::SetPlayListManager(PlayListManager *plm) {
    m_plm = plm;
}

int foo,bar,baz,biz;
Window win;
unsigned int width, height;	/* window size */
int x, y; 	/* window position */
unsigned int border_width = 4;	/* four pixels */
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
    
    /* Note that in a real application, x and y would default to 0
     * but would be settable from the command line or resource database.  
     */
    x = y = 0;
    
    /* size window with enough room for text */
    //width = display_width/3, height = display_height/4;
    width = 433;
    height = 51;
    /* create opaque window */
    win = XCreateSimpleWindow(display, RootWindow(display,screen_num), 
			      x, y, width, height, border_width, 
			      BlackPixel(display,screen_num), WhitePixel(display,screen_num));

    double_buffer_pixmap = XCreatePixmap(display,win,width,height,16);

    /* Get available icon sizes from Window manager */
    
    if (XGetIconSizes(display, RootWindow(display,screen_num), 
		      &size_list, &count) == 0) {
	//(void) fprintf( stderr, "%s: Window manager didn't set icon sizes - using default.\n", progname);
	;
    } else {
	;
	/* A real application would search through size_list
	 * here to find an acceptable icon size, and then
	 * create a pixmap of that size.  This requires
	 * that the application have data for several sizes
	 * of icons. */
    }
    
    /* Create pixmap of depth 1 (bitmap) for icon */
//    icon_pixmap = XCreateBitmapFromData(display, win, icon_bitmap_bits, 
//					icon_bitmap_width, icon_bitmap_height);

    XpmCreatePixmapFromData(display,win,logo,&icon_pixmap,NULL,NULL);
//    XpmReadFileToPixmap(display,win,"logo.xpm",&icon_pixmap,NULL,NULL);
    XpmCreatePixmapFromData(display,win,leftside,&left_side_pixmap,NULL,NULL);
//    XpmReadFileToPixmap(display,win,"leftside.xpm",&left_side_pixmap,NULL,NULL);
    XpmCreatePixmapFromData(display,win,dials,&dials_pixmap,NULL,NULL);
    //XpmReadFileToPixmap(display,win,"dials.xpm",&dials_pixmap,NULL,NULL);
    XpmCreatePixmapFromData(display,win,lcd,&lcd_pixmap,NULL,NULL);
    //XpmReadFileToPixmap(display,win,"lcd.xpm",&lcd_pixmap,NULL,NULL);

    //XReadBitmapFile(display,win,"player_full_mask.xbm",&foo,&bar,&player_full_mask_pixmap,&baz,&biz);
    player_full_mask_pixmap =XCreateBitmapFromData(display,win,player_full_mask_bits,PLAYER_FULL_MASK_WIDTH,PLAYER_FULL_MASK_HEIGHT);

    XpmCreatePixmapFromData(display,win,rightside,&right_side_pixmap,NULL,NULL);

//    if (XpmReadFileToPixmap(display,win,"rightside.xpm",&right_side_pixmap,NULL,NULL)) {
//	fprintf( stderr, "%s: Couldn't read colorful xpm from disk\n",progname);
//    } else {
//	;//fprintf( stderr, "%s: Read xpm from disk\n",progname);
//    }

    /* Set size hints for window manager.  The window manager may
     * override these settings.  Note that in a real
     * application if size or position were set by the user
     * the flags would be UPosition and USize, and these would
     * override the window manager's preferences for this window. */
    
    /* x, y, width, and height hints are now taken from
     * the actual settings of the window when mapped. Note
     * that PPosition and PSize must be specified anyway. */
    
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
    XSelectInput(display, win, ExposureMask | KeyPressMask | ButtonReleaseMask | KeyReleaseMask |
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

    gtkListenThread = Thread::CreateThread();
    gtkListenThread->Create(FreeAmpUI::x11ServiceFunction,this);
    

}

void FreeAmpUI::x11ServiceFunction(void *p) {
    XEvent report;
    FreeAmpUI *pMe = (FreeAmpUI*)p;

    /* Display window */
    XMapWindow(display, win);
    
    /* get events, use first to display text and graphics */
    while (1)  {
	XNextEvent(display, &report);
	switch  (report.type) {
	    case Expose:
		fprintf(stderr, "%s: expose\n",progname);
		/* unless this is the last contiguous expose,
		 * don't draw the window */
		if (report.xexpose.count != 0)
		    break;
		
				/* place graphics in window, */
		draw_graphics(win, gc, width, height);
		break;
	    case ConfigureNotify:
		/* window has been resized, change width and
		 * height to send to draw_text and draw_graphics
		 * in next Expose */
		fprintf(stderr,"%s: ConfigureNotify\n",progname);
		width = report.xconfigure.width;
		height = report.xconfigure.height;
		if ((width < size_hints->min_width) || 
		    (height < size_hints->min_height))
		    window_size = TOO_SMALL;
		else
		    window_size = BIG_ENOUGH;
		break;
	    case ButtonPress:
		fprintf(stderr, "%s: button press\n",progname);
		break;
	    case ButtonRelease:
		fprintf(stderr, "%s: button release\n",progname);
		
		break;
	    case EnterNotify:
		fprintf(stderr, "%s: entered window\n",progname);
		break;
	    case LeaveNotify:
		fprintf(stderr,"%s: left window\n",progname);
		break;
	    case KeyPress: {
		char foo[32];
		XLookupString((XKeyEvent *)&report,foo,sizeof(foo),NULL,NULL);
		fprintf(stderr, "%s: keypress %s\n",progname,foo);
		if (foo[0] == 'q') {
		    XFreeGC(display, gc);
		    XCloseDisplay(display);
		    exit(1);
		}
		break;
	    }
	    case KeyRelease:
		fprintf(stderr,"%s: key released\n",progname);
		break;
	    case DestroyNotify:
		fprintf(stderr, "%s: destroy notify\n",progname);
		XFreeGC(display, gc);
		XCloseDisplay(display);
		exit(1);
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
}









