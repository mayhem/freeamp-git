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
#include "event.h"
#include "eventdata.h"
#include "mutex.h"

PlayListManager::PlayListManager(EventQueue * pPlayer)
{
   m_plMutex = new Mutex();
   m_target = pPlayer;
   m_pMediaElems = new Vector < PlayListItem * >();
   m_pOrderList = new Vector < OrderListItem * >();
   if (!m_pMediaElems)
   {
      cerr << "Out of memory!" << endl;
      exit(1);
   }
   m_current = -1;
   m_skipNum = 0;
   m_order = SHUFFLE_NOT_SHUFFLED;
   m_repeat = REPEAT_NOT;
   srand((unsigned int) time(NULL));
}

PlayListManager::~PlayListManager()
{
   if (m_pMediaElems)
   {
      m_pMediaElems->DeleteAll();
      delete    m_pMediaElems;

      m_pMediaElems = NULL;
   }
   if (m_pOrderList)
   {
      m_pOrderList->DeleteAll();
      delete    m_pOrderList;

      m_pOrderList = NULL;
   }
   if (m_plMutex)
   {
      delete    m_plMutex;

      m_plMutex = NULL;
   }
}

Error PlayListManager::RemoveAll()
{
   GetPLManipLock();
   m_pMediaElems->DeleteAll();
   m_pOrderList->DeleteAll();
   m_current = -1;
   m_skipNum = 0;

   m_target->AcceptEvent(new MediaInfoEvent());
   ReleasePLManipLock();
   return kError_NoErr;
}

void PlayListManager::Add(char *pc, int type)
{
   GetPLManipLock();

   if(pc)
   {
        PlayListItem *item = new PlayListItem();

        if (!item)
        {
         cerr << "Out of memory!" << endl;
         exit(1);
        }

        item->SetURL(pc);
        item->SetType(type);

        m_pMediaElems->Insert(item);

        if (m_pMediaElems->NumElements() == 1)
        {
            m_current = 0;         // set current to first
        }
        // add a corresponding element to the order list
        OrderListItem *orderItem = new OrderListItem();

        orderItem->m_indexToRealVector = m_pMediaElems->NumElements() - 1;
        m_pOrderList->Insert(orderItem);

        //PLMGetMediaInfoEvent *gmi = new PLMGetMediaInfoEvent();
        //gmi->SetPlayListItem(item);
        //m_target->AcceptEvent(gmi);
        m_target->AcceptEvent(new Event(INFO_PlayListUpdated));
   }

   ReleasePLManipLock();
}

Error     PlayListManager::
AddAt(char *pc, int type, int32 at)
{
    Error     rtn = kError_UnknownErr;

    GetPLManipLock();
    if ((at >= 0) && (at <= m_pMediaElems->NumElements()))
    {
        if (pc)
        {
            PlayListItem *item = new PlayListItem();

            if (!item)
            {
                cerr << "Out of memory!!!" << endl;
                exit(1);
            }

            item->SetURL(pc);
            item->SetType(type);

            if (m_order == SHUFFLE_SHUFFLED)
            {
                m_pMediaElems->Insert(item); // put the real thing at the end

            }
            else
            {
                m_pMediaElems->InsertAt(at, item);
            }
            if (m_pMediaElems->NumElements() == 1)
            {
                m_current = 0;
            }

            OrderListItem *orderItem = new OrderListItem();

            if (m_order == SHUFFLE_SHUFFLED)
            {
                orderItem->m_indexToRealVector = m_pMediaElems->NumElements() - 1;
                m_pOrderList->InsertAt(at, orderItem);
            }
            else
            {
                orderItem->m_indexToRealVector = at;
                m_pOrderList->InsertAt(at, orderItem);
            }

            //PLMGetMediaInfoEvent *gmi = new PLMGetMediaInfoEvent();
            //gmi->SetPlayListItem(item);
            //m_target->AcceptEvent(gmi);
            m_target->AcceptEvent(new Event(INFO_PlayListUpdated));

            rtn = kError_NoErr;
        }
    }

    ReleasePLManipLock();

    return rtn;
}

Error PlayListManager::RemoveItem(int32 at)
{
   Error     rtn = kError_UnknownErr;

   GetPLManipLock();
   if ((at >= 0) && (at < m_pMediaElems->NumElements()))
   {
      if (m_order == SHUFFLE_SHUFFLED)
      {
         int32     realOne = m_pOrderList->ElementAt(at)->m_indexToRealVector;

         m_pOrderList->DeleteElementAt(at);
         m_pMediaElems->DeleteElementAt(realOne);
      }
      else
      {
         // just delete the last element.  when not shuffled, the ordered
         // vector isn't used
         m_pMediaElems->DeleteElementAt(at);
         m_pOrderList->DeleteElementAt(m_pOrderList->NumElements() - 1);
      }
      m_target->AcceptEvent(new Event(INFO_PlayListUpdated));
      rtn = kError_NoErr;
   }
   ReleasePLManipLock();
   return rtn;
}

PlayListItem *PlayListManager::ItemAt(int32 at)
{
   PlayListItem *rtn = NULL;

   GetPLManipLock();
   if ((at >= 0) && (at < m_pMediaElems->NumElements()))
   {
      if (m_order == SHUFFLE_SHUFFLED)
      {
         int32     realOne = m_pOrderList->ElementAt(at)->m_indexToRealVector;

         rtn = m_pMediaElems->ElementAt(realOne);
      }
      else
      {
         rtn = m_pMediaElems->ElementAt(at);
      }
   }
   ReleasePLManipLock();
   return rtn;
}

PlayListItem *PlayListManager::GetCurrent()
{
   GetPLManipLock();
   PlayListItem *pli = NULL;

   if ((m_current < 0) || (m_current >= m_pMediaElems->NumElements()))
   {
      ;
   }
   else
   {
      if (m_order == SHUFFLE_SHUFFLED)
      {
         pli = m_pMediaElems->ElementAt((m_pOrderList->ElementAt(m_current))->m_indexToRealVector);
      }
      else
      {
         pli = m_pMediaElems->ElementAt(m_current);
      }
   }

   ReleasePLManipLock();
   return pli;
}

void PlayListManager::SetFirst()
{
   GetPLManipLock();
   int32     elems = m_pMediaElems->NumElements();

   if (elems)
   {
      if (m_order == SHUFFLE_RANDOM)
      {
         SetNext();
      }
      else
      {
         m_current = 0;
      }
      SendInfoToPlayer();
   }
   else
   {
      m_current = -1;
   }

   ReleasePLManipLock();
}

bool PlayListManager::NextIsSame()
{
   GetPLManipLock();
   bool      rtn = false;

   if (m_repeat == REPEAT_CURRENT)
   {
      rtn = true;
   }
   else
   {
      if (m_order == SHUFFLE_RANDOM)
      {
         if (m_pMediaElems->NumElements() == 1)
            rtn = true;
         else
            rtn = false;
      }
      else
      {
         if (m_repeat == REPEAT_ALL)
         {
            // only same if there is only one song in list
            if (m_pMediaElems->NumElements() == 1)
               rtn = true;
         }
         else
         {
            // only same if current is the last song
            if (m_current == m_pMediaElems->NumElements() - 1)
               rtn = true;
         }
      }
   }
   ReleasePLManipLock();
   return rtn;
}

void PlayListManager::SetNext(bool bUserAction)
{
   GetPLManipLock();
   int32     elems = m_pMediaElems->NumElements();

   if (elems)
   {
      if (!(m_repeat == REPEAT_CURRENT))
      {
         int32     count = m_pOrderList->NumElements();

         if (count != 0)
         {
            if (SHUFFLE_RANDOM == m_order)
            {
               m_current = (int32) (((double) count * rand()) / (RAND_MAX + 1.0));
            }
            else
            {
               m_current++;
               if ((m_current >= count) && 
                   (m_repeat == REPEAT_ALL || bUserAction))
               {
                  m_current = 0;
               }
            }
            SendInfoToPlayer();
         }
      }
   }
   else
   {
      m_current = -1;
   }
   ReleasePLManipLock();
}

void PlayListManager::SetPrev(bool bUserAction)
{
   GetPLManipLock();
   int32     elems = m_pMediaElems->NumElements();

   if (elems)
   {
      if (!(m_repeat == REPEAT_CURRENT))
      {
         int32     count = m_pOrderList->NumElements();

         if (m_order == SHUFFLE_RANDOM)
         {
            SetNext();
         }
         else
         {
            m_current--;
            if ((m_current < 0) && 
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

void PlayListManager::SendInfoToPlayer()
{
   PlayListItem         *pli = GetCurrent();
   PLMGetMediaInfoEvent *gmi = new PLMGetMediaInfoEvent();

   if (gmi && pli)
   {
       gmi->SetPlayListItem(pli);
       m_target->AcceptEvent(gmi);
   }

   if (pli)
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

void PlayListManager::SendShuffleModeToPlayer()
{
   m_target->AcceptEvent(new PlayListShuffleEvent(m_order));
}

void PlayListManager::SendRepeatModeToPlayer()
{
   m_target->AcceptEvent(new PlayListRepeatEvent(m_repeat));
}

void PlayListManager::SetShuffle(ShuffleMode oop)
{
   GetPLManipLock();
   if ((oop >= 0) && (oop < SHUFFLE_INTERNAL_NUMBER))
   {
      int32     elems = m_pOrderList->NumElements();

      switch (oop)
      {
      case SHUFFLE_SHUFFLED:
         // reshuffle
         ShuffleOrder();
         break;
      case SHUFFLE_NOT_SHUFFLED:
         // m_current points into the ordered list, restore it to 'correct'
         // element

         if (m_order == SHUFFLE_SHUFFLED)
         {
            if ((elems > 0) && (m_current >= 0) && (m_current < elems))
            {
               m_current = (m_pOrderList->ElementAt(m_current))->m_indexToRealVector;
            }
            else
            {
               m_current = -1;
            }
         }
         break;
      default:
         break;
      }
      m_order = oop;
      SendShuffleModeToPlayer();
   }
   ReleasePLManipLock();
}

void PlayListManager::SetRepeat(RepeatMode rp)
{
   GetPLManipLock();
   m_repeat = rp;
   SendRepeatModeToPlayer();
   ReleasePLManipLock();
}

void PlayListManager::ShuffleOrder()
{
   int32     element_count = m_pOrderList->NumElements();

   if (element_count < 1)
   {
      return;
   }

   OrderListItem *order_item;

   // remember the song that m_current is "really" pointing at
   int32     actual_current, i;

   if (m_current == -1)
   {
      actual_current = -1;
   }
   else if (m_order == SHUFFLE_SHUFFLED)
   {
      actual_current = (m_pOrderList->ElementAt(m_current))->m_indexToRealVector;
   }
   else
   {
      actual_current = m_current;
   }

   for (i = 0; i < element_count; i++)
   {
      order_item = m_pOrderList->ElementAt(i);
      order_item->m_indexToRealVector = i;
      order_item->m_random = (int32) rand();
   }

   QuickSortOrderList(0, element_count - 1);

   // set proper current pointer
   if (actual_current == -1)
   {
      m_current = -1;
   }
   else
   {
      for (i = 0; i < element_count; i++)
      {
         order_item = m_pOrderList->ElementAt(i);
         if (order_item->m_indexToRealVector == actual_current)
         {
            m_current = i;
            break;
         }
      }
   }
}

void PlayListManager::QuickSortOrderList(int32 first, int32 last)
{
   if (first < last)
   {
      int32     q = PartitionOrderList(first, last);

      QuickSortOrderList(first, q);
      QuickSortOrderList(q + 1, last);
   }
}

int32 PlayListManager::PartitionOrderList(int32 first, int32 last)
{
   int32     x = m_pOrderList->ElementAt(first)->m_random;
   int32     i = first - 1;
   int32     j = last + 1;

   for (;;)
   {
      do
      {
         j--;
      }
      while (m_pOrderList->ElementAt(j)->m_random > x);
      do
      {
         i++;
      }
      while (m_pOrderList->ElementAt(i)->m_random < x);
      if (i < j)
      {
         m_pOrderList->Swap(i, j);
      }
      else
      {
         return j;
      }
   }
}

void PlayListManager::AcceptEvent(Event * e)
{
   if (e)
   {
      switch (e->Type())
      {
      case CMD_PLMSetMediaInfo:
         {
            PLMSetMediaInfoEvent *smi = (PLMSetMediaInfoEvent *) e;

            if (smi->IsComplete())
            {
               PlayListItem *pItem = smi->GetPlayListItem();

               // make sure pItem still exists
               pItem->SetPMIRegistryItem(smi->GetPMIRegistryItem());
               pItem->SetLMCRegistryItem(smi->GetLMCRegistryItem());
               //RAK
               //pItem->SetPMI(smi->GetPMI());
               pItem->SetMediaInfo(smi->GetMediaInfo());
               // if pItem is the current item, send out the info posthaste
               if (pItem == GetCurrent())
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

Error     PlayListManager::
ToggleShuffle()
{
   SetShuffle((ShuffleMode) ((m_order + 1) % SHUFFLE_INTERNAL_NUMBER));
   return kError_NoErr;
}

Error     PlayListManager::
ToggleRepeat()
{
   SetRepeat((RepeatMode) ((m_repeat + 1) % REPEAT_INTERNAL_NUMBER));
   return kError_NoErr;
}
