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

#include <stdio.h>
#include <unistd.h>
#include "facontext.h"
#include "errors.h"
#include "Theme.h"
#include "GTKWindow.h"
#include "GTKCanvas.h"
#include "GTKUtility.h" 

static GtkTargetEntry main_drop[] =
{
    { "text/plain", 0, 1 }
};

void mouse_move(GtkWidget *w, GdkEvent *e, GTKWindow *ui)
{
    Pos oPos;

    oPos.x = (int)e->motion.x_root;
    oPos.y = (int)e->motion.y_root;
    gdk_threads_leave();
    ui->HandleMouseMove(oPos);
    gdk_threads_enter();
}

void button_down(GtkWidget *w, GdkEvent *e, GTKWindow *ui)
{
    Pos oPos;

    oPos.x = (int)e->button.x_root;
    oPos.y = (int)e->button.y_root;
    gdk_threads_leave();
    if (e->button.button == 1) 
        ui->HandleMouseLButtonDown(oPos);
    gdk_threads_enter();
}

void button_up(GtkWidget *w, GdkEvent *e, GTKWindow *ui)
{
    Pos oPos;
    oPos.x = (int)e->button.x_root;
    oPos.y = (int)e->button.y_root;
    gdk_threads_leave();
    if (e->button.button == 1)
        ui->HandleMouseLButtonUp(oPos);
    gdk_threads_enter();
}

void key_press(GtkWidget *w, GdkEvent *e, GTKWindow *ui)
{
    char *str = e->key.string;
    gdk_threads_leave();
    ui->Keystroke(str[0]);
    gdk_threads_enter();
}

void drop_file(GtkWidget *w, GdkDragContext *context, gint x, gint y,
               GtkSelectionData *data, guint info, guint time, 
               GTKWindow *ui)
{
    gdk_threads_leave();
    if (data->data) {
        ui->DropFiles((char *)data->data);
    }
    gdk_threads_enter();
}
      
gint do_timeout(GTKWindow *ui)
{
    ui->TimerEvent();
}

GTKWindow::GTKWindow(Theme *pTheme, string &oName)
          :Window(pTheme, oName)
{
    m_pCanvas = new GTKCanvas(this);

    gdk_threads_enter();
    mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_app_paintable(mainWindow, TRUE);
    gtk_window_set_title(GTK_WINDOW(mainWindow), BRANDING);
    gtk_window_set_policy(GTK_WINDOW(mainWindow), TRUE, TRUE, TRUE);
    gtk_widget_set_events(mainWindow, GDK_SUBSTRUCTURE_MASK | GDK_STRUCTURE_MASK
                          | GDK_POINTER_MOTION_MASK | GDK_BUTTON_MOTION_MASK |
                          GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK |
                          GDK_KEY_PRESS_MASK);
    gtk_widget_realize(mainWindow);
    gtk_signal_connect(GTK_OBJECT(mainWindow), "motion_notify_event",
                       GTK_SIGNAL_FUNC(mouse_move), this);
    gtk_signal_connect(GTK_OBJECT(mainWindow), "button_press_event",
                       GTK_SIGNAL_FUNC(button_down), this);
    gtk_signal_connect(GTK_OBJECT(mainWindow), "button_release_event",
                       GTK_SIGNAL_FUNC(button_up), this);
    gtk_signal_connect(GTK_OBJECT(mainWindow), "key_press_event",
                       GTK_SIGNAL_FUNC(key_press), this);
    gtk_drag_dest_set(mainWindow, GTK_DEST_DEFAULT_ALL, main_drop, 1, 
                      GDK_ACTION_COPY);
    gtk_signal_connect(GTK_OBJECT(mainWindow), "drag_data_received", 
                       GTK_SIGNAL_FUNC(drop_file), this);
    gdk_window_set_decorations(mainWindow->window, (GdkWMDecoration)0);
    gdk_threads_leave();

    initialized = false;
}

GTKWindow::~GTKWindow(void)
{
    delete m_pCanvas;
    m_pCanvas = NULL;
}

Error GTKWindow::Run(Pos &oPos)
{
    Rect oRect;
    int iMaxX, iMaxY;

    m_oWindowPos = oPos;
    m_pCanvas->Init();

    GetCanvas()->GetBackgroundRect(oRect);

    gdk_threads_enter();
    iMaxX = gdk_screen_width();
    iMaxY = gdk_screen_height();
 
    if (m_oWindowPos.x > iMaxX || m_oWindowPos.x + oRect.Width() < 0)
       m_oWindowPos.x = 0;
    if (m_oWindowPos.y > iMaxY || m_oWindowPos.y + oRect.Height() < 0)
       m_oWindowPos.y = 0;

    if (m_oWindowPos.x == -1 && m_oWindowPos.y == -1) {
        m_oWindowPos.x = iMaxX - (oRect.Width()) / 2;
        m_oWindowPos.y = iMaxY - (oRect.Height()) / 2;
    }

    gtk_widget_set_uposition(mainWindow, m_oWindowPos.x, m_oWindowPos.y);
    gtk_widget_set_usize(mainWindow, oRect.Width(), oRect.Height());
    gdk_threads_leave();

    Init();
    GdkBitmap *mask = ((GTKCanvas *)m_pCanvas)->GetMask();

    gdk_threads_enter();
    if (mask)
        gdk_window_shape_combine_mask(mainWindow->window, mask, 0, 0);
    gdk_threads_leave();

    ((GTKCanvas *)GetCanvas())->Paint(oRect);

    gdk_threads_enter();
    gtk_widget_show(mainWindow);
    gdk_flush(); 
    gtkTimer = gtk_timeout_add(250, do_timeout, this);
    gdk_threads_leave();
 
    initialized = true;

    quitLoop = false;
    while (!quitLoop) 
       sleep(1);

    oPos = m_oWindowPos;

    return kError_NoErr;
}

Error GTKWindow::VulcanMindMeld(Window *pOther)
{
    Error eRet;
    Rect  oRect;

    eRet = Window::VulcanMindMeld(pOther);
    if (IsError(eRet))
        return eRet;

    m_pCanvas->GetBackgroundRect(oRect);
    GdkBitmap *mask = ((GTKCanvas *)m_pCanvas)->GetMask();

    gdk_threads_enter();
    gtk_widget_set_usize(mainWindow, oRect.Width(), oRect.Height());
    if (mask)
        gdk_window_shape_combine_mask(mainWindow->window, mask, 0, 0);
    gdk_threads_leave();

    ((GTKCanvas *)m_pCanvas)->SetParent(this);
    m_pCanvas->Update();
    return kError_NoErr;
}

void GTKWindow::SaveWindowPos(Pos &oPos)
{
    m_oWindowPos = oPos;
}

Error GTKWindow::Close(void)
{
    Rect oRect;
    Pos oPos;
   
    if (quitLoop)
        return kError_NoErr;

    GetWindowPosition(oRect);
    oPos.x = oRect.x1;
    oPos.y = oRect.y1;
    SaveWindowPos(oPos);

    gdk_threads_enter();
    gtk_timeout_remove(gtkTimer);
    gtk_widget_destroy(mainWindow);
    gdk_flush();
    gdk_threads_leave();

    quitLoop = true;
    return kError_NoErr;
}

Error GTKWindow::Enable(void)
{
    return kError_NoErr;
}

Error GTKWindow::Disable(void)
{
    return kError_NoErr;
}

Error GTKWindow::Show(void)
{
    gdk_threads_enter();
    gtk_widget_show(mainWindow);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::Hide(void)
{
    gdk_threads_enter();
    gtk_widget_hide(mainWindow);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::SetTitle(string &oTitle)
{
    gdk_threads_enter();
    gtk_window_set_title(GTK_WINDOW(mainWindow), oTitle.c_str());
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::CaptureMouse(bool bCapture)
{
    gdk_threads_enter();
    if (bCapture)
        gdk_pointer_grab(mainWindow->window, FALSE, (GdkEventMask)
                         (GDK_SUBSTRUCTURE_MASK | GDK_STRUCTURE_MASK | 
                          GDK_POINTER_MOTION_MASK | GDK_BUTTON_MOTION_MASK | 
                          GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK), 
                          mainWindow->window, NULL, 0);
    else 
        gdk_pointer_ungrab(0);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::HideMouse(bool bHide)
{
    return kError_NoErr;
}

Error GTKWindow::SetMousePos(Pos &oPos)
{
    gdk_threads_enter();
    WarpPointer(mainWindow->window, oPos.x, oPos.y);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::GetMousePos(Pos &oPos)
{
    if (initialized) {
        gdk_threads_enter();
        gtk_widget_get_pointer(mainWindow, &oPos.x, &oPos.y);
        gdk_threads_leave();
    }
    return kError_NoErr;
}

Error GTKWindow::SetWindowPosition(Rect &oWindowRect)
{
    gdk_threads_enter();
    gdk_window_move(mainWindow->window, oWindowRect.x1, oWindowRect.y1);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::GetWindowPosition(Rect &oWindowRect) 
{
    if (!mainWindow->window)
        return kError_NoErr;
    gdk_threads_enter();
    gdk_window_get_position(mainWindow->window, &oWindowRect.x1, 
                            &oWindowRect.y1);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::Minimize(void)
{
    gdk_threads_enter();
    IconifyWindow(mainWindow->window);
    gdk_threads_leave();
    return kError_NoErr;
}

Error GTKWindow::Restore(void)
{
    return kError_NoErr;
}

void GTKWindow::DropFiles(char *filename)
{
    vector<string> oFileList;
    if (filename) {
        oFileList.push_back(string(filename));
        m_pTheme->DropFiles(&oFileList);
    }
}   
