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

#include "preferences.h"


#define DEFAULT_UI  

#if 0
const HKEY  kMainKey = HKEY_CURRENT_USER;
const char* kFreeAmpKey = "SOFTWARE\\FreeAmp\\FreeAmp v1.0";
const char* kInstallDirPref = "InstallDirectory";
const char* kUIPref = "UI";
const char* kDefaultUI = "simple";
#endif

Preferences::Preferences() {
#if 0
    LONG    result;
    DWORD   type;
	DWORD   size;
    char    temp[MAX_PATH] = {0x00};
    char    cwd[MAX_PATH]= {0x00};

    // Where are we starting the program from?
    GetCurrentDirectory(sizeof(cwd), cwd);

    m_prefsKey = NULL;

    result = RegOpenKeyEx(kMainKey,kFreeAmpKey,0,KEY_ALL_ACCESS,&m_prefsKey);
    if(result == ERROR_SUCCESS)	{
        // people DO move their apps around on windows
	result = RegQueryValueEx(m_prefsKey,kInstallDirPref, NULL, &type, (LPBYTE)temp, &size);
        if(strcmp(cwd, temp)) {
            result = RegSetValueEx( m_prefsKey, kInstallDirPref, NULL, REG_SZ, (LPBYTE)cwd, strlen(cwd) + 1);
        }
    } else { // keys need to be created for the first time
        DWORD disposition;
        // create the key in the windows registry
        result = RegCreateKeyEx(kMainKey,kFreeAmpKey, NULL,  "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_prefsKey, &disposition);

        if(result == ERROR_SUCCESS) {
            // set install directory value
            result = RegSetValueEx( m_prefsKey, kInstallDirPref, NULL, REG_SZ, (LPBYTE)cwd, strlen(cwd) + 1);
            // set default ui value
            result = RegSetValueEx( m_prefsKey, kUIPref, NULL, REG_SZ, (LPBYTE)kDefaultUI, strlen(kDefaultUI) + 1);
        }
    }
#endif
}

Preferences::~Preferences() {
#if 0
    if(m_prefsKey) {
        RegCloseKey(m_prefsKey);
    }
#endif
}


Error Preferences::GetInstallDirectory(char* path, uint32* len) {
    getcwd(path,*len);
    return kError_NoErr;
#if 0
    return GetPrefString(kInstallDirPref, path, len);
#endif
}


Error Preferences::SetInstallDirectory(char* path) {
    return kError_NoErr;
#if 0
    return SetPrefString(kInstallDirPref, path);
#endif
}


Error Preferences::GetDefaultUI(char* name, uint32* len) {
    return kError_NoErr;
#if 0
    return GetPrefString(kUIPref, name, len);
#endif
}

Error Preferences::SetDefaultUI(char* name) {
    return kError_NoErr;
#if 0
    return SetPrefString(kUIPref, name);
#endif
}


Error Preferences::GetPrefString(const char* pref, char* buf, uint32* len) {
    return kError_NoErr;
#if 0
    Error   error = kError_UnknownErr;
    DWORD   type;
    LONG    result;
    
    if(!buf || !len) {
        error = kError_InvalidParam;
        return error;
    }
    *buf = 0x00;
    if(m_prefsKey) {
	result = RegQueryValueEx(   m_prefsKey, pref, NULL, &type, (LPBYTE)buf, (DWORD*)len);
        if(result == ERROR_SUCCESS)
            error = kError_NoErr;
        else if(result == ERROR_MORE_DATA)
            error = kError_BufferTooSmall;    
    } else {
        error = kError_NoPrefs;
    }
    return error;
#endif
}

Error Preferences::SetPrefString(const char* pref, char* buf) {
    return kError_NoErr;
#if 0
    Error   error = kError_UnknownErr;
    LONG    result;
    
    if(!buf) {
        error = kError_InvalidParam;
        return error;
    }
    *buf = 0x00;
    if(m_prefsKey) {
		// set install directory value
        result = RegSetValueEx( m_prefsKey, pref, NULL, REG_SZ, (LPBYTE)buf, strlen(buf) + 1);
        if(result == ERROR_SUCCESS)
            error = kError_NoErr;
    } else {
        error = kError_NoPrefs;
    }
    
    return error;
#endif
}
