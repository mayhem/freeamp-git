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

#ifndef _BEOS_THREAD_H
#define _BEOS_THREAD_H

#include <kernel/OS.h>
#include "thread.h"

#include "mutex.h"

class beosThread : public Thread{

public:
						beosThread();
						~beosThread();

	virtual bool		Create(thread_function function, void* arg);
	virtual void		Destroy();
	virtual void		Suspend();
	virtual void		Resume();
	virtual void		Join();
	virtual Priority	GetPriority() const;
	virtual Priority	SetPriority(Priority priority);

	static int32		internalThreadFunction(void *);
	void *				InternalThreadFunction();

private:
	Priority			m_priority;
//	pthread_t			m_threadHandle;
	thread_id			m_threadHandle;
	unsigned			m_threadId;
	bool				m_suspended;
	Mutex *				m_suspendMutex;
	thread_function		m_function;
	void *				m_arg;
};

// Utility functions.

inline
int32
beos_priority( Priority freeamp_prio )
{
	switch ( freeamp_prio ) {
	case Idle:
		return B_NORMAL_PRIORITY;
		break;
	case Low:
		return B_NORMAL_PRIORITY;
		break;
	case BelowNormal:
		return B_NORMAL_PRIORITY;
		break;
	case Normal:
		return B_NORMAL_PRIORITY;
		break;
	case AboveNormal:
		return B_NORMAL_PRIORITY;
		break;
	case High:
		return B_NORMAL_PRIORITY;
		break;
	case Critical:
		return B_NORMAL_PRIORITY;
		break;
	default:
		return B_NORMAL_PRIORITY;
		break;
	}
}

#endif /* _BEOS_THREAD_H */
