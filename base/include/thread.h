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

#ifndef THREAD_H
#define THREAD_H

#include "config.h"


typedef void (*thread_function)(void * arg);

typedef enum Priority{
	Idle			= -15,
	Low				= -2,
	BelowNormal		= -1,
	Normal			= 0,
	AboveNormal		= 1,
	High			= 2,
	Critical		= 15,

}Priority;

class Thread {

public:
    virtual ~Thread(){}


	virtual bool Create(thread_function function, void* arg) = 0;
	virtual void Destroy() = 0;
	virtual void Suspend() = 0;
	virtual void Resume() = 0;
	virtual void Join() = 0;

	virtual Priority GetPriority() const = 0;
	virtual Priority SetPriority(Priority priority) = 0;

    static Thread* CreateThread();
};



#endif /* THREAD_H */
