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

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utility.h"
#include "eventdata.h"
#include "fileselector.h"
#include "musicbrowser.h"
#include "musicsearchui.h"

gboolean search_destroy(GtkWidget *widget, gpointer p)
{
    bool quitmain = (bool)p;
    if (quitmain)
        gtk_main_quit();
    return FALSE;
}

void start_search_button_event(GtkWidget *widget, musicsearchUI *p)
{
    if (p->searchDone) 
        gtk_widget_destroy(p->m_window);
    else if (p->searchInProgress)
        p->EndSearch();
    else  
        p->StartSearch();
}

void search_cancel_button_event(GtkWidget *widget, musicsearchUI *p)
{
    if (p->searchInProgress)
        p->EndSearch();
    gtk_widget_destroy(p->m_window);
}

void musicsearchUI::UpdateEntry(void)
{
    gtk_widget_set_sensitive(textEntry, custom);
    gtk_widget_set_sensitive(browseButton, custom);
    gtk_entry_set_text(GTK_ENTRY(textEntry), searchPath.c_str());
}

void search_select_entire(GtkWidget *widget, musicsearchUI *p)
{
    p->SetSearchPath("/");
    p->custom = false;
    p->UpdateEntry();
}

void search_select_home(GtkWidget *widget, musicsearchUI *p)
{
    char *homeDir = getenv("HOME");
    if (!homeDir)
        p->SetSearchPath("/");
    else
        p->SetSearchPath(homeDir);
    p->custom = false;
    p->UpdateEntry();
}

void search_select_share(GtkWidget *widget, musicsearchUI *p)
{
    p->SetSearchPath("/usr/share");
    p->custom = false;
    p->UpdateEntry();
}

void search_select_custom(GtkWidget *widget, musicsearchUI *p)
{
    if (!p->custom)
        p->SetSearchPath("/");
    p->custom = true;
    p->UpdateEntry();
}

void search_entry_change(GtkWidget *w, musicsearchUI *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    p->SetSearchPath(text);
}

void search_browse(GtkWidget *w, musicsearchUI *p)
{
    FileSelector *filesel = new FileSelector("Select a Directory to begin searching in..");
    
    if (filesel->Run()) {
        char *filepath = filesel->GetReturnPath();
 
        struct stat st;
        stat(filepath, &st);
        if (!S_ISDIR(st.st_mode)) {
            char *temp = strrchr(filepath, '/');
            if (temp) 
                *temp = '\0';
            stat(filepath, &st);
        }
        if (S_ISDIR(st.st_mode)) {
            p->SetSearchPath(filepath);
            p->UpdateEntry();
        }
    }

    delete filesel;
}

musicsearchUI::musicsearchUI(FAContext *context)
{
    m_context = context;
}

void musicsearchUI::Show(bool runMain)
{
   GtkWidget *vbox;
   GtkWidget *hbox;
   GtkWidget *separator;

   searchInProgress = false;
   searchDone = false;
   searchPath = "/";
   custom = false;
   done = false;

   m_main = runMain;

   m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(m_window), BRANDING" - Search For Music");
   gtk_signal_connect(GTK_OBJECT(m_window), "destroy",
                      GTK_SIGNAL_FUNC(search_destroy), (void *)runMain);
   gtk_container_set_border_width(GTK_CONTAINER(m_window), 5);

   vbox = gtk_vbox_new(FALSE, 5);
   gtk_container_add(GTK_CONTAINER(m_window), vbox);
   gtk_widget_show(vbox);

   hbox = gtk_hbox_new(FALSE, 5);
   gtk_container_add(GTK_CONTAINER(vbox), hbox);
   gtk_widget_show(hbox);

   GtkWidget *label = gtk_label_new("Look for music in: ");
   gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
   gtk_widget_show(label);

   GtkWidget *optionmenu = gtk_option_menu_new();
   GtkWidget *menu = gtk_menu_new();

   GtkWidget *menuitem = gtk_menu_item_new_with_label("Entire Filesystem");
   gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                      GTK_SIGNAL_FUNC(search_select_entire), this);   
   gtk_widget_show(menuitem);
   gtk_menu_append(GTK_MENU(menu), menuitem);

   menuitem = gtk_menu_item_new_with_label("My Home Directory");
   gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                      GTK_SIGNAL_FUNC(search_select_home), this);
   gtk_widget_show(menuitem);
   gtk_menu_append(GTK_MENU(menu), menuitem);

   menuitem = gtk_menu_item_new_with_label("/usr/share");
   gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                      GTK_SIGNAL_FUNC(search_select_share), this);
   gtk_widget_show(menuitem);
   gtk_menu_append(GTK_MENU(menu), menuitem);

   menuitem = gtk_menu_item_new_with_label("Let me select a directory");
   gtk_signal_connect(GTK_OBJECT(menuitem), "activate",
                      GTK_SIGNAL_FUNC(search_select_custom), this);
   gtk_widget_show(menuitem);
   gtk_menu_append(GTK_MENU(menu), menuitem);

   gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), menu);
   gtk_box_pack_start(GTK_BOX(hbox), optionmenu, FALSE, FALSE, 0);
   gtk_widget_show(optionmenu);
   gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu), 0);

   hbox = gtk_hbox_new(FALSE, 5);
   gtk_container_add(GTK_CONTAINER(vbox), hbox);
   gtk_widget_show(hbox);

   label = gtk_label_new("Searching in: ");
   gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
   gtk_widget_show(label);

   textEntry = gtk_entry_new();
   gtk_entry_set_text(GTK_ENTRY(textEntry), "/");
   gtk_signal_connect(GTK_OBJECT(textEntry), "changed", 
                      GTK_SIGNAL_FUNC(search_entry_change), this);
   gtk_box_pack_start(GTK_BOX(hbox), textEntry, FALSE, FALSE, 0);
   gtk_widget_show(textEntry);

   browseButton = gtk_button_new_with_label(" Browse ");
   gtk_signal_connect(GTK_OBJECT(browseButton), "clicked", 
                      GTK_SIGNAL_FUNC(search_browse), this);
   gtk_box_pack_start(GTK_BOX(hbox), browseButton, FALSE, FALSE, 0);
   gtk_widget_show(browseButton);

   UpdateEntry();

   /* Control buttons at the bottom */
   separator = gtk_hseparator_new();
   gtk_container_add(GTK_CONTAINER(vbox), separator);
   gtk_widget_show(separator);

   hbox = gtk_hbox_new(FALSE, 10);
   gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
   gtk_container_add(GTK_CONTAINER(vbox), hbox);
   gtk_widget_show(hbox);

   GtkWidget *button = gtk_button_new_with_label("Quit");
   gtk_signal_connect(GTK_OBJECT(button), "clicked",
                      GTK_SIGNAL_FUNC(search_cancel_button_event), this);
   gtk_container_add(GTK_CONTAINER(hbox), button);
   gtk_widget_show(button);

   m_searchButton = gtk_button_new();
   buttonLabel = gtk_label_new("Start Search >");
   gtk_misc_set_alignment(GTK_MISC(buttonLabel), 0.5, 0.5);
   gtk_container_add(GTK_CONTAINER(m_searchButton), buttonLabel);
   gtk_widget_show(buttonLabel);
   gtk_signal_connect(GTK_OBJECT(m_searchButton), "clicked",
                      GTK_SIGNAL_FUNC(start_search_button_event), this);
   gtk_container_add(GTK_CONTAINER(hbox), m_searchButton);
   gtk_widget_show(m_searchButton);

   gtk_widget_show(m_window);

   if (runMain)
       gtk_main();
}

musicsearchUI::~musicsearchUI()
{
    if (searchInProgress)
        EndSearch();
    gtk_widget_destroy(m_window);
}

void musicsearchUI::StartSearch(void)
{
    vector<string> oPathList;
    oPathList.push_back(searchPath);
    searchInProgress = true;
    m_context->browser->SearchMusic(oPathList);
    gtk_label_set_text(GTK_LABEL(buttonLabel), "Cancel Search");
}

void musicsearchUI::EndSearch(void)
{
    m_context->browser->StopSearchMusic();
    gtk_label_set_text(GTK_LABEL(buttonLabel), "Restart Search >");
    searchInProgress = false;    
}

int32 musicsearchUI::AcceptEvent(Event *e)
{
    switch (e->Type()) {
        case INFO_SearchMusicDone: {
            if (searchInProgress == false)
                break;
            searchInProgress = false;
            searchDone = false;
            gdk_threads_enter();
            UpdateEntry();
            gtk_label_set_text(GTK_LABEL(buttonLabel), "Start Search >");
            gdk_threads_leave();
            break;
        }
        case INFO_BrowserMessage: {
            gdk_threads_enter();
            gtk_entry_set_text(GTK_ENTRY(textEntry), ((BrowserMessageEvent *)e)->GetBrowserMessage());
            gdk_threads_leave();
            break;
        }
    }
}
