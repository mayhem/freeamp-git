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

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

#include "utility.h"
#include "gtkmusicbrowser.h"
#include "infoeditor.h"
#include "fileselector.h"
#include "browsermenu.h"
#include "eventdata.h"
#include "player.h"
#include "musicbrowserui.h"
#include "gtkmessagedialog.h"
#include "help.h"

#include "../res/down_pic.xpm"
#include "../res/edit_pic.xpm"
#include "../res/new_pic.xpm"
#include "../res/import_pic.xpm"
#include "../res/add_file.xpm"
#include "../res/add_track.xpm"
#include "../res/save_pic.xpm"
#include "../res/trash_pic.xpm"
#include "../res/up_pic.xpm"
#include "../res/album_pix.xpm"
#include "../res/all_pix.xpm"
#include "../res/artist_pix.xpm"
#include "../res/catalog_pix.xpm"
#include "../res/playlist_pix.xpm"
#include "../res/track_pix.xpm"
#include "../res/uncatagorized_pix.xpm"

extern "C" {
void new_plist(GTKMusicBrowser *p, guint action, GtkWidget *w);
void open_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void save_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void saveas_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void export_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void import_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void music_search(GTKMusicBrowser *p, guint action, GtkWidget *w);
void quit_menu(GTKMusicBrowser *p, guint action, GtkWidget *w);
void add_track_mb(GTKMusicBrowser *p, guint action, GtkWidget *w);
void add_track(GTKMusicBrowser *p, guint action, GtkWidget *w);
void infoedit(GTKMusicBrowser *p, guint action, GtkWidget *w);
void delete_sel(GTKMusicBrowser *p, guint action, GtkWidget *w);
void move_up(GTKMusicBrowser *p, guint action, GtkWidget *w);
void move_down(GTKMusicBrowser *p, guint action, GtkWidget *w);
void clear_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void catalog_tog(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_normal(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_random(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_random2(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_location(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_time(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_genre(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_track(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_year(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_title(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_album(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_artist(GTKMusicBrowser *p, guint action, GtkWidget *w);
void options_show(GTKMusicBrowser *p, guint action, GtkWidget *w);
void stop_menu(GTKMusicBrowser *p, guint action, GtkWidget *w);
void play_menu(GTKMusicBrowser *p, guint action, GtkWidget *w);
void next_menu(GTKMusicBrowser *p, guint action, GtkWidget *w);
void prev_menu(GTKMusicBrowser *p, guint action, GtkWidget *w);
void repeat_none(GTKMusicBrowser *p, guint action, GtkWidget *w);
void repeat_one(GTKMusicBrowser *p, guint action, GtkWidget *w);
void repeat_all(GTKMusicBrowser *p, guint action, GtkWidget *w);
void show_help(GTKMusicBrowser *p, guint action, GtkWidget *w);
void freeamp_web(GTKMusicBrowser *p, guint action, GtkWidget *w);
void emusic_web(GTKMusicBrowser *p, guint action, GtkWidget *w);
void show_about(GTKMusicBrowser *p, guint action, GtkWidget *w);
}

FAContext *BADContext = NULL;

void repeat_none(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
   p->SetRepeat(0);
}

void repeat_one(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
   p->SetRepeat(1);
}

void repeat_all(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
   p->SetRepeat(2);
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

void set_active(GtkWidget *w, gpointer data)
{
    if (GTK_IS_CHECK_MENU_ITEM(GTK_OBJECT(w))) {
        gtk_check_menu_item_set_show_toggle(GTK_CHECK_MENU_ITEM(w), TRUE);
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

void show_help(GTKMusicBrowser *p, guint action, GtkWidget *w)
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
        string oMessage("Cannot find the help files. Please make sure that the help files are properly installed, and you are not running "BRANDING" from the build directory.");
        oBox.Show(oMessage.c_str(), string(BRANDING), kMessageOk, true);
    }
}

void freeamp_web(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    LaunchBrowser("http://www.freeamp.org/");
}

void emusic_web(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    LaunchBrowser("http://www.emusic.com/");
}

void show_about(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ShowOptions(5);
}

void stop_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->StopMenu();
}

void GTKMusicBrowser::StopMenu()
{
    m_context->target->AcceptEvent(new Event(CMD_Stop));
}

void play_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->PlayMenu();
}

void GTKMusicBrowser::PlayMenu()
{
    if (pauseState) 
        m_context->target->AcceptEvent(new Event(CMD_Play));
    else 
        m_context->target->AcceptEvent(new Event(CMD_Pause));
}

void next_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->NextMenu();
}

void GTKMusicBrowser::NextMenu(void)
{
    m_context->target->AcceptEvent(new Event(CMD_NextMediaPiece));
}

void prev_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->PrevMenu();
}

void GTKMusicBrowser::PrevMenu(void)
{
    m_context->target->AcceptEvent(new Event(CMD_PrevMediaPiece));
}

void options_show(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ShowOptions();
}

void GTKMusicBrowser::ShowOptions(int page)
{
    m_context->target->AcceptEvent(new ShowPreferencesEvent(page));
}

TreeData *GTKMusicBrowser::NewTreeData(int type, MusicCatalog *cat, 
                                       ArtistList *art, AlbumList *alb, 
                                       PlaylistItem *tr, char *pname,
                                       char *message)
{
    TreeData *data = new TreeData;
    data->type = type;
    data->catalog = cat;
    data->artist = art;
    data->album = alb;
    data->track = tr;
    if (pname)
        data->playlistname = pname;
    else
        data->playlistname = ""; 
    if (message)
        data->message = message;
    else 
        data->message = "";
    return data;
}

static vector<PlaylistItem *> *getTreeSelection(GtkCTree *tree) 
{
    vector<PlaylistItem *> *newlist = new vector<PlaylistItem *>;

    GtkCList *clist = GTK_CLIST(tree);
    GList *selection = clist->selection;

    while (selection) {
      GtkCTreeNode *node = (GtkCTreeNode *)selection->data;
      TreeData *data = (TreeData *)gtk_ctree_node_get_row_data(tree, node);

      if (!data)
          return newlist;

      switch (data->type) {
        case kTreeAll: {
            MusicCatalog *cat = data->catalog;
            if (!cat)
                return newlist;
            vector<ArtistList *> *artistList = 
                                   (vector<ArtistList *>*)cat->GetMusicList();
            vector<PlaylistItem *> *unsorted = 
                             (vector<PlaylistItem *>*)cat->GetUnsortedMusic();
            vector<ArtistList *>::iterator h = artistList->begin();
            for (; h != artistList->end(); h++) {
                vector<AlbumList *>::iterator i = (*h)->m_albumList->begin();
                for (; i != (*h)->m_albumList->end(); i++) {
                    vector<PlaylistItem *>::iterator j = (*i)->m_trackList->begin();
                    for (; j != (*i)->m_trackList->end(); j++) {
                        PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*j);
                        newlist->push_back(item);
                    }
                }
            }
            vector<PlaylistItem *>::iterator k = unsorted->begin();
            for (; k != unsorted->end(); k++) {
                PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*k);
                newlist->push_back(item);
            }
            break; }
        case kTreeArtist: {
            ArtistList *list = data->artist;
            vector<AlbumList *>::iterator i = list->m_albumList->begin();
            for (; i != list->m_albumList->end(); i++) {
                vector<PlaylistItem *>::iterator j = (*i)->m_trackList->begin();
                for (; j != (*i)->m_trackList->end(); j++) {
                    PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*j);
                    newlist->push_back(item);
                }
            }
            break; }
        case kTreeAlbum: {
            AlbumList *list = data->album;
            vector<PlaylistItem *>::iterator j = list->m_trackList->begin();
            for (; j != list->m_trackList->end(); j++) {
                PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*j);
                newlist->push_back(item);
            }
            break; }
        case kTreeTrack: {
            PlaylistItem *i = new PlaylistItem(*(data->track));
            newlist->push_back(i);
            break; }
        case kTreePlaylist: {
            char *fname = (char *)data->playlistname.c_str();
            BADContext->plm->ReadPlaylist(fname, newlist);
            break; }
        case kTreeUncat: {
            MusicCatalog *cat = data->catalog;
            vector<PlaylistItem *> *unsorted = 
                               (vector<PlaylistItem *>*)cat->GetUnsortedMusic();
            vector<PlaylistItem *>::iterator k = unsorted->begin();
            for (; k != unsorted->end(); k++) {
                PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*k);
                newlist->push_back(item);
            }
            break; }
        default:
            break;
      }

      selection = selection->next;
    }
    return newlist;
}

static gboolean list_drag_drop_internal(GtkWidget *widget, 
                                        GdkDragContext *context,
                                        gint x, gint y, guint time, 
                                        GTKMusicBrowser *p)
{
    if (widget == gtk_drag_get_source_widget(context))
        return FALSE;
    GList *list = context->targets;
    GdkAtom atom = gdk_atom_intern("tree-drag", FALSE);
    while (list) {
        if ((int)atom == GPOINTER_TO_INT(list->data)) 
            return TRUE;
        list = list->next;
    }
    return FALSE;
}

static void drag_dest_info_destroy(gpointer data)
{
    GtkCListDestInfo *info = (GtkCListDestInfo *)data;
    g_free(info);
}

#define ROW_TOP_YPIXEL(clist, row) (((clist)->row_height * (row)) + \
                                    ((row) + 1) + (clist)->voffset)
#define ROW_FROM_YPIXEL(clist, y)  (((y) - (clist)->voffset) / \
                                    ((clist)->row_height + 1))
#define GTK_CLIST_CLASS_FW(_widget_) GTK_CLIST_CLASS (((GtkObject*) (_widget_))->klass)


static gint COLUMN_FROM_XPIXEL (GtkCList * clist, gint x)
{
  gint i, cx;

  for (i = 0; i < clist->columns; i++)
    if (clist->column[i].visible)
      {
        cx = clist->column[i].area.x + clist->hoffset;

        if (x >= (cx - 4) &&
            x <= (cx + clist->column[i].area.width + 3))
          return i;
      }

  return -1;
}

static void drag_dest_cell (GtkCList *clist, gint x, gint y, 
                     GtkCListDestInfo *dest_info)
{
    GtkWidget *widget;

    widget = GTK_WIDGET (clist);
    dest_info->insert_pos = GTK_CLIST_DRAG_NONE;

    y -= (GTK_CONTAINER (clist)->border_width + 
          widget->style->klass->ythickness + clist->column_title_area.height);

    dest_info->cell.row = ROW_FROM_YPIXEL (clist, y);
    if (dest_info->cell.row >= clist->rows) {
        dest_info->cell.row = clist->rows - 1;
        y = ROW_TOP_YPIXEL (clist, dest_info->cell.row) + clist->row_height;
    }
    if (dest_info->cell.row < -1)
        dest_info->cell.row = -1;

    x -= GTK_CONTAINER (widget)->border_width + 
         widget->style->klass->xthickness;
    dest_info->cell.column = COLUMN_FROM_XPIXEL (clist, x);

    if (dest_info->cell.row >= 0) {
        gint y_delta;
        gint h = 0;

        y_delta = y - ROW_TOP_YPIXEL (clist, dest_info->cell.row);

        if (GTK_CLIST_DRAW_DRAG_RECT(clist)) {
            dest_info->insert_pos = GTK_CLIST_DRAG_INTO;
            h = clist->row_height / 4;
        }
        else if (GTK_CLIST_DRAW_DRAG_LINE(clist)) {
            dest_info->insert_pos = GTK_CLIST_DRAG_BEFORE;
            h = clist->row_height / 2;
        }
 
        if (GTK_CLIST_DRAW_DRAG_LINE(clist)) {
            if (y_delta < h)
                dest_info->insert_pos = GTK_CLIST_DRAG_BEFORE;
            else if (clist->row_height - y_delta < h)
                dest_info->insert_pos = GTK_CLIST_DRAG_AFTER;
        }
    }
}

static void list_drag_rec_internal(GtkWidget *widget, GdkDragContext *context,
                                   gint x, gint y, GtkSelectionData *data,
                                   guint info, guint time, GTKMusicBrowser *p)
{
    if (widget == gtk_drag_get_source_widget(context))
        return;
    if (data->length == sizeof(vector<PlaylistItem *> *)) {
        vector<PlaylistItem *> *newlist = *((vector<PlaylistItem *> **)(data->data));
        if (newlist) {
            GtkCListDestInfo dest_info;

            drag_dest_cell(GTK_CLIST(widget), x, y, &dest_info);
            if (dest_info.insert_pos == GTK_CLIST_DRAG_AFTER)
                dest_info.cell.row++;

            p->m_currentindex = dest_info.cell.row;
            p->AddTracksPlaylistEvent(newlist);

            p->SetClickState(kContextPlaylist);

            g_dataset_remove_data(context, "gtk-clist-drag-dest");
        }
    }
}

static void list_drag_leave_internal(GtkWidget *widget, GdkDragContext *context,
                                     guint time, GTKMusicBrowser *p)
{
    if (widget == gtk_drag_get_source_widget(context))
        return;

    GtkCList *clist = GTK_CLIST(widget);
    GtkCListDestInfo *dest_info;
    dest_info = (GtkCListDestInfo *)g_dataset_get_data(context, 
                                                       "gtk-clist-drag-dest");
    if (dest_info) {
        if (dest_info->cell.row >= 0) {
            GList *list = context->targets;
            GdkAtom atom = gdk_atom_intern("tree-drag", FALSE);
            while (list) {
                if ((int)atom == GPOINTER_TO_INT(list->data)) {
                    GTK_CLIST_CLASS_FW(clist)->draw_drag_highlight(clist,
                            (GtkCListRow *)g_list_nth(clist->row_list,
                                                     dest_info->cell.row)->data,
                             dest_info->cell.row, dest_info->insert_pos); 
                    break;
                }
                list = list->next;
            }
        }
        g_dataset_remove_data(context, "gtk-clist-drag-dest");
    }
}

static gint list_drag_motion_internal(GtkWidget *widget, 
                                      GdkDragContext *context,
                                      gint x, gint y, guint time, 
                                      GTKMusicBrowser *p)
{
    if (widget == gtk_drag_get_source_widget(context))
        return FALSE;

    GtkCList *clist = GTK_CLIST(widget);
    GtkCListDestInfo new_info;
    GtkCListDestInfo *dest_info;

    dest_info = (GtkCListDestInfo *)g_dataset_get_data(context, 
                                                       "gtk-clist-drag-dest");
    if (!dest_info) {
        dest_info = g_new(GtkCListDestInfo, 1);
        dest_info->insert_pos = GTK_CLIST_DRAG_NONE;
        dest_info->cell.row = -1;
        dest_info->cell.column = -1;
        g_dataset_set_data_full(context, "gtk-clist-drag-dest", dest_info,
                                drag_dest_info_destroy);
    }
    drag_dest_cell(clist, x, y, &new_info);
    
    GList *list = context->targets;
    GdkAtom atom = gdk_atom_intern("tree-drag", FALSE);
    list = context->targets;
    while (list) {
        if ((int)atom == GPOINTER_TO_INT(list->data))
            break;
        list = list->next;
    }
    if (list) {
/*        if (new_info.insert_pos == GTK_CLIST_DRAG_NONE) {
            if (dest_info->cell.row < 0) {
                gdk_drag_status(context, GDK_ACTION_DEFAULT, time);
                return FALSE;
            }
            return TRUE;
        } */
        if (new_info.cell.row != dest_info->cell.row ||
            (new_info.cell.row == dest_info->cell.row &&
             dest_info->insert_pos != new_info.insert_pos)) {
            if (dest_info->cell.row >= 0)
                GTK_CLIST_CLASS_FW(clist)->draw_drag_highlight(clist,
                        (GtkCListRow *)g_list_nth(clist->row_list, 
                                                  dest_info->cell.row)->data,
                        dest_info->cell.row, dest_info->insert_pos);
            dest_info->insert_pos = new_info.insert_pos;
            dest_info->cell.row = new_info.cell.row;
            dest_info->cell.column = new_info.cell.column;
 
            GTK_CLIST_CLASS_FW(clist)->draw_drag_highlight(clist,
                        (GtkCListRow *)g_list_nth(clist->row_list, 
                                                  dest_info->cell.row)->data,
                        dest_info->cell.row, dest_info->insert_pos);
            gdk_drag_status(context, context->suggested_action, time);
        }
        return TRUE;
    }

    dest_info->insert_pos = new_info.insert_pos;
    dest_info->cell.row = new_info.cell.row;
    dest_info->cell.column = new_info.cell.column;
    return TRUE;
}

static void tree_clicked(GtkWidget *widget, GdkEventButton *event, 
                         GTKMusicBrowser *p)
{
    if (!event)
        return;

    g_return_if_fail(widget != NULL);
    g_return_if_fail(GTK_IS_CTREE(widget));
    g_return_if_fail(event != NULL);

    GtkCTree *ctree = GTK_CTREE(widget);
    GtkCList *clist = GTK_CLIST(widget);

    if (event->window != clist->clist_window)
        return;

    if (event->type == GDK_2BUTTON_PRESS) {
        vector<PlaylistItem *> *newlist = getTreeSelection(ctree);
        p->AddTracksPlaylistEvent(newlist, true);
    }
    else {
        int x, y, row, column;

        x = (int)event->x;
        y = (int)event->y;
 
        if (!gtk_clist_get_selection_info(clist, x, y, &row, &column)) 
            return;

        GtkCTreeNode *node = GTK_CTREE_NODE(g_list_nth(clist->row_list, row));
        p->mbSelection = (TreeData *)gtk_ctree_node_get_row_data(ctree, node);

        if (p->mbSelection->type == kTreePlaylist) 
            p->SetTreeClick(kClickPlaylist);
        else if (p->mbSelection->type == kTreeTrack) 
            p->SetTreeClick(kClickTrack);
        else
            p->SetTreeClick(kClickNone);
    }

    p->SetClickState(kContextBrowser); 
}

static void tree_source_destroy(gpointer data)
{
}

static void tree_drag_begin(GtkWidget *w, GdkDragContext *context)
{
    GtkCTree *ctree;
    GtkCList *clist;
 
    g_return_if_fail(w != NULL);
    g_return_if_fail(GTK_IS_CTREE(w));
    g_return_if_fail(context != NULL);

    ctree = GTK_CTREE(w);
    clist = GTK_CLIST(w);

    vector<PlaylistItem *> *newlist = getTreeSelection(ctree);

    if (newlist->size() == 0) {
        delete newlist;

        GtkCTreeNode *node = GTK_CTREE_NODE(g_list_nth(clist->row_list,
                                                       clist->click_cell.row));
        if (node)
            gtk_ctree_select(ctree, node);

        newlist = getTreeSelection(ctree);

        if (newlist->size() == 0)
            return;
    }

    vector<PlaylistItem *> *test = 
         (vector<PlaylistItem *> *)g_dataset_get_data(context, 
                                                      "tree-drag-source");
    if (!test) 
        g_dataset_set_data_full(context, "tree-drag-source", newlist,
                                tree_source_destroy);
}

static void tree_drag_data_get(GtkWidget *w, GdkDragContext *context,
                               GtkSelectionData *selection_data, guint info,
                               guint time, GtkWidget *widget)
{
    if (selection_data->target == gdk_atom_intern("tree-drag", FALSE)) {
        vector<PlaylistItem *> *newlist = 
                (vector<PlaylistItem *> *)g_dataset_get_data(context,
                                                             "tree-drag-source");
        gtk_selection_data_set(selection_data, selection_data->target,
                               0, (guchar *)&newlist, sizeof(vector<PlaylistItem *>*));
    }
}

static void tree_status(GtkWidget *w, GdkEventMotion *event, 
                        GTKMusicBrowser *p)
{
    int x, y, row, column;
    GtkCTree *ctree = GTK_CTREE(w);
    GtkCList *clist = GTK_CLIST(w);

    x = (int)event->x;
    y = (int)event->y;

    if (!gtk_clist_get_selection_info(clist, x, y, &row, &column)) {
        gdk_threads_leave();
        p->AcceptEvent(new BrowserMessageEvent(" "));
        gdk_threads_enter();
        return;
    }

    GtkCTreeNode *node = GTK_CTREE_NODE(g_list_nth(clist->row_list, row));
    TreeData *data = (TreeData *)gtk_ctree_node_get_row_data(ctree, node);

    gdk_threads_leave();
    switch (data->type) {
        case kTreeMyMusic:
        case kTreeAll:
        case kTreeUncat:
        case kTreePlaylistHead:
        case kTreePlaylist:
            p->AcceptEvent(new BrowserMessageEvent(data->message.c_str()));
            break;
        default:
            p->AcceptEvent(new BrowserMessageEvent(" "));
            break;
    }
    gdk_threads_enter();
}

static void tree_status_clear(GtkWidget *w, GdkEventCrossing *event, 
                              GTKMusicBrowser *p)
{
    gdk_threads_leave();
    p->AcceptEvent(new BrowserMessageEvent(" "));
    gdk_threads_enter();
}

static gint nocase_compare(GtkCList *clist, gconstpointer ptr1, 
                           gconstpointer ptr2)
{
    char *text1 = NULL;
    char *text2 = NULL;

    GtkCListRow *row1 = (GtkCListRow *) ptr1;
    GtkCListRow *row2 = (GtkCListRow *) ptr2;

    switch (row1->cell[clist->sort_column].type) {
        case GTK_CELL_TEXT:
            text1 = GTK_CELL_TEXT (row1->cell[clist->sort_column])->text;
            break;
        case GTK_CELL_PIXTEXT:
            text1 = GTK_CELL_PIXTEXT (row1->cell[clist->sort_column])->text;
            break;
        default:
            break;
    }

    switch (row2->cell[clist->sort_column].type) {
        case GTK_CELL_TEXT:
            text2 = GTK_CELL_TEXT (row2->cell[clist->sort_column])->text;
            break;
        case GTK_CELL_PIXTEXT:
            text2 = GTK_CELL_PIXTEXT (row2->cell[clist->sort_column])->text;
            break;
        default:
            break;
    }

    if (!text2)
        return (text1 != NULL);

    if (!text1)
        return -1;

    return strcasecmp (text1, text2);
}

static gint TreeDataCompare(TreeData *a, TreeData *b)
{
    bool retvalue = true;
    if ((a->type == b->type) && (a->catalog == b->catalog) && 
        (a->artist == b->artist) && (a->album == b->album) && 
        (a->track == b->track) && (a->playlistname == b->playlistname))
        retvalue = false;
    return retvalue;
}
           
GtkCTreeNode *GTKMusicBrowser::FindNode(TreeNodeType type, ArtistList *artist, 
                                        AlbumList  *album, PlaylistItem *item,
                                        GtkCTreeNode *searchFrom)
{
    TreeData *data = NewTreeData(type, NULL, artist, album, item);
    GtkCTreeNode *retnode;

    retnode = gtk_ctree_find_by_row_data_custom(musicBrowserTree, searchFrom,
                                                data, 
                                                (GCompareFunc)TreeDataCompare);
    delete data;
    return retnode;
}

GtkCTreeNode *GTKMusicBrowser::FindPlaylistNode(string playlist)
{
    GtkCTreeNode *retnode;
    TreeData *data = NewTreeData(kTreePlaylist, NULL, NULL, NULL, NULL, 
                                 (char *)playlist.c_str());

    retnode = gtk_ctree_find_by_row_data_custom(musicBrowserTree, playlistTree,
                                                data, 
                                                (GCompareFunc)TreeDataCompare);

    delete data;
    return retnode;
}

void GTKMusicBrowser::RemoveCatTrack(ArtistList *artist, AlbumList *album,
                                     PlaylistItem *item)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    GtkCTreeNode *toRemove;

    toRemove = FindNode(kTreeTrack, artist, album, item, allTree);
 
    if (toRemove)
        gtk_ctree_remove_node(musicBrowserTree, toRemove);

    toRemove = FindNode(kTreeTrack, artist, album, item);

    if (toRemove) {
        GtkCTreeRow *row = GTK_CTREE_ROW(toRemove);
        GtkCTreeNode *alb = row->parent;
        GtkCTreeNode *art = GTK_CTREE_ROW(alb)->parent;

        gtk_ctree_remove_node(musicBrowserTree, toRemove);
        if (!GTK_CTREE_ROW(alb)->children) {
            gtk_ctree_remove_node(musicBrowserTree, alb);
            if (!GTK_CTREE_ROW(art)->children) 
                gtk_ctree_remove_node(musicBrowserTree, art);
        }
    }

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::AddCatTrack(ArtistList *artist, AlbumList *album,
                                  PlaylistItem *item, bool expand)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    char      *name[1];
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);
    TreeData  *data;

    if (!artist) {
        GtkCTreeNode *treeItem;
        MetaData mdata = item->GetMetaData();
        name[0] = (char *)mdata.Title().c_str();
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          track_pix);
        treeItem = gtk_ctree_insert_node(musicBrowserTree, uncatTree,
                                         NULL, name, 5,
                                         pixmap, mask, pixmap, mask, true,
                                         false);
        data = NewTreeData(kTreeTrack, NULL, NULL, NULL, item);
        gtk_ctree_node_set_row_data(musicBrowserTree, treeItem, data);
        if (expand) {
            gtk_ctree_expand(musicBrowserTree, uncatTree);
            gtk_ctree_select(musicBrowserTree, treeItem);
            gtk_ctree_node_moveto(musicBrowserTree, treeItem, 0, 0.5, 0);
        }

        treeItem = gtk_ctree_insert_node(musicBrowserTree, allTree,
                                         NULL, name, 5, pixmap, mask, pixmap,
                                         mask, true, false);
        data = NewTreeData(kTreeTrack, NULL, NULL, NULL, item);
        gtk_ctree_node_set_row_data(musicBrowserTree, treeItem, data);

        //gtk_ctree_sort_recursive(musicBrowserTree, allTree);
        //gtk_ctree_sort_recursive(musicBrowserTree, uncatTree);
        gtk_clist_thaw(GTK_CLIST(musicBrowserTree));

        return;
    }

    GtkCTreeNode *artTree, *albTree, *newItem;

    albTree = FindNode(kTreeAlbum, artist, album, NULL);
    if (!albTree) {
        artTree = FindNode(kTreeArtist, artist, NULL, NULL);
        if (!artTree) {
            name[0] = (char *)artist->name.c_str();
            pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, 
                                                  &mask,
                                                  &style->bg[GTK_STATE_NORMAL],
                                                  artist_pix);
            artTree = gtk_ctree_insert_node(musicBrowserTree, mainTree,
                                            NULL, name, 5, pixmap, mask,
                                            pixmap, mask, false, false);
            data = NewTreeData(kTreeArtist, NULL, artist);
            gtk_ctree_node_set_row_data(musicBrowserTree, artTree, data); 
            gtk_ctree_expand(musicBrowserTree, artTree);
        }
        name[0] = (char *)album->name.c_str();
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window,
                                              &mask, 
                                              &style->bg[GTK_STATE_NORMAL],
                                              album_pix);
        albTree = gtk_ctree_insert_node(musicBrowserTree, artTree,
                                          NULL, name, 5, pixmap, mask,
                                          pixmap, mask, false, false);
        data = NewTreeData(kTreeAlbum, NULL, artist, album);
        gtk_ctree_node_set_row_data(musicBrowserTree, albTree, data);
        gtk_ctree_expand(musicBrowserTree, albTree);
    }

    name[0] = (char *)item->GetMetaData().Title().c_str();
    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window,
                                          &mask, &style->bg[GTK_STATE_NORMAL],
                                          track_pix);
    newItem = gtk_ctree_insert_node(musicBrowserTree, albTree, NULL, name, 5, 
                                    pixmap, mask, pixmap, mask, true, false);
    data = NewTreeData(kTreeTrack, NULL, artist, album, item);
    gtk_ctree_node_set_row_data(musicBrowserTree, newItem, data);
    if (expand) {
        gtk_ctree_expand_recursive(musicBrowserTree, newItem);
        gtk_ctree_select(musicBrowserTree, newItem);
        gtk_ctree_node_moveto(musicBrowserTree, newItem, 0, 0.5, 0);
    }

    newItem = gtk_ctree_insert_node(musicBrowserTree, allTree, NULL, name, 5, 
                                    pixmap, mask, pixmap, mask, true, false);
    data = NewTreeData(kTreeTrack, NULL, artist, album, item);
    gtk_ctree_node_set_row_data(musicBrowserTree, newItem, data);

    //gtk_ctree_sort_recursive(musicBrowserTree, allTree);
    //gtk_ctree_sort_node(musicBrowserTree, albTree);
    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::ClearTree(void)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));
    gtk_clist_clear(GTK_CLIST(musicBrowserTree));

    CreateMainTreeItems();

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::AddCatPlaylist(string playlist)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    char *fullname = new char[playlist.length() + 1];
    strcpy(fullname, playlist.c_str());
    char *listname = fullname;
    char *temp = strrchr(fullname, '.');
    if (temp)
        *temp = '\0';
    temp = strrchr(fullname, '/');
    if (temp)
        listname = temp + 1;

    char *name[1];
    name[0] = listname;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);
    TreeData  *data;
    GtkCTreeNode *allItem;
    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          playlist_pix);
    allItem = gtk_ctree_insert_node(musicBrowserTree, playlistTree, NULL,
                                    name, 5, pixmap, mask, pixmap, mask,
                                    true, false);
    data = NewTreeData(kTreePlaylist, NULL, NULL, NULL, NULL,
                       (char *)playlist.c_str(), (char *)playlist.c_str());
    gtk_ctree_node_set_row_data(musicBrowserTree, allItem, data);

    delete [] fullname;

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::RemoveCatPlaylist(string playlist)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    GtkCTreeNode *toRemove;

    toRemove = FindPlaylistNode(playlist);

    if (toRemove)
        gtk_ctree_remove_node(musicBrowserTree, toRemove);

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::CreateMainTreeItems(void)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    TreeData *data;
    char *name[1];
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          catalog_pix);
    name[0] = "My Music";
    mainTree = gtk_ctree_insert_node(musicBrowserTree, NULL, NULL, name, 5,
                                     pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeMyMusic, NULL, NULL, NULL, NULL, NULL,
                       "This tree item contains all of your music");
    gtk_ctree_node_set_row_data(musicBrowserTree, mainTree, data);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask, 
                                          &style->bg[GTK_STATE_NORMAL],
                                          all_pix);
    name[0] = "All Tracks";
    allTree = gtk_ctree_insert_node(musicBrowserTree, mainTree, NULL, name, 5,
                                    pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeAll, m_musicCatalog, NULL, NULL, NULL, NULL,
                       "This tree item lists all of your music tracks");
    gtk_ctree_node_set_row_data(musicBrowserTree, allTree, data);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          uncatagorized_pix);
    name[0] = "Uncategorized Tracks";
    uncatTree = gtk_ctree_insert_node(musicBrowserTree, mainTree, NULL,
                                      name, 5, pixmap, mask, pixmap, mask,
                                      false, false);
    data = NewTreeData(kTreeUncat, m_musicCatalog, NULL, NULL, NULL, NULL,
                       "This tree item lists all of your uncategorized music "
                       "tracks");
    gtk_ctree_node_set_row_data(musicBrowserTree, uncatTree, data);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          playlist_pix);
    name[0] = "My Playlists";
    playlistTree = gtk_ctree_insert_node(musicBrowserTree, NULL, NULL, name, 5,
                                         pixmap, mask, pixmap, mask, false,
                                         false);
    data = NewTreeData(kTreePlaylistHead, NULL, NULL, NULL, NULL, NULL,
                       "This tree item contains all of your playlists");
    gtk_ctree_node_set_row_data(musicBrowserTree, playlistTree, data);

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}
    
void GTKMusicBrowser::UpdateCatalog(void)

{
    m_musicCatalog = m_context->catalog;
    static bool triedUpdate = false;

    m_musicCatalog->GetCatalogLock();

    vector<ArtistList *> *artistList = 
                         (vector<ArtistList *> *)m_musicCatalog->GetMusicList();
    vector<PlaylistItem *> *unsorted = 
                   (vector<PlaylistItem *> *)m_musicCatalog->GetUnsortedMusic();
    vector<string> *playlists = 
                               (vector<string> *)m_musicCatalog->GetPlaylists();

    if ((artistList->size() == 0) && (unsorted->size() == 0) && 
        (playlists->size() == 0) && (triedUpdate == false)) {
        GTKMessageDialog oBox;
        string oMessage = string("Your music database does not contain any items.  Would you like to start a music search to find music and playlists on your machine?"); 
       
        triedUpdate = true;
        if (oBox.Show(oMessage.c_str(), "MusicBrowser", kMessageYesNo)
            == kMessageReturnYes)
            StartMusicSearch(false); 
        m_musicCatalog->ReleaseCatalogLock();
        return;
    }

    if ((artistList->size() == 0) && (unsorted->size() == 0) &&
        (playlists->size() == 0)) {
        m_musicCatalog->ReleaseCatalogLock(); 
        return;
    }

    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    ClearTree();

    GtkCTreeNode *allItem, *uncatItem;
    TreeData *data;
    char *name[1];

    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);

    uncatItem = NULL;
    allItem = NULL;
    vector<PlaylistItem *>::iterator l = unsorted->begin();
    for (; l != unsorted->end(); l++) {
        MetaData mdata = (*l)->GetMetaData();
        name[0] = (char *)mdata.Title().c_str();
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          track_pix);
        uncatItem = gtk_ctree_insert_node(musicBrowserTree, uncatTree, 
                                          NULL, name, 5, 
                                          pixmap, mask, pixmap, mask, true, 
                                          false);
        data = NewTreeData(kTreeTrack, NULL, NULL, NULL, *l);
        gtk_ctree_node_set_row_data(musicBrowserTree, uncatItem, data);

        allItem = gtk_ctree_insert_node(musicBrowserTree, allTree,
                                        NULL, name, 5, pixmap, mask, pixmap, 
                                        mask, true, false);
        data = NewTreeData(kTreeTrack, NULL, NULL, NULL, *l);
        gtk_ctree_node_set_row_data(musicBrowserTree, allItem, data);
    }

    vector<ArtistList *>::iterator i = artistList->begin();
    for (; i != artistList->end(); i++) {
        GtkCTreeNode *artTree, *artItem = NULL;

        name[0] = (char *)(*i)->name.c_str();
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          artist_pix);
        artTree = gtk_ctree_insert_node(musicBrowserTree, mainTree,
                                        NULL, name, 5, pixmap, mask, 
                                        pixmap, mask, false, false);
        data = NewTreeData(kTreeArtist, NULL, (*i));
        gtk_ctree_node_set_row_data(musicBrowserTree, artTree, data);

        vector<AlbumList *>::iterator j = (*i)->m_albumList->begin();
        for (; j != (*i)->m_albumList->end(); j++) {
            GtkCTreeNode *trackItem = NULL;

            name[0] = (char *)(*j)->name.c_str();
            pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, 
                                          &mask, &style->bg[GTK_STATE_NORMAL],
                                          album_pix);
            artItem = gtk_ctree_insert_node(musicBrowserTree, artTree,
                                            NULL, name, 5, pixmap, mask, 
                                            pixmap, mask, false, false);
            data = NewTreeData(kTreeAlbum, NULL, (*i), (*j));
            gtk_ctree_node_set_row_data(musicBrowserTree, artItem, data);

            vector<PlaylistItem *>::iterator k = (*j)->m_trackList->begin();
            for (;k != (*j)->m_trackList->end(); k++) {
                name[0] = (char *)(*k)->GetMetaData().Title().c_str();
                pixmap =gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, 
                                          &mask, &style->bg[GTK_STATE_NORMAL],
                                          track_pix);
                trackItem = gtk_ctree_insert_node(musicBrowserTree, artItem,
                                                  NULL, name,  
                                                  5, pixmap, mask, pixmap, mask,
                                                  true, false);
                data = NewTreeData(kTreeTrack, NULL, (*i), (*j), (*k));
                gtk_ctree_node_set_row_data(musicBrowserTree, trackItem, data);


                allItem = gtk_ctree_insert_node(musicBrowserTree, allTree,
                                                NULL, name,
                                                5, pixmap, mask, pixmap, mask,
                                                true, false);
                data = NewTreeData(kTreeTrack, NULL, (*i), (*j), (*k));
                gtk_ctree_node_set_row_data(musicBrowserTree, allItem, data);
            }
        }
    }

    gtk_ctree_sort_recursive(musicBrowserTree, allTree);

    vector<string>::iterator m = playlists->begin();
    for (; m != playlists->end(); m++) {
        char *fullname = new char[(*m).length() + 1];
        strcpy(fullname, (*m).c_str());
        char *listname = fullname;
        char *temp = strrchr(fullname, '.');
        if (temp)
            *temp = '\0';
        temp = strrchr(fullname, '/');
        if (temp)
            listname = temp + 1;

        name[0] = listname;
        allItem = NULL;
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          playlist_pix);
        allItem = gtk_ctree_insert_node(musicBrowserTree, playlistTree, NULL, 
                                        name, 5, pixmap, mask, pixmap, mask, 
                                        true, false);
        data = NewTreeData(kTreePlaylist, NULL, NULL, NULL, NULL,
                           (char *)(*m).c_str(), (char *)(*m).c_str());
        gtk_ctree_node_set_row_data(musicBrowserTree, allItem, data);

        delete [] fullname;
    } 
    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));

    m_musicCatalog->ReleaseCatalogLock();
}

static void music_search(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->StartMusicSearch();
}

void GTKMusicBrowser::AddFileCMD()
{
    FileSelector *filesel = new FileSelector("Select a file to play");
    if (filesel->Run(false)) {
        char *returnpath = filesel->GetReturnPath();
        char *ext = m_context->player->GetExtension(returnpath);
        uint32 length = strlen(returnpath) + 10;
        char *tempurl = new char[length];
        if (IsntError(FilePathToURL(returnpath, tempurl, &length))) {

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
        delete [] tempurl;
        delete ext;

        m_currentindex = 0; 
        PlayEvent();
    }
    delete filesel;
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
            if (m_context->plm->IsSupportedPlaylistFormat(ext))
                p->ImportPlaylist(tempurl);
            else if (m_context->player->IsSupportedExtension(ext)) {
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
        if (p->GetTreeClick() == kClickPlaylist) {
            p->GetContext()->catalog->RemovePlaylist(p->mbSelection->playlistname.c_str());
        }
        else if (p->GetTreeClick() == kClickTrack) {
            p->GetContext()->catalog->RemoveSong(p->mbSelection->track->URL().c_str());
        }
    }
}

static void edit_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    if (p->GetClickState() == kContextPlaylist) { 
        p->PopUpInfoEditor();
    }
    else if (p->GetClickState() == kContextBrowser) {
        if (p->GetTreeClick() == kClickPlaylist) {
            p->CreateNewEditor((char *)p->mbSelection->playlistname.c_str());
        }
        else if (p->GetTreeClick() == kClickTrack) {
            p->PopUpInfoEditor(p->mbSelection->track);
        }
    }
}
    
void GTKMusicBrowser::CreateExpanded(void)
{
    GtkTargetEntry tree_target_table = {"tree-drag", 0, 1};

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

    musicBrowserTree = GTK_CTREE(gtk_ctree_new(1, 0));
    gtk_widget_add_events(GTK_WIDGET(musicBrowserTree), 
                          GDK_POINTER_MOTION_MASK | GDK_LEAVE_NOTIFY_MASK);
    gtk_container_add(GTK_CONTAINER(musicBrowserWindow),
                      GTK_WIDGET(musicBrowserTree));
    gtk_signal_connect_after(GTK_OBJECT(musicBrowserTree), "button_press_event",
                             GTK_SIGNAL_FUNC(tree_clicked), this);
    gtk_signal_connect(GTK_OBJECT(musicBrowserTree),"motion_notify_event",
                       GTK_SIGNAL_FUNC(tree_status), this);
    gtk_signal_connect(GTK_OBJECT(musicBrowserTree), "leave_notify_event",
                       GTK_SIGNAL_FUNC(tree_status_clear), this);
    gtk_drag_source_set(GTK_WIDGET(musicBrowserTree), GDK_BUTTON1_MASK, 
                        &tree_target_table, 1, GDK_ACTION_MOVE);
    gtk_signal_connect(GTK_OBJECT(musicBrowserTree), "drag_data_get",
                             GTK_SIGNAL_FUNC(tree_drag_data_get), this);
    gtk_signal_connect(GTK_OBJECT(musicBrowserTree), "drag_begin",
                       GTK_SIGNAL_FUNC(tree_drag_begin), this);
    gtk_clist_set_compare_func(GTK_CLIST(musicBrowserTree), nocase_compare);

    CreateMainTreeItems();

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
    if (m_state == kStateCollapsed) {
        CreateExpanded();
        m_state = kStateExpanded;
        if (lastPanedPosition != -1) {
            gtk_paned_set_position(GTK_PANED(masterBox), lastPanedPosition);
            gtk_paned_set_handle_size(GTK_PANED(masterBox), lastPanedHandle);
        }
        gtk_window_set_title(GTK_WINDOW(musicBrowser), BRANDING" - My Music");
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
        gtk_window_set_title(GTK_WINDOW(musicBrowser), BRANDING" - Playlist Editor");
        GtkWidget *w = gtk_item_factory_get_widget(menuFactory,
                                                   "/View/View Playlist Only");
        gtk_container_foreach(GTK_CONTAINER(w), set_label_menu, (gpointer)"View My Music");
    }
}

void GTKMusicBrowser::ToggleVisEvent(void)
{
    gdk_threads_leave();
    Close();
    gdk_threads_enter();
}

void GTKMusicBrowser::ToggleVisEventDestroyed(void)
{
    ToggleVisEvent();
    m_initialized = false;
    musicBrowserTree = NULL;
    m_browserCreated = false; 
}

static gboolean toggle_vis_destroy(GtkWidget *w, GTKMusicBrowser *p)
{
    p->ToggleVisEventDestroyed();
    return FALSE;
}

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
            if (p->GetContext()->plm->IsSupportedPlaylistFormat(ext)) {
                string tobeloaded = tempurl;
                p->LoadPlaylist(tobeloaded);
            }
            else {
                char *filereturn = strdup_new(filesel->GetReturnPath());
                if (filereturn) {
                    char *temp;
                    char *first= strtok(filereturn, "\n");

                    while ((temp = strtok(NULL, "\n"))) {
                        p->AddTrackPlaylistEvent(temp);
                        p->m_currentindex++;
                    }
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

static void playlist_row_move_internal(GtkWidget *widget, int source, int dest, 
                                       GTKMusicBrowser *p)
{
    p->MoveItemEvent(source, dest);
    p->SetClickState(kContextPlaylist);
}

static void set_current_index_internal(GtkWidget *widget, int row, int column, 
                                       GdkEventButton *button, 
                                       GTKMusicBrowser *p)
{
    p->m_currentindex = row;
    if (button && button->type == GDK_2BUTTON_PRESS)
        p->PlayEvent();
    p->SetClickState(kContextPlaylist);
}

static void quit_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ToggleVisEvent();
}

static void sort_artist(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Artist, PlaylistSortType_Ascending);
}

static void sort_album(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Album, PlaylistSortType_Ascending);
}

static void sort_title(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Title, PlaylistSortType_Ascending);

}

static void sort_year(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Year, PlaylistSortType_Ascending);
}

static void sort_track(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Track, PlaylistSortType_Ascending);
}

static void sort_genre(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Genre, PlaylistSortType_Ascending);
}

static void sort_time(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Time, PlaylistSortType_Ascending);
}

static void sort_location(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Location, PlaylistSortType_Ascending);
}

static void sort_normal(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->iSetShuffleMode = true;
    p->SortPlaylistEvent(kPlaylistSortKey_Random, PlaylistSortType_Descending);
}

static void sort_random2(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->iSetShuffleMode = true;
    p->SortPlaylistEvent(kPlaylistSortKey_Random, PlaylistSortType_Ascending);
}

static void sort_random(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SortPlaylistEvent(kPlaylistSortKey_Random, PlaylistSortType_Ascending);
}

static void catalog_tog(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ExpandCollapseEvent();
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

    if (oBox.Show(oMessage.c_str(), "Delete Confirmation", kMessageOkCancel,
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
    save_list(p, 0, w);
}

static void export_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    FileSelector *filesel = new FileSelector("Export This Playlist to Disk");
    if (filesel->Run())
        p->SaveCurrentPlaylist(filesel->GetReturnPath());
    delete filesel;
}

static void import_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    import_tool(w, p);
}

static void add_track_mb(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    if (p->GetClickState() == kContextBrowser) {
        vector<PlaylistItem *> *newlist = getTreeSelection(p->musicBrowserTree);
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
        if (p->GetTreeClick() == kClickPlaylist) 
            urlToDel = p->mbSelection->playlistname;
        else if (p->GetTreeClick() == kClickTrack)
            urlToDel = p->mbSelection->track->URL();
        else
            return;
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
                if (p->GetTreeClick() == kClickPlaylist) {
                    p->GetContext()->catalog->RemovePlaylist(p->mbSelection->playlistname.c_str());
                }
                else if (p->GetTreeClick() == kClickTrack) {
                    p->GetContext()->catalog->RemoveSong(p->mbSelection->track->URL().c_str());
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

void GTKMusicBrowser::CreateMenu(GtkWidget *topbox)
{
    GtkAccelGroup *accel_group;
    GtkWidget *separator;

    accel_group = gtk_accel_group_new();
    menuFactory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<blah>", 
                                        accel_group); 
    CreateMenuItems(menuFactory, (void *)this);
    
    gtk_accel_group_attach(accel_group, GTK_OBJECT(musicBrowser));
    gtk_box_pack_start(GTK_BOX(topbox), gtk_item_factory_get_widget(
                       menuFactory, "<blah>"), FALSE, TRUE, 0);
    gtk_widget_show(gtk_item_factory_get_widget(menuFactory, "<blah>"));
    
    separator = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(topbox), separator, FALSE, TRUE, 0);
    gtk_widget_show(separator);
}

void GTKMusicBrowser::UpdatePlaylistList(void)
{
    if (!playlistList || !m_plm)
        return;

    gtk_clist_freeze(GTK_CLIST(playlistList));
    gtk_clist_clear(GTK_CLIST(playlistList));

    uint32 iLoop = m_plm->CountItems();

    for (uint32 i = 0; i < iLoop; i++) {
        PlaylistItem *item = m_plm->ItemAt(i);

        if (!item)
            continue;

        while (item->GetState() == kPlaylistItemState_RetrievingMetaData)
           usleep(10);

        MetaData mdata = item->GetMetaData();
        char *iText[4];
        char position[10];
        char *title;
        char *artist;
        char length[50];

        sprintf(position, "%d", i + 1);
        title = (char *)mdata.Title().c_str();
        artist = (char *)mdata.Artist().c_str();

        if (mdata.Time() == 0)
            sprintf(length, "Unknown");
        else {
            int secs = mdata.Time();
            if (secs > 3600)
                sprintf(length, "%d:%02d:%02d", secs / 3600, (secs / 60) % 60, 
                        secs % 60);
            else
                sprintf(length, "%d:%02d", (secs / 60) % 60, secs % 60);
        }

        iText[0] = position;
        iText[1] = title;
        iText[2] = artist;
        iText[3] = length;

        gtk_clist_append(GTK_CLIST(playlistList), iText);
    }

    if (iLoop == 0) {
        gtk_clist_set_column_width(GTK_CLIST(playlistList), 0, 10);
        gtk_clist_set_column_width(GTK_CLIST(playlistList), 1, 45);
        gtk_clist_set_column_width(GTK_CLIST(playlistList), 2, 45);
    }
    else
        gtk_clist_columns_autosize(GTK_CLIST(playlistList));
    gtk_clist_select_row(GTK_CLIST(playlistList), m_currentindex, 0);
    gtk_clist_moveto(GTK_CLIST(playlistList), m_currentindex, 0, 0.5, -1);
    gtk_clist_thaw(GTK_CLIST(playlistList));
}

void list_keypress(GtkWidget *w, GdkEventKey *ev, GTKMusicBrowser *p)
{
    if (ev->keyval == GDK_Delete)
        p->DeleteEvent();
}

void GTKMusicBrowser::CreatePlaylistList(GtkWidget *box)
{
    static char *titles[] =
    {
      "# ", "Title", "Artist", "Length"
    };

    playlistList = gtk_clist_new_with_titles(4, titles);
    gtk_container_add(GTK_CONTAINER(box), playlistList);
    gtk_signal_connect(GTK_OBJECT(playlistList), "row_move",
                       GTK_SIGNAL_FUNC(playlist_row_move_internal), this);   
    gtk_signal_connect(GTK_OBJECT(playlistList), "select_row",
                       GTK_SIGNAL_FUNC(set_current_index_internal), this);
    GtkTargetEntry new_clist_target_table[2] = {
        {"gtk-clist-drag-reorder", 0, 0},
        {"tree-drag", 0, 1} 
    };
    GTK_CLIST_SET_FLAG(GTK_CLIST(playlistList), CLIST_REORDERABLE);
    gtk_drag_dest_set(playlistList, (GtkDestDefaults)(GTK_DEST_DEFAULT_MOTION | 
                      GTK_DEST_DEFAULT_DROP), 
                      (GtkTargetEntry *)&new_clist_target_table, 2,
                      GDK_ACTION_MOVE);
    gtk_signal_connect(GTK_OBJECT(playlistList), "drag_leave",  
                       GTK_SIGNAL_FUNC(list_drag_leave_internal), this);
    gtk_signal_connect(GTK_OBJECT(playlistList), "drag_drop", 
                       GTK_SIGNAL_FUNC(list_drag_drop_internal), this);
    gtk_signal_connect(GTK_OBJECT(playlistList), "drag_data_received",
                       GTK_SIGNAL_FUNC(list_drag_rec_internal), this);
    gtk_signal_connect(GTK_OBJECT(playlistList), "drag_motion",
                       GTK_SIGNAL_FUNC(list_drag_motion_internal), this);
    gtk_signal_connect(GTK_OBJECT(playlistList), "key_press_event",
                       GTK_SIGNAL_FUNC(list_keypress), this);

    gtk_clist_columns_autosize(GTK_CLIST(playlistList));
    gtk_widget_show(playlistList);

    m_currentindex = m_plm->GetCurrentIndex();

    UpdatePlaylistList();
}

void GTKMusicBrowser::SetStatusText(const char *text)
{
    if (statusContext > 0) 
        gtk_statusbar_pop(GTK_STATUSBAR(statusBar), statusContext);
    else
        statusContext = 1;

    gtk_statusbar_push(GTK_STATUSBAR(statusBar), 1, text);
}

void GTKMusicBrowser::LoadPlaylist(string &oPlaylist)
{
    if (oPlaylist == m_currentListName)
        return;

    if (oPlaylist.length() == 0) 
        return;

    if (m_currentListName.length() != 0)
        SaveCurrentPlaylist(NULL);

    if (!strncmp("file://", oPlaylist.c_str(), 7)) {
        m_plm->ReadPlaylist(oPlaylist.c_str());
        m_currentListName = oPlaylist;
        UpdatePlaylistList();
    }
    else {
        uint32 length = _MAX_PATH;
        char *PlaylistURL = new char[length];
        if (IsntError(FilePathToURL(oPlaylist.c_str(), PlaylistURL, &length))) {
            m_plm->ReadPlaylist(PlaylistURL);
            m_currentListName = PlaylistURL;
            UpdatePlaylistList();
        }
        delete [] PlaylistURL;
    }
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
        if (m_currentListName.length() == 0)
            titlestr += string("New Playlist");
        else
            titlestr += string("Editing playlist ") + m_currentListName;
    }
    gtk_window_set_title(GTK_WINDOW(musicBrowser), titlestr.c_str());
    gtk_window_set_policy(GTK_WINDOW(musicBrowser), TRUE, TRUE, FALSE);
    gtk_signal_connect(GTK_OBJECT(musicBrowser), "destroy",
                       GTK_SIGNAL_FUNC(toggle_vis_destroy), this);
    gtk_container_set_border_width(GTK_CONTAINER(musicBrowser), 0);
    gtk_widget_realize(musicBrowser);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(musicBrowser), vbox);
    gtk_widget_show(vbox);

    statusBar = gtk_statusbar_new();
    
    gtk_box_pack_end(GTK_BOX(vbox), statusBar, FALSE, TRUE, 0);
    gtk_widget_show(statusBar);

    CreateMenu(vbox);

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
        string labelstr = string("Editing playlist: ");
        if (m_currentListName.length() == 0) 
            labelstr += string("New Playlist");
        else
            labelstr += m_currentListName;
        playlistLabel = gtk_label_new(labelstr.c_str());
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

    CreatePlaylistList(playlistwindow);

    SetClickState(kContextNone);
    gtk_widget_show(musicBrowser);

    m_state = kStateCollapsed;
}

void GTKMusicBrowser::DeleteListEvent(void)
{
    m_plm->RemoveAll();
    m_context->target->AcceptEvent(new Event(CMD_Stop));
    UpdatePlaylistList();
    m_currentindex = kInvalidIndex;
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

void GTKMusicBrowser::SetClickState(ClickState newState)
{
    if ((m_clickState == newState) && (newState != kContextPlaylist))
        return;

    m_clickState = newState;
    if (m_clickState == kContextPlaylist) {
        gtk_widget_set_sensitive(toolRemove, TRUE);
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
                                 "/Edit/Remove Items from My Music"), TRUE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Move Up"), FALSE);
        gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                                 "/Edit/Move Down"), FALSE);
        gtk_clist_unselect_all(GTK_CLIST(playlistList));

        if (m_mbState == kClickNone) {
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
        else if (m_mbState == kClickPlaylist) {
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
        else {
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
    }
    if (master && stopped && (m_plm->CountItems() - 1 > deleteindex)) {
        m_plm->SetCurrentIndex(m_currentindex + 1);
        m_context->target->AcceptEvent(new Event(CMD_Play));
    }
    m_plm->RemoveItem(deleteindex);
    UpdatePlaylistList();
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

    if (additReally) {
        m_plm->AddItem(tempurl, m_currentindex);
        UpdatePlaylistList();
    }

    if (needToDelete)
        delete [] tempurl;
}

void GTKMusicBrowser::AddTrackPlaylistEvent(PlaylistItem *newitem)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    m_plm->AddItem(newitem, m_currentindex, true);
    UpdatePlaylistList();
}

void GTKMusicBrowser::AddTracksPlaylistEvent(vector<PlaylistItem *> *newlist,
                                             bool end)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    else if (end)
        m_currentindex = m_plm->CountItems();
    m_plm->AddItems(newlist, m_currentindex, true);
    UpdatePlaylistList();
}

void GTKMusicBrowser::PlayEvent(void)
{
    m_plm->SetCurrentIndex(m_currentindex);
    m_context->target->AcceptEvent(new Event(CMD_Play));
}

void GTKMusicBrowser::StartMusicSearch(bool runMain)
{
    parentUI->StartSearch(runMain);
}

void GTKMusicBrowser::SortPlaylistEvent(PlaylistSortKey order, PlaylistSortType
                                        type)
{
    if (order == kPlaylistSortKey_Random) {
        if (type == PlaylistSortType_Ascending)
            m_plm->SetShuffleMode(true);
        else
            m_plm->SetShuffleMode(false);
    }
    else
        m_plm->Sort(order, type);
    UpdatePlaylistList();
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
                                 string playlistURL)
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

    parentUI = masterUI;
 
    if (playlistURL.find("currentlist.m3u") <= playlistURL.length()) {
        m_plm = context->plm;
        BADContext = context;
        master = true;
    }
    else {
        m_plm = new PlaylistManager(context);
        m_plm->SetActivePlaylist(kPlaylistKey_ExternalPlaylist);
        master = false;
    }
  
    LoadPlaylist(playlistURL);
}

GTKMusicBrowser::~GTKMusicBrowser(void)
{
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
    gdk_threads_leave();
}

void GTKMusicBrowser::ShowMusicBrowser(void)
{
    gdk_threads_enter();
    isVisible = true;
    if (m_initialized)
        gtk_widget_show(musicBrowser);
    else {
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
    }

    SetToolbarType();
    gdk_threads_leave();
}

void GTKMusicBrowser::Close(bool inMain)
{
    gdk_threads_enter();
    isVisible = false;

    if (m_plm && m_currentListName.length() == 0 && m_plm->CountItems() > 0) {
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

int32 GTKMusicBrowser::AcceptEvent(Event *e)
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
            if (master) {
                gdk_threads_enter();
                UpdatePlaylistList();
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogTrackAdded: {
            MusicCatalogTrackAddedEvent *mct = (MusicCatalogTrackAddedEvent *)e;
            if (m_initialized) {
                gdk_threads_enter();
                AddCatTrack((ArtistList *)mct->Artist(), 
                            (AlbumList *)mct->Album(), 
                            (PlaylistItem *)mct->Item(), false);
                gdk_threads_leave();
            }
            break; }
        case INFO_MusicCatalogPlaylistAdded: {
            MusicCatalogPlaylistAddedEvent *mcp = 
                              (MusicCatalogPlaylistAddedEvent *)e;
            if (m_initialized) {
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
                gdk_threads_leave();
            }
            break; }
        case INFO_PlaylistCurrentItemInfo: {
            m_playingindex = m_plm->GetCurrentIndex();
            break; } 
        default:
            break;
    }
    return 0;
}

void GTKMusicBrowser::CreateNewEditor(char *playlisturl)
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
    parentUI->CreateNewEditor(newURL);
}

