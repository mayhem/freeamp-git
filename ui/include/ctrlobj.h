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

// ctrlobj.h
// Controller Input and Output  Object

#ifndef _CTRLOBJ_H_
#define _CTRLOBJ_H_

#include "event.h"

class COO {
 public:
    virtual int32 acceptCOOEvent(Event *) = 0;
    virtual void setArgs(int, char **) = 0;
    virtual ~COO() {}
};

class CIO {
 public:
    virtual int32 acceptCIOEvent(Event *) = 0;
    virtual void setArgs(int, char **) = 0;
    virtual ~CIO() {}
};

#endif // _CTRLOBJ_H_
