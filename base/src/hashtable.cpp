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
	
	$$
____________________________________________________________________________*/


#include <stdlib.h>
#include <memory.h>
#include <iostream.h>
#include <string.h>

#include "hashtable.h"

HashTable::HashTable(int32 slots) {
    if (slots < 5) slots = 5;
    hashSlots = new HashElem*[slots];
    memset(hashSlots,'\0',slots*sizeof(HashElem *));
    numSlots = slots;
}

HashTable::~HashTable() {
    if (hashSlots) {
	delete hashSlots;
	hashSlots = NULL;
    }
}

void *HashTable::Value(const char *key) {
    int32 num = hashkey(key);
//    cout << "Keynum: " << num << endl;
    if (num >= 0) {
	HashElem *pElem = hashSlots[num];
	while (pElem) {
	    if (!strncmp(key,pElem->key,KEYLENGTH-1)) {
		return pElem->val;
	    }
	    pElem = pElem->next;
	}
	return NULL;
    } else {
	return NULL;
    }
}

void HashTable::Insert(const char *key, void *val) {
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
	    strncpy(pIns->next->key,key,KEYLENGTH-1);
	} else {
	    hashSlots[num] = new HashElem();
	    hashSlots[num]->next = NULL;
	    strncpy(hashSlots[num]->key,key,KEYLENGTH-1);
	    hashSlots[num]->val = val;
	}
    }
}

void HashTable::Remove(const char *key) {
    int32 num = hashkey(key);
    if (num >= 0) {
	HashElem *pElem = hashSlots[num];
	HashElem *pPrev = NULL;
	while (pElem) {
	    if (!strncmp(pElem->key,key,KEYLENGTH-1)) {
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

int32 HashTable::hashkey(const char *key) {
    if (key) {
	int ind = 0;
	int32 total = 0;
	while (key[ind]) {
	    total += (int32)(key[ind]);
	    ind++;
	}
	return total % numSlots;
    }
    return -1;
}
