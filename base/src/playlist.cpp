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

PlayList::PlayList() {
    m_pMediaElems = new Vector<PlayListItem *>();
    m_current = -1;
    m_skipNum = 0;
}

PlayList::~PlayList() {
    if (m_pMediaElems) {
        m_pMediaElems->DeleteAll();
	    delete m_pMediaElems;
	    m_pMediaElems = NULL;
    }
}

void PlayList::Add(char *pc, int type) {
    if (pc) {
	//int len = strlen(pc) + 1;
	//char *pNewC = new char[len];
	PlayListItem* item = new PlayListItem;
	int len = strlen(pc) + 1;
	if(item->m_url = new char[len]) {
	    //strcpy(item->m_url,pc);
	    memcpy(item->m_url,pc,len);
	} else {
	    //XXX FIXME Uhhh...what if we run out of memory?
	    // --jdw:  then we're screwwwwwwwed
	    cerr << "Out of memory!\n";
	}
	
	item->m_type = type;
	
	m_pMediaElems->Insert(item);
    }
}

void PlayList::Shuffle(void) {
    double count= (double) m_pMediaElems->NumElements();
    if (count < 2) {
	return;
    }
    
    srand((unsigned int) time(NULL));
    
    int32 first, second;
//    PlayListItem *temp = (PlayListItem *)new char[sizeof(PlayListItem)];
    for (int32 i = 0; i < 3 * count; i++) {
	first = (int32) ((count * rand()) / (RAND_MAX+1.0));
	second = (int32) ((count * rand()) / (RAND_MAX+1.0));
	m_pMediaElems->Swap(first,second);
//	if (first != second) {
//	    memcpy(temp, m_pMediaElems->elementAt(first), sizeof(PlayListItem));
//	    memcpy(m_pMediaElems->elementAt(first), m_pMediaElems->elementAt(second), sizeof(PlayListItem));
//	    memcpy(m_pMediaElems->elementAt(second), temp, sizeof(PlayListItem));
//	}
    }
//    delete (char *)temp;
}

 PlayListItem *PlayList::GetFirst() {
    SetFirst();
    return GetCurrent();
}

 PlayListItem *PlayList::GetNext() {
    SetNext();
    return GetCurrent();
}

 PlayListItem *PlayList::GetPrev() {
    SetPrev();
    return GetCurrent();
}

 PlayListItem *PlayList::GetCurrent() {
    return m_pMediaElems->ElementAt(m_current);
}

 void PlayList::SetFirst() { m_current = 0; }
 void PlayList::SetNext() { m_current++; }
 void PlayList::SetPrev() { m_current--; }

PlayListItem::PlayListItem() {
    m_url = NULL;
}
    
PlayListItem::~PlayListItem() {
    if(m_url)
        delete m_url;
}

