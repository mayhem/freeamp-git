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
#include "timer.h"

#ifdef __linux__
#define GoToSleep(x) usleep(x*1000)
#elif defined(WIN32)
#include <windows.h>
#define GoToSleep(x) Sleep(x)
#else
    #error GoToSleep needs to be defined for this platform
#endif


Timer::Timer(uint32 milliseconds):
m_ticks(milliseconds), m_thread(NULL), m_alive(true)
{
     
}

Timer::~Timer()
{
    m_alive = false;

    if(m_thread)
    {
        m_thread->Resume();
        m_semaphore.Wait();
    }
}

void Timer::Set(uint32 milliseconds)
{
    m_ticks = milliseconds;
}

void Timer::Start()
{
    if(m_thread)
        m_thread->Resume();
    else
    {
        m_thread = Thread::CreateThread();
        m_thread->Create(Timer::thread_function, this);
    }
}

void Timer::Stop()
{
    if(m_thread)
        m_thread->Suspend();
}

void Timer::ThreadFunction()
{
    do
    {
        Tick();

        GoToSleep(m_ticks);

    }while(m_alive);  

    m_semaphore.Signal();
}

void Timer::thread_function(void* arg)
{
    Timer* _this = (Timer*)arg;

    _this->ThreadFunction();
}
