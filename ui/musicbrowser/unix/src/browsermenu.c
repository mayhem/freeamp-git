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
extern void import_list();
extern void export_list();
extern void music_search();
extern void quit_menu();
extern void infoedit();
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
extern void about();

void CreateMenuItems(GtkItemFactory *item_factory)
{
    GtkItemFactoryEntry menu_items[] = {
     {"/_File",                 NULL,           0,         0, "<Branch>" },
     {"/File/tearoff1",         NULL,           0,         0, "<Tearoff>" },
     {"/File/_New Playlist",    "<control>N",   new_plist, 0, 0 },
//     {"/File/_Open Playlist",   "<control>O",   open_list, 0, 0 },
//     {"/File/_Save Playlist",   "<control>S",   save_list, 0, 0 },
     {"/File/_Import Playlist", "<control>I",   open_list, 0, 0 },
     {"/File/_Export Playlist", "<control>E",   save_list, 0, 0 },
     {"/File/sep1",             NULL,           0,         0, "<Separator>" },
     {"/File/Search for Music", NULL,           music_search, 0, 0 },
     {"/File/sep2",             NULL,           0,         0, "<Separator>" },
     {"/File/_Quit",            "<control>Q",   quit_menu, 0, 0 }, 

     {"/_Edit",                 NULL,           0,         0, "<Branch>" },
//     {"/_Edit/Cut",             "<control>X",   0,         0, 0 },
//     {"/_Edit/_Copy",           "<control>C",   0,         0, 0 },
//     {"/_Edit/_Paste",          "<control>V",   0,         0, 0 },
//     {"/_Edit/_Delete",         "<control>D",   0,         0, 0 },
//     {"/_Edit/sep3",            NULL,           0,         0, "<Separator>" },
//     {"/_Edit/Undo",            "<control>Z",   0,         0, 0 },
//     {"/_Edit/Redo",            NULL,           0,         0, 0 },
//     {"/_Edit/sep4",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Information Viewer", NULL,        infoedit,  0, 0 },
     
     {"/_View",                 NULL,           0,         0, "<Branch>" },
     {"/_View/Music Catalog",   NULL,           catalog_tog, 0, "<CheckItem>" },

     {"/_Sort",                 NULL,           0,         0, "<Branch>" },
     {"/_Sort/Artist",          NULL,           sort_artist, 0, 0 },
     {"/_Sort/Title",           NULL,           sort_title, 0, 0 },
     {"/_Sort/Year",            NULL,           sort_year,  0, 0 },
     {"/_Sort/Track Number",    NULL,           sort_track, 0, 0 },
     {"/_Sort/Genre",           NULL,           sort_genre, 0, 0 },
     {"/_Sort/Length",          NULL,           sort_time,  0, 0 },
     {"/_Sort/Location",        NULL,           sort_location, 0, 0 },
     {"/_Sort/Randomize",       NULL,           sort_random, 0, 0 },

//     {"/_Help",                 NULL,           0,          0, "<LastBranch>" },
//     {"/_Help/_About",          NULL,           about,      0, 0 },
    };
    
    int nmenu_items = sizeof(menu_items) / sizeof(menu_items[0]);

    gtk_item_factory_create_items(item_factory, nmenu_items, menu_items, NULL);
}
