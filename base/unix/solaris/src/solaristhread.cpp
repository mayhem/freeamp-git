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

#include <pthread.h>
#include <signal.h>
#include <iostream.h>

#include "solaristhread.h"
#include "mutex.h"

void solarisThread::Join() {
    pthread_join(m_threadHandle,NULL);
}

solarisThread::
solarisThread():
Thread()
{
    m_priority		= 0;
    m_threadHandle	= (pthread_t) NULL;
    m_threadId		= 0;
    m_suspended         = false;
    m_suspendMutex      = new Mutex();
}

solarisThread::
~solarisThread()
{
    //pthread_cancel(m_threadHandle);
    if (m_suspendMutex) {
	delete m_suspendMutex;
	m_suspendMutex = NULL;
    }
}

void *
solarisThread::
internalThreadFunction(void* arg)
{
    solarisThread* thread = (solarisThread*) arg;
    thread->InternalThreadFunction();
    return NULL;
}

void *
solarisThread::
InternalThreadFunction()
{
    if (!m_function) {
//	cout << "screwwwwwwwwwwwed..." << endl;
	return 0;
    } else {
//	cout << "fine..." << endl;
    }
    m_function(m_arg);

    return 0;
}

bool 
solarisThread::
Create(thread_function function, void* arg)
{
//    cout << "Thread: Create" << endl;
    bool result = true;
    m_function = function;
    m_arg = arg;
    if(pthread_create(&m_threadHandle, NULL,solarisThread::internalThreadFunction, this))
    {
//	cout << "Create failed!!" << endl;
	result = false;
    }
//    cout << "Thread: Create: done" << endl;
    return result;
}

void 
solarisThread::
Destroy()
{
    pthread_cancel(m_threadHandle);
}

void 
solarisThread::
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
solarisThread::
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
solarisThread::
GetPriority() const
{
    return((Priority) 0);
}

Priority 
solarisThread::
SetPriority(Priority priority)
{
    return((Priority) 0);
}

