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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utility.h"
#include "fileselector.h"
#include "missingfileui.h"
#include "missingfile.h"

static const char *szMissingMessage1 =
  "For some reason, "the_BRANDING" cannot find the following file: ";
static const char *szMissingMessage2 =
  "Would you like to: ";
static const char *szBrowseMessage =
  "Browse to find the file";
static const char *szIgnoreMessage =
  "Remove this track from the playlist";
static const char *szFindMessage =
  "Find it for me";

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

static void ignore_select(GtkWidget *w, int *func)
{
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) 
       *func = 0;
}

static void browse_select(GtkWidget *w, int *func)
{
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) 
       *func = 1;
}

static void find_select(GtkWidget *w, int *func)
{
   if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(w))) 
       *func = 2;
}

static void ok_click(GtkWidget *w, int *ret)
{
   *ret = 1;
}

void MissingFileUI::Run(void)
{
    assert(m_missing);

    gdk_threads_enter();

    int iRet = 0;
    int iFunc = 0;

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
    gtk_label_set_line_wrap(GTK_LABEL(message), FALSE);
    gtk_label_set_justify(GTK_LABEL(message), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), message, FALSE, FALSE, 2); 
    gtk_widget_show(message);

    char *filename = strdup_new(m_missing->URL().c_str());
    uint32 length = m_missing->URL().size();
    URLToFilePath(m_missing->URL().c_str(), filename, &length);
    message = gtk_label_new(filename);
    delete [] filename;
    gtk_label_set_line_wrap(GTK_LABEL(message), TRUE);
    gtk_label_set_justify(GTK_LABEL(message), GTK_JUSTIFY_LEFT);
    gtk_box_pack_start(GTK_BOX(vbox), message, FALSE, FALSE, 2);
    gtk_widget_show(message);

    message = gtk_label_new(szMissingMessage2);
    gtk_label_set_line_wrap(GTK_LABEL(message), FALSE);
    gtk_label_set_justify(GTK_LABEL(message), GTK_JUSTIFY_LEFT);
    gtk_misc_set_alignment(GTK_MISC(message), 0.0, 0.5);
    gtk_box_pack_start(GTK_BOX(vbox), message, FALSE, FALSE, 2);
    gtk_widget_show(message);

    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_container_add(GTK_CONTAINER(vbox), frame);
    gtk_widget_show(frame);

    GtkWidget *ovbox = gtk_vbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(ovbox), 5); 
    gtk_container_add(GTK_CONTAINER(frame), ovbox);
    gtk_widget_show(ovbox);

    GtkWidget *button = gtk_radio_button_new_with_label(NULL, szIgnoreMessage);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(ignore_select), &iFunc);
    if (iFunc == 0)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_box_pack_start(GTK_BOX(ovbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    button = gtk_radio_button_new_with_label(
                              gtk_radio_button_group(GTK_RADIO_BUTTON(button)),
                              szBrowseMessage);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(browse_select), &iFunc);
    if (iFunc == 1)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_box_pack_start(GTK_BOX(ovbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    button = gtk_radio_button_new_with_label(
                              gtk_radio_button_group(GTK_RADIO_BUTTON(button)),
                              szFindMessage);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(find_select), &iFunc);
    if (iFunc == 2)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
    gtk_box_pack_start(GTK_BOX(ovbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_widget_show(hbox);

    button = gtk_button_new_with_label("  OK  ");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(ok_click), &iRet);
    gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                              GTK_SIGNAL_FUNC(gtk_widget_destroy),
                              GTK_OBJECT(window));
    gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    gtk_widget_show(button);   
 
    gtk_widget_show(window);

    gdk_threads_leave();

    while (iRet == 0)
        usleep(20);

    switch (iFunc) {
        /* browse */
        case 1: {
            MissingFile *mf = new MissingFile(m_context);

            FileSelector *filesel = new FileSelector("Browse to Missing File");
            if (filesel->Run(false)) {
                string filepath = filesel->GetReturnPath();
                struct stat st;

                if (stat(filepath.c_str(), &st) == 0) 
                    mf->AcceptLocation(m_missing, filepath);
            }
              
            delete filesel;  
            delete mf;
            break; }
        /* find */
        case 2: {
            MissingFile *mf = new MissingFile(m_context);
            string search = "/";
            string newurl;

            if (IsntError(mf->FindMissingFile(m_missing, search, newurl)))
                mf->AcceptLocation(m_missing, newurl);
            delete mf;
            break; }
        /* ignore */
        default: m_context->plm->RemoveItem(m_missing);
    }
}
