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

#include "../res/test.xpm"

extern "C" {
void new_plist(GTKMusicBrowser *p, guint action, GtkWidget *w);
void open_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void save_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void saveas_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void import_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void music_search(GTKMusicBrowser *p, guint action, GtkWidget *w);
void quit_menu(GTKMusicBrowser *p, guint action, GtkWidget *w);
void add_track(GTKMusicBrowser *p, guint action, GtkWidget *w);
void infoedit(GTKMusicBrowser *p, guint action, GtkWidget *w);
void delete_sel(GTKMusicBrowser *p, guint action, GtkWidget *w);
void realdelete_sel(GTKMusicBrowser *p, guint action, GtkWidget *w);
void move_up(GTKMusicBrowser *p, guint action, GtkWidget *w);
void move_down(GTKMusicBrowser *p, guint action, GtkWidget *w);
void clear_list(GTKMusicBrowser *p, guint action, GtkWidget *w);
void catalog_tog(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_random(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_location(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_time(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_genre(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_track(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_year(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_title(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_album(GTKMusicBrowser *p, guint action, GtkWidget *w);
void sort_artist(GTKMusicBrowser *p, guint action, GtkWidget *w);
}

struct {
    void *data;
    int type;
} TreeData;

static vector<PlaylistItem *> *getTreeSelection(GtkWidget *item)
{
    int type = (int)gtk_object_get_data(GTK_OBJECT(item), "type");
    vector<PlaylistItem *> *newlist = new vector<PlaylistItem *>;
    switch (type) {
        case 0: {
            MusicCatalog *cat = (MusicCatalog *)gtk_object_get_user_data(GTK_OBJECT(item));
            const vector<ArtistList *> *artistList = cat->GetMusicList();
            const vector<PlaylistItem *> *unsorted = cat->GetUnsortedMusic();
            vector<ArtistList *>::const_iterator h = artistList->begin();
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
            vector<PlaylistItem *>::const_iterator k = unsorted->begin();
            for (; k != unsorted->end(); k++) {
                PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*k);
                newlist->push_back(item);
            }
            break; }
        case 1: {
            ArtistList *list = (ArtistList *)gtk_object_get_user_data(GTK_OBJECT(item));
            vector<AlbumList *>::iterator i = list->m_albumList->begin();
            for (; i != list->m_albumList->end(); i++) {
                vector<PlaylistItem *>::iterator j = (*i)->m_trackList->begin();
                for (; j != (*i)->m_trackList->end(); j++) {
                    PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*j);
                    newlist->push_back(item);
                }
            }
            break; }
        case 2: {
            AlbumList *list = (AlbumList *)gtk_object_get_user_data(GTK_OBJECT(item));
            vector<PlaylistItem *>::iterator j = list->m_trackList->begin();
            for (; j != list->m_trackList->end(); j++) {
                PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*j);
                newlist->push_back(item);
            }
            break; }
        case 3: {
            PlaylistItem *i = new PlaylistItem(*(PlaylistItem *)gtk_object_get_user_data(GTK_OBJECT(item)));
            newlist->push_back(i);
            break; }
        case 4: {
            PlaylistItem *i = new PlaylistItem(*(PlaylistItem *)gtk_object_get_user_data(GTK_OBJECT(item)));
            newlist->push_back(i);
            break; }
        case 5: {
            char *fname = (char *)gtk_object_get_user_data(GTK_OBJECT(item));
//            localui->ReadPlaylist(fname, newlist);
            break; }
        case 8: {
            vector<PlaylistItem *> *unsorted = (vector<PlaylistItem *> *)
                                    gtk_object_get_user_data(GTK_OBJECT(item));
            vector<PlaylistItem *>::iterator k = unsorted->begin();
            for (; k != unsorted->end(); k++) {
                PlaylistItem *item = new PlaylistItem(*(PlaylistItem *)*k);
                newlist->push_back(item);
            }
            break; }
        case 9: {
            cout << "Danger, Wil Robinson, Danger!  To be implemented soon\n";
            break; }
        default:
            break;
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

            p->clickState = CONTEXT_PLAYLIST;

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
        if (new_info.insert_pos == GTK_CLIST_DRAG_NONE) {
            if (dest_info->cell.row < 0) {
                gdk_drag_status(context, GDK_ACTION_DEFAULT, time);
                return FALSE;
            }
        return TRUE;
        }
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
    GtkWidget *item;

    if (!event)
        return;

    g_return_if_fail(widget != NULL);
    g_return_if_fail(GTK_IS_TREE(widget));
    g_return_if_fail(event != NULL);

    item = gtk_get_event_widget((GdkEvent *)event);

    while (item && !GTK_IS_TREE_ITEM(item))
        item = item->parent;
 
    if (!item || (item->parent != widget))
        return;

    if (event->type == GDK_2BUTTON_PRESS) {
        vector<PlaylistItem *> *newlist = getTreeSelection(item);
        p->AddTracksPlaylistEvent(newlist);
    }
    else {
        int type = (int)gtk_object_get_data(GTK_OBJECT(item), "type");
        if (type == 5) {
            p->mbState = MB_PLAYLIST;
            p->mbSelName = (char *)gtk_object_get_user_data(GTK_OBJECT(item));
        }
        else if (type == 3 || type == 4) {
            p->mbState = MB_TRACK;
            p->mbSel = (PlaylistItem *)gtk_object_get_user_data(GTK_OBJECT(item));
            p->mbSelName = (char *)(p->mbSel->URL().c_str());
        }
        else
            p->mbState = MB_NONE;
    }

    p->clickState = CONTEXT_BROWSER;
}

static void tree_source_destroy(gpointer data)
{
}

static void tree_drag_begin(GtkWidget *w, GdkDragContext *context)
{
    GList *selected = GTK_TREE_SELECTION(GTK_TREE(w));

    if (selected == NULL)
        return;

    GtkWidget *item = GTK_WIDGET(selected->data);

    if (!item && !GTK_IS_TREE_ITEM(item))
        return;

    vector<PlaylistItem *> *newlist = getTreeSelection(item);

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
        
void GTKMusicBrowser::UpdateCatalog(void)
{
    m_musicCatalog = m_context->browser->m_catalog;
    GtkTargetEntry tree_target_table = {"tree-drag", 0, 1};

    const vector<ArtistList *> *artistList = m_musicCatalog->GetMusicList();
    const vector<PlaylistItem *> *unsorted = m_musicCatalog->GetUnsortedMusic();
    const vector<string> *playlists = m_musicCatalog->GetPlaylists();

    if ((artistList->size() == 0) && (unsorted->size() == 0) && 
        (playlists->size() == 0))
        return;

    if (musicBrowserTree) {
        gtk_widget_destroy(musicBrowserTree);
        musicBrowserTree = gtk_tree_new();
        gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(musicBrowserWindow),
                                              musicBrowserTree);
        gtk_widget_show(musicBrowserTree);
    }

    GtkWidget *item_subtree1, *item_subtree2, *item_subtree3;
    GtkWidget *item_new1, *item_new2, *item_new3;
    GtkWidget *all_subtree, *all_new;

    artistSubTree = gtk_tree_item_new_with_label("Music Catalog");
    gtk_tree_append(GTK_TREE(musicBrowserTree), artistSubTree);
    gtk_widget_show(artistSubTree);

    item_subtree1 = gtk_tree_new();
    gtk_signal_connect(GTK_OBJECT(item_subtree1), "button_press_event",
                       GTK_SIGNAL_FUNC(tree_clicked), this);
    gtk_drag_source_set(item_subtree1, GDK_BUTTON1_MASK, &tree_target_table, 1,
                        GDK_ACTION_MOVE);
    gtk_signal_connect(GTK_OBJECT(item_subtree1), "drag_data_get",
                       GTK_SIGNAL_FUNC(tree_drag_data_get), this);
    gtk_signal_connect(GTK_OBJECT(item_subtree1), "drag_begin",
                       GTK_SIGNAL_FUNC(tree_drag_begin), this);
    gtk_tree_item_set_subtree(GTK_TREE_ITEM(artistSubTree), item_subtree1);
    
    all_new = gtk_tree_item_new_with_label("All");
    gtk_object_set_user_data(GTK_OBJECT(all_new), m_musicCatalog);
    gtk_object_set_data(GTK_OBJECT(all_new), "type", (gpointer)0);
    gtk_tree_append(GTK_TREE(item_subtree1), all_new);
    all_subtree = gtk_tree_new();
    gtk_tree_item_set_subtree(GTK_TREE_ITEM(all_new), all_subtree);
    gtk_signal_connect(GTK_OBJECT(all_subtree), "button_press_event",
                       GTK_SIGNAL_FUNC(tree_clicked), this);
    gtk_drag_source_set(all_subtree, GDK_BUTTON1_MASK, &tree_target_table, 1,
                        GDK_ACTION_MOVE);
    gtk_signal_connect(GTK_OBJECT(all_subtree), "drag_data_get",
                       GTK_SIGNAL_FUNC(tree_drag_data_get), this);
    gtk_signal_connect(GTK_OBJECT(all_subtree), "drag_begin",
                       GTK_SIGNAL_FUNC(tree_drag_begin), this);

    item_new1 = gtk_tree_item_new_with_label("Uncategorized Songs");
    gtk_object_set_user_data(GTK_OBJECT(item_new1), (void *)unsorted);
    gtk_object_set_data(GTK_OBJECT(item_new1), "type", (gpointer)8);
    gtk_tree_append(GTK_TREE(item_subtree1), item_new1);
    item_subtree2 = gtk_tree_new();
    gtk_tree_item_set_subtree(GTK_TREE_ITEM(item_new1), item_subtree2);
    gtk_signal_connect(GTK_OBJECT(item_subtree2), "button_press_event",
                       GTK_SIGNAL_FUNC(tree_clicked), this);
    gtk_drag_source_set(item_subtree2, GDK_BUTTON1_MASK, &tree_target_table, 1,
                        GDK_ACTION_MOVE);
    gtk_signal_connect(GTK_OBJECT(item_subtree2), "drag_data_get",
                       GTK_SIGNAL_FUNC(tree_drag_data_get), this);
    gtk_signal_connect(GTK_OBJECT(item_subtree2), "drag_begin",
                       GTK_SIGNAL_FUNC(tree_drag_begin), this);
    vector<PlaylistItem *>::const_iterator l = unsorted->begin();
    for (; l != unsorted->end(); l++) {
        item_new2 = gtk_tree_item_new_with_label((char *)(*l)->URL().c_str());
        gtk_object_set_user_data(GTK_OBJECT(item_new2), *l);
        gtk_object_set_data(GTK_OBJECT(item_new2), "type", (gpointer)4);
        gtk_tree_append(GTK_TREE(item_subtree2), item_new2);
        gtk_widget_show(item_new2);
        item_new2 = gtk_tree_item_new_with_label((char *)(*l)->URL().c_str());
        gtk_object_set_user_data(GTK_OBJECT(item_new2), *l);
        gtk_object_set_data(GTK_OBJECT(item_new2), "type", (gpointer)4);
        gtk_tree_append(GTK_TREE(all_subtree), item_new2);
        gtk_widget_show(item_new2);
    }
    gtk_widget_show(item_new1);

    vector<ArtistList *>::const_iterator i = artistList->begin();
    for (; i != artistList->end(); i++) {
        item_new1 = gtk_tree_item_new_with_label((char *)(*i)->name.c_str());
        gtk_object_set_user_data(GTK_OBJECT(item_new1), *i);
        gtk_object_set_data(GTK_OBJECT(item_new1), "type", (gpointer)1);
        gtk_tree_append(GTK_TREE(item_subtree1), item_new1);

        vector<AlbumList *>::iterator j = (*i)->m_albumList->begin();
        item_subtree2 = gtk_tree_new();
        gtk_tree_item_set_subtree(GTK_TREE_ITEM(item_new1), item_subtree2);
        gtk_signal_connect(GTK_OBJECT(item_subtree2), "button_press_event",
                           GTK_SIGNAL_FUNC(tree_clicked), this);
        gtk_drag_source_set(item_subtree2, GDK_BUTTON1_MASK, &tree_target_table,
                            1, GDK_ACTION_MOVE);
        gtk_signal_connect(GTK_OBJECT(item_subtree2), "drag_data_get",
                           GTK_SIGNAL_FUNC(tree_drag_data_get), this);
        gtk_signal_connect(GTK_OBJECT(item_subtree2), "drag_begin",
                           GTK_SIGNAL_FUNC(tree_drag_begin), this);

        for (; j != (*i)->m_albumList->end(); j++) {
            item_new2 = gtk_tree_item_new_with_label((char *)(*j)->name.c_str());
            gtk_object_set_user_data(GTK_OBJECT(item_new2), *j);
            gtk_object_set_data(GTK_OBJECT(item_new2), "type", (gpointer)2);
            gtk_tree_append(GTK_TREE(item_subtree2), item_new2);

            vector<PlaylistItem *>::iterator k = (*j)->m_trackList->begin();
            item_subtree3 = gtk_tree_new();
            gtk_tree_item_set_subtree(GTK_TREE_ITEM(item_new2), item_subtree3);
            gtk_signal_connect(GTK_OBJECT(item_subtree3), "button_press_event",
                               GTK_SIGNAL_FUNC(tree_clicked), this);
            gtk_drag_source_set(item_subtree3, GDK_BUTTON1_MASK, 
                                &tree_target_table, 1, GDK_ACTION_MOVE);
            gtk_signal_connect(GTK_OBJECT(item_subtree3), "drag_data_get",
                               GTK_SIGNAL_FUNC(tree_drag_data_get), this);
            gtk_signal_connect(GTK_OBJECT(item_subtree3), "drag_begin",
                               GTK_SIGNAL_FUNC(tree_drag_begin), this);

            for (;k != (*j)->m_trackList->end(); k++) {
                item_new3 = gtk_tree_item_new_with_label((char *)(*k)->GetMetaData().Title().c_str());
                gtk_object_set_user_data(GTK_OBJECT(item_new3), *k);
                gtk_object_set_data(GTK_OBJECT(item_new3), "type", (gpointer)3);
                gtk_tree_append(GTK_TREE(item_subtree3), item_new3);
                gtk_widget_show(item_new3);
                item_new3 = gtk_tree_item_new_with_label((char *)(*k)->GetMetaData().Title().c_str());
                gtk_object_set_user_data(GTK_OBJECT(item_new3), *k);
                gtk_object_set_data(GTK_OBJECT(item_new3), "type", (gpointer)3);
                gtk_tree_append(GTK_TREE(all_subtree), item_new3);
                gtk_widget_show(item_new3);
            }
            gtk_widget_show(item_new2);
        }
        gtk_widget_show(item_new1);
    }

    gtk_widget_show(all_new);

    playlistSubTree = gtk_tree_item_new_with_label("Playlists");
    gtk_tree_append(GTK_TREE(musicBrowserTree), playlistSubTree);
    gtk_widget_show(playlistSubTree);
    
    item_subtree1 = gtk_tree_new();
    gtk_signal_connect(GTK_OBJECT(item_subtree1), "button_press_event",
                       GTK_SIGNAL_FUNC(tree_clicked), this);
    gtk_drag_source_set(item_subtree1, GDK_BUTTON1_MASK, &tree_target_table, 1,
                        GDK_ACTION_MOVE);
    gtk_signal_connect(GTK_OBJECT(item_subtree1), "drag_data_get",
                       GTK_SIGNAL_FUNC(tree_drag_data_get), this);
    gtk_signal_connect(GTK_OBJECT(item_subtree1), "drag_begin",
                       GTK_SIGNAL_FUNC(tree_drag_begin), this);

    gtk_tree_item_set_subtree(GTK_TREE_ITEM(playlistSubTree), item_subtree1);

    vector<string>::const_iterator m = playlists->begin();
    m++;
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
        item_new1 = gtk_tree_item_new_with_label(listname);
        delete [] fullname;
        gtk_object_set_user_data(GTK_OBJECT(item_new1), 
                                 (char *)(*m).c_str());
        gtk_object_set_data(GTK_OBJECT(item_new1), "type", (gpointer)5);
        gtk_tree_append(GTK_TREE(item_subtree1), item_new1);
        gtk_widget_show(item_new1);
    }
}

static void music_search(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->StartMusicSearch();
}

static void music_search_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    p->StartMusicSearch();
}

static void import_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    FileSelector *filesel = new FileSelector("Import a file to the Music Catalog");
    if (filesel->Run()) {
        FAContext *m_context = p->GetContext();
        char *returnpath = filesel->GetReturnPath();
        char *ext = m_context->player->GetExtension(returnpath);
        if (!strncmp("M3U", ext, 3))
            p->ImportPlaylist(filesel->GetReturnPath());

        else if (m_context->player->IsSupportedExtension(ext)) {
            uint32 length = strlen(returnpath) + 10;
            char *tempurl = new char[length];

            if (IsntError(FilePathToURL(returnpath, tempurl, &length))) {

                PlaylistItem *plist = new PlaylistItem(tempurl);
                m_context->plm->RetrieveMetaData(plist);

                while (plist->GetState() != kPlaylistItemState_Normal)
                    usleep(5);

                m_context->browser->WriteMetaDataToDatabase(tempurl, 
                                                          plist->GetMetaData());

                m_context->browser->m_catalog->AddSong(tempurl);

                p->UpdateCatalog();

                delete plist;
            }
            delete [] tempurl;
        }
        delete ext;
    }
    delete filesel;
}

static void remove_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    if (p->clickState == CONTEXT_PLAYLIST) {
        p->DeleteEvent();
    }
    else if (p->clickState == CONTEXT_BROWSER) {
        if (p->mbState == MB_PLAYLIST) {
            p->GetContext()->browser->m_catalog->RemovePlaylist(p->mbSelName);
        }
        else if (p->mbState == MB_TRACK) {
            p->GetContext()->browser->m_catalog->RemoveSong(p->mbSelName);
        }
        p->UpdateCatalog();
    }
}

static void edit_tool(GtkWidget *w, GTKMusicBrowser *p)
{
    if (p->clickState == CONTEXT_PLAYLIST) { 
        p->PopUpInfoEditor();
    }
    else if (p->clickState == CONTEXT_BROWSER) {
        if (p->mbState == MB_PLAYLIST) {
            p->CreateNewEditor(p->mbSelName);
        }
        else if (p->mbState == MB_TRACK) {
            p->PopUpInfoEditor(p->mbSel);
        }
    }
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
    gtk_box_pack_start(GTK_BOX(masterBox), masterBrowserBox, TRUE, TRUE, 0);
    gtk_widget_show(masterBrowserBox);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(masterBrowserBox), hbox, FALSE, FALSE, 5);
    gtk_widget_show(hbox);

    browserlabel = gtk_label_new("My Music Catalog:");
    gtk_box_pack_start(GTK_BOX(hbox), browserlabel, FALSE, FALSE, 5);
    gtk_widget_show(browserlabel);

    browservbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(masterBrowserBox), browservbox);
    gtk_container_set_border_width(GTK_CONTAINER(browservbox), 5);
    gtk_widget_show(browservbox);

    musicBrowserWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(musicBrowserWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(browservbox), musicBrowserWindow, TRUE, TRUE, 0);
    gtk_widget_set_usize(musicBrowserWindow, 200, 200);
    gtk_widget_show(musicBrowserWindow);

    musicBrowserTree = gtk_tree_new();
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(musicBrowserWindow),
                                          musicBrowserTree);
    gtk_widget_show(musicBrowserTree);
}

void GTKMusicBrowser::ExpandCollapseEvent(void)
{
    if (m_state == STATE_COLLAPSED) {
        CreateExpanded();
        m_state = STATE_EXPANDED;
        gtk_widget_show(masterBrowserBox);
        gtk_window_set_title(GTK_WINDOW(musicBrowser), BRANDING" - Music Browser");
        UpdateCatalog();
    }
    else {
        m_state = STATE_COLLAPSED;

        gtk_widget_hide(masterBrowserBox);
        gtk_window_set_title(GTK_WINDOW(musicBrowser), BRANDING" - Playlist Editor");
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
}

static gboolean toggle_vis_destroy(GtkWidget *w, GTKMusicBrowser *p)
{
    p->ToggleVisEventDestroyed();
    return FALSE;
}

static void delete_list_internal(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->DeleteListEvent();
}

static void delete_internal(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->DeleteEvent();
}

static void add_track_plist_internal(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
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

static void move_up_internal(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->MoveUpEvent();
}

static void move_down_internal(GtkWidget *widget, GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->MoveDownEvent();
}

static void playlist_row_move_internal(GtkWidget *widget, int source, int dest, 
                                       GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->MoveItemEvent(source, dest);
}

static void set_current_index_internal(GtkWidget *widget, int row, int column, 
                                       GdkEventButton *button, 
                                       GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->m_currentindex = row;
    if (button && button->type == GDK_2BUTTON_PRESS)
        p->PlayEvent();
}

static void quit_menu(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->ToggleVisEvent();
}

static void sort_playlist_internal(int column, GTKMusicBrowser *p, 
                                   PlaylistSortType type = 
                                                    PlaylistSortType_Ascending)
{
    PlaylistSortKey key;

    p->clickState = CONTEXT_PLAYLIST;

    /* Not a one-to-one, but I suppose I could write it to be...  */
    switch (column) {
        case 0: key = kPlaylistSortKey_Title; break;
        case 1: key = kPlaylistSortKey_Artist; break;
        case 2: key = kPlaylistSortKey_Time; break;
        case 3: key = kPlaylistSortKey_Album; break;
        case 4: key = kPlaylistSortKey_Year; break;
        case 5: key = kPlaylistSortKey_Track; break;
        case 6: key = kPlaylistSortKey_Genre; break;
        case 7: key = kPlaylistSortKey_Location; break;
        default: key = kPlaylistSortKey_Random; break;
    }

    p->SortPlaylistEvent(key, type);
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
    if (p->clickState == CONTEXT_BROWSER && p->mbState == MB_PLAYLIST)
        p->CreateNewEditor(p->mbSelName);
    else 
        p->PopUpInfoEditor();
}

static void new_plist(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SaveCurrentPlaylist();

    FileSelector *filesel = new FileSelector("Choose Name for the New Playlist");
    if (filesel->Run())
        p->CreateNewEditor(filesel->GetReturnPath());
    delete filesel;
}

static void open_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SaveCurrentPlaylist();

    FileSelector *filesel = new FileSelector("Open a Playlist from Disk");
    if (filesel->Run())
        p->CreateNewEditor(filesel->GetReturnPath());
    delete filesel;
}

static void save_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->SaveCurrentPlaylist();
}

static void saveas_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    FileSelector *filesel = new FileSelector("Save This Playlist to Disk");
    if (filesel->Run())
        p->SaveCurrentPlaylist(filesel->GetReturnPath());
    delete filesel;
}

static void save_list_internal(GtkWidget *w, GTKMusicBrowser *p)
{
    p->clickState = CONTEXT_PLAYLIST;
    p->SaveCurrentPlaylist();
}

static void import_list(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    import_tool(w, p);
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
    if (p->clickState == CONTEXT_PLAYLIST) {
        p->DeleteEvent();
    }
    else if (p->clickState == CONTEXT_BROWSER) {
        if (p->mbState == MB_PLAYLIST) {
            p->GetContext()->browser->m_catalog->RemovePlaylist(p->mbSelName);
        }
        else if (p->mbState == MB_TRACK) {
            p->GetContext()->browser->m_catalog->RemoveSong(p->mbSelName);
        }
        p->UpdateCatalog();
    }
}

static void realdelete_sel(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    string urlToDelete;
    if (p->clickState == CONTEXT_PLAYLIST) {
        urlToDelete = p->GetContext()->plm->ItemAt(p->m_currentindex)->URL();
        p->DeleteEvent();
    }
    else if (p->clickState == CONTEXT_BROWSER) {
        if (p->mbState == MB_PLAYLIST) {
            p->GetContext()->browser->m_catalog->RemovePlaylist(p->mbSelName);
        }
        else if (p->mbState == MB_TRACK) {
            p->GetContext()->browser->m_catalog->RemoveSong(p->mbSelName);
        }
        urlToDelete = p->mbSelName;
        p->UpdateCatalog();
    }
    uint32 length = urlToDelete.length();
    char *filename = new char[length];
 
//    if (IsntError(URLToFilePath(urlToDelete.c_str(), filename, &length)))
//        unlink(filename);
 
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
    GtkItemFactory *item_factory;
    GtkAccelGroup *accel_group;
    GtkWidget *separator;

    accel_group = gtk_accel_group_new();
    item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<blah>", 
                                        accel_group); 
    CreateMenuItems(item_factory, (void *)this);
    
    gtk_accel_group_attach(accel_group, GTK_OBJECT(musicBrowser));
    gtk_box_pack_start(GTK_BOX(topbox), gtk_item_factory_get_widget(
                       item_factory, "<blah>"), FALSE, TRUE, 0);
    gtk_widget_show(gtk_item_factory_get_widget(item_factory, "<blah>"));
    
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
        char *iText[3];
        char *title;
        char *artist;
        char length[50];

        if (mdata.Title() == "" || mdata.Title() == " ") {
            char *urlname = new char[_MAX_PATH];
            char *tempdir = strrchr(item->URL().c_str(), '/');
            if (tempdir)
                strcpy(urlname, tempdir + 1);
            else 
                strcpy(urlname, item->URL().c_str());
            tempdir = strrchr(urlname, '.');
            if (tempdir)
                *tempdir = '\0';
            
            uint32 filelength = strlen(urlname) + 1;
            char *filename = new char[filelength];

            if (IsntError(URLToFilePath(urlname, filename, &filelength)))
                mdata.SetTitle(filename);
            else
                mdata.SetTitle(urlname);
        }
        title = (char *)mdata.Title().c_str();
        artist = (char *)mdata.Artist().c_str();

        if (mdata.Time() == 0)
            sprintf(length, "Unknown");
        else
            sprintf(length, "%d", mdata.Time());

        iText[0] = title;
        iText[1] = artist;
        iText[2] = length;

        gtk_clist_append(GTK_CLIST(playlistList), iText);
    }

    gtk_clist_columns_autosize(GTK_CLIST(playlistList));
    gtk_clist_select_row(GTK_CLIST(playlistList), m_currentindex, 0);
    gtk_clist_moveto(GTK_CLIST(playlistList), m_currentindex, 0, 0.5, -1);
    gtk_clist_thaw(GTK_CLIST(playlistList));
}

static void playlist_column_click_internal(GtkCList *clist, gint column, 
                                           GTKMusicBrowser *p)
{
    if (column == p->m_playlistLastSort) {
        if (p->m_playlistColumnSort == PlaylistSortType_Ascending)
            p->m_playlistColumnSort = PlaylistSortType_Descending;
        else
            p->m_playlistColumnSort = PlaylistSortType_Ascending;
    }
    else
        p->m_playlistColumnSort = PlaylistSortType_Ascending;
    p->m_playlistLastSort = column;

    sort_playlist_internal(column, p, p->m_playlistColumnSort);
}
    
void GTKMusicBrowser::CreatePlaylistList(GtkWidget *box)
{
    static char *titles[] =
    {
      "Title", "Artist", "Length"
    };

    playlistList = gtk_clist_new_with_titles(3, titles);
    gtk_container_add(GTK_CONTAINER(box), playlistList);
    gtk_signal_connect(GTK_OBJECT(playlistList), "row_move",
                       GTK_SIGNAL_FUNC(playlist_row_move_internal), this);   
    gtk_signal_connect(GTK_OBJECT(playlistList), "select_row",
                       GTK_SIGNAL_FUNC(set_current_index_internal), this);
    if (master)
        gtk_signal_connect(GTK_OBJECT(playlistList), "click_column", 
                           GTK_SIGNAL_FUNC(playlist_column_click_internal), this);
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
            m_currentListName = oPlaylist;
            UpdatePlaylistList();
        }
        delete [] PlaylistURL;
    }
}

static GtkWidget *new_pixmap(GtkWidget *w)
{
    GtkWidget *wpixmap;
    GdkPixmap *pixmap;
    GdkBitmap *mask;

    pixmap = gdk_pixmap_create_from_xpm_d(w->window, &mask, 
                                          &w->style->bg[GTK_STATE_NORMAL], 
                                          openfile);
    wpixmap = gtk_pixmap_new(pixmap, mask);
    return wpixmap;
}

void GTKMusicBrowser::CreatePlaylist(void)
{
    GtkWidget *vbox;
    GtkWidget *buttonvbox;
    GtkWidget *playlistwindow;
    GtkWidget *arrow;
    GtkWidget *button;

    musicBrowser = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    string titlestr = string(BRANDING) + string(" - Music Browser: ");
    if (master)
        titlestr += string("Current listening list");
    else
        titlestr += string("Editing playlist ") + m_currentListName;
    gtk_window_set_title(GTK_WINDOW(musicBrowser), titlestr.c_str());
    gtk_window_set_policy(GTK_WINDOW(musicBrowser), TRUE, TRUE, TRUE);
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

    GtkWidget *toolbar = gtk_toolbar_new(GTK_ORIENTATION_HORIZONTAL, 
                                         GTK_TOOLBAR_BOTH);
    gtk_toolbar_set_button_relief(GTK_TOOLBAR(toolbar), GTK_RELIEF_NONE);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Music Search", 
                            "Search for Music Files on your System", 
                            "Toolbar/MusicSearch", new_pixmap(musicBrowser),
                            GTK_SIGNAL_FUNC(music_search_tool), this);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Import",
                            "Import a Playlist/Track into the Music Catalog",
                            "Toolbar/Import", new_pixmap(musicBrowser),
                            GTK_SIGNAL_FUNC(import_tool), this);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Remove",
                            "Remove a Track or Playlist",
                            "Toolbar/Remove", new_pixmap(musicBrowser),
                            GTK_SIGNAL_FUNC(remove_tool), this);

    gtk_toolbar_append_item(GTK_TOOLBAR(toolbar), "Edit",
                            "Edit a Track or Playlist",
                            "Toolbar/Edit", new_pixmap(musicBrowser),
                            GTK_SIGNAL_FUNC(edit_tool), this);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, TRUE, 0);
    gtk_widget_show(toolbar);

    GtkWidget *separator = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, TRUE, 0);
    gtk_widget_show(separator);

    masterBox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), masterBox, TRUE, TRUE, 0);
    gtk_widget_show(masterBox);

    masterPlaylistBox = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_end(GTK_BOX(masterBox), masterPlaylistBox, TRUE, TRUE, 0);
    gtk_widget_show(masterPlaylistBox);

    GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(masterPlaylistBox), hbox, FALSE, FALSE, 5);
    gtk_widget_show(hbox);

    GtkWidget *playlistLabel;

    if (master)
        playlistLabel = gtk_label_new("Currently listening to:");
    else {
        string labelstr = string("Editing playlist: ") + m_currentListName;
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

    buttonvbox = gtk_vbox_new(FALSE, 10);
    gtk_container_set_border_width(GTK_CONTAINER(buttonvbox), 0);
    gtk_box_pack_start(GTK_BOX(hbox), buttonvbox, FALSE, FALSE, 0);
    gtk_widget_show(buttonvbox);

    button = gtk_button_new();
    arrow = gtk_arrow_new(GTK_ARROW_UP, GTK_SHADOW_IN);
    gtk_widget_show(arrow);
    gtk_container_add(GTK_CONTAINER(button), arrow);
    gtk_box_pack_start(GTK_BOX(buttonvbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(move_up_internal), this);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Delete");
    gtk_box_pack_start(GTK_BOX(buttonvbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(delete_internal), this);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Add Track");
    gtk_box_pack_start(GTK_BOX(buttonvbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(add_track_plist_internal), this);
    gtk_widget_show(button);

    button = gtk_button_new();
    arrow = gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_IN);
    gtk_widget_show(arrow);
    gtk_container_add(GTK_CONTAINER(button), arrow);
    gtk_box_pack_start(GTK_BOX(buttonvbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(move_down_internal), this);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Save List");
    gtk_box_pack_end(GTK_BOX(buttonvbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked",
                       GTK_SIGNAL_FUNC(save_list_internal), this);
    gtk_widget_show(button);

    button = gtk_button_new_with_label("Clear List");
    gtk_box_pack_end(GTK_BOX(buttonvbox), button, FALSE, FALSE, 0);
    gtk_signal_connect(GTK_OBJECT(button), "clicked", 
                       GTK_SIGNAL_FUNC(delete_list_internal), this);
    gtk_widget_show(button);

    gtk_widget_show(musicBrowser);

    m_state = STATE_COLLAPSED;
}

void GTKMusicBrowser::DeleteListEvent(void)
{
    m_plm->RemoveAll();
    UpdatePlaylistList();
    m_currentindex = kInvalidIndex;
}

void GTKMusicBrowser::DeleteEvent(void)
{
    m_plm->RemoveItem(m_currentindex);
    m_currentindex = m_plm->GetCurrentIndex();
    UpdatePlaylistList();
}

void GTKMusicBrowser::MoveUpEvent(void)
{
    if (m_currentindex == 0)
        return;
    m_plm->SwapItems(m_currentindex, m_currentindex - 1);
    m_currentindex--;
    UpdatePlaylistList();
}

void GTKMusicBrowser::MoveDownEvent(void)
{
    if (m_currentindex == m_plm->CountItems() - 1)
        return;
    m_plm->SwapItems(m_currentindex, m_currentindex + 1);
    m_currentindex++;
    UpdatePlaylistList();
}

void GTKMusicBrowser::MoveItemEvent(int source, int dest)
{
    m_plm->MoveItem(source, dest);
    UpdatePlaylistList();
}

void GTKMusicBrowser::AddTrackPlaylistEvent(char *path)
{
    char *tempurl = new char[_MAX_PATH];
    uint32 length = _MAX_PATH;
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    if (IsntError(FilePathToURL(path, tempurl, &length))) {
        m_plm->AddItem(tempurl, m_currentindex);
        UpdatePlaylistList();
    }
    delete [] tempurl;
}

void GTKMusicBrowser::AddTrackPlaylistEvent(PlaylistItem *newitem)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    m_plm->AddItem(newitem, m_currentindex, false);
    UpdatePlaylistList();
}

void GTKMusicBrowser::AddTracksPlaylistEvent(vector<PlaylistItem *> *newlist)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    m_plm->AddItems(newlist, m_currentindex, false);
    UpdatePlaylistList();
}

void GTKMusicBrowser::PlayEvent(void)
{
    m_plm->SetCurrentIndex(m_currentindex);
    m_context->target->AcceptEvent(new Event(CMD_Play));
}

void GTKMusicBrowser::StartMusicSearch(void)
{
    parentUI->StartSearch();
}

void GTKMusicBrowser::SortPlaylistEvent(PlaylistSortKey order, PlaylistSortType
                                        type)
{
    if (order == kPlaylistSortKey_Random)
        m_plm->SetShuffleMode(true);
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
    if (path)
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
    }
    else {
        uint32 urlLength = m_currentListName.length() + 20;
        char *writeURL = new char[urlLength];
        Error err = FilePathToURL(m_currentListName.c_str(), writeURL, &urlLength);
        if (IsntError(err))
            m_plm->WritePlaylist(writeURL, &format);
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
        m_context->browser->m_catalog->AddPlaylist(path);
    delete [] url;
    UpdateCatalog();
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
    m_state = STATE_COLLAPSED;
    statusContext = 0;
    playlistList = NULL;
    m_musicCatalog = NULL;
    m_browserCreated = false;

    parentUI = masterUI;
 
    clickState = CONTEXT_PLAYLIST;

    if (playlistURL.length() == 0) {
        playlistURL = string("file://") + FreeampDir(NULL) + 
                      string("/currentlist.m3u");

        m_plm = context->plm;
        master = true;
    }
    else {
        m_plm = new PlaylistManager(context);
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

    if (m_state == STATE_EXPANDED)
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
    if (m_state == STATE_COLLAPSED)
        ExpandCollapseEvent();
    gdk_threads_leave();
}

void GTKMusicBrowser::Close(void)
{
    gdk_threads_enter();
    isVisible = false;
    if (m_initialized) {
        gtk_widget_hide(musicBrowser);
        if (!master)
            parentUI->WindowClose(this); 
    }
    gdk_threads_leave();

    SaveCurrentPlaylist(NULL);
}

int32 GTKMusicBrowser::AcceptEvent(Event *e)
{
    switch (e->Type()) {
        case INFO_SearchMusicDone: {
            if (m_initialized) {
                gdk_threads_enter();
                UpdateCatalog();
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
        default:
            break;
    }
    return 0;
}

void GTKMusicBrowser::CreateNewEditor(char *playlisturl)
{
    string newURL;
    if (!strncmp("file://", playlisturl, 7))
        newURL = playlisturl;
    else
        newURL = string("file://") + string(playlisturl);
    parentUI->CreateNewEditor(newURL);
}

