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

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "config.h"

template <class T>
class Item {
 public:
    Item(T i, Item* n = NULL, Item* l = NULL) 
    { 
        m_member = i; 
        m_next = n; 
        m_last = l;
    } 

    Item* Next() const { return m_next;}
    Item* Last() const { return m_last;}

    Item* Next(Item* n)
    { 
        Item* temp = m_next;
        m_next = n;
        return temp;
    }

    Item* Last(Item* l)
    { 
        Item* temp = m_last;
        m_last = l;
        return temp;
    }

    T Member() const { return m_member;}



 protected:
    T   m_member;
    Item* m_next;
    Item* m_last;
};

template<class T>
class LinkedList {
 public:
     LinkedList(){ m_first = m_last = NULL;}
     ~LinkedList(){ DeleteAll(); m_first = m_last = NULL;}

    Item<T>* FirstItem() const { return m_first;}
    Item<T>* LastItem() const { return m_last;}
    Item<T>* NextItem(Item<T>* item){ return item->Next();}
    Item<T>* PriorItem(Item<T>* item){ return item->Last();}

    void InsertBefore(Item<T>* item, T member)
    {
        Item<T>* newItem = new Item<T>(member);

        InsertBefore(item, newItem);
    }

    void InsertBefore(Item<T>* item, Item<T>* newItem)
    {
        assert(newItem);

        if( item == NULL && 
            m_first == NULL && 
            m_last == NULL)
        {
            m_first = m_last = newItem;

            newItem->Next(NULL);
            newItem->Last(NULL);
        }
        else if(item == m_first)
        {
            assert(item);

            newItem->Next(item);
            newItem->Last(NULL);
            item->Last(newItem);

            m_first = newItem;
        }
        else
        {
            assert(item);

            newItem->Next(item->Last()->Next(newItem));
            newItem->Last(item->Last(newItem));
        }
    }

    void InsertAfter(Item<T>* item, T member)
    {
        Item<T>* newItem = new Item<T>(member);

        InsertAfter(item, newItem);
    }

    void InsertAfter(Item<T>* item, Item<T>* newItem)
    {
        assert(newItem);

        if( item == NULL && 
            m_first == NULL && 
            m_last == NULL)
        {
            m_first = m_last = newItem;

            newItem->Next(NULL);
            newItem->Last(NULL);
        }
        else if(item == m_last)
        {
            assert(item);

            newItem->Last(item);
            newItem->Next(NULL);
            item->Next(newItem);

            m_last = newItem;
        }
        else
        {
            assert(item);

            newItem->Last(item->Next()->Last(newItem));
            newItem->Next(item->Next(newItem));
        }
    }

    void Append(T member)
    {
        Item<T>* newItem = new Item<T>(member);

        InsertAfter(LastItem(), newItem);
    }

    void Append(Item<T>* newItem)
    {
        assert(newItem);

        InsertAfter(LastItem(), newItem);
    }

    void Prepend(T member)
    {
        Item<T>* newItem = new Item<T>(member);

        InsertBefore(FirstItem(), newItem);
    }

    void Prepend(Item<T>* newItem)
    {
        assert(newItem);

        InsertBefore(FirstItem(), newItem);
    }

    Item<T>* RemoveItem(Item<T>* item)
    {
        assert(item);

        if(item->Next())
        {
            item->Next()->Last(item->Last());
        }

        if(item->Last())
        {
            item->Last()->Next(item->Next());
        }

        if(item == m_first)
        {
            m_first = item->Next();
        }

        if(item == m_last)
        {
            m_last = item->Last();
        }
        
        return item;
    }

    void RemoveAll()
    {
        Item<T>* i = FirstItem();

        while(i)
        {
            Item<T>* next = i->Next();

            RemoveItem(i);

            i = next;
        }
    }

    void DeleteItem(Item<T>* item)
    {
        assert(item);

        delete RemoveItem(item);
    }

    void DeleteAll()
    {
        Item<T>* i = FirstItem();

        while(i)
        {
            Item<T>* next = i->Next();

            delete RemoveItem(i);

            i = next;
        }
    }

    int32 Count()
    {
        Item<T>* i = FirstItem();
        int32 count = 0;

        while(i)
        {
            Item<T>* next = i->Next();

            delete RemoveItem(i);

            i = next;

            count++;
        }

        return count;
    }
 
 private:
     Item<T>* m_first;
     Item<T>* m_last;
};





#endif // _LINKED_LIST_H_
