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

#include <unistd.h>
#include <stdlib.h>

#include "preferences.h"



Preferences::Preferences() {

}

Preferences::~Preferences() {

}


Error Preferences::GetInstallDirectory(char* path, uint32* len) {
//    getcwd(path,*len);
    return kError_NoErr;
}


Error Preferences::SetInstallDirectory(char* path) {
    return kError_NoErr;
}


Error Preferences::GetDefaultUI(char* name, uint32* len) {
    return kError_NoErr;
}

Error Preferences::SetDefaultUI(char* name) {
    return kError_NoErr;
}


Error Preferences::GetPrefString(const char* pref, char* buf, uint32* len) {
    return kError_NoErr;
}

Error Preferences::SetPrefString(const char* pref, char* buf) {
    return kError_NoErr;
}

HANDLE Preferences::GetFirstLibDir(char *path, uint32 *len) {
    // if no FREEAMP_PATH, libdirs = ~/.freeamp : @lib_installdir@/freeamp : .
    // if FREEAMP_PATH, then its FREEAMP_PATH
    char *pEnv = getenv("FREEAMP_PATH");
    char *pPath = NULL;
    if (pEnv) {
//	cout << "Using env: " << pEnv << endl;
	pPath = strdup(pEnv);
    } else {
	pPath = new char[1024];
	strcpy(pPath,".:~/.freeamp:");
	strcat(pPath,UNIX_LIBDIR);
	strcat(pPath,"/freeamp");
//	cout << "Using default: " << pPath << endl;
    }
    pEnv = pPath;
    LibDirFindHandle *pldfh = new LibDirFindHandle();
    pldfh->m_pLibDirs = new Vector<char *>();
    pldfh->m_current = 0;

    char *pCol = (char *)1;
    char *pPart = pPath;
    while (pCol) {
	pCol = strchr(pPart,':');
	if (pCol) *pCol = '\0';
	char *pFoo = strdup(pPart);
	pldfh->m_pLibDirs->Insert(pFoo);
	pPart = pCol + sizeof(char);
    }

    pPath = pldfh->m_pLibDirs->ElementAt(0);
    if (pPath) {
	strncpy(path,pPath,*len);
	*len = strlen(pPath);
    } else {
	*path = '\0';
	*len = 0;
	delete pldfh->m_pLibDirs;
	delete pldfh;
	pldfh = NULL;
    }

    if (pEnv) delete pEnv;
    //cout << "returning " << path << endl;
    return (HANDLE)pldfh;
}


Error Preferences::GetNextLibDir(HANDLE hLibDirFind,char *path, uint32 *len) {
    if (hLibDirFind) {
	LibDirFindHandle *pldfh = (LibDirFindHandle *)hLibDirFind;
	pldfh->m_current++;
	char *pPath = pldfh->m_pLibDirs->ElementAt(pldfh->m_current);
	if (pPath) {
	    strncpy(path,pPath,*len);
	    *len = strlen(pPath);
//	    cout << "returning next: " << path << endl;
	    return kError_NoErr;
	} else {
	    *path = '\0';
	    *len = 0;
//	    cout << "returning no next " << path << endl;
	    return kError_NoMoreLibDirs;
	}
    }
    return kError_NoMoreLibDirs;
}

Error Preferences::GetLibDirClose(HANDLE hLibDirFind) {
    if (hLibDirFind) {
	LibDirFindHandle *p = (LibDirFindHandle *)hLibDirFind;
	p->m_pLibDirs->DeleteAll();
	delete p->m_pLibDirs;
	delete p;
    }
    return kError_NoErr;
}




