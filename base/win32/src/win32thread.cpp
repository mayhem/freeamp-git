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

#include <process.h>

#include "win32thread.h"


win32Thread::
win32Thread():
Thread()
{
	m_priority		= Normal;
	m_threadHandle	= NULL;	
	m_threadId		= 0;
    m_function      = NULL;
    m_arg           = NULL;
}

win32Thread::
~win32Thread()
{
    if(m_threadHandle)
    {
	    CloseHandle(m_threadHandle);
    }
}

unsigned long __stdcall 
win32Thread::
internalThreadFunction(void* arg)
{
    win32Thread* thread = (win32Thread*) arg;

    return thread->InternalThreadFunction();
}

uint32 
win32Thread::
InternalThreadFunction()
{
    m_function(m_arg);

    return 0;
}

bool 
win32Thread::
Create(thread_function function, void* arg)
{
	bool result = false;

    m_function      = function;
    m_arg           = arg;

	//m_threadHandle = (HANDLE) _beginthreadex(
	/*LEAK*/m_threadHandle = ::CreateThread(
									NULL,
									0,
									internalThreadFunction,
									this,
									0,
									&m_threadId);
	if(m_threadHandle)
	{
		result = true;
	}
	
	return result;
}

void 
win32Thread::
Destroy()
{
	 TerminateThread(m_threadHandle, 0);
}

void 
win32Thread::
Suspend()
{
	 SuspendThread(m_threadHandle);
}

void 
win32Thread::
Resume()
{
	ResumeThread(m_threadHandle);
}

void
win32Thread::
Join()
{
	WaitForSingleObject(m_threadHandle, INFINITE);
}

Priority 
win32Thread::
GetPriority() const
{
	return (Priority) GetThreadPriority(m_threadHandle);
}

Priority 
win32Thread::
SetPriority(Priority priority)
{
	Priority old = (Priority) GetThreadPriority(m_threadHandle);

	SetThreadPriority(m_threadHandle, priority);

	return old;
}

