/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1999 EMusic.com

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "config.h"
#include "errors.h"
#include "utility.h"

#include "misc.h"



extern "C"
{
   MetaDataFormat *Initialize(FAContext* context)
   {
      return new Misc(context);
   }
}

Misc::Misc(FAContext* context):MetaDataFormat(context)
{
    m_context = context;
}

Misc::~Misc()
{

}

bool Misc::ReadMetaData(const char* url, MetaData* metadata)
{
    bool result = false;
    Error error;

    assert(url);
    assert(metadata);

    char path[_MAX_PATH];
    uint32 length = sizeof(path);

    error = URLToFilePath(url, path, &length);

    if(IsntError(error))
    {
        
    }
    return true;
}

bool Misc::WriteMetaData(const char* url, const MetaData& metadata)
{
    bool result = false;
    return result;
}

