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

#include <stdio.h>
#include <assert.h>
#include "win32prefs.h"

// location
const HKEY  kMainKey = HKEY_CURRENT_USER;
//const char* kPrefsKey = "SOFTWARE\\FreeAmp\\FreeAmp v1.2\\Main";
const char* kFreeAmpKey = "SOFTWARE\\FreeAmp";
const char* kFreeAmpVersionKey = "FreeAmp v1.2";
const char* kMainComponentKey = "Main";

// default values
const char*  kDefaultUI = "freeamp.ui";
const char*  kDefaultPMO = "soundcard.pmo";
const bool   kDefaultStayOnTop = false;
const bool   kDefaultLiveInTray = false;
const int32  kDefaultWindowPosition = 0;


Win32Prefs::
Win32Prefs()
{
    LONG    result;
    char*   prefsKey = NULL;
    int32   keyLength = strlen(kFreeAmpKey) + 
                        strlen(kFreeAmpVersionKey) +
                        strlen(kMainComponentKey) + 3;

    m_prefsKey = NULL;

    prefsKey = new char[keyLength];

    sprintf(prefsKey, "%s\\%s\\%s", kFreeAmpKey, 
                                    kFreeAmpVersionKey, 
                                    kMainComponentKey);

    result = RegOpenKeyEx(	kMainKey,
							prefsKey,
							0, 
							KEY_ALL_ACCESS,
							&m_prefsKey);

    delete [] prefsKey;

    if(result == ERROR_SUCCESS)
		Initialize();
	else
        m_prefsKey = NULL;
}

Win32Prefs::
Win32Prefs(const char* componentName)
{
    LONG    result;
    char*   prefsKey = NULL;
    int32   keyLength = strlen(kFreeAmpKey) + 
                        strlen(kFreeAmpVersionKey) + 3;

    assert(componentName);

    m_prefsKey = NULL;

    if(componentName)
    {
        keyLength += strlen(componentName);

        prefsKey = new char[keyLength];

        sprintf(prefsKey, "%s\\%s\\%s", kFreeAmpKey, 
                                        kFreeAmpVersionKey, 
                                        componentName);

        result = RegOpenKeyEx(	kMainKey,
							    prefsKey,
							    0, 
							    KEY_ALL_ACCESS,
							    &m_prefsKey);

        delete [] prefsKey;

        if(result != ERROR_SUCCESS)
        {
            DWORD disposition;
            HKEY freeampKey;
            HKEY versionKey;

            // create the main key in the windows registry
            result = RegCreateKeyEx(kMainKey,
                                    kFreeAmpKey,
                                    NULL, 
                                    "",
                                    REG_OPTION_NON_VOLATILE,
                                    KEY_ALL_ACCESS,
                                    NULL,
                                    &freeampKey,
                                    &disposition);

            if(result == ERROR_SUCCESS)
            {
                // create the version key under the freeamp key
                result = RegCreateKeyEx(freeampKey,
                                        kFreeAmpVersionKey,
                                        NULL, 
                                        "",
                                        REG_OPTION_NON_VOLATILE,
                                        KEY_ALL_ACCESS,
                                        NULL,
                                        &versionKey,
                                        &disposition);
            }

            if(result == ERROR_SUCCESS)
            {
                // create the version key under the freeamp key
                result = RegCreateKeyEx(versionKey,
                                        componentName,
                                        NULL, 
                                        "",
                                        REG_OPTION_NON_VOLATILE,
                                        KEY_ALL_ACCESS,
                                        NULL,
                                        &m_prefsKey,
                                        &disposition);
            }

            if(result != ERROR_SUCCESS)
            {
                m_prefsKey = NULL;
            }
        }
		Initialize();
    }
}

Win32Prefs::
~Win32Prefs()
{
    if(m_prefsKey)
    {
        RegCloseKey(m_prefsKey);
    }
}

Error
Win32Prefs::
Initialize()
{
    LONG    result;
	uint32  length;
    char    path[MAX_PATH] = {0x00};
    char    cwd[MAX_PATH]= {0x00};
    Error   error = kError_UnknownErr;

    // Where are we starting the program from?
    GetCurrentDirectory(sizeof(cwd), cwd);

    if(m_prefsKey)
	{
        // people DO move their apps around on windows
        length = sizeof(path);

        error = GetInstallDirectory(path, &length);

		char foo[MAX_PATH] = {0x00};
		sprintf(foo,"%s\\plugins",cwd);
		WIN32_FIND_DATA win32fd;

        // check for plugins directory in cwd
		HANDLE h = FindFirstFile(foo,&win32fd);

		if (h != INVALID_HANDLE_VALUE) 
        {
			if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
            {
				if(IsError(error) || strcmp(cwd, path))
				{
					result = RegSetValueEx( m_prefsKey,
											kInstallDirPref, 
											NULL, 
											REG_SZ, 
											(LPBYTE)cwd, 
											strlen(cwd) + 1);
				}
			}

            FindClose(h);
		}
        error = kError_NoErr;
    }
    else // keys need to be created for the first time
    {
        DWORD disposition;
        HKEY freeampKey;
        HKEY versionKey;

        // create the main key in the windows registry
        result = RegCreateKeyEx(kMainKey,
                                kFreeAmpKey,
                                NULL, 
                                "",
                                REG_OPTION_NON_VOLATILE,
                                KEY_ALL_ACCESS,
                                NULL,
                                &freeampKey,
                                &disposition);

        if(result == ERROR_SUCCESS)
        {
            // create the version key under the freeamp key
            result = RegCreateKeyEx(freeampKey,
                                    kFreeAmpVersionKey,
                                    NULL, 
                                    "",
                                    REG_OPTION_NON_VOLATILE,
                                    KEY_ALL_ACCESS,
                                    NULL,
                                    &versionKey,
                                    &disposition);
        }

        if(result == ERROR_SUCCESS)
        {
            // create the version key under the freeamp key
            result = RegCreateKeyEx(versionKey,
                                    kMainComponentKey,
                                    NULL, 
                                    "",
                                    REG_OPTION_NON_VOLATILE,
                                    KEY_ALL_ACCESS,
                                    NULL,
                                    &m_prefsKey,
                                    &disposition);
        }

        if(result == ERROR_SUCCESS)
            error = SetDefaults();
        else
            error = kError_NoPrefs;

        RegCloseKey(freeampKey);
        RegCloseKey(versionKey);
    }

    return error;
}

Error
Win32Prefs::
SetDefaults()
{
    char    cwd[MAX_PATH]= {0x00};

    // Where are we starting the program from?
    GetCurrentDirectory(sizeof(cwd), cwd);

    // set install directory value
    SetPrefString(kInstallDirPref, cwd);  // XXX: Use real value for Unix
    
    // set default ui value
    SetPrefString(kUIPref, kDefaultUI);
    
    // set default pmo value
    SetPrefString(kPMOPref, kDefaultPMO);

    // set default open/save dlg path value
    SetPrefString(kOpenSaveDirPref, cwd); // XXX: What for Unix?

    // set default for window staying on top
    SetPrefBoolean(kStayOnTopPref, kDefaultStayOnTop);

    // set default for minimizing to tray
    SetPrefBoolean(kLiveInTrayPref, kDefaultLiveInTray);

    // set default for window position
    SetPrefInt32(kWindowPositionLeftPref, kDefaultWindowPosition);
    SetPrefInt32(kWindowPositionTopPref, kDefaultWindowPosition);
    SetPrefInt32(kWindowPositionWidthPref, kDefaultWindowPosition);
    SetPrefInt32(kWindowPositionHeightPref, kDefaultWindowPosition);

    Preferences::SetDefaults();

    return kError_NoErr;
}

Error
Win32Prefs::
Save()
{
	// XXX: Should we do anything more here?
	return kError_NoErr;
}

Preferences *
Win32Prefs::
ComponentPrefs(const char *componentName)
{
	return new Win32Prefs(componentName);
}

Error
Win32Prefs::
GetPrefString(const char* pref, char* buf, uint32* len)
{
    Error   error = kError_UnknownErr;
	DWORD   type;
	LONG    result;

    if(!buf || !len || !pref)
    {
        error = kError_InvalidParam;
        return error;
    }

    *buf = 0x00;

	if(m_prefsKey)
	{
		result = RegQueryValueEx(   m_prefsKey,
                                    pref, 
                                    NULL, 
                                    &type, 
                                    (LPBYTE)buf, 
                                    (DWORD*)len);

        if(result == ERROR_SUCCESS)
            error = kError_NoErr;
        else if(result == ERROR_MORE_DATA)
            error = kError_BufferTooSmall;    
	    
    }
    else
    {
        error = kError_NoPrefs;
    }
  
    return error;
}

Error
Win32Prefs::
SetPrefString(const char* pref, const char* buf)
{
    Error   error = kError_UnknownErr;
	LONG    result;

    if(!buf || !pref)
    {
        error = kError_InvalidParam;
        return error;
    }

	if(m_prefsKey)
	{
		// set install directory value
        result = RegSetValueEx( m_prefsKey,
                                pref, 
                                NULL, 
                                REG_SZ, 
                                (LPBYTE)buf, 
                                strlen(buf) + 1);

        if(result == ERROR_SUCCESS)
            error = kError_NoErr;
    }
    else
    {
        error = kError_NoPrefs;
    }
  
    return error;
}

Error 
Win32Prefs::
GetPrefBoolean(const char* pref, bool* value)
{
    Error   error = kError_UnknownErr;
	DWORD   type;
	LONG    result;
    DWORD   buf = 0;
    DWORD   len = sizeof(DWORD);

    if(!value || !pref)
    {
        error = kError_InvalidParam;
        return error;
    }

	if(m_prefsKey)
	{
		result = RegQueryValueEx(   m_prefsKey,
                                    pref, 
                                    NULL, 
                                    &type, 
                                    (LPBYTE)&buf, 
                                    (DWORD*)&len);

        if(result == ERROR_SUCCESS)
        {
            error = kError_NoErr;

            *value = (buf == 0 ? false : true);
        }
        else if(result == ERROR_MORE_DATA)
            error = kError_BufferTooSmall;    
    }
    else
    {
        error = kError_NoPrefs;
    }
  
    return error;
}

Error
Win32Prefs:: 
SetPrefBoolean(const char* pref, bool value)
{
    Error   error = kError_UnknownErr;
	LONG    result;
    DWORD   buf = (value ? 1 : 0);

    if(!pref)
    {
        error = kError_InvalidParam;
        return error;
    }


	if(m_prefsKey)
	{
		// set install directory value
        result = RegSetValueEx( m_prefsKey,
                                pref, 
                                NULL, 
                                REG_DWORD , 
                                (LPBYTE)&buf, 
                                sizeof(DWORD));

        if(result == ERROR_SUCCESS)
            error = kError_NoErr;
    }
    else
    {
        error = kError_NoPrefs;
    }
  
    return error;
}

Error 
Win32Prefs::
GetPrefInt32(const char* pref, int32* value)
{
    Error   error = kError_UnknownErr;
	DWORD   type;
	LONG    result;
    DWORD   buf = 0;
    DWORD   len = sizeof(DWORD);

    if(!value || !pref)
    {
        error = kError_InvalidParam;
        return error;
    }

	if(m_prefsKey)
	{
		result = RegQueryValueEx(   m_prefsKey,
                                    pref, 
                                    NULL, 
                                    &type, 
                                    (LPBYTE)&buf, 
                                    (DWORD*)&len);

        if(result == ERROR_SUCCESS)
        {
            error = kError_NoErr;

            *value = (int32)buf;
        }
        else if(result == ERROR_MORE_DATA)
            error = kError_BufferTooSmall;    
    }
    else
    {
        error = kError_NoPrefs;
    }
  
    return error;
}

Error
Win32Prefs:: 
SetPrefInt32(const char* pref, int32 value)
{
    Error   error = kError_UnknownErr;
	LONG    result;
    DWORD   buf = value;

    if(!pref)
    {
        error = kError_InvalidParam;
        return error;
    }


    if(m_prefsKey)
	{
	    // set install directory value
        result = RegSetValueEx( m_prefsKey,
                                pref, 
                                NULL, 
                                REG_DWORD , 
                                (LPBYTE)&buf, 
                                sizeof(DWORD));

        if(result == ERROR_SUCCESS)
            error = kError_NoErr;
    }
    else
    {
        error = kError_NoPrefs;
    }
  
    return error;
}
