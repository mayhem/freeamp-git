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

#include <windows.h>
#include <windowsx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

#include "config.h"
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"
#include "debug.h"

void MusicBrowserUI::AddPlaylist(const string &oName)
{
    vector<PlaylistItem *>            oList;
    vector<PlaylistItem *>::iterator  j;
    int                               i;
    char                              url[MAX_PATH];
    uint32                            len = MAX_PATH;

    FilePathToURL(oName.c_str(), url, &len);
    m_oPlm->SetActivePlaylist(kPlaylistKey_ExternalPlaylist);
    m_oPlm->SetExternalPlaylist(url);
    for(i = 0; i < m_oPlm->CountItems(); i++)
        oList.push_back(m_oPlm->ItemAt(i));

    m_oPlm->SetActivePlaylist(kPlaylistKey_MasterPlaylist);
    for(j = oList.begin(); j != oList.end(); j++)
        m_oPlm->AddItem(new PlaylistItem(*(*j)), true);

    m_bListChanged = true;

    //UpdatePlaylistList();
}

void MusicBrowserUI::LoadPlaylist(const string &oPlaylist)
{
    m_oPlm->RemoveAll();
    m_oPlm->ReadPlaylist((char *)oPlaylist.c_str());
       
    m_currentindex = 0;
    //UpdatePlaylistList();
}

void MusicBrowserUI::WritePlaylist(void)
{
    PlaylistFormatInfo     oInfo;              
    char                   ext[MAX_PATH];
    Error                  eRet = kError_NoErr;
    vector<PlaylistItem *> oTempList;
    int                    i;
    bool                   bNewList = false;

    if (!m_bListChanged)
        return;

    if (!m_pParent)
    {
//        string lastPlaylist = FreeampDir(m_context->prefs);
//        lastPlaylist += "\\currentlist.m3u";
    
//        SaveCurrentPlaylist((char *)lastPlaylist.c_str());  

        m_bListChanged = false;
        return;
    }
  
    Debug_v("items: %d", m_oPlm->CountItems());
    if (m_currentListName.length() == 0)
    {
        if (SaveNewPlaylist(m_currentListName))
           bNewList = true;
        else
        {
           return;
        }   
        Debug_v("items: %d", m_oPlm->CountItems());
        SetTitles();
        Debug_v("items: %d", m_oPlm->CountItems());
    }

    Debug_v("items: %d", m_oPlm->CountItems());
    _splitpath(m_currentListName.c_str(), NULL, NULL, NULL, ext);
    for(i = 0; ; i++)
    {
        eRet = m_oPlm->GetSupportedPlaylistFormats(&oInfo, i);
        if (IsError(eRet))
           break;

        if (strcasecmp(oInfo.GetExtension(), ext + 1) == 0)
           break;   
    }
    Debug_v("items: %d", m_oPlm->CountItems());
    if (!IsError(eRet))
    {
        char   url[MAX_PATH];
        uint32 len = MAX_PATH;
        
        FilePathToURL(m_currentListName.c_str(), url, &len);
        Debug_v("items: %d", m_oPlm->CountItems());
        Debug_v("Writing playlist to: %s", url);
        eRet = m_oPlm->WritePlaylist(url, &oInfo);   
        if (IsError(eRet))
        {
           MessageBox(m_hWnd, "Cannot save playlist to disk. Out of disk space?", BRANDING, MB_OK);                              
           return;
        }
    }
    if (bNewList)
    {
        m_context->browser->m_catalog->AddPlaylist(m_currentListName.c_str());
    }

    m_bListChanged = false;
}

void MusicBrowserUI::SaveAsPlaylist(void)
{
    string oName;
    
    if (SaveNewPlaylist(oName))
    {
       m_currentListName = oName;
       SetTitles();
       WritePlaylist();
    }   
}

bool MusicBrowserUI::SaveNewPlaylist(string &oName)
{
    int32              i, iOffset = 0;
    uint32             size;
    PlaylistFormatInfo format;
    char               szFilter[512];
    OPENFILENAME       sOpen;
    char               szFile[MAX_PATH], szInitialDir[MAX_PATH];
        
    for(i = 0; ; i++)
    {
       if (m_oPlm->GetSupportedPlaylistFormats(&format, i) != kError_NoErr)
          break;
    
       sprintf(szFilter + iOffset, "%s (.%s)", 
            format.GetDescription(),
            format.GetExtension());
       iOffset += strlen(szFilter + iOffset) + 1;     

       sprintf(szFilter + iOffset, "*.%s", 
            format.GetExtension());
       iOffset += strlen(szFilter + iOffset) + 1;     
    }
    
    strcpy(szFilter + iOffset, "All Files (*.*)\0");
    iOffset += strlen(szFilter + iOffset) + 1;     
    strcpy(szFilter + iOffset, "*.*\0");
    iOffset += strlen(szFilter + iOffset) + 1;     
    szFilter[iOffset] = 0;

    size = MAX_PATH;
    if (!IsError(m_context->prefs->GetInstallDirectory(szInitialDir, &size)))
    {
        struct _stat buf;
        
        strcat(szInitialDir, "\\Playlists");
        if (_stat(szInitialDir, &buf) != 0)
           _mkdir(szInitialDir);
           
        sOpen.lpstrInitialDir = szInitialDir;
    }
    else
        sOpen.lpstrInitialDir = NULL;
    
    szFile[0] = 0;
    sOpen.lStructSize = sizeof(OPENFILENAME);
    sOpen.hwndOwner = m_hWnd;
    sOpen.hInstance = NULL;
    sOpen.lpstrFilter = szFilter;
    sOpen.lpstrCustomFilter = NULL;
    sOpen.nMaxCustFilter = 0;
    sOpen.nFilterIndex = 1;
    sOpen.lpstrFile = szFile;
    sOpen.nMaxFile = MAX_PATH;
    sOpen.lpstrFileTitle = NULL;
    sOpen.lpstrTitle = "Save Playlist as";
    sOpen.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
                  OFN_PATHMUSTEXIST;
    sOpen.lpstrDefExt = "m3u";
      
    if (GetSaveFileName(&sOpen))
    {
        oName = sOpen.lpstrFile;
        return true;
    }
    return false;
}

void MusicBrowserUI::OpenPlaylist(void)
{
    int32              i, iOffset = 0;
    PlaylistFormatInfo format;
    char               szFilter[512];
    OPENFILENAME       sOpen;
    char               szFile[MAX_PATH];
        
    for(i = 0; ; i++)
    {
       if (m_oPlm->GetSupportedPlaylistFormats(&format, i) != kError_NoErr)
          break;
    
       sprintf(szFilter + iOffset, "%s (.%s)", 
            format.GetDescription(),
            format.GetExtension());
       iOffset += strlen(szFilter + iOffset) + 1;     

       sprintf(szFilter + iOffset, "*.%s", 
            format.GetExtension());
       iOffset += strlen(szFilter + iOffset) + 1;     
    }
    
    strcpy(szFilter + iOffset, "All Files (*.*)\0");
    iOffset += strlen(szFilter + iOffset) + 1;     
    strcpy(szFilter + iOffset, "*.*\0");
    iOffset += strlen(szFilter + iOffset) + 1;     
    szFilter[iOffset] = 0;
    
    szFile[0] = 0;
    sOpen.lStructSize = sizeof(OPENFILENAME);
    sOpen.hwndOwner = m_hWnd;
    sOpen.hInstance = NULL;
    sOpen.lpstrFilter = szFilter;
    sOpen.lpstrCustomFilter = NULL;
    sOpen.nMaxCustFilter = 0;
    sOpen.nFilterIndex = 1;
    sOpen.lpstrFile = szFile;
    sOpen.nMaxFile = MAX_PATH;
    sOpen.lpstrFileTitle = NULL;
    sOpen.lpstrInitialDir = NULL;
    sOpen.lpstrTitle = "Open Playlist";
    sOpen.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    sOpen.lpstrDefExt = "m3u";
      
    if (GetOpenFileName(&sOpen))
    {
        string playlist = sOpen.lpstrFile;
        
        //m_context->browser->m_catalog->AddPlaylist(sOpen.lpstrFile);
        //InitTree();
        //FillPlaylistCombo();
        LoadPlaylist(playlist);
    }
}