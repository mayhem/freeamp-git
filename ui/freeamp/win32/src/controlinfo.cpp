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

/* system headers */
#define STRICT
#include <windows.h>

/* project headers */
#include "config.h"
#include "utility.h"

Color g_controlColors[] = {
        {0,   0,   255, 0}, // kPlayControl
        {255, 255, 0,   0}, // kPauseControl
        {0,   255, 0,   0}, // kLastControl
        {0,   255, 255, 0}, // kNextControl
        {0,   0,   255, 0}, // kStopControl
        {128, 128, 0,   0}, // kModeControl
        {128, 0,   128, 0}, // kMinimizeControl
        {0,   0,   128, 0}, // kCloseControl
        {0,   128, 128, 0}, // kRepeatControl
        {0,   128, 0,   0}, // kShuffleControl
        {192, 220, 192, 0}, // kOpenControl
        {255, 0,   255, 0}, // kVolumeControl
        {128, 0,   0,   0}, // kSeekControl
        {255, 0,   0,   0}, // kPlaylistControl
        {128, 128, 128, 0}, // kSongTitleControl
        {42,  63,  85,  0}, // kSongInfoControl
        {255, 255, 85,  0}, // kRepeatStatusControl
        {0,   191, 255, 0}, // kRepeatAllStatusControl
        {255, 95,  170, 0}, // kShuffleStatusControl
        {42,  191, 255, 0}, // kPlaylistDisplayControl
        {212, 0,   85,  0}, // kPanelBackingControl
        {0,   191, 85, 0}   // kScrollbarControl
};
