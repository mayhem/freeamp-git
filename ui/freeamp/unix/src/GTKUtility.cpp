/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#include <string>
#include "thread.h"
#include "GTKUtility.h"
#include "facontext.h"
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <iostream>
#include <unistd.h>

static Thread *gtkThread = NULL;
static bool weAreGTK = false;

void IconifyWindow(GdkWindow *win)
{
    Window window = GDK_WINDOW_XWINDOW(win);
    XIconifyWindow(GDK_DISPLAY(), window, DefaultScreen(GDK_DISPLAY()));
 
}

void WarpPointer(GdkWindow *win, int x, int y)
{
    Window window = GDK_WINDOW_XWINDOW(win);
    XWarpPointer(GDK_DISPLAY(), window, window, 0, 0, 0, 0, x, y);
}

static void runGTK(void *c)
{
    gtk_main();
}

void InitializeGTK(FAContext *context)
{
    if (gtkThread)
        return;
    context->gtkLock.Acquire();
    if (!context->gtkInitialized) {
        context->gtkInitialized = true;

	g_thread_init(NULL);
	gtk_init(&context->argc, &context->argv);
	gdk_rgb_init();
	weAreGTK = true;
    }
    context->gtkLock.Release();
    
    if (weAreGTK) {
        gtkThread = Thread::CreateThread();
        gtkThread->Create(runGTK, NULL);
    }
}

void ShutdownGTK(void)
{
    if (weAreGTK) {
        gdk_threads_enter();
        gtk_main_quit();
        gdk_threads_leave();
        weAreGTK = false;
//        gtkThread->Join();
// gtk_main() NEVER QUITS... stupid, what am I missing?
    }
}

bool ListFonts(char *mask)
{
    int count;
    int maxnames = 32767;
    char **fontnames;
    bool retvalue = false;
    string realmask = string("-*-") + string(mask) + string("-*");
    fontnames = XListFonts(GDK_DISPLAY(), realmask.c_str(), maxnames, &count);
    XFreeFontNames(fontnames);
    if (count > 0)
        retvalue = true;
    return retvalue;
}
