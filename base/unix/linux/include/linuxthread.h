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

#ifndef _LINUX_THREAD_H
#define _LINUX_THREAD_H

#include <pthread.h>
#include "thread.h"

#include "mutex.h"


class linuxThread : public Thread{

public:
	linuxThread();
	~linuxThread();


	virtual bool Create(thread_function function, void* arg);
	virtual void Destroy();
	virtual void Suspend();
	virtual void Resume();
	virtual void Join();
	virtual static void SetDieImmediately(); // for pthreads, must set PTHREAD_CANCEL_ASYNCHRONOUS for proper operation
	virtual static void ClearDieImmediately();
	virtual Priority GetPriority() const;
	virtual Priority SetPriority(Priority priority);


private:
	Priority	m_priority;
	pthread_t   m_threadHandle;
	unsigned	m_threadId;
	bool        m_suspended;
	Mutex *     m_suspendMutex;
};

#endif /* _LINUX_THREAD_H */




