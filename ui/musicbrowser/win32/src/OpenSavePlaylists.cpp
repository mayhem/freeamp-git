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

#define STRICT
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

void MusicBrowserUI::LoadPlaylist(const string &oPlaylist)
{
    vector<PlaylistItem*> items;

    m_oPlm->RemoveAll();
    m_oPlm->ReadPlaylist((char *)oPlaylist.c_str(), &items);

    m_initialCount = items.size();

    m_oPlm->AddItems(&items);
}

void MusicBrowserUI::SavePlaylist(void)
{
    if(!m_bListChanged)
        return;

    // if this is the root window or this list has no
    // name then treat this as a SaveAs command.
    if(!m_pParent || m_currentListName.length() == 0)
    {
        SavePlaylistAs();
        return;
    }
  
    //char   url[MAX_PATH];
    //uint32 len = MAX_PATH;
    
    //FilePathToURL(m_currentListName.c_str(), url, &len);

    if(IsError(m_oPlm->WritePlaylist(m_currentListName.c_str())))
    {
       MessageBox(m_hWnd, "Cannot save playlist to disk. Make sure there "
                          "is room on the drive or that the directory is "
                          "not read-only?", BRANDING, MB_OK);                              
    }
    else
    {
        m_bListChanged = false;
        UpdateButtonMenuStates();
    }
}

void MusicBrowserUI::SavePlaylistAs(void)
{
    string oName = m_currentListName;
    
    if(SaveNewPlaylist(oName))
    {
        if(m_pParent)
        {
            m_currentListName = oName;
            SetTitles();
        }
    }   
}

bool MusicBrowserUI::SaveNewPlaylist(string &oName)
{
    bool                result = false;
    int32               i, iOffset = 0;
    uint32              length;
    PlaylistFormatInfo  format;
    char                szFilter[512];
    OPENFILENAME        sOpen;
    char                szPlaylistDir[MAX_PATH];
    char                szFile[MAX_PATH] = {0x00};
    char                szInitialDir[MAX_PATH] = {0x00};
    char                szExt[MAX_PATH] = {0x00};
    bool                addToDB = false;
    
    length = MAX_PATH;
    m_context->prefs->GetInstallDirectory(szPlaylistDir, &length);

    strcat(szPlaylistDir, "\\Playlists");

    struct _stat buf;
    if (_stat(szPlaylistDir, &buf) != 0)
       _mkdir(szPlaylistDir);

    if(oName.length())
    {
        length = sizeof(szInitialDir);
        URLToFilePath(oName.c_str(), szInitialDir, &length);

        char* cp = NULL;

        if(cp = strrchr(szInitialDir, '\\'))
        {
            *cp = 0x00;
            strcpy(szFile, cp + 1);

            if(cp = strrchr(szFile, '.'))
            {
                *cp = 0x00;
            }
        }
    }
    else
    {
        char szName[256];
        length = sizeof(szName);

        m_context->prefs->GetUsersName(szName, &length);

        while(*szName && isspace(*szName))
        {
            strcpy(szName, szName + 1);
        }

        if(strlen(szName))
        {
            char* cp = NULL;

            if(cp = strchr(szName, ' '))
            {
                *cp = 0x00;
            }

            sprintf(szFile, "%s\'s Greatest Hits", szName);
        }
        else
        {
            strcpy(szFile, "My Greatest Hits");
        }

        
        HANDLE findFileHandle = NULL;
        WIN32_FIND_DATA findData;
        char findPath[MAX_PATH];
        char tempFile[MAX_PATH];
        uint32 index = 2;

        strcpy(tempFile, szFile);

        do
        {
            sprintf(findPath, "%s\\%s.m3u", szPlaylistDir, tempFile);

            findFileHandle = FindFirstFile(findPath, &findData);
            
            if(findFileHandle != INVALID_HANDLE_VALUE)
            {
                sprintf(tempFile, "%s #%d", szFile, index++);
            }

            FindClose(findFileHandle);

        }while(findFileHandle != INVALID_HANDLE_VALUE);

        strcpy(szFile, tempFile);
        strcpy(szInitialDir, szPlaylistDir);
    }

    // is this playlist saved in our default location?
    // if so we are gonna just ask for the name, if not
    // they are editing a file elsewhere and we bring
    // up a file selector 
    if(strstr(szInitialDir, szPlaylistDir))
    {
        if(0 < DialogBoxParam(g_hinst, 
                          MAKEINTRESOURCE(IDD_SAVEPLAYLIST),
                          m_hWnd, 
                          ::SavePlaylistDlgProc, 
                          (LPARAM )szFile))
        {        
            oName = szInitialDir;
            oName += "\\";
            oName += szFile;
            oName += ".m3u";

            addToDB = true;
            result = true;
        }
    }
    else
    {
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

        sOpen.lStructSize = sizeof(OPENFILENAME);
        sOpen.hwndOwner = m_hWnd;
        sOpen.hInstance = NULL;
        sOpen.lpstrFilter = szFilter;
        sOpen.lpstrCustomFilter = NULL;
        sOpen.nMaxCustFilter = 0;
        sOpen.nFilterIndex = 1;
        sOpen.lpstrInitialDir = szInitialDir;
        sOpen.lpstrFile = szFile;
        sOpen.nMaxFile = MAX_PATH;
        sOpen.lpstrFileTitle = NULL;
        sOpen.lpstrTitle = "Save Playlist As";
        sOpen.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
                      OFN_PATHMUSTEXIST;
        sOpen.lpstrDefExt = "m3u";
      
        if(GetSaveFileName(&sOpen))
        {
            oName = sOpen.lpstrFile;
            result =  true;
        }
    }

    if(result)
    {
        char   url[MAX_PATH + 7]; // make room for file://
        uint32 len = sizeof(url);
    
        FilePathToURL(oName.c_str(), url, &len);
        
        if(IsError(m_oPlm->WritePlaylist(url)))
        {
           MessageBox(m_hWnd, "Cannot save playlist to disk. Make sure there "
                              "is room on the drive or that the directory is "
                              "not read-only?", BRANDING, MB_OK);                              
           result = false;
        }
        else
        {
            if(addToDB)
            {
                m_context->catalog->AddPlaylist(url); 
            }

            if(m_pParent)
            {
                m_currentListName = oName;
                SetTitles();
            }

            m_bListChanged = false;
            UpdateButtonMenuStates();
        }
    }

    return result;
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
        char   url[MAX_PATH + 8];
        uint32 len = sizeof(url);

        FilePathToURL(sOpen.lpstrFile, url, &len);

        string playlist = url;
        
        EditPlaylist(playlist);
    }
}

void MusicBrowserUI::NewPlaylist(void)
{
    MusicBrowserUI *pNew;
    
    if(m_pParent)
    {
       pNew = new MusicBrowserUI(m_context, m_pParent, m_hWnd, string(""));
       m_pParent->AddMusicBrowserWindow(pNew);
    }   
    else   
    {
       pNew = new MusicBrowserUI(m_context, this, m_hWnd, string(""));
       AddMusicBrowserWindow(pNew);
    }   
       
    pNew->Init(SECONDARY_UI_STARTUP);
}

void MusicBrowserUI::EditPlaylist(const string &oList)
{
    MusicBrowserUI *pNew;
    
    if(m_pParent)
    {
       pNew = new MusicBrowserUI(m_context, m_pParent, m_hWnd, oList);
       m_pParent->AddMusicBrowserWindow(pNew);
    }   
    else   
    {
       pNew = new MusicBrowserUI(m_context, this, m_hWnd, oList);
       AddMusicBrowserWindow(pNew);
    }   
       
    pNew->Init(SECONDARY_UI_STARTUP);
}

void MusicBrowserUI::EditPortablePlaylist(DeviceInfo* device)
{
    MusicBrowserUI *pNew;
    
    if(m_pParent)
    {
       pNew = new MusicBrowserUI(m_context, m_pParent, m_hWnd, device);
       m_pParent->AddMusicBrowserWindow(pNew);
    }   
    else   
    {
       pNew = new MusicBrowserUI(m_context, this, m_hWnd, device);
       AddMusicBrowserWindow(pNew);
    }   
       
    pNew->Init(SECONDARY_UI_STARTUP);
}

void MusicBrowserUI::SavePortablePlaylist()
{
    if(m_oPlm->IsPortableAvailable(m_portableDevice))
    {
        m_oPlm->SyncPortablePlaylist(m_portableDevice);
    }
}

void MusicBrowserUI::ImportTracksAndPlaylists(void)
{
    PlaylistFormatInfo format;
    int32 i, iOffset = 0;
    
    char szFilter[1024] = "MPEG Audio Streams (.mp1;.mp2;.mp3;.mpp)\0"
                          "*.mp1;*.mp2;*.mp3;*.mpp\0";

    // we need a way to iterate LMCs...
    iOffset += strlen(szFilter) + 1; 
    iOffset += strlen(szFilter + iOffset) + 1;  
        
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
    
    vector<string> oFileList;

    if (FileOpenDialog(m_hWnd, "Import Tracks and Playlists",
                       szFilter, 
                       &oFileList,
                       m_context->prefs, 
                       false) )
    {
        vector<string>::iterator i;

        for(i = oFileList.begin(); i != oFileList.end(); i++)
        {
            char* ext = NULL;
            char path[MAX_PATH];

            strcpy(path, (*i).c_str());

            ext = strrchr(path, '.');
            
            if(ext && m_oPlm->IsSupportedPlaylistFormat(++ext))
                m_context->catalog->AddPlaylist((*i).c_str());
            else
                m_context->catalog->AddSong((*i).c_str());
        }
    }
}


bool MusicBrowserUI::ExportPlaylist(string &oPlaylist)
{
    bool                result = false;
    int32               i, iOffset = 0;
    uint32              size;
    PlaylistFormatInfo  format;
    char                szFilter[512];
    OPENFILENAME        sOpen;
    char                szPlaylistDir[MAX_PATH];
    char                szFile[MAX_PATH] = {0x00};
    char                szInitialDir[MAX_PATH] = {0x00};
    char                szExt[MAX_PATH] = {0x00};
    bool                addToDB = false;
    
    size = MAX_PATH;
    m_context->prefs->GetOpenSaveDirectory(szInitialDir, &size);

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

    strcpy(szPlaylistDir, oPlaylist.c_str());

    char* cp = NULL;

    // this should be a url so search for /
    if(cp = strrchr(szPlaylistDir, '/'))
    {
        strcpy(szFile, cp + 1);
    }
    else if(cp = strrchr(szPlaylistDir, '\\'))
    {
        strcpy(szFile, cp + 1);
    }

    if(cp = strrchr(szFile, '.'))
    {
        *cp = 0x00; // get rid of extension
    }

    sOpen.lStructSize = sizeof(OPENFILENAME);
    sOpen.hwndOwner = m_hWnd;
    sOpen.hInstance = NULL;
    sOpen.lpstrFilter = szFilter;
    sOpen.lpstrCustomFilter = NULL;
    sOpen.nMaxCustFilter = 0;
    sOpen.nFilterIndex = 1;
    sOpen.lpstrInitialDir = szInitialDir;
    sOpen.lpstrFile = szFile;
    sOpen.nMaxFile = MAX_PATH;
    sOpen.lpstrFileTitle = NULL;
    sOpen.lpstrTitle = "Export Playlist As";
    sOpen.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
                  OFN_PATHMUSTEXIST;
    sOpen.lpstrDefExt = "m3u";
  
    if(GetSaveFileName(&sOpen))
    {
        result =  true;
    }

    if(result)
    {
        vector<PlaylistItem*> items;

        m_oPlm->ReadPlaylist((char*)oPlaylist.c_str(), &items);

        char   url[MAX_PATH + 7]; // make room for file://
        uint32 len = sizeof(url);
    
        FilePathToURL(sOpen.lpstrFile, url, &len);
        
        if(IsError(m_oPlm->WritePlaylist(url, &items)))
        {
           MessageBox(m_hWnd, "Cannot save playlist to disk. Make sure there "
                              "is room on the drive or that the directory is "
                              "not read-only?", BRANDING, MB_OK);                              
           result = false;
        }

        vector<PlaylistItem*>::iterator i;

        for(i = items.begin(); i != items.end(); i++)
            delete (*i);
    }

    return result;
}
