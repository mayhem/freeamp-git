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
 public:
    Vector(int32 th = 10); // set m_threshhold to its arg
    ~Vector();
    int32 RemoveAll();
    int32 DeleteAll();
    T ElementAt(int32);
    T RemoveElementAt(int32);
    T RandomElement();
    void Swap(int32,int32);
    int32 DeleteElementAt(int32);
    int32 Insert(T &);
    int32 NumElements();
 
 private:
    T *m_pObjs;  // pointer to array of objects
    int32 m_threshhold;
    int32 m_insertionPoint;
    int32 m_currentLength;
 
};


template<class T> T Vector<T>::RandomElement() {
    srand((unsigned int) time (NULL));
    int32 foo = (int32) (((double)m_insertionPoint * rand()) / (RAND_MAX+1.0));
    return m_pObjs[foo];
}

template<class T> void Vector<T>::Swap(int32 s1, int32 s2) {
    if ((s1 < 0) ||
	(s2 < 0) ||
	(s1 >= m_insertionPoint) ||
	(s2 >= m_insertionPoint) ||
	(s1 == s2)) {
	return;
    }
    T tmp = m_pObjs[s1];
    m_pObjs[s1] = m_pObjs[s2];
    m_pObjs[s2] = tmp;
}

template<class T> Vector<T>::Vector(int32 th) {
    m_threshhold = th;
    m_pObjs = new T[m_threshhold];
    for (int32 i=0;i<m_threshhold;i++) m_pObjs[i] = NULL;
    m_insertionPoint = 0;
    m_currentLength = m_threshhold;
}

template<class T> Vector<T>::~Vector() {
    delete m_pObjs;
}

template<class T> int32 Vector<T>::NumElements() {
    return m_insertionPoint;
}

template<class T> int32 Vector<T>::RemoveAll() {
    for (int32 i=0;i<m_insertionPoint;i++) m_pObjs[i] = NULL;
    m_insertionPoint = 0;
    return 0;
}

template<class T> int32 Vector<T>::DeleteAll() {
    for(int32 i=0;i<m_insertionPoint;i++) {
	delete m_pObjs[i];
	m_pObjs[i] = NULL;
    }
    m_insertionPoint = 0;
    return 0;
}

template<class T> T Vector<T>::ElementAt(int32 e) {
    if ((e >= m_insertionPoint) || (e < 0)) {
	return NULL;
    }
    return m_pObjs[e];
}

template<class T> T Vector<T>::RemoveElementAt(int32 e) {
    if ((e >= m_insertionPoint) || (e < 0)) {
	return NULL;
    }
    T rtnval = m_pObjs[e];
    memmove(&(m_pObjs[e]),&(m_pObjs[e+1]),sizeof(T)*(m_insertionPoint-e-1));
    //for (int32 i = e;i<m_insertionPoint;i++) {
    //   m_pObjs[i] = m_pObjs[i+1];
    //}
    m_insertionPoint--;
    return rtnval;
}

template<class T> int32 Vector<T>::DeleteElementAt(int32 e) {
    T* p = removeElementAt(e);
    if (p) {
	delete p;
	return 0;
    } else {
	return 255;
    }
}

template<class T> int32 Vector<T>::Insert(T &pE) {
//    cout << "v:inserting" << endl;
    if (pE) {
	if (m_insertionPoint == m_currentLength) {
	    //cout << "v:adding more" << endl;
	    // add more and copy over
	    T *pNewObjs = new T[m_currentLength+m_threshhold];
	    //cout << "v:got it " << m_currentLength+m_threshhold << endl;
	    memcpy(pNewObjs,m_pObjs,(sizeof (T))*m_currentLength);
	    //cout << "v:did copy" << endl;
	    delete m_pObjs;
	    m_pObjs = pNewObjs;
	    for (int32 i=m_currentLength;i<m_currentLength + m_threshhold;i++) m_pObjs[i] = NULL;
	    m_currentLength = m_currentLength + m_threshhold;
	}
	m_pObjs[m_insertionPoint] = pE;
	m_insertionPoint++;
	//cout << "v:did it" << endl;
	return m_insertionPoint-1;
    } else {
	return -1;
    }
}


#endif // _VECTOR_H_
