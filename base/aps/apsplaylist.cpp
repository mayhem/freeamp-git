/*__________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 2000 Relatable

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/


/***************************************************************************
                          apsplaylist.cpp  -  description
                             -------------------
    begin                : Sat Jun 3 2000
    copyright            : (C) 2000 by Relatable 
    written by           : Sean Ward
    email                : sward@relatable.com
 ***************************************************************************/

#include "apsplaylist.h"

// sets the current playlist item to the beginning of the (empty) playlist list
APSPlaylist::APSPlaylist()
{
    srand((unsigned)time(NULL));
    m_pPlayList = new PlayList;
}

// Simply clears the playlist queue and unloads the list construct
APSPlaylist::~APSPlaylist()
{
    if (m_pPlayList != NULL)
    {
        delete m_pPlayList;
        m_pPlayList = NULL;
    }
}

int APSPlaylist::Size()
{
    return m_pPlayList->size();
}

// Simply inserts NewElement and NewUrl at the end of the list
int APSPlaylist::Insert(const char* pczNewElement, const char* pczNewUrl)
{
    if (pczNewElement == NULL) 
        return APS_PARAMERROR;

    m_pPlayList->push_back(PlayList::value_type(pczNewElement, pczNewUrl));
    return APS_NOERROR;
}

// Insert pPlaylist contents into m_pPlaylist
int APSPlaylist::Insert(APSPlaylist *pPlaylist)
{
    if ((pPlaylist == NULL) || (pPlaylist->Size() == 0)) 
        return APS_PARAMERROR;
	
    m_pPlayList->reserve(m_pPlayList->size() + pPlaylist->m_pPlayList->size());

    for (APSPlaylist::iterator i = pPlaylist->begin(); i.isvalid(); i.next())
    {
        m_pPlayList->push_back(PlayList::value_type(i.first(), i.second()));
    }
    return APS_NOERROR;		
}


// Inserts NewElement and NewUrl before Existing Element in order.	
int APSPlaylist::InsertBefore(const char* pczNewElement, 
                              const char* pczExistingElement, 
                              const char* pczNewUrl)
{
    if ((pczNewElement == NULL) || (pczExistingElement == NULL)) 
        return APS_PARAMERROR;
    if (m_pPlayList->empty()) 
        return APS_ITEMNOTINLIST;
	
    APSPlaylist::iterator i;
    string strTemp(pczExistingElement);

    i = this->begin();
	
    while (i.isvalid())
    {
        if (i.first() == strTemp)
        {
            m_pPlayList->insert(i.m_it, PlayList::value_type(pczNewElement, 
                                                             pczNewUrl));
            return APS_NOERROR;
        }
        i.next();
    }

    return APS_ITEMNOTINLIST;
}

// Inserts NewElement and NewUrl before Existing Element in order.
int APSPlaylist::InsertBefore(const char* pczNewElement, 
                              APSPlaylist::iterator itPlace, 
                              const char* pczNewUrl)
{
    if (pczNewElement == NULL) 
        return APS_PARAMERROR;
    if (m_pPlayList->empty()) 
        return APS_ITEMNOTINLIST;

    m_pPlayList->insert(itPlace.m_it, 
                        PlayList::value_type(pczNewElement, pczNewUrl));
    return APS_NOERROR;
}

// Inserts NewElement and NewUrl after Existing Element in order. 
int APSPlaylist::InsertAfter(const char* pczNewElement, 
                             const char* pczExistingElement, 
                             const char* pczNewUrl)
{
    if ((pczNewElement == NULL) || (pczExistingElement == NULL)) 
        return APS_PARAMERROR;

    if (m_pPlayList->size() == 0) 
        return APS_ITEMNOTINLIST;
	
    APSPlaylist::iterator i;
    string strTemp(pczExistingElement);

    i = this->begin();

    while (i.isvalid())
    {
        if (i.first() == strTemp)
        {
            i.next();    // increment to next element
            m_pPlayList->insert(i.m_it, PlayList::value_type(pczNewElement, 
                                                             pczNewUrl));
            return APS_NOERROR;
        }
        i.next();
    }

    return APS_ITEMNOTINLIST;
}

// Inserts NewElement and NewUrl after Existing Element in order.
int APSPlaylist::InsertAfter(const char* pczNewElement, 
                             APSPlaylist::iterator itPlace, 
                             const char* pczNewUrl)
{
    if (pczNewElement == NULL) 
        return APS_PARAMERROR;
    if (m_pPlayList->empty()) 
        return APS_ITEMNOTINLIST;
		
    m_pPlayList->insert(itPlace.m_it, PlayList::value_type(pczNewElement, 
                                                           pczNewUrl));
    return APS_NOERROR;
}

// Removes pczElement from PlayList. 
int APSPlaylist::Remove(const char* pczElement, const char* pczNewUrl)
{
    if (pczElement == NULL) 
        return APS_PARAMERROR;
    if (m_pPlayList->empty()) 
        return APS_ITEMNOTINLIST;

    APSPlaylist::iterator i;
    string strTemp(pczElement);

    i = this->begin();
	
    while (i.isvalid())
    {
        if (i.first() == strTemp)
        {
            m_pPlayList->erase(i.m_it);
            return APS_NOERROR;
        }
        i.next();
    }
    return APS_NOERROR;	
}	

int APSPlaylist::Remove(APSPlaylist::iterator itPlace)
{
    if (!itPlace.isvalid()) 
        return APS_PARAMERROR;
    if (m_pPlayList->empty())
        return APS_ITEMNOTINLIST;

    m_pPlayList->erase(itPlace.m_it);
    return APS_NOERROR;	
}


// Clears the queue, and sets the iterator to the (empty) list beginning
int APSPlaylist::Clear()
{
//	m_pPlayList->clear();
    if (m_pPlayList != NULL) 
        delete m_pPlayList;
    m_pPlayList = new PlayList;
    return APS_NOERROR;
}


// Finds the listed element, and sets the current pointer to it
APSPlaylist::iterator APSPlaylist::Find(const char* pczElement, APSPlaylist::iterator* pIt)
{

    if (m_pPlayList->empty())
    {
        return APSPlaylist::iterator(m_pPlayList->end(), m_pPlayList->end());
    }

    string strTemp(pczElement);

    for (PlayList::iterator i = m_pPlayList->begin(); i != m_pPlayList->end(); 
         i++)
    {
        if ((*i).first == strTemp)
        {
            pIt->m_it = i;
            pIt->m_itEnd = m_pPlayList->end();
        }
    }
    return *pIt;
}

// Sets the current item to a random item, and returns that id
APSPlaylist::iterator APSPlaylist::Random()
{
    if (m_pPlayList->empty())
    {
        return APSPlaylist::iterator(m_pPlayList->end(), m_pPlayList->end());
    }

    float fTemp = rand() / (float)INT_MAX;

    int i = (int)(m_pPlayList->size() * fTemp);

    APSPlaylist::iterator iReturn(m_pPlayList->begin(), m_pPlayList->end());
	
    iReturn.adv(i);
        
    return iReturn;
}
