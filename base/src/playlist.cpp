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
#include <sys/stat.h>


#include "playlist.h"
#include "event.h"
#include "eventdata.h"
#include "mutex.h"
#include "thread.h"
#ifdef WIN32
#include "win32thread.h"
#endif

#include "std.h"
#include "rio.h"

PlayListManager::
PlayListManager(EventQueue * pPlayer)
{
   m_mutex = new Mutex();
   m_target = pPlayer;
   m_playList = new List < PlayListItem * >();
   m_shuffleList = NULL; 
   m_current = -1;
   m_skipNum = 0;
   m_order = SHUFFLE_NOT_SHUFFLED;
   m_repeat = REPEAT_NOT;
   srand((unsigned int) time(NULL));
   m_rioThread = NULL;
}

PlayListManager::
~PlayListManager()
{
   if(m_playList)
   {
      m_playList->DeleteAll();
      delete m_playList;
      m_playList = NULL;
   }

   if(m_shuffleList)
   {
      m_shuffleList->DeleteAll();
      delete m_shuffleList;
      m_shuffleList = NULL;
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

    if ((m_current >= 0) && (m_current < m_playList->CountItems()))
    {
        pli = m_playList->ItemAt(m_current);
    }

    ReleasePLManipLock();
    return pli;
}

void 
PlayListManager::
SetFirst()
{
    GetPLManipLock();
    int32 elems = m_playList->CountItems();

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
HasAnotherSong()
{
    GetPLManipLock();
    bool result = false;

    if(m_repeat == REPEAT_CURRENT || m_repeat == REPEAT_ALL)
    {
        result = true;
    }
    else
    {
        if(m_order == SHUFFLE_RANDOM)
        {
            if(m_playList->CountItems() != 1)
                result = true;
        }
        else
        {
            if (m_current != m_playList->CountItems() - 1)
                result = true;
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
    int32 count = m_playList->CountItems();

    if(count)
    {
        if(!(m_repeat == REPEAT_CURRENT))
        {
            if(SHUFFLE_RANDOM == m_order)
            {
                m_current = 0;

                if( m_shuffleList->CountItems() != m_playList->CountItems() ||
                    m_shuffle >= count)
                {
                    CreateShuffleList();
                }

                ShuffleItem* item = m_shuffleList->ItemAt(m_shuffle++);

                if(item)
                {
                    m_current = item->m_index;
                }                
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
    int32 count = m_playList->CountItems();

    if(count)
    {
        if(!(m_repeat == REPEAT_CURRENT))
        {
            if(SHUFFLE_RANDOM == m_order)
            {
                m_current = 0;

                if( m_shuffleList->CountItems() != m_playList->CountItems())
                {
                    CreateShuffleList();
                }

                m_shuffle--;

                if(m_shuffle < 0)
                {
                    m_shuffle = count - 1;
                }

                ShuffleItem* item = m_shuffleList->ItemAt(m_shuffle);

                if(item)
                {
                    m_current = item->m_index;
                }                
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
SetCurrent(int32 index)
{
    GetPLManipLock();

    index = CheckIndex(index);

    if(index >= 0 )
    {
        m_current = index;
        SendInfoToPlayer();
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
			// LEAK
            /*LEAK*/MediaInfoEvent *pMIE = new MediaInfoEvent(pli->URL(), 0);

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

    if(m_order == SHUFFLE_RANDOM)
    {
        CreateShuffleList();
    }

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
CreateShuffleList()
{
    if(m_shuffleList)
    {
        m_shuffleList->DeleteAll();
        delete m_shuffleList;
        m_shuffleList = NULL;
    }

    m_shuffle = 0;

    int32 count = CountItems();
    int32 i = 0;

    m_shuffleList = new List < ShuffleItem * >(count);

    srand( (unsigned)time( NULL ) );

    for (i = 0;i < count;i++) 
    {
        ShuffleItem* item = new ShuffleItem;

        item->m_index = i;
        item->m_random = (int32) rand();

	    m_shuffleList->AddItem(item);
    }

    QuickSortShuffleList(0, count - 1);

    /*for (i = 0;i < count;i++) 
    {
        char temp[10];
        sprintf(temp, "%d\r\n", m_shuffleList->ItemAt(i)->m_index);
        OutputDebugString(temp);
    }

    OutputDebugString("\r\n");*/

}

void 
PlayListManager::
QuickSortShuffleList(int32 first, int32 last) 
{
    if (first < last) 
    {
	    int32 q = PartitionShuffleList(first, last);
	    QuickSortShuffleList(first, q);
	    QuickSortShuffleList(q + 1,last);
    }
}

int32 
PlayListManager::
PartitionShuffleList(int32 first, int32 last) 
{
    int32 x = m_shuffleList->ItemAt(first)->m_random;
    int32 i = first - 1;
    int32 j = last + 1;

    for(;;) 
    {
	    do 
        {
	        j--;
	    } while (m_shuffleList->ItemAt(j)->m_random > x);

	    do 
        {
	        i++;
	    } while (m_shuffleList->ItemAt(i)->m_random < x);

	    if (i < j) 
        {
	        m_shuffleList->Swap(i,j);
	    } 
        else 
        {
	        return j;
	    }
    }
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

    result = m_playList->FirstItem();

    ReleasePLManipLock();

    return result;
}

PlayListItem* 
PlayListManager::
LastItem()
{
    PlayListItem* result = NULL;

    GetPLManipLock();

    result = m_playList->LastItem();

    ReleasePLManipLock();

    return result;
}

bool 
PlayListManager::
HasItem(PlayListItem* item)
{
    bool result = false;

    GetPLManipLock();

    result = m_playList->HasItem(item);

    ReleasePLManipLock();

    return result;
}

int32 
PlayListManager::
CountItems()
{
    int32 result = 0;

    GetPLManipLock();

    result = m_playList->CountItems();

    ReleasePLManipLock();

    return result;
}

PlayListItem*   
PlayListManager::
ItemAt(int32 index)
{
    PlayListItem* result = NULL;

    GetPLManipLock();

    result = m_playList->ItemAt(index);

    ReleasePLManipLock();

    return result;
}

int32
PlayListManager::
IndexOf(PlayListItem* item)
{
    int32 result = 0;

    GetPLManipLock();

    result = m_playList->IndexOf(item);

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

            m_playList->AddItem(item);

            if (m_playList->CountItems() == 1)
            {
                m_current = 0; // set current to first
            }

            result = kError_NoErr;

            PLMGetMediaTitleEvent *pTitle = new PLMGetMediaTitleEvent();
            pTitle->SetPlayListItem(item);
            m_target->AcceptEvent(pTitle);
            m_target->AcceptEvent(new Event(INFO_PlayListUpdated));
            //OutputDebugString("AddItem::AcceptEvent(INFO_PlayListUpdated)\r\n");
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

                m_playList->AddItem(item, index);

                if (m_playList->CountItems() == 1)
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
        m_playList->AddItem(item);

        if(m_playList->CountItems() == 1)
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
            m_playList->AddItem(item, index);

            if(m_playList->CountItems() == 1)
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
        m_playList->AddList(*items, CountItems());

        if(m_playList->CountItems() == 1)
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
            m_playList->AddList(*items, index);

            if(m_playList->CountItems() == 1)
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
        result = m_playList->RemoveItem(index);
    }

    if(m_current >= index)
        m_current = CountItems() - 1;

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
		    m_playList->RemoveItem(count);
        }

        if(m_current >= index)
            m_current = CountItems() - 1;

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
        m_playList->RemoveList(*items);

        result = kError_NoErr;

        if(m_current > CountItems() - 1)
            m_current = CountItems() - 1;

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
            m_playList->RemoveList(*items);

            m_playList->AddList(*items, index);

            if(m_playList->CountItems() == 1)
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

    m_playList->DeleteAll();

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

    result = m_playList->IsEmpty();

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
		if((*func)((PlayListItem*) m_playList->ItemAt(i)))
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
		if((*func)((PlayListItem*) m_playList->ItemAt(i), cookie))
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
    return (const PlayListItem**)(m_playList->Items());
}


Error 
PlayListManager::
RemoveAll()
{
   GetPLManipLock();

   m_playList->RemoveAll();
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
Error 
PlayListManager::
ExpandM3U(char *szM3UFile, List<char *> &MP3List)
{
	FILE  *fpFile;
	char  *szLine, *szTemp, *szMP3 = NULL;
	int32 iIndex;

	fpFile = fopen(szM3UFile, "r");
	if (fpFile == NULL)
        return kError_FileNotFound;

	szLine = new char[iMaxFileNameLen];
    szTemp = new char[iMaxFileNameLen];

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
             if( !strncmp(szLine, "..", 2) ||
                 (strncmp(szLine + 1, ":\\", 2) &&
                  strncmp(szLine, "\\", 1)) &&
                 (strncmp(szLine, "http://", 7) &&
                  strncmp(szLine, "rtp://", 6)) )
             {
                char* cp;

                strcpy(szTemp, szM3UFile);

                cp = strrchr(szTemp, '\\');

                if(cp)
                {
                    strcpy(cp + 1, szLine);
                    strcpy(szLine, szTemp);
                }
             }

			 szMP3 = new char[strlen(szLine) + 1];
			 strcpy(szMP3, szLine);
			 MP3List.AddItem(szMP3);
			 szMP3 = NULL;
		 }
    }

	delete [] szLine;
    delete [] szTemp;
	fclose(fpFile);

	return kError_NoErr;
}

Error
PlayListManager::
ExportToM3U(const char* file)
{
    Error result = kError_FileNotFound;
    FILE* fp;

	fp = fopen(file, "w");

	if(fp)
    {
        int32 i = 0;
        PlayListItem* item = NULL;

        while((item = m_playList->ItemAt(i++)))
        {
            fprintf(fp, "%s%s", item->URL(), LINE_END_MARKER_STR);
            result = kError_NoErr;
        }

	    fclose(fp);
    }

	return result;
}

Error
PlayListManager::
ExportToRio()
{
    Error result = kError_NoErr;

    if(!m_rioThread)
    {
        m_rioThread = Thread::CreateThread();
        m_rioThread->Create(rio_thread_function, this);
    }

	return result;
}

///////////////////////////////////////////////////////////////////////////////
// return file size
static long GetFileSize( char* pszPathFile )
{
	long lReturn = 0;

	FILE* fpFile = fopen( pszPathFile, "rb" );
	if ( fpFile )
	{
		struct stat sStat;
		if ( !stat(pszPathFile, &sStat) )
			lReturn = sStat.st_size;

		fclose( fpFile );
	}

	return lReturn;
}


BOOL
PlayListManager:: 
ProgressCallBack( int pos, int count)
{
    char foo[1024];
    int32 percentDone = (100 * pos)/count;
    sprintf(foo, "%d%% - %s", percentDone, m_txSong);
    m_target->AcceptEvent(new StatusMessageEvent(foo));
    return TRUE;
}

BOOL
PlayListManager:: 
progress_call_back( int pos, int count, void* cookie)
{
    PlayListManager* plm = (PlayListManager*)cookie;

	return plm->ProgressCallBack(pos, count);
}

void
PlayListManager::
RioThreadFunction()
{
#ifdef WIN32
    int32 ports[] = { 0x378, 0x278, 0x03BC };
    bool rioPresent = false;

    CRio* rio = new CRio;

    // try to find the rio

    m_target->AcceptEvent(new StatusMessageEvent("Searching for Rio..."));

    for(int32 count = 0; count < sizeof(ports); count++)
    {
        if(rio->Set(ports[count]) && rio->CheckPresent())
        {
            rioPresent = true;
            break;
        }
    }

    if(rioPresent)
    {
        m_target->AcceptEvent(new StatusMessageEvent("Rio found!"));

        if(rio->RxDirectory())
        {
            rio->RemoveAllFiles();

            rio->TxDirectory();

            CDirBlock& cDirBlock = rio->GetDirectoryBlock();
	        CDirHeader& cDirHeader = cDirBlock.m_cDirHeader;
            int32 lSizeAvailable = (int32)cDirHeader.m_usCount32KBlockAvailable * CRIO_SIZE_32KBLOCK;
	        int32 lSizeCurrent = (int32)cDirHeader.m_usCount32KBlockUsed * CRIO_SIZE_32KBLOCK;
	        int32 iCountEntryCurrent = cDirHeader.m_usCountEntry;
            
            int32 i = 0;
            PlayListItem* item = NULL;

            while(item = m_playList->ItemAt(i++))
            {
                // get file size
		        int32 lSize = GetFileSize( item->URL() );

                // check space
		        lSizeCurrent += lSize;

                if( lSizeCurrent > lSizeAvailable )
                {
                    break;
                }

                // check enough entries
		        ++iCountEntryCurrent;

                if( iCountEntryCurrent > CRIO_MAX_DIRENTRY )
                {
                    break;
                }

                m_txSong = item->StringForPlayerToDisplay();

                char foo[1024];
                sprintf(foo, "0%% - %s", m_txSong);
                m_target->AcceptEvent(new StatusMessageEvent(foo));
                
                rio->TxFile(item->URL(), progress_call_back, this);
            }

            rio->TxDirectory();
        }
        else
        {
            m_target->AcceptEvent(new StatusMessageEvent("Error Reading Directory"));
        }
    }
    else
    {
        m_target->AcceptEvent(new StatusMessageEvent("Unable to find Rio!"));
    }

    delete rio;

    delete m_rioThread;
    m_rioThread = NULL;
#endif    
}

void 
PlayListManager::
rio_thread_function(void* arg)
{
    PlayListManager* plm = (PlayListManager*)arg;

    plm->RioThreadFunction();
}
