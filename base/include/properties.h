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

#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include "config.h"
#include "errors.h"

class PropertyWatcher {
 public:
    virtual Error PropertyChange(const char *, void *) = 0;
};

class Properties {
 public:
    virtual Error GetProperty(const char *, void **) = 0;
    virtual Error SetProperty(const char *, void *, bool) = 0;
    virtual Error RegisterPropertyWatcher(const char *, PropertyWatcher *) = 0;
    virtual Error RemovePropertyWatcher(const char *, PropertyWatcher *) = 0;
};


#endif // _PROPERTIES_H_
