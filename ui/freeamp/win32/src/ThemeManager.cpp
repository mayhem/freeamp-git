/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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
#ifdef WIN32
#include <winsock.h>
#else
#include <netinet/in.h>
#endif
#include "ThemeManager.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);

ThemeManager::ThemeManager(FAContext *pContext)
{
    m_pContext = pContext;
	m_oCurrentTheme = "";
}

ThemeManager::~ThemeManager(void)
{
}

Error ThemeManager::GetDefaultTheme(string &oThemePath)
{
    char              dir[MAX_PATH];
    uint32            len = sizeof(dir);

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oThemePath = string(dir);
    oThemePath += string("\\themes\\freeamp.fat");    

	return kError_NoErr;
}

Error ThemeManager::GetThemeList(map<string, string> &oThemeFileMap)
{
    WIN32_FIND_DATA find;
    HANDLE          handle;
    char            dir[MAX_PATH], *ptr;
    uint32          len = sizeof(dir);
    string          oThemePath, oThemeBasePath, oThemeFile;

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oThemeBasePath = string(dir) + "\\themes";
    oThemePath = oThemeBasePath + string("\\*.fat");    

    handle = FindFirstFile(oThemePath.c_str(), &find);
    if(handle == INVALID_HANDLE_VALUE)
        return kError_NoErr;

	do
    {
		oThemeFile = oThemeBasePath + string("\\") + 
                     string(find.cFileName);
    	ptr = strrchr(find.cFileName, '.');
        if (ptr)
           *ptr = 0;

        oThemeFileMap[find.cFileName] = oThemeFile;
    }
    while(FindNextFile(handle, &find));

	return kError_NoErr;
}

Error ThemeManager::UseTheme(string &oThemeFile)
{
    char   dir[MAX_PATH];
    uint32 len = sizeof(dir);
    string oThemePath;

	_splitpath(oThemeFile.c_str(), NULL, NULL, dir, NULL);
    if (strcmp(dir, m_oCurrentTheme.c_str()) == 0)
    {
       return kError_NoErr;
    }   

    m_pContext->prefs->SetThemePath((char *)oThemeFile.c_str());
	m_oCurrentTheme = string(dir);
    
	return kError_NoErr;
}

Error ThemeManager::AddTheme(string &oThemeFile)
{
    char            dir[MAX_PATH], ext[MAX_PATH];
    uint32          len = sizeof(dir);
    string          oThemeDest;

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oThemeDest = string(dir);
    
	_splitpath(oThemeFile.c_str(), NULL, NULL, dir, ext);
    if (strcmp(dir, m_oCurrentTheme.c_str()) == 0)
    {
       return kError_NoErr;
    }   

	oThemeDest += string("\\themes\\") + string(dir) + string(ext);
    return CopyFile(oThemeFile.c_str(), oThemeDest.c_str(), false) ? 
           kError_NoErr : kError_CopyFailed;
}

Error ThemeManager::DeleteTheme(string &oThemeFile)
{
	return _unlink(oThemeFile.c_str()) == 0 ? kError_NoErr : kError_UnlinkFailed;
}

Error ThemeManager::GetCurrentTheme(string &oTheme)
{
	oTheme = m_oCurrentTheme;
	return kError_NoErr;
}

    

