/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 EMusic.com

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
#include "config.h"

#include <kernel/OS.h>
#include <support/Debug.h>
#include <string.h>

#include "mutex.h"

Mutex::
Mutex( bool createOwned )
:	m_lock( true )
{
	if ( createOwned ) {
		m_lock.Lock();
	}
}

Mutex::
~Mutex()
{
}


bool 
Mutex::
Acquire(long timeout)
{
	bool	result = false;

	if ( timeout == WAIT_FOREVER ) {
		result = m_lock.Lock();
	} else {
		// timeout is in second, bigtime_t is in miliseconds.
		status_t st =  m_lock.LockWithTimeout( (bigtime_t)(timeout * 1000) );
		result = ( st == B_NO_ERROR );
	}
    return ( result );
}

void 
Mutex::
Release()
{
	m_lock.Unlock();
}

void Mutex::DumpMutex(void)
{
}
