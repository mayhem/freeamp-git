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

#include "fileselector.h"
#include <unistd.h>

FileSelector::FileSelector(char *windowtitle)
{
    title = windowtitle;
    returnpath = "";
    extended = true;
    ok = false;
    done = false;
}

gboolean filesel_destroy(GtkWidget *widget, gpointer p)
{
    bool runmain = (bool)p;
    if (runmain)
        gtk_main_quit();
    return FALSE;
}

void ok_internal(GtkWidget *widget, FileSelector *p)
{
    p->AddEvent();
}

void cancel_internal(GtkWidget *widget, FileSelector *p)
{
    p->CancelEvent();
}

void FileSelector::CancelEvent()
{
    gtk_widget_destroy(filesel);
    ok = false;
    done = true;
}

void FileSelector::AddEvent()
{
    GtkFileSelection *gfile = GTK_FILE_SELECTION(filesel);
    char *raw_path = NULL;
    GList *row = GTK_CLIST(gfile->file_list)->row_list;
    gint rownum = 0;
    char *temp, *path_temp;

    returnpath = gtk_file_selection_get_filename(gfile);
    path_temp = strdup(returnpath.c_str());

    char *tempdir = strrchr(path_temp, '/');
    if (!tempdir) {
        strcpy(path_temp, "./");
        path_temp = (char *)realloc(path_temp, strlen(path_temp) + 1);
    }
    else
        tempdir[0] = '\0';

    raw_path = gtk_entry_get_text(GTK_ENTRY(gfile->selection_entry));
    while (row) {
        if (GTK_CLIST_ROW(row)->state == GTK_STATE_SELECTED) {
            if (gtk_clist_get_cell_type(GTK_CLIST(gfile->file_list), rownum, 0)
                == GTK_CELL_TEXT) {
                gtk_clist_get_text(GTK_CLIST(gfile->file_list), rownum, 0, &temp);
                if (!strcmp(temp, raw_path))
                    goto next_iter;
                returnpath += "\n";
                returnpath += path_temp;
                returnpath += "/";
                returnpath += temp;
            }
        }
    next_iter:
        rownum++;
        row = g_list_next(row);
    }
    gtk_widget_destroy(GTK_WIDGET(gfile));
    free(path_temp);

    ok = true;
    done = true;
}

bool FileSelector::Run(bool runMain)
{
    if (!runMain)
        gdk_threads_enter();

    filesel = gtk_file_selection_new(title.c_str());

    gtk_window_set_modal(GTK_WINDOW(filesel), TRUE);
    gtk_signal_connect(GTK_OBJECT(filesel), "destroy",
                       GTK_SIGNAL_FUNC(filesel_destroy), (gpointer)runMain);
    gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->cancel_button),
                       "clicked", GTK_SIGNAL_FUNC(cancel_internal), this);
    gtk_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(filesel)->ok_button),
                       "clicked", GTK_SIGNAL_FUNC(ok_internal), this);

    if (extended)
        gtk_clist_set_selection_mode(GTK_CLIST(GTK_FILE_SELECTION(filesel)->
                                     file_list), GTK_SELECTION_EXTENDED);
    gtk_widget_show(filesel);

    if (runMain)
        gtk_main();
    else {
        gdk_threads_leave();
        while (!done)
            usleep(20);
    }

    if (ok)
       return true;
    return false;
}
