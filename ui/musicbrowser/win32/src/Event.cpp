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
    UpdatePlaylistList();
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
    UpdatePlaylistList();
}

#if 0
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
#endif

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
            BeginDrag(GetDlgItem(m_hWnd, IDC_MUSICTREE), 
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
            UpdateMenuItems();
            return 0;
        }
	    if (pTreeView->hdr.code == NM_DBLCLK)
        {
            int32 lParam;
            
            lParam = GetCurrentItemFromMousePos();
            if (m_oTreeIndex.IsTrack(lParam))
            {
                PlaylistItem *item;
                
                item = new PlaylistItem(*m_oTreeIndex.Data(lParam).m_pTrack);
                m_oPlm->AddItem(item, false);
                //UpdatePlaylistList();
                m_bListChanged = true;
            } 
            else if (m_oTreeIndex.IsPlaylist(lParam))
            {
                AddPlaylist(m_oTreeIndex.Data(lParam).m_oPlaylistPath);
                SetFocus(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX));
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
	    if (pListView->hdr.code == LVN_ITEMCHANGED)
        {
            m_currentindex = pListView->lParam;
            UpdateButtonStates();
        }

	    if (pListView->hdr.code == NM_DBLCLK)
        {
            // only do this for the root browser
            if(!m_pParent)
            {
                m_playerEQ->AcceptEvent(new Event(CMD_Stop));
                m_oPlm->SetCurrentIndex(pListView->iItem);
                m_playerEQ->AcceptEvent(new Event(CMD_Play));
            }
        }   
        
        if (pListView->hdr.code == LVN_COLUMNCLICK)
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
            
        return 0;
    }

	if (pTreeView->hdr.code == TTN_NEEDTEXT)
    {
        pToolTipText = (LPTOOLTIPTEXT)pHdr;
        switch(pToolTipText->hdr.idFrom)
        {
           case ID_FILE_SEARCHFORMUSIC:
              pToolTipText->lpszText = "Music Search";
              return true;
           case ID_FILE_NEWPLAYLIST:
              pToolTipText->lpszText = "New Playlist";
              return true;
           case ID_FILE_IMPORT:
              pToolTipText->lpszText = "Import Tracks";
              return true;
           case ID_EDIT_REMOVE:
              pToolTipText->lpszText = "Remove Track/Playlist";
              return true;
           case ID_EDIT_EDIT:
              pToolTipText->lpszText = "Edit Track/Playlist";
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

    m_oPlm->Sort(newKey, type);
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


void MusicBrowserUI::AddEvent(void)
{
    vector<char*>           oFileList;
    vector<char*>::iterator i;
    Error                   eRet;

    if (FileOpenDialog(m_hWnd, "Add playlist item",
                       kAudioFileFilter, 
                       &oFileList,
                       m_context->prefs))
    {
        for(i = oFileList.begin(); i != oFileList.end(); i++)
           eRet = m_oPlm->AddItem(*i);
        m_bListChanged = true;
        //UpdatePlaylistList();
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

void MusicBrowserUI::EditEvent(void)
{
    TV_ITEM   sItem;
    
    sItem.mask = TVIF_PARAM;
    sItem.hItem = TreeView_GetSelection(GetDlgItem(m_hWnd, IDC_MUSICTREE));
    if (sItem.hItem != m_hPlaylistItem &&
        sItem.hItem != m_hCatalogItem &&
        sItem.hItem != m_hAllItem &&
        sItem.hItem != m_hUncatItem)
    {
       TreeView_GetItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sItem);
       
       if (sItem.lParam < 0)
       { 
           const vector<string> *p;
           int                   i = -sItem.lParam;
           
           p = m_context->browser->m_catalog->GetPlaylists();
           EditPlaylist((*p)[(-sItem.lParam) - 1]);
       }    
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

void MusicBrowserUI::ImportEvent(void)
{
    vector<char*>           oFileList;
    vector<char*>::iterator i;

    if (FileOpenDialog(m_hWnd, "Import Track",
                       kAudioFileFilter, 
                       &oFileList,
                       m_context->prefs))
    {
        for(i = oFileList.begin(); i != oFileList.end(); i++)
            m_context->browser->m_catalog->AddSong(*i);
        InitTree();    
    }
}

