
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



#include "playlist.h"
#include "vector.h"

PlayList::PlayList() {
    pMediaElems = new Vector<PlayListItem *>();
    current = -1;
}

PlayList::~PlayList() {
    if (pMediaElems) {
	pMediaElems->DeleteAll();
	delete pMediaElems;
	pMediaElems = NULL;
    }
}

void PlayList::add(char *pc, int type) {
    if (pc) {
	//int len = strlen(pc) + 1;
	//char *pNewC = new char[len];
	PlayListItem* item = new PlayListItem;
	strcpy(item->url,pc);
	item->type = type;

	pMediaElems->insert(item);
    }
}

 PlayListItem *PlayList::getFirst() {
    setFirst();
    return getCurrent();
}

 PlayListItem *PlayList::getNext() {
    setNext();
    return getCurrent();
}

 PlayListItem *PlayList::getPrev() {
    setPrev();
    return getCurrent();
}

 PlayListItem *PlayList::getCurrent() {
    return pMediaElems->elementAt(current);
}

 void PlayList::setFirst() { current = 0; }
 void PlayList::setNext() { current++; }
 void PlayList::setPrev() { current--; }




