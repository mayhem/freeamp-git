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

// List.h

#ifndef _LIST_H_
#define _LIST_H_

#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include "config.h"



template<class T>
class List {
 public:
    List(int32 th = 10); // set m_threshhold to its arg
    ~List();
    bool RemoveAll();
    bool DeleteAll();
    T ElementAt(int32);
    T RemoveElementAt(int32);
    T RandomElement();
    void Swap(int32,int32);
    bool DeleteElementAt(int32);
    bool Insert(T &);
    bool InsertAt(int32,T &);
    int32 NumElements();

    bool AddList(List<T> &);
    bool AddListAt(List<T> &,int32);
    bool RemoveElement(T &);
    bool DeleteElement(T &);
    bool RemoveElements(int32,int32);
    bool DeleteElements(int32,int32);
    int32 IndexOf(T &);
    T FirstElement();
    T LastElement();
    bool HasElement(T &);
    bool IsEmpty();
    void DoForEach(bool (*func)(T &));
    void DoForEach(bool (*func)(T &, void *), void *);
    T *Elements() const { return m_pObjs; }
    // cmp returns true if first arg is 'greater' than the second ; 
    // sorts from smallest to largest according to cmp
    void SortItems(int (*cmp)(const T &, const T &)); 
    //bool Mirror(List<T> &);

 private:
    T *m_pObjs;  // pointer to array of objects
    int32 m_threshhold;
    int32 m_insertionPoint;
    int32 m_currentLength;
 
};

/*template<class T> 
bool 
List<T>::
Mirror(List<T> &src) 
{
    bool result = false;

    // quick check to see if they are the same
    int32 srcCount = src->NumElements();
    int32 destCount = NumElements();
    bool different = false;

    if(srcCount != destCount)
        different = true;
    else if(memcmp(src->Elements(), Elements(), destCount))
        different = true;

    if(different)
    {
        result = true;



    }

    return result;
}*/


// FIXME: how to back out if one of the Inserts fails?
template<class T> bool List<T>::AddList(List<T> &from) {
    bool result = true;
    for(int i=0;i<from.NumElements();i++) {
	if (!Insert(from.ElementAt(i))) {
	    result = false;
	    break;
	}
    }
    return result;
}

// FIXME: how to back out if one of the InsertAt's fails?
template<class T> bool List<T>::AddListAt(List<T> &from,int32 at) {
    bool result = true;
    for(int i=0;i<from.NumElements();i++) {
        T foo = from.ElementAt(i);

	    if (!InsertAt(at + i, foo)) {
	        result = false;
	        break;
	    }
    }
    return result;
}

template<class T> bool List<T>::RemoveElement(T &elem) {
    bool result = false;
    int32 index = IndexOf(elem);
    if (index >= 0) {
	RemoveElementAt(index);
	result = true;
    }
    return result;
}

template<class T> bool List<T>::DeleteElement(T &elem) {
    bool result = false;
    int32 index = IndexOf(elem);
    if (index >= 0) {
	DeleteElementAt(index);
	result = true;
    }
    return result;
}

template<class T> bool List<T>::RemoveElements(int32 begin, int32 end) {
    bool result = false;
    if (!((begin >= m_insertionPoint) || (begin < 0) || (end > begin) || (end >= m_insertionPoint))) {
	memmove(&(m_pObjs[begin]),&(m_pObjs[end+1]),sizeof(T)*(m_insertionPoint-end-1));
	m_insertionPoint -= end-begin+1;
	result = true;
    }
    return result;
}
template<class T> bool List<T>::DeleteElements(int32 begin, int32 end) {
    bool result = false;
    if (!((begin >= m_insertionPoint) || (begin < 0) || (end > begin) || (end >= m_insertionPoint))) {
	for(int i=begin;i<=end;i++) {
	    delete m_pObjs[i];
	}
	result = RemoveElements(begin,end);
    }
    return result;
}
template<class T> int32 List<T>::IndexOf(T &mem) {
    int32 result = -1;
    for(int i=0;i<m_insertionPoint;i++) {
	if (m_pObjs[i] == mem) {
	    result = i;
	    break;
	}
    }
    return result;
}

template<class T> T List<T>::FirstElement() {
    if (m_insertionPoint > 0) {
	return m_pObjs[0];
    } else {
	return NULL;
    }
}

template<class T> T List<T>::LastElement() {
    if (m_insertionPoint > 0) {
	return m_pObjs[m_insertionPoint - 1];
    } else {
	return NULL;
    }
}

template<class T> bool List<T>::HasElement(T &item) {
    if (IndexOf(item) >= 0) {
	return true;
    } else {
	return false;
    }
}

template<class T> bool List<T>::IsEmpty() {
    return !m_insertionPoint;
}
template<class T> void List<T>::DoForEach(bool (*func)(T &)) {
    for(int i=0;i<m_insertionPoint;i++) {
	*func(m_pObjs[i]);
    }
}

template<class T> void List<T>::DoForEach(bool (*func)(T &, void *), void *arg) {
    for(int i=0;i<m_insertionPoint;i++) {
	*func(m_pObjs[i],arg);
    }
}

// FIXME: just a bubble sort for now
template<class T> void List<T>::SortItems(int (*cmp)(const T &, const T &)) {
    for(int end = m_insertionPoint-2;i>= 0;end--) {
	for(int i=0;i<end;i++) {
	    if (*cmp(m_pObjs[i],m_pObjs[i+1])) {
		Swap(i,i+1);
	    }
	}
    }
}

template<class T> T List<T>::RandomElement() {
    srand((unsigned int) time (NULL));
    int32 foo = (int32) (((double)m_insertionPoint * rand()) / (RAND_MAX+1.0));
    return m_pObjs[foo];
}

template<class T> void List<T>::Swap(int32 s1, int32 s2) {
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

template<class T> List<T>::List(int32 th) {
    m_threshhold = th;
    m_pObjs = new T[m_threshhold];
    for (int32 i=0;i<m_threshhold;i++) m_pObjs[i] = NULL;
    m_insertionPoint = 0;
    m_currentLength = m_threshhold;
}

template<class T> List<T>::~List() {
    delete m_pObjs;
}

template<class T> int32 List<T>::NumElements() {
    return m_insertionPoint;
}

template<class T> bool List<T>::RemoveAll() {
    for (int32 i=0;i<m_insertionPoint;i++) m_pObjs[i] = NULL;
    m_insertionPoint = 0;
    return true;
}

template<class T> bool List<T>::DeleteAll() {
    for(int32 i=0;i<m_insertionPoint;i++) {
	delete m_pObjs[i];
	m_pObjs[i] = NULL;
    }
    m_insertionPoint = 0;
    return true;
}

template<class T> T List<T>::ElementAt(int32 e) {
    if ((e >= m_insertionPoint) || (e < 0)) {
	return NULL;
    }
    return m_pObjs[e];
}

template<class T> T List<T>::RemoveElementAt(int32 e) {
    if ((e >= m_insertionPoint) || (e < 0)) {
	return NULL;
    }
    T resultval = m_pObjs[e];

    if(e == m_insertionPoint - 1) // if it is the last one just NULL it
    {
        m_pObjs[e] = 0x00;
    }
    else // shift the other ones up 
    {
        memmove(&(m_pObjs[e]),&(m_pObjs[e+1]),sizeof(T)*(m_insertionPoint-e-1));
    }
    //for (int32 i = e;i<m_insertionPoint;i++) {
    //   m_pObjs[i] = m_pObjs[i+1];
    //}
    m_insertionPoint--;
    return resultval;
}

template<class T> bool List<T>::DeleteElementAt(int32 e) {
    T p = RemoveElementAt(e);
    if (p) 
    {
	    delete p;
	    return true;
    } 
    else 
    {
	    return false;
    }
}

template<class T> bool List<T>::InsertAt(int32 point, T &pE) {
    bool result = false;
    if (pE) {
	    if ((point<=m_insertionPoint) && (point >= 0)) {
	        if (m_insertionPoint == (m_currentLength-1)) {
		    T *pNewObjs = new T[m_currentLength+m_threshhold];
		    memcpy(pNewObjs,m_pObjs,(sizeof (T))*point);
		    pNewObjs[point] = pE;
		    if (point < m_insertionPoint) {
		        memcpy(&(pNewObjs[point+1]), &(m_pObjs[point]),(sizeof (T))*(m_insertionPoint-point));
		    }
		    delete m_pObjs;
		    m_pObjs = pNewObjs;
		    memset(&(m_pObjs[m_insertionPoint+1]),0,m_insertionPoint + m_threshhold - 2);
		    m_currentLength += m_threshhold;
	        } else {
		    memmove(&(m_pObjs[point+1]),&(m_pObjs[point]), (m_insertionPoint - point) * sizeof(T));
		    m_pObjs[point] = pE;
	        }
	        m_insertionPoint++;
	        result = true;
	    }
    }
    return result;
}

template<class T> bool List<T>::Insert(T &pE) {
//    cout << "v:inserting" << endl;
    bool result = false;

    if (pE) 
    {
	    if (m_insertionPoint == (m_currentLength-1)) {
	        //cout << "v:adding more" << endl;
	        // add more and copy over
	        T *pNewObjs = new T[m_currentLength+m_threshhold];
	        //cout << "v:got it " << m_currentLength+m_threshhold << endl;
	        memcpy(pNewObjs,m_pObjs,(sizeof (T))*m_currentLength);
	        //cout << "v:did copy" << endl;
	        delete m_pObjs;
	        m_pObjs = pNewObjs;
	        memset(&(m_pObjs[m_currentLength]),0,m_threshhold);
	        //for (int32 i=m_currentLength;i<m_currentLength + m_threshhold;i++) m_pObjs[i] = NULL;
	        m_currentLength = m_currentLength + m_threshhold;
	    }
	    m_pObjs[m_insertionPoint] = pE;
	    m_insertionPoint++;
	    //cout << "v:did it" << endl;
	    result = true;
    } 
   
    return result;
}


#endif // _List_H_
