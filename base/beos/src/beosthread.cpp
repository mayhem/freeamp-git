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

//#include <pthread.h>
#include <signal.h>
#include <iostream.h>

#include "beosthread.h"
#include "mutex.h"
#include <support/Debug.h>

void
beosThread::Join()
{
	status_t	thread_return_value;

//	pthread_join(m_threadHandle,NULL);
	wait_for_thread( m_threadHandle, &thread_return_value );
}

beosThread::
beosThread():
Thread()
{
    m_priority		= Normal;
    m_threadHandle	= (thread_id) NULL;
    m_threadId		= 0;
    m_suspended		= false;
    m_suspendMutex	= new Mutex();
}

beosThread::
~beosThread()
{
	//pthread_cancel(m_threadHandle);
	if ( m_suspendMutex ) {
		delete m_suspendMutex;
		m_suspendMutex = NULL;
	}
}

int32
beosThread::
internalThreadFunction( void* arg )
{
    beosThread* thread = (beosThread*) arg;
    thread->InternalThreadFunction();
    return B_NO_ERROR; 
}

void *
beosThread::
InternalThreadFunction()
{
	if ( !m_function ) {
//		cout << "screwwwwwwwwwwwed..." << endl;
		return 0;
	} else {
//		cout << "fine..." << endl;
	}
	m_function( m_arg );

	return (void *)0;
}

bool 
beosThread::
Create( thread_function function, void* arg )
{
#if 0
//    cout << "Thread: Create" << endl;
    bool result = true;
    m_function = function;
    m_arg = arg;
    if(pthread_create(&m_threadHandle, NULL,beosThread::internalThreadFunction, this))
    {
//	cout << "Create failed!!" << endl;
	result = false;
    }
//    cout << "Thread: Create: done" << endl;
    return result;
#endif
	bool	result = true;
	m_function = function;
	m_arg = arg;
	m_threadHandle = spawn_thread(
						beosThread::internalThreadFunction,
						"FreeAmp",
						beos_priority( m_priority ),
						this
						);
	if ( m_threadHandle < B_NO_ERROR ) {
		PRINT(( "Thread creation failed\n" ));
		result = false;
	}

	PRINT(( "Thread successfully created\n" ));

	if ( resume_thread( m_threadHandle ) < B_NO_ERROR ) {
		PRINT(( "couldn't start(resume) thread\n" ));
		result = false;
	}

	PRINT(( "Thread started successfully\n" ));

	return result;
}

void 
beosThread::
Destroy()
{
//	pthread_cancel(m_threadHandle);
	kill_thread( m_threadHandle );
}

void 
beosThread::
Suspend()
{
    m_suspendMutex->Acquire( WAIT_FOREVER );
    if ( !m_suspended ) {
//		pthread_kill(m_threadHandle, SIGSTOP);
		suspend_thread( m_threadHandle );
		m_suspended = true;
    }
    m_suspendMutex->Release();
}

void 
beosThread::
Resume()
{
    m_suspendMutex->Acquire(WAIT_FOREVER);
    if (m_suspended) {
//		pthread_kill(m_threadHandle, SIGCONT);
		resume_thread( m_threadHandle );
		m_suspended = false;
    }
    m_suspendMutex->Release();
}


Priority 
beosThread::
GetPriority() const
{
    return((Priority) 0);
}

Priority 
beosThread::
SetPriority(Priority priority)
{
    return((Priority) 0);
}

