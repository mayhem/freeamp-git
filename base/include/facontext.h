/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Copyright (C) 1999 Mark H. Weaver <mhw@netris.org>

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

#ifndef _FACONTEXT_H_
#define _FACONTEXT_H_

#include "config.h"
#include "preferences.h"
#include "log.h"

static const int32 c_majorVersion = 1;
static const int32 c_minorVersion = 0;

class FAContext
{
 public:
    FAContext()
	: majorVersion(c_majorVersion),
	  minorVersion(c_minorVersion),
	  prefs(0),
	  log(0) { }
    
    ~FAContext()
    {
	if (log)
	    delete log;
	if (prefs)
	    delete prefs;
    }

    bool CompatibleVersion() { return majorVersion == c_majorVersion; }

    int32 majorVersion, minorVersion;
    Preferences *prefs;
    LogFile *log;
};

#endif // _FACONTEXT_H_
