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

#ifndef _PROPIMPL_H_
#define _PROPIMPL_H_

#include <stdlib.h>

#include "config.h"
#include "hashtable.h"
#include "vector.h"
#include "mutex.h"
#include "properties.h"

class PropertiesImpl : public Properties {
 private:
    class PropElem {
    public:
	PropElem() { m_deleteWhenDone = false; m_val = NULL; }
	~PropElem() { if (m_deleteWhenDone) free(m_val); }
	Vector<PropertyWatcher *> m_propWatchers;
	void *m_val;
	bool m_deleteWhenDone;
    };
    HashTable<PropElem *> *m_props;
    Mutex m_lock;
 public:
    PropertiesImpl();
    ~PropertiesImpl();
    virtual Error GetProperty(const char *, void **);
    virtual Error SetProperty(const char *, void *, bool);
    virtual Error RegisterPropertyWatcher(const char *, PropertyWatcher *);
    virtual Error RemovePropertyWatcher(const char *, PropertyWatcher *);
};

#endif // _PROPIMPL_H_


