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
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include "ThemeManager.h"
#include "ThemeZip.h"
#include "utility.h"
#include "win32impl.h"

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
    char              dir[_MAX_PATH];
    uint32            len = sizeof(dir);

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oThemePath = string(dir);
    oThemePath += string("/themes/FreeAmp.fat");    

    return kError_NoErr;
}

Error ThemeManager::GetThemeList(map<string, string> &oThemeFileMap)
{
    WIN32_FIND_DATA find;
    HANDLE          handle;
    char            dir[_MAX_PATH], *ptr;
    uint32          len = sizeof(dir);
    string          oThemePath, oThemeBasePath, oThemeFile;

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oThemeBasePath = string(dir) + "/themes";
    oThemePath = oThemeBasePath + string("/*.fat");    

    handle = FindFirstFile((char *)oThemePath.c_str(), &find);
    if(handle != INVALID_HANDLE_VALUE)
        do {
   	    oThemeFile = oThemeBasePath + string("/") + string(find.cFileName);
    	    ptr = strrchr(find.cFileName, '.');
            if (ptr)
               *ptr = 0;

            oThemeFileMap[find.cFileName] = oThemeFile;
        }
        while(FindNextFile(handle, &find));

    oThemeBasePath = FreeampDir(NULL) + string("/themes");

    struct stat st;
    if (-1 == stat(oThemeBasePath.c_str(), &st))
        mkdir(oThemeBasePath.c_str(), 0755);
    oThemePath = oThemeBasePath + string("/*.fat");
    handle = FindFirstFile((char *)oThemePath.c_str(), &find);
    if(handle != INVALID_HANDLE_VALUE)
        do {
            oThemeFile = oThemeBasePath + string("/") + string(find.cFileName);
            ptr = strrchr(find.cFileName, '.');
            if (ptr)
               *ptr = 0;
            oThemeFileMap[find.cFileName] = oThemeFile;
        }
        while(FindNextFile(handle, &find));

#if 0
    oThemeBasePath = "./themes";
    oThemePath = oThemeBasePath + string("/*.fat");
    handle = FindFirstFile((char *)oThemePath.c_str(), &find);
    if(handle != INVALID_HANDLE_VALUE)
        do {
            oThemeFile = oThemeBasePath + string("/") + string(find.cFileName);
            ptr = strrchr(find.cFileName, '.');
            if (ptr)
               *ptr = 0;
            oThemeFileMap[find.cFileName] = oThemeFile;
        }
        while(FindNextFile(handle, &find));
#endif

    return kError_NoErr;
}

Error ThemeManager::UseTheme(string &oThemeFile)
{
    char   dir[_MAX_PATH];
    string oThemePath;

    char *temp_dir;
    temp_dir = strrchr(oThemeFile.c_str(), '/');
    if (temp_dir) {
        temp_dir = temp_dir + 1;
	strcpy(dir, temp_dir);
        if (oThemeFile == m_oCurrentTheme)
        {
            return kError_NoErr;
        }
    }

    m_pContext->prefs->SetThemePath((char *)oThemeFile.c_str());
    m_oCurrentTheme = string(dir);
    
    return kError_NoErr;
}

Error ThemeManager::AddTheme(string &oThemeFile, bool bRename)
{
    string oThemeDest;
    bool bRenameFailed = false;
    char fcopy[_MAX_PATH], *filename = NULL, *ext = NULL;

    oThemeDest = FreeampDir(NULL) + string("/themes");
    if (bRename)
    {
        ThemeZip oZip;
        string   oName;

        if (IsntError(oZip.GetDescriptiveName(oThemeFile, oName)) &&
            oName.length() > 0)
        {
            int i;

            for (i = oName.length() - 1; i >= 0; i--)
                if (!isalnum(oName[i]) && oName[i] != ' ')
                    oName.erase(i, 1);

            oThemeDest += string("/") + oName + string(".fat");
        }
        else
            bRenameFailed = true;
    }

    if (!bRename || bRenameFailed)
    {
        strcpy(fcopy, oThemeFile.c_str());
        filename = strrchr(fcopy, '/');
        if (filename)
            filename = filename + 1;
        else
            filename = fcopy;
        ext = strrchr(filename, '.');
        if (ext) {
            *ext = '\0';
            ext++;
        }
        if (!filename)
            filename = "unknown";

        oThemeDest += string("/") + string(filename);
        if (ext)
            oThemeDest += string(".") + string(ext);   
    }

    if (!CopyFile(oThemeFile.c_str(), oThemeDest.c_str(), false))
        return kError_FileNotFound;

    // So the caller knows where the theme ended up
    oThemeFile = oThemeDest;
    
    return kError_NoErr;
}

Error ThemeManager::DeleteTheme(string &oThemeFile)
{
    return unlink(oThemeFile.c_str()) == 0 ? kError_NoErr : kError_UnlinkFailed;
}

Error ThemeManager::GetCurrentTheme(string &oTheme)
{
    oTheme = m_oCurrentTheme;
    return kError_NoErr;
}
