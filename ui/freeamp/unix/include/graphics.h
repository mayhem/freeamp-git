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


#include "logo.xpm"
#include "logo256.xpm"

#define RIGHT_SIDE_WIDTH 83
#define RIGHT_SIDE_HEIGHT 51
#include "rightside.xpm"
#include "rightside256.xpm"
Pixmap right_side_pixmap;

#define LEFT_SIDE_WIDTH 138
#define LEFT_SIDE_HEIGHT 51
#include "leftside.xpm"
#include "leftside256.xpm"
Pixmap left_side_pixmap;

#define LCD_UPPER_X 149
#define LCD_UPPER_Y 0
#define LCD_UPPER_WIDTH 190
#define LCD_UPPER_HEIGHT 25
#include "lcd_upper_frame.xpm"
#include "lcd_upper_frame256.xpm"
#include "lcd_upper_frame_mask.xbm"
Pixmap lcd_upper_pixmap;
Pixmap lcd_upper_mask_pixmap;

#define LCD_DISPLAY_X (LCD_UPPER_X + 16)
#define LCD_DISPLAY_Y (LCD_UPPER_Y + 8)
#define LCD_DISPLAY_WIDTH 159
#define LCD_DISPLAY_HEIGHT 31
#include "lcd_display.xpm"
#include "lcd_display256.xpm"
Pixmap lcd_display_pixmap;

#define PLAYLIST_DRAWER_X 149
#define PLAYLIST_DRAWER_Y 25
#define PLAYLIST_DRAWER_WIDTH 190
#define PLAYLIST_DRAWER_HEIGHT 26
#include "playlist_drawer.xpm"
#include "playlist_drawer256.xpm"
#include "playlist_drawer_mask.xbm"
Pixmap playlist_drawer_pixmap;
Pixmap playlist_drawer_mask_pixmap;

#define VOLUME_DIAL_X LEFT_SIDE_WIDTH
#define VOLUME_DIAL_Y 0
#define SEEK_DIAL_X LEFT_SIDE_WIDTH + SINGLE_DIAL_WIDTH + LCD_UPPER_WIDTH
#define SEEK_DIAL_Y 0
#define DIALS_WIDTH 66
#define DIALS_HEIGHT 51
#define SINGLE_DIAL_WIDTH 11
#include "dials.xpm"
#include "dials256.xpm"
Pixmap dials_pixmap;

#define TOTAL_WIDTH (LEFT_SIDE_WIDTH + SINGLE_DIAL_WIDTH + LCD_UPPER_WIDTH + SINGLE_DIAL_WIDTH + RIGHT_SIDE_WIDTH)
#define TOTAL_HEIGHT 51

#include "player_full_mask.xbm"

#define PLAYER_FULL_MASK_WIDTH player_full_mask_width
#define PLAYER_FULL_MASK_HEIGHT player_full_mask_height
Pixmap player_full_mask_pixmap;

#define DISTANCE_BETWEEN_MAJORS 27

#define PLAY_BUTTON_X 29
#define PLAY_BUTTON_Y 15
#define PLAY_BUTTON_WIDTH 20
#define PLAY_BUTTON_HEIGHT 20
#include "play_buttons.xpm"
#include "play_buttons256.xpm"
Pixmap play_buttons_pixmap;

#define STOP_BUTTON_X 29
#define STOP_BUTTON_Y 15
#define STOP_BUTTON_WIDTH 20
#define STOP_BUTTON_HEIGHT 20
#include "stop_buttons.xpm"
#include "stop_buttons256.xpm"
Pixmap stop_buttons_pixmap;

#define PAUSE_BUTTON_X (DISTANCE_BETWEEN_MAJORS + PLAY_BUTTON_X)
#define PAUSE_BUTTON_Y 15
#define PAUSE_BUTTON_WIDTH 20
#define PAUSE_BUTTON_HEIGHT 20
#include "pause_buttons.xpm"
#include "pause_buttons256.xpm"
Pixmap pause_buttons_pixmap;


#define PREV_BUTTON_X (DISTANCE_BETWEEN_MAJORS + PAUSE_BUTTON_X - 1)
#define PREV_BUTTON_Y 15
#define PREV_BUTTON_WIDTH 20
#define PREV_BUTTON_HEIGHT 20
#include "last_buttons.xpm"
#include "last_buttons256.xpm"
Pixmap prev_buttons_pixmap;

#define NEXT_BUTTON_X (DISTANCE_BETWEEN_MAJORS + PREV_BUTTON_X)
#define NEXT_BUTTON_Y 15
#define NEXT_BUTTON_WIDTH 20
#define NEXT_BUTTON_HEIGHT 20
#include "next_buttons.xpm"
#include "next_buttons256.xpm"
Pixmap next_buttons_pixmap;


#define MAJOR_BUTTON_MASK_WIDTH 20
#define MAJOR_BUTTON_MASK_HEIGHT 20
#include "major_button_mask.xbm"
Pixmap major_button_mask_pixmap;

#define MINOR_BUTTON_MASK_WIDTH 13
#define MINOR_BUTTON_MASK_HEIGHT 13
#include "minor_button_mask.xbm"
Pixmap minor_button_mask_pixmap;

#define SWITCH_MODE_BUTTON_X 360
#define SWITCH_MODE_BUTTON_Y 9
#define SWITCH_MODE_BUTTON_WIDTH 13
#define SWITCH_MODE_BUTTON_HEIGHT 13
#include "mode_buttons.xpm"
#include "mode_buttons256.xpm"
Pixmap mode_buttons_pixmap;

#define MINIMIZE_BUTTON_X (SWITCH_MODE_BUTTON_X + 16)
#define MINIMIZE_BUTTON_Y 9
#define MINIMIZE_BUTTON_WIDTH 13
#define MINIMIZE_BUTTON_HEIGHT 13
#include "minimize_buttons.xpm"
#include "minimize_buttons256.xpm"
Pixmap minimize_buttons_pixmap;

#define CLOSE_BUTTON_X (MINIMIZE_BUTTON_X + 16)
#define CLOSE_BUTTON_Y 9
#define CLOSE_BUTTON_WIDTH 13
#define CLOSE_BUTTON_HEIGHT 13
#include "close_buttons.xpm"
#include "close_buttons256.xpm"
Pixmap close_buttons_pixmap;

#define REPEAT_BUTTON_X 360
#define REPEAT_BUTTON_Y 30
#define REPEAT_BUTTON_WIDTH 13
#define REPEAT_BUTTON_HEIGHT 13
#include "repeat_buttons.xpm"
#include "repeat_buttons256.xpm"
Pixmap repeat_buttons_pixmap;

#define SHUFFLE_BUTTON_X (REPEAT_BUTTON_X + 16)
#define SHUFFLE_BUTTON_Y 30
#define SHUFFLE_BUTTON_WIDTH 13
#define SHUFFLE_BUTTON_HEIGHT 13
#include "shuffle_buttons.xpm"
#include "shuffle_buttons256.xpm"
Pixmap shuffle_buttons_pixmap;

#define OPEN_BUTTON_X (SHUFFLE_BUTTON_X + 16)
#define OPEN_BUTTON_Y 30
#define OPEN_BUTTON_WIDTH 13
#define OPEN_BUTTON_HEIGHT 13
#include "open_buttons.xpm"
#include "open_buttons256.xpm"
Pixmap open_buttons_pixmap;


#include "small_font.xpm"
#include "small_font256.xpm"
Pixmap small_font_pixmap;

#include "big_font.xpm"
#include "big_font256.xpm"
Pixmap big_font_pixmap;

#define SHUFFLED_ICON_X 145
#define SHUFFLED_ICON_Y 4
#define SHUFFLED_ICON_WIDTH 11
#define SHUFFLED_ICON_HEIGHT 6
#include "shuffled_icon.xpm"
#include "shuffled_icon256.xpm"
Pixmap shuffled_icon_pixmap;

#define REPEAT_ICON_X 144
#define REPEAT_ICON_Y 14
#define REPEAT_ICON_WIDTH 12
#define REPEAT_ICON_HEIGHT 7
#include "repeat_icon.xpm"
#include "repeat_icon256.xpm"
Pixmap repeat_icon_pixmap;

#define REPEAT_ALL_ICON_X 145
#define REPEAT_ALL_ICON_Y 21
#define REPEAT_ALL_ICON_WIDTH 11
#define REPEAT_ALL_ICON_HEIGHT 8
#include "all_icon.xpm"
#include "all_icon256.xpm"
Pixmap all_icon_pixmap;


#endif // _GRAPHICS_H_
