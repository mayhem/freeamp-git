
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
	
	$$
____________________________________________________________________________*/


#ifndef _WindowHash_H_
#define _WindowHash_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <iostream.h>

#include "config.h"
#include "fawindow.h"

class WindowHash {
    class HashElem {
    public:
	HashElem *next;
	Window key;
	FAWindow *val;
    };
    HashElem **hashSlots;
    int32 numSlots;

    int32 hashkey(Window);
 public:
    WindowHash(int32 slots = 10);
    ~WindowHash();

    FAWindow *Value(Window key);
    void Insert(Window key, FAWindow *val);
    void Remove(Window key);
};

#endif // _WindowHash_H_
