/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999 EMusic.com

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

#include <unistd.h>

#include "utility.h"
#include "fileselector.h"
#include "missingfileui.h"
#include "missingfile.h"

static const char *szMissingMessage1 =
  "For some reason, "the_BRANDING" cannot find the following file: ";
static const char *szMissingMessage2 =
  "Would you like to: ";

static gboolean missing_destroy(GtkWidget *w, gpointer p)
{
    return FALSE;
}

static gint missing_kill(GtkWidget *w, GdkEvent *ev, int *ret)
{
    *ret = 4;
    return FALSE;
}
    
MissingFileUI::MissingFileUI(FAContext *context, PlaylistItem *missing)
{
    m_context = context;
    m_missing = missing;
}

MissingFileUI::~MissingFileUI()
{
}

void MissingFileUI::Run(void)
{
    assert(m_missing);

    gdk_threads_enter();

    int iRet = 0;
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
                       GTK_SIGNAL_FUNC(missing_destroy), NULL);
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
                       GTK_SIGNAL_FUNC(missing_kill), &iRet);
    gtk_window_set_title(GTK_WINDOW(window), "Missing File Error");
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    GtkWidget *message = gtk_label_new(szMissingMessage1);
    gtk_label_set_line_wrap(GTK_LABEL(message), TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), message); 
    gtk_widget_show(message);

    message = gtk_label_new(m_missing->URL().c_str());
    gtk_label_set_line_wrap(GTK_LABEL(message), TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), message);
    gtk_widget_show(message);

    message = gtk_label_new(szMissingMessage2);
    gtk_label_set_line_wrap(GTK_LABEL(message), TRUE);
    gtk_container_add(GTK_CONTAINER(vbox), message);
    gtk_widget_show(message);

    gtk_widget_show(window);

    gdk_threads_leave();

    while (iRet == 0)
        usleep(20);

}
