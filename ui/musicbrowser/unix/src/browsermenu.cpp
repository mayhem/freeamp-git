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
#include <unistd.h>

#include "gtkmusicbrowser.h"
#include "fileselector.h"
#include "gtkmessagedialog.h"
#include "player.h"
#include "help.h"

#include "../res/new_pic.xpm"
#include "../res/save_pic.xpm"
#include "../res/import_pic.xpm"
#include "../res/edit_pic.xpm"
#include "../res/add_track.xpm"
#include "../res/add_file.xpm"
#include "../res/up_pic.xpm"
#include "../res/down_pic.xpm"
#include "../res/trash_pic.xpm"


static void add_tool(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->SetClickState(kContextPlaylist);
    FileSelector *filesel = new FileSelector("Add a Track");
    filesel->SetExtended();
    if (filesel->Run()) {
        char *returnpath = filesel->GetReturnPath();
        char *ext = p->GetContext()->player->GetExtension(returnpath);
        uint32 length = strlen(returnpath) + 10;
        char *tempurl = new char[length];
        if (IsntError(FilePathToURL(returnpath, tempurl, &length))) {
            if (ext && p->GetContext()->plm->IsSupportedPlaylistFormat(ext)) {
                string tobeloaded = tempurl;
                p->LoadPlaylist(tobeloaded);
            }
            else {
                char *filereturn = strdup_new(filesel->GetReturnPath());
                if (filereturn) {
                    char *temp;
                    char *first= strtok(filereturn, "\n");

                    while ((temp = strtok(NULL, "\n"))) {
                        ext = p->GetContext()->player->GetExtension(temp);
                        if (ext &&
                           p->GetContext()->player->IsSupportedExtension(ext)) {
                            p->AddTrackPlaylistEvent(temp);
                            p->m_currentindex++;
                        }
                    }
                    ext = p->GetContext()->player->GetExtension(first);
                    if (ext &&
                        p->GetContext()->player->IsSupportedExtension(ext))
                        p->AddTrackPlaylistEvent(first);
                }
                delete [] filereturn;
            }
       }
       delete [] tempurl;
    }
    delete filesel;
}

static void move_up_tool(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->MoveUpEvent();
    p->SetClickState(kContextPlaylist);
}

static void move_down_tool(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->MoveDownEvent();
    p->SetClickState(kContextPlaylist);
}

static void import_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    FileSelector *filesel = new FileSelector("Import a Track or Playlist into My Music");
    if (filesel->Run()) {
        FAContext *m_context = p->GetContext();
        char *returnpath = filesel->GetReturnPath();
        char *ext = m_context->player->GetExtension(returnpath);
        uint32 length = strlen(returnpath) + 10;
        char *tempurl = new char[length];

        if (IsntError(FilePathToURL(returnpath, tempurl, &length))) {
            if (ext && m_context->plm->IsSupportedPlaylistFormat(ext))
                p->ImportPlaylist(tempurl);
            else if (ext && m_context->player->IsSupportedExtension(ext)) {
                PlaylistItem *plist = new PlaylistItem(tempurl);
                m_context->plm->RetrieveMetaData(plist);

                while (plist->GetState() != kPlaylistItemState_Normal)
                    usleep(5);

                m_context->catalog->WriteMetaDataToDatabase(tempurl,
                                                          plist->GetMetaData());

                m_context->catalog->AddSong(tempurl);

                delete plist;
            }
        }
        delete [] tempurl;
        delete ext;
    }
    delete filesel;
}

static void remove_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    if (p->GetClickState() == kContextPlaylist) {
        p->DeleteEvent();
    }
    else if (p->GetClickState() == kContextBrowser) {
        switch (p->GetTreeClick()) {
            case kTreePlaylist: {
                p->GetContext()->catalog->RemovePlaylist(p->mbSelection->playlistname.c_str());
                break; }
            case kTreeTrack: {
                p->GetContext()->catalog->RemoveSong(p->mbSelection->track->URL().c_str());
                break; }
            default:
                break;
        }
    }
}

static void edit_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    if (p->GetClickState() == kContextPlaylist) {
        p->PopUpInfoEditor();
    }
    else if (p->GetClickState() == kContextBrowser) {
        switch (p->GetTreeClick()) {
            case kTreePlaylist: {
                p->CreateNewEditor((char *)p->mbSelection->playlistname.c_str());
                break; }
            case kTreeTrack: {
                p->PopUpInfoEditor(p->mbSelection->track);
                break; }
            default:
                break;
        }
    }
}

static void infoedit(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    edit_tool(w, p);
}

static void new_plist(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SaveCurrentPlaylist();

    p->CreateNewEditor(NULL);
}

static void new_plist_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    new_plist(p, 1, w);
}

static void open_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SaveCurrentPlaylist();

    FileSelector *filesel = new FileSelector("Open a Playlist from Disk");
    if (filesel->Run())
        p->CreateNewEditor(filesel->GetReturnPath());
    delete filesel;
}

static void saveas_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    GTKMessageDialog oBox;
    string oMessage = string("What do you want to call this playlist?");

    if (oBox.Show(oMessage.c_str(), "Save This List to Disk?", kMessageOkCancel,
                  true, true)
                  == kMessageReturnOk) {

        char *temp = oBox.GetEntryText();
        if (!temp)
            return;

        for (unsigned int i = 0; i < strlen(temp); i++)
            if (temp[i] == '/')
                temp[i] = '_';

        string name = FreeampDir(NULL) + string("/") + string(temp) +
                      string(".m3u");
        p->m_currentListName = string(name);

        p->SaveCurrentPlaylist();
    }
}

static void save_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    if (p->m_currentListName.length() == 0) {
        saveas_list(p, action, w);
    }
    else
        p->SaveCurrentPlaylist();
}

static void save_list_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    save_list(p, 1, w);
}

static void import_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    import_tool(w, p);
}

static void export_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    FileSelector *filesel = new FileSelector("Export This Playlist to Disk");
    if (filesel->Run())
        p->SaveCurrentPlaylist(filesel->GetReturnPath());
    delete filesel;
}

static void music_search(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->StartMusicSearch();
}

static void quit_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ToggleVisEvent();
}

static void add_track_mb(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    if (p->GetClickState() == kContextBrowser) {
        vector<PlaylistItem *> *newlist = p->GetTreeSelection();
        p->AddTracksPlaylistEvent(newlist, true);
    }
}

static void add_track_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    add_track_mb(p, 0, w);
}

static void add_track(GTKMusicBrowser *p, guint action, GtkWidget *w)
{   
    FileSelector *filesel = new FileSelector("Add a Track");
    filesel->SetExtended();
    if (filesel->Run()) {
        char *filereturn = strdup_new(filesel->GetReturnPath());
        if (filereturn) {
            char *temp;
            char *first;
            first = strtok(filereturn, "\n");
            while ((temp = strtok(NULL, "\n"))) {
                p->AddTrackPlaylistEvent(temp);
                p->m_currentindex++;
            }
            p->AddTrackPlaylistEvent(first);
       }
       delete filereturn;
    }
    delete filesel;
}

static void delete_sel(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    string urlToDel;

    if (p->GetClickState() == kContextPlaylist)
        urlToDel = p->GetContext()->plm->ItemAt(p->m_currentindex)->URL();
    else if (p->GetClickState() == kContextBrowser) {
        switch (p->GetTreeClick()) {
            case kTreePlaylist:
                urlToDel = p->mbSelection->playlistname;
                break;
            case kTreeTrack:
                urlToDel = p->mbSelection->track->URL();
                break;
            default: 
                return;
        }
    }

    uint32 length = urlToDel.length();
    char *filename = new char[length];

    if (IsntError(URLToFilePath(urlToDel.c_str(), filename, &length))) {
        GTKMessageDialog oBox;
        string oMessage = string("Are you sure you want to delete ")
                          + string(filename) + string("?");

        if (oBox.Show(oMessage.c_str(), "Delete Confirmation", kMessageYesNo,
                      true, false, "Delete it Permantly From the Disk")
                      == kMessageReturnYes) {

            if (oBox.GetCheckStatus())
                unlink(filename);

            if (p->GetClickState() == kContextPlaylist) {
                p->DeleteEvent();
            }
            else if (p->GetClickState() == kContextBrowser) {
                switch (p->GetTreeClick()) {
                    case kTreePlaylist: {
                        p->GetContext()->catalog->RemovePlaylist(p->mbSelection->playlistname.c_str());
                        break; }
                    case kTreeTrack: {
                        p->GetContext()->catalog->RemoveSong(p->mbSelection->track->URL().c_str());
                        break; }
                    default:
                        break;
                }
            }
        }
    }

    delete [] filename;
}

static void move_up(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->MoveUpEvent();
}

static void move_down(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->MoveDownEvent();
}

static void clear_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->DeleteListEvent();
}

static void catalog_tog(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ExpandCollapseEvent();
}

static void play_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->PlayMenu();
}

static void stop_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->StopMenu();
}

static void next_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->NextMenu();
}

static void prev_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->PrevMenu();
}

static void repeat_none(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
   p->SetRepeat(0);
}

static void repeat_one(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
   p->SetRepeat(1);
}

static void repeat_all(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
   p->SetRepeat(2);
}

static void sort_artist(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Artist, kPlaylistSortType_Ascending);
}

static void sort_album(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Album, kPlaylistSortType_Ascending);
}

static void sort_title(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Title, kPlaylistSortType_Ascending);

}

static void sort_year(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Year, kPlaylistSortType_Ascending);
}

static void sort_track(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Track, kPlaylistSortType_Ascending);
}

static void sort_genre(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Genre, kPlaylistSortType_Ascending);
}

static void sort_time(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Time, kPlaylistSortType_Ascending);
}

static void sort_location(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Location, kPlaylistSortType_Ascending);
}

static void sort_normal(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->iSetShuffleMode = true;
    p->SortPlaylistEvent(kPlaylistSortKey_LastKey, kPlaylistSortType_Descending);
}

static void sort_random2(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->iSetShuffleMode = true;
    p->SortPlaylistEvent(kPlaylistSortKey_LastKey, kPlaylistSortType_Ascending);
}

static void sort_random(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Random, kPlaylistSortType_Ascending);
}

static void show_help(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    string oHelpFile;
    char   dir[_MAX_PATH];
    uint32 len = _MAX_PATH;

    p->GetContext()->prefs->GetInstallDirectory(dir, &len);
    oHelpFile = string(dir) + string(DIR_MARKER_STR) + string("../share/");
    oHelpFile += string(HELP_FILE);

    struct stat st;

    if (stat(oHelpFile.c_str(), &st) == 0 && st.st_mode & S_IFREG)
        LaunchBrowser((char *)oHelpFile.c_str());
    else {
        GTKMessageDialog oBox;
        string oMessage("Cannot find the help files. Please make sure that the help files are properly installed, and you are not running "the_BRANDING" from the build directory.");
        oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk, true);
    }
}

static void freeamp_web(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    LaunchBrowser("http://www.freeamp.org/");
}

static void emusic_web(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    LaunchBrowser("http://www.emusic.com/");
}

static void show_about(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ShowOptions(5);
}

static void options_show(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ShowOptions();
}

static void create_cd(GTKMusicBrowser *p, guint action, GtkWidget *w)
{

}

static void add_stream(GTKMusicBrowser *p, guint action, GtkWidget *w)
{

}

static void eject_cd(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->EjectCD();
}

void GTKMusicBrowser::CreateMenu(GtkWidget *topbox)
{
    GtkItemFactoryEntry menu_items[] = {
     {"/_File",                 NULL,           0,         0, "<Branch>" },
     {"/File/_New Playlist",    "<control>N",   (void(*)())new_plist, 0, 0 },
     {"/File/_Open Playlist",   "<control>O",   (void(*)())open_list, 0, 0 },
     {"/File/_Save Playlist",   "<control>S",   (void(*)())save_list, 0, 0 },
     {"/File/Save Playlist _As","<control>A",   (void(*)())saveas_list,0, 0 },
     {"/File/sep1",             NULL,           0,         0, "<Separator>" },
     {"/File/_Import Tracks and Playlists","<control>I", (void(*)())import_list, 0, 0 },
     {"/File/_Export Playlist", NULL,           (void(*)())export_list, 0, 0 },
     {"/File/sep0",             NULL,           0,         0, "<Separator>" },
     {"/File/Create New Audio CD", NULL,        (void(*)())create_cd, 0, 0 },
     {"/File/Search Computer for Music", NULL,  (void(*)())music_search, 0, 0 },
     {"/File/sep2",             NULL,           0,         0, "<Separator>" },
     {"/File/_Close",           "<control>Q",   (void(*)())quit_menu, 0, 0 },

     {"/_Edit",                 NULL,           0,         0, "<Branch>" },
     {"/_Edit/Add Items to Playlist", NULL,     (void(*)())add_track_mb, 0, 0 },
     {"/_Edit/Add Tracks or Playlists from Disk", NULL,  (void(*)())add_track, 0, 0 },
     {"/_Edit/Remove Items from My Music", NULL,(void(*)())delete_sel,0, 0 },
     {"/_Edit/Add Stream to Favorites", NULL,   (void(*)())add_stream, 0, 0 },
     {"/_Edit/sep3",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Move Up",         NULL,           (void(*)())move_up,   0, 0 },
     {"/_Edit/Move Down",       NULL,           (void(*)())move_down, 0, 0 },
     {"/_Edit/sep4",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Clear Playlist",  NULL,           (void(*)())clear_list, 0, 0 },
     {"/_Edit/Edit Info",       NULL,           (void(*)())infoedit,  0, 0 },

     {"/_View",                 NULL,           0,         0, "<Branch>" },
     {"/_View/View Playlist Only",NULL,         (void(*)())catalog_tog, 0, 0 },
     {"/_View/Options",         NULL,           (void(*)())options_show, 0, 0 },

     {"/_Controls",             NULL,           0,         0, "<Branch>" },
     {"/_Controls/Play",        NULL,           (void(*)())play_menu,0, 0 },
     {"/_Controls/Stop",        NULL,           (void(*)())stop_menu, 0, 0 },
     {"/_Controls/sep6",        NULL,           0,         0, "<Separator>" },
     {"/_Controls/Eject CD",    NULL,           (void(*)())eject_cd, 0, 0 },
     {"/_Controls/sep65",       NULL,           0,         0, "<Separator>" },
     {"/_Controls/Next Track",   NULL,          (void(*)())next_menu, 0, 0 },
     {"/_Controls/Previous Track", NULL,        (void(*)())prev_menu, 0, 0 },
     {"/_Controls/sep7",        NULL,           0,         0, "<Separator>" },
     {"/_Controls/Play Tracks in Normal Order", NULL, (void(*)())sort_normal, 0, "<RadioItem>" },
     {"/_Controls/Play Tracks in Random Order", NULL, (void(*)())sort_random2, 0, "/Controls/Play Tracks in Normal Order" },
     {"/_Controls/sep8",        NULL,           0,         0, "<Separator>" },
     {"/_Controls/Repeat No Tracks", NULL,      (void(*)())repeat_none, 0, "<RadioItem>" },
     {"/_Controls/Repeat One Track",  NULL,     (void(*)())repeat_one, 0, "/Controls/Repeat No Tracks" },
     {"/_Controls/Repeat All Tracks",  NULL,    (void(*)())repeat_all, 0, "/Controls/Repeat No Tracks" },

     {"/_Sort Playlist",        NULL,           0,         0, "<Branch>" },
     {"/_Sort Playlist/by Artist",  NULL,       (void(*)())sort_artist, 0, 0 },
     {"/_Sort Playlist/by Album", NULL,         (void(*)())sort_album, 0, 0 },
     {"/_Sort Playlist/by Title", NULL,         (void(*)())sort_title, 0, 0 },
     {"/_Sort Playlist/by Year", NULL,          (void(*)())sort_year,  0, 0 },
     {"/_Sort Playlist/by Track Number", NULL,  (void(*)())sort_track, 0, 0 },
     {"/_Sort Playlist/by Genre", NULL,         (void(*)())sort_genre, 0, 0 },
     {"/_Sort Playlist/by Length", NULL,        (void(*)())sort_time,  0, 0 },
     {"/_Sort Playlist/by Location", NULL,      (void(*)())sort_location, 0, 0 },
     {"/_Sort Playlist/Randomly", NULL,         (void(*)())sort_random, 0, 0 },

     {"/_Help",                 NULL,           0,          0, "<Branch>" },
     {"/_Help/Contents",        NULL,           (void(*)())show_help,  0, 0 },
     {"/_Help/sep9",            NULL,           0,          0, "<Separator>" },
     {"/_Help/FreeAmp Web Site", NULL,          (void(*)())freeamp_web, 0, 0 },
     {"/_Help/EMusic.com Web Site", NULL,       (void(*)())emusic_web, 0, 0 },
     {"/_Help/sep10",           NULL,           0,          0, "<Separator>" },
     {"/_Help/About",           NULL,           (void(*)())show_about, 0, 0 }
    };

    int nmenu_items = sizeof(menu_items) / sizeof(menu_items[0]);


    GtkAccelGroup *accel_group;
    GtkWidget *separator;

    accel_group = gtk_accel_group_new();
    menuFactory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<blah>",
                                        accel_group);
    gtk_item_factory_create_items(menuFactory, nmenu_items, menu_items, 
                                  (void *)this);

    gtk_accel_group_attach(accel_group, GTK_OBJECT(musicBrowser));
    gtk_box_pack_start(GTK_BOX(topbox), gtk_item_factory_get_widget(
                       menuFactory, "<blah>"), FALSE, TRUE, 0);
    gtk_widget_show(gtk_item_factory_get_widget(menuFactory, "<blah>"));

    separator = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(topbox), separator, FALSE, TRUE, 0);
    gtk_widget_show(separator);
}

void GTKMusicBrowser::SetToolbarType(void)
{
    bool useText, useImages;
    m_context->prefs->GetShowToolbarTextLabels(&useText);
    m_context->prefs->GetShowToolbarImages(&useImages);

    if (useText && useImages)
        gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
    else if (useText)
        gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_TEXT);
    else if (useImages)
        gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
}

void GTKMusicBrowser::CreateToolbar(GtkWidget *vbox)
{
    toolbar = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL,
                                         GTK_TOOLBAR_BOTH);
    gtk_toolbar_set_button_relief(GTK_TOOLBAR(toolbar), GTK_RELIEF_NONE);
    gtk_toolbar_set_space_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_SPACE_LINE);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "New Playlist",
                            "Start Editing a new Playlist",
                            "Toolbar/New", NewPixmap(new_pic),
                            GTK_SIGNAL_FUNC(new_plist_tool), this);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Save Playlist",
                            "Save the Current Playlist to Disk",
                            "Toolbar/Save", NewPixmap(save_pic),
                            GTK_SIGNAL_FUNC(save_list_tool), this);

    gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Import Items",
                            "Import a Playlist or Track into My Music",
                            "Toolbar/Import", NewPixmap(import_pic),
                            GTK_SIGNAL_FUNC(import_tool), this);

    toolRemove = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Remove Track",
                                         "Remove a Track from the Playlist",
                                         "Toolbar/Remove", NewPixmap(trash_pic),
                                         GTK_SIGNAL_FUNC(remove_tool), this);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Edit",
                            "Edit a Track or Playlist",
                            "Toolbar/Edit", NewPixmap(edit_pic),
                            GTK_SIGNAL_FUNC(edit_tool), this);

    gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));

    addTrack = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Add Items",
                            "Add Music from My Music to the Playlist",
                            "Toolbar/AddItem", NewPixmap(add_track_pic),
                            GTK_SIGNAL_FUNC(add_track_tool), this);

    addFile = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Add File",
                            "Add a File to the Playlist",
                            "Toolbar/Add", NewPixmap(add_file),
                            GTK_SIGNAL_FUNC(add_tool), this);

    toolUp = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Move Up",
                            "Move The Currently Selected Track in the Playlist Up",
                            "Toolbar/Up", NewPixmap(up_pic),
                            GTK_SIGNAL_FUNC(move_up_tool), this);

    toolDown = gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Move Down",
                            "Move the Currently Selected Track in the Playlist Down",
                            "Toolbar/Down", NewPixmap(down_pic),
                            GTK_SIGNAL_FUNC(move_down_tool), this);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
    SetToolbarType();
    gtk_widget_show(toolbar);
}
