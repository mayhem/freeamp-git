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

#include "config.h"

#include <stdio.h>
#include <string>
#include "thread.h"
#include "GTKUtility.h"
#include "facontext.h"
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <iostream>
#include <unistd.h>

#include <vector>
#include <map>
using namespace std;

#include "MessageDialog.h"

static Thread *gtkThread = NULL;
static bool weAreGTK = false;
static bool doQuitNow = false;
static FAContext *ourContext;

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

Pos GetFocusPos(void)
{
    Window win, tempwin;
    XWindowAttributes win_attr;
    int v, rx = -1, ry = -1;
    char *name;
    Pos retpos;

    if (XGetInputFocus(GDK_DISPLAY(), &win, &v) &&
        XFetchName(GDK_DISPLAY(), win, &name) &&
        strncmp(name, BRANDING, strlen(BRANDING)))
    {
        if (XGetWindowAttributes(GDK_DISPLAY(), win, &win_attr)) 
            XTranslateCoordinates(GDK_DISPLAY(), win, win_attr.root,
                                  -win_attr.border_width, 
                                  -win_attr.border_width, &rx, &ry,
                                  &tempwin);
    }

    retpos.x = rx;
    retpos.y = ry;

    return retpos;
}

static gint theme_timeout(void *c)
{
    ourContext->gtkRunning = true;
    if (doQuitNow)
        gtk_main_quit();

    return TRUE;
}

static void runGTK(void *c)
{
    gtk_timeout_add(250, theme_timeout, NULL);
    gtk_main();
    gdk_threads_leave();
}

void InitializeGTK(FAContext *context)
{
    ourContext = context;

    if (gtkThread)
        return;
    context->gtkLock.Acquire();
    if (!context->gtkInitialized) {
        context->gtkInitialized = true;
	g_thread_init(NULL);
        gtk_set_locale();
	gtk_init(&context->argc, &context->argv);
	gdk_rgb_init();
	weAreGTK = true;
    }
    context->gtkLock.Release();
    
    if (weAreGTK) {
        gtkThread = Thread::CreateThread();
        gtkThread->Create(runGTK, NULL);
    }

    bool running = false;

    while (!running) {
        context->gtkLock.Acquire();
        running = context->gtkRunning;
        context->gtkLock.Release();
        usleep(50);
    }

    bool reclaimFileTypes, askBeforeReclaiming;
    context->prefs->GetReclaimFiletypes(&reclaimFileTypes);
    context->prefs->GetAskToReclaimFiletypes(&askBeforeReclaiming);

    if (reclaimFileTypes)
        ReclaimFileTypes(context, askBeforeReclaiming);
}

void ShutdownGTK(void)
{
    if (weAreGTK && gtkThread) {
        gdk_threads_enter();
        gtk_main_quit();
        gdk_threads_leave();
        weAreGTK = false;
        doQuitNow = true;
        gtkThread->Join();
        gtkThread = NULL;
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

const char *kMimeTypes[] = {
    "audio/x-mpeg",
    "audio/x-mp3",
    "audio/x-mpegurl",
    "audio/x-scpls",
    "audio/mpeg",
    "audio/mp3",
    "audio/mpegurl",
    "audio/scpls",
    "application/vnd.rn-rn_music_package",
    "application/x-freeamp-theme",
    NULL
};
#define kNumMime 10

const char* kNotifyStolen = "Music files normally associated with "the_BRANDING "\n"
                            "have been associated with another application.\n"
                            "Do you want to reclaim these music files?";

void AddMissingMimeTypes(void)
{
    FILE *f;
    char *buffer, *mime;
    uint32 index;
    bool needRewrite = false; 
    vector<string> oldfile;
    bool found[kNumMime];

    for (index = 0; index < kNumMime; index++)
        found[index] = false;

    buffer = new char[1024];
    mime = new char[1024];

    string mimeTypes = string(getenv("HOME")) + string("/.mime.types");

    f = fopen(mimeTypes.c_str(), "r");

    if (f) {
        while (fgets(buffer, 1024, f)) 
            oldfile.push_back(buffer);
        fclose(f);

        vector<string>::iterator i;
        for (i = oldfile.begin(); i != oldfile.end(); i++) {
            strcpy(buffer, (*i).c_str());
            if (!strncmp("type", buffer, 4)) {
                sscanf(buffer, "type=%s", mime);
                if (*mime) {
                    for (index = 0; ; index++) {
                        if (kMimeTypes[index] == NULL)
                            break;
                        if (!strcmp(mime, kMimeTypes[index])) {
                            found[index] = true;
                            break;
                        }
                    }
                }
            }
        }  
    }
    else {
        oldfile.push_back("#--Netscape Communications Corporation MIME Information\n"); 
        oldfile.push_back("#Do not delete the above line. It is used to identify the file type.\n");
        oldfile.push_back("#\n");
    }

    for (index = 0; index < kNumMime; index++) {
        if (!found[index]) {
            sprintf(buffer, "type=%s\n", kMimeTypes[index]);
            needRewrite = true;
            oldfile.push_back(buffer);
        }
    }            

    if (needRewrite) {
        f = fopen(mimeTypes.c_str(), "w");
        if (!f)
            goto failed_open;
        
        vector<string>::iterator i;
        for (i = oldfile.begin(); i != oldfile.end(); i++) {
             strcpy(buffer, (*i).c_str());
             fputs(buffer, f);
        }
        fclose(f);
    }

failed_open:
    delete [] buffer;
    delete [] mime;
}

void ReclaimFileTypes(FAContext *context, bool askBeforeReclaiming)
{
    AddMissingMimeTypes();

    FILE *f;
    char *buffer, *mime, *app;
    uint32 index;
    bool needRewrite = false;
    map<string, string> oldfile;
    bool found[kNumMime];

    for (index = 0; index < kNumMime; index++)
        found[index] = false;

    buffer = new char[1024];
    mime = new char[1024];
    app = new char[1024];

    string mimeTypes = string(getenv("HOME")) + string("/.mailcap");

    f = fopen(mimeTypes.c_str(), "r");

    if (f) {
        while (fgets(buffer, 1024, f)) {
            if (buffer[0] == '#')
                continue;
            sscanf(buffer, "%[^;];%[^\n]", mime, app);
            oldfile[mime] = app;
        }
        fclose(f);
    }

    for (index = 0; index < kNumMime; index++) {
        string tempstr = kMimeTypes[index];
        if (oldfile.find(tempstr) != oldfile.end()) {
            if (strncmp(oldfile[tempstr].c_str(), BRANDING_APP_NAME,
                        strlen(BRANDING_APP_NAME)))
                needRewrite = true;
            else
                found[index] = true;
        }
        else
            needRewrite = true;
        
    }

    if (needRewrite) {
        if (askBeforeReclaiming) {
            MessageDialog oBox(context);
            string oMessage(kNotifyStolen);

            if (oBox.Show(oMessage.c_str(), string("Reclaim File Types?"),
                          kMessageYesNo) == kMessageReturnNo)
                goto failed_reclaim_open;
        }

        sprintf(buffer, "%s %%s\n", BRANDING_APP_NAME); 
        for (index = 0; index < kNumMime; index++) {
            if (found[index])
                continue;
            oldfile[kMimeTypes[index]] = string(buffer);
        }

        f = fopen(mimeTypes.c_str(), "w");
        if (!f)
            goto failed_reclaim_open;

        map<string, string>::iterator i;
        for (i = oldfile.begin(); i != oldfile.end(); i++) {
            if (i->first.size() > 0 && i->first.c_str()[0] != '\n') {
                sprintf(mime, "%s;%s", i->first.c_str(), i->second.c_str());
                fputs(mime, f);
                fputs("\n", f);
            }
        }
        fclose(f);
    }

failed_reclaim_open:
    delete [] buffer;
    delete [] mime;
    delete [] app;   
}
