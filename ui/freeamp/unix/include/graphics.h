/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <X11/xpm.h>

#include <X11/extensions/shape.h>

#ifdef LOAD_GRAPHICS
#define EXTERN
#else
#define EXTERN extern
#endif

#ifdef LOAD_GRAPHICS
#include "logo.xpm"
#include "logo256.xpm"
#else
extern char *logo[];
extern char *logo256[];
#endif

#define RIGHT_SIDE_WIDTH 83
#define RIGHT_SIDE_HEIGHT 51
#ifdef LOAD_GRAPHICS
#include "rightside.xpm"
#include "rightside256.xpm"
#else
extern char *rightside[];
extern char *rightside256[];
#endif
EXTERN Pixmap right_side_pixmap;

#define LEFT_SIDE_WIDTH 138
#define LEFT_SIDE_HEIGHT 51
#ifdef LOAD_GRAPHICS
#include "leftside.xpm"
#include "leftside256.xpm"
#else
extern char *leftside[];
extern char *leftside256[];
#endif
EXTERN Pixmap left_side_pixmap;

#define LCD_UPPER_X 149
#define LCD_UPPER_Y 0
#define LCD_UPPER_WIDTH 190
#define LCD_UPPER_HEIGHT 25
#ifdef LOAD_GRAPHICS
#include "lcd_upper_frame.xpm"
#include "lcd_upper_frame256.xpm"
#else
extern char *lcd_upper_frame[];
extern char *lcd_upper_frame256[];
#endif
#include "lcd_upper_frame_mask.xbm"
EXTERN Pixmap lcd_upper_pixmap;
EXTERN Pixmap lcd_upper_mask_pixmap;

#define LCD_DISPLAY_X (LCD_UPPER_X + 16)
#define LCD_DISPLAY_Y (LCD_UPPER_Y + 8)
#define LCD_DISPLAY_WIDTH 159
#define LCD_DISPLAY_HEIGHT 31
#ifdef LOAD_GRAPHICS
#include "lcd_display.xpm"
#include "lcd_display256.xpm"
#else
extern char *lcd_display[];
extern char *lcd_display256[];
#endif
EXTERN Pixmap lcd_display_pixmap;

#define PLAYLIST_DRAWER_X 149
#define PLAYLIST_DRAWER_Y 25
#define PLAYLIST_DRAWER_WIDTH 190
#define PLAYLIST_DRAWER_HEIGHT 26
#ifdef LOAD_GRAPHICS
#include "playlist_drawer.xpm"
#include "playlist_drawer256.xpm"
#else
extern char *playlist_drawer[];
extern char *playlist_drawer256[];
#endif
#include "playlist_drawer_mask.xbm"
EXTERN Pixmap playlist_drawer_pixmap;
EXTERN Pixmap playlist_drawer_mask_pixmap;

#define VOLUME_DIAL_X LEFT_SIDE_WIDTH
#define VOLUME_DIAL_Y 0
#define SEEK_DIAL_X LEFT_SIDE_WIDTH + SINGLE_DIAL_WIDTH + LCD_UPPER_WIDTH
#define SEEK_DIAL_Y 0
#define DIALS_WIDTH 66
#define DIALS_HEIGHT 51
#define SINGLE_DIAL_WIDTH 11
#ifdef LOAD_GRAPHICS
#include "dials.xpm"
#include "dials256.xpm"
#else
extern char *dials[];
extern char *dials256[];
#endif
EXTERN Pixmap dials_pixmap;

#define TOTAL_WIDTH (LEFT_SIDE_WIDTH + SINGLE_DIAL_WIDTH + LCD_UPPER_WIDTH + SINGLE_DIAL_WIDTH + RIGHT_SIDE_WIDTH)
#define TOTAL_HEIGHT 51

#include "player_full_mask.xbm"

#define PLAYER_FULL_MASK_WIDTH player_full_mask_width
#define PLAYER_FULL_MASK_HEIGHT player_full_mask_height
EXTERN Pixmap player_full_mask_pixmap;

#define DISTANCE_BETWEEN_MAJORS 27

#define PLAY_BUTTON_X 29
#define PLAY_BUTTON_Y 15
#define PLAY_BUTTON_WIDTH 20
#define PLAY_BUTTON_HEIGHT 20
#ifdef LOAD_GRAPHICS
#include "play_buttons.xpm"
#include "play_buttons256.xpm"
#else
extern char *play_buttons[];
extern char *play_buttons256[];
#endif
EXTERN Pixmap play_buttons_pixmap;

#define STOP_BUTTON_X 29
#define STOP_BUTTON_Y 15
#define STOP_BUTTON_WIDTH 20
#define STOP_BUTTON_HEIGHT 20
#ifdef LOAD_GRAPHICS
#include "stop_buttons.xpm"
#include "stop_buttons256.xpm"
#else
extern char *stop_buttons[];
extern char *stop_buttons256[];
#endif
EXTERN Pixmap stop_buttons_pixmap;

#define PAUSE_BUTTON_X (DISTANCE_BETWEEN_MAJORS + PLAY_BUTTON_X)
#define PAUSE_BUTTON_Y 15
#define PAUSE_BUTTON_WIDTH 20
#define PAUSE_BUTTON_HEIGHT 20
#ifdef LOAD_GRAPHICS
#include "pause_buttons.xpm"
#include "pause_buttons256.xpm"
#else
extern char *pause_buttons[];
extern char *pause_buttons256[];
#endif
EXTERN Pixmap pause_buttons_pixmap;


#define PREV_BUTTON_X (DISTANCE_BETWEEN_MAJORS + PAUSE_BUTTON_X - 1)
#define PREV_BUTTON_Y 15
#define PREV_BUTTON_WIDTH 20
#define PREV_BUTTON_HEIGHT 20
#ifdef LOAD_GRAPHICS
#include "last_buttons.xpm"
#include "last_buttons256.xpm"
#else
extern char *last_buttons[];
extern char *last_buttons256[];
#endif
EXTERN Pixmap prev_buttons_pixmap;

#define NEXT_BUTTON_X (DISTANCE_BETWEEN_MAJORS + PREV_BUTTON_X)
#define NEXT_BUTTON_Y 15
#define NEXT_BUTTON_WIDTH 20
#define NEXT_BUTTON_HEIGHT 20
#ifdef LOAD_GRAPHICS
#include "next_buttons.xpm"
#include "next_buttons256.xpm"
#else
extern char *next_buttons[];
extern char *next_buttons256[];
#endif
EXTERN Pixmap next_buttons_pixmap;


#define MAJOR_BUTTON_MASK_WIDTH 20
#define MAJOR_BUTTON_MASK_HEIGHT 20
#include "major_button_mask.xbm"
EXTERN Pixmap major_button_mask_pixmap;

#define MINOR_BUTTON_MASK_WIDTH 13
#define MINOR_BUTTON_MASK_HEIGHT 13
#include "minor_button_mask.xbm"
EXTERN Pixmap minor_button_mask_pixmap;

#define SWITCH_MODE_BUTTON_X 360
#define SWITCH_MODE_BUTTON_Y 9
#define SWITCH_MODE_BUTTON_WIDTH 13
#define SWITCH_MODE_BUTTON_HEIGHT 13
#ifdef LOAD_GRAPHICS
#include "mode_buttons.xpm"
#include "mode_buttons256.xpm"
#else
extern char *mode_buttons[];
extern char *mode_buttons256[];
#endif
EXTERN Pixmap mode_buttons_pixmap;

#define MINIMIZE_BUTTON_X (SWITCH_MODE_BUTTON_X + 16)
#define MINIMIZE_BUTTON_Y 9
#define MINIMIZE_BUTTON_WIDTH 13
#define MINIMIZE_BUTTON_HEIGHT 13
#ifdef LOAD_GRAPHICS
#include "minimize_buttons.xpm"
#include "minimize_buttons256.xpm"
#else
extern char *minimize_buttons[];
extern char *minimize_buttons256[];
#endif
EXTERN Pixmap minimize_buttons_pixmap;

#define CLOSE_BUTTON_X (MINIMIZE_BUTTON_X + 16)
#define CLOSE_BUTTON_Y 9
#define CLOSE_BUTTON_WIDTH 13
#define CLOSE_BUTTON_HEIGHT 13
#ifdef LOAD_GRAPHICS
#include "close_buttons.xpm"
#include "close_buttons256.xpm"
#else
extern char *close_buttons[];
extern char *close_buttons256[];
#endif
EXTERN Pixmap close_buttons_pixmap;

#define REPEAT_BUTTON_X 360
#define REPEAT_BUTTON_Y 30
#define REPEAT_BUTTON_WIDTH 13
#define REPEAT_BUTTON_HEIGHT 13
#ifdef LOAD_GRAPHICS
#include "repeat_buttons.xpm"
#include "repeat_buttons256.xpm"
#else
extern char *repeat_buttons[];
extern char *repeat_buttons256[];
#endif
EXTERN Pixmap repeat_buttons_pixmap;

#define SHUFFLE_BUTTON_X (REPEAT_BUTTON_X + 16)
#define SHUFFLE_BUTTON_Y 30
#define SHUFFLE_BUTTON_WIDTH 13
#define SHUFFLE_BUTTON_HEIGHT 13
#ifdef LOAD_GRAPHICS
#include "shuffle_buttons.xpm"
#include "shuffle_buttons256.xpm"
#else
extern char *shuffle_buttons[];
extern char *shuffle_buttons256[];
#endif
EXTERN Pixmap shuffle_buttons_pixmap;

#define OPEN_BUTTON_X (SHUFFLE_BUTTON_X + 16)
#define OPEN_BUTTON_Y 30
#define OPEN_BUTTON_WIDTH 13
#define OPEN_BUTTON_HEIGHT 13
#ifdef LOAD_GRAPHICS
#include "open_buttons.xpm"
#include "open_buttons256.xpm"
#else
extern char *open_buttons[];
extern char *open_buttons256[];
#endif
EXTERN Pixmap open_buttons_pixmap;


#ifdef LOAD_GRAPHICS
#include "small_font.xpm"
#include "small_font256.xpm"
#else
extern char *small_font[];
extern char *small_font256[];
#endif
EXTERN Pixmap small_font_pixmap;


#ifdef LOAD_GRAPHICS
#include "big_font.xpm"
#include "big_font256.xpm"
#else
extern char *big_font[];
extern char *big_font256[];
#endif
EXTERN Pixmap big_font_pixmap;

#define SHUFFLED_ICON_X 145
#define SHUFFLED_ICON_Y 4
#define SHUFFLED_ICON_WIDTH 11
#define SHUFFLED_ICON_HEIGHT 6
#ifdef LOAD_GRAPHICS
#include "shuffled_icon.xpm"
#include "shuffled_icon256.xpm"
#else
extern char *shuffled_icon[];
extern char *shuffled_icon256[];
#endif
EXTERN Pixmap shuffled_icon_pixmap;

#define REPEAT_ICON_X 144
#define REPEAT_ICON_Y 14
#define REPEAT_ICON_WIDTH 12
#define REPEAT_ICON_HEIGHT 7
#ifdef LOAD_GRAPHICS
#include "repeat_icon.xpm"
#include "repeat_icon256.xpm"
#else
extern char *repeat_icon[];
extern char *repeat_icon256[];
#endif
EXTERN Pixmap repeat_icon_pixmap;

#define REPEAT_ALL_ICON_X 145
#define REPEAT_ALL_ICON_Y 21
#define REPEAT_ALL_ICON_WIDTH 11
#define REPEAT_ALL_ICON_HEIGHT 8
#ifdef LOAD_GRAPHICS
#include "all_icon.xpm"
#include "all_icon256.xpm"
#else
extern char *all_icon[];
extern char *all_icon256[];
#endif
EXTERN Pixmap all_icon_pixmap;

#undef EXTERN
#endif // _GRAPHICS_H_
