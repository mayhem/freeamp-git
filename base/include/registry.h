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

#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include "config.h"
#include "vector.h"

class RegistryInfo {
 public:
    RegistryInfo();
    virtual ~RegistryInfo();

    virtual void SetPath(char* path);
    virtual const char* Path() const {return m_path;}

    virtual void SetDescription(char* description);
    virtual const char* Description() const { return m_description;}


 private:
    char* m_path;
    char* m_description;

};

class Registry {
 private:
    Vector<RegistryInfo*> *m_elements;
    int32 m_count;
    
 public:
    Registry();
    virtual ~Registry();

    virtual void Add(RegistryInfo*);
    virtual RegistryInfo* GetInfo(int32 index);
};


#endif // _REGISTRY_H_
