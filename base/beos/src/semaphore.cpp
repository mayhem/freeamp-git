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


#include "config.h"

#include <kernel/OS.h>
#include <support/Debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

#include "semaphore.h"

#define SEM_DEBUG 0

Semaphore::Semaphore( int cnt )
:	count( cnt - 1 )
{
	if ( ( mutex = create_sem( count, "Semaphore" ) ) < B_NO_ERROR )
	{
		perror( "error creating semaphore" );
	}
	PRINT(( "Semaphore ctor success, sem_id(%d), count(%d)\n", mutex, count ));
}

Semaphore::Semaphore( const char* name, int cnt )
:	count( cnt - 1 )
{
	if ( ( mutex = create_sem( count, name ) ) < B_NO_ERROR )
	{
		perror( "error creating semaphore" );
	}
}

Semaphore::~Semaphore()
{
	if ( delete_sem( mutex ) < B_NO_ERROR )
	{
		perror( "Semaphore: couldn't delete semaphore" );
	}
}

void Semaphore::Wait()
{
#if SEM_DEBUG
	int32	tc;
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Wait:id(%d):count(%d)\n", mutex, tc ));
#endif
	if ( acquire_sem( mutex ) < B_NO_ERROR )
	{
		perror( "Semaphore::Wait: couldn't acquire sem" );
	}
#if SEM_DEBUG
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Wait:id(%d):count(%d) done\n", mutex, tc ));
#endif
}

bool Semaphore::Wait( long timeout )
{
	status_t	stat;

	if ( timeout == WAIT_FOREVER )
	{
		stat = acquire_sem( mutex );
	}
	else
	{
		stat = acquire_sem_etc( mutex, 1, B_TIMEOUT, timeout * 1e6 );
	}

	return( stat == B_NO_ERROR );
}

void Semaphore::Signal()
{
#if SEM_DEBUG
	int32	tc;
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Signal:id(%d):count(%d)\n", mutex, tc ));
#endif
	if ( release_sem( mutex ) < B_NO_ERROR )
	{
		perror( "Semaphore::Signal: couldn't release sem" );
	}
#if SEM_DEBUG
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Signal:id(%d):count(%d) done\n", mutex, tc ));
#endif
}
