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

#include <string.h>
#include <pthread.h>
#include <iostream.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include "mutex.h"

Mutex::Mutex(bool createOwned)
{
    pthread_mutexattr_t attr;

    m_iBusy = 0;
    m_tOwner = 0;
    pthread_cond_init(&m_tCond, NULL);

    pthread_mutexattr_init(&attr);
    memset(&m_mutex, '\0', sizeof(m_mutex));
    if (!pthread_mutex_init(&m_mutex, &attr))
        if (createOwned)
            pthread_mutex_lock(&m_mutex);
    pthread_mutexattr_destroy(&attr);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_tCond);
}

#ifdef DEBUG_MUTEXES
bool Mutex::__Acquire(char *filename, int line, long timeout)
#else
bool Mutex::Acquire(long timeout)
#endif
{
#ifdef DEBUG_MUTEXES
    cerr << filename << ":" << line << " trying to acquire mutex " << this 
         << endl;
#endif
    pthread_mutex_lock(&m_mutex);
    if (m_iBusy != 0) {
        if (m_tOwner != pthread_self()) {
            do {
                if (timeout == WAIT_FOREVER)
                    pthread_cond_wait(&m_tCond, &m_mutex);
                else {
                    struct timespec sTime;
                    struct timeval  sNow;

                    gettimeofday(&sNow, NULL);
                    sTime.tv_sec = sNow.tv_sec;
                    sTime.tv_nsec = sNow.tv_usec + timeout;
                    if (sTime.tv_nsec >= 1000000) {
                        sTime.tv_nsec -= 1000000;
                        sTime.tv_sec++;
                    }
                    sTime.tv_nsec *= 1000;

                    if (pthread_cond_timedwait(&m_tCond, &m_mutex, &sTime) == 
                        ETIMEDOUT) {
                        pthread_mutex_unlock(&m_mutex);
                        return false;
                    }
                }
            } while(m_iBusy != 0);
        }
    }
    m_tOwner = pthread_self();
    m_iBusy++;
    pthread_mutex_unlock(&m_mutex);

#ifdef DEBUG_MUTEXES
    cerr << filename << ":" << line << " sucessfully acquired mutex " << this
         << endl;
#endif
    return true;
}

#ifdef DEBUG_MUTEXES
void Mutex::__Release(char *filename, int line)
#else
void Mutex::Release()
#endif
{
    int iOwnerShip;

#ifdef DEBUG_MUTEXES
    cerr << filename << ":" << line << " releasing mutex " << this << endl;
#endif
    pthread_mutex_lock(&m_mutex);
    iOwnerShip = --m_iBusy;
    pthread_mutex_unlock(&m_mutex);

    if (iOwnerShip == 0)
        pthread_cond_signal(&m_tCond);
#ifdef DEBUG_MUTEXES
    cerr << filename << ":" << line << " released mutex " << this << endl;
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
