/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hoitem that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
	
	$Id$
____________________________________________________________________________*/

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

    T FirstItem();
    T LastItem();

    T ItemAt(int32 index);
    void SetItemAt(const T &item, int32 index);
    int32 IndexOf(T &item);
    bool HasItem(T &item);
    bool IsEmpty();

    int32 CountItems();
    
    bool AddItem(const T &item);
    bool AddItem(const T &item, int32 index);

    bool AddList(List<T> &list);
    bool AddList(List<T> &list,int32 index);

    bool RemoveItem(T &item);
    bool DeleteItem(T &item);

    bool RemoveItems(int32 begin, int32 end);
    bool DeleteItems(int32 begin, int32 end);

    T RemoveItem(int32 index);
    bool DeleteItem(int32 index);

    bool RemoveList(List<T> &list);
    bool DeleteList(List<T> &list);

    bool RemoveAll();
    bool DeleteAll();
    
    T RandomItem();

    void Swap(int32 index1, int32 index2);
    
    void DoForEach(bool (*func)(T &));
    void DoForEach(bool (*func)(T &, void *), void *);

    T* Items() const { return m_list; }

    // cmp returns true if first arg is 'greater' than the second ; 
    // sorts list smallest to largest according to cmp
    void SortItems(int (*cmp)(const T &, const T &)); 

 private:
    T*      m_list;  // array of objects
    int32   m_threshhold;
    int32   m_insertionPoint;
    int32   m_currentLength;
};

template<class T> 
List<T>::
List(int32 threshold) 
{
    m_threshhold = threshold;
    m_list = new T[m_threshhold];

    for (int32 i=0;i<m_threshhold;i++) 
        m_list[i] = NULL;

    m_insertionPoint = 0;
    m_currentLength = m_threshhold;
}

template<class T> 
List<T>::
~List() 
{
    delete [] m_list;
}


template<class T> 
T 
List<T>::
FirstItem() 
{
    if(m_insertionPoint > 0) 
    {
	    return m_list[0];
    } 
    else 
    {
	    return NULL;
    }
}

template<class T> 
T 
List<T>::
LastItem() 
{
    if(m_insertionPoint > 0) 
    {
	    return m_list[m_insertionPoint - 1];
    } 
    else 
    {
	    return NULL;
    }
}

template<class T> 
T 
List<T>::
ItemAt(int32 index) 
{
    if((index >= m_insertionPoint) || (index < 0)) 
    {
	    return NULL;
    }

    return m_list[index];
}

template<class T>
void
List<T>::
SetItemAt(const T &item, int32 index)
{
    if (index < m_insertionPoint && index >= 0)
    {
	m_list[index] = item;
    }
}

template<class T> 
int32 
List<T>::
IndexOf(T &mem) 
{
    int32 result = -1;

    for(int32 i=0;i<m_insertionPoint;i++) 
    {
	    if(m_list[i] == mem) 
        {
	        result = i;
	        break;
	    }
    }

    return result;
}

template<class T> 
bool 
List<T>::
HasItem(T &item) 
{
    if(IndexOf(item) >= 0) 
    {
	    return true;
    } 
    else 
    {
	    return false;
    }
}

template<class T> 
bool 
List<T>::
IsEmpty() 
{
    return !m_insertionPoint;
}

template<class T> 
int32 
List<T>::
CountItems() 
{
    return m_insertionPoint;
}

template<class T> 
bool 
List<T>::
AddItem(const T &item) 
{
    bool result = false;

    if(item) 
    {
	    if (m_insertionPoint == (m_currentLength-1)) 
        {
	        // add more and copy over
	        T *pNewObjs = new T[m_currentLength+m_threshhold];
	        memcpy(pNewObjs,m_list,(sizeof (T))*m_currentLength);

	        delete [] m_list;

	        m_list = pNewObjs;

	        memset(&(m_list[m_currentLength]),0,m_threshhold);
	        m_currentLength = m_currentLength + m_threshhold;
	    }

	    m_list[m_insertionPoint] = item;
	    m_insertionPoint++;

	    result = true;
    } 
   
    return result;
}

template<class T> 
bool 
List<T>::
AddItem(const T &item, int32 index) 
{
    bool result = false;

    if (item) 
    {
	    if ((index<=m_insertionPoint) && (index >= 0)) 
        {
	        if (m_insertionPoint == (m_currentLength-1)) 
            {
		        T *pNewObjs = new T[m_currentLength+m_threshhold];

		        memcpy(pNewObjs,m_list,(sizeof (T))*index);
		        pNewObjs[index] = item;

		        if (index < m_insertionPoint) 
                {
		            memcpy(&(pNewObjs[index+1]), &(m_list[index]),(sizeof (T))*(m_insertionPoint-index));
		        }

		        delete [] m_list;

		        m_list = pNewObjs;
                m_currentLength += m_threshhold;

		        memset(&(m_list[m_insertionPoint+1]),0, m_currentLength - m_insertionPoint - 1);
	        } 
            else 
            {
		        memmove(&(m_list[index+1]),&(m_list[index]), (m_insertionPoint - index) * sizeof(T));
		        m_list[index] = item;
	        }

	        m_insertionPoint++;
	        result = true;
	    }
    }

    return result;
}

// FIXME: how to back out if one of the Adds fails?
template<class T> 
bool 
List<T>::
AddList(List<T> &list) 
{
    bool result = true;

    for(int32 i = 0; i < list.CountItems(); i++) 
    {
	    if (!AddItem(list.ItemAt(i))) 
        {
	        result = false;
	        break;
	    }
    }

    return result;
}

// FIXME: how to back out if one of the Adds fails?
template<class T> 
bool 
List<T>::
AddList(List<T> &list,int32 index) 
{
    bool result = true;

    for(int32 i = 0; i < list.CountItems(); i++) 
    {
        T foo = list.ItemAt(i);

	    if (!AddItem(foo, index + i)) 
        {
	        result = false;
	        break;
	    }
    }

    return result;
}

template<class T> 
bool 
List<T>::
RemoveItem(T &item) 
{
    bool result = false;

    int32 index = IndexOf(item);

    if (index >= 0) 
    {
	    RemoveItem(index);
	    result = true;
    }

    return result;
}

template<class T> 
bool 
List<T>::
DeleteItem(T &item) 
{
    bool result = false;

    int32 index = IndexOf(item);

    if (index >= 0) 
    {
	    DeleteItem(index);
	    result = true;
    }

    return result;
}

template<class T> 
bool List<T>::
RemoveItems(int32 begin, int32 end) 
{
    bool result = false;

    if (!((begin >= m_insertionPoint)   || 
        (begin < 0) || (end > begin)    || 
        (end >= m_insertionPoint))) 
    {
	    memmove(&(m_list[begin]),&(m_list[end+1]),sizeof(T)*(m_insertionPoint-end-1));
	    m_insertionPoint -= end-begin+1;
	    result = true;
    }

    return result;
}

template<class T> 
bool 
List<T>::
DeleteItems(int32 begin, int32 end) 
{
    bool result = false;

    if (!((begin >= m_insertionPoint)   || 
        (begin < 0)                     || 
        (end > begin)                   || 
        (end >= m_insertionPoint))) 
    {
	    for(int32 i = begin; i <= end; i++) 
        {
	        delete m_list[i];
	    }

	    result = RemoveItems(begin, end);
    }
    return result;
}

template<class T> 
T 
List<T>::
RemoveItem(int32 index) 
{
    if ((index >= m_insertionPoint) || (index < 0)) 
    {
	    return NULL;
    }

    T resultval = m_list[index];

    if(index == m_insertionPoint - 1) // if it is the last one just NULL it
    {
        m_list[index] = 0x00;
    }
    else // shift the other ones up 
    {
        memmove(&(m_list[index]),&(m_list[index+1]),sizeof(T)*(m_insertionPoint-index-1));
    }
   
    m_insertionPoint--;

    return resultval;
}

template<class T> 
bool 
List<T>::
DeleteItem(int32 index) 
{
    T p = RemoveItem(index);

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

template<class T> 
bool 
List<T>::
RemoveList(List<T> &list)
{
    bool result = true;

    for(int32 i = 0; i < list.CountItems(); i++) 
    {
        T foo = list.ItemAt(i);

	    if(!RemoveItem(foo)) 
        {
	        result = false; // let them know some were not removed
	    }
    }

    return result;
}

template<class T> 
bool 
List<T>::
DeleteList(List<T> &list)
{
    bool result = true;

    for(int32 i = 0; i < list.CountItems(); i++) 
    {
        T foo = list.ItemAt(i);

	    if(!DeleteItem(foo)) 
        {
	        result = false; // let them know some were not deleted
	    }
    }

    return result;
}


template<class T> 
bool 
List<T>::
RemoveAll() 
{
    for (int32 i = 0; i < m_insertionPoint; i++) 
        m_list[i] = NULL;

    m_insertionPoint = 0;

    return true;
}

template<class T> 
bool 
List<T>::
DeleteAll() 
{
    T item;
    int32 i = 0;

    while(item = ItemAt(i++))
    {
        delete item;
    }

    /*for(int32 i = 0; i < m_insertionPoint; i++) 
    {
	    delete m_list[i];
	    m_list[i] = NULL;
    }*/

    m_insertionPoint = 0;

    return true;
}

template<class T> 
T 
List<T>::
RandomItem() 
{
    srand((unsigned int) time (NULL)); // XXX: Yuck!  Fix this.
    int32 foo = (int32) (((double)m_insertionPoint * rand()) / (RAND_MAX+1.0));
    return m_list[foo];
}

template<class T> 
void 
List<T>::
Swap(int32 s1, int32 s2) 
{
    if ((s1 < 0)                ||
	(s2 < 0)                    ||
	(s1 >= m_insertionPoint)    ||
	(s2 >= m_insertionPoint)    ||
	(s1 == s2)) 
    {
	    return;
    }

    T tmp = m_list[s1];
    m_list[s1] = m_list[s2];
    m_list[s2] = tmp;
}

template<class T> 
void 
List<T>::
DoForEach(bool (*func)(T &)) 
{
    for(int i=0;i<m_insertionPoint;i++) 
    {
	    *func(m_list[i]);
    }
}

template<class T> 
void 
List<T>::
DoForEach(bool (*func)(T &, void *), void *arg) 
{
    for(int i=0;i<m_insertionPoint;i++) 
    {
	    *func(m_list[i],arg);
    }
}

// FIXME: just a bubble sort for now
template<class T> 
void 
List<T>::
SortItems(int (*cmp)(const T &, const T &)) 
{
    for(int end = m_insertionPoint-2; end > 0; end--) 
	for(int i=0; i < end; i++) 
	    if (cmp(m_list[i],m_list[i+1])) 
		Swap(i,i+1);
}


#endif // _LIST_H_


