
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

#include <pthread.h>
#include <signal.h>
#include <iostream.h>

#include "thread.h"
#include "mutex.h"

void Thread::SetDieImmediately() {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
}
void Thread::ClearDieImmediately() {
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
}
void Thread::Join() {
    pthread_join(m_threadHandle,NULL);
}

Thread::
Thread()
{
    m_priority		= Normal;
    m_threadHandle	= (pthread_t) NULL;
    m_threadId		= 0;
    m_suspended         = false;
    m_suspendMutex      = new Mutex();
}

Thread::
~Thread()
{
    //pthread_cancel(m_threadHandle);
    if (m_suspendMutex) {
	delete m_suspendMutex;
	m_suspendMutex = NULL;
    }
}


bool 
Thread::
Create(thread_function function, void* arg)
{
//    cout << "Thread: Create" << endl;
    bool result = true;

    if(pthread_create(&m_threadHandle, NULL, function, arg))
    {
//	cout << "Create failed!!" << endl;
	result = false;
    }
//    cout << "Thread: Create: done" << endl;
    return result;
}

void 
Thread::
Destroy()
{
    pthread_cancel(m_threadHandle);
}

void 
Thread::
Suspend()
{
    m_suspendMutex->Acquire(WAIT_FOREVER);
    if (!m_suspended) {
	pthread_kill(m_threadHandle, SIGSTOP);
	m_suspended = true;
    }
    m_suspendMutex->Release();
}

void 
Thread::
Resume()
{
    m_suspendMutex->Acquire(WAIT_FOREVER);
    if (m_suspended) {
	pthread_kill(m_threadHandle, SIGCONT);
	m_suspended = false;
    }
    m_suspendMutex->Release();
}


Priority 
Thread::
GetPriority() const
{
    return((Priority) 0);
}

Priority 
Thread::
SetPriority(Priority priority)
{
    return((Priority) 0);
}

