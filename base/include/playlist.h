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

#include <assert.h>

#include "config.h"

#include "list.h"
#include "errors.h"
#include "event.h"
#include "registry.h"
#include "eventdata.h"
#include "mutex.h"
#include "pmi.h"

const int iMaxFileNameLen = 1024;

class PlayListItem {

 public:
    
    PlayListItem() 
    {
	    m_mie = NULL;
	    m_url = NULL;
	    m_pmi = NULL;
	    m_pmiRegItem = NULL;
	    m_lmcRegItem = NULL;
        m_DisplayString = NULL;
    }

    ~PlayListItem() 
    {
	    if (m_mie) 
        {
	        delete m_mie;
	        m_mie = NULL;
	    }

	    if (m_pmi) 
        {
	        delete m_pmi;
	        m_pmi = NULL;
	    }

	    m_pmiRegItem = NULL;
	    m_lmcRegItem = NULL;

	    if (m_url) 
        {
	        delete m_url;
	        m_url = NULL;
	    }
        if (m_DisplayString)
        {
            delete m_DisplayString;
            m_DisplayString = NULL;
        }
    }

    char* URL() const { return m_url;}

    void SetURL(char* url)
    { 
        assert(url);

        if(url)
        {
            if(m_url)
            {
                delete [] m_url;
                m_url = NULL;
            }

            m_url = new char[strlen(url) + 1];
            
            if(m_url)
            {
                strcpy(m_url, url);
            }
        }
    }

    int32 Type() const { return m_type; }
    void SetType(int32 type) { m_type = type; }

    int32 StartFrame() const { return m_startFrame; }
    void SetStartFrame(int32 frame) { m_startFrame = frame; }

    char* StringForPlayerToDisplay() const 
	{ 
        char* result = NULL;

        if(m_DisplayString)
            result = m_DisplayString;
        else if(m_url)
        {
            result = strrchr(m_url, '\\');

            if(result)
                result++;
            else
                result = m_url;
        }
        else
            result = "";

		return result;
	}

    void SetDisplayString(char *pDisplayString)
    {
        if (m_DisplayString)
            delete m_DisplayString;

        m_DisplayString = new char[strlen(pDisplayString) + 1];
        strcpy(m_DisplayString, pDisplayString);
    }


    RegistryItem *GetPMIRegistryItem() { return m_pmiRegItem; }
    void SetPMIRegistryItem(RegistryItem *ri) { m_pmiRegItem = ri; }
    RegistryItem *GetLMCRegistryItem() { return m_lmcRegItem; }
    void SetLMCRegistryItem(RegistryItem *ri) { m_lmcRegItem = ri; }
    MediaInfoEvent *GetMediaInfo() { return m_mie; }
    void SetMediaInfo(MediaInfoEvent *mie) { m_mie = mie; }
    PhysicalMediaInput *GetPMI() { return m_pmi; }
    void SetPMI(PhysicalMediaInput *pmi) { m_pmi = pmi; }

 private:
    RegistryItem *m_pmiRegItem;
    RegistryItem *m_lmcRegItem;
    MediaInfoEvent *m_mie;
    PhysicalMediaInput *m_pmi;

    char* m_url, *m_DisplayString;
    int32 m_type;
    int32 m_startFrame;
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
	 PhysicalMediaInput *m_pmi;
 public:
    PLMSetMediaInfoEvent() {
	m_type = CMD_PLMSetMediaInfo;
	m_complete = false;
	m_pmiRegItem = NULL;
	m_lmcRegItem = NULL;
	m_mie = NULL;
	m_pli = NULL;
	m_pmi = NULL;
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
    PhysicalMediaInput *GetPMI() { return m_pmi; }
    void SetPMI(PhysicalMediaInput *pmi) { m_pmi = pmi; }
};

enum ShuffleMode {
	SHUFFLE_NOT_SHUFFLED = 0,
	SHUFFLE_RANDOM,                             
    SHUFFLE_LAST_ENUM

};
    
enum RepeatMode {
	REPEAT_NOT = 0,
	REPEAT_CURRENT,
	REPEAT_ALL,
	REPEAT_LAST_ENUM
};

class PlayListItemUpdatedEvent : public Event {
private:
	PlayListItem* m_item;
public:
	PlayListItemUpdatedEvent(PlayListItem* item) 
    { m_type = INFO_PlayListItemUpdated; m_item = item; }
	virtual ~PlayListItemUpdatedEvent() {}

	PlayListItem* UpdatedItem() { return m_item; }
};

class PlayListRepeatEvent : public Event {
private:
	RepeatMode m_rm;
public:
	PlayListRepeatEvent(RepeatMode rm) 
    { m_type = INFO_PlayListRepeat; m_rm = rm; }
	virtual ~PlayListRepeatEvent() {}

	RepeatMode GetRepeatMode() { return m_rm; }
};

class PlayListShuffleEvent : public Event {
private:
	ShuffleMode m_sm;
public:
	PlayListShuffleEvent(ShuffleMode sm) 
    { m_type = INFO_PlayListShuffle; m_sm = sm; }
	virtual ~PlayListShuffleEvent() {}

	ShuffleMode GetShuffleMode() { return m_sm; }
};


class PlayListManager {
 public:
    PlayListManager(EventQueue *);
    ~PlayListManager();
    // logical media units to skip at beginning
    // helps us get past id3v2 tags
    void SetSkip(int32 f) { m_skipNum = f; } 
    int32 GetSkip() { return m_skipNum; }

    void AcceptEvent(Event *);

    bool HasAnotherSong();

    PlayListItem *GetCurrent();
    void SetFirst();
    void SetNext(bool bUserAction = false);
    void SetPrev(bool bUserAction = false);

    int32 Current() const {return m_current;}
    void SetCurrent(int32 index);

    void SetShuffle(ShuffleMode oop);
    void SetRepeat(RepeatMode rp);
    ShuffleMode GetOrder() {return m_order;}
    RepeatMode GetRepeat() {return m_repeat;}
    Error ToggleRepeat();
    Error ToggleShuffle();

    PlayListItem* FirstItem();
	PlayListItem* LastItem();

	bool HasItem(PlayListItem* item);
	int32 CountItems();

    PlayListItem*   ItemAt(int32 index);
	int32	        IndexOf(PlayListItem* item);

    virtual	Error AddItem(char *url,int32 type);
    virtual	Error AddItem(char *url,int32 type, int32 index);

    virtual	Error AddItem(PlayListItem* item);
    virtual Error AddItem(PlayListItem* item, int32 index);
    virtual Error AddList(List<PlayListItem*>* items);
    virtual Error AddList(List<PlayListItem*>* items, int32 index);

    virtual Error           RemoveItem(PlayListItem* item);
    virtual PlayListItem*   RemoveItem(int32 index);
    virtual Error           RemoveItems(int32 index, int32 count);
    virtual Error           RemoveList(List<PlayListItem*>* items);
    virtual Error           RemoveAll();

    virtual Error           MoveList(List<PlayListItem*>* items, int32 index);

    virtual	void MakeEmpty();
		    bool IsEmpty();

    void DoForEach(bool (*func)(PlayListItem*));
    void DoForEach(bool (*func)(PlayListItem*, void*), void*);

    const PlayListItem** Items() const;

    Error ExpandM3U(char *szM3UFile, List<char *> &MP3List);
    Error ExportAsM3U(const char* file);


 protected:
    inline int32 CheckIndex(int32 index);

    void SendInfoToPlayer();
    void SendShuffleModeToPlayer();
    void SendRepeatModeToPlayer();

    void InitializeOrder();
    void ShuffleOrder();
    void QuickSortOrderList(int32 first, int32 last);
    int32 PartitionOrderList(int32 first, int32 last);

    inline void GetPLManipLock() { m_mutex->Acquire(WAIT_FOREVER); }
    inline void ReleasePLManipLock() { m_mutex->Release(); }

 private:
    
    EventQueue*			    m_target;
    List<PlayListItem*>*    m_list;
    int32                   m_current;
    int32                   m_skipNum;
	Mutex*					m_mutex;

    
    ShuffleMode             m_order;
    RepeatMode              m_repeat;

    
};


#endif // _PLAYLIST_H_










