/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 EMusic.com

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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <assert.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

#include "config.h"

#include "playlist.h"
#include "errors.h"
#include "mutex.h"
#include "thread.h"
#include "metadata.h"
#include "registrar.h"
#include "event.h"
#include "eventdata.h"
#include "musicbrowser.h"


// Function object used for sorting PlaylistItems in PlaylistManager
bool PlaylistItemSort::operator() (PlaylistItem* item1, 
                                   PlaylistItem* item2) const
{
    bool result = true;

    assert(item1);
    assert(item2);

    if(item1 && item2)
    {
        MetaData m1 = item1->GetMetaData();
        MetaData m2 = item2->GetMetaData();

        switch(m_sortKey)
        {
            case kPlaylistSortKey_Artist:
            {
                result = m1.Artist() < m2.Artist();
                break;
            }

            case kPlaylistSortKey_Album:
            {
                result = m1.Album() < m2.Album();
                break;
            }

            case kPlaylistSortKey_Title:
            {
                result = m1.Title() < m2.Title();
                break;
            }

            case kPlaylistSortKey_Year:
            {
                result = m1.Year() < m2.Year();
                break;
            }

            case kPlaylistSortKey_Track:
            {
                result = m1.Track() < m2.Track();
                break;
            }

            case kPlaylistSortKey_Genre:
            {
                result = m1.Genre() < m2.Genre();
                break;
            }

            case kPlaylistSortKey_Time:
            {
                result = m1.Time() < m2.Time();
                break;
            }

            case kPlaylistSortKey_Location:
            {
                result = item1->URL() < item2->URL();
                break;
            }

            default:
            {
                cerr << "Whoa! Why are we here?" << endl;
                break;
            }

        }
    }

    return result;
}


// Implementation of the PlaylistManager Class
PlaylistManager::PlaylistManager(FAContext* context)
{
    m_context = context;
    m_activeList = &m_masterList;
    m_playlistKey = kPlaylistKey_MasterPlaylist;
    m_current = kInvalidIndex;
    m_shuffle = false;
    m_repeatMode = kPlaylistMode_RepeatNone;
    m_sortKey = kPlaylistSortKey_Random;

    m_context->prefs->GetPlaylistShuffle(&m_shuffle);
    m_context->prefs->GetPlaylistRepeat((int32*)&m_repeatMode);

    srand( (unsigned)time( NULL ) );

    Registrar registrar;

    registrar.SetSubDir("plugins");
    registrar.SetSearchString("*.plf");
    registrar.InitializeRegistry(&m_playlistRegistry, context->prefs);

    registrar.SetSearchString("*.mdf");
    registrar.InitializeRegistry(&m_metadataRegistry, context->prefs);

    registrar.SetSearchString("*.ppp");
    registrar.InitializeRegistry(&m_portableRegistry, context->prefs);

    const RegistryItem* module = NULL;
    MetaDataFormat* mdf = NULL;
    int32 i = 0;

    while((module = m_metadataRegistry.GetItem(i++)))
    {
        mdf = (MetaDataFormat*) module->InitFunction()(m_context);

        if(mdf)
        {
            m_metadataFormats.push_back(mdf);
        }
    }

    i = 0;
    PlaylistFormat* plf = NULL;

    while((module = m_playlistRegistry.GetItem(i++)))
    {
        plf = (PlaylistFormat*) module->InitFunction()(m_context);

        if(plf)
        {
            PlaylistFormatInfo plfi;

            uint32 index = 0;

            // error != kError_NoMoreFormats
            while(IsntError(plf->GetSupportedFormats(&plfi, index++)))
            {
                plfi.SetRef(plf);
                m_playlistFormats.push_back(new PlaylistFormatInfo(plfi));
            }
        }
    }

    i = 0;
    PortableDevice* pd = NULL;

    while((module = m_portableRegistry.GetItem(i++)))
    {
        pd = (PortableDevice*) module->InitFunction()(m_context);

        if(pd)
        {
            DeviceInfo di;

            uint32 index = 0;
            // error != kError_NoMoreDevices
            while(IsntError(pd->GetSupportedDevices(&di, index++)))
            {
                di.SetRef(pd);
                m_portablePlayers.push_back(new DeviceInfo(di));
            }
        }
    }
}

PlaylistManager::~PlaylistManager()
{
    uint32 index = 0;
    uint32 size = 0;
    PlaylistItem* item = NULL;
    //uint32 count = 0;

    size = m_masterList.size();

    for(index = 0; index < size; index++)
    {
        item = m_masterList[index];

        if(item)
        {
            // if the metadata thread is still accessing this item
            // we don't wanna delete the item  out from under it.
            // instead we set a flag and let the metadata thread
            // clean up when it returns.
            if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
            {
                item->SetState(kPlaylistItemState_Delete);  
            }
            else
            {
                delete item;
            }
        }
    }
    
    size = m_externalList.size();

    for(index = 0; index < size; index++)
    {
        item = m_externalList[index];

        if(item)
        {
            // if the metadata thread is still accessing this item
            // we don't wanna delete the item  out from under it.
            // instead we set a flag and let the metadata thread
            // clean up when it returns.
            if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
            {
                item->SetState(kPlaylistItemState_Delete);  
            }
            else
            {
                delete item;
            }
        }
    }

    size = m_portableList.size();

    for(index = 0; index < size; index++)
    {
        item = m_portableList[index];

        if(item)
        {
            // if the metadata thread is still accessing this item
            // we don't wanna delete the item  out from under it.
            // instead we set a flag and let the metadata thread
            // clean up when it returns.
            if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
            {
                item->SetState(kPlaylistItemState_Delete);  
            }
            else
            {
                delete item;
            }
        }
    }

    size = m_playlistFormats.size();

    for(index = 0; index < size; index++)
    {
        delete m_playlistFormats[index]->GetRef();
        delete m_playlistFormats[index];
    }

    size = m_portablePlayers.size();

    for(index = 0; index < size; index++)
    {
        delete m_portablePlayers[index]->GetRef();
        delete m_portablePlayers[index];
    }

    m_context->prefs->SetPlaylistShuffle(m_shuffle);
    m_context->prefs->SetPlaylistRepeat(m_repeatMode);
}


// Playlist actions
Error PlaylistManager::SetCurrentItem(PlaylistItem* item)
{
    return SetCurrentIndex(IndexOf(item));
}

const PlaylistItem*  PlaylistManager::GetCurrentItem()
{
    PlaylistItem* result = NULL;
    m_mutex.Acquire();
    
    if(m_masterList.size())
    {
        if(m_shuffle)
            result = m_shuffleList[m_current];
        else
            result = m_masterList[m_current];
    }
   
    m_mutex.Release();
    return result;
}


Error PlaylistManager::SetCurrentIndex(uint32 index)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    index = CheckIndex(index);

    if(index != kInvalidIndex)
    {
        InternalSetCurrentIndex(index);
        
        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

void PlaylistManager::InternalSetCurrentIndex(uint32 index)
{
    m_current = index;
    m_context->target->AcceptEvent(new PlaylistCurrentItemInfoEvent(GetCurrentItem()));
}

uint32 PlaylistManager::GetCurrentIndex() const
{
    uint32 result = kInvalidIndex;

    if(m_masterList.size())
        result = m_current;

    return m_current;
}


Error PlaylistManager::GotoNextItem(bool userAction)
{
    m_mutex.Acquire();

    Error result = kError_NoErr;
    uint32 count = m_masterList.size();
    uint32 index = m_current;
    
    if(count)
    {
        if(!(kPlaylistMode_RepeatOne == m_repeatMode) || userAction)
        {
            index++;

            if( (index >= count) && 
                (m_repeatMode == kPlaylistMode_RepeatAll || userAction))
            {
                index = 0;

                if(m_shuffle)
                {
                    random_shuffle(m_shuffleList.begin(), m_shuffleList.end());
                }
            }
            else if(index >= count)
            {
                index = m_current;
            }
        }

        InternalSetCurrentIndex(index);
    }
    else
    {
        m_current = kInvalidIndex;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::GotoPreviousItem(bool userAction)
{
    m_mutex.Acquire();

    Error result = kError_NoErr;
    uint32 count = m_masterList.size();
    uint32 index = m_current;
    
    if(count)
    {
        if(!(kPlaylistMode_RepeatOne == m_repeatMode) || userAction)
        {
            if( (index == 0) && 
                (m_repeatMode == kPlaylistMode_RepeatAll || userAction))
            {
                index = count - 1;

                if(m_shuffle)
                {
                    random_shuffle(m_shuffleList.begin(), m_shuffleList.end());
                }
            }
            else if(index != 0)
            {
                index--;
            }
        }

        InternalSetCurrentIndex(index);
    }
    else
    {
        m_current = kInvalidIndex;
    }

    m_mutex.Release();

    return result;
}

bool PlaylistManager::HasAnotherItem()
{
    m_mutex.Acquire();
    bool result = false;

    if(m_repeatMode == kPlaylistMode_RepeatOne || 
        m_repeatMode == kPlaylistMode_RepeatAll)
    {
        result = true;
    }
    else
    {
        result = (m_current < m_masterList.size() - 1);
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::SetShuffleMode(bool shuffle)
{
    m_mutex.Acquire();

    m_shuffle = shuffle;

    if(m_shuffle)
    {
        random_shuffle(m_shuffleList.begin(), m_shuffleList.end());
    }

    m_context->target->AcceptEvent(new PlaylistShuffleEvent(m_shuffle));

    m_mutex.Release();

    return kError_NoErr;
}

Error PlaylistManager::SetRepeatMode(RepeatMode mode)
{
    m_repeatMode = mode;
    m_context->target->AcceptEvent(new PlaylistRepeatEvent(m_repeatMode));
    return kError_NoErr;
}

Error PlaylistManager::ToggleRepeatMode()
{
    Error result = kError_NoErr;

    if(m_repeatMode == kPlaylistMode_RepeatNone)
        result = SetRepeatMode(kPlaylistMode_RepeatOne);
    else if(m_repeatMode == kPlaylistMode_RepeatOne)
        result = SetRepeatMode(kPlaylistMode_RepeatAll);
    else if(m_repeatMode == kPlaylistMode_RepeatAll)
        result = SetRepeatMode(kPlaylistMode_RepeatNone);

    return result;
}

Error PlaylistManager::ToggleShuffleMode()
{
    return SetShuffleMode(!m_shuffle);
}


// Functions for adding items to playlist       
Error PlaylistManager::AddItem(const char* url)
{
    Error result = kError_InvalidParam;

    assert(url);

    if(url)
    {
        result = ReadPlaylist(url);

        if(IsError(result))
        {
            result = kError_OutOfMemory;

            PlaylistItem* item = new PlaylistItem(url);

            if(item)
            {
                result = AddItem(item, true);
            }
        }
    }

    return result;
}

Error PlaylistManager::AddItem(const char* url, uint32 index)
{
    Error result = kError_InvalidParam;

    assert(url);

    if(url)
    {
        result = ReadPlaylist(url);

        if(IsError(result))
        {
            result = kError_OutOfMemory;

            PlaylistItem* item = new PlaylistItem(url);

            if(item)
            {
                result = AddItem(item, index, true);
            }
        }
    }

    return result;
}

Error PlaylistManager::AddItem(PlaylistItem* item, bool queryForMetaData)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(item);

    if(item)
    {
        m_activeList->push_back(item);

        if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
        {
            AddItemToShuffleList(item);

            if(m_current == kInvalidIndex && m_activeList->size())
                InternalSetCurrentIndex(0);
        }

        if(queryForMetaData)
            RetrieveMetaData(item);
     
        m_context->target->AcceptEvent(new PlaylistItemAddedEvent(item));

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::AddItem(PlaylistItem* item, uint32 index, bool queryForMetaData)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(item);

    if(index > m_activeList->size())
        index = kInvalidIndex;

    //index = CheckIndex(index);

    if(item && index != kInvalidIndex)
    {
        m_activeList->insert(&(*m_activeList)[index],item);

        if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
        {
            AddItemToShuffleList(item);

            if(m_current == kInvalidIndex && m_activeList->size())
                InternalSetCurrentIndex(0);
        }

        if(queryForMetaData)
            RetrieveMetaData(item);

        m_context->target->AcceptEvent(new PlaylistItemAddedEvent(item));

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::AddItems(vector<PlaylistItem*>* list, bool queryForMetaData)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(list);

    if(list)
    {
        m_activeList->insert(m_activeList->end(),
                             list->begin(), 
                             list->end());

        if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
        {
            AddItemsToShuffleList(list);

            if(m_current == kInvalidIndex && m_activeList->size())
                InternalSetCurrentIndex(0);
        }

        // we need our own vector so user can delete theirs
        vector<PlaylistItem*>* items = new vector<PlaylistItem*>;

        if(items)
        {
            items->insert(items->end(), list->begin(), list->end());

            if(queryForMetaData)
                RetrieveMetaData(items);
        }

        vector<PlaylistItem *>::iterator i = list->begin();
        for (; i != list->end(); i++)
            m_context->target->AcceptEvent(new PlaylistItemAddedEvent(*i));

        result = kError_NoErr;
    }

    m_mutex.Release();

    return result;
}



Error PlaylistManager::AddItems(vector<PlaylistItem*>* list, uint32 index, bool queryForMetaData)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(list);

    if(index > m_activeList->size())
        index = kInvalidIndex;

    //index = CheckIndex(index);

    if(list && index != kInvalidIndex)
    {
        m_activeList->insert(&(*m_activeList)[index],
                             list->begin(), 
                             list->end());

        if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
        {
            AddItemsToShuffleList(list);

            if(m_current == kInvalidIndex && m_activeList->size())
                InternalSetCurrentIndex(0);
        }

        // we need our own vector so user can delete theirs
        vector<PlaylistItem*>* items = new vector<PlaylistItem*>;

        if(items)
        {
            items->insert(items->end(), list->begin(), list->end());

            if(queryForMetaData)
                RetrieveMetaData(items);
        }

        vector<PlaylistItem *>::iterator i = list->begin();
        for (; i != list->end(); i++)
            m_context->target->AcceptEvent(new PlaylistItemAddedEvent(*i));

        result = kError_NoErr;
    }

    m_mutex.Release();

    return result;
}


// Functions for removing items from playlist
Error PlaylistManager::RemoveItem(PlaylistItem* item)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(item);

    if(item)
    {
        result = RemoveItem(IndexOf(item));
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::RemoveItem(uint32 index)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    index = CheckIndex(index);

    if(index != kInvalidIndex)
    {
        PlaylistItem* item = (*m_activeList)[index];

        m_activeList->erase(&(*m_activeList)[index]);

        if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
        {
            int32 shuffleIndex = InternalIndexOf(&m_shuffleList, item);

            m_shuffleList.erase(&m_shuffleList[shuffleIndex]);

            if(!m_activeList->size())
            {
               m_current = kInvalidIndex;
            }
        }

        // if the metadata thread is still accessing this item
        // we don't wanna delete the item  out from under it.
        // instead we set a flag and let the metadata thread
        // clean up when it returns.
        if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
        {
            item->SetState(kPlaylistItemState_Delete);  
        }
        else
        {
            delete item;
        }

        m_context->target->AcceptEvent(new PlaylistItemRemovedEvent(item));

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::RemoveItems(uint32 index, uint32 count)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    index = CheckIndex(index);

    if(index != kInvalidIndex)
    {
        for(uint32 i = 0; i < count; i++)
        {
            PlaylistItem* item = (*m_activeList)[index + i];

            if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
            {
                int32 shuffleIndex = InternalIndexOf(&m_shuffleList, item);

                m_shuffleList.erase(&m_shuffleList[shuffleIndex]);

                if(!m_activeList->size())
                {
                   m_current = kInvalidIndex;
                }
            }

            // if the metadata thread is still accessing this item
            // we don't wanna delete the item  out from under it.
            // instead we set a flag and let the metadata thread
            // clean up when it returns.
            if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
            {
                item->SetState(kPlaylistItemState_Delete);  
            }
            else
            {
                delete item;
            }

            m_context->target->AcceptEvent(new PlaylistItemRemovedEvent(item));
        }

        m_activeList->erase(&(*m_activeList)[index], &(*m_activeList)[index + count]);

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::RemoveItems(vector<PlaylistItem*>* items)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(items);

    if(items)
    {
        uint32 index = 0;
        uint32 size = 0;
        PlaylistItem* item = NULL;

        size = items->size();

        for(index = 0; index < size; index++)
        {
            item = (*items)[index];

            if(item)
            {
                m_activeList->erase(&(*m_activeList)[IndexOf(item)]);

                if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
                {
                    int32 shuffleIndex = InternalIndexOf(&m_shuffleList, item);

                    m_shuffleList.erase(&m_shuffleList[shuffleIndex]);

                    if(!m_activeList->size())
                    {
                       m_current = kInvalidIndex;
                    }
                }

                // if the metadata thread is still accessing this item
                // we don't wanna delete the item  out from under it.
                // instead we set a flag and let the metadata thread
                // clean up when it returns.
                if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
                {
                    item->SetState(kPlaylistItemState_Delete);  
                }
                else
                {
                    delete item;
                }

                m_context->target->AcceptEvent(new PlaylistItemRemovedEvent(item));

                result = kError_NoErr;
            }
        }  
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::RemoveAll()
{
    Error result = kError_InvalidParam;
    uint32 index = 0;
    uint32 size = 0;
    PlaylistItem* item = NULL;
    m_mutex.Acquire();

    size = m_activeList->size();

    for(index = 0; index < size; index++)
    {
        item = (*m_activeList)[index];

        if(item)
        {            
            // if the metadata thread is still accessing this item
            // we don't wanna delete the item  out from under it.
            // instead we set a flag and let the metadata thread
            // clean up when it returns.
            if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
            {
                item->SetState(kPlaylistItemState_Delete);  
            }
            else
            {
                delete item;
            }          
            
            m_context->target->AcceptEvent(new PlaylistItemRemovedEvent(item));
        }
    }  

    m_activeList->clear();

    if(kPlaylistKey_MasterPlaylist == GetActivePlaylist())
    {
        m_shuffleList.clear();
        m_current = kInvalidIndex;
    }

    result = kError_NoErr;

    m_mutex.Release();
    return result;
}


// Functions for moving items around
Error PlaylistManager::SwapItems(PlaylistItem* item1, PlaylistItem* item2)
{
    return SwapItems(IndexOf(item1), IndexOf(item2));
}

Error PlaylistManager::SwapItems(uint32 index1, uint32 index2)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    index1 = CheckIndex(index1);
    index2 = CheckIndex(index2);

    if(index1 != kInvalidIndex && index2 != kInvalidIndex)
    {
        PlaylistItem* temp;

        temp = (*m_activeList)[index1];
        (*m_activeList)[index1] = (*m_activeList)[index2];
        (*m_activeList)[index2] = temp;

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::MoveItem(PlaylistItem* item, uint32 index)
{
    return MoveItem(IndexOf(item), index);
}

Error PlaylistManager::MoveItem(uint32 oldIndex, uint32 newIndex)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    oldIndex = CheckIndex(oldIndex);
    newIndex = CheckIndex(newIndex);

    if(oldIndex != kInvalidIndex && newIndex != kInvalidIndex)
    {
        PlaylistItem* item = (*m_activeList)[oldIndex];

        m_activeList->erase(&(*m_activeList)[oldIndex]);

        if(newIndex > m_activeList->size())
            newIndex = m_activeList->size();

        m_activeList->insert(&(*m_activeList)[newIndex],item);
        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::MoveItems(vector<PlaylistItem*>* items, uint32 index)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(items);

    index = CheckIndex(index);

    if(items && index != kInvalidIndex)
    {
        uint32 size = 0;
        PlaylistItem* item = NULL;

        size = items->size();

        for(uint32 i = 0; i < size; i++)
        {
            item = (*items)[i];

            if(item)
            {
                cout << "Erasing " << item->URL() << endl;
                m_activeList->erase(&(*m_activeList)[IndexOf(item)]);
            }
        }  

        if(index > m_activeList->size())
            index = m_activeList->size();

        m_activeList->insert(&(*m_activeList)[index],
                             items->begin(), 
                             items->end());

        result = kError_NoErr;
    }

    m_mutex.Release();
    return result;
}


// Functions for sorting
Error PlaylistManager::Sort(PlaylistSortKey key, PlaylistSortType type)
{
    Error result = kError_InvalidParam;

    if(key >= kPlaylistSortKey_FirstKey && key < kPlaylistSortKey_LastKey)
    {
        if(type == PlaylistSortType_Ascending)
            sort(m_activeList->begin(), m_activeList->end(), PlaylistItemSort(key));
        else
            sort(m_activeList->begin(), m_activeList->end(), not2(PlaylistItemSort(key)));
    
        m_sortKey = key;
        result = kError_NoErr;
    }
    else if(key == kPlaylistSortKey_Random)
    {
        random_shuffle(m_activeList->begin(), m_activeList->end());
        m_sortKey = key;
        result = kError_NoErr;
    }

    return result;
}

PlaylistSortKey PlaylistManager::GetPlaylistSortKey() const
{
    return m_sortKey;
}


// Which playlist are we dealing with for purposes of editing:
// 1) Master Playlist - list of songs to play
// 2) Secondary Playlist - a playlist that we want to edit
//      - External playlist
//      - Portable playlist

Error PlaylistManager::SetActivePlaylist(PlaylistKey key)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    if(key >= kPlaylistKey_FirstKey && key < kPlaylistKey_LastKey)
    {
        m_playlistKey = key;
        result = kError_NoErr;

        switch(key)
        {
            case kPlaylistKey_MasterPlaylist:
            {
                m_activeList = &m_masterList;
                break;
            }

            case kPlaylistKey_ExternalPlaylist:
            {
                m_activeList = &m_externalList;
                break;
            }
    

            case kPlaylistKey_PortablePlaylist:
            {
                m_activeList = &m_portableList;
                break;
            }

            default:
            {
                result = kError_InvalidParam;
            }
        }
    }

    m_mutex.Release();
    return result;
}

PlaylistKey PlaylistManager::GetActivePlaylist() const
{
    return m_playlistKey;  
}

Error PlaylistManager::SetExternalPlaylist(char* url)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(url);

    if(url)
    {
        // first delete old playlist
        uint32 index = 0;
        uint32 numItems = 0;
        PlaylistItem* item = NULL;

        numItems = m_externalList.size();

        for(index = 0; index < numItems; index++)
        {
            item = m_externalList[index];

            if(item)
            {
                // if the metadata thread is still accessing this item
                // we don't wanna delete the item  out from under it.
                // instead we set a flag and let the metadata thread
                // clean up when it returns.
                if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
                {
                    item->SetState(kPlaylistItemState_Delete);  
                }
                else
                {
                    delete item;
                }
            }
        }

        m_externalList.clear();

        result = ReadPlaylist(url, &m_externalList);

        vector<PlaylistItem*>* items = new vector<PlaylistItem*>;

        items->insert(items->end(), 
                      m_externalList.begin(), 
                      m_externalList.end());

        RetrieveMetaData(items);
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::GetExternalPlaylist(char* url, uint32* length)
{
    Error result = kError_InvalidParam;
    m_mutex.Acquire();

    assert(url);
    assert(length);

    if(url && length)
    {
        if(*length >= m_externalPlaylist.size() + 1)
        {
            strcpy(url, m_externalPlaylist.c_str());
            result = kError_NoErr;
        }
        else
        {
            result = kError_BufferTooSmall;
        }

        *length = m_externalPlaylist.size() + 1;
    }

    m_mutex.Release();
    return result;
}

Error PlaylistManager::SetPortablePlaylist(DeviceInfo* device, 
                                           PLMCallBackFunction function,
                                           void* cookie)
{
    Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        result = ReadPortablePlaylist(device, function, cookie);
        m_portableDevice = *device;
    }

    return result;
}

Error PlaylistManager::GetPortablePlaylist(DeviceInfo* device)
{
    Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        result = kError_NoErr;
        *device = m_portableDevice;
    }

    return result;
}


// External playlist support
Error PlaylistManager::GetSupportedPlaylistFormats(PlaylistFormatInfo* format, 
                                                   uint32 index)
{
    Error result = kError_InvalidParam;

    assert(format);

    if(format)
    {
        result = kError_NoMoreFormats;

        uint32 numFormats = m_playlistFormats.size();

        if(index < numFormats)
        {
            result = kError_NoErr;

            *format = *m_playlistFormats[index];
        }
    }

    return result;
}

Error PlaylistManager::ReadPlaylist(const char* url, 
                                    vector<PlaylistItem*>* items,
                                    PLMCallBackFunction function,
                                    void* cookie)
{
    Error result = kError_InvalidParam;

    assert(url);

    if(url)
    {
        // find a suitable plugin
        result = kError_FormatNotSupported;
        char* extension = strrchr(url, '.');

        if(extension)
        {
            extension++;

            uint32 numFormats = m_playlistFormats.size();

            for(uint32 index = 0; index < numFormats; index++)
            {
                PlaylistFormatInfo* format = m_playlistFormats[index];
                
                if(!strcasecmp(extension, format->GetExtension()))
                {
                    bool addToActiveList = false;

                    if(!items)
                    {
                        items = new vector<PlaylistItem*>;
                        addToActiveList = true;
                    }

                    result = format->GetRef()->ReadPlaylist(url, 
                                                            items, 
                                                            function, 
                                                            cookie);

                    if(addToActiveList)
                    {
                        AddItems(items);
                        delete items;
                    }

                    break;
                }
            }
        }
    }

    return result;
}

Error PlaylistManager::WritePlaylist(const char* url, 
                                     PlaylistFormatInfo* format, 
                                     vector<PlaylistItem*>* items,
                                     PLMCallBackFunction function,
                                     void* cookie)
{
    Error result = kError_InvalidParam;

    assert(url);
    assert(format);

    if(url && format)
    {
        if(!items)
        {
            items = m_activeList;
        }

        result = format->GetRef()->WritePlaylist(url, 
                                                 format, 
                                                 items, 
                                                 function, 
                                                 cookie);
    }

    return result;
}

Error PlaylistManager::WritePlaylist(const char* url, 
                                     vector<PlaylistItem*>* items,
                                     PLMCallBackFunction function,
                                     void* cookie)
{
    Error result = kError_InvalidParam;

    assert(url);

    if(url)
    {
        // find a suitable plugin
        result = kError_FormatNotSupported;
        char* extension = strrchr(url, '.');

        if(extension)
        {
            extension++;

            uint32 numFormats = m_playlistFormats.size();

            for(uint32 index = 0; index < numFormats; index++)
            {
                PlaylistFormatInfo* format = m_playlistFormats[index];
                
                if(!strcasecmp(extension, format->GetExtension()))
                {
                    if(!items)
                    {
                        items = m_activeList;
                    }

                    result = format->GetRef()->WritePlaylist(url, 
                                                             format, 
                                                             items, 
                                                             function, 
                                                             cookie);
                    break;
                }
            }
        }
    }

    return result;
}


// Portable player communication
Error PlaylistManager::GetSupportedPortables(DeviceInfo* device, uint32 index)
{
    Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        result = kError_NoMoreDevices;

        uint32 numDevices = m_portablePlayers.size();

        if(index < numDevices)
        {
            result = kError_NoErr;

            *device = *m_portablePlayers[index];
        }
    }

    return result;
}

bool PlaylistManager::IsPortableAvailable(DeviceInfo* device)
{
    bool result = false;

    assert(device);

    if(device)
    {
        result = device->GetRef()->IsDeviceAvailable(device);
    }

    return result;
}

Error PlaylistManager::GetDeviceInfo(DeviceInfo* device)
{
   Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        result = device->GetRef()->GetDeviceInfo(device);
    }

    return result;
}

Error PlaylistManager::InitializeDevice(DeviceInfo* device, 
                                        PLMCallBackFunction function,
                                        void* cookie)
{
    Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        result = device->GetRef()->InitializeDevice(device, function, cookie);
    }

    return result;

}

Error PlaylistManager::ReadPortablePlaylist(DeviceInfo* device,
                                            PLMCallBackFunction function,
                                            void* cookie)
{
    Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        // first delete old playlist
        uint32 index = 0;
        uint32 numItems = 0;
        PlaylistItem* item = NULL;

        numItems = m_portableList.size();

        for(index = 0; index < numItems; index++)
        {
            item = m_portableList[index];

            if(item)
            {
                // if the metadata thread is still accessing this item
                // we don't wanna delete the item  out from under it.
                // instead we set a flag and let the metadata thread
                // clean up when it returns.
                if(item->GetState() == kPlaylistItemState_RetrievingMetaData)
                {
                    item->SetState(kPlaylistItemState_Delete);  
                }
                else
                {
                    delete item;
                }
            }
        }

        m_portableList.clear();

        result = device->GetRef()->ReadPlaylist(device, 
                                                &m_portableList,
                                                function, 
                                                cookie);
    }

    return result;
}

Error PlaylistManager::SyncPortablePlaylist(DeviceInfo* device,
                                            PLMCallBackFunction function,
                                            void* cookie)
{
    Error result = kError_InvalidParam;

    assert(device);

    if(device)
    {
        result = device->GetRef()->WritePlaylist(device, 
                                                &m_portableList,
                                                function, 
                                                cookie);
    }

    return result;
}

Error PlaylistManager::DownloadItemFromPortable(DeviceInfo* device,
                                                PlaylistItem* item,
                                                const char* url,
                                                PLMCallBackFunction function,
                                                void* cookie)
{
    Error result = kError_InvalidParam;

    assert(device);
    assert(item);
    assert(url);

    if(device && item && url)
    {
        result = device->GetRef()->DownloadSong(device, 
                                                item,
                                                url,
                                                function, 
                                                cookie);
    }

    return result;
}

// Utility Functions
bool PlaylistManager::IsEmpty()
{
    bool result;
    m_mutex.Acquire();

    result = m_activeList->empty();

    m_mutex.Release();
    return result;
}

uint32 PlaylistManager::CountItems()
{
    uint32 result;
    m_mutex.Acquire();

    result = m_activeList->size();

    m_mutex.Release();
    return result;
}

PlaylistItem* PlaylistManager::ItemAt(uint32 index)
{
    PlaylistItem* result = NULL;
    m_mutex.Acquire();
    
    index = CheckIndex(index);

    if(index != kInvalidIndex)
    {
        result = (*m_activeList)[index];
    }
    
    m_mutex.Release();
    return result;
}

uint32 PlaylistManager::IndexOf(PlaylistItem* item)
{
    return InternalIndexOf(m_activeList, item);
}

uint32 PlaylistManager::InternalIndexOf(vector<PlaylistItem*>* list,
                                        PlaylistItem* item)
{
    uint32 result = kInvalidIndex;
    uint32 index = 0;
    uint32 size = 0;

    assert(list);
    assert(item);

    if(list && item)
    {
        size = list->size();

        for(index = 0; index < size; index++)
        {
            if(item == (*list)[index])
            {
                result = index;
                break;
            }
        }
    }
    
    return result;
}

bool PlaylistManager::HasItem(PlaylistItem* item)
{
    return (IndexOf(item) != kInvalidIndex);
}

// Internal functions

inline uint32 PlaylistManager::CheckIndex(uint32 index)
{
	// If we're dealing with a bogus index then set it to -1.
	if(index >= CountItems())
    {
		index = kInvalidIndex;
    }

	return index;
}

void PlaylistManager::AddItemToShuffleList(PlaylistItem* item)
{
    m_shuffleList.push_back(item);
}

void PlaylistManager::AddItemsToShuffleList(vector<PlaylistItem*>* list)
{
    random_shuffle(list->begin(), list->end());

    m_shuffleList.insert(m_shuffleList.end(),
                         list->begin(), 
                         list->end());
}

void
PlaylistManager::
MetaDataThreadFunction(vector<PlaylistItem*>* list)
{
    assert(list);

    if(list)
    {
        uint32 index = 0;
        uint32 numItems = 0;
        PlaylistItem* item = NULL;

        numItems = list->size();

        for(index = 0; index < numItems; index++)
        {
            item = (*list)[index];

            if(item)
            {
                MetaData metadata;
                MetaData* dbData = NULL;

                // is there any metadata stored for this item in the music db?
                dbData = m_context->browser->ReadMetaDataFromDatabase(item->URL().c_str());

                if(dbData)
                {
                    metadata = *dbData;
                    delete dbData;
                }
                else // if not look to see if the file contains metadata
                {
                    metadata = item->GetMetaData();
                    MetaDataFormat* mdf = NULL;
                    uint32 numFormats;

                    numFormats = m_metadataFormats.size();

                    for(uint32 i = 0; i < numFormats; i++)
                    {
                        mdf = m_metadataFormats[i];

                        if(mdf)
                        {
                            mdf->ReadMetaData(item->URL().c_str(), &metadata);
                        }
                    }
                }

                // check to see if the song length is unknown
                // and if so calculate it.
                if(metadata.Time() == 0)
                {

                }

                m_mutex.Acquire();

                if(item->GetState() == kPlaylistItemState_Delete)
                {
                    delete item;
                }
                else
                {
                    item->SetMetaData(&metadata);
                    item->SetState(kPlaylistItemState_Normal);

                    m_context->target->AcceptEvent(new PlaylistItemUpdatedEvent(item));
                }

                m_mutex.Release();
            }
        }

        delete list;
    }
}

typedef struct MetaDataThreadStruct{
    PlaylistManager* plm;
    vector<PlaylistItem*>* items;
    Thread* thread;
} MetaDataThreadStruct;

void 
PlaylistManager::
metadata_thread_function(void* arg)
{
    MetaDataThreadStruct* mts = (MetaDataThreadStruct*)arg;

    mts->plm->MetaDataThreadFunction(mts->items);

    delete mts->thread;
    delete mts;
}

void PlaylistManager::RetrieveMetaData(PlaylistItem* item)
{
    assert(item);

    if(item)
    {
        vector<PlaylistItem*>* items = new vector<PlaylistItem*>;

        if(items)
        {
            items->push_back(item);
    
            RetrieveMetaData(items);
        }
    }
}

void PlaylistManager::RetrieveMetaData(vector<PlaylistItem*>* list)
{
    uint32 index = 0;
    uint32 size = 0;
    PlaylistItem* item = NULL;

    assert(list);

    if(list)
    {
        size = list->size();

        for(index = 0; index < size; index++)
        {
            item = (*list)[index];

            if(item && item->GetState() == kPlaylistItemState_Normal)
            {
                item->SetState(kPlaylistItemState_RetrievingMetaData);
            }
        }

        Thread* thread = Thread::CreateThread();

        if(thread)
        {
            MetaDataThreadStruct* mts = new MetaDataThreadStruct;

            mts->plm = this;
            mts->items = list;
            mts->thread = thread;

            thread->Create(metadata_thread_function, mts);
        }
    }
}

