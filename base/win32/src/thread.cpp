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

#include "thread.h"

Thread::
Thread()
{
	m_priority		= Normal;
	m_threadHandle	= NULL;	
	m_threadId		= 0;
}

Thread::
~Thread()
{
	CloseHandle(m_threadHandle);
}



bool 
Thread::
Create(thread_function function, void* arg)
{
	bool result = false;

	m_threadHandle = (HANDLE) _beginthreadex(
									NULL,
									0,
									function,
									arg,
									0,
									&m_threadId);
	if(m_threadHandle)
	{
		result = true;
	}
	
	return result;
}

void 
Thread::
Destroy()
{
	 TerminateThread( m_threadHandle, 0 );

	 //CloseHandle(m_threadHandle);
}

void 
Thread::
Suspend()
{
	 SuspendThread(m_threadHandle);
}

void 
Thread::
Resume()
{
	ResumeThread(m_threadHandle);
}

void
Thread::
Join()
{
	WaitForSingleObject(m_threadHandle, INFINITE);
}

Priority 
Thread::
GetPriority() const
{
	return (Priority) GetThreadPriority(m_threadHandle);
}

Priority 
Thread::
SetPriority(Priority priority)
{
	Priority old = (Priority) GetThreadPriority(m_threadHandle);

	SetThreadPriority(m_threadHandle, priority);

	return old;
}

