/*____________________________________________________________________________
	
	FreeAMP - The Free MP3 Player
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

#ifndef INCLUDED_TIMER_H_
#define INCLUDED_TIMER_H_

#include "thread.h"
#include "semaphore.h"

class Timer {
 public:
    Timer(uint32 milliseconds);
    virtual ~Timer();

    void Set(uint32 milliseconds);
    void Start();
    void Stop();

    virtual void Tick() = 0;

    static void thread_function(void* arg);
    void ThreadFunction();

 private:
    uint32 m_ticks;
    Thread* m_thread;
    bool m_alive;
    Semaphore m_semaphore;

};

#endif // INCLUDED_TIMER_H_
