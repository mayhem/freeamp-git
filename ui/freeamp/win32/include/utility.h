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

#ifndef _UTILITY_H_
#define _UTILITY_H_

/* system headers */
#define STRICT
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>

/* project headers */  
#include "list.h"
#include "dib.h"

extern const char* kSaveToRio;

HRGN
DetermineRegion(DIB* bitmap, 
			    Color* color);

void
DetermineControlRegions(DIB* bitmap, 
                        HRGN controlRegions[],
			            Color controlColors[],
                        int32 numControls);

bool FileOpenDialog(HWND hwnd, 
                    const char* title,
                    const char* filter,
                    List<char*>* fileList);

bool FileSaveDialog(HWND hwnd, 
                    const char* title,
                    const char* filter,
                    char* path,
                    uint32* pathLength);


#endif /* _UTILITY_H_ */