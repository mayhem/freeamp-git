
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

// queue.h

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "config.h"
#include "mutex.h"

template<class T>
class Queue {

 public:
    Queue(bool dwd = true);
    ~Queue();
    
    T read();
    int32 write(T &);
    bool isEmpty();

 private:
    class QueueMember {
        public:
	    QueueMember *m_behindMe;
	    T m_myMember;
    };
    QueueMember *m_head;
    QueueMember *m_tail;
    
    bool m_deleteWhenDone;
    void getLock();
    void releaseLock();
    Mutex *m_myLock;
 
};


template<class T> Queue<T>::Queue(bool dwd) {
    m_deleteWhenDone = dwd;
    m_head = NULL;
    m_tail = NULL;
    m_myLock = new Mutex();
}

template<class T> Queue<T>::~Queue() {
    getLock();
    QueueMember *pQM = m_head;
    QueueMember *next = NULL;
    m_head = NULL;
    m_tail = NULL;
    releaseLock();
    while (pQM != NULL) {
	next = pQM->m_behindMe;
	if (m_deleteWhenDone) {
	    if (pQM->m_myMember) {
		delete pQM->m_myMember;
	    }
	}
	delete pQM;
	pQM = next;
    }
    if (m_myLock) {
	delete m_myLock;
    }
}

template<class T> T Queue<T>::read() {
    getLock();
    //cout << "queue:: read" << endl;
    T rtnVal = NULL;
    if (m_head) {
	//cout << "queue:: read got m_head" << endl;
	QueueMember *tmp = m_head;
	rtnVal = tmp->m_myMember;
	m_head = tmp->m_behindMe;
	delete tmp;
    }
    if (m_head == NULL) {
	// no m_tail anymore
	m_tail = NULL;
    }
    //cout << "queue:: read ending" << endl;
    releaseLock();
    return rtnVal;
}

template<class T> int32 Queue<T>::write(T &c) {
    getLock();
    QueueMember *pQM = new QueueMember();
    pQM->m_myMember = c;
    pQM->m_behindMe = NULL;
    if (m_tail) {
	m_tail->m_behindMe = pQM;
    }
    m_tail = pQM;
    if (m_head == NULL) {
	// no m_head, this T we just pushed on is now the m_head
	m_head = pQM;
    }
    releaseLock();
    return 0;
}

template<class T> bool Queue<T>::isEmpty() {
    return (m_head ? false : true);
}

template<class T> void Queue<T>::getLock() {
    m_myLock->Acquire(WAIT_FOREVER);
    return;
}

template<class T> void Queue<T>::releaseLock() {
    m_myLock->Release();
    return;
}


#endif //_QUEUE_H_




