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
#include "thread.h"

#ifdef __LINUX__
    #include "linuxthread.h"
#elif WIN32
    #include "win32thread.h"
#else
    #error thread class needs to be defined for this platform
#endif




Thread* Thread::CreateThread()
{
    Thread* thread = NULL;
#ifdef __linux__
    thread = new linuxThread();
#elif WIN32
    thread = new win32Thread();
#else
    #error thread class needs to be defined for this platform
#endif

    return thread;
}
