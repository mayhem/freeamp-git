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
#include "event.h"
#include "eventdata.h"
#include "mutex.h"

PlayListManager::
PlayListManager(EventQueue * pPlayer)
{
   m_mutex = new Mutex();
   m_target = pPlayer;
   m_list = new List < PlayListItem * >();
 
   m_current = -1;
   m_skipNum = 0;
   m_order = SHUFFLE_NOT_SHUFFLED;
   m_repeat = REPEAT_NOT;
   srand((unsigned int) time(NULL));
}

PlayListManager::
~PlayListManager()
{
   if(m_list)
   {
      m_list->DeleteAll();
      delete m_list;
      m_list = NULL;
   }
   
   if(m_mutex)
   {
      delete m_mutex;
      m_mutex = NULL;
   }
}

PlayListItem*
PlayListManager::
GetCurrent()
{
    GetPLManipLock();
    PlayListItem *pli = NULL;

    if ((m_current >= 0) && (m_current < m_list->CountItems()))
    {
        pli = m_list->ItemAt(m_current);
    }

    ReleasePLManipLock();
    return pli;
}

void 
PlayListManager::
SetFirst()
{
    GetPLManipLock();
    int32 elems = m_list->CountItems();

    if(elems)
    {
        m_current = 0;

        SendInfoToPlayer();
    }
    else
    {
        m_current = -1;
    }

    ReleasePLManipLock();
}

bool 
PlayListManager::
NextIsSame()
{
    GetPLManipLock();
    bool result = false;

    if (m_repeat == REPEAT_CURRENT)
    {
        result = true;
    }
    else
    {
        if (m_order == SHUFFLE_RANDOM)
        {
            if (m_list->CountItems() == 1)
                result = true;
            else
                result = false;
        }
        else
        {
            if (m_repeat == REPEAT_ALL)
            {
                // only same if there is only one song in list
                if (m_list->CountItems() == 1)
                    result = true;
            }
            else
            {
                // only same if current is the last song
                if (m_current == m_list->CountItems() - 1)
                    result = true;
            }
        }
    }
    ReleasePLManipLock();
    return result;
}

void 
PlayListManager::
SetNext(bool bUserAction)
{
    GetPLManipLock();
    int32 count = m_list->CountItems();

    if(count)
    {
        if(!(m_repeat == REPEAT_CURRENT))
        {
            if(SHUFFLE_RANDOM == m_order)
            {
                m_current = (int32) (((double) count * rand()) / (RAND_MAX + 1.0));
            }
            else
            {
                m_current++;

                if( (m_current >= count) && 
                    (m_repeat == REPEAT_ALL || bUserAction))
                {
                    m_current = 0;
                }
            }

            SendInfoToPlayer();
        }
    }
    else
    {
        m_current = -1;
    }

    ReleasePLManipLock();
}

void 
PlayListManager::
SetPrev(bool bUserAction)
{
    GetPLManipLock();
    int32 count = m_list->CountItems();

    if(count)
    {
        if(!(m_repeat == REPEAT_CURRENT))
        {
            if(m_order == SHUFFLE_RANDOM)
            {
                m_current = (int32) (((double) count * rand()) / (RAND_MAX + 1.0));
            }
            else
            {
                m_current--;

                if( (m_current < 0) && 
                    (m_repeat == REPEAT_ALL || bUserAction))
                {
                    m_current = count - 1;
                }
            }

            SendInfoToPlayer();
        }
    }
    else
    {
        m_current = -1;
    }
    ReleasePLManipLock();
}

void 
PlayListManager::
SendInfoToPlayer()
{
    PlayListItem         *pli = GetCurrent();
    PLMGetMediaInfoEvent *gmi = new PLMGetMediaInfoEvent();

    if(gmi && pli)
    {
        gmi->SetPlayListItem(pli);
        m_target->AcceptEvent(gmi);
    }

    if(pli)
    {
        MediaInfoEvent *mie = pli->GetMediaInfo();

        if (mie)
        {
            m_target->AcceptEvent(mie);
        }
        else
        {
            MediaInfoEvent *pMIE = new MediaInfoEvent(pli->URL(), 0);

            m_target->AcceptEvent(pMIE);
        }
    }
}

void 
PlayListManager::
SendShuffleModeToPlayer()
{
    m_target->AcceptEvent(new PlayListShuffleEvent(m_order));
}

void 
PlayListManager::
SendRepeatModeToPlayer()
{
    m_target->AcceptEvent(new PlayListRepeatEvent(m_repeat));
}

void 
PlayListManager::
SetShuffle(ShuffleMode oop)
{
    GetPLManipLock();

    m_order = oop;
    SendShuffleModeToPlayer();

    ReleasePLManipLock();
}

void
PlayListManager::
SetRepeat(RepeatMode rp)
{
    GetPLManipLock();
    m_repeat = rp;
    SendRepeatModeToPlayer();
    ReleasePLManipLock();
}

void 
PlayListManager::
AcceptEvent(Event * e)
{
    if(e)
    {
        switch(e->Type())
        {
            case CMD_PLMSetMediaInfo:
            {
                PLMSetMediaInfoEvent *smi = (PLMSetMediaInfoEvent *) e;

                if(smi->IsComplete())
                {
                    PlayListItem *pItem = smi->GetPlayListItem();

                    // make sure pItem still exists
                    pItem->SetPMIRegistryItem(smi->GetPMIRegistryItem());
                    pItem->SetLMCRegistryItem(smi->GetLMCRegistryItem());
                    pItem->SetMediaInfo(smi->GetMediaInfo());
                    // if pItem is the current item, send out the info posthaste
                    if(pItem == GetCurrent())
                    {
                        SendInfoToPlayer();
                    }
                }

                break;
            }

            default:
                break;
        }
    }
}

Error
PlayListManager::
ToggleShuffle()
{
    SetShuffle((ShuffleMode) ((m_order + 1) % SHUFFLE_LAST_ENUM));
    return kError_NoErr;
}

Error
PlayListManager::
ToggleRepeat()
{
    SetRepeat((RepeatMode) ((m_repeat + 1) % REPEAT_LAST_ENUM));
    return kError_NoErr;
}


PlayListItem* 
PlayListManager::
FirstItem()
{
    PlayListItem* result = NULL;

    GetPLManipLock();

    result = m_list->FirstItem();

    ReleasePLManipLock();

    return result;
}

PlayListItem* 
PlayListManager::
LastItem()
{
    PlayListItem* result = NULL;

    GetPLManipLock();

    result = m_list->LastItem();

    ReleasePLManipLock();

    return result;
}

bool 
PlayListManager::
HasItem(PlayListItem* item)
{
    bool result = false;

    GetPLManipLock();

    result = m_list->HasItem(item);

    ReleasePLManipLock();

    return result;
}

int32 
PlayListManager::
CountItems()
{
    int32 result = 0;

    GetPLManipLock();

    result = m_list->CountItems();

    ReleasePLManipLock();

    return result;
}

PlayListItem*   
PlayListManager::
ItemAt(int32 index)
{
    PlayListItem* result = NULL;

    GetPLManipLock();

    result = m_list->ItemAt(index);

    ReleasePLManipLock();

    return result;
}

int32
PlayListManager::
IndexOf(PlayListItem* item)
{
    int32 result = 0;

    GetPLManipLock();

    result = m_list->IndexOf(item);

    ReleasePLManipLock();

    return result;
}

Error 
PlayListManager::
AddItem(char *url, int32 type)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();

    if(url)
    {
        PlayListItem *item = new PlayListItem();

        if(item)
        {
            item->SetURL(url);
            item->SetType(type);

            m_list->AddItem(item);

            if (m_list->CountItems() == 1)
            {
                m_current = 0; // set current to first
            }

            result = kError_NoErr;

            PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
            pTitle->SetPlayListItem(item);
            m_target->AcceptEvent(pTitle);
            m_target->AcceptEvent(new Event(INFO_PlayListUpdated));
            OutputDebugString("AddItem::AcceptEvent(INFO_PlayListUpdated)\r\n");
        }  
    }

    ReleasePLManipLock();

    return result;
}

Error 
PlayListManager::
AddItem(char *url,int32 type, int32 index)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();

    index = CheckIndex(index);

    if(index >= 0 )
    {
        if(url)
        {
            PlayListItem *item = new PlayListItem();

            if(item)
            {
                item->SetURL(url);
                item->SetType(type);

                m_list->AddItem(item, index);

                if (m_list->CountItems() == 1)
                {
                    m_current = 0; // set current to first
                }

                result = kError_NoErr;

                PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
                pTitle->SetPlayListItem(item);
                m_target->AcceptEvent(pTitle);
                m_target->AcceptEvent(new Event(INFO_PlayListUpdated));
            }  
        }
    }

    ReleasePLManipLock();

    return result;
}

Error 
PlayListManager::
AddItem(PlayListItem* item)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();
    
    if(item)
    {
        m_list->AddItem(item);

        if(m_list->CountItems() == 1)
        {
            m_current = 0;
        }

        PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
        pTitle->SetPlayListItem(item);
        m_target->AcceptEvent(pTitle);
        m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

        result = kError_NoErr;
    }
    
    ReleasePLManipLock();

    return result;
}

Error 
PlayListManager::
AddItem(PlayListItem* item, int32 index)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();

    index = CheckIndex(index);

    if(index >= 0 )
    {
        if(item)
        {
            m_list->AddItem(item, index);

            if(m_list->CountItems() == 1)
            {
                m_current = 0;
            }

            PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
            pTitle->SetPlayListItem(item);
            m_target->AcceptEvent(pTitle);
            m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

            result = kError_NoErr;
        }
    }

    ReleasePLManipLock();

    return result;
}

Error 
PlayListManager::
AddList(List<PlayListItem*>* items)
{
    Error result = kError_UnknownErr;
    int   i;

    GetPLManipLock();
    
    if(items)
    {
        m_list->AddList(*items, CountItems());

        if(m_list->CountItems() == 1)
        {
            m_current = 0;
        }


        for(i = 0; i < items->CountItems(); i++)
        {
            PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
            pTitle->SetPlayListItem(items->ItemAt(i));
            m_target->AcceptEvent(pTitle);
        }
        m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

        result = kError_NoErr;
    }
    
    ReleasePLManipLock();

    return result;
}

Error 
PlayListManager::
AddList(List<PlayListItem*>* items, int32 index)
{
    Error result = kError_UnknownErr;
    int   i;

    GetPLManipLock();

    index = CheckIndex(index);

    if(index >= 0 )
    {
        if(items)
        {
            m_list->AddList(*items, index);

            if(m_list->CountItems() == 1)
            {
                m_current = 0;
            }

            for(i = 0; i < items->CountItems(); i++)
            {
               PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
               pTitle->SetPlayListItem(items->ItemAt(i));
               m_target->AcceptEvent(pTitle);
            }
            m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

            result = kError_NoErr;
        }
    }

    ReleasePLManipLock();

    return result;
}

Error           
PlayListManager::
RemoveItem(PlayListItem* item)
{
    Error result = kError_UnknownErr;

    if(RemoveItem(IndexOf(item)))
    {
        result = kError_NoErr;
    }

    return result;
}

PlayListItem*   
PlayListManager::
RemoveItem(int32 index)
{
    PlayListItem* result = NULL;

    GetPLManipLock();

    index = CheckIndex(index);

    if(index >= 0 )
    {
        result = m_list->RemoveItem(index);
    }

    m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

    ReleasePLManipLock();

    return result;
}

Error           
PlayListManager::
RemoveItems(int32 index, int32 count)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();

    index = CheckIndex(index);

    if(index >= 0 )
    {
        if(count > (CountItems() - index))
            count = CountItems() - index;

        while(count--)
        {
		    m_list->RemoveItem(count);
        }

        m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

        result = kError_NoErr;
    }

    ReleasePLManipLock();

    return result;
}

Error           
PlayListManager::
RemoveList(List<PlayListItem*>* items)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();

    if(items)
    {
        m_list->RemoveList(*items);

        result = kError_NoErr;

        m_target->AcceptEvent(new Event(INFO_PlayListUpdated));
    }

    ReleasePLManipLock();

    return result;
}

Error           
PlayListManager::
MoveList(List<PlayListItem*>* items, int32 index)
{
    Error result = kError_UnknownErr;

    GetPLManipLock();

    if(items)
    {
        index = CheckIndex(index);

        if(index >= 0 )
        {
            m_list->RemoveList(*items);

            m_list->AddList(*items, index);

            if(m_list->CountItems() == 1)
            {
                m_current = 0;
            }

            m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

            result = kError_NoErr;
        }
    }

    ReleasePLManipLock();

    return result;
}

void 
PlayListManager::
MakeEmpty()
{
    GetPLManipLock();

    m_list->DeleteAll();

    m_current = -1;
    m_skipNum = 0;

    //m_target->AcceptEvent(new MediaInfoEvent());
    m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

    ReleasePLManipLock();
}

bool 
PlayListManager::
IsEmpty()
{
    bool result = false;

    GetPLManipLock();

    result = m_list->IsEmpty();

    ReleasePLManipLock();

    return result;
}

void 
PlayListManager::
DoForEach(bool (*func)(PlayListItem*))
{
    GetPLManipLock();

    int32 count = CountItems();

	for(int32 i = 0; i < count; i++)
    {
		if((*func)((PlayListItem*) m_list->ItemAt(i)))
        {
			break;
        }
    }

    ReleasePLManipLock();
}

void 
PlayListManager::
DoForEach(bool (*func)(PlayListItem*, void*), void* cookie)
{
    GetPLManipLock();

    int32 count = CountItems();

	for(int32 i = 0; i < count; i++)
    {
		if((*func)((PlayListItem*) m_list->ItemAt(i), cookie))
        {
			break;
        }
    }

    ReleasePLManipLock();
}

const 
PlayListItem** 
PlayListManager::
Items() const
{
    return (const PlayListItem**)(m_list->Items());
}


Error 
PlayListManager::
RemoveAll()
{
   GetPLManipLock();

   m_list->RemoveAll();
   m_current = -1;
   m_skipNum = 0;

   //m_target->AcceptEvent(new MediaInfoEvent());
   m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

   ReleasePLManipLock();

   return kError_NoErr;
}

inline 
int32 
PlayListManager::
CheckIndex(int32 index)
{
	// If we're dealing with a bogus index then set it to -1.
	if(index < 0 || index >= CountItems())
    {
		index = -1;
    }

	return index;
}

// Note: This function does not clear the list ahead of time!
Error PlayListManager::
ExpandM3U(char *szM3UFile, List<char *> &MP3List)
{
	FILE  *fpFile;
	char  *szLine, *szMP3 = NULL;
	int    iIndex;

	fpFile = fopen(szM3UFile, "r");
	if (fpFile == NULL)
        return kError_FileNotFound;

	szLine = new char[iMaxFileNameLen];
	for(;;)
    {
		 if (fgets(szLine, iMaxFileNameLen - 1, fpFile) == NULL)
			 break;

		 for(iIndex = strlen(szLine) -1; iIndex >= 0; iIndex--)
			 if (szLine[iIndex] == '\n' || 
				 szLine[iIndex] == '\r' ||
				 szLine[iIndex] == ' ')
				szLine[iIndex] = 0;
			 else
				break;

		 if (strlen(szLine))
		 {
			 szMP3 = new char[strlen(szLine) + 1];
			 strcpy(szMP3, szLine);
			 MP3List.AddItem(szMP3);
			 szMP3 = NULL;
		 }
    }
	delete szLine;
	fclose(fpFile);

	return kError_NoErr;
}