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

#ifndef _CONTROL_INFO_H_
#define _CONTROL_INFO_H_ 

#include "utility.h"

extern Color g_controlColors[];


#define kPlayControl            0
#define kPauseControl           1
#define kLastControl            2
#define kNextControl            3
#define kStopControl            4
#define kModeControl            5
#define kMinimizeControl        6
#define kCloseControl           7
#define kRepeatControl          8
#define kShuffleControl         9
#define kOpenControl            10
#define kVolumeControl          11
#define kSeekControl            12
#define kPlaylistControl        13
#define kSongTitleControl       14
#define kSongInfoControl        15
#define kRepeatStatusControl    16
#define kRepeatAllStatusControl 17
#define kShuffleStatusControl   18
#define kResizeControl          19
#define kPlaylistDisplayControl 20
#define kPanelBackingControl    21
#define kScrollbarControl       22
#define kAddControl             23
#define kDeleteControl          24
#define kSaveControl            25
#define kLoadControl            26

#define kFinalControl           27


#define kNumControls            kFinalControl

// "virtual controls"...
#define kAboutControl           kFinalControl + 1
#define kPrefControl            kFinalControl + 2


#endif /* _CONTROL_INFO_H_ */