/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1998-1999 EMusic.com

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

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

#define WAIT_FOREVER -1

class Mutex {

public:
         Mutex(bool createOwned = false);
        ~Mutex();

#ifdef DEBUG_MUTEXES
    bool __Acquire(char *filename, int line, long timeout = WAIT_FOREVER);
#define Acquire() __Acquire(__FILE__, __LINE__)
    void __Release(char *filename, int line);
#define Release() __Release(__FILE__, __LINE__)
#else
    bool Acquire(long timeout = WAIT_FOREVER);
    void Release();
#endif
    void DumpMutex(void);

 private:
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_tCond;
    int             m_iBusy;
    pthread_t       m_tOwner;
};

#endif /* MUTEX_H */
