/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise
	Portions Copyright (C) 1998 "Michael Bruun Petersen" <mbp@image.dk>

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

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "playlist.h"
#include "vector.h"

PlayList::PlayList() {
    m_pMediaElems = new Vector<PlayListItem *>();
    m_pOrderList = new Vector<OrderListItem *>();
    if (!m_pMediaElems || !m_pOrderList) {
	cerr << "Out of memory!" << endl;
	exit(1);
    }
    m_current = -1;
    m_skipNum = 0;
    m_order = ORDER_STRAIGHT;
    m_repeat = REPEAT_NOT;
    srand((unsigned int) time(NULL));
}

PlayList::~PlayList() {
    if (m_pMediaElems) {
        m_pMediaElems->DeleteAll();
	    delete m_pMediaElems;
	    m_pMediaElems = NULL;
    }
    if (m_pOrderList) {
	m_pOrderList->DeleteAll();
	delete m_pOrderList;
	m_pOrderList = NULL;
    }
}

void PlayList::Add(char *pc, int type) {
    if (pc) {
	//int len = strlen(pc) + 1;
	//char *pNewC = new char[len];
	PlayListItem* item = new PlayListItem;
	if (!item) {
	    cerr << "Out of memory!" << endl;
	    exit(1);
	}
	int len = strlen(pc) + 1;
	if(item->m_url = new char[len]) {
	    //strcpy(item->m_url,pc);
	    memcpy(item->m_url,pc,len);
	} else {
	    //XXX FIXME Uhhh...what if we run out of memory?
	    // --jdw:  then we're screwwwwwwwed
	    cerr << "Out of memory!\n";
	    exit(1);
	}
	item->m_type = type;
	m_pMediaElems->Insert(item);
	// add a corresponding element to the order list
	OrderListItem *order_item = new OrderListItem();
	if (!order_item) {
	    cerr << "Out of memory!" << endl;
	    exit(1);
	}
	order_item->m_index = m_pMediaElems->NumElements() - 1;
	order_item->m_random = 0;
	m_pOrderList->Insert(order_item);
    }
    if (m_order == ORDER_SHUFFLED) {
	int32 count = m_pOrderList->NumElements();
	int32 first, second;
	if (count > 1) {
	    first = (int32)(((double) count * rand()) / (RAND_MAX+1.0));
	    second = (int32)(((double) count * rand()) / (RAND_MAX+1.0));
	    //m_pMediaElems->Swap(first,second);
	    m_pOrderList->Swap(first,second);
	    if (m_current == first) {
	    m_current = second;
	    } else if (m_current == second) {
		m_current = first;
	    }
	}
    }
}

PlayListItem *PlayList::GetFirst() {
    SetFirst();
    return GetCurrent();
}

PlayListItem *PlayList::GetNext() {
    SetNext();
    return GetCurrent();
}

PlayListItem *PlayList::GetPrev() {
    SetPrev();
    return GetCurrent();
}

PlayListItem *PlayList::GetCurrent() {
    OrderListItem *order_item = m_pOrderList->ElementAt(m_current);
    if (order_item) {
	return m_pMediaElems->ElementAt(order_item->m_index);
    } else {
	return NULL;
    }
}

void PlayList::SetFirst() { 
    if (m_order == ORDER_RANDOM) {
	SetNext();
    } else {
	m_current = 0; 
    }
}

void PlayList::SetNext() { 
    if (m_repeat == REPEAT_CURRENT) {
	return;
    }
    int32 count = m_pOrderList->NumElements();
    if (count == 0) return;

    if (ORDER_RANDOM == m_order) { 
	m_current = (int32) (( (double)count * rand()) / (RAND_MAX+1.0));
    } else {
	if ((m_current >= count) && (m_repeat == REPEAT_ALL)) {
	    m_current = 0;
	} else {
	    m_current++;
	}
    }
}

void PlayList::SetPrev() {
    if (m_repeat == REPEAT_CURRENT) {
	return;
    }
    int32 count = m_pOrderList->NumElements();
    if (m_order == ORDER_RANDOM) {
	SetNext();
    } else {
	m_current--;
	if ((m_current < 0) && (m_repeat == REPEAT_ALL)) {
	    m_current = count - 1;
	}
    }
}

void PlayList::SetOrder(OrderOfPlay oop) {
    if (oop == m_order) return;
    if (oop == ORDER_SHUFFLED) {
	ShuffleOrder();
    } else if (m_order == ORDER_SHUFFLED) {
	InitializeOrder();
    }
    m_order = oop;
}

void PlayList::SetRepeat(RepeatPlay rp) {
    m_repeat = rp;
}

void PlayList::InitializeOrder() {
    int32 element_count = m_pOrderList->NumElements();
    if (element_count < 1) {
	return;
    }

    OrderListItem *order_item = NULL;

    // make sure that the current will be at the same song afterwards
    if ((m_current >= 0) && (m_current < element_count)) {
	order_item = m_pOrderList->ElementAt(m_current);
	if (order_item) {
	    m_current = order_item->m_index;
	}
    }

    for (int32 i=0;i< element_count;i++) {
	order_item = m_pOrderList->ElementAt(i);
	order_item->m_index = i;
	order_item->m_random = 0;
    }
}

void PlayList::ShuffleOrder() {
    int32 element_count = m_pOrderList->NumElements();
    if (element_count < 1) {
	return;
    }

    OrderListItem *order_item;

    //remember the song that m_current is "really" pointing at
    int32 actual_current, i;
    if ((m_current >=0) && (m_current < element_count)) {
	order_item = m_pOrderList->ElementAt(m_current);
	if (order_item) {
	    actual_current = order_item->m_index;
	}
    }

    for (i = 0;i < element_count;i++) {
	order_item = m_pOrderList->ElementAt(i);
	order_item->m_index = i;
	order_item->m_random = (int32) rand();
    }

    QuickSortOrderList(0, element_count - 1);

    for (i = 0;i < element_count; i++) {
	order_item = m_pOrderList->ElementAt(i);
	if (order_item->m_index == actual_current) {
	    m_current = i;
	    break;
	}
    }
}

void PlayList::QuickSortOrderList(int32 first, int32 last) {
    if (first < last) {
	int32 q = PartitionOrderList(first, last);
	QuickSortOrderList(first, q);
	QuickSortOrderList(q + 1,last);
    }
}

int32 PlayList::PartitionOrderList(int32 first, int32 last) {
    int32 x = m_pOrderList->ElementAt(first)->m_random;
    int32 i = first - 1;
    int32 j = last + 1;

    for(;;) {
	do {
	    j--;
	} while (m_pOrderList->ElementAt(j)->m_random > x);
	do {
	    i++;
	} while (m_pOrderList->ElementAt(i)->m_random < x);
	if (i < j) {
	    m_pOrderList->Swap(i,j);
	} else {
	    return j;
	}
    }
}

PlayListItem::PlayListItem() {
    m_url = NULL;
}
    
PlayListItem::~PlayListItem() {
    if(m_url)
        delete m_url;
}





