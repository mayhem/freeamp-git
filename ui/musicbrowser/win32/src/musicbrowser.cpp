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

#include "Win32MusicBrowser.h"
#include "debug.h"
#include "resource.h"
#include "eventdata.h"

extern "C" {

UserInterface *Initialize(FAContext *context) {
    return new MusicBrowserUI(context, NULL, string(""));
}
           }

MusicBrowserUI::MusicBrowserUI(FAContext *context, 
                               MusicBrowserUI *parent,
                               const string &oPlaylistName)
{
    m_context = context;
    m_initialized = false;
    isVisible = false;
    m_currentindex = 0;
    m_currentListName = "";
    m_state = STATE_EXPANDED;
    m_hWnd = NULL;    
    m_sMinSize.x = -1;
    m_bListChanged = false;
    m_bSearchInProgress = false;
    m_currentplaying = -1;
    m_bDragging = false;
    m_pParent = parent;
    if (parent == NULL)
       m_oPlm = m_context->plm;
    else
    {
       m_oPlm = new PlaylistManager(context);   
       m_currentListName = oPlaylistName;
    }   
}

MusicBrowserUI::~MusicBrowserUI()
{
    if (m_pParent)
       delete m_oPlm;
}

Error MusicBrowserUI::Init(int32 startup_level) 
{
    if ((m_startupType = startup_level) != SECONDARY_UI_STARTUP) {
        cout << "The musicbrowser cannot run as the primary ui\n"; 
        return kError_InitFailedSafely;
    }

    m_playerEQ = m_context->target;
    m_initialized = true;
    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(MusicBrowserUI::UIThreadFunc, this);

    return kError_NoErr;
}

int32 MusicBrowserUI::AcceptEvent(Event *event)
{
    switch (event->Type()) 
    {
        case CMD_Cleanup: 
        {
            if (m_currentListName.length() != 0)
                WritePlaylist();     
            else
            {
                string lastPlaylist = FreeampDir(m_context->prefs);
                lastPlaylist += "\\currentlist.m3u";

                SaveCurrentPlaylist((char *)lastPlaylist.c_str());  
            }    
       
            CloseMainDialog();
            m_playerEQ->AcceptEvent(new Event(INFO_ReadyToDieUI));

            break; 
        }
        case INFO_PlaylistItemUpdated:
        case INFO_PlaylistCurrentItemInfo:
        {
            m_currentplaying = m_oPlm->GetCurrentIndex();
            UpdatePlaylistList();
            break; 
        }

        case INFO_SearchMusicDone: 
        {
            if (m_bSearchInProgress)
                SendMessage(m_hStatus, SB_SETTEXT, 0, 
                            (LPARAM)"Music search completed.");
            else                
                SendMessage(m_hStatus, SB_SETTEXT, 0, 
                            (LPARAM)"Music search interrupted -- your database may be incomplete.");
                            
            SetWindowText(GetDlgItem(m_hWnd, IDC_SEARCH), "Music Search");
            m_bSearchInProgress = false;
                        
            InitTree();
            TreeView_Expand(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                            m_hPlaylistItem, TVE_EXPAND);
            TreeView_Expand(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                            m_hCatalogItem, TVE_EXPAND);
            
            break; 
        }
        case INFO_BrowserMessage: 
        {
            SendMessage(m_hStatus, SB_SETTEXT, 0, (LPARAM)
                          ((BrowserMessageEvent *)event)->GetBrowserMessage());
            break; 
        }
        case CMD_TogglePlaylistUI: 
        {
            if (m_initialized && isVisible) 
            {
                isVisible = false;
            }
            else 
            {
                isVisible = true;
                if (m_initialized)
                {
					ShowBrowser(true);
                }
                else 
                {
                    m_initialized = true;
                }
            }
            if (m_state == STATE_EXPANDED)
                ExpandCollapseEvent();
            break; 
        }
        case CMD_ToggleMusicBrowserUI: 
        {
            if (m_initialized && isVisible) 
            {
				HideBrowser();
                isVisible = false;
            }
            else 
            {
                isVisible = true;
                if (m_initialized)
                {
					ShowBrowser(true);
                }
                else 
                {
                    m_initialized = true;
                }
            }
            if (m_state == STATE_COLLAPSED)
                ExpandCollapseEvent();
            break; 
        }

        default:
            break;
    }
    return 0;
}

void MusicBrowserUI::DeleteListEvent(void)
{
    m_bListChanged = true;
    m_oPlm->RemoveAll();
    UpdatePlaylistList();
}

void MusicBrowserUI::DeleteEvent(void)
{
    m_oPlm->RemoveItem(m_currentindex);
    m_bListChanged = true;
    m_currentplaying = m_oPlm->GetCurrentIndex();
    UpdatePlaylistList();
}

void MusicBrowserUI::MoveUpEvent(void)
{
    if (m_currentindex == 0)
        return;

    m_bListChanged = true;
    m_oPlm->SwapItems(m_currentindex, m_currentindex - 1);
    m_currentindex--;
    UpdatePlaylistList();
}

void MusicBrowserUI::MoveDownEvent(void)
{
    if (m_currentindex == m_oPlm->CountItems() - 1)
        return;

    m_bListChanged = true;
    m_oPlm->SwapItems(m_currentindex, m_currentindex + 1);
    m_currentindex++;
    UpdatePlaylistList();
}

void MusicBrowserUI::MoveItemEvent(int source, int dest)
{
    m_oPlm->MoveItem(source, dest);
    UpdatePlaylistList();
}

void MusicBrowserUI::AddTrackPlaylistEvent(char *path)
{
    char *tempurl = new char[_MAX_PATH];
    uint32 length = _MAX_PATH;

    FilePathToURL(path, tempurl, &length);
    if (m_currentindex == kInvalidIndex) 
        m_currentindex = 0;
    m_oPlm->AddItem(tempurl, m_currentindex);
   
    delete [] tempurl;

    UpdatePlaylistList();
}

void MusicBrowserUI::AddTrackPlaylistEvent(PlaylistItem *newitem)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    m_oPlm->AddItem(newitem, m_currentindex, false);
    UpdatePlaylistList();
}

void MusicBrowserUI::AddTracksPlaylistEvent(vector<PlaylistItem *> *newlist)
{
    if (m_currentindex == kInvalidIndex)
        m_currentindex = 0;
    m_oPlm->AddItems(newlist, m_currentindex, false);
    UpdatePlaylistList();
}

void MusicBrowserUI::PlayEvent(void)
{
    m_oPlm->SetCurrentIndex(m_currentindex);
    m_playerEQ->AcceptEvent(new Event(CMD_Play));
}

void MusicBrowserUI::StartMusicSearch(void)
{
    DWORD           dwDrives;
    char           *szPath = "X:\\";
    int32           i, ret;
    vector<string>  oPathList;

    if (m_bSearchInProgress)
    {
        m_bSearchInProgress = false;
        m_context->browser->StopSearchMusic();
        return;
    }
        
    dwDrives = GetLogicalDrives();
    for(i = 0; i < sizeof(DWORD) * 8; i++)
    {
       if (dwDrives & (1 << i))
       {
          szPath[0] = 'A' + i;
          ret = GetDriveType(szPath);
          if (ret != DRIVE_CDROM && ret != DRIVE_REMOVABLE)
              oPathList.push_back(string(szPath));
       }   
    }
    m_context->browser->SearchMusic(oPathList);

    m_bSearchInProgress = true;
    SetWindowText(GetDlgItem(m_hWnd, IDC_SEARCH), "Stop Search");
}

void MusicBrowserUI::SortPlaylistEvent(PlaylistSortKey order, PlaylistSortType
                                       type)
{
    if (order == kPlaylistSortKey_Random)
        m_oPlm->SetShuffleMode(true);
    else
        m_oPlm->Sort(order, type);
    UpdatePlaylistList();
}

void MusicBrowserUI::PopUpInfoEditor(void)
{
}

void MusicBrowserUI::SaveCurrentPlaylist(char *path)
{
    if (path)
        m_currentListName = path;

    if (m_currentListName.length() == 0)
        return;

    char *ext = strrchr(m_currentListName.c_str(), '.');
    if (ext)
        ext = ext + 1;
    Error result = kError_NoErr;
    int i = 0;
    bool found = false;

    PlaylistFormatInfo format;
    while (ext && result == kError_NoErr) {
        result = m_oPlm->GetSupportedPlaylistFormats(&format, i);
        if (!strcmp(ext, format.GetExtension())) {
            found = true;
            break;
        }
        i++;
    }
    if (!found) {
        m_oPlm->GetSupportedPlaylistFormats(&format, 0);
        m_currentListName += "." ;
        m_currentListName += format.GetExtension();
    }

    uint32 urlLength = m_currentListName.length() + 20;
    char *writeURL = new char[urlLength];

    Error err = FilePathToURL(m_currentListName.c_str(), writeURL, &urlLength);
    if (IsntError(err))
        m_oPlm->WritePlaylist(writeURL, &format);

    delete [] writeURL;
}

void MusicBrowserUI::ReadPlaylist(char *path, vector<PlaylistItem *> *plist)
{
    m_oPlm->ReadPlaylist(path, plist);
}

