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

#define DEBUG 0
#include <be/kernel/OS.h>
#include <be/support/Debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	status_t	err;
#if SEM_DEBUG
	int32		tc;
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Wait:id(%d):count(%d)\n", mutex, tc ));
#endif
	if ( ( err = acquire_sem( mutex ) ) < B_NO_ERROR )
	{
		puts( "Sem::Wait:couldn't acquire sem" );
		DEBUGGER(( "Semaphore::Wait: couldn't acquire sem : %s\n",
					strerror( err ) ));
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
	status_t	err;
#if SEM_DEBUG
	int32		tc;
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Signal:id(%d):count(%d)\n", mutex, tc ));
#endif
	if ( ( err = release_sem( mutex ) ) < B_NO_ERROR )
	{
		puts( "Sem::Signal:couldn't acquire sem" );
		DEBUGGER(( "Semaphore::Signal: couldn't release sem : %s\n",
					strerror( err ) ));
	}
#if SEM_DEBUG
	get_sem_count( mutex, &tc );
	PRINT(( "Semaphore::Signal:id(%d):count(%d) done\n", mutex, tc ));
#endif
}
