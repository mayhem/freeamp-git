/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999-2000 EMusic.com

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

#include <gtk/gtk.h>

#include <iostream>
using namespace std;

#include "utility.h"
#include "gtkmusicbrowser.h"
#include "infoeditor.h"
#include "fileselector.h"
#include "eventdata.h"
#include "player.h"
#include "musicbrowserui.h"
#include "gtkmessagedialog.h"

#include "cdaudio.h"
#include "cdpmo.h"

void GTKMusicBrowser::AddStreamToFavs(void)
{
    PlaylistItem *stream = mbSelection->track;
    m_context->catalog->WriteMetaDataToDatabase(stream->URL().c_str(),
                                                stream->GetMetaData(),
                                                kTypeStream);
    m_context->catalog->AddStream(stream->URL().c_str());
}

static gboolean add_new_destroy(GtkWidget *w, gpointer p)
{
    gtk_main_quit();
    return FALSE;
}

static gint add_new_kill(GtkWidget *w, GdkEvent *e, int *ret)
{
    *ret = 2;
    return FALSE;
}

static void add_new_ok(GtkWidget *w, int *ret)
{
    *ret = 1;
}

static void add_new_cancel(GtkWidget *w, int *ret)
{
    *ret = 2;
}

static void add_new_title(GtkWidget *w, string *title)
{
    *title = gtk_entry_get_text(GTK_ENTRY(w));
}

static void add_new_url(GtkWidget *w, string *url)
{
    *url = gtk_entry_get_text(GTK_ENTRY(w));
}

void GTKMusicBrowser::AddNewStream(void)
{
    int iRet = 0;

    string url, title;

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
                       GTK_SIGNAL_FUNC(add_new_destroy), NULL);
    gtk_signal_connect(GTK_OBJECT(window), "delete_event",
                       GTK_SIGNAL_FUNC(add_new_kill), &iRet);
    gtk_window_set_title(GTK_WINDOW(window), "Add a New Stream");
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    GtkWidget *table = gtk_table_new(8, 2, FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 0);
    gtk_widget_show(table);

    GtkWidget *label = gtk_label_new("Title:");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_FILL, GTK_FILL,
                     10, 1);
    gtk_widget_show(label);

    GtkWidget *titleEntry = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), titleEntry, 1, 2, 0, 1);
    gtk_signal_connect(GTK_OBJECT(titleEntry), "changed",
                       GTK_SIGNAL_FUNC(add_new_title), &title);
    gtk_widget_show(titleEntry);

    label = gtk_label_new("URL:");
    gtk_misc_set_alignment(GTK_MISC(label), (gfloat)1.0, (gfloat)0.5);
    gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_FILL, GTK_FILL,
                     10, 1);
    gtk_widget_show(label);

    GtkWidget *urlEntry = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), urlEntry, 1, 2, 1, 2);
    gtk_signal_connect(GTK_OBJECT(urlEntry), "changed",
                       GTK_SIGNAL_FUNC(add_new_url), &url);
    gtk_widget_show(urlEntry);

    GtkWidget *separator = gtk_hseparator_new();
    gtk_container_add(GTK_CONTAINER(vbox), separator);
    gtk_widget_show(separator);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 10);
    gtk_container_set_border_width(GTK_CONTAINER(hbox), 5);
    gtk_container_add(GTK_CONTAINER(vbox), hbox);
    gtk_widget_show(hbox);

    GtkWidget *button = gtk_button_new_with_label("OK");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(add_new_ok), &iRet);
    gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                              GTK_SIGNAL_FUNC(gtk_widget_destroy),
                              GTK_OBJECT(window));
    gtk_container_add(GTK_CONTAINER(hbox), button);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Cancel");
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(add_new_cancel), &iRet);
    gtk_signal_connect_object(GTK_OBJECT(button), "clicked",
                              GTK_SIGNAL_FUNC(gtk_widget_destroy),
                              GTK_OBJECT(window));
    gtk_container_add(GTK_CONTAINER(hbox), button);
    gtk_widget_show(button);

    gtk_widget_show(window);

    gtk_main();

    if (iRet != 1)
        return;

    if (!url.size() || !title.size())
        return;

    PlaylistItem *newitem = new PlaylistItem;
    MetaData metadata;

    newitem->SetURL(url.c_str());
    metadata.SetTitle(title.c_str());
    newitem->SetMetaData(&metadata);

    m_context->catalog->WriteMetaDataToDatabase(newitem->URL().c_str(),
                                                newitem->GetMetaData(),
                                                kTypeStream);
    m_context->catalog->AddStream(newitem->URL().c_str());
}

void GTKMusicBrowser::EjectCD(void)
{
    Registry *pmoRegistry = m_context->player->GetPMORegistry();
    RegistryItem *pmo_item = NULL;
    int32 i = 0;

    while (NULL != (pmo_item = pmoRegistry->GetItem(i++))) {
        if (!strcmp("cd.pmo", pmo_item->Name())) {
            break;
        }
    }

    if (!pmo_item)
        return;

    PhysicalMediaOutput *pmo;
    pmo = (PhysicalMediaOutput *)pmo_item->InitFunction()(m_context);
    pmo->SetPropManager((Properties *)(m_context->player));

    pmo->Init(NULL);
    ((CDPMO*)pmo)->Eject();

    delete pmo;
}

void GTKMusicBrowser::PlayMenu()
{
    if (pauseState)
        m_context->target->AcceptEvent(new Event(CMD_Play));
    else
        m_context->target->AcceptEvent(new Event(CMD_Pause));
}

void GTKMusicBrowser::StopMenu()
{
    m_context->target->AcceptEvent(new Event(CMD_Stop));
}

void GTKMusicBrowser::NextMenu(void)
{
    m_context->target->AcceptEvent(new Event(CMD_NextMediaPiece));
}

void GTKMusicBrowser::PrevMenu(void)
{
    m_context->target->AcceptEvent(new Event(CMD_PrevMediaPiece));
}

void GTKMusicBrowser::SetRepeat(int numrepeat)
{
   iSetRepeatMode = true;
   switch(numrepeat) {
       case 0: {
           m_plm->SetRepeatMode(kPlaylistMode_RepeatNone);
           break; }
       case 1: {
           m_plm->SetRepeatMode(kPlaylistMode_RepeatOne);
           break; }
       case 2: {
           m_plm->SetRepeatMode(kPlaylistMode_RepeatAll);
           break; }
   }
}

void GTKMusicBrowser::SetRepeatType(RepeatMode mode)
{
    GtkWidget *w;

    if (mode == kPlaylistMode_RepeatNone)
        w = gtk_item_factory_get_widget(menuFactory,
                                        "/Controls/Repeat No Tracks");
    else if (mode == kPlaylistMode_RepeatOne)
        w = gtk_item_factory_get_widget(menuFactory,
                                        "/Controls/Repeat One Track");
    else
        w = gtk_item_factory_get_widget(menuFactory, 
                                        "/Controls/Repeat All Tracks");
    if (GTK_IS_CHECK_MENU_ITEM(GTK_OBJECT(w))) 
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(w), TRUE);
}

void GTKMusicBrowser::SetShuffleType(bool shuffled)
{
    iSetShuffleMode = false;
    GtkWidget *w;
    if (shuffled)
        w = gtk_item_factory_get_widget(menuFactory, 
                                       "/Controls/Play Tracks in Random Order");
    else
        w = gtk_item_factory_get_widget(menuFactory, 
                                       "/Controls/Play Tracks in Normal Order");
    if (GTK_IS_CHECK_MENU_ITEM(GTK_OBJECT(w)))
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(w), TRUE);
}

void GTKMusicBrowser::ShowOptions(int page)
{
    m_context->target->AcceptEvent(new ShowPreferencesEvent(page));
}

void GTKMusicBrowser::AddFileCMD()
{
    bool playNow = true;
    m_context->prefs->GetPlayImmediately(&playNow);

    FileSelector *filesel = new FileSelector("Select a file to play");
    if (filesel->Run(false)) {
        char *returnpath = filesel->GetReturnPath();
        char *ext = m_context->player->GetExtension(returnpath);
        if (ext) {
            uint32 length = strlen(returnpath) + 10;
            char *tempurl = new char[length];
            if (IsntError(FilePathToURL(returnpath, tempurl, &length))) {

                if (playNow)
                    DeleteListEvent();

                if (m_plm->IsSupportedPlaylistFormat(ext)) {
                    string tobeloaded = tempurl;
                    LoadPlaylist(tobeloaded);
                }
                else {
                    char *filereturn = strdup_new(filesel->GetReturnPath());
                    if (filereturn) {
                        char *temp;
                        char *first= strtok(filereturn, "\n");

                        while ((temp = strtok(NULL, "\n"))) {
                            AddTrackPlaylistEvent(temp);
                            m_currentindex++;
                        }
                        AddTrackPlaylistEvent(first);
                    }
                    delete [] filereturn;
                }
            }
            delete ext;
            delete [] tempurl;
        }

        if (playNow) {
            m_currentindex = 0;
            PlayEvent();
        }
    }
    delete filesel;
}

void GTKMusicBrowser::CreateExpanded(void)
{
    GtkWidget *browserlabel;
    GtkWidget *browservbox;
    GtkWidget *hbox;

    if (m_browserCreated)
        return;

    m_browserCreated = true;

    masterBrowserBox = gtk_vbox_new(FALSE, 0);
    gtk_paned_pack1(GTK_PANED(masterBox), masterBrowserBox, TRUE, TRUE);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(masterBrowserBox), hbox, FALSE, FALSE, 5);

    browserlabel = gtk_label_new("My Music:");
    gtk_box_pack_start(GTK_BOX(hbox), browserlabel, FALSE, FALSE, 5);

    browservbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(masterBrowserBox), browservbox);
    gtk_container_set_border_width(GTK_CONTAINER(browservbox), 5);

    musicBrowserWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(musicBrowserWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(browservbox), musicBrowserWindow, TRUE, TRUE, 0);
    gtk_widget_set_usize(musicBrowserWindow, 200, 200);

    CreateTree();

    gtk_widget_show_all(masterBrowserBox);
}

void set_label_menu(GtkWidget *w, gchar *newtitle)
{
    if (GTK_IS_ACCEL_LABEL(GTK_OBJECT(w))) {
        gtk_label_set_text(&(GTK_ACCEL_LABEL(w)->label), newtitle);
    }
}

void GTKMusicBrowser::UpdatePlayPause(void)
{
    GtkWidget *w = gtk_item_factory_get_widget(menuFactory, "/Controls/Play");
    if (pauseState) 
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu, (gpointer)"Pause");
    else
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu, (gpointer)"Play");
    w = gtk_item_factory_get_widget(menuFactory, "/Controls/Stop");
    if (stopState)
        gtk_widget_set_sensitive(w, FALSE);
    else
        gtk_widget_set_sensitive(w, TRUE);
}

void GTKMusicBrowser::ExpandCollapseEvent(void)
{
    string title = string(BRANDING);

    if (m_state == kStateCollapsed) {
        CreateExpanded();
        m_state = kStateExpanded;
        if (lastPanedPosition != -1) {
            gtk_paned_set_position(GTK_PANED(masterBox), lastPanedPosition);
            gtk_paned_set_handle_size(GTK_PANED(masterBox), lastPanedHandle);
        }
        title += string(" - My Music: ");
        GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                   "/View/View Playlist Only");
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu, (gpointer)"View Playlist Only");
        UpdateCatalog();
    }
    else {
        m_state = kStateCollapsed;
        lastPanedPosition = ((GtkPaned *)masterBox)->child1_size;
        lastPanedHandle = ((GtkPaned *)masterBox)->handle_size;
        gtk_paned_set_position(GTK_PANED(masterBox), 0);
        gtk_paned_set_handle_size(GTK_PANED(masterBox), 0);
        title += string(" - Playlist Editor: ");
        GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                   "/View/View Playlist Only");
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu, (gpointer)"View My Music");
    }

    if (master)
        title += string("Current listening list");
    else {
        if (m_bCDMode)
            title += string("New Audio CD");
        else if (m_currentListName.length() == 0)
            title += string("New Playlist");
        else
            title += string("Editing playlist ") + m_currentListName;
    }

    gtk_window_set_title(GTK_WINDOW(musicBrowser), title.c_str());
}

void GTKMusicBrowser::ToggleVisEvent(void)
{
    gdk_threads_leave();
    Close();
    gdk_threads_enter();
}

static gint toggle_vis_delete_event(GtkWidget *w, GdkEvent *e, 
                                    GTKMusicBrowser *p)
{
    p->ToggleVisEvent();
    return TRUE;
}

void GTKMusicBrowser::SetStatusText(const char *text)
{
    if (statusContext > 0) 
        gtk_statusbar_pop(GTK_STATUSBAR(statusBar), statusContext);
    else
        statusContext = 1;

    gtk_statusbar_push(GTK_STATUSBAR(statusBar), 1, text);
}

void GTKMusicBrowser::CreateNewEditor(char *playlisturl, bool cd_mode)
{
    string newURL;
    if (playlisturl) {
        if (!strncmp("file://", playlisturl, 7))
            newURL = playlisturl;
        else
            newURL = string("file://") + string(playlisturl);
    }
    else
        newURL = "";
    parentUI->CreateNewEditor(newURL, cd_mode);
}

GtkWidget *GTKMusicBrowser::NewPixmap(char **data)
{
    GtkWidget *wpixmap;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowser);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowser->window, &mask, 
                                          &style->bg[GTK_STATE_NORMAL], data);

    wpixmap = gtk_pixmap_new(pixmap, mask);
    return wpixmap;
}

void GTKMusicBrowser::CreatePlaylist(void)
{
    GtkWidget *vbox;
    GtkWidget *playlistwindow;

    musicBrowser = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    string titlestr = string(BRANDING) + string(" - My Music: ");
    if (master)
        titlestr += string("Current listening list");
    else {
        if (m_bCDMode)
            titlestr += string("New Audio CD");
        else if (m_currentListName.length() == 0)
            titlestr += string("New Playlist");
        else
            titlestr += string("Editing playlist ") + m_currentListName;
    }
    gtk_window_set_title(GTK_WINDOW(musicBrowser), titlestr.c_str());
    gtk_window_set_policy(GTK_WINDOW(musicBrowser), TRUE, TRUE, FALSE);
    gtk_signal_connect(GTK_OBJECT(musicBrowser), "delete_event",
                       GTK_SIGNAL_FUNC(toggle_vis_delete_event), this);
    gtk_container_set_border_width(GTK_CONTAINER(musicBrowser), 0);
    gtk_widget_realize(musicBrowser);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(musicBrowser), vbox);
    gtk_widget_show(vbox);

    statusBar = gtk_statusbar_new();
    
    gtk_box_pack_end(GTK_BOX(vbox), statusBar, FALSE, TRUE, 0);
    gtk_widget_show(statusBar);

    CreateMenu(vbox);

    CreateToolbar(vbox);

    GtkWidget *separator = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, TRUE, 0);
    gtk_widget_show(separator);

    masterBox = gtk_hpaned_new();
    gtk_box_pack_start(GTK_BOX(vbox), masterBox, TRUE, TRUE, 0);
    gtk_widget_show(masterBox);

    masterPlaylistBox = gtk_vbox_new(FALSE, 0);
    gtk_paned_pack2(GTK_PANED(masterBox), masterPlaylistBox, TRUE, TRUE);
    gtk_widget_show(masterPlaylistBox);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(masterPlaylistBox), hbox, FALSE, FALSE, 5);
    gtk_widget_show(hbox);

    GtkWidget *playlistLabel;

    if (master)
        playlistLabel = gtk_label_new("Currently listening to:");
    else {
        if (m_bCDMode) {
            playlistLabel = gtk_label_new("New Audio CD:");
        }
        else {
            string labelstr = string("Editing playlist: ");
            if (m_currentListName.length() == 0) 
                labelstr += string("New Playlist");
            else
                labelstr += m_currentListName;
            playlistLabel = gtk_label_new(labelstr.c_str());
        }
    }
    gtk_box_pack_start(GTK_BOX(hbox), playlistLabel, FALSE, FALSE, 5);
    gtk_widget_show(playlistLabel);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(masterPlaylistBox), hbox, TRUE, TRUE, 0);
    gtk_container_set_border_width(GTK_CONTAINER(masterPlaylistBox), 5);
    gtk_widget_show(hbox);
 
    playlistwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(playlistwindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(hbox), playlistwindow, TRUE, TRUE, 5);
    gtk_widget_set_usize(playlistwindow, 200, 200);
    gtk_widget_show(playlistwindow);

    normStyle = gtk_style_copy(gtk_widget_get_style(musicBrowser));
    boldStyle = gtk_style_copy(normStyle);

    GdkFont *font =
            gdk_font_load("-adobe-helvetica-bold-r-normal--*-120-*-*-*-*-*-*");     if (!font)
        font = gdk_font_load("fixed");
    gdk_font_unref(boldStyle->font);
    boldStyle->font = font;
    gdk_font_ref(boldStyle->font);

    GdkColor red;
    GdkColor green;

    red.red     = 56000;
    red.green   = 0;
    red.blue    = 0;
    green.red   = 0;
    green.green = 56000;
    green.blue  = 000;

    greenStyle = gtk_style_copy(normStyle);
    greenStyle->fg[GTK_STATE_NORMAL] = green;
    redStyle = gtk_style_copy(normStyle);
    redStyle->fg[GTK_STATE_NORMAL] = red;

    CreatePlaylistList(playlistwindow);

    SetClickState(kContextNone);
    gtk_widget_show(musicBrowser);

    m_state = kStateCollapsed;
}

void GTKMusicBrowser::DeleteListEvent(void)
{
    if (m_currentindex == kInvalidIndex)
        return;

    m_plm->RemoveAll();
    if (master)
        m_context->target->AcceptEvent(new Event(CMD_Stop));
    m_currentindex = kInvalidIndex;
}

void GTKMusicBrowser::SetClickState(ClickState newState)
{
    if (m_bCDMode) {
        GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                        "/File/Create New Audio CD");
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                              (gpointer)"Burn Audio CD");
    }

    m_clickState = newState;
    if (m_clickState == kContextPlaylist) {
        gtk_widget_set_sensitive(toolRemove, TRUE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Add Stream to Favorites"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Remove Items from My Music"), FALSE);
        if (m_currentindex != 0) {
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Move Up"), TRUE);
            gtk_widget_set_sensitive(toolUp, TRUE);
        }
        else {
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Move Up"), FALSE);
            gtk_widget_set_sensitive(toolUp, FALSE);
        }
        if (m_currentindex != m_plm->CountItems() - 1) {
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Move Down"), TRUE);
            gtk_widget_set_sensitive(toolDown, TRUE);
        }
        else {
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Move Down"), FALSE);
            gtk_widget_set_sensitive(toolDown, FALSE);
        }
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory, 
                                 "/Edit/Edit Info"), TRUE);

        if (musicBrowserTree)
            gtk_clist_unselect_all(GTK_CLIST(musicBrowserTree));
 
        GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Edit Info");
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu, 
                              (gpointer)"Edit Track Info");

    }
    else if (m_clickState == kContextBrowser) {
        gtk_widget_set_sensitive(toolUp, FALSE);
        gtk_widget_set_sensitive(toolDown, FALSE);
        gtk_widget_set_sensitive(toolRemove, FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Add Stream to Favorites"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Remove Items from My Music"), TRUE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Move Up"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Move Down"), FALSE);
        gtk_clist_unselect_all(GTK_CLIST(playlistList));

        if (m_mbState == kTreePlaylist) {
            GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Edit Info");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Edit Playlist");
            w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Add Items to Playlist");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Add Playlist to Playlist");
            w = gtk_item_factory_get_widget(menuFactory,
                                            "/Edit/Remove Items from My Music");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                  (gpointer)"Remove Playlist from My Music");
        }
        else if (m_mbState == kTreeTrack) {
            GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Edit Info");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Edit Track Info");
            w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Add Items to Playlist");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Add Track to Playlist");
            w = gtk_item_factory_get_widget(menuFactory,
                                            "/Edit/Remove Items from My Music");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                  (gpointer)"Remove Track from My Music");
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Edit Info"), TRUE);
        }
        else if (m_mbState == kTreeStream) {
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Add Stream to Favorites"), TRUE);
            GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Edit Info");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Edit Stream Info");
            w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Add Items to Playlist");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Add Stream to Playlist");
            w = gtk_item_factory_get_widget(menuFactory,
                                            "/Edit/Remove Items from My Music");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                  (gpointer)"Remove Stream from My Music");
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Edit Info"), TRUE);
        }
        else if (m_mbState == kTreeCD) {
            GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Edit Info");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Edit Info");
            w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Add Items to Playlist");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Add Track to Playlist");
            w = gtk_item_factory_get_widget(menuFactory,
                                            "/Edit/Remove Items from My Music");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                  (gpointer)"Remove Items from My Music");
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Edit Info"), FALSE);
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                    "/Edit/Remove Items from My Music"), FALSE);
        }
        else {
            GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Edit Info");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Edit Info");
            w = gtk_item_factory_get_widget(menuFactory,
                                                 "/Edit/Add Items to Playlist");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                          (gpointer)"Add Items to Playlist");
            w = gtk_item_factory_get_widget(menuFactory,
                                            "/Edit/Remove Items from My Music");
            gtk_container_foreach(GTK_CONTAINER(w), set_label_menu,
                                  (gpointer)"Remove Items from My Music");
            gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                     "/Edit/Edit Info"), FALSE);
        }
    }
    else {
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Edit Info"), FALSE);
        gtk_widget_set_sensitive(toolUp, FALSE);
        gtk_widget_set_sensitive(toolDown, FALSE);
        gtk_clist_unselect_all(GTK_CLIST(playlistList));
        if (musicBrowserTree)
            gtk_clist_unselect_all(GTK_CLIST(musicBrowserTree));
    }

    if (!master) {
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Play"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Stop"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Next Track"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Previous Track"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Repeat No Tracks"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Repeat One Track"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Controls/Repeat All Tracks"), FALSE);
    }
}

void GTKMusicBrowser::DeleteEvent(void)
{
    bool stopped = false;
    uint32 deleteindex = m_currentindex;
    if (master) {
        if (m_currentindex == m_playingindex) {
            m_context->target->AcceptEvent(new Event(CMD_Stop));
            stopped = true;
        }
        else if (m_playingindex > m_currentindex)
            m_playingindex--;

        if (stopped && (m_plm->CountItems() - 1 > deleteindex)) {
            m_plm->SetCurrentIndex(m_currentindex + 1);
            m_context->target->AcceptEvent(new Event(CMD_Play));
        }
    }
    m_plm->RemoveItem(deleteindex);
}

void GTKMusicBrowser::MoveUpEvent(void)
{
    if (m_currentindex == 0 || m_currentindex == kInvalidIndex)
        return;
    m_plm->SwapItems(m_currentindex, m_currentindex - 1);
    m_currentindex--;
    UpdatePlaylistList();
}

void GTKMusicBrowser::MoveDownEvent(void)
{
    if (m_currentindex == m_plm->CountItems() - 1 || 
        m_currentindex == kInvalidIndex)
        return;
    m_plm->SwapItems(m_currentindex, m_currentindex + 1);
    m_currentindex++;
    UpdatePlaylistList();
}

void GTKMusicBrowser::MoveItemEvent(int source, int dest)
{
    m_plm->MoveItem(source, dest);
}

void GTKMusicBrowser::AddTrackPlaylistEvent(char *path)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;

    char *tempurl;
    bool additReally = false;
    bool needToDelete = false;

    if ((tempurl = strstr(path, "http://")))
        additReally = true;
    else if ((tempurl = strstr(path, "file://")))
        additReally = true;
    else if ((tempurl = strstr(path, "rtp://")))
        additReally = true;
    else {
        tempurl = new char[_MAX_PATH];
        uint32 length = _MAX_PATH; 
        if (IsntError(FilePathToURL(path, tempurl, &length))) {
            additReally = true;
            needToDelete = true;
        }
    }

    if (additReally)
        m_plm->AddItem(tempurl, m_currentindex);

    if (needToDelete)
        delete [] tempurl;
}

void GTKMusicBrowser::AddTrackPlaylistEvent(PlaylistItem *newitem)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    m_plm->AddItem(newitem, m_currentindex, true);
}

void GTKMusicBrowser::AddTracksPlaylistEvent(vector<PlaylistItem *> *newlist,
                                             bool end, bool forcePlay, 
                                             bool forceNoPlay)
{
    bool play = false;
    int playPos = 0;

    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    else if (end)
        m_currentindex = m_plm->CountItems();

    if (master && (m_plm->CountItems() == 0)) {
        bool playNow = false;
        m_context->prefs->GetPlayImmediately(&playNow);

        if (playNow)
            play = true;
    }
    else 
        playPos = m_currentindex;

    m_plm->AddItems(newlist, m_currentindex, true);

    if (forceNoPlay)
        play = false;
    if (forcePlay)
        play = true;

    if (play) {
        m_currentindex = playPos;
        PlayEvent();
    }
}

void GTKMusicBrowser::AddTracksDoubleClick(vector<PlaylistItem *> *newlist)
{
    bool playNow = false;

    m_context->prefs->GetPlayImmediately(&playNow);

    if (playNow) {
        DeleteListEvent();
        m_currentindex = 0;
        AddTracksPlaylistEvent(newlist, true, true);
    }
    else
        AddTracksPlaylistEvent(newlist, true);
}

void GTKMusicBrowser::PlayEvent(void)
{
    m_plm->SetCurrentIndex(m_currentindex);
    m_context->target->AcceptEvent(new Event(CMD_Play));
}

void GTKMusicBrowser::StartMusicSearch(bool runMain, bool intro)
{
    parentUI->StartSearch(runMain, intro);
}

void GTKMusicBrowser::SortPlaylistEvent(PlaylistSortKey order, PlaylistSortType
                                        type)
{
    if (order == kPlaylistSortKey_LastKey) {
        if (type == kPlaylistSortType_Ascending)
            m_plm->SetShuffleMode(true);
        else
            m_plm->SetShuffleMode(false);
    }
    else
        m_plm->Sort(order, type);
}

void GTKMusicBrowser::PopUpInfoEditor(PlaylistItem *editee)
{
    if ((m_currentindex == kInvalidIndex) && editee == NULL)
        return;
   
    infoeditorUI *infoedit;
    if (editee == NULL)
        infoedit = new infoeditorUI(m_context, m_plm->ItemAt(m_currentindex));
    else
        infoedit = new infoeditorUI(m_context, editee);

    infoedit->DisplayInfo();
}

void GTKMusicBrowser::SaveCurrentPlaylist(char *path)
{
    if (path != NULL)
        m_currentListName = path;

    if (m_currentListName.length() == 0)
        return;

    char *ext = strrchr(m_currentListName.c_str(), '.');
    if (ext)
        ext = ext + 1;
    Error result = kError_NoErr;
    int i = 0;
    bool found = false;
    PlaylistFormatInfo format;
    while (ext && result == kError_NoErr) {
        result = m_plm->GetSupportedPlaylistFormats(&format, i);
        if (!strcmp(ext, format.GetExtension())) {
            found = true;
            break;
        }
        i++;
    }
    if (!found) {
        m_plm->GetSupportedPlaylistFormats(&format, 0);
        m_currentListName += "." ;
        m_currentListName += format.GetExtension();
    }
   
    if (!strncmp("file://", m_currentListName.c_str(), 7)) {
        m_plm->WritePlaylist(m_currentListName.c_str(), &format);
        m_context->catalog->AddPlaylist(m_currentListName.c_str());
    }
    else {
        uint32 urlLength = m_currentListName.length() + 20;
        char *writeURL = new char[urlLength];
        Error err = FilePathToURL(m_currentListName.c_str(), writeURL, &urlLength);
        if (IsntError(err)) {
            m_plm->WritePlaylist(writeURL, &format);
            m_context->catalog->AddPlaylist(writeURL);
        }
        delete [] writeURL;
    }
}

void GTKMusicBrowser::LoadPlaylist(string &oPlaylist)
{
    if (oPlaylist == m_currentListName)
        return;

    if (oPlaylist.length() == 0)
        return;

    if (m_currentListName.length() != 0 && !master)
        SaveCurrentPlaylist(NULL);

    if (!strncmp("file://", oPlaylist.c_str(), 7)) {
        m_plm->ReadPlaylist(oPlaylist.c_str());
        m_currentListName = oPlaylist;
    }
    else {
        uint32 length = _MAX_PATH;
        char *PlaylistURL = new char[length];
        if (IsntError(FilePathToURL(oPlaylist.c_str(), PlaylistURL, &length))) {
            m_plm->ReadPlaylist(PlaylistURL);
            m_currentListName = PlaylistURL;
        }
        delete [] PlaylistURL;
    }
}

void GTKMusicBrowser::ImportPlaylist(char *path)
{
    if (!path)
        return;
    uint32 length = strlen(path) + 10;
    char *url = new char[length];
    if (IsntError(FilePathToURL(path, url, &length)))
        m_context->catalog->AddPlaylist(path);
    delete [] url;
}

void GTKMusicBrowser::ReadPlaylist(char *path, vector<PlaylistItem *> *plist)
{
    m_plm->ReadPlaylist(path, plist);
}

GTKMusicBrowser::GTKMusicBrowser(FAContext *context, MusicBrowserUI *masterUI,
                                 string playlistURL, bool cdCreationMode)
{
    m_context = context;
    m_initialized = false;
    isVisible = false;
    m_currentindex = 0;
    m_currentListName = "";
    m_state = kStateCollapsed;
    statusContext = 0;
    playlistList = NULL;
    m_musicCatalog = NULL;
    m_browserCreated = false;
    lastPanedPosition = -1;
    lastPanedHandle = -1;
    pauseState = 0;
    stopState = 1;
    musicBrowserTree = NULL;
    m_playingindex = kInvalidIndex;
    iSetRepeatMode = false;
    iSetShuffleMode = false;
    CD_DiscID = 0;
    CD_numtracks = 0;
    scheduleCDredraw = false;
    CDTracks = new vector<PlaylistItem *>;
    m_bIgnoringMusicCatalogMessages = false;
    m_bCDMode = cdCreationMode;
    ice_timer_started = false;

    parentUI = masterUI;
 
    if (playlistURL.find("currentlist.m3u") <= playlistURL.length()) {
        m_plm = context->plm;
        master = true;
    }
    else {
        m_plm = new PlaylistManager(context);
        m_plm->SetActivePlaylist(kPlaylistKey_ExternalPlaylist);
        master = false;
    }

    if (master) {
        bool saveOnExit;
        m_context->prefs->GetSaveCurrentPlaylistOnExit(&saveOnExit);

        if (saveOnExit) {
            LoadPlaylist(playlistURL);
            uint32 pos = 0; 
            m_context->prefs->GetSavedPlaylistPosition(&pos);

            m_plm->SetCurrentIndex(pos);   
        }
        else
            m_currentListName = playlistURL;
    }
    else if (playlistURL != "")
        LoadPlaylist(playlistURL);
}

GTKMusicBrowser::~GTKMusicBrowser(void)
{
    if (m_initialized)
        gtk_widget_destroy(musicBrowser);
}

void GTKMusicBrowser::ShowPlaylist(void)
{
    gdk_threads_enter();
    isVisible = true;
    if (m_initialized) 
        gtk_widget_show(musicBrowser);
    else {
        CreatePlaylist();
        m_initialized = true;
    }

    if (m_state == kStateExpanded)
        ExpandCollapseEvent();
    UpdatePlaylistList();

    gdk_threads_leave();
}

void GTKMusicBrowser::ShowMusicBrowser(void)
{
    bool first_time = false;
    
    gdk_threads_enter();
    isVisible = true;
    if (m_initialized)
        gtk_widget_show(musicBrowser);
    else {
        first_time = true;
        CreatePlaylist();
        m_initialized = true;
    }
    if (!master) {
        if (m_state == kStateCollapsed)
            ExpandCollapseEvent();
    }
    else {  
         bool viewMusicBrowser = true;

         m_context->prefs->GetViewMusicBrowser(&viewMusicBrowser);
         
         if (viewMusicBrowser && m_state == kStateCollapsed)
	     ExpandCollapseEvent();
         else if (!viewMusicBrowser && m_state == kStateExpanded) 
             ExpandCollapseEvent();
	 else if (m_state == kStateCollapsed && first_time) {
	     m_state = kStateExpanded;
	     ExpandCollapseEvent();
         }
    }

    SetToolbarType();
    if (scheduleCDredraw)
        RegenerateCDTree();
    UpdatePlaylistList();

    gdk_threads_leave();
}

void GTKMusicBrowser::Close(bool inMain)
{
    gdk_threads_enter();
    isVisible = false;

    if (master) {
        bool saveOnExit = false;
        m_context->prefs->GetSaveCurrentPlaylistOnExit(&saveOnExit);

        if (saveOnExit && m_plm) {
            SaveCurrentPlaylist(NULL);
            m_context->prefs->SetSavedPlaylistPosition(m_plm->GetCurrentIndex());
        }
    }
    else {
        if (m_plm && m_currentListName.length() == 0 && m_plm->CountItems() > 0) 
        {
            gdk_window_raise(musicBrowser->window);
            GTKMessageDialog oBox;
            string oMessage = string("Do you want to save this playlist to disk? ");

            if (oBox.Show(oMessage.c_str(), "Save Confirmation", kMessageYesNo,
                          inMain) == kMessageReturnYes) {

                FileSelector *filesel = new FileSelector("Save This Playlist to Disk");
                if (filesel->Run())
                    m_currentListName = filesel->GetReturnPath();

                delete filesel;
            }
        }

        if (m_plm)
            SaveCurrentPlaylist(NULL);
    }
 
    if (m_initialized) {
        gtk_widget_hide(musicBrowser);
        if (!master && m_plm) {
            delete m_plm; 
            m_plm = NULL;
        }
        if (!master && inMain)
            parentUI->WindowClose(this);
        
        if (master)
            m_context->prefs->SetViewMusicBrowser(m_state == kStateExpanded);
    }

    gdk_threads_leave();
}

Error GTKMusicBrowser::AcceptEvent(Event *e)
{
    switch (e->Type()) {
        case INFO_PrefsChanged: {
            if (m_initialized && isVisible) {
                gdk_threads_enter();
                SetToolbarType();
                gdk_threads_leave();
            }
            break; } 
        case INFO_SearchMusicDone: {
            if (m_initialized) {
                gdk_threads_enter();
                SetStatusText("");
                gdk_threads_leave();
            }
            break; }
        case INFO_BrowserMessage: {
            if (m_initialized) {
                gdk_threads_enter();
                SetStatusText(((BrowserMessageEvent *)e)->GetBrowserMessage());
                gdk_threads_leave();
            }
            break; }
        case CMD_AddFiles: {
            if (master)
                AddFileCMD();
            break; }
        case INFO_Playing: {
            pauseState = 1;
            stopState = 0;
            if (master) {
                gdk_threads_enter();
                UpdatePlayPause();
                gdk_threads_leave();
            }
            pauseState = 0;
            break; }
        case INFO_Stopped: {
            stopState = 1;
            pauseState = 0;
            if (master) {
                gdk_threads_enter();
                UpdatePlayPause();
                gdk_threads_leave();
            }
            pauseState = 1;
            break; }
        case INFO_Paused: {
            pauseState = 0;
            stopState = 0;
            if (master) {
                gdk_threads_enter();
                UpdatePlayPause();
                gdk_threads_leave();
            }
            pauseState = 1;
            break; }  
        case INFO_PlaylistRepeat: {
            PlaylistRepeatEvent *pre = (PlaylistRepeatEvent *)e;
            if (!iSetRepeatMode) {
                gdk_threads_enter();
                SetRepeatType(pre->GetRepeatMode());
                gdk_threads_leave();
            }
            else
                iSetRepeatMode = false;
            break; }
        case INFO_PlaylistShuffle: {
            PlaylistShuffleEvent *pse = (PlaylistShuffleEvent *)e;
            if (!iSetShuffleMode) {
                gdk_threads_enter();
                SetShuffleType(pse->GetShuffleMode());
                gdk_threads_leave();
            }
            else
                iSetShuffleMode = false;
            break; }
        case INFO_PlaylistSorted: {
            PlaylistSortedEvent *pse = (PlaylistSortedEvent *)e;
            if (pse->Manager() == m_plm) {
                gdk_threads_enter();
                UpdatePlaylistList();
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogTrackAdded: {
            MusicCatalogTrackAddedEvent *mct = (MusicCatalogTrackAddedEvent *)e;
            if (m_initialized && !m_bIgnoringMusicCatalogMessages) {
                gdk_threads_enter();
                AddCatTrack((ArtistList *)mct->Artist(), 
                            (AlbumList *)mct->Album(), 
                            (PlaylistItem *)mct->Item(), false);
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogStreamAdded: {
            MusicCatalogStreamAddedEvent *mcsae = 
                              (MusicCatalogStreamAddedEvent *)e;
            if (m_initialized && !m_bIgnoringMusicCatalogMessages &&
                !m_bCDMode) {
                gdk_threads_enter();
                AddCatStream((PlaylistItem *)mcsae->Item());
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogStreamRemoved: {
            MusicCatalogStreamRemovedEvent *mcsre =
                              (MusicCatalogStreamRemovedEvent *)e;
            if (m_initialized && !m_bCDMode) {
                gdk_threads_enter();
                RemoveCatStream((PlaylistItem *)mcsre->Item());
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogPlaylistAdded: {
            MusicCatalogPlaylistAddedEvent *mcp = 
                              (MusicCatalogPlaylistAddedEvent *)e;
            if (m_initialized && !m_bIgnoringMusicCatalogMessages) {
                gdk_threads_enter();
                AddCatPlaylist((string)mcp->Item());
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogPlaylistRemoved: {
            MusicCatalogPlaylistRemovedEvent *mcp =
                              (MusicCatalogPlaylistRemovedEvent *)e;
            if (m_initialized) {
                gdk_threads_enter();
                RemoveCatPlaylist((string)mcp->Item());
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogTrackRemoved: {
            MusicCatalogTrackRemovedEvent *mct = 
                                              (MusicCatalogTrackRemovedEvent *)e;
            if (m_initialized) {
                gdk_threads_enter();
                RemoveCatTrack((ArtistList *)mct->Artist(),
                               (AlbumList *)mct->Album(),
                               (PlaylistItem *)mct->Item());
                gdk_threads_leave();
            }
            break; }            
        case INFO_MusicCatalogTrackChanged: {
            MusicCatalogTrackChangedEvent *mct =
                                            (MusicCatalogTrackChangedEvent *)e;
            if (m_initialized) {
                gdk_threads_enter();
                RemoveCatTrack((ArtistList *)mct->OldArtist(),
                               (AlbumList *)mct->OldAlbum(),
                               (PlaylistItem *)mct->OldItem());
                AddCatTrack((ArtistList *)mct->NewArtist(),
                            (AlbumList *)mct->NewAlbum(),
                            (PlaylistItem *)mct->NewItem(), true);
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogCleared: {
            if (m_initialized) {
                gdk_threads_enter();
                ClearTree();
                if (!m_bCDMode)
                    RegenerateCDTree();
                gdk_threads_leave();
            }
            break; }
        case INFO_CDDiscStatus: {
            CDInfoEvent *cie = (CDInfoEvent *)e;

            if (cie->GetCDDB() != CD_DiscID && !m_bCDMode) {
                CD_DiscID = cie->GetCDDB();
                CD_numtracks = cie->GetNumTracks();
                if (isVisible) {
                    gdk_threads_enter();
                    RegenerateCDTree();
                    gdk_threads_leave();
                }
                else if (m_initialized)
                    scheduleCDredraw = true;
            }
            break; }
        case INFO_PlaylistItemAdded: {
            if (m_initialized) {
                PlaylistItemAddedEvent *piae = (PlaylistItemAddedEvent *)e;
                if (piae->Manager() == m_plm) {
                    vector<PlaylistItem *> list;
                    list.push_back((PlaylistItem *)(piae->Item()));
            
                    gdk_threads_enter();         
                    AddPlaylistItems(&list);
                    gdk_threads_leave();
                }
            }
            break; }
        case INFO_PlaylistItemsAdded: {
            if (m_initialized) {
                PlaylistItemsAddedEvent *piae = (PlaylistItemsAddedEvent *)e;
                if (piae->Manager() == m_plm) {
                    gdk_threads_enter();
                    AddPlaylistItems((vector<PlaylistItem*>*)piae->Items());
                    gdk_threads_leave();
                }
            }
            break; }
        case INFO_PlaylistItemRemoved: {
            if (m_initialized) {
                PlaylistItemRemovedEvent *pire = (PlaylistItemRemovedEvent *)e;
                if (pire->Manager() == m_plm) {
                    gdk_threads_enter();
                    RemovePlaylistItems((vector<uint32>*)pire->Indices());
                    gdk_threads_leave();
                }
            }
            break; }
        case INFO_PlaylistItemUpdated: {
            PlaylistItemUpdatedEvent *piue = (PlaylistItemUpdatedEvent *)e;
            PlaylistItem *item = (PlaylistItem *)piue->Item();

            char *url = (char *)item->URL().c_str();
            char *ext = strrchr(url, '.');;
            if (ext)
                ext++;
            if (ext && *ext) {
                if (!strncasecmp("CDA", ext, 3) && isVisible && !m_bCDMode)
                    UpdateCDTree(item);
            }

            if (piue->Manager() == m_plm && isVisible) {
                gdk_threads_enter();
                UpdatePlaylistItem(item);
                gdk_threads_leave();
            }
            break; }
        case INFO_PlaylistItemMoved: {
            PlaylistItemMovedEvent *pime = (PlaylistItemMovedEvent *)e;
            
            if (pime->Manager() == m_plm && isVisible) {
                gdk_threads_enter();

                uint32 start = pime->OldIndex();
                if (pime->NewIndex() < start)
                    start = pime->NewIndex();

                RenumberPlaylistList(start);
                gdk_threads_leave();
            }
            break; }
        case INFO_PlaylistCurrentItemInfo: {
            int temp = m_plm->GetCurrentIndex();
            PlaylistCurrentItemInfoEvent *pciie = 
                                              (PlaylistCurrentItemInfoEvent *)e;
            if (isVisible && pciie->Manager() == m_plm) {
                gdk_threads_enter();
                ChangeCurrentPlayingIndex(m_playingindex, temp);   
                gdk_threads_leave();
            }
            m_playingindex = temp;
            break; } 
        case INFO_MusicCatalogRegenerating: {
            m_bIgnoringMusicCatalogMessages = true;
            break; }
        case INFO_MusicCatalogDoneRegenerating: {
            m_bIgnoringMusicCatalogMessages = false;
            if (isVisible) {
                gdk_threads_enter();
                UpdateCatalog();
                gdk_threads_leave();
            }
            break; }
        default:
            break;
    }
    return kError_NoErr;
}
