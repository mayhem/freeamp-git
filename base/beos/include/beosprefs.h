/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 GoodNoise

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


#ifndef _BEOSPREFS_H
#define _BEOSPREFS_H

#include "config.h"
#include "errors.h"
#include "list.h"
#include "win32impl.h"


class BeOSPrefs : public Preferences {

 public:
    BeOSPrefs();
    ~BeOSPrefs();

    virtual LibDirFindHandle *GetFirstLibDir(char *path, uint32 *len);
    virtual Error GetNextLibDir(LibDirFindHandle *hLibDirFind,
				char *path, uint32 *len);
    virtual Error GetLibDirClose(LibDirFindHandle *hLibDirFind);

    virtual const char *GetLibDirs();

 protected:
    Error GetPrefString(const char* pref, char* buf, uint32* len);
    Error SetPrefString(const char* pref, char* buf);

 private:
//     HKEY   m_prefsKey;
    static char *m_libDirs;
};

#endif /* _BEOSPREFS_H */
