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

#ifndef _CTRLOBJ_H_
#define _CTRLOBJ_H_

#include "event.h"


class UserInterface : public EventQueue {
 public:
    virtual int32 AcceptEvent(Event *) = 0;
    virtual ~UserInterface() {}
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct UI{
    void*   ref;

    EventQueue* (*Target)   (struct UI);
    void        (*Cleanup)  (struct UI*);

}UI, *UIRef;

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _CTRLOBJ_H_
