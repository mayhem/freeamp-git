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
extern void options_show();
extern void pause_menu();
extern void play_menu();
extern void next_menu();
extern void prev_menu();
extern void repeat_none();
extern void repeat_one();
extern void repeat_all();
extern void show_help();
extern void freeamp_web();
extern void emusic_web();
extern void show_about();

void CreateMenuItems(GtkItemFactory *item_factory, void *p)
{
    GtkItemFactoryEntry menu_items[] = {
     {"/_File",                 NULL,           0,         0, "<Branch>" },
     {"/File/_New Playlist...",    "<control>N",new_plist, 0, 0 },
     {"/File/_Open Playlist...",   "<control>O",open_list, 0, 0 },
     {"/File/_Save Playlist",   "<control>S",   save_list, 0, 0 },
     {"/File/Save Playlist _As...","<control>A",saveas_list,0, 0 },
     {"/File/sep1",             NULL,           0,         0, "<Separator>" },
     {"/File/_Import Tracks and Playlists...","<control>I", import_list, 0, 0 },
     {"/File/Search for Music...", NULL,        music_search, 0, 0 },
     {"/File/sep2",             NULL,           0,         0, "<Separator>" },
     {"/File/_Close",           "<control>Q",   quit_menu, 0, 0 }, 

     {"/_Edit",                 NULL,           0,         0, "<Branch>" },
     {"/_Edit/Add Files from Disk...",    NULL, add_track, 0, 0 },
     {"/_Edit/Remove from Catalog", NULL,       delete_sel,0, 0 },
     {"/_Edit/Remove from Disk",NULL,           realdelete_sel, 0, 0},
     {"/_Edit/sep3",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Move Up",         NULL,           move_up,   0, 0 },
     {"/_Edit/Move Down",       NULL,           move_down, 0, 0 },
     {"/_Edit/sep4",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Clear Playlist",  NULL,           clear_list, 0, 0 },
     {"/_Edit/Randomize Playlist",NULL,         sort_random, 0, 0 },
     {"/_Edit/sep5",            NULL,           0,         0, "<Separator>" },
     {"/_Edit/Edit Track Info and Playlists..", NULL, infoedit,  0, 0 },

     {"/_View",                 NULL,           0,         0, "<Branch>" },
     {"/_View/View Music Browser",NULL,         catalog_tog, 0, 0 },
     {"/_View/Options",         NULL,           options_show, 0, 0 },
 
     {"/_Controls",             NULL,           0,         0, "<Branch>" },
     {"/_Controls/Pause",       NULL,           pause_menu,0, 0 },
     {"/_Controls/Play",        NULL,           play_menu, 0, 0 },
     {"/_Controls/sep6",        NULL,           0,         0, "<Separator>" },
     {"/_Controls/Next Song",   NULL,           next_menu, 0, 0 },
     {"/_Controls/Previous Song", NULL,         prev_menu, 0, 0 },
     {"/_Controls/sep7",        NULL,           0,         0, "<Separator>" },
     {"/_Controls/Shuffle",     NULL,           sort_random, 0, 0 },
     {"/_Controls/sep8",        NULL,           0,         0, "<Separator>" },
     {"/_Controls/Repeat None", NULL,           repeat_none, 0, "<RadioItem>" },
     {"/_Controls/Repeat One",  NULL,  repeat_one, 0, "/Controls/Repeat None" },
     {"/_Controls/Repeat All",  NULL,  repeat_all, 0, "/Controls/Repeat None" },

     {"/_Sort",                 NULL,           0,         0, "<Branch>" },
     {"/_Sort/Artist",          NULL,           sort_artist, 0, 0 },
     {"/_Sort/Album",           NULL,           sort_album, 0, 0 },
     {"/_Sort/Title",           NULL,           sort_title, 0, 0 },
     {"/_Sort/Year",            NULL,           sort_year,  0, 0 },
     {"/_Sort/Track Number",    NULL,           sort_track, 0, 0 },
     {"/_Sort/Genre",           NULL,           sort_genre, 0, 0 },
     {"/_Sort/Length",          NULL,           sort_time,  0, 0 },
     {"/_Sort/Location",        NULL,           sort_location, 0, 0 },

     {"/_Help",                 NULL,           0,          0, "<Branch>" },
     {"/_Help/Contents",        NULL,           show_help,  0, 0 },
     {"/_Help/sep9",            NULL,           0,          0, "<Separator>" },
     {"/_Help/FreeAmp Website", NULL,           freeamp_web, 0, 0 },
     {"/_Help/EMusic.com Website", NULL,        emusic_web, 0, 0 }, 
     {"/_Help/sep10",           NULL,           0,          0, "<Separator>" },
     {"/_Help/About",           NULL,           show_about, 0, 0 }
    };
    
    int nmenu_items = sizeof(menu_items) / sizeof(menu_items[0]);

    gtk_item_factory_create_items(item_factory, nmenu_items, menu_items, p);
}
