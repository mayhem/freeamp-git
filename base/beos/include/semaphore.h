
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



#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <kernel/OS.h>
#include "mutex.h"

class Semaphore
{
public:
			Semaphore( int cnt = 1 );
			Semaphore( const char* name, int cnt = 1);
			~Semaphore();
    void	Wait();
    bool	Wait( long timeout );
    void	Signal();
	sem_id	ID( void ) const { return mutex; }

protected:

private:
	int		count;
	sem_id	mutex;
};

#endif // _SEMAPHORE_H_
