
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

#include <string.h>
#include <pthread.h>
#include <iostream.h>
#include <unistd.h>
#include <stdio.h>

#include "mutex.h"

Mutex::Mutex(bool createOwned)
{

   pthread_mutexattr_t attr;

   myTid = 0;

   pthread_mutexattr_init(&attr);

// If the mutexen become sane, we can do the following
// pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_PRIVATE);
// pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

#ifdef SOLDEBUGMUTEX
   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
   // This is really, really, really helpful.
   // There's all sorts of multiple locking that shouldn't be going on,
   // and quite frankly, I don't know if freeamp will work at all with
   // "normal" non-debugging/recursive mutexen.
#endif

   if (!pthread_mutex_init(&m_mutex, &attr))
   {
//	printf("created mutex %p\n", &m_mutex);
      if (createOwned)
      {
//	printf("with createOwned!\n");
         pthread_mutex_lock(&m_mutex);
#ifdef USE_HACK
         myTid = pthread_self();
#endif
      }
      else
      {
         pthread_mutex_unlock(&m_mutex);
      }
   }
   pthread_mutexattr_destroy(&attr);
}

Mutex::
~Mutex()
{

   pthread_mutex_destroy(&m_mutex);
}

bool
Mutex::
Acquire(long timeout)
{
   bool      result = false;
   void *zot;

   zot = (void *) &m_mutex;
   //   printf("woii! Acquire %p!\n", zot);
 
   while (pthread_mutex_trylock(&m_mutex)) {
     //     printf("woii! %p sleeping!\n",&m_mutex);
     usleep(999999);
   }
   result = true;
   
   return (result);
}

void
          Mutex::
Release()
{
//printf("Woi! Release %p!\n", (void *) &m_mutex);
   pthread_mutex_unlock(&m_mutex);
#ifdef USE_HACK
   myTid = 0;
#endif
}

void      Mutex::
DumpMutex(void)
{
#if HAVE_LINUXTHREADS
   cerr << "pMutex = " << this << "\n";
   cerr << "Spinlock = " << m_mutex.m_spinlock << "\n";
   cerr << "Count = " << m_mutex.m_count << "\n";
   cerr << "Kind = " << m_mutex.m_owner << "\n";
   cerr << "Waiting = " << m_mutex.m_waiting.head << "\n";
#endif

}
