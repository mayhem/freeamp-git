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

extern void new_plist();
extern void open_list();
extern void save_list();
extern void saveas_list();
extern void import_list();
extern void music_search();
extern void quit_menu();
extern void add_track();
extern void infoedit();
extern void delete_sel();
extern void realdelete_sel();
extern void move_up();
extern void move_down();
extern void clear_list();
extern void catalog_tog();
extern void sort_random();
extern void sort_location();
extern void sort_time();
extern void sort_genre();
extern void sort_track();
extern void sort_year();
extern void sort_title();
extern void sort_album();
extern void sort_artist();

void CreateMenuItems(GtkItemFactory *item_factory, void *p)
{
    GtkItemFactoryEntry menu_items[] = {
     {"/_File",                 NULL,           0,         0, "<Branch>" },
     {"/File/tearoff1",         NULL,           0,         0, "<Tearoff>" },
     {"/File/_New Playlist",    "<control>N",   new_plist, 0, 0 },
     {"/File/_Open Playlist",   "<control>O",   open_list, 0, 0 },
     {"/File/_Save Playlist",   "<control>S",   save_list, 0, 0 },
     {"/File/Save _As Playlist","<control>A",   saveas_list,0, 0 },
     {"/File/sep1",             NULL,           0,         0, "<Separator>" },
     {"/File/_Import Playlist", "<control>I",   import_list, 0, 0 },
     {"/File/Search for Music", NULL,           music_search, 0, 0 },
     {"/File/sep2",             NULL,           0,         0, "<Separator>" },
     {"/File/_Close",           "<control>Q",   quit_menu, 0, 0 }, 

     {"/_Edit",                 NULL,           0,         0, "<Branch>" },
     {"/_Edit/Add Track",       NULL,           add_track, 0, 0 },
     {"/_Edit/Edit Track Information\\Playlist", NULL,  infoedit,  0, 0 },
     {"/_Edit/Remove Track\\Playlist", NULL,     delete_sel,0, 0 },
     {"/_Edit/Remove Track\\Playlist From Disk", NULL, realdelete_sel, 0, 0},
     {"/_Edit/sep3",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Up",              NULL,           move_up,   0, 0 },
     {"/_Edit/Down",            NULL,           move_down, 0, 0 },
     {"/_Edit/sep4",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Clear Playlist",  NULL,           clear_list, 0, 0 },
     {"/_Edit/Randomize",       NULL,           sort_random, 0, 0 },

     {"/_View",                 NULL,           0,         0, "<Branch>" },
     {"/_View/Music Catalog",   NULL,           catalog_tog, 0, 0 },

     {"/_Sort",                 NULL,           0,         0, "<Branch>" },
     {"/_Sort/Artist",          NULL,           sort_artist, 0, 0 },
     {"/_Sort/Album",           NULL,           sort_album, 0, 0 },
     {"/_Sort/Title",           NULL,           sort_title, 0, 0 },
     {"/_Sort/Year",            NULL,           sort_year,  0, 0 },
     {"/_Sort/Track Number",    NULL,           sort_track, 0, 0 },
     {"/_Sort/Genre",           NULL,           sort_genre, 0, 0 },
     {"/_Sort/Length",          NULL,           sort_time,  0, 0 },
     {"/_Sort/Location",        NULL,           sort_location, 0, 0 },
     {"/_Sort/Randomize",       NULL,           sort_random, 0, 0 },
    };
    
    int nmenu_items = sizeof(menu_items) / sizeof(menu_items[0]);

    gtk_item_factory_create_items(item_factory, nmenu_items, menu_items, p);
}
