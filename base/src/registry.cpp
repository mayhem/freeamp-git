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

#include "Registry.h"


Registry::
Registry()
{
    m_elements = new Vector<RegistryInfo*>();
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
Add(RegistryInfo* info)
{
    if (info) {
	    m_elements->insert(info);
    }
}

RegistryInfo* 
Registry::
GetInfo(int32 index)
{
    return m_elements->elementAt(index);
}


RegistryInfo::
RegistryInfo()
{
    m_path = NULL;
    m_description = NULL;
}

RegistryInfo::
~RegistryInfo()
{
    if(m_path)
        delete m_path;

    if(m_description)
        delete m_description;

}

void RegistryInfo::SetPath(char* path)
{
    if(path)
    {
        if(m_path)
            delete m_path;

        m_path = new char [strlen(path) + 1];

        strcpy(m_path, path);
    }
}
 

void RegistryInfo::SetDescription(char* description)
{
    if(description)
    {
        if(m_description)
            delete m_description;

        m_description = new char [strlen(description) + 1];

        strcpy(m_description, description);
    }
}