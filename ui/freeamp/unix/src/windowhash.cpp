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


#include <stdlib.h>
#include <memory.h>
#include <iostream.h>
#include <string.h>

#include "windowhash.h"

WindowHash::WindowHash(int32 slots) {
    if (slots < 5) slots = 5;
    hashSlots = new HashElem*[slots];
    memset(hashSlots,'\0',slots*sizeof(HashElem *));
    numSlots = slots;
}

WindowHash::~WindowHash() {
    if (hashSlots) {
	delete hashSlots;
	hashSlots = NULL;
    }
}

FAWindow *WindowHash::Value(Window key) {
    int32 num = hashkey(key);
//    cout << "Keynum: " << num << endl;
    if (num >= 0) {
	HashElem *pElem = hashSlots[num];
	while (pElem) {
	    if (key == pElem->key) {
		return pElem->val;
	    }
	    pElem = pElem->next;
	}
	return NULL;
    } else {
	return NULL;
    }
}

void WindowHash::Insert(Window key, FAWindow *val) {
    int32 num = hashkey(key);
//    cout << "Keynum: " << num << endl;
    if (num >= 0) {
	if (hashSlots[num]) {
	    HashElem *pIns = hashSlots[num];
	    while (pIns->next) {
		pIns = pIns->next;
	    }
	    pIns->next = new HashElem();
	    pIns->next->next = NULL;
	    pIns->next->val = val;
	    pIns->next->key = key;
	} else {
	    hashSlots[num] = new HashElem();
	    hashSlots[num]->next = NULL;
	    hashSlots[num]->key = key;
	    hashSlots[num]->val = val;
	}
    }
}

void WindowHash::Remove(Window key) {
    int32 num = hashkey(key);
    if (num >= 0) {
	HashElem *pElem = hashSlots[num];
	HashElem *pPrev = NULL;
	while (pElem) {
	    if (pElem->key == key) {
		if (pPrev) {
		    pPrev->next = pElem->next;
		} else {
		    hashSlots[num] = pElem->next;
		}
		delete pElem;
		return;
	    }
	    pPrev = pElem;
	    pElem = pElem->next;
	}
    }
}

int32 WindowHash::hashkey(Window key) {
    return (int32)key % numSlots;
}








