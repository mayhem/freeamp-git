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

const char* kAudioFileFilter =
            "MPEG Audio Streams (.mpg, .mp1, .mp2, .mp3, .mpp)\0"
            "*.mpg;*.mp1;*.mp2;*.mp3;*.mpp\0"
            "All Files (*.*)\0"
            "*.*\0";

void MusicBrowserUI::DeleteListEvent(void)
{
    m_bListChanged = true;
    m_oPlm->RemoveAll();
    UpdateButtonMenuStates();
}

void MusicBrowserUI::DeleteEvent(void)
{
    // first figure out which control has focus
    HWND hwndFocus = GetFocus();

    if(hwndFocus == m_hPlaylistView)
    {
        uint32 count = ListView_GetSelectedCount(m_hPlaylistView);
        uint32 found = 0;
        uint32 index = ListView_GetItemCount(m_hPlaylistView) - 1;

        while(found < count)
        {
            uint32 state = ListView_GetItemState(m_hPlaylistView, 
                                                 index, 
                                                 LVIS_SELECTED);

            if(state & LVIS_SELECTED)
            {
                found++;
                m_oPlm->RemoveItem(index);
            }

            index--;
        }

        UpdateButtonMenuStates();
    }
    else if(hwndFocus == m_hMusicCatalog)
    {
        MessageBox(NULL, "Deleting from Catalog not yet implemented.", "doh!", MB_OK);
    }
}

void MusicBrowserUI::MoveUpEvent(void)
{
    uint32 count = ListView_GetSelectedCount(m_hPlaylistView);
    uint32 found = 0;
    uint32 index = 0;

    while(found < count)
    {
        uint32 state = ListView_GetItemState(m_hPlaylistView, 
                                             index, 
                                             LVIS_SELECTED);

        if(state & LVIS_SELECTED)
        {
            found++;

            if(index == 0)
                break;

            m_oPlm->MoveItem(index, index - 1);
        }

        index++;
    }    
}

void MusicBrowserUI::MoveDownEvent(void)
{
    uint32 count = ListView_GetSelectedCount(m_hPlaylistView);
    uint32 found = 0;
    uint32 index = ListView_GetItemCount(m_hPlaylistView) - 1;;

    while(found < count)
    {
        uint32 state = ListView_GetItemState(m_hPlaylistView, 
                                             index, 
                                             LVIS_SELECTED);

        if(state & LVIS_SELECTED)
        {
            found++;

            if(index == m_oPlm->CountItems() - 1)
                break;

            m_oPlm->MoveItem(index, index + 1);
        }

        index--;
    }    
}

void MusicBrowserUI::MoveItemEvent(int source, int dest)
{
    m_oPlm->MoveItem(source, dest);
}

void MusicBrowserUI::StartStopMusicSearch(void)
{  
    HMENU           hMenu;
    MENUITEMINFO    sItem;

    if (m_bSearchInProgress)
    {
        m_bSearchInProgress = false;
        m_context->browser->StopSearchMusic();
        return;
    }

    m_searchPathList.clear();

    if(0 < DialogBoxParam(g_hinst, 
                          MAKEINTRESOURCE(IDD_MUSICSEARCH),
                          m_hWnd, 
                          (int (__stdcall *)(void))::MusicSearchDlgProc, 
                          (LPARAM )this))
    {
        
        m_context->browser->SearchMusic(m_searchPathList);

        m_bSearchInProgress = true;
        hMenu = GetMenu(m_hWnd);
        hMenu = GetSubMenu(hMenu, 0);
        sItem.cbSize = sizeof(MENUITEMINFO);
        sItem.fMask = MIIM_TYPE;
        sItem.fType = MFT_STRING;
        sItem.dwTypeData = "Stop &Music Search";
        sItem.cch = strlen(sItem.dwTypeData);
        SetMenuItemInfo(hMenu, ID_FILE_SEARCHFORMUSIC, false, &sItem);
    }
}

void MusicBrowserUI::ExportPlaylistEvent()
{
    TV_ITEM tv_item;

    // get the first playlist item
    tv_item.hItem = TreeView_GetChild(m_hMusicCatalog, m_hPlaylistItem);
    tv_item.mask = TVIF_STATE|TVIF_PARAM;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    // skip the "Create New Playlist..." item
    tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_item.hItem);


    if(tv_item.hItem)
    {
        BOOL result = FALSE;

        // iterate the items and see if any are selected.
        // for now we only grab the first one on an export
        // but i need to figure out a good way to allow a
        // user to export multiple items since we allow
        // multi-select in the treeview.
        do
        {
            result = TreeView_GetItem(m_hMusicCatalog, &tv_item);

            if(result && (tv_item.state & TVIS_SELECTED))
            {
                string playlistPath;

                playlistPath = m_oTreeIndex.Data(tv_item.lParam).m_oPlaylistPath;

                ExportPlaylist(playlistPath);  
                break;
            }
            
        }while(result && 
               (tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, 
                                                        tv_item.hItem)));
    }
}

int32 MusicBrowserUI::Notify(WPARAM command, NMHDR *pHdr)
{
	NM_TREEVIEW *pTreeView;
	NM_LISTVIEW *pListView;
	TOOLTIPTEXT *pToolTipText;

    pTreeView = (NM_TREEVIEW *)pHdr;
    if (pTreeView->hdr.idFrom == IDC_MUSICTREE)
    {
	    if (pTreeView->hdr.code == TVN_BEGINDRAG )//&&
            //m_oTreeData.GetLevel(pTreeView->itemNew.lParam) == 3)
        {
            TVBeginDrag(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                      (NM_TREEVIEW*)pHdr);
            return 0;
        }    
 
	    if (pTreeView->hdr.code == TVN_ITEMEXPANDING && 
            pTreeView->itemNew.hItem == m_hPlaylistItem)
        {    
            if (TreeView_GetChild(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                m_hPlaylistItem) == NULL)
            {    
                FillPlaylists();
                return 0;
            }
            return 0;
        }    

	    if (pTreeView->hdr.code == TVN_ITEMEXPANDING && 
            pTreeView->itemNew.hItem == m_hAllItem)
        {    
            if (TreeView_GetChild(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                m_hAllItem) == NULL)
            {    
                FillAllTracks();
                return 0;
            }
            return 0;
        }    

	    if (pTreeView->hdr.code == TVN_ITEMEXPANDING && 
            pTreeView->itemNew.hItem == m_hUncatItem)
        {    
            if (TreeView_GetChild(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                m_hUncatItem) == NULL)
            {    
                FillUncatTracks();
                return 0;
            }
            return 0;
        }    
        
	    if (pTreeView->hdr.code == TVN_ITEMEXPANDING && 
            pTreeView->itemNew.hItem == m_hCatalogItem)
        { 
            if (TreeView_GetNextSibling(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
                m_hUncatItem) == NULL)
            {    
                FillArtists();
                return 0;
            }
            return 0;
        }    
        
	    if (pTreeView->hdr.code == TVN_ITEMEXPANDING && 
            m_oTreeIndex.GetLevel(pTreeView->itemNew.lParam) == 1 &&
            TreeView_GetChild(
            GetDlgItem(m_hWnd, IDC_MUSICTREE), pTreeView->itemNew.hItem) == NULL)
        {    
            FillAlbums(&pTreeView->itemNew);
            return 0;
        }
        
	    if (pTreeView->hdr.code == TVN_ITEMEXPANDING && 
            m_oTreeIndex.GetLevel(pTreeView->itemNew.lParam) == 2 &&
            TreeView_GetChild(
            GetDlgItem(m_hWnd, IDC_MUSICTREE), pTreeView->itemNew.hItem) == NULL)
        {    
            FillTracks(&pTreeView->itemNew);
            return 0;
        }
        
	    if (pTreeView->hdr.code == TVN_SELCHANGED)
        {
            UpdateButtonMenuStates();
            return 0;
        }
	    if (pTreeView->hdr.code == NM_DBLCLK)
        {
            //int32 lParam;
            //lParam = GetCurrentItemFromMousePos();

            TV_ITEM sItem;
            TV_HITTESTINFO tv_htinfo;


            GetCursorPos(&tv_htinfo.pt);
            ScreenToClient(m_hMusicCatalog, &tv_htinfo.pt);

            if(TreeView_HitTest(m_hMusicCatalog, &tv_htinfo))
            {
                sItem.hItem = TreeView_GetSelection(m_hMusicCatalog); 
                sItem.mask = TVIF_PARAM | TVIF_HANDLE;
                TreeView_GetItem(m_hMusicCatalog, &sItem);

                if(m_oTreeIndex.IsTrack(sItem.lParam))
                {
                    PlaylistItem *item;
                
                    item = new PlaylistItem(*m_oTreeIndex.Data(sItem.lParam).m_pTrack);
                    m_oPlm->AddItem(item, false);
                } 
                else if(m_oTreeIndex.IsPlaylist(sItem.lParam))
                {
                    AddPlaylist(m_oTreeIndex.Data(sItem.lParam).m_oPlaylistPath);
                    SetFocus(m_hPlaylistView);
                }
                else if(tv_htinfo.hItem == m_hNewPlaylistItem)
                {
                    NewPlaylist();
                }
            }


            
        }
        
        // What is the define for -17? what is -17??
	    if (pTreeView->hdr.code == -17)
        {
            TV_HITTESTINFO sHit;
            HTREEITEM      hItem;
            POINT          sPoint;
            
            sHit.flags = TVHT_ONITEM;
          
            GetCursorPos(&sPoint);
            ScreenToClient(m_hWnd, &sPoint);
            ClientToWindow(m_hWnd, &sPoint); 
            sHit.pt = sPoint;
            hItem = TreeView_HitTest(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sHit);
            if (hItem == m_hPlaylistItem)
               SendMessage(m_hStatus, SB_SETTEXT, 0, 
                           (LPARAM)"This tree item contains all of your playlists.");
            else               
            if (hItem == m_hCatalogItem)
               SendMessage(m_hStatus, SB_SETTEXT, 0, 
                           (LPARAM)"This tree item contains all of your music.");
            else               
            if (hItem == m_hAllItem)
               SendMessage(m_hStatus, SB_SETTEXT, 0, 
                           (LPARAM)"This tree item lists all of your music tracks.");
            else               
            if (hItem == m_hUncatItem)
               SendMessage(m_hStatus, SB_SETTEXT, 0, 
                           (LPARAM)"This tree item lists all of your uncategorized music tracks.");
            else               
               SendMessage(m_hStatus, SB_SETTEXT, 0, (LPARAM)"");
        }

        //if (pTreeView->hdr.code == NM_CLICK)
        //{
        //    SendMessage();
        //}

        if (pTreeView->hdr.code == NM_RCLICK)
        {
            POINT sPoint;
            
            GetCursorPos(&sPoint);
            TrackPopupMenu(GetSubMenu(LoadMenu(g_hinst, MAKEINTRESOURCE(IDR_POPUP)), 0), 
                           TPM_LEFTALIGN, sPoint.x, sPoint.y, 
                           0, m_hWnd, NULL);
        }
        
        return 0;
    }    

    pListView = (NM_LISTVIEW *)pHdr;
    if (pListView->hdr.idFrom == IDC_PLAYLISTBOX)
    {
        if (pListView->hdr.code == LVN_BEGINDRAG )
        {
            LVBeginDrag(m_hPlaylistView, (NM_LISTVIEW*)pHdr);
            return 0;
        }    

	    if(pListView->hdr.code == LVN_ITEMCHANGED)
        {
            UpdateButtonMenuStates();
        }
	    else if(pListView->hdr.code == NM_DBLCLK)
        {
            // only do this for the root browser
            if(!m_pParent)
            {
                m_playerEQ->AcceptEvent(new Event(CMD_Stop));
                m_oPlm->SetCurrentIndex(pListView->iItem);
                m_playerEQ->AcceptEvent(new Event(CMD_Play));
            }
        }   
        else if(pListView->hdr.code == LVN_COLUMNCLICK)
        {
            int column = pListView->iSubItem;

            switch(column)
            {
                case 1:
                    SendMessage(m_hWnd, WM_COMMAND, ID_SORT_TITLE, 0);
                    break;

                case 2:
                    SendMessage(m_hWnd, WM_COMMAND, ID_SORT_ARTIST, 0);
                    break;

                case 3:
                    SendMessage(m_hWnd, WM_COMMAND, ID_SORT_ALBUM, 0);
                    break;

                case 4:
                    SendMessage(m_hWnd, WM_COMMAND, ID_SORT_LENGTH, 0);
                    break;
            }
        }   
        else if(pListView->hdr.code == LVN_KEYDOWN)
        {
            LV_KEYDOWN* pnkd = (LV_KEYDOWN*)pHdr; 

            if(pnkd->wVKey == VK_DELETE)
            {
                DeleteEvent();  
            }
            else if(pnkd->wVKey == 'A' && (GetKeyState(VK_CONTROL) < 0))
            {
                uint32 count = ListView_GetItemCount(pListView->hdr.hwndFrom);
                
                for(uint32 index = 0; index < count; index++)
                    ListView_SetItemState(pListView->hdr.hwndFrom,
                                          index,
                                          LVIS_SELECTED,
                                          LVIS_SELECTED);
            }

        }
            
        return 0;
    }

	if(pHdr->code == TTN_NEEDTEXT)
    {
        pToolTipText = (LPTOOLTIPTEXT)pHdr;

        switch(pToolTipText->hdr.idFrom)
        {
            case ID_FILE_NEWPLAYLIST:
                pToolTipText->lpszText = "Create a new playlist";
                return true;
            case ID_FILE_SAVEPLAYLIST:
                pToolTipText->lpszText = "Save current playlist";
                return true;
            case ID_FILE_IMPORT:
                pToolTipText->lpszText = "Import tracks and playlists from disk";
                return true;
            case ID_EDIT_REMOVE:
                pToolTipText->lpszText = "Remove selected tracks and playlists";
                return true;
            case ID_EDIT_EDITINFO:
                pToolTipText->lpszText = "Edit track or playlist info";
                return true;
            case ID_EDIT_ADDTRACK:
                pToolTipText->lpszText = "Add selected tracks and playlists to playlist";
                return true;
            case ID_EDIT_ADDFILE:
                pToolTipText->lpszText = "Add files from disk to playlist";
                return true;
            case ID_EDIT_MOVEUP:
                pToolTipText->lpszText = "Move selected playlist items up";
                return true;
            case ID_EDIT_MOVEDOWN:
                pToolTipText->lpszText = "Move selected playlist items down";
                return true;
        }      
    }
    
	return 1;
}


void MusicBrowserUI::SortEvent(int id)
{
    PlaylistSortKey oldKey, newKey;
    PlaylistSortType type;
    
    switch(id)
    {
        case ID_SORT_ARTIST:
             newKey = kPlaylistSortKey_Artist;
        break;     
        case ID_SORT_ALBUM:
             newKey = kPlaylistSortKey_Album;
        break;     
        case ID_SORT_TITLE:
             newKey = kPlaylistSortKey_Title;
        break;     
        case ID_SORT_LENGTH:
             newKey = kPlaylistSortKey_Time;
        break;     
        case ID_SORT_YEAR:
             newKey = kPlaylistSortKey_Year;
        break;     
        case ID_SORT_TRACK:
             newKey = kPlaylistSortKey_Track;
        break;     
        case ID_SORT_GENRE:
             newKey = kPlaylistSortKey_Genre;
        break;     
        case ID_SORT_LOCATION:
             newKey = kPlaylistSortKey_Location;
        break;     
        case IDC_RANDOMIZE:
        case ID_SORT_RANDOMIZE:
             newKey = kPlaylistSortKey_Random;
        break;     
        default:
             return;
    }

    oldKey = m_oPlm->GetPlaylistSortKey();

    type = (oldKey == newKey && 
            m_oPlm->GetPlaylistSortType() == PlaylistSortType_Ascending)
            ? PlaylistSortType_Descending : PlaylistSortType_Ascending;

    ::SetCursor(LoadCursor(NULL, IDC_WAIT));
    m_oPlm->Sort(newKey, type);
    ::SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void MusicBrowserUI::ToggleVisEvent(void)
{
    Event *e;

    if (m_state == STATE_COLLAPSED)
        e = new Event(CMD_TogglePlaylistUI);
    else
        e = new Event(CMD_ToggleMusicBrowserUI);

    AcceptEvent(e);
    
    delete e;
}


void MusicBrowserUI::AddTrackEvent(void)
{
    vector<string> urls;

    GetSelectedMusicTreeItems(&urls);    

    // we know that we are gonna be adding a 
    // bunch of items so let windows know.
    // it will make the adds more efficient
    uint32 newSize = ListView_GetItemCount(m_hPlaylistView);
    newSize += urls.size();
    ListView_SetItemCount(m_hPlaylistView, newSize);

    m_oPlm->AddItems(urls);
    
}
void MusicBrowserUI::AddFileEvent(void)
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

    if (FileOpenDialog(m_hWnd, "Add Tracks and Playlists",
                       szFilter, 
                       &oFileList,
                       m_context->prefs))
    {
        // we know that we are gonna be adding a 
        // bunch of items so let windows know.
        // it will make the adds more efficient
        uint32 newSize = ListView_GetItemCount(m_hPlaylistView);
        newSize += oFileList.size();
        ListView_SetItemCount(m_hPlaylistView, newSize);

        m_oPlm->AddItems(oFileList); 
    }  
}

void MusicBrowserUI::EmptyDBCheck(void)
{
    int iRet;
    
    if (m_context->browser->m_catalog->GetPlaylists()->size() > 0 ||
        m_context->browser->m_catalog->GetMusicList()->size() > 0 ||
        m_context->browser->m_catalog->GetUnsortedMusic()->size() > 0)
        return;
        
    iRet = MessageBox(m_hWnd, "Your music database does not contain any items. "
                       "Would you like to\r\nstart a music search to find "
                       "music and playlists on your machine?",
                       "MusicBrowser", MB_YESNO);
    if (iRet == IDYES)
        StartStopMusicSearch();
}

void MusicBrowserUI::EditPlaylistEvent(void)
{
    TV_ITEM tv_item;

    // get the first playlist item
    tv_item.hItem = TreeView_GetChild(m_hMusicCatalog, m_hPlaylistItem);
    tv_item.mask = TVIF_STATE|TVIF_PARAM;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    // skip the "Create New Playlist..." item
    tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_item.hItem);


    if(tv_item.hItem)
    {
        BOOL result = FALSE;

        do
        {
            result = TreeView_GetItem(m_hMusicCatalog, &tv_item);

            if(result && (tv_item.state & TVIS_SELECTED))
            {
                string playlistPath;

                playlistPath = m_oTreeIndex.Data(tv_item.lParam).m_oPlaylistPath;

                EditPlaylist(playlistPath);
            }
            
        }while(result && 
               (tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, 
                                                        tv_item.hItem)));
    }
}


void MusicBrowserUI::RemoveEvent(void)
{
    int32     lParam;
    HTREEITEM hItem;

    lParam = GetMusicTreeSelection(hItem);
    if (m_oTreeIndex.IsTrack(lParam))
    {
        m_context->browser->m_catalog->RemoveSong(
             m_oTreeIndex.Data(lParam).m_pTrack->URL().c_str());
        TreeView_DeleteItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), hItem);
    }        
    if (m_oTreeIndex.IsPlaylist(lParam))
    {
        m_context->browser->m_catalog->RemovePlaylist( 
             m_oTreeIndex.Data(lParam).m_oPlaylistPath.c_str());
        TreeView_DeleteItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), hItem);
    }             
}

void MusicBrowserUI::RemoveFromDiskEvent(void)
{
    int32     lParam;
    char      szConfirm[MAX_PATH];
    char      szBase[MAX_PATH];
    HTREEITEM hItem;

    lParam = GetMusicTreeSelection(hItem);
    if (m_oTreeIndex.IsTrack(lParam))
    {
        _splitpath(m_oTreeIndex.Data(lParam).m_pTrack->URL().c_str(),
                   NULL, NULL, szBase, NULL);
        sprintf(szConfirm, "Are you sure you want to remove track\r\n"
                           "%s from the music catalog and the disk?",
                           szBase);
        if (MessageBox(m_hWnd, szConfirm, BRANDING, MB_YESNO) == IDYES)
        {
           unlink(m_oTreeIndex.Data(lParam).m_pTrack->URL().c_str());
           m_context->browser->m_catalog->RemoveSong(
                  m_oTreeIndex.Data(lParam).m_pTrack->URL().c_str());
           TreeView_DeleteItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), hItem);
        }   
    }        
    if (m_oTreeIndex.IsPlaylist(lParam))
    {
        _splitpath(m_oTreeIndex.Data(lParam).m_oPlaylistPath.c_str(),
                   NULL, NULL, szBase, NULL);
        sprintf(szConfirm, "Are you sure you want to remove the playlist\r\n"
                           "%s from the music catalog and the disk?",
                           szBase);
        if (MessageBox(m_hWnd, szConfirm, BRANDING, MB_YESNO) == IDYES)
        {
            unlink(m_oTreeIndex.Data(lParam).m_oPlaylistPath.c_str());
            m_context->browser->m_catalog->RemovePlaylist( 
                 m_oTreeIndex.Data(lParam).m_oPlaylistPath.c_str());
            TreeView_DeleteItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), hItem);
        }            
    }             
}

void MusicBrowserUI::PlayerControlsEvent(int command)
{
    switch(command)
    {
        case ID_CONTROLS_PLAYPAUSE:

            if(m_playerState == PLAYERSTATE_PLAYING)
                m_context->target->AcceptEvent(new Event(CMD_Pause));
            else
                m_context->target->AcceptEvent(new Event(CMD_Play));

            break;

        case ID_CONTROLS_STOP:
            m_context->target->AcceptEvent(new Event(CMD_Stop));
            break;

        case ID_CONTROLS_PREVIOUSSONG:
            m_context->target->AcceptEvent(new Event(CMD_PrevMediaPiece));
            break;

        case ID_CONTROLS_NEXTSONG:
            m_context->target->AcceptEvent(new Event(CMD_NextMediaPiece));
            break;

        case ID_CONTROLS_NORMALORDER:
            m_oPlm->SetShuffleMode(false);
            break;

        case ID_CONTROLS_SHUFFLE:
            m_oPlm->SetShuffleMode(true);
            break;

        case ID_CONTROLS_REPEATNONE:
            m_oPlm->SetRepeatMode(kPlaylistMode_RepeatNone);
            break;
        case ID_CONTROLS_REPEATONE:
            m_oPlm->SetRepeatMode(kPlaylistMode_RepeatOne);
            break;
        case ID_CONTROLS_REPEATALL:
            m_oPlm->SetRepeatMode(kPlaylistMode_RepeatAll);
            break;


    }
}

void MusicBrowserUI::ChangePlayerState(int32 event)
{
    HMENU           menu;
    MENUITEMINFO    item;

    menu = GetMenu(m_hWnd);
    menu = GetSubMenu(menu, 3);
    item.cbSize = sizeof(MENUITEMINFO);
    item.fMask = MIIM_TYPE;
    item.fType = MFT_STRING;

    switch(event) 
    {
        case INFO_Playing: 
        {   
            m_playerState = PLAYERSTATE_PLAYING;
            
            item.dwTypeData = "Pause";
            item.cch = strlen(item.dwTypeData);
            SetMenuItemInfo(menu, ID_CONTROLS_PLAYPAUSE, false, &item);
                   
	        break; 
        }

        case INFO_Paused: 
        case INFO_Stopped: 
        {
            if(event == INFO_Paused)
                m_playerState = PLAYERSTATE_PAUSED;
            else
                m_playerState = PLAYERSTATE_STOPPED;

            item.dwTypeData = "Play";
            item.cch = strlen(item.dwTypeData);
            SetMenuItemInfo(menu, ID_CONTROLS_PLAYPAUSE, false, &item);

	        break; 
        }
    }
}

void MusicBrowserUI::ChangeShuffleMode(bool shuffled)
{
    HMENU           menu;
    MENUITEMINFO    item;

    menu = GetMenu(m_hWnd);
    menu = GetSubMenu(menu, 3);
    item.cbSize = sizeof(MENUITEMINFO);
    item.fMask = MIIM_STATE;
    item.fState = shuffled ? MFS_CHECKED:MFS_UNCHECKED;
    SetMenuItemInfo(menu, ID_CONTROLS_SHUFFLE, false, &item);
    
    item.fState = shuffled ? MFS_UNCHECKED: MFS_CHECKED;
    SetMenuItemInfo(menu, ID_CONTROLS_NORMALORDER, false, &item);
}

void MusicBrowserUI::ChangeRepeatMode(RepeatMode mode)
{
    HMENU           menu;
    MENUITEMINFO    item;

    menu = GetMenu(m_hWnd);
    menu = GetSubMenu(menu, 3);
    item.fMask = MIIM_STATE;

    item.cbSize = sizeof(MENUITEMINFO);

	switch(mode) 
    {
		case kPlaylistMode_RepeatNone:
            item.fState = MFS_CHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATNONE, false, &item);
            item.fState = MFS_UNCHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATONE, false, &item);
            item.fState = MFS_UNCHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATALL, false, &item);
			break;

		case kPlaylistMode_RepeatOne:
            item.fState = MFS_UNCHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATNONE, false, &item);
            item.fState = MFS_CHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATONE, false, &item);
            item.fState = MFS_UNCHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATALL, false, &item);
			break;

		case kPlaylistMode_RepeatAll:
            item.fState = MFS_UNCHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATNONE, false, &item);
            item.fState = MFS_UNCHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATONE, false, &item);
            item.fState = MFS_CHECKED;
            SetMenuItemInfo(menu, ID_CONTROLS_REPEATALL, false, &item);
            break;
	}

}