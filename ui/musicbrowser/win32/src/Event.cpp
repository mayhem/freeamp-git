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
#include "EditTrackInfoDialog.h"
#include "eventdata.h"
#include "debug.h"

const char* kAudioFileFilter =
            "MPEG Audio Streams (.mpg, .mp1, .mp2, .mp3, .mpp)\0"
            "*.mpg;*.mp1;*.mp2;*.mp3;*.mpp\0"
            "All Files (*.*)\0"
            "*.*\0";

void MusicBrowserUI::ClearPlaylistEvent(void)
{
    m_context->target->AcceptEvent(new Event(CMD_Stop));

    m_oPlm->RemoveAll();
}

void MusicBrowserUI::RenameEvent(void)
{
    HWND hwnd = m_hMusicView;

    HTREEITEM item;
    //TV_HITTESTINFO hti;

    //GetCursorPos(&hti.pt);
    //ScreenToClient(hwnd, &hti.pt);
    //item = TreeView_HitTest(hwnd, &hti);  

    item = TreeView_GetSelection(hwnd);

    if(item /*&& (hti.flags & TVHT_ONITEM)*/ &&
       item != m_hMyMusicItem &&
       item != m_hPlaylistItem &&
       item != m_hAllItem &&
       item != m_hUncatItem &&
       item != m_hNewPlaylistItem && 
       item != m_hPortableItem &&
       TreeView_GetParent(m_hMusicView, item) != m_hPortableItem)
    {
        EditItemLabel(hwnd, item);
    }
}

const char kErrorMsg[] = "Cannot delete %s: Access is denied.\r\n\r\n"
                        "Make sure the file is not currently in use.";

bool MusicBrowserUI::DeleteFromDrive(const char* url)
{
    bool result = true;
    char path[MAX_PATH];
    uint32 length = sizeof(path);
    BOOL success = FALSE;

    URLToFilePath(url, path, &length);

    do
    {
        success = DeleteFile(path);

        if(!success)
        {
            int ret;
            char msg[MAX_PATH + sizeof(kErrorMsg) + 1];
            char* cp;

            cp = strrchr(path, '\\');

            if(cp)
                cp++;
            else
                cp = path;

            sprintf(msg, kErrorMsg, cp);

            ret = MessageBox(m_hWnd, 
                              msg,
                              "Unable To Delete File",
                              MB_ABORTRETRYIGNORE|MB_ICONSTOP);

            switch(ret)
            {
                case IDABORT:
                    result = false;
                    success = TRUE;
                    break;

                case IDRETRY:
                    result = true;
                    success = FALSE;
                    break;

                case IDIGNORE:
                    result = true;
                    success = TRUE;
                    break;
            }
        }

    }while(!success);

    return result;
}

void MusicBrowserUI::RemoveEvent(void)
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
                bool isCurrent = (m_oPlm->GetCurrentIndex() == index);

                if(isCurrent)
                    m_context->target->AcceptEvent(new Event(CMD_Stop));

                m_oPlm->RemoveItem(index);

                if(isCurrent)
                    m_context->target->AcceptEvent(new Event(CMD_Play));
            }

            index--;
        }        
    }
    else if(hwndFocus == m_hMusicView)
    {
        bool deleteFromDrive = false;

        if(0 < DialogBoxParam(g_hinst, 
                              MAKEINTRESOURCE(IDD_REMOVETRACKS),
                              m_hWnd, 
                              ::RemoveTracksDlgProc, 
                              (LPARAM)&deleteFromDrive))
        {       
            vector<PlaylistItem*> items;
            GetSelectedMusicTreeItems(&items); 
            bool keepGoing = true;

            vector<PlaylistItem*>::iterator i;

            for(i = items.begin(); i != items.end(); i++)
            {
                m_context->catalog->RemoveSong((*i)->URL().c_str());

                if(deleteFromDrive)
                {
                    keepGoing = DeleteFromDrive((*i)->URL().c_str());

                    if(!keepGoing)
                        break;
                }
            }

            if(keepGoing)
            {
                vector<string> urls;            
                GetSelectedPlaylistItems(&urls);

                vector<string>::iterator j;
            
                for(j = urls.begin(); j != urls.end(); j++)
                {
                    m_context->catalog->RemovePlaylist((*j).c_str());

                    if(deleteFromDrive)
                    {
                        keepGoing = DeleteFromDrive((*j).c_str());

                        if(!keepGoing)
                            break;
                    }
                }
            }
        }
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

void MusicBrowserUI::StartStopMusicSearch(bool useWizard)
{  
    HMENU           hMenu;
    MENUITEMINFO    sItem;
    bool            doSearch = false;

    if (m_bSearchInProgress)
    {
        m_bSearchInProgress = false;
        m_context->catalog->StopSearchMusic();
        return;
    }

    m_searchPathList.clear();

    if(useWizard)
        doSearch = IntroductionWizard(&m_searchPathList);
    else
        doSearch = (0 < DialogBoxParam(g_hinst, 
                          MAKEINTRESOURCE(IDD_MUSICSEARCH),
                          m_hWnd, 
                          ::MusicSearchDlgProc, 
                          (LPARAM )this));

    if(doSearch)
    {
        
        m_context->catalog->SearchMusic(m_searchPathList);

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
    tv_item.hItem = TreeView_GetChild(m_hMusicView, m_hPlaylistItem);
    tv_item.mask = TVIF_STATE|TVIF_PARAM;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    // skip the "Create New Playlist..." item
    tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, tv_item.hItem);


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
            result = TreeView_GetItem(m_hMusicView, &tv_item);

            if(result && (tv_item.state & TVIS_SELECTED))
            {
                string playlistPath;

                TreeData* treedata = (TreeData*)tv_item.lParam;

                if(treedata)
                {
                    playlistPath = treedata->m_oPlaylistPath;

                    ExportPlaylist(playlistPath);
                }
                break;
            }
            
        }while(result && 
               (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
                                                        tv_item.hItem)));
    }
}

const char* kMultipleArtists =  "<Multiple Artists Selected>";
const char* kMultipleAlbums =   "<Multiple Albums Selected>";
const char* kMultipleGenres =   "<Multiple Genres Selected>";
const char* kMultipleTracks =   "<Multiple Tracks Selected>";
const char* kMultipleComments = "<Enter a new comment for all selected tracks.>";

void MusicBrowserUI::EditInfoEvent()
{
    MetaData metadata;

    vector<PlaylistItem*> items;

    if(m_hPlaylistView == GetFocus())
        GetSelectedPlaylistItems(&items); 
    else if(m_hMusicView == GetFocus())
        GetSelectedMusicTreeItems(&items); 

    if(!items.size())
        return;

    metadata = items[0]->GetMetaData();

    bool sameArtist = true;
    bool sameAlbum = true;
    bool sameGenre = true;
    bool sameYear = true;
    vector<PlaylistItem*>::iterator track;

    for(track = items.begin(); track != items.end(); track++)
    {
        MetaData metadata = (*track)->GetMetaData();

        if(metadata.Artist() != metadata.Artist())
            sameArtist = false;

        if(metadata.Album() != metadata.Album())
            sameAlbum = false;

        if(metadata.Genre() != metadata.Genre())
            sameGenre = false;

        if(metadata.Year() != metadata.Year())
            sameYear = false;
    }

    if(!sameArtist)
        metadata.SetArtist(kMultipleArtists);

    if(!sameAlbum)
        metadata.SetAlbum(kMultipleAlbums);

    if(!sameYear)
        metadata.SetYear(-1);

    if(!sameGenre)
        metadata.SetGenre(kMultipleGenres);

    if(items.size() > 1)
    {
        metadata.SetTitle(kMultipleTracks);
        metadata.SetTrack(-1);
        metadata.SetComment(kMultipleComments);
    }

    EditTrackInfoDialog editTrackInfo(m_context,
                                      m_hWnd,
                                      m_context->catalog->GetMusicList(),
                                      &metadata);
    if(editTrackInfo.Show())
    {
        for(track = items.begin(); track != items.end(); track++)
        {
            MetaData oldMetaData, newMetaData;

            oldMetaData = newMetaData = (*track)->GetMetaData();

            if(metadata.Artist() != kMultipleArtists)
                newMetaData.SetArtist(metadata.Artist().c_str());

            if(metadata.Album() != kMultipleAlbums)
                newMetaData.SetAlbum(metadata.Album().c_str());

            if(metadata.Title() != kMultipleTracks)
                newMetaData.SetTitle(metadata.Title().c_str());

            if(metadata.Genre() != kMultipleGenres)
                newMetaData.SetGenre(metadata.Genre().c_str());

            if(metadata.Comment() != kMultipleComments)
                newMetaData.SetComment(metadata.Comment().c_str());

            if(metadata.Year() != -1)
               newMetaData.SetYear(metadata.Year());

            if(metadata.Track() != -1)
                newMetaData.SetTrack(metadata.Track());

            if(newMetaData != oldMetaData)
            {
                (*track)->SetMetaData(&newMetaData);

                m_context->catalog->UpdateSong(*track);

                m_oPlm->UpdateTrackMetaData(*track);
            }
        } 
    }
}

int32 MusicBrowserUI::Notify(WPARAM command, NMHDR *pHdr)
{
    int32 result = 0;

	TOOLTIPTEXT *pToolTipText;
    
    
    if (pHdr->idFrom == IDC_MUSICTREE)
    {
        NM_TREEVIEW* pTreeView = (NM_TREEVIEW*)pHdr;

        switch(pTreeView->hdr.code)
        {
            case TVN_DELETEITEM:
            {
                TreeData* treedata = (TreeData*)pTreeView->itemOld.lParam;

                if(treedata)
                    delete treedata; 

                break;
            }

            case TVN_BEGINDRAG:
            {
                TVBeginDrag(m_hMusicView, pTreeView);
                break;
            }

            case TVN_KEYDOWN:
            {
                TV_KEYDOWN* pnkd = (TV_KEYDOWN*)pHdr; 

                if(pnkd->wVKey == VK_DELETE)
                    RemoveEvent();  

                break;
            }

            case TVN_BEGINLABELEDIT:
            {
                TV_DISPINFO* info = (TV_DISPINFO*)pHdr;
                HTREEITEM item = info->item.hItem;

                if(item == m_hMyMusicItem ||
                   item == m_hPlaylistItem ||
                   item == m_hAllItem ||
                   item == m_hUncatItem ||
                   item == m_hNewPlaylistItem ||
                   item == m_hPortableItem ||
                   TreeView_GetParent(m_hMusicView, item) == m_hPortableItem)
                {
                    result = TRUE;
                }

                break;
            }

            case TVN_ENDLABELEDIT:
            {
                TV_DISPINFO* info = (TV_DISPINFO*)pHdr;
                TV_ITEM tv_item = info->item;

                // was the operation cancelled?
                if(tv_item.pszText)
                {
                    TreeData* treedata = (TreeData*)tv_item.lParam;

                    if(treedata)
                    {
                        if(treedata->IsTrack())
                        {
                            // just change the title for this song
                            UpdateTrackName(treedata->m_pTrack, 
                                            tv_item.pszText);
                        } 
                        else if(treedata->IsPlaylist())
                        {
                            // just change the title for this playlist
                            UpdatePlaylistName(treedata->m_oPlaylistPath, 
                                               tv_item.pszText);
                        }
                        else if(treedata->IsAlbum())
                        {
                            // need to change the album for all tracks in album
                            UpdateAlbumName(treedata->m_pAlbum, 
                                            tv_item.pszText);
                        }
                        else if(treedata->IsArtist())
                        {
                            // need to change the artist for all albums
                            // and tracks by this artist
                            UpdateArtistName(treedata->m_pArtist, 
                                             tv_item.pszText);
                        }
                        else if(treedata->IsUncatagorized())
                        {
                            // just change the title for this song
                            UpdateUncatagorizedTrackName(treedata->m_pTrack, 
                                            tv_item.pszText);
                        }
                    }

                    result = TRUE;
                }

                break;
            }

            case TVN_ITEMEXPANDING:
            {
                if(!TreeView_GetChild(m_hMusicView, 
                                      pTreeView->itemNew.hItem))
                {
                    TreeData* treedata = (TreeData*)pTreeView->itemNew.lParam;

                    if(pTreeView->itemNew.hItem == m_hMyMusicItem)
                    {
                        FillArtists();
                    }
                    else if(pTreeView->itemNew.hItem == m_hAllItem)
                    {
                        FillAllTracks();
                    }
                    else if(pTreeView->itemNew.hItem == m_hUncatItem)
                    {
                        FillUncatTracks();
                    }
                    else if(treedata && treedata->GetLevel() == 1)
                    {
                        FillAlbums(&pTreeView->itemNew);
                    }
                    else if(treedata && treedata->GetLevel() == 2)
                    {
                        FillTracks(&pTreeView->itemNew);
                    }
                    else if(pTreeView->itemNew.hItem == m_hPlaylistItem)
                    {
                        FillPlaylists();
                    }
                    else if(pTreeView->itemNew.hItem == m_hIceCastItem)
                    {
                        FillIceCast();
                    }
                    else if(pTreeView->itemNew.hItem == m_hWiredPlanetItem)
                    {
                        FillWiredPlanet();
                    }
                    else if(pTreeView->itemNew.hItem == m_hPortableItem)
                    {
                        FillPortables();
                    }
                }

                break;
            }

            case TVN_SELCHANGED:
            {
                UpdateButtonMenuStates();
                break;
            }

            case NM_DBLCLK:
            {
                /*int32 lParam;
                lParam = GetCurrentItemFromMousePos();

                TV_ITEM tv_item;
                TV_HITTESTINFO tv_htinfo;


                GetCursorPos(&tv_htinfo.pt);
                ScreenToClient(m_hMusicView, &tv_htinfo.pt);

                if(TreeView_HitTest(m_hMusicView, &tv_htinfo))
                {
                    tv_item.hItem = TreeView_GetSelection(m_hMusicView); 
                    tv_item.mask = TVIF_PARAM | TVIF_HANDLE;
                    TreeView_GetItem(m_hMusicView, &tv_item);

                    TreeData* treedata = (TreeData*)tv_item.lParam;

                    if(treedata->IsTrack())
                    {
                        PlaylistItem *item;
                
                        item = new PlaylistItem(*treedata->m_pTrack);
                        m_oPlm->AddItem(item, false);
                    } 
                    else if(treedata->IsPlaylist())
                    {
                        m_oPlm->ReadPlaylist(treedata->m_oPlaylistPath.c_str());
                    }
                    else if(treedata->IsPortable())
                    {
                        EditPortablePlaylist(treedata->m_pPortable);
                    }
                    else if(tv_htinfo.hItem == m_hNewPlaylistItem)
                    {
                        NewPlaylist();
                    }
                    else if(tv_htinfo.hItem == m_hNewPortableItem)
                    {
                        m_context->target->AcceptEvent(new ShowPreferencesEvent(3));
                    }
                }*/
                break;
            }

            case NM_RCLICK:
            {
                HMENU menu;
                HMENU subMenu;
                POINT sPoint;
                uint32 trackCount = 0;
                uint32 playlistCount = 0;

                trackCount = GetSelectedTrackCount();
                playlistCount = GetSelectedPlaylistCount();
            
                GetCursorPos(&sPoint);

                menu = LoadMenu(g_hinst, MAKEINTRESOURCE(IDR_TVPOPUP));
                subMenu = GetSubMenu(menu, 0);

                if(m_pParent)
                {
                    DeleteMenu(subMenu, ID_POPUP_ADDTRACK_PLAY, MF_BYCOMMAND);
                }

                if( IsItemSelected(m_hMyMusicItem) ||
                    IsItemSelected(m_hPlaylistItem) ||
                    IsItemSelected(m_hAllItem) ||
                    IsItemSelected(m_hUncatItem) ||
                    (IsItemSelected(m_hNewPlaylistItem) && !(playlistCount + trackCount)))
                {
                    EnableMenuItem(subMenu,
                                   ID_POPUP_REMOVE,
                                   MF_BYCOMMAND|MF_GRAYED);
                }


                if( playlistCount > 1 ||
                    IsItemSelected(m_hMyMusicItem) ||
                    IsItemSelected(m_hPlaylistItem) ||
                    IsItemSelected(m_hAllItem) ||
                    IsItemSelected(m_hUncatItem) ||
                    (IsItemSelected(m_hNewPlaylistItem) && !(playlistCount + trackCount)))
                {
                    EnableMenuItem(subMenu,
                                   ID_POPUP_EDITINFO,
                                   MF_BYCOMMAND|MF_GRAYED);    
                }


                if( trackCount > 1 ||
                    playlistCount > 1 ||
                    IsItemSelected(m_hMyMusicItem) ||
                    IsItemSelected(m_hPlaylistItem) ||
                    IsItemSelected(m_hAllItem) ||
                    IsItemSelected(m_hUncatItem) ||
                    IsItemSelected(m_hNewPlaylistItem))
                {
                    EnableMenuItem(subMenu,
                                   ID_POPUP_RENAME,
                                   MF_BYCOMMAND|MF_GRAYED);                         
                }

                if(trackCount == 0)
                {
                    EnableMenuItem(subMenu,
                                   ID_POPUP_EDITINFO,
                                   MF_BYCOMMAND|MF_GRAYED);

                    if( IsItemSelected(m_hNewPlaylistItem) &&
                        playlistCount == 1)
                    {
                        EnableMenuItem(subMenu,
                                   ID_POPUP_ADDTRACK,
                                   MF_BYCOMMAND|MF_GRAYED);

                        EnableMenuItem(subMenu,
                                   ID_POPUP_ADDTRACK_PLAY,
                                   MF_BYCOMMAND|MF_GRAYED);

                        EnableMenuItem(subMenu,
                                   ID_POPUP_REMOVE,
                                   MF_BYCOMMAND|MF_GRAYED);

                        EnableMenuItem(subMenu,
                                   ID_POPUP_RENAME,
                                   MF_BYCOMMAND|MF_GRAYED);

                        EnableMenuItem(subMenu,
                                   ID_POPUP_EDITPLAYLIST,
                                   MF_BYCOMMAND|MF_GRAYED);
                    }
                }

                if(!playlistCount || trackCount)
                {
                    DeleteMenu(subMenu, ID_POPUP_EDITPLAYLIST, MF_BYCOMMAND);
                }

                TrackPopupMenu(subMenu, 
                               TPM_LEFTALIGN|TPM_LEFTBUTTON, 
                               sPoint.x, sPoint.y, 
                               0, m_hWnd, NULL);

                DestroyMenu(menu);

                break;
            }

            /*case ??:
            {
                TV_HITTESTINFO sHit;
                HTREEITEM      hItem;
                POINT          sPoint;
            
                sHit.flags = TVHT_ONITEM;
          
                GetCursorPos(&sPoint);
                ScreenToClient(m_hWnd, &sPoint);
                ClientToWindow(m_hWnd, &sPoint); 
                sHit.pt = sPoint;
                hItem = TreeView_HitTest(m_hMusicView, &sHit);
                if (hItem == m_hPlaylistItem)
                   SendMessage(m_hStatus, SB_SETTEXT, 0, 
                               (LPARAM)"This tree item contains all of your playlists.");
                else               
                if (hItem == m_hMyMusicItem)
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

                break;
            }*/

        }
    }
    else if(pHdr->idFrom == IDC_PLAYLISTBOX)
    {
        NM_LISTVIEW* pListView = (NM_LISTVIEW*)pHdr;

        switch(pListView->hdr.code)
        {
            case LVN_BEGINDRAG:
            {
                LVBeginDrag(m_hPlaylistView, pListView);
                break;
            }

            case NM_RCLICK:
            {
                HMENU menu;
                HMENU subMenu;
                POINT sPoint;
                uint32 trackCount = 0;
                uint32 playlistCount = 0;

                trackCount = GetSelectedTrackCount();
                playlistCount = GetSelectedPlaylistCount();
            
                GetCursorPos(&sPoint);

                menu = LoadMenu(g_hinst, MAKEINTRESOURCE(IDR_LVPOPUP));
                subMenu = GetSubMenu(menu, 0);

                if(m_pParent)
                {
                    DeleteMenu(subMenu, ID_POPUP_PLAY, MF_BYCOMMAND);
                }

                // Can we move items up and down?
                uint32 count = ListView_GetItemCount(m_hPlaylistView);
                uint32 selected = ListView_GetSelectedCount(m_hPlaylistView);

                if(count)
                {
                    uint32 state;

                    EnableMenuItem(subMenu, ID_POPUP_PLAY, MF_BYCOMMAND|
                                   (selected > 1) ? MF_GRAYED : MF_ENABLED );

                    state = ListView_GetItemState(m_hPlaylistView,
                                                  count - 1, 
                                                  LVIS_SELECTED);

                    EnableMenuItem(subMenu, ID_POPUP_MOVEDOWN, MF_BYCOMMAND|
                                   (state & LVIS_SELECTED) ? MF_GRAYED : MF_ENABLED );

                    state = ListView_GetItemState(m_hPlaylistView, 
                                                  0, 
                                                  LVIS_SELECTED);

                    EnableMenuItem(subMenu, ID_POPUP_MOVEUP, MF_BYCOMMAND|
                                   (state & LVIS_SELECTED) ? MF_GRAYED : MF_ENABLED );
                }
                else
                {
                    EnableMenuItem(subMenu, ID_POPUP_PLAY, MF_BYCOMMAND|MF_GRAYED);
                    EnableMenuItem(subMenu, ID_POPUP_MOVEUP, MF_BYCOMMAND|MF_GRAYED);
                    EnableMenuItem(subMenu, ID_POPUP_MOVEDOWN, MF_BYCOMMAND|MF_GRAYED);
                    EnableMenuItem(subMenu, ID_POPUP_REMOVE, MF_BYCOMMAND|MF_GRAYED);
                    EnableMenuItem(subMenu, ID_POPUP_EDITINFO, MF_BYCOMMAND|MF_GRAYED);
                }

                TrackPopupMenu(subMenu, 
                               TPM_LEFTALIGN, sPoint.x, sPoint.y, 
                               0, m_hWnd, NULL);

                DestroyMenu(menu);
                break;
            }

            case LVN_ITEMCHANGED:
            {
                UpdateButtonMenuStates();
                break;
            }

            case NM_DBLCLK:
            {
                // only do this for the root browser
                if(!m_pParent)
                {
                    m_playerEQ->AcceptEvent(new Event(CMD_Stop));
                    m_oPlm->SetCurrentIndex(pListView->iItem);
                    m_playerEQ->AcceptEvent(new Event(CMD_Play));
                }
                break;
            }

            case LVN_COLUMNCLICK:
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
                break;
            }

            case LVN_KEYDOWN:
            {
                LV_KEYDOWN* pnkd = (LV_KEYDOWN*)pHdr; 

                if(pnkd->wVKey == VK_DELETE)
                {
                    RemoveEvent();  
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

                break;
            }
        }
    }
    
    if(pHdr->code == TTN_NEEDTEXT)
    {
        pToolTipText = (LPTOOLTIPTEXT)pHdr;

        switch(pToolTipText->hdr.idFrom)
        {
            case ID_FILE_NEWPLAYLIST:
                pToolTipText->lpszText = "Create a new playlist";
                result = TRUE;
                break;
            case ID_FILE_SAVEPLAYLIST:
                pToolTipText->lpszText = "Save current playlist";
                result = TRUE;
                break;
            case ID_FILE_IMPORT:
                pToolTipText->lpszText = "Import tracks and playlists from disk";
                result = TRUE;
                break;
            case ID_EDIT_REMOVE:
                pToolTipText->lpszText = "Remove selected tracks and playlists";
                result = TRUE;
                break;
            case ID_EDIT_EDITINFO:
                pToolTipText->lpszText = "Edit track or playlist info";
                result = TRUE;
                break;
            case ID_EDIT_ADDTRACK:
                pToolTipText->lpszText = "Add selected tracks and playlists to playlist";
                result = TRUE;
                break;
            case ID_EDIT_ADDFILE:
                pToolTipText->lpszText = "Add files from disk to playlist";
                result = TRUE;
                break;
            case ID_EDIT_MOVEUP:
                pToolTipText->lpszText = "Move selected playlist items up";
                result = TRUE;
                break;
            case ID_EDIT_MOVEDOWN:
                pToolTipText->lpszText = "Move selected playlist items down";
                result = TRUE;
                break;
        }      
    }
 
	return result;
}


void MusicBrowserUI::SortEvent(int id)
{
    PlaylistSortKey oldKey, newKey;
    PlaylistSortType type;
    
    if(!m_oPlm->CountItems())
        return;

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
            m_oPlm->GetPlaylistSortType() == kPlaylistSortType_Ascending)
            ? kPlaylistSortType_Descending : kPlaylistSortType_Ascending;

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

void MusicBrowserUI::PlayNowEvent(void)
{
    if(!m_pParent)
    {
        uint32 count = ListView_GetItemCount(m_hPlaylistView);
        uint32 index = 0;

        for(index = 0; index < count; index++)
        {
            uint32 state = ListView_GetItemState(m_hPlaylistView, 
                                                 index, 
                                                 LVIS_SELECTED);
            if(state & LVIS_SELECTED)
            {
                // only do this for the root browser
                m_playerEQ->AcceptEvent(new Event(CMD_Stop));
                m_oPlm->SetCurrentIndex(index);
                m_playerEQ->AcceptEvent(new Event(CMD_Play));
                break;
            }
        }
    }
}

void MusicBrowserUI::AddTrackEvent(void)
{
    vector<PlaylistItem*> items;
    vector<string> urls;

    GetSelectedMusicTreeItems(&items);   
    
    vector<PlaylistItem*>::iterator i;

    for(i = items.begin(); i != items.end(); i++)
    {
        urls.push_back((*i)->URL().c_str());
    }

    GetSelectedPlaylistItems(&urls);
    //GetSelectedStreamItems(&urls);

    // we know that we are gonna be adding a 
    // bunch of items so let windows know.
    // it will make the adds more efficient
    uint32 newSize = ListView_GetItemCount(m_hPlaylistView);
    newSize += urls.size();
    ListView_SetItemCount(m_hPlaylistView, newSize);

    m_oPlm->AddItems(urls);
    
}

void MusicBrowserUI::AddTrackAndPlayEvent(void)
{
    uint32 count = m_oPlm->CountItems();

    AddTrackEvent();

    if(m_oPlm->CountItems())
    {
        m_oPlm->SetCurrentIndex(count);
        m_context->target->AcceptEvent(new Event(CMD_Play));
    }
}

void MusicBrowserUI::AddFileEvent(HWND hwndParent)
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

    if (FileOpenDialog(hwndParent, 
                       "Add Tracks and Playlists",
                       szFilter, 
                       &oFileList,
                       m_context->prefs,
                       true) )
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
    
    if (m_context->catalog->GetPlaylists()->size() > 0 ||
        m_context->catalog->GetMusicList()->size() > 0 ||
        m_context->catalog->GetUnsortedMusic()->size() > 0)
        return;

    bool welcome = false;

    m_context->prefs->GetPrefBoolean(kWelcomePref, &welcome);

    if(welcome)
    {
        m_context->prefs->SetPrefBoolean(kWelcomePref, false);
        StartStopMusicSearch(true); 
    }
    else
    {
        iRet = MessageBox(m_hWnd, "Your music database does not contain any items.\r\n"
                                  "Would you like to start a search to find music\r\n"
                                  "and playlists on your machine?",
                                  "Search For Music", MB_YESNO|MB_SETFOREGROUND);
        if (iRet == IDYES)
            StartStopMusicSearch();   
    }
}

void MusicBrowserUI::EditPlaylistEvent(void)
{
    TV_ITEM tv_item;

    // get the first playlist item
    tv_item.hItem = TreeView_GetChild(m_hMusicView, m_hPlaylistItem);
    tv_item.mask = TVIF_STATE|TVIF_PARAM;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    // skip the "Create New Playlist..." item
    tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, tv_item.hItem);

    if(tv_item.hItem)
    {
        BOOL result = FALSE;

        do
        {
            result = TreeView_GetItem(m_hMusicView, &tv_item);

            if(result && (tv_item.state & TVIS_SELECTED))
            {
                string playlistPath;

                TreeData* treedata = (TreeData*)tv_item.lParam;

                if(treedata)
                {
                    playlistPath = treedata->m_oPlaylistPath;

                    EditPlaylist(playlistPath);
                }
            }
            
        }while(result && 
               (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
                                                        tv_item.hItem)));
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
            
            item.dwTypeData = "Pa&use the Current Track";
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

            item.dwTypeData = "&Play the Current Track";
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
