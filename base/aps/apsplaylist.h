/*_________________________________________________________________________
         
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
                          apsplaylist.h  -  description
                             -------------------
    begin                : Sat Jun 3 2000
    copyright            : (C) 2000 by Relatable
    written by           : Sean Ward
    email                : sward@relatable.com
 ***************************************************************************/

#ifndef APS_PLAYLIST_HEADER
#define APS_PLAYLIST_HEADER

#ifdef WIN32
#pragma warning( disable : 4786 4503 )
#endif

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

class APSPlaylist;

#define APS_NOERROR       0
#define APS_EMPTYLIST     1
#define APS_ITEMNOTINLIST 2
#define APS_PARAMERROR    3
#define APS_ENDLIST       4

class APSPlaylistIterator
{
public:
    APSPlaylistIterator() { m_it = m_itEnd; }
   ~APSPlaylistIterator() {}

friend class APSPlaylist;
    string& first() { return (*m_it).first; }
    void first(string& strNewValue) { (*m_it).first = strNewValue; }

    string& second() { return (*m_it).second; }
    void second(string& strNewValue) { (*m_it).second = strNewValue; }

    void next() { m_it++; }
    void prev() { m_it--; }
    void adv(int nAmount) { m_it += nAmount; }
    bool isvalid() { return (m_it != m_itEnd); }

    typedef vector<pair<string, string> >::iterator PlayListIterator;

private:
    APSPlaylistIterator(PlayListIterator it, PlayListIterator itEnd)
    { 
        m_it = it;
        m_itEnd = itEnd;
    }
    PlayListIterator m_it;
    PlayListIterator m_itEnd;
};

class APSPlaylist
{
public:
    int GetBufLen()
    {
        APSPlaylist::iterator i = this->begin();
        int nBufLen = sizeof(int);
        for (; i.isvalid(); i.next())
        {
            nBufLen += i.first().size();
            nBufLen += i.second().size();
            nBufLen += sizeof(char)*2;
        }
        return nBufLen;
    }

    int ToBuffer(char* pBuffer, int& nBufUsed)
    {
        int nNumEntries = m_pPlayList->size();
        int nOffset = sizeof(int);
        int nTemp = 0;
        APSPlaylist::iterator i = this->begin();
        memcpy(pBuffer, &nNumEntries, sizeof(int));
        for (; i.isvalid(); i.next())
        {
            nTemp = i.first().size();
            memcpy(&pBuffer[nOffset], i.first().c_str(), nTemp);
            nOffset += nTemp;
            pBuffer[nOffset] = '\0';
            nOffset += sizeof(char);
                       
            nTemp = i.second().size();
            memcpy(&pBuffer[nOffset], i.second().c_str(), nTemp);
            nOffset += nTemp;
            pBuffer[nOffset] = '\0';
            nOffset += sizeof(char);
        }
        nBufUsed = nOffset;
        return APS_NOERROR;
    }
       
    int FromBuffer(char* pBuffer, int& nBufUsed)
    {
        this->Clear();
        int nOffset = 0;
        int nNumEntries = 0;
        memcpy(&nNumEntries, pBuffer, sizeof(int));
        int i = 0;
        string strOne, strTwo;
        nOffset = sizeof(int);
        for (i = 0; i < nNumEntries; i++)
        {
            strOne = &pBuffer[nOffset];
            nOffset += strOne.size() + sizeof(char);
            strTwo = &pBuffer[nOffset];
            nOffset += strTwo.size() + sizeof(char);
            m_pPlayList->push_back(pair<string, string>(strOne, strTwo));
        }
        nBufUsed = nOffset;
        return APS_NOERROR;
    }

// Typdefs and iterator support
friend class APSPlaylistIterator;
    typedef APSPlaylistIterator iterator;
    typedef vector<pair<string, string> > PlayList;

    void Display()
    {
        PlayList::iterator i;
        for (i = m_pPlayList->begin(); i != m_pPlayList->end(); i++)
        {
            cout << (*i).first << " : " << (*i).second << endl;
        }
    }

    APSPlaylist();
   ~APSPlaylist();

    int Size();
    int Empty() { return m_pPlayList->empty(); }
  
    // Insertion methods
    int Insert(const char* pczNewElement, const char* pczURL = NULL);
    int Insert(APSPlaylist* pPlaylist);
    int InsertBefore(const char* pczNewElement, const char* pczExistingElement, 
                     const char* = NULL);
    int InsertBefore(const char* pczNewElement, APSPlaylist::iterator itPlace, 
                     const char* = NULL);
    int InsertAfter(const char* pczNewElement, const char* pczExistingElement, 
                    const char* = NULL);
    int InsertAfter(const char* pczNewElement, APSPlaylist::iterator itPlace, 
                    const char* = NULL);

    // Removal methods
    int Remove(const char* pczElement, const char* = NULL);
    int Remove(APSPlaylist::iterator itPlace);
    int Clear();

    // Location control
    APSPlaylist::iterator begin() 
    { 
        return APSPlaylist::iterator(m_pPlayList->begin(), m_pPlayList->end()); 
    }
    APSPlaylist::iterator end() 
    { 
        return APSPlaylist::iterator(m_pPlayList->end(), m_pPlayList->end()); 
    }

    APSPlaylist::iterator Find(const char* pczElement, 
                               APSPlaylist::iterator* pIt);
    APSPlaylist::iterator Random();
private:
    PlayList* m_pPlayList;
};

#endif

