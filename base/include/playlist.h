
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

#ifndef _PLAYLIST_H_
#define _PLAYLIST_H_

#include "vector.h"

class PlayListItem {
 public:
    char* m_url;
    int32 m_type;
    int32 m_startFrame;

    PlayListItem();
    ~PlayListItem();
};

class OrderListItem {
 public:
    int32 m_index;
    int32 m_random;
};

class PlayList {
public:
    enum OrderOfPlay {
	ORDER_STRAIGHT = 1,
	ORDER_SHUFFLED = 2,
	ORDER_RANDOM = 3
    };
    
    enum RepeatPlay {
	REPEAT_NOT = 1,
	REPEAT_ALL,
	REPEAT_CURRENT
    };

 public:
    PlayList();
    ~PlayList();
    void Add(char *,int);
    void SetSkip(int32 f) { m_skipNum = f; } // logical media units to skip at beginning
    int32 GetSkip() { return m_skipNum; }
    PlayListItem *GetFirst();
    PlayListItem *GetNext();
    PlayListItem *GetPrev();
    PlayListItem *GetCurrent();
    void SetFirst();
    void SetNext();
    void SetPrev();

    void SetOrder(OrderOfPlay oop);
    void SetRepeat(RepeatPlay rp);
    OrderOfPlay GetOrder() {return m_order;}
    RepeatPlay GetRepeat() {return m_repeat;}

 private:
    Vector<PlayListItem *>* m_pMediaElems;
    int32                   m_current;
    int32                   m_skipNum;

    Vector<OrderListItem *> *m_pOrderList;
    OrderOfPlay m_order;
    RepeatPlay m_repeat;

    void InitializeOrder();
    void ShuffleOrder();
    void QuickSortOrderList(int32 first, int32 last);
    int32 PartitionOrderList(int32 first, int32 last);
 
};


#endif // _PLAYLIST_H_










