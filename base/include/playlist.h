
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
#include "errors.h"
#include "event.h"
#include "registry.h"
#include "eventdata.h"

class PlayListItem {
 private:
    RegistryItem *m_pmiRegItem;
    RegistryItem *m_lmcRegItem;
    MediaInfoEvent *m_mie;
 public:
    char* m_url;
    int32 m_type;
    int32 m_startFrame;

    PlayListItem() {
	m_mie = NULL;
	m_url = NULL;
	m_pmiRegItem = NULL;
	m_lmcRegItem = NULL;
    }
    ~PlayListItem() {
	if (m_mie) {
	    delete m_mie;
	    m_mie = NULL;
	}
	m_pmiRegItem = NULL;
	m_lmcRegItem = NULL;
	if (m_url) {
	    delete m_url;
	    m_url = NULL;
	}
    }

    RegistryItem *GetPMIRegistryItem() { return m_pmiRegItem; }
    void SetPMIRegistryItem(RegistryItem *ri) { m_pmiRegItem = ri; }
    RegistryItem *GetLMCRegistryItem() { return m_lmcRegItem; }
    void SetLMCRegistryItem(RegistryItem *ri) { m_lmcRegItem = ri; }
    MediaInfoEvent *GetMediaInfo() { return m_mie; }
    void SetMediaInfo(MediaInfoEvent *mie) { m_mie = mie; }
};

class OrderListItem {
 public:
    int32 m_index;
    int32 m_random;
};

class PLMGetMediaInfoEvent : public Event {
 private:
    PlayListItem *m_plItem;
 public:
    PLMGetMediaInfoEvent() {
	m_type = CMD_PLMGetMediaInfo;
	m_plItem = NULL;
    }
    virtual ~PLMGetMediaInfoEvent() {}
    void SetPlayListItem(PlayListItem *pli) { m_plItem = pli; }
    PlayListItem *GetPlayListItem() { return m_plItem; }
};

class PLMSetMediaInfoEvent : public Event {
 private:
    bool m_complete;
    RegistryItem *m_pmiRegItem;
    RegistryItem *m_lmcRegItem;
    MediaInfoEvent *m_mie;
    PlayListItem *m_pli;
 public:
    PLMSetMediaInfoEvent() {
	m_type = CMD_PLMSetMediaInfo;
	m_complete = false;
	m_pmiRegItem = NULL;
	m_lmcRegItem = NULL;
	m_mie = NULL;
	m_pli = NULL;
    }
    virtual ~PLMSetMediaInfoEvent() {}
    void SetPlayListItem(PlayListItem *pli) { m_pli = pli; }
    PlayListItem *GetPlayListItem() { return m_pli; }
    void SetComplete() { m_complete = true; }
    bool IsComplete() { return m_complete; }
    void SetPMIRegistryItem(RegistryItem *item) { m_pmiRegItem = item; }
    RegistryItem *GetPMIRegistryItem() { return m_pmiRegItem; }
    void SetLMCRegistryItem(RegistryItem *item) { m_lmcRegItem = item; }
    RegistryItem *GetLMCRegistryItem() { return m_lmcRegItem; }
    void SetMediaInfo(MediaInfoEvent *mie) { m_mie = mie; }
    MediaInfoEvent *GetMediaInfo() { return m_mie; }
};


class PlayListManager {
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
    PlayListManager(EventQueue *);
    ~PlayListManager();
    void Add(char *,int);
	Error RemoveAll();
    void SetSkip(int32 f) { m_skipNum = f; } // logical media units to skip at beginning
    int32 GetSkip() { return m_skipNum; }

    void AcceptEvent(Event *);

    PlayListItem *GetFirst();
    PlayListItem *GetNext();
    PlayListItem *GetPrev();
    PlayListItem *GetCurrent();
    void SetFirst();
    void SetNext();
    void SetPrev();

    int32 Current() const {return m_current;}
    int32 Total() const {return m_pOrderList->NumElements();}

    void SetOrder(OrderOfPlay oop);
    void SetRepeat(RepeatPlay rp);
    OrderOfPlay GetOrder() {return m_order;}
    RepeatPlay GetRepeat() {return m_repeat;}

 private:
    
    EventQueue *			m_target;
    Vector<PlayListItem *>* m_pMediaElems;
    int32                   m_current;
    int32                   m_skipNum;

    Vector<OrderListItem *> *m_pOrderList;
    OrderOfPlay m_order;
    RepeatPlay m_repeat;

    void SendInfoToPlayer();

    void InitializeOrder();
    void ShuffleOrder();
    void QuickSortOrderList(int32 first, int32 last);
    int32 PartitionOrderList(int32 first, int32 last);
 
};


#endif // _PLAYLIST_H_










