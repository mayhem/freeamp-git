
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

#include <pthread.h>

#include "mutex.h"

#define THREAD_RETURN void *
#define THREAD_LINKAGE

typedef THREAD_RETURN  ( THREAD_LINKAGE *thread_function)( void * arg);

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
	Thread();
	~Thread();


	bool Create(thread_function function, void* arg);
	void Destroy();
	void Suspend();
	void Resume();
	void Join();
	static void SetDieImmediately(); // for pthreads, must set PTHREAD_CANCEL_ASYNCHRONOUS for proper operation
	static void ClearDieImmediately();
	Priority GetPriority() const;
	Priority SetPriority(Priority priority);


private:
	Priority	m_priority;
	pthread_t       m_threadHandle;
	unsigned	m_threadId;
	bool            m_suspended;
	Mutex *         m_suspendMutex;
};

#endif /* THREAD_H */




