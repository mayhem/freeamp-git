
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
#include "mutex.h"

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
    int32 m_indexToRealVector;
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

enum ShuffleMode {
	SHUFFLE_NOT_SHUFFLED = 0,  // normal playlist playback
	SHUFFLE_SHUFFLED,      // playlist is shuffled, with playback in order of shuffled list
	SHUFFLE_INTERNAL_NUMBER, // swap this and SHUFFLE_RANDOM to enable random shuffling
	SHUFFLE_RANDOM         // random song is played (and, GetCurrent() never returns NULL)
};
    
enum RepeatMode {
	REPEAT_NOT = 0,
	REPEAT_CURRENT,
	REPEAT_ALL,
	REPEAT_INTERNAL_NUMBER
};



class PlayListRepeatEvent : public Event {
private:
	RepeatMode m_rm;
public:
	PlayListRepeatEvent(RepeatMode rm) { m_type = INFO_PlayListRepeat; m_rm = rm; }
	virtual ~PlayListRepeatEvent() {}

	RepeatMode GetRepeatMode() { return m_rm; }
};

class PlayListShuffleEvent : public Event {
private:
	ShuffleMode m_sm;
public:
	PlayListShuffleEvent(ShuffleMode sm) { m_type = INFO_PlayListShuffle; m_sm = sm; }
	virtual ~PlayListShuffleEvent() {}

	ShuffleMode GetShuffleMode() { return m_sm; }
};


class PlayListManager {
 public:
    PlayListManager(EventQueue *);
    ~PlayListManager();
    void Add(char *,int);
    Error RemoveAll();
    void SetSkip(int32 f) { m_skipNum = f; } // logical media units to skip at beginning
    int32 GetSkip() { return m_skipNum; }

    void AcceptEvent(Event *);

    bool NextIsSame();

    PlayListItem *GetCurrent();
    void SetFirst();
    void SetNext();
    void SetPrev();

    int32 Current() const {return m_current;}
    int32 Total() const {return m_pOrderList->NumElements();}

    void SetShuffle(ShuffleMode oop);
    void SetRepeat(RepeatMode rp);
    ShuffleMode GetOrder() {return m_order;}
    RepeatMode GetRepeat() {return m_repeat;}
	Error ToggleRepeat();
	Error ToggleShuffle();

 private:
    
    EventQueue *			m_target;
    Vector<PlayListItem *>* m_pMediaElems;
    int32                   m_current;
    int32                   m_skipNum;
	Mutex *					m_plMutex;

    Vector<OrderListItem *> *m_pOrderList;
    ShuffleMode m_order;
    RepeatMode m_repeat;

    void SendInfoToPlayer();
	void SendShuffleModeToPlayer();
	void SendRepeatModeToPlayer();

    void InitializeOrder();
    void ShuffleOrder();
    void QuickSortOrderList(int32 first, int32 last);
    int32 PartitionOrderList(int32 first, int32 last);
 
	void GetPLManipLock() { m_plMutex->Acquire(WAIT_FOREVER); }
	void ReleasePLManipLock() { m_plMutex->Release(); }
};


#endif // _PLAYLIST_H_










