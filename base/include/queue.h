
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
 private:
    class QueueMember {
    public:
	QueueMember *behindMe;
	T myMember;
    };
    QueueMember *head;
    QueueMember *tail;
    
    bool deleteWhenDone;
    void getLock();
    void releaseLock();
    Mutex *myLock;
 public:
    Queue(bool dwd = true);
    ~Queue();
    
    T read();
    int32 write(T &);
    bool isEmpty();
};


template<class T> Queue<T>::Queue(bool dwd) {
    deleteWhenDone = dwd;
    head = NULL;
    tail = NULL;
    myLock = new Mutex();
}

template<class T> Queue<T>::~Queue() {
    getLock();
    QueueMember *pQM = head;
    QueueMember *next = NULL;
    head = NULL;
    tail = NULL;
    releaseLock();
    while (pQM != NULL) {
	next = pQM->behindMe;
	if (deleteWhenDone) {
	    if (pQM->myMember) {
		delete pQM->myMember;
	    }
	}
	delete pQM;
	pQM = next;
    }
    if (myLock) {
	delete myLock;
    }
}

template<class T> T Queue<T>::read() {
    getLock();
    //cout << "queue:: read" << endl;
    T rtnVal = NULL;
    if (head) {
	//cout << "queue:: read got head" << endl;
	QueueMember *tmp = head;
	rtnVal = tmp->myMember;
	head = tmp->behindMe;
	delete tmp;
    }
    if (head == NULL) {
	// no tail anymore
	tail = NULL;
    }
    //cout << "queue:: read ending" << endl;
    releaseLock();
    return rtnVal;
}

template<class T> int32 Queue<T>::write(T &c) {
    getLock();
    QueueMember *pQM = new QueueMember();
    pQM->myMember = c;
    pQM->behindMe = NULL;
    if (tail) {
	tail->behindMe = pQM;
    }
    tail = pQM;
    if (head == NULL) {
	// no head, this T we just pushed on is now the head
	head = pQM;
    }
    releaseLock();
    return 0;
}

template<class T> bool Queue<T>::isEmpty() {
    return (head ? false : true);
}

template<class T> void Queue<T>::getLock() {
    myLock->Acquire(WAIT_FOREVER);
    return;
}

template<class T> void Queue<T>::releaseLock() {
    myLock->Release();
    return;
}


#endif //_QUEUE_H_




