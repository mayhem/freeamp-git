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

// vector.h

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include "config.h"



template<class T>
class Vector {
 private:
    T *pObjs;  // pointer to array of objects
    int32 threshhold;
    int32 insertionPoint;
    int32 currentLength;
 public:
    Vector(int32 th = 10); // set threshhold to its arg
    ~Vector();
    int32 RemoveAll();
    int32 DeleteAll();
    T elementAt(int32);
    T removeElementAt(int32);
    int32 deleteElementAt(int32);
    int32 insert(T &);
    int32 numElements();
};




template<class T> Vector<T>::Vector(int32 th) {
    threshhold = th;
    pObjs = new T[threshhold];
    for (int32 i=0;i<threshhold;i++) pObjs[i] = NULL;
    insertionPoint = 0;
    currentLength = threshhold;
}

template<class T> Vector<T>::~Vector() {
    delete pObjs;
}

template<class T> int32 Vector<T>::numElements() {
    return insertionPoint;
}

template<class T> int32 Vector<T>::RemoveAll() {
    for (int32 i=0;i<insertionPoint;i++) pObjs[i] = NULL;
    insertionPoint = 0;
    return 0;
}

template<class T> int32 Vector<T>::DeleteAll() {
    for(int32 i=0;i<insertionPoint;i++) {
	delete pObjs[i];
	pObjs[i] = NULL;
    }
    insertionPoint = 0;
    return 0;
}

template<class T> T Vector<T>::elementAt(int32 e) {
    if ((e >= insertionPoint) || (e < 0)) {
	return NULL;
    }
    return pObjs[e];
}

template<class T> T Vector<T>::removeElementAt(int32 e) {
    if ((e >= insertionPoint) || (e < 0)) {
	return NULL;
    }
    T rtnval = pObjs[e];
    memmove(&(pObjs[e]),&(pObjs[e+1]),sizeof(T)*(insertionPoint-e-1));
    //for (int32 i = e;i<insertionPoint;i++) {
    //   pObjs[i] = pObjs[i+1];
    //}
    insertionPoint--;
    return rtnval;
}

template<class T> int32 Vector<T>::deleteElementAt(int32 e) {
    T* p = removeElementAt(e);
    if (p) {
	delete p;
	return 0;
    } else {
	return 255;
    }
}

template<class T> int32 Vector<T>::insert(T &pE) {
//    cout << "v:inserting" << endl;
    if (pE) {
	if (insertionPoint == currentLength) {
	    //cout << "v:adding more" << endl;
	    // add more and copy over
	    T *pNewObjs = new T[currentLength+threshhold];
	    //cout << "v:got it " << currentLength+threshhold << endl;
	    memcpy(pNewObjs,pObjs,(sizeof (T))*currentLength);
	    //cout << "v:did copy" << endl;
	    delete pObjs;
	    pObjs = pNewObjs;
	    for (int32 i=currentLength;i<currentLength + threshhold;i++) pObjs[i] = NULL;
	    currentLength = currentLength + threshhold;
	}
	pObjs[insertionPoint] = pE;
	insertionPoint++;
	//cout << "v:did it" << endl;
	return insertionPoint-1;
    } else {
	return -1;
    }
}


#endif // _VECTOR_H_
