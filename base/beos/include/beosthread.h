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

#ifndef INCLUDED_BEOS_THREAD_H
#define INCLUDED_BEOS_THREAD_H

#include <kernel/OS.h>
#include <support/Locker.h>
#include "thread.h"

#include "mutex.h"

class beosThread : public Thread
{
public:
						beosThread();
	virtual				~beosThread();

	virtual bool		Create(thread_function function, void* arg);
	virtual void		Destroy();
	virtual void		Suspend();
	virtual void		Resume();
	virtual void		Join();
	virtual uint32      GetPriority( void ) const;
	virtual uint32      SetPriority( uint32 priority );
	virtual void		DumpThreadInfo( void ) const;

	static int32		internalThreadFunction(void *);
	void *				InternalThreadFunction();

protected:
	bool				Lock( void );
	void				Unlock( void );

private:
	uint32              m_priority;
	thread_id			m_threadHandle;
	unsigned			m_threadId;
	bool				m_suspended;
	BLocker				m_lock;
	thread_function		m_function;
	void *				m_arg;
};

// Inline functions

inline bool
beosThread::Lock( void )
{
	return m_lock.Lock();
}

inline void
beosThread::Unlock( void )
{
	Unlock();
}

// Utility functions.

inline
int32
beos_priority( uint32 freeamp_prio )
{
#if 0
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
#endif
	return B_NORMAL_PRIORITY;
}

#endif /* _BEOS_THREAD_H */
