
/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 EMusic.com

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

#include <pthread.h>
#include <iostream.h>

#include "semaphore.h"


Semaphore::Semaphore(int cnt) {
    count = cnt;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
}

Semaphore::~Semaphore() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

void Semaphore::Wait() {
    //decrement the semaphore
    pthread_mutex_lock(&mutex);
    count--;
    while (count <=0) {
	pthread_cond_wait(&cond,&mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void Semaphore::Signal() {
    // increment the semaphore
    pthread_mutex_lock(&mutex);
    count++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
}
