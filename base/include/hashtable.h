
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


#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#define KEYLENGTH 64

#include <iostream.h>

#include "config.h"

template<class T> class HashElem {
 public:
    ~HashElem() { delete val; }
    HashElem *next;
    char key[KEYLENGTH];
    T val;
};


template<class T> class HashTable  {
 public:
    HashTable(int32 slots = 10);
    ~HashTable();
    
    T Value(const char *key);
    void Insert(const char *key, T val);
    void Remove(const char *key);
 private:
    HashElem<T> **hashSlots;
    int32 numSlots;
    
    int32 hashkey(const char *);
};

template<class T> HashTable<T>::HashTable(int32 slots) {
    if (slots < 5) slots = 5;
    hashSlots = new HashElem<T>*[slots];
    memset(hashSlots,'\0',slots*sizeof(HashElem<T> *));
    numSlots = slots;
}


template<class T> HashTable<T>::~HashTable() {
    if (hashSlots) {
	HashElem<T> *phe;
	for (int i=0;i<numSlots;i++) {
	    phe = hashSlots[i];
	    while (phe != NULL) {
		HashElem<T> *next = phe->next;
		delete phe;
		phe = next;
	    }
	}
	delete hashSlots;
	hashSlots = NULL;
    }
}

template<class T> T HashTable<T>::Value(const char *key) {
    int32 num = hashkey(key);
//    cout << "Keynum: " << num << endl;
    if (num >= 0) {
	HashElem<T> *pElem = hashSlots[num];
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


template<class T> void HashTable<T>::Insert(const char *key, T val) {
    int32 num = hashkey(key);
//    cout << "Keynum: " << num << endl;
    if (num >= 0) {
	if (hashSlots[num]) {
	    HashElem<T> *pIns = hashSlots[num];
	    while (pIns->next) {
		pIns = pIns->next;
	    }
	    pIns->next = new HashElem<T>();
	    pIns->next->next = NULL;
	    pIns->next->val = val;
	    strncpy(pIns->next->key,key,KEYLENGTH-1);
	} else {
	    hashSlots[num] = new HashElem<T>();
	    hashSlots[num]->next = NULL;
	    strncpy(hashSlots[num]->key,key,KEYLENGTH-1);
	    hashSlots[num]->val = val;
	}
    }
}


template<class T> void HashTable<T>::Remove(const char *key) {
    int32 num = hashkey(key);
    if (num >= 0) {
	HashElem<T> *pElem = hashSlots[num];
	HashElem<T> *pPrev = NULL;
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


template<class T> int32 HashTable<T>::hashkey(const char *key) {
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


#endif // _HASHTABLE_H_










