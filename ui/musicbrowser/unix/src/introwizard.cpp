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
#include "musiccatalog.h"
#include "introwizard.h"
#include "musicsearchui.h"
#include "musicbrowserui.h"

#include "../res/wizard.xpm"
//#include "../res/relatable.xpm"

void IntroWizardUI::DeleteEvent(void)
{
    if (m_main)
        gtk_main_quit();
    m_parent->WizardClose();
}

gboolean wiz_destroy(GtkWidget *widget, IntroWizardUI *p)
{
    p->DeleteEvent();
    return FALSE;
}

void IntroWizardUI::CheckCreateProfile(void)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(profileEntry));
    if (text && strlen(text) > 0) {
        m_context->aps->CreateProfile(text);

        m_context->target->AcceptEvent(new Event(INFO_UnsignaturedTracksExist));
    }
}

static void start_search_button_event(GtkWidget *widget, IntroWizardUI *p)
{
    if (p->page == 1)
        p->GoToPage2();
    else if (p->page == 2) {
        if (p->skipRelatableTwo)
            p->GoToPage4();
        else
            p->GoToPage3();
    }
    else if (p->page == 3) {
        p->CheckCreateProfile();
        p->GoToPage4();
    }
    else if (p->searchDone) 
        gtk_widget_destroy(p->m_window);
    else if (p->searchInProgress)
        p->EndSearch();
    else  
        p->StartSearch();
}

static void search_cancel_button_event(GtkWidget *widget, IntroWizardUI *p)
{
    if (p->searchInProgress)
        p->EndSearch();
    gtk_widget_destroy(p->m_window);
}

static void search_back_event(GtkWidget *widget, IntroWizardUI *p)
{
    if (p->page == 4) {
        if (p->skipRelatableTwo)
            p->GoToPage2();
        else 
            p->GoToPage3();
    }
    if (p->page == 3)
        p->GoToPage2();
    else if (p->page == 2)
        p->GoToPage1();
}

void IntroWizardUI::UpdateEntry(void)
{
    gtk_widget_set_sensitive(textEntry, custom);
    gtk_widget_set_sensitive(browseButton, custom);
    gtk_entry_set_text(GTK_ENTRY(textEntry), searchPath.c_str());
}

static void search_select_entire(GtkWidget *widget, IntroWizardUI *p)
{
    p->SetSearchPath("/");
    p->custom = false;
    p->UpdateEntry();
}

static void search_select_home(GtkWidget *widget, IntroWizardUI *p)
{
    char *homeDir = getenv("HOME");
    if (!homeDir)
        p->SetSearchPath("/");
    else
        p->SetSearchPath(homeDir);
    p->custom = false;
    p->UpdateEntry();
}

static void search_select_share(GtkWidget *widget, IntroWizardUI *p)
{
    p->SetSearchPath("/usr/share");
    p->custom = false;
    p->UpdateEntry();
}

static void search_select_custom(GtkWidget *widget, IntroWizardUI *p)
{
    if (!p->custom)
        p->SetSearchPath("/");
    p->custom = true;
    p->UpdateEntry();
}

static void search_entry_change(GtkWidget *w, IntroWizardUI *p)
{
    char *text = gtk_entry_get_text(GTK_ENTRY(w));
    p->SetSearchPath(text);
}

static void search_browse(GtkWidget *w, IntroWizardUI *p)
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

IntroWizardUI::IntroWizardUI(FAContext *context, MusicBrowserUI *parent)
{
    m_context = context;
    m_parent = parent;
}

void IntroWizardUI::GoToPage4(void)
{
   gtk_widget_hide(page1);
   gtk_widget_hide(page2);
   gtk_widget_hide(page3);
   gtk_widget_show(page4);

   gtk_widget_set_sensitive(m_backButton, TRUE);
   gtk_label_set_text(GTK_LABEL(buttonLabel), "  Start Search >  ");

   page = 4;
}

void IntroWizardUI::GoToPage3(void)
{
   gtk_widget_hide(page4);
   gtk_widget_hide(page2);
   gtk_widget_hide(page1);
   gtk_widget_show(page3);

   gtk_widget_set_sensitive(m_backButton, TRUE); 
   gtk_label_set_text(GTK_LABEL(buttonLabel), "  Next >  ");

   page = 3;
}

void IntroWizardUI::GoToPage2(void)
{
   gtk_widget_hide(page4);
   gtk_widget_hide(page1);
   gtk_widget_hide(page3);
   gtk_widget_show(page2);

   gtk_widget_set_sensitive(m_backButton, TRUE);
   gtk_label_set_text(GTK_LABEL(buttonLabel), "  Next >  ");

   page = 2;
}

void IntroWizardUI::GoToPage1(void)
{
   gtk_widget_hide(page4);
   gtk_widget_hide(page2);
   gtk_widget_hide(page3);
   gtk_widget_show(page1);

   gtk_widget_set_sensitive(m_backButton, FALSE);
   gtk_label_set_text(GTK_LABEL(buttonLabel), "  Next >  ");

   page = 1; 
}

GtkWidget *IntroWizardUI::IntroPage(void)
{
   GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
   gtk_widget_show(vbox);

   GtkWidget *label = gtk_label_new("Welcome!");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("This wizard will help you begin organizing your music         \ncollection and get you started playing your music.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("What is 'My Music'?");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("My Music helps you organize the music you have on your\ncomputer. The My Music window is divided into two panes: \nMy Music Collection and Currently Listening To.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("My Music Collection provides a convenient, organized\nview of the music you have on your computer. It might help \nto think of this pane as a CD rack for your computer.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Currently Listening To displays a list of the songs you have \nchosen to play. In order to listen to music, all you have to\ndo is add items to the list by dragging them from the My\nMusic Collection pane on the left to the Currently Listening\nTo pane on the right.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);
	       
   label = gtk_label_new("For a more detailed explanation of how to use the My Music\nwindow, you should access the application's help system\nthrough the Help menu.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   return vbox;
}

static void opt_in_selected(GtkWidget *w, IntroWizardUI *p)
{
    p->skipRelatableTwo = false;
}

static void opt_out_selected(GtkWidget *w, IntroWizardUI *p)
{
    p->skipRelatableTwo = true;
}

GtkWidget *IntroWizardUI::RelatablePage(void)
{
   GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
   gtk_widget_show(vbox);

/*
   GtkWidget *caption = gtk_label_new("Check out Relatable's new recommendation features");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);
*/

   GtkWidget *label = gtk_label_new("This version of FreeAmp offers Relatable features that automatically recommend music playlists and streams. It's our first public test of an exciting new approach to discovering music. Relatable's system 'learns' listener preferences through FreeAmp and compares them with the preferences of like-minded listeners. Like virtual 'word of mouth', Relatable introduces you to music that people like you have enjoyed.");
   gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Private by Design");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Please note that Relatable profiles are totally anonymous and individual profiles are never shared with third parties. We don't collect any personally identifiable information. Feel free to check our code on that. Each anonymous music profile is stored on Relatable's secure servers, and we're continually adding safeguards to protect our users. To enjoy these features and help us test the system, just opt in. You will never be contacted by Relatable or anyone else (unless you ask, of course!). That's Relatable's privacy promise. If you have any questions, please contact us at info@relatable.com.");
   gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   GtkWidget *button = gtk_radio_button_new_with_label(NULL, "Enjoy Relatable Features");
   gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
   gtk_signal_connect(GTK_OBJECT(button), "clicked",
                      GTK_SIGNAL_FUNC(opt_in_selected), this);
   gtk_widget_show(button);

   button = gtk_radio_button_new_with_label(gtk_radio_button_group(
                                            GTK_RADIO_BUTTON(button)),
                                            "Cool! But I'll pass");
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), TRUE);
   skipRelatableTwo = true;
   gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 2);
   gtk_signal_connect(GTK_OBJECT(button), "clicked",
                      GTK_SIGNAL_FUNC(opt_out_selected), this);
   gtk_widget_show(button);

   return vbox;
}

static void relatable_web(GtkWidget *w, IntroWizardUI *p)
{
    LaunchBrowser("http://beta.relatable.com/");
}

GtkWidget *IntroWizardUI::RelatableTwoPage(void)
{
   GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
   gtk_widget_show(vbox);

   GtkWidget *label = gtk_label_new("Here's how to enjoy Relatable Features");
   gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Relatable automaticallly generates personalized playlists that are based on the files located on your computer and the music you listen to on the Web. The more you use the player, the better it works!");
   gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Enter a profile name in the window below.  Then start listening to music. After you have listened for a while, hit the \"Suggest\" button to view a personalized music playlist. If you'd like, check out the \"Recommended Streams\" item in the My Music tree.");
   gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Note: You can create additional music profiles by selecting 'Relatable Profiles' under the Options menu in the My Music window.  Check our the 'Help' menu for more information on all Relatable features, or drop us a note with questions to info@relatable.com");
   gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);
		  
   GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 5);
   gtk_widget_show(hbox);

   label = gtk_label_new("Profile Name: ");
   gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
   gtk_widget_show(label);

   profileEntry = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(hbox), profileEntry, FALSE, FALSE, 0);
   gtk_widget_show(profileEntry);

   GtkWidget *button = gtk_button_new_with_label(" Relatable Web Page ");
   gtk_signal_connect(GTK_OBJECT(button), "clicked",
                      GTK_SIGNAL_FUNC(relatable_web), this);
   gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
   gtk_widget_show(button);

   return vbox;
}

GtkWidget *IntroWizardUI::SearchPage(void)
{
   GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
   gtk_widget_show(vbox);

   GtkWidget *label = gtk_label_new("Search Computer For Music");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("In order to populate the My Music Collection pane\n"the_BRANDING" will search your computer for supported music\nfiles. The files will not be moved or modified during this        \nprocess. Their location will be remembered in order to\nprovide you with an organized view of your music    \ncollection.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("Where Would You Like to Look for Music?");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);

   label = gtk_label_new("By default "the_BRANDING" will search all the disk drives\non your computer for music. If you would like to limit the\nscope of the search you may do so by selecting a\ndirectory for us to search. If you wish, you may also just     \nrestrict the search to your home directory.");
   gtk_label_set_line_wrap(GTK_LABEL(label), FALSE);
   gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 2);
   gtk_widget_show(label);
	       
   GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 5);
   gtk_widget_show(hbox);

   label = gtk_label_new("Look for music in: ");
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
   gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, FALSE, 5);
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

   return vbox; 
}

void IntroWizardUI::Show(bool runMain)
{
   GtkWidget *hbox;
   GtkWidget *separator;

   searchInProgress = false;
   searchDone = false;
   searchPath = "/";
   custom = false;
   done = false;

   m_main = runMain;

   m_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(m_window), BRANDING" - Introduction");
   gtk_signal_connect(GTK_OBJECT(m_window), "destroy",
                      GTK_SIGNAL_FUNC(wiz_destroy), this);
   gtk_container_set_border_width(GTK_CONTAINER(m_window), 5);
   gtk_widget_realize(m_window);

   GtkWidget *tempvbox = gtk_vbox_new(FALSE, 5);
   gtk_container_add(GTK_CONTAINER(m_window), tempvbox);
   gtk_widget_show(tempvbox);
   
   GtkWidget *temphbox = gtk_hbox_new(FALSE, 5);
   gtk_box_pack_start(GTK_BOX(tempvbox), temphbox, TRUE, FALSE, 0);
   gtk_widget_show(temphbox);
  
   GdkPixmap *pixmap;
   GdkBitmap *mask;
   GtkStyle *style = gtk_widget_get_style(m_window);
   pixmap = gdk_pixmap_create_from_xpm_d(m_window->window, &mask, 
                                         &style->bg[GTK_STATE_NORMAL],
					 wizard);
//                                         relatable);
   GtkWidget *g_pixmap = gtk_pixmap_new(pixmap, mask);
   gtk_box_pack_start(GTK_BOX(temphbox), g_pixmap, TRUE, TRUE, 0);
   gtk_widget_show(g_pixmap);
  
   page1 = IntroPage();
   page2 = RelatablePage();
   page3 = RelatableTwoPage();
   page4 = SearchPage();

   page = 1;
   gtk_widget_hide(page4);
   gtk_widget_hide(page3);
   gtk_widget_hide(page2);
   gtk_widget_hide(page1);

   gtk_box_pack_end(GTK_BOX(temphbox), page4, FALSE, FALSE, 0);
   gtk_box_pack_end(GTK_BOX(temphbox), page3, FALSE, FALSE, 0);
   gtk_box_pack_end(GTK_BOX(temphbox), page2, FALSE, FALSE, 0);
   gtk_box_pack_end(GTK_BOX(temphbox), page1, FALSE, FALSE, 0);
   
   gtk_widget_show(page1);

   /* Control buttons at the bottom */
   separator = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(tempvbox), separator, FALSE, FALSE, 0);
   gtk_widget_show(separator);

   hbox = gtk_hbox_new(FALSE, 10);
   gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
   gtk_box_pack_start(GTK_BOX(tempvbox), hbox, FALSE, FALSE, 0);
   gtk_widget_show(hbox);

   GtkWidget *button = gtk_button_new_with_label("  Cancel  ");
   gtk_signal_connect(GTK_OBJECT(button), "clicked",
                      GTK_SIGNAL_FUNC(search_cancel_button_event), this);
   gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 5);
   gtk_widget_show(button);
   
   m_searchButton = gtk_button_new();
   buttonLabel = gtk_label_new("  Next >  ");
   gtk_misc_set_alignment(GTK_MISC(buttonLabel), 0.5, 0.5);
   gtk_container_add(GTK_CONTAINER(m_searchButton), buttonLabel);
   gtk_widget_show(buttonLabel);
   gtk_signal_connect(GTK_OBJECT(m_searchButton), "clicked",
                      GTK_SIGNAL_FUNC(start_search_button_event), this);
   gtk_box_pack_end(GTK_BOX(hbox), m_searchButton, FALSE, FALSE, 0);
   gtk_widget_show(m_searchButton);

   m_backButton = gtk_button_new_with_label("  < Back  ");
   gtk_signal_connect(GTK_OBJECT(m_backButton), "clicked",
                      GTK_SIGNAL_FUNC(search_back_event), this);
   gtk_box_pack_end(GTK_BOX(hbox), m_backButton, FALSE, FALSE, 0);
   gtk_widget_set_sensitive(m_backButton, FALSE);
   gtk_widget_show(m_backButton);

   gtk_widget_show(m_window);

   if (runMain)
       gtk_main();
}

void IntroWizardUI::Close()
{
    if (searchInProgress)
        EndSearch();
    gtk_widget_destroy(m_window);
}

IntroWizardUI::~IntroWizardUI()
{
}

void IntroWizardUI::StartSearch(void)
{
    vector<string> oPathList;
    oPathList.push_back(searchPath);
    searchInProgress = true;
    m_context->catalog->SearchMusic(oPathList);
    gtk_label_set_text(GTK_LABEL(buttonLabel), "Cancel Search");
}

void IntroWizardUI::EndSearch(void)
{
    m_context->catalog->StopSearchMusic();
    gtk_label_set_text(GTK_LABEL(buttonLabel), "Restart Search >");
    searchInProgress = false;    
}

Error IntroWizardUI::AcceptEvent(Event *e)
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

    return kError_NoErr;
}
