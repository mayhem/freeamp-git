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


#include <stdlib.h>
#include <memory.h>
#include <iostream.h>
#include <string.h>
#include <stdio.h>


#include "fawindow.h"
#include "lcd_display_mask.xbm"
#include "lcd_icons_mask.xbm"


FAWindow::FAWindow() { 
    m_mapped = false;
}

FAWindow::~FAWindow() { }

void FAWindow::SetTarget(EventQueue *eq) { m_playerEQ = eq; }

Window FAWindow::GetParent() { return m_parent; } 

Window FAWindow::GetWindow() { return m_me; }

GC FAWindow::GetGC() { return m_gc; }

void FAWindow::SetGC(GC gc) { m_gc = gc; }

void FAWindow::SetPixmap(Pixmap pm) { m_pixmap = pm; }

void FAWindow::SetMask(Pixmap mask) { XShapeCombineMask(m_display,m_me,ShapeBounding,0,0,mask,ShapeSet); }

void FAWindow::SelectInput(int32 mask) { XSelectInput(m_display,m_me,mask); }

void FAWindow::MapWindow() { 
    XMapWindow(m_display,m_me); 
    m_mapped = true;
}

void FAWindow::UnMapWindow() { 
    XUnmapWindow(m_display,m_me); 
    m_mapped = false;
}

FAMainWindow::FAMainWindow(Display *display, int32 screen_num,GC gc, Window parent,int32 x,int32 y,int32 w, int32 h) {
    m_display = display;
    m_screenNum = screen_num;
    m_gc = gc;
    m_parent = parent;
    m_xParent = x;
    m_yParent = y;
    m_width = w;
    m_height = h;
    m_me = XCreateSimpleWindow(m_display,m_parent,m_xParent,m_yParent,m_width,m_height,0,BlackPixel(m_display,m_screenNum),WhitePixel(m_display,m_screenNum));
    //fprintf(stderr,"created main win\n");
}

FAMainWindow::~FAMainWindow() { }
void FAMainWindow::DoEvent(XEvent e) {
    switch (e.type) {
	case MotionNotify:
	    if (e.xmotion.state & Button1Mask) {
		XMoveWindow(m_display,m_me,e.xmotion.x_root-m_buttonClickSpotX,e.xmotion.y_root-m_buttonClickSpotY);
	    }
	    break;
	case Expose:
	    if (e.xexpose.count != 0)
		return;
	    Draw();
	    break;
	case ButtonPress:
	    m_buttonClickSpotX = e.xbutton.x;
	    m_buttonClickSpotY = e.xbutton.y;
	    break;
	case KeyRelease: {
	    char foo[32];
	    //fprintf(stderr,"key released\n");
	    XLookupString(&(e.xkey),foo,sizeof(foo),NULL,NULL);
	    if (foo[0] == 'q') {
		m_playerEQ->AcceptEvent(new Event(CMD_QuitPlayer));
	    }
	    //fprintf(stderr,"done key release\n");
	    break;
	}
    }
}

void FAMainWindow::Draw() {
    if (m_mapped) {
	XCopyArea(m_display,m_pixmap,m_me,m_gc,0,0,m_width,m_height,0,0);
	XFlush(m_display);
    }
}

FATriStateWindow::FATriStateWindow(Display *display, int32 screen_num,GC gc, Window parent,int32 x,int32 y,int32 w, int32 h) {
    m_display = display;
    m_screenNum = screen_num;
    m_gc = gc;
    m_parent = parent;
    m_xParent = x;
    m_yParent = y;
    m_width = w;
    m_height = h;
    m_me = XCreateSimpleWindow(m_display,m_parent,m_xParent,m_yParent,m_width,m_height,0,BlackPixel(m_display,m_screenNum),WhitePixel(m_display,m_screenNum));
    m_activated = false;
    m_pressed = false;
    m_insideButton = false;
    //fprintf(stderr,"Created tristate window\n");
}

FATriStateWindow::~FATriStateWindow() { }

void FATriStateWindow::SetPartialHeight(int32 ph) {
    m_partialHeight = ph;
}

void FATriStateWindow::SetActivated() {
    //cerr << "SetActivated" << endl;
    m_activated = true;
    Draw();
//    XEvent x;
//    x.xexpose.count = 0;
//    XSendEvent(m_display,m_me,false,ExposureMask,&x);
//    XFlush(m_display);
}

void FATriStateWindow::ClearActivated() {
    //cerr << "ClearActivated" << endl;
    m_activated = false;
    Draw();
//    XEvent x;
//    x.xexpose.count = 0;
//    XSendEvent(m_display,m_me,false,ExposureMask,&x);
//    XFlush(m_display);
}

void FATriStateWindow::DoEvent(XEvent e) {
    switch (e.type) {
	case Expose:
	    //cerr << "got exposure" << endl;
	    if (e.xexpose.count != 0)
		return;
	    Draw();
	    break;
	case ButtonPress:
	    m_pressed = true;
	    Draw();
	    break;
	case ButtonRelease:
	    //cerr << "button release" << endl;
	    m_pressed = false;
	    if (m_insideButton) {
		m_clickFunction(m_cookie);
	    }
	    //cerr << "about to draw" << endl;
	    Draw();
	    //cerr << "done drawing" << endl;
	    break;
	case EnterNotify:
	    m_insideButton = true;
	    Draw();
	    break;
	case LeaveNotify:
	    m_insideButton = false;
	    Draw();
	    break;

    }
}

void FATriStateWindow::Draw() {
    //cerr << "Drawing..." << endl;
    if (m_mapped) {
	int32 theInt;
	if (m_activated) {
	    theInt = 2;
	} else {
	    if (m_pressed) {
		if (m_insideButton) {
		    theInt = 2;
		} else {
		    theInt = 0;
		}
	    } else {
		if (m_insideButton) {
		    theInt = 1;
		} else {
		    theInt = 0;
		}
	    }
	}
	//fprintf(stderr, "tristate: window: %x  pixmap: %x  src_y: %x\n",m_me,m_pixmap,theInt*m_partialHeight);
	XCopyArea(m_display,m_pixmap,m_me,m_gc,
		  0, //src_x
		  theInt*m_partialHeight, // src_y
		  m_width, // width
		  m_partialHeight, //height
		  0,  //dest_x
		  0); // dest_y
	XFlush(m_display);
    }
}

void FATriStateWindow::SetClickAction(action_function f, void *p) {
    m_cookie = p;
    m_clickFunction = f;
}


FALcdWindow::FALcdWindow(Display *display, int32 screen_num,GC gc, Window parent,int32 x,int32 y,int32 w, int32 h) {
    m_display = display;
    m_screenNum = screen_num;
    m_parent = parent;
    m_xParent = x;
    m_yParent = y;
    m_width = w;
    m_height = h;
    m_me = XCreateSimpleWindow(m_display,m_parent,m_xParent,m_yParent,m_width,m_height,0,BlackPixel(m_display,m_screenNum),WhitePixel(m_display,m_screenNum));
    m_doubleBufferPixmap = XCreatePixmap(m_display,m_me,m_width,m_height,DefaultDepth(m_display,m_screenNum));

    XGCValues gcv;
    gcv.fill_style = FillSolid;
    gcv.foreground = 1;
    gcv.background = 0;
    m_gc = XCreateGC(m_display,m_me, GCForeground | GCBackground | GCFillStyle,&gcv);


    m_mainTextMask = XCreateBitmapFromData(m_display,m_me,(char *)lcd_display_mask_bits,lcd_display_mask_width,lcd_display_mask_height);
    m_iconMask = XCreateBitmapFromData(m_display,m_me,(char *)lcd_icons_mask_bits,lcd_icons_mask_width,lcd_icons_mask_height);

    m_text = new char[19];
    strcpy(m_text,"Welcome to FreeAmp");
    m_displayState = TotalTimeState;
};

FALcdWindow::~FALcdWindow() { 
    XFreePixmap(m_display,m_doubleBufferPixmap);
    XFreeGC(m_display,m_gc);
}

void FALcdWindow::DoEvent(XEvent e) {
    switch (e.type) {
	case Expose:
	    //cerr << "got exposure" << endl;
	    if (e.xexpose.count != 0)
		return;
	    Draw();
	    break;
	case ButtonPress:
	    break;
	case ButtonRelease:
	    break;
	case EnterNotify:
	    break;
	case LeaveNotify:
	    break;
    }
}

void FALcdWindow::SetMainText(const char *pText) {
    if (m_text) delete m_text;
    int32 l = strlen(pText);
    m_text = new char[l+1];
    memcpy(m_text,pText,l*sizeof(char));
    Draw();
}

void FALcdWindow::SetSmallFontPixmap(Pixmap p) {
    m_smallFontPixmap = p;
}

void FALcdWindow::SetSmallFontWidth(int *i) {
    m_smallFontWidth = i;
}

void FALcdWindow::SetLargeFontPixmap(Pixmap p) {
    m_largeFontPixmap = p;
}

void FALcdWindow::SetLargeFontWidth(int *i) {
    m_largeFontWidth = i;
}

void FALcdWindow::Draw() {
    if (!m_mapped) return;
    XSetClipMask(m_display,m_gc,None);
    XCopyArea(m_display,m_pixmap,m_doubleBufferPixmap,m_gc,0,0,m_width,m_height,0,0);

    cerr << "Drawing text: " << m_text << endl;

    switch (m_displayState) {
	case IntroState: {
	    XSetClipMask(m_display,m_gc,m_mainTextMask);
	    BlitText(m_doubleBufferPixmap,20,10,m_text,SmallFont);
	    break;
	}
	case VolumeState: {
	    XSetClipMask(m_display,m_gc,m_mainTextMask);
	    BlitText(m_doubleBufferPixmap,20,10,m_text,SmallFont);
	    break;
	}
	case CurrentTimeState: {
	    XSetClipMask(m_display,m_gc,m_mainTextMask);
	    BlitText(m_doubleBufferPixmap,20,10,m_text,SmallFont);
	    
	    break;
	}
	case SeekTimeState: {
	    XSetClipMask(m_display,m_gc,m_mainTextMask);
	    BlitText(m_doubleBufferPixmap,20,10,m_text,SmallFont);
	    
	    break;
	}
	case RemainingTimeState: {
	    XSetClipMask(m_display,m_gc,m_mainTextMask);
	    BlitText(m_doubleBufferPixmap,20,10,m_text,SmallFont);
	    
	    break;
	}
	case TotalTimeState: {
	    XSetClipMask(m_display,m_gc,m_mainTextMask);
	    BlitText(m_doubleBufferPixmap,20,10,m_text,SmallFont);
	    break;
	}
    }

    XCopyArea(m_display,m_doubleBufferPixmap,m_me,m_gc,0,0,m_width,m_height,0,0);
    XFlush(m_display);
}

void FALcdWindow::BlitText(Drawable d, int32 x, int32 y, const char *text, int32 font) {
    switch (font) {
	case LargeFont: {
	    int32 offset = x;
	    for (int i=0;text[i];i++) {
		XCopyArea(m_display,m_largeFontPixmap,d,m_gc,
			  0,(text[i] - 32)*12,
			  m_largeFontWidth[text[i] - 32], 12,
			  offset, y);
		offset += m_largeFontWidth[text[i] - 32];
	    }
	    break;
	}
	case SmallFont: {
	    int32 offset = x;
	    for (int i=0;text[i];i++) {
		XCopyArea(m_display,m_smallFontPixmap,d,m_gc,
			  0,(text[i] - 32)*10,
			  m_smallFontWidth[text[i] - 32], 10,
			  offset, y);
		offset += m_smallFontWidth[text[i] - 32];
	    }
	    break;
	}
    }
}






