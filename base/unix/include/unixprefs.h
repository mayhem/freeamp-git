/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998-1999 GoodNoise
	Portions Copyright (C) 1999 Mark H. Weaver <mhw@netris.org>

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


#ifndef _UNIXPREFS_H
#define _UNIXPREFS_H

#include "config.h"
#include "errors.h"
#include "list.h"
#include "preferences.h"
#include "hashtable.h"
#include "win32impl.h"
#include "mutex.h"

class UnixPrefEntry
{
 public:
    UnixPrefEntry() : prefix(0), key(0), separator(0), value(0), suffix(0) { }
    ~UnixPrefEntry();

    char *prefix;	// Preceeding comments and indentation
    char *key;
    char *separator;
    char *value;
    char *suffix;	// Trailing space and comment (including NL)
};

class UnixPrefs : public Preferences
{
 public:
    UnixPrefs();
    ~UnixPrefs();

    virtual Error SetDefaults();
    virtual Error Save();

    virtual Preferences *ComponentPrefs(const char *componentName);

    virtual Error GetPrefString(const char* pref, char* buf, uint32* len);
    virtual Error SetPrefString(const char* pref, const char* buf);

    virtual LibDirFindHandle *GetFirstLibDir(char *path, uint32 *len);
    virtual Error GetNextLibDir(LibDirFindHandle *hLibDirFind,
				char *path, uint32 *len);
    virtual Error GetLibDirClose(LibDirFindHandle *hLibDirFind);

    virtual const char *GetLibDirs();

    int GetErrorLineNumber() const { return m_errorLineNumber; }

 private:
    Mutex m_mutex;

//     HKEY   m_prefsKey;
    static char *m_libDirs;

    char *m_prefsFilePath;
    bool m_saveEnable, m_changed;
    int m_errorLineNumber;	// 0 if no error

    List<UnixPrefEntry *> m_entries;
    HashTable<UnixPrefEntry *> m_ht;
};

#endif /* _UNIXPREFS_H */
