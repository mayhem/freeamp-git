
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

#include "config.h"

class HashTable {
    class HashElem {
    public:
	HashElem *next;
	char key[KEYLENGTH];
	void *val;
    };
    HashElem **hashSlots;
    int32 numSlots;

    int32 hashkey(const char *);
 public:
    HashTable(int32 slots = 10);
    ~HashTable();

    void *Value(const char *key);
    void Insert(const char *key, void *val);
    void Remove(const char *key);
};

#endif // _HASHTABLE_H_
