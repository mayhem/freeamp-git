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
#include <sys/types.h>
#include <sys/stat.h>
#ifdef WIN32
#include <winsock.h>
#else
#include <netinet/in.h>
#endif
#include "ThemeZip.h"
#include "ThemeManager.h"
#include "debug.h"

#define DB Debug_v("%s:%d\n", __FILE__, __LINE__);
#define THEME_IN_DEVEL "<theme in development>"

ThemeManager::ThemeManager(FAContext *pContext)
{
    char     szThemePath[MAX_PATH];
    uint32   len = MAX_PATH;
    Error    eRet;
    string   oThemeName;
    ThemeZip oZip;
    
    m_pContext = pContext;
    m_oCurrentTheme = "";
    m_bDevelTheme = false;
    
    szThemePath[0] = 0;
    eRet = pContext->prefs->GetThemePath(szThemePath, &len);
    if (IsError(eRet) || strlen(szThemePath) == 0)
    {
        GetDefaultTheme(m_oCurrentTheme);
        _splitpath(m_oCurrentTheme.c_str(), NULL, NULL, szThemePath, NULL);
        m_oCurrentTheme = szThemePath;
    }    
    else
    {
        struct   _stat buf;

        m_oCurrentTheme = szThemePath;
        if (_stat(szThemePath, &buf) == 0 && (buf.st_mode & _S_IFDIR))
        {
           m_bDevelTheme = true;
           m_oDevelTheme = m_oCurrentTheme;
           m_oCurrentTheme = THEME_IN_DEVEL;
        }   
        else
        {
           oZip.GetDescriptiveName(m_oCurrentTheme,oThemeName);
           if (oThemeName.size() > 0)
               m_oCurrentTheme = oThemeName;
           else
           {    
               _splitpath(m_oCurrentTheme.c_str(), NULL, NULL, szThemePath, NULL);
               m_oCurrentTheme = szThemePath;
           }    
        }   
    }    
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
    string          oThemeName;
    ThemeZip        oZip;

    if (m_bDevelTheme)
       oThemeFileMap[THEME_IN_DEVEL] = m_oDevelTheme;

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

        // check for extended info
        oZip.GetDescriptiveName(oThemeFile,oThemeName); 
        if (oThemeName.size() > 0)
        {
            oThemeFileMap[oThemeName] = oThemeFile;
        }
        else
        {
            // fine... use the filename instead.
            ptr = strrchr(find.cFileName, '.');
            if (ptr)
               *ptr = 0;
            oThemeFileMap[find.cFileName] = oThemeFile;
        }
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
    if (oThemeFile == m_oDevelTheme)
        m_oCurrentTheme = THEME_IN_DEVEL;
    else    
        m_oCurrentTheme = string(dir);

    return kError_NoErr;
}

Error ThemeManager::AddTheme(string &oThemeFile)
{
    char            dir[MAX_PATH], ext[MAX_PATH];
    uint32          len = sizeof(dir);
    string          oThemeDest;
    Error           eErr;

    m_pContext->prefs->GetInstallDirectory(dir, &len);
    oThemeDest = string(dir);
    
    _splitpath(oThemeFile.c_str(), NULL, NULL, dir, ext);
    oThemeDest += string("\\themes\\") + string(dir) + string(ext);
    eErr = CopyFile(oThemeFile.c_str(), oThemeDest.c_str(), false) ? 
           kError_NoErr : kError_CopyFailed;

    if (!IsError(eErr))
       // So the caller knows where the theme ended up
       oThemeFile = oThemeDest;
              
    return eErr;       
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

