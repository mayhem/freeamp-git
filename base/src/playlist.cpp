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
    pMediaElems = new Vector<PlayListItem *>();
    current = -1;
    skipNum = 0;
}

PlayList::~PlayList() {
    if (pMediaElems) {
        pMediaElems->DeleteAll();
	    delete pMediaElems;
	    pMediaElems = NULL;
    }
}

void PlayList::Add(char *pc, int type) {
    if (pc) {
	//int len = strlen(pc) + 1;
	//char *pNewC = new char[len];
	PlayListItem* item = new PlayListItem;
	int len = strlen(pc) + 1;
	if(item->url = new char[len]) {
	    //strcpy(item->url,pc);
	    memcpy(item->url,pc,len);
	} else {
	    //XXX FIXME Uhhh...what if we run out of memory?
	    // --jdw:  then we're screwwwwwwwed
	    cerr << "Out of memory!\n";
	}
	
	item->type = type;
	
	pMediaElems->insert(item);
    }
}

void PlayList::Shuffle(void) {
    double count= (double) pMediaElems->numElements();
    if (count < 2) {
	return;
    }
    
    srand((unsigned int) time(NULL));
    
    int32 first, second;
//    PlayListItem *temp = (PlayListItem *)new char[sizeof(PlayListItem)];
    for (int32 i = 0; i < 3 * count; i++) {
	first = (int32) ((count * rand()) / (RAND_MAX+1.0));
	second = (int32) ((count * rand()) / (RAND_MAX+1.0));
	pMediaElems->Swap(first,second);
//	if (first != second) {
//	    memcpy(temp, pMediaElems->elementAt(first), sizeof(PlayListItem));
//	    memcpy(pMediaElems->elementAt(first), pMediaElems->elementAt(second), sizeof(PlayListItem));
//	    memcpy(pMediaElems->elementAt(second), temp, sizeof(PlayListItem));
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
    return pMediaElems->elementAt(current);
}

 void PlayList::SetFirst() { current = 0; }
 void PlayList::SetNext() { current++; }
 void PlayList::SetPrev() { current--; }

PlayListItem::PlayListItem() {
    url = NULL;
}
    
PlayListItem::~PlayListItem() {
    if(url)
        delete url;
}
