
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

#if HAVE_BASELINUXTHREADS
#if HAVE_ENHLINUXTHREADS
#define USE_NATIVE
#define LOCAL_MUTEX_TYPE PTHREAD_MUTEX_ERRORCHECK_NP
#else
#define USE_HACK
#define LOCAL_MUTEX_TYPE PTHREAD_MUTEX_FAST_NP
#endif
#elif HAVE_MITPTHREADS
#define LOCAL_MUTEX_TYPE MUTEX_TYPE_DEBUG
#endif

#include "mutex.h"

Mutex::Mutex(bool createOwned)
{

   pthread_mutexattr_t attr;

   myTid = 0;

   pthread_mutexattr_init(&attr);
   pthread_mutexattr_setkind_np(&attr, LOCAL_MUTEX_TYPE);

   memset(&m_mutex, '\0', sizeof(m_mutex));
#if HAVE_LINUXTHREADS
   m_mutex.m_kind = LOCAL_MUTEX_TYPE;
#elif HAVE_MITPTHREADS
   m_mutex.m_type = MUTEX_TYPE_DEBUG;
#endif
   if (!pthread_mutex_init(&m_mutex, &attr))
   {
      if (createOwned)
      {
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

   if (timeout == WAIT_FOREVER)
   {
#ifdef USE_HACK
      if (!pthread_equal(myTid, pthread_self()))
      {                         // _SHOULD_ simulate an ERRORCHECK mutex
#endif
         if (!pthread_mutex_lock(&m_mutex))
         {
#ifdef USE_HACK
            myTid = pthread_self();
#endif
            result = true;
         }
#ifdef USE_HACK
      }
      else
      {
         result = true;
      }
#endif

   }
   else
   {
      if (pthread_mutex_trylock(&m_mutex))
      {
         usleep(timeout);
#ifdef USE_HACK
         if (!pthread_equal(myTid, pthread_self()))
         {
#endif
            if (!pthread_mutex_trylock(&m_mutex))
            {
#ifdef USE_HACK
               myTid = pthread_self();
#endif
               result = true;
            }
#ifdef USE_HACK
         }
         else
         {
            result = true;
         }
#endif
      }
      else
      {
         result = true;
      }
   }

   return (result);
}

void
          Mutex::
Release()
{
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
