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

#include "registry.h"


Registry::
Registry()
{
    m_elements = new List<RegistryItem*>();
    m_count = 0;
}

Registry::
~Registry()
{
    if (m_elements) {
        m_elements->DeleteAll();
	    delete m_elements;
	    m_elements = NULL;
    }
}

void 
Registry::
Add(RegistryItem* info)
{
    if (info) {
	    m_elements->AddItem(info);
    }
}

RegistryItem* 
Registry::
GetItem(int32 index)
{
    return m_elements->ItemAt(index);
}

int32 
Registry::
GetNumItems()
{
    return m_elements->CountItems();
}

RegistryItem::
RegistryItem()
{
    m_path = NULL;
    m_description = NULL;
    m_name = NULL;
    m_init = NULL;
    m_module = NULL;
}

RegistryItem::
~RegistryItem()
{
    if(m_path)
        delete m_path;

    if(m_name)
        delete m_name;

    if(m_description)
        delete m_description;
}

void 
RegistryItem::
SetPath(char* path)
{
    if(path)
    {
        if(m_path)
            delete m_path;

        m_path = new char [strlen(path) + 1];

        strcpy(m_path, path);
    }
}

void 
RegistryItem::
SetName(char* name)
{
    if(name)
    {
        if(m_name)
            delete m_name;

        m_name = new char [strlen(name) + 1];

        strcpy(m_name, name);
    }
}
 

void 
RegistryItem::
SetDescription(char* description)
{
    if(description)
    {
        if(m_description)
            delete m_description;

        m_description = new char [strlen(description) + 1];

        strcpy(m_description, description);
    }
}

void 
RegistryItem::
SetInitFunction(InitializeFunction function)
{
    if(function)
        m_init = function;
}

void 
RegistryItem::
SetModule(void* module)
{
    if(module)
        m_module = module;
}

