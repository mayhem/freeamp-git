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

#include "gtkmusicbrowser.h"
#include "gtkmessagedialog.h"
#include "eventdata.h"

#include "../res/album_pix.xpm"
#include "../res/all_pix.xpm"
#include "../res/artist_pix.xpm"
#include "../res/catalog_pix.xpm"
#include "../res/playlist_pix.xpm"
#include "../res/track_pix.xpm"
#include "../res/uncatagorized_pix.xpm"
#include "../res/cd_pix.xpm"
#include "../res/icecast_pix.xpm"
#include "../res/shoutcast_pix.xpm"
#include "../res/streams_pix.xpm"
#include "../res/wiredplanet_pix.xpm"

FAContext *BADContext = NULL;

void kill_treedata(TreeData *dead)
{
    delete dead;
}

TreeData *GTKMusicBrowser::NewTreeData(TreeNodeType type, MusicCatalog *cat,
                                       ArtistList *art, AlbumList *alb,
                                       PlaylistItem *tr, char *pname,
                                       char *message,
                                       vector<PlaylistItem *> *cdlist)
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
    data->cdtracks = cdlist;
    return data;
}

vector<PlaylistItem *> *GTKMusicBrowser::GetTreeSelection(void)
{
    GtkCTree *tree = musicBrowserTree;

    vector<PlaylistItem *> *newlist = new vector<PlaylistItem *>;

    GtkCList *clist = GTK_CLIST(tree);
    GList *selection = clist->selection;

    while (selection) {
      GtkCTreeNode *node = (GtkCTreeNode *)selection->data;
      TreeData *data = (TreeData *)gtk_ctree_node_get_row_data(tree, node);

      if (!data)
          return newlist;

      switch (data->type) {
        case kTreeMyMusic:
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
        case kTreeCD:
        case kTreeStream:
        case kTreeFavStream:
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
        case kTreeCDHead: {
            vector<PlaylistItem *> *cd = data->cdtracks;
            vector<PlaylistItem *>::iterator k = cd->begin();
            for (; k != cd->end(); k++) {
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

void GTKMusicBrowser::AddCatTrack(ArtistList *artist, AlbumList *album,
                                  PlaylistItem *item, bool expand)
{
    char      *name[1];
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);
    TreeData  *data;

    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

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
        gtk_ctree_node_set_row_data_full(musicBrowserTree, treeItem, data,
                                         (GtkDestroyNotify)kill_treedata);
        if (expand) {
            gtk_ctree_expand(musicBrowserTree, uncatTree);
            gtk_ctree_select(musicBrowserTree, treeItem);
            gtk_ctree_node_moveto(musicBrowserTree, treeItem, 0, 0.5, 0);
        }

        treeItem = gtk_ctree_insert_node(musicBrowserTree, allTree,
                                         NULL, name, 5, pixmap, mask, pixmap,
                                         mask, true, false);
        data = NewTreeData(kTreeTrack, NULL, NULL, NULL, item);
        gtk_ctree_node_set_row_data_full(musicBrowserTree, treeItem, data, 
                                         (GtkDestroyNotify)kill_treedata);
         
        gtk_ctree_sort_recursive(musicBrowserTree, allTree);
        gtk_ctree_sort_recursive(musicBrowserTree, uncatTree);
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

            GtkCTreeNode *sib = GTK_CTREE_ROW(mainTree)->children;
            /* skip uncat and all subtrees */
            sib = GTK_CTREE_ROW(sib)->sibling;
            sib = GTK_CTREE_ROW(sib)->sibling;

            while (sib) {
                GtkCListRow *row = (GtkCListRow *)GTK_CTREE_ROW(sib);
                char *sibtext = GTK_CELL_PIXTEXT(row->cell[0])->text;
                if (strcasecmp(name[0], sibtext) < 0)
                    break;
                sib = GTK_CTREE_ROW(sib)->sibling;
            }

            artTree = gtk_ctree_insert_node(musicBrowserTree, mainTree,
                                            sib, name, 5, pixmap, mask,
                                            pixmap, mask, false, false);
            data = NewTreeData(kTreeArtist, NULL, artist);
            gtk_ctree_node_set_row_data_full(musicBrowserTree, artTree, data,
                                             (GtkDestroyNotify)kill_treedata);
            if (expand)
                gtk_ctree_expand(musicBrowserTree, artTree);
            gtk_ctree_sort_node(musicBrowserTree, artTree);
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
        gtk_ctree_node_set_row_data_full(musicBrowserTree, albTree, data,
                                         (GtkDestroyNotify)kill_treedata);
        if (expand)
            gtk_ctree_expand(musicBrowserTree, albTree);
        gtk_ctree_sort_node(musicBrowserTree, albTree);
    }

    name[0] = (char *)item->GetMetaData().Title().c_str();
    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window,
                                          &mask, &style->bg[GTK_STATE_NORMAL],
                                          track_pix);
    newItem = gtk_ctree_insert_node(musicBrowserTree, albTree, NULL, name, 5,
                                    pixmap, mask, pixmap, mask, true, false);
    data = NewTreeData(kTreeTrack, NULL, artist, album, item);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, newItem, data,
                                     (GtkDestroyNotify)kill_treedata);
    if (expand) {
        gtk_ctree_expand_recursive(musicBrowserTree, newItem);
        gtk_ctree_select(musicBrowserTree, newItem);
        gtk_ctree_node_moveto(musicBrowserTree, newItem, 0, 0.5, 0);
    }

    newItem = gtk_ctree_insert_node(musicBrowserTree, allTree, NULL, name, 5,
                                    pixmap, mask, pixmap, mask, true, false);
    data = NewTreeData(kTreeTrack, NULL, artist, album, item);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, newItem, data, 
                                     (GtkDestroyNotify)kill_treedata);

    gtk_ctree_sort_recursive(musicBrowserTree, allTree);
    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::RemoveCatTrack(ArtistList *artist, AlbumList *album,
                                     PlaylistItem *item)
{
    GtkCTreeNode *toRemove;

    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

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

GtkCTreeNode *GTKMusicBrowser::FindStreamNode(PlaylistItem *stream)
{
    GtkCTreeNode *retnode;
    TreeData *data = NewTreeData(kTreeFavStream, NULL, NULL, NULL, stream);

    retnode = gtk_ctree_find_by_row_data_custom(musicBrowserTree, favoritesTree,
                                                data,
                                                (GCompareFunc)TreeDataCompare);

    delete data;
    return retnode;
}

void GTKMusicBrowser::AddCatStream(PlaylistItem *item)
{
    char         *name[1];
    GdkPixmap    *pixmap;
    GdkBitmap    *mask;
    GtkStyle     *style = gtk_widget_get_style(musicBrowserWindow);
    TreeData     *data;
    GtkCTreeNode *node;

    name[0] = (char *)(item->GetMetaData().Title().c_str());

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          streams_pix);
    node = gtk_ctree_insert_node(musicBrowserTree, favoritesTree, NULL,
                                 name, 5, pixmap, mask, pixmap, mask,
                                 true, false);
    data = NewTreeData(kTreeFavStream, NULL, NULL, NULL, item);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, node, data,
                                     (GtkDestroyNotify)kill_treedata);
}

void GTKMusicBrowser::RemoveCatStream(PlaylistItem *item)
{
    GtkCTreeNode *toRemove;

    toRemove = FindStreamNode(item);

    if (toRemove)
        gtk_ctree_remove_node(musicBrowserTree, toRemove);
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

void GTKMusicBrowser::AddCatPlaylist(string playlist)
{
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
    gtk_ctree_node_set_row_data_full(musicBrowserTree, allItem, data,
                                     (GtkDestroyNotify)kill_treedata);

    delete [] fullname;
}

void GTKMusicBrowser::RemoveCatPlaylist(string playlist)
{
    GtkCTreeNode *toRemove;

    toRemove = FindPlaylistNode(playlist);

    if (toRemove)
        gtk_ctree_remove_node(musicBrowserTree, toRemove);
}

void GTKMusicBrowser::UpdateCDTree(PlaylistItem *update)
{
    if (!CDTree)
        return;

    GtkCTreeNode *find = FindNode(kTreeCD, NULL, NULL, update, CDTree);

    if (!find) {
        return;
    }
    GdkPixmap *pixmap;
    GdkBitmap *mask;

    MetaData mdata = (MetaData)update->GetMetaData();
    if (mdata.Title().size() > 0) {
        gtk_ctree_node_get_pixtext(musicBrowserTree, find, 0, NULL, NULL,
                                   &pixmap, &mask);
        gtk_ctree_node_set_pixtext(musicBrowserTree, find, 0,
                                   mdata.Title().c_str(), 5, pixmap, mask);
    }

    char *tempstr = new char[mdata.Album().size() + mdata.Artist().size() + 50];
    if (mdata.Album().size() > 0)
        sprintf(tempstr, "%s ", mdata.Album().c_str());
    else
        sprintf(tempstr, "Unknown Album ");
    strcat(tempstr, "(");
    if (mdata.Artist().size() > 0)
        strcat(tempstr, mdata.Artist().c_str());
    else
        strcat(tempstr, "Unknown Artist");
    strcat(tempstr, ")");

    gtk_ctree_node_get_pixtext(musicBrowserTree, CDTree, 0, NULL, NULL,
                               &pixmap, &mask);
    gtk_ctree_node_set_pixtext(musicBrowserTree, CDTree, 0, tempstr, 5,
                               pixmap, mask);

    delete [] tempstr;
}

void GTKMusicBrowser::RegenerateCDTree(void)
{
    GtkCTreeRow *row = GTK_CTREE_ROW(CDTree);

    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));

    while (row->children) {
        GtkCTreeNode *todelete = row->children;
        gtk_ctree_remove_node(musicBrowserTree, todelete);
    }

    while (CDTracks->size() > 0)
        CDTracks->erase(CDTracks->begin());

    char url[40];
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);
    char *name[1];
    GtkCTreeNode *cdItem;
    TreeData *data;
    PlaylistItem *newitem;

    for (uint32 tracknum = 1; tracknum <= CD_numtracks; tracknum++) {
        sprintf(url, "file://%d.cda", tracknum);
        newitem = new PlaylistItem(url);

        name[0] = (char *)newitem->URL().c_str();
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          track_pix);
        cdItem = gtk_ctree_insert_node(musicBrowserTree, CDTree, NULL, name,
                                       5, pixmap, mask, pixmap, mask, true,
                                       false);
        data = NewTreeData(kTreeCD, NULL, NULL, NULL, newitem);
        gtk_ctree_node_set_row_data_full(musicBrowserTree, cdItem, data,
                                         (GtkDestroyNotify)kill_treedata);

        CDTracks->push_back(newitem);
    }
    if (CDTracks->size() > 0) {
        vector<PlaylistItem *> *metalist =
                                   new vector<PlaylistItem *>(CDTracks->size());

        copy(CDTracks->begin(), CDTracks->end(), metalist->begin());
        m_plm->RetrieveMetaData(metalist);
    }

    gtk_ctree_node_get_pixtext(musicBrowserTree, CDTree, 0, NULL, NULL, &pixmap,
                               &mask);
    gtk_ctree_node_set_pixtext(musicBrowserTree, CDTree, 0, "CD Audio", 5,
                               pixmap, mask);

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));

    gtk_widget_set_sensitive(gtk_item_factory_get_widget(menuFactory,
                             "/Controls/Eject CD"), (CD_numtracks > 0));
}

void GTKMusicBrowser::ClearTree(void)
{
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));
    gtk_clist_clear(GTK_CLIST(musicBrowserTree));

    CreateMainTreeItems();

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

bool GTKMusicBrowser::CheckEmptyDatabase(void)
{
    if (m_context->catalog->GetPlaylists()->size() > 0 ||
        m_context->catalog->GetMusicList()->size() > 0 ||
        m_context->catalog->GetUnsortedMusic()->size() > 0)
        return false;

    bool welcome = false;

    m_context->prefs->GetPrefBoolean(kWelcomePref, &welcome);

    if (welcome) {
        m_context->prefs->SetPrefBoolean(kWelcomePref, false);
        StartMusicSearch(false, true);
    }
    else {
        GTKMessageDialog oBox;
        string oMessage = string("Your music database does not contain any items.  Would you like to start a music search to find music and playlists on your machine?");

        if (oBox.Show(oMessage.c_str(), "MusicBrowser", kMessageYesNo)
            == kMessageReturnYes)
           StartMusicSearch(false);
    }
    return true;
}

void GTKMusicBrowser::UpdateCatalog(void)

{
    m_musicCatalog = m_context->catalog;
    m_musicCatalog->GetCatalogLock();

    vector<ArtistList *> *artistList =
                         (vector<ArtistList *> *)m_musicCatalog->GetMusicList();
    vector<PlaylistItem *> *unsorted =
                   (vector<PlaylistItem *> *)m_musicCatalog->GetUnsortedMusic();
    vector<string> *playlists =
                               (vector<string> *)m_musicCatalog->GetPlaylists();
    vector<PlaylistItem *> *streams =
                         (vector<PlaylistItem *> *)m_musicCatalog->GetStreams();

    if (CheckEmptyDatabase()) {
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
        gtk_ctree_node_set_row_data_full(musicBrowserTree, uncatItem, data,
                                         (GtkDestroyNotify)kill_treedata);

        allItem = gtk_ctree_insert_node(musicBrowserTree, allTree,
                                        NULL, name, 5, pixmap, mask, pixmap,
                                        mask, true, false);
        data = NewTreeData(kTreeTrack, NULL, NULL, NULL, *l);
        gtk_ctree_node_set_row_data_full(musicBrowserTree, allItem, data,
                                         (GtkDestroyNotify)kill_treedata);
    }

    gtk_ctree_sort_recursive(musicBrowserTree, uncatTree);

    vector<ArtistList *>::iterator i = artistList->begin();
    for (; i != artistList->end(); i++) {
        GtkCTreeNode *artTree, *artItem = NULL;

        name[0] = (char *)(*i)->name.c_str();
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          artist_pix);

        GtkCTreeNode *sib = GTK_CTREE_ROW(mainTree)->children;
        /* skip uncat and all subtrees */
        sib = GTK_CTREE_ROW(sib)->sibling;
        sib = GTK_CTREE_ROW(sib)->sibling;

        while (sib) {
            GtkCListRow *row = (GtkCListRow *)GTK_CTREE_ROW(sib);
            char *sibtext = GTK_CELL_PIXTEXT(row->cell[0])->text;
            if (strcasecmp(name[0], sibtext) < 0)
                break;
            sib = GTK_CTREE_ROW(sib)->sibling;
        }

        artTree = gtk_ctree_insert_node(musicBrowserTree, mainTree,
                                        sib, name, 5, pixmap, mask,
                                        pixmap, mask, false, false);
        data = NewTreeData(kTreeArtist, NULL, (*i));
        gtk_ctree_node_set_row_data_full(musicBrowserTree, artTree, data,
                                         (GtkDestroyNotify)kill_treedata);

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
            gtk_ctree_node_set_row_data_full(musicBrowserTree, artItem, data,
                                             (GtkDestroyNotify)kill_treedata);

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
                gtk_ctree_node_set_row_data_full(musicBrowserTree, trackItem, 
                                                 data,
                                                 (GtkDestroyNotify)kill_treedata);


                allItem = gtk_ctree_insert_node(musicBrowserTree, allTree,
                                                NULL, name,
                                                5, pixmap, mask, pixmap, mask,
                                                true, false);
                data = NewTreeData(kTreeTrack, NULL, (*i), (*j), (*k));
                gtk_ctree_node_set_row_data_full(musicBrowserTree, allItem, 
                                                 data,
                                                 (GtkDestroyNotify)kill_treedata);
            }
        }
        gtk_ctree_sort_recursive(musicBrowserTree, artTree);
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
        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          playlist_pix);
        allItem = gtk_ctree_insert_node(musicBrowserTree, playlistTree, NULL,
                                        name, 5, pixmap, mask, pixmap, mask,
                                        true, false);
        data = NewTreeData(kTreePlaylist, NULL, NULL, NULL, NULL,
                           (char *)(*m).c_str(), (char *)(*m).c_str());
        gtk_ctree_node_set_row_data_full(musicBrowserTree, allItem, data,
                                         (GtkDestroyNotify)kill_treedata);

        delete [] fullname;
    }

    gtk_ctree_sort_recursive(musicBrowserTree, playlistTree);

    if (m_bCDMode) {
        gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
        m_musicCatalog->ReleaseCatalogLock();
        return;
    }

    l = streams->begin();
    for (; l != streams->end(); l++) {

        name[0] = (char *)((*l)->GetMetaData().Title().c_str());

        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          streams_pix);
        allItem = gtk_ctree_insert_node(musicBrowserTree, favoritesTree, NULL,
                                        name, 5, pixmap, mask, pixmap, mask,
                                        true, false);
        data = NewTreeData(kTreeFavStream, NULL, NULL, NULL, (*l));
        gtk_ctree_node_set_row_data_full(musicBrowserTree, allItem, data, 
                                         (GtkDestroyNotify)kill_treedata);    
    }
    gtk_ctree_sort_recursive(musicBrowserTree, favoritesTree);

    RegenerateCDTree();

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));

    m_musicCatalog->ReleaseCatalogLock();
}

void GTKMusicBrowser::CreateMainTreeItems(void)
{
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
    gtk_ctree_node_set_row_data_full(musicBrowserTree, mainTree, data, 
                                     (GtkDestroyNotify)kill_treedata);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          all_pix);
    name[0] = "All Tracks";
    allTree = gtk_ctree_insert_node(musicBrowserTree, mainTree, NULL, name, 5,
                                    pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeAll, m_musicCatalog, NULL, NULL, NULL, NULL,
                       "This tree item lists all of your music tracks");
    gtk_ctree_node_set_row_data_full(musicBrowserTree, allTree, data,
                                     (GtkDestroyNotify)kill_treedata);

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
    gtk_ctree_node_set_row_data_full(musicBrowserTree, uncatTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          playlist_pix);
    name[0] = "My Playlists";
    playlistTree = gtk_ctree_insert_node(musicBrowserTree, NULL, NULL, name, 5,
                                         pixmap, mask, pixmap, mask, false,
                                         false);
    data = NewTreeData(kTreePlaylistHead, NULL, NULL, NULL, NULL, NULL,
                       "This tree item contains all of your playlists");
    gtk_ctree_node_set_row_data_full(musicBrowserTree, playlistTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          streams_pix);

    if (m_bCDMode)
        return;

    name[0] = "My Streams";
    streamTree = gtk_ctree_insert_node(musicBrowserTree, NULL, NULL, name, 5,
                                       pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeStreamsHead, NULL, NULL, NULL, NULL, NULL,
                 "This tree item contains information on various music streams");
    gtk_ctree_node_set_row_data_full(musicBrowserTree, streamTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          streams_pix);
    name[0] = "Favorites";
    favoritesTree = gtk_ctree_insert_node(musicBrowserTree, streamTree, NULL, 
                                          name, 5, pixmap, mask, pixmap, mask, 
                                          false, false);    
    data = NewTreeData(kTreeFavoriteStreamsHead, NULL, NULL, NULL, NULL, NULL,
                "This tree item contains all your favorite streams");
    gtk_ctree_node_set_row_data_full(musicBrowserTree, favoritesTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          wiredplanet_pix);
    name[0] = "Wired Planet";
    wiredplanetTree = gtk_ctree_insert_node(musicBrowserTree, streamTree, NULL, 
                                       name, 5,
                                       pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeWiredPlanetHead);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, wiredplanetTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    wiredplanetSpace = gtk_ctree_insert_node(musicBrowserTree, wiredplanetTree, 
                                             NULL, NULL, 5, NULL, NULL, NULL, 
                                             NULL, true, false);
    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          icecast_pix);
    name[0] = "IceCast";
    icecastTree = gtk_ctree_insert_node(musicBrowserTree, streamTree, NULL, 
                                       name, 5,
                                       pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeIcecastHead);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, icecastTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    icecastSpace = gtk_ctree_insert_node(musicBrowserTree, icecastTree, NULL,
                                         NULL, 5, NULL, NULL, NULL, NULL, true,
                                         false);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          shoutcast_pix);
    name[0] = "Shoutcast";
    shoutcastTree = gtk_ctree_insert_node(musicBrowserTree, streamTree, NULL, 
                                       name, 5,
                                       pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeShoutcastHead);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, shoutcastTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    shoutcastSpace = gtk_ctree_insert_node(musicBrowserTree, shoutcastTree, NULL,
                                           NULL, 5, NULL, NULL, NULL, NULL, 
                                           true, false);

    pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                          &style->bg[GTK_STATE_NORMAL],
                                          cd_pix);
    name[0] = "CD Audio";
    CDTree = gtk_ctree_insert_node(musicBrowserTree, NULL, NULL, name, 5,
                                   pixmap, mask, pixmap, mask, false, false);
    data = NewTreeData(kTreeCDHead, NULL, NULL, NULL, NULL, NULL,
                       "This tree item contains information on the CD that is currently in your CD-ROM", CDTracks);
    gtk_ctree_node_set_row_data_full(musicBrowserTree, CDTree, data,
                                     (GtkDestroyNotify)kill_treedata);

    wiredplanetExpanded = false;
    icecastExpanded = false;
    shoutcastExpanded = false;

    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

static void tree_status_clear(GtkWidget *w, GdkEventCrossing *event,
                              GTKMusicBrowser *p)
{
    gdk_threads_leave();
    p->AcceptEvent(new BrowserMessageEvent(" "));
    gdk_threads_enter();
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

    if (data) {
        gdk_threads_leave();
        switch (data->type) {
            case kTreeMyMusic:
            case kTreeAll:
            case kTreeUncat:
            case kTreePlaylistHead:
            case kTreePlaylist:
            case kTreeCDHead:
            case kTreeStreamsHead:
            case kTreeFavoriteStreamsHead:
                p->AcceptEvent(new BrowserMessageEvent(data->message.c_str()));
                break;
            default:
                p->AcceptEvent(new BrowserMessageEvent(" "));
                break;
        }
        gdk_threads_enter();
    }
}

static void tree_source_destroy(gpointer data)
{
}

static void tree_drag_begin(GtkWidget *w, GdkDragContext *context,
                            GTKMusicBrowser *p)
{
    GtkCTree *ctree;
    GtkCList *clist;

    g_return_if_fail(w != NULL);
    g_return_if_fail(GTK_IS_CTREE(w));
    g_return_if_fail(context != NULL);

    ctree = GTK_CTREE(w);
    clist = GTK_CLIST(w);

    GtkCTreeNode *node = GTK_CTREE_NODE(g_list_nth(clist->row_list,
                                                   clist->click_cell.row));
    if (node)
        gtk_ctree_select(ctree, node);

    vector<PlaylistItem *> *newlist = p->GetTreeSelection();

    if (newlist->size() == 0)
        return;

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
        vector<PlaylistItem *> *newlist = p->GetTreeSelection();
        p->AddTracksPlaylistEvent(newlist, true);

//        if (p->GetTreeClick() == kTreeFavoriteStreamsHead)
//            p->AddNewStream();
    }
    else {
        int row, column;

        if (!gtk_clist_get_selection_info(clist, (int)event->x, (int)event->y,
                                          &row, &column)) 
            return;

        GtkCTreeNode *node = GTK_CTREE_NODE(g_list_nth(clist->row_list, row));
        p->mbSelection = (TreeData *)gtk_ctree_node_get_row_data(ctree, node);
        p->SetTreeClick(p->mbSelection->type);

        if (event->button == 3) {
            gtk_ctree_select(ctree, node);
            p->TreeRightClick((int)event->x_root, (int)event->y_root,
                              event->time);
        }
    }

    p->SetClickState(kContextBrowser);
}

void GTKMusicBrowser::FillWiredPlanet(void)
{
    if (wiredplanetExpanded)
        return;

    if (wiredplanetSpace)
        gtk_ctree_remove_node(musicBrowserTree, wiredplanetSpace);
    wiredplanetSpace = NULL;

    char* stations[] = {
        "Alpha 2.0", // (Downtempo / Trip-Hop)",
        "Amplified", // (Punk, Garage, and Hard Alternative),
        "Axis", // (Jazz in its many forms)",
        "Butter", // (Melted grooves and dubbed out world-flavored Funk.),
        "DJ Mix",
        "Flipside",
        "Hearts of Space",
        "Nova Mundi", // (New Age / Ambient)",
        "Oscillations", // (Electronic / Trance / House)",
        "Pop 3",
        "Red Shift", // (Drum \'n Bass / Jungle)",
        "The Cut",
        "The Fringe",
        "World",
        "Radio EMusic",
        "Radio Epitaph",
        "Radio Radio",
        "Radio TMBG", // (They Might Be Giants)",
        "Noise Pop"
    };

    char* urls[] = {
        "http://ice1.wiredplanet.com:8001/alph",
        "http://ice1.wiredplanet.com:8001/ampp",
        "http://ice1.wiredplanet.com:8001/axis",
        "http://ice1.wiredplanet.com:8001/cjwp",
        "http://ice1.wiredplanet.com:8001/oaky",
        "http://ice1.wiredplanet.com:8001/flip",
        "http://ice1.wiredplanet.com:8001/hrts",
        "http://ice1.wiredplanet.com:8001/nova",
        "http://ice1.wiredplanet.com:8001/oscl",
        "http://ice1.wiredplanet.com:8001/popo",
        "http://ice1.wiredplanet.com:8001/reds",
        "http://ice1.wiredplanet.com:8001/tcut",
        "http://ice1.wiredplanet.com:8001/frng",
        "http://ice1.wiredplanet.com:8001/wrld",
        "http://ice1.wiredplanet.com:8001/emus",
        "http://ice1.wiredplanet.com:8001/punk",
        "http://ice1.wiredplanet.com:8001/elvi",
        "http://ice1.wiredplanet.com:8001/tmbg",
        "http://ice1.wiredplanet.com:8001/nois"
    };

    uint32 numStations = (sizeof(stations) / sizeof(char*));


    char *name[1];
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    GtkStyle  *style = gtk_widget_get_style(musicBrowserWindow);
    TreeData  *data;
    GtkCTreeNode *stream;
    gtk_clist_freeze(GTK_CLIST(musicBrowserTree));
 
    for (uint32 i = 0; i < numStations; i++) {
        PlaylistItem *newitem = new PlaylistItem;
        MetaData metadata;

        newitem->SetURL(urls[i]);
        metadata.SetTitle(stations[i]);
        metadata.SetArtist("Wired Planet");
        newitem->SetMetaData(&metadata);

        name[0] = stations[i];

        pixmap = gdk_pixmap_create_from_xpm_d(musicBrowserWindow->window, &mask,
                                              &style->bg[GTK_STATE_NORMAL],
                                              streams_pix);
        stream = gtk_ctree_insert_node(musicBrowserTree, wiredplanetTree, NULL,
                                        name, 5, pixmap, mask, pixmap, mask,
                                        true, false);
        data = NewTreeData(kTreeStream, NULL, NULL, NULL, newitem);
        gtk_ctree_node_set_row_data_full(musicBrowserTree, stream, data,
                                         (GtkDestroyNotify)kill_treedata);
    }
    gtk_clist_thaw(GTK_CLIST(musicBrowserTree));
}

void GTKMusicBrowser::FillIceCast(void)
{
    if (icecastExpanded)
        return;
    if (icecastSpace)
        gtk_ctree_remove_node(musicBrowserTree, icecastSpace);
    icecastSpace = NULL;
}

void GTKMusicBrowser::FillShoutCast(void)
{
    if (shoutcastExpanded)
        return;
    if (shoutcastSpace)
        gtk_ctree_remove_node(musicBrowserTree, shoutcastSpace);
    shoutcastSpace = NULL;
}

static void ctree_expand(GtkWidget *widget, GList *list, GTKMusicBrowser *p)
{
    GtkCTree *ctree = GTK_CTREE(widget);

    GtkCTreeNode *node = (GtkCTreeNode *)list;
    TreeData *data = (TreeData *)gtk_ctree_node_get_row_data(ctree, node);

    if (data) {
        switch (data->type) {
            case kTreeWiredPlanetHead: {
                p->FillWiredPlanet();
                break; }
            case kTreeIcecastHead: {
                p->FillIceCast();
                break; }
            case kTreeShoutcastHead: {
                p->FillShoutCast();
                break; }
            default:
                break;
        }
    }
}

void GTKMusicBrowser::TreeRightClick(int x, int y, uint32 time)
{
    GtkItemFactory *itemfact = NULL;

    switch (mbSelection->type) {
        case kTreeCD:
        case kTreeCDHead:
            itemfact = cdPopup;
            break;
        case kTreeStream:
            itemfact = streamPopup;
            break;
        case kTreeFavStream:
            itemfact = favPopup;
            break;
        case kTreeTrack:
            itemfact = trackPopup;
            break;
        case kTreePlaylist:
            itemfact = playlistCatPopup;
            break;
        case kTreeMyMusic:
        case kTreeAll:
        case kTreeUncat:
        case kTreeArtist:
        case kTreeAlbum:
            itemfact = otherPopup;
            break;
        case kTreeStreamsHead:
        case kTreeFavoriteStreamsHead:
            itemfact = genstreamPopup;
            break;
        default:
            break;
    }

    if (!itemfact)
        return;

    gtk_item_factory_popup(itemfact, x, y, 3, time);
}

static void add_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    vector<PlaylistItem *> *newlist = p->GetTreeSelection();
    p->AddTracksPlaylistEvent(newlist, true, false, true);
}

static void add_play_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    vector<PlaylistItem *> *newlist = p->GetTreeSelection();
    p->AddTracksPlaylistEvent(newlist, true, true, false);
}

static void remove_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    switch (p->GetTreeClick()) {
        case kTreePlaylist: {
            p->GetContext()->catalog->RemovePlaylist(p->mbSelection->playlistname.c_str());
            break; }
        case kTreeTrack: {
            p->GetContext()->catalog->RemoveSong(p->mbSelection->track->URL().c_str());
            break; }
        case kTreeFavStream: {
            p->GetContext()->catalog->RemoveStream(p->mbSelection->track->URL().c_str());
            break; }
        default:
            break;
    }
}
    
static void add_stream_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->AddNewStream();
}

static void add_fav_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->AddStreamToFavs();
}

static void eject_cd_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
    p->EjectCD();
}

static void edit_info_pop(GTKMusicBrowser *p, guint action, GtkWidget *w)
{
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

void GTKMusicBrowser::CreateTreePopups(void)
{
    GtkItemFactoryEntry genstream_items[] = {
     {"/Add New Stream", NULL,    (void(*)())add_stream_pop, 0, 0 }
    };
    int ngenstream_items = sizeof(genstream_items) / sizeof(genstream_items[0]);

    genstreamPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<genstream_popup>",
                                          NULL);
    gtk_item_factory_create_items(genstreamPopup, ngenstream_items, 
                                  genstream_items, (void*)this);

    GtkItemFactoryEntry stream_items[] = {
     {"/Add To Playlist",NULL,    (void(*)())add_pop,       0, 0 },
     {"/Add and Play Now",NULL,   (void(*)())add_play_pop,  0, 0 },
     {"/sep1",         NULL,      0,                        0, "<Separator>" },
     {"/Add To Favorites", NULL,  (void(*)())add_fav_pop,   0, 0 }
    };
    int nstream_items = sizeof(stream_items) / sizeof(stream_items[0]);

    streamPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<stream_popup>",
                                       NULL);
    gtk_item_factory_create_items(streamPopup, nstream_items, stream_items,
                                  (void*)this);

    GtkItemFactoryEntry fav_items[] = {
     {"/Add To Playlist",NULL,    (void(*)())add_pop,       0, 0 },
     {"/Add and Play Now",NULL,   (void(*)())add_play_pop,  0, 0 },
     {"/sep1",         NULL,      0,                        0, "<Separator>" },
     {"/Remove",       NULL,      (void(*)())remove_pop,    0, 0 }
    };
    int nfav_items = sizeof(fav_items) / sizeof(fav_items[0]);
    
    favPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<fav_popup>", NULL);
    gtk_item_factory_create_items(favPopup, nfav_items, fav_items, (void*)this);

    GtkItemFactoryEntry cd_items[] = {
     {"/Add To Playlist",NULL,  (void(*)())add_pop,  0, 0 },
     {"/Add and Play Now",NULL, (void(*)())add_play_pop,   0, 0 },
     {"/sep1",         NULL,    0,                        0, "<Separator>" },
     {"/Eject CD",     NULL,    (void(*)())eject_cd_pop,0, 0 }
    };
    int ncd_items = sizeof(cd_items) / sizeof(cd_items[0]);

    cdPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<cd_popup>", NULL);
    gtk_item_factory_create_items(cdPopup, ncd_items, cd_items, (void*)this);

    GtkItemFactoryEntry plist_items[] = {
     {"/Add To Playlist",NULL,  (void(*)())add_pop,  0, 0 },
     {"/Add and Play Now",NULL, (void(*)())add_play_pop,   0, 0 },
     {"/sep1",         NULL,    0,                        0, "<Separator>" },
     {"/Remove",       NULL,    (void(*)())remove_pop,     0, 0 },
     {"/sep2",         NULL,    0,                        0, "<Separator>" },
     {"/Edit",         NULL,    (void(*)())edit_info_pop,0, 0 }
    };
    int nplist_items = sizeof(plist_items) / sizeof(plist_items[0]);

    playlistCatPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<plist_popup>", 
                                            NULL);
    gtk_item_factory_create_items(playlistCatPopup, nplist_items, plist_items, 
                                  (void*)this);
    
    GtkItemFactoryEntry track_items[] = {
     {"/Add To Playlist",NULL,  (void(*)())add_pop,  0, 0 },
     {"/Add and Play Now",NULL, (void(*)())add_play_pop,   0, 0 },
     {"/sep1",         NULL,    0,                        0, "<Separator>" },
     {"/Remove",       NULL,    (void(*)())remove_pop,     0, 0 },
     {"/sep2",         NULL,    0,                        0, "<Separator>" },
     {"/Edit Info", NULL,       (void(*)())edit_info_pop,0, 0 }
    };
    int ntrack_items = sizeof(track_items) / sizeof(track_items[0]);

    trackPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<track_popup>", 
                                            NULL);
    gtk_item_factory_create_items(trackPopup, ntrack_items, track_items, 
                                  (void*)this);

    GtkItemFactoryEntry other_items[] = {
     {"/Add To Playlist",NULL,  (void(*)())add_pop,  0, 0 },
     {"/Add and Play Now",NULL, (void(*)())add_play_pop,   0, 0 },
    };
    int nother_items = sizeof(other_items) / sizeof(other_items[0]);

    otherPopup = gtk_item_factory_new(GTK_TYPE_MENU, "<other_popup>",      
                                            NULL);
    gtk_item_factory_create_items(otherPopup, nother_items, other_items,      
                                  (void*)this);
}

void GTKMusicBrowser::CreateTree(void)
{
    BADContext = m_context;

    GtkTargetEntry tree_target_table = {"tree-drag", 0, 1};

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
    gtk_signal_connect(GTK_OBJECT(musicBrowserTree), "tree_expand",
                       GTK_SIGNAL_FUNC(ctree_expand), this);
    gtk_clist_set_compare_func(GTK_CLIST(musicBrowserTree), nocase_compare);

    gtk_clist_set_row_height(GTK_CLIST(musicBrowserTree), 16);
    CreateMainTreeItems();

    CreateTreePopups();
}
