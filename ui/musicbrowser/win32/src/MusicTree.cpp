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

char* kMusicCatalog = "My Music Catalog";
char* kAllTracks = "<All>";
char* kUncatagorized = "<Uncategorized>";
char* kPlaylists = "My Playlists";
char* kStreams = "My Favorite Streams";
char* kPortables = "My Portables";


void MusicBrowserUI::InitTree(void)
{                          
    TV_ITEM         sItem;
    TV_INSERTSTRUCT sInsert;
    
    TreeView_DeleteAllItems(GetDlgItem(m_hWnd, IDC_MUSICTREE));
    m_oTreeIndex.Clear();
    
    sItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 
        
    sItem.pszText = kMusicCatalog;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 0;
    sItem.iSelectedImage = 0;
    sItem.cChildren= 1;
    sItem.lParam = -1;
    //sItem.state = TVIS_SELECTED;
    //sItem.stateMask = TVIS_SELECTED;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_FIRST;
    sInsert.hParent = NULL;
    m_hCatalogItem = TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);

    sItem.pszText = kAllTracks;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 1;
    sItem.iSelectedImage = 1;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_FIRST;
    sInsert.hParent = m_hCatalogItem;
    m_hAllItem = TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);

    sItem.pszText = kUncatagorized;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 1;
    sItem.iSelectedImage = 1;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_LAST;
    sInsert.hParent = m_hCatalogItem;
    m_hUncatItem = TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);

    sItem.pszText = kPlaylists;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 1;
    sItem.iSelectedImage = 1;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_LAST;
    sInsert.hParent = NULL;
    m_hPlaylistItem = TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
}


void MusicBrowserUI::FillArtists(void)
{
    TV_INSERTSTRUCT                 sInsert;
    vector<ArtistList *>           *pList;
    vector<ArtistList *>::iterator  i;
    int                             iIndex;
    TreeData                        oCrossRef;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    oCrossRef.m_iLevel = 1;
    pList = (vector<ArtistList *> *)m_context->browser->
                                    m_catalog->GetMusicList();
    for(i = pList->begin(), iIndex = 0; i != pList->end(); i++,iIndex++)
    {
       oCrossRef.m_pArtist = (*i);
       
       sInsert.item.pszText = (char *)(*i)->name.c_str();
       sInsert.item.cchTextMax = (*i)->name.length();
       sInsert.item.iImage = 2;
       sInsert.item.iSelectedImage = 2;
       sInsert.item.cChildren= 1;
       sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
       sInsert.hInsertAfter = TVI_LAST;
       sInsert.hParent = m_hCatalogItem;
       TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
    }    
}

void MusicBrowserUI::FillAlbums(TV_ITEM *pItem)
{
    TV_INSERTSTRUCT                sInsert;
    vector<AlbumList *>::iterator  i;
    int                            iIndex;
    TreeData                       oCrossRef;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    oCrossRef.m_iLevel = 2;
    oCrossRef.m_pArtist = m_oTreeIndex.Data(pItem->lParam).m_pArtist;
    for(i = oCrossRef.m_pArtist->m_albumList->begin(), iIndex = 0; 
        i != oCrossRef.m_pArtist->m_albumList->end(); i++, iIndex++)
    {
        oCrossRef.m_pAlbum = (*i);
        
        if ((*i)->name == string(" ") || 
            (*i)->name.length() == 0)
            sInsert.item.pszText = "Unknown";
        else    
            sInsert.item.pszText = (char *)(*i)->name.c_str();
            
        sInsert.item.cchTextMax = (*i)->name.length();
        sInsert.item.iImage = 3;
        sInsert.item.iSelectedImage = 3;
        sInsert.item.cChildren= 1;
        sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
        sInsert.hInsertAfter = TVI_SORT;
        sInsert.hParent = pItem->hItem;
        TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
    }
}

void MusicBrowserUI::FillTracks(TV_ITEM *pItem)
{
    TV_INSERTSTRUCT                   sInsert;
    vector<PlaylistItem *>::iterator  i;
    int                               iIndex;
    TreeData                          oCrossRef;
    MetaData                          oData;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    oCrossRef.m_iLevel = 3;
    oCrossRef.m_pArtist = m_oTreeIndex.Data(pItem->lParam).m_pArtist;
    oCrossRef.m_pAlbum = m_oTreeIndex.Data(pItem->lParam).m_pAlbum;
    for(i = oCrossRef.m_pAlbum->m_trackList->begin(), iIndex = 0; 
        i != oCrossRef.m_pAlbum->m_trackList->end(); i++, iIndex++)
    {
        oCrossRef.m_pTrack = (*i);

        oData = (*i)->GetMetaData();

        if (oData.Title() == string(" ") || 
            oData.Title().length() == 0)
            sInsert.item.pszText = "Unknown";
        else    
            sInsert.item.pszText = (char *)(oData.Title().c_str());
            
        sInsert.item.cchTextMax = oData.Title().length();
        sInsert.item.iImage = 4;
        sInsert.item.iSelectedImage = 4;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
        sInsert.hInsertAfter = TVI_SORT;
        sInsert.hParent = pItem->hItem;
        TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
    }
}

void MusicBrowserUI::FillAllTracks(void)
{
    TV_INSERTSTRUCT                   sInsert;
    vector<ArtistList *>             *pArtistList;
    vector<ArtistList *>::iterator    i;    
    vector<AlbumList *>              *pAlbumList;
    vector<AlbumList *>::iterator     j;
    vector<PlaylistItem *>           *pList;
    vector<PlaylistItem *>::iterator  k;
    TreeData                          oCrossRef;
    MetaData                          oData;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    oCrossRef.m_iLevel = 3;
    pArtistList = (vector<ArtistList *>*)m_context->browser->
                                         m_catalog->GetMusicList();
    for(i = pArtistList->begin(); i != pArtistList->end(); i++)
    {
        pAlbumList = (vector<AlbumList *>*)(*i)->m_albumList;
        for(j = pAlbumList->begin(); j != pAlbumList->end(); j++)
        {
            pList = (vector<PlaylistItem *>*)(*j)->m_trackList; 
            for(k = pList->begin(); k != pList->end(); k++)
            {
                oCrossRef.m_pArtist = (*i);
                oCrossRef.m_pAlbum = (*j);
                oCrossRef.m_pTrack = (*k);
            
                oData = (*k)->GetMetaData();
            
                if (oData.Title() == string(" ") || 
                    oData.Title().length() == 0)
                    sInsert.item.pszText = "Unknown";
                else    
                    sInsert.item.pszText = (char *)(oData.Title().c_str());
                    
                sInsert.item.cchTextMax = oData.Title().length();
                sInsert.item.iImage = 4;
                sInsert.item.iSelectedImage = 4;
                sInsert.item.cChildren= 0;
                sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
                sInsert.hInsertAfter = TVI_SORT;
                sInsert.hParent = m_hAllItem;
                TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
            }
        }
    }       
    
    oCrossRef.m_iLevel = 3;
    oCrossRef.m_pArtist = NULL;
    oCrossRef.m_pAlbum = NULL;
    
    pList = (vector<PlaylistItem *>*)m_context->browser->
                                     m_catalog->GetUnsortedMusic();
    for(k = pList->begin(); k != pList->end(); k++)
    {
        oCrossRef.m_pTrack = (*k);

        oData = (*k)->GetMetaData();

        if (oData.Title() == string(" ") || 
            oData.Title().length() == 0)
            sInsert.item.pszText = "Unknown";
        else    
            sInsert.item.pszText = (char *)(oData.Title().c_str());
            
        sInsert.item.cchTextMax = oData.Title().length();
        sInsert.item.iImage = 4;
        sInsert.item.iSelectedImage = 4;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
        sInsert.hInsertAfter = TVI_SORT;
        sInsert.hParent = m_hAllItem;
        TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
    }
}

void MusicBrowserUI::FillUncatTracks(void)
{
    TV_INSERTSTRUCT                   sInsert;
    vector<PlaylistItem *>           *pList;
    vector<PlaylistItem *>::iterator  i;
    int                               iIndex;
    TreeData                          oCrossRef;
    MetaData                          oData;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    oCrossRef.m_iLevel = 3;
    oCrossRef.m_pArtist = NULL;
    oCrossRef.m_pAlbum = NULL;
    
    pList = (vector<PlaylistItem *>*)m_context->browser->
                                     m_catalog->GetUnsortedMusic();
    for(i = pList->begin(), iIndex = 0; i != pList->end(); i++, iIndex++)
    {
        oCrossRef.m_pTrack = (*i);
        oData = (*i)->GetMetaData();

        if (oData.Title() == string(" ") || 
            oData.Title().length() == 0)
            sInsert.item.pszText = "Unknown";
        else    
            sInsert.item.pszText = (char *)(oData.Title().c_str());
            
        sInsert.item.cchTextMax = oData.Title().length();
        sInsert.item.iImage = 4;
        sInsert.item.iSelectedImage = 4;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
        sInsert.hInsertAfter = TVI_SORT;
        sInsert.hParent = m_hUncatItem;
        TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
    }
}

int32 MusicBrowserUI::GetMusicTreeSelection(HTREEITEM hItem)
{
    TV_ITEM sItem;
    
    sItem.mask = TVIF_PARAM;
    sItem.hItem = TreeView_GetSelection(GetDlgItem(m_hWnd, IDC_MUSICTREE));
    if (sItem.hItem)
    {
       hItem = sItem.hItem;
       TreeView_GetItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sItem);
       return sItem.lParam;   
    }
    else
    {
       hItem = NULL;
       return -1;
    }   
}    

void MusicBrowserUI::FillPlaylists(void)
{
    TV_INSERTSTRUCT           sInsert;
    vector<string>::iterator  i;
    vector<string>           *pList;
    int                       iIndex;
    char                      szBase[MAX_PATH];
    TreeData                  oData;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    pList = (vector<string>*)m_context->browser->
                             m_catalog->GetPlaylists(); 

    oData.m_iLevel = 1;
    for(i = pList->begin(), iIndex = 0; i != pList->end(); i++,iIndex++)
    {
       if (!iIndex) 
          continue;

       _splitpath((char *)(*i).c_str(), NULL, NULL, szBase, NULL);   
       oData.m_oPlaylistName = szBase;
       oData.m_oPlaylistPath = *i;

       sInsert.item.pszText = szBase;
       sInsert.item.cchTextMax = strlen(szBase);
       sInsert.item.iImage = 2;
       sInsert.item.iSelectedImage = 2;
       sInsert.item.cChildren= 0;
       sInsert.item.lParam = m_oTreeIndex.Add(oData);
       sInsert.hInsertAfter = TVI_SORT;
       sInsert.hParent = m_hPlaylistItem;
       TreeView_InsertItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sInsert);
    }    
}


int32 MusicBrowserUI::GetCurrentItemFromMousePos(void)
{
    TV_ITEM sItem;
    TV_HITTESTINFO tv_htinfo;

    GetCursorPos(&tv_htinfo.pt);
    ScreenToClient(GetDlgItem(m_hWnd, IDC_MUSICTREE), &tv_htinfo.pt);
    if(TreeView_HitTest(GetDlgItem(m_hWnd, IDC_MUSICTREE), &tv_htinfo))
    {
        sItem.hItem = TreeView_GetSelection(GetDlgItem(m_hWnd, IDC_MUSICTREE)); 
        sItem.mask = TVIF_PARAM | TVIF_HANDLE;
        TreeView_GetItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sItem);
        
        return sItem.lParam;
    }
    return -1;
}

void MusicBrowserUI::AddAllTrackURLs(vector<string>* urls)
{
    vector<ArtistList*>*            artistList;
    vector<ArtistList*>::iterator   artist;    

    artistList = (vector<ArtistList*>*)
            m_context->browser->m_catalog->GetMusicList();

    for(artist = artistList->begin(); 
        artist != artistList->end(); 
        artist++)
    {
        vector<AlbumList*>::iterator album;

        for(album = (*artist)->m_albumList->begin();
            album != (*artist)->m_albumList->end();
            album++)
        {
            vector<PlaylistItem*>::iterator track;

            for(track = (*album)->m_trackList->begin();
                track != (*album)->m_trackList->end();
                track++)
            {
                urls->push_back((*track)->URL());
            }
        }
    }

    AddUncatagorizedTrackURLs(urls);
}

void MusicBrowserUI::AddUncatagorizedTrackURLs(vector<string>* urls)
{
    vector<PlaylistItem*>*          trackList;
    vector<PlaylistItem*>::iterator track;

    trackList = (vector<PlaylistItem*>*)
            m_context->browser->m_catalog->GetUnsortedMusic();

    for(track = trackList->begin(); 
        track != trackList->end(); 
        track++)
    {
        urls->push_back((*track)->URL());
    }
}

void MusicBrowserUI::AddTrackURLs(TV_ITEM* tv_item, 
                                  vector<string>* urls)
{
    // we need to determine what this item is 
    // so we can properly iterate it

    if(m_oTreeIndex.IsArtist(tv_item->lParam))
    {
        ArtistList* artist = m_oTreeIndex.Data(tv_item->lParam).m_pArtist;
        vector<AlbumList*>::iterator album;
        
        for(album = artist->m_albumList->begin();
            album != artist->m_albumList->end();
            album++)
        {
            vector<PlaylistItem*>::iterator track;

            for(track = (*album)->m_trackList->begin();
                track != (*album)->m_trackList->end();
                track++)
            {
                urls->push_back((*track)->URL());
            }

        }
    }
    else if(m_oTreeIndex.IsAlbum(tv_item->lParam))
    {
        AlbumList* album = m_oTreeIndex.Data(tv_item->lParam).m_pAlbum;
        vector<PlaylistItem *>::iterator track;

        for(track = album->m_trackList->begin();
            track != album->m_trackList->end();
            track++)
        {
            urls->push_back((*track)->URL());
        }
    }
    else if(m_oTreeIndex.IsTrack(tv_item->lParam))
    {
        PlaylistItem* track = m_oTreeIndex.Data(tv_item->lParam).m_pTrack;

        urls->push_back(track->URL());
    }
}

BOOL MusicBrowserUI::FindSelectedItems(HWND hwnd, HTREEITEM root, vector<string>* urls)
{
    BOOL result = FALSE;
    TV_ITEM tv_item;
    char name[256];

    tv_item.hItem = root;
    tv_item.mask = TVIF_STATE|TVIF_TEXT|TVIF_PARAM;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;
    tv_item.pszText = name;
    tv_item.cchTextMax = sizeof(name);

    do
    {
        result = TreeView_GetItem(hwnd, &tv_item);

        HTREEITEM childItem = TreeView_GetChild(hwnd, tv_item.hItem);

        if(result && (tv_item.state & TVIS_SELECTED))
        {
            AddTrackURLs(&tv_item, urls);
        }
        else if(result && childItem)
        {
            FindSelectedItems(hwnd, childItem, urls);        
        }

    }while(result && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

// get all the selected items... there are some special
// cases:
//
// 1. If "Music Catalog" item is selected we just 
//    add all the items as if it were "All Tracks".
//
// 2. If either "All Tracks" is selected we add all 
//    tracks and stop iterating.
//
// 3. Otherwise we iterate the children and if selected
//    add the track items beneath that item. 

void MusicBrowserUI::GetSelectedMusicTreeItems(vector<string>* urls)
{
    // need to iterate all the items and add selected
    // items and their children
    HTREEITEM rootItem = TreeView_GetRoot(m_hMusicCatalog);

    if(rootItem)
    {
        // is the "Music Catalog" item selected?
        TV_ITEM tv_root;

        tv_root.hItem = rootItem;
        tv_root.mask = TVIF_STATE;
        tv_root.stateMask = TVIS_SELECTED;
        tv_root.state = 0;

        TreeView_GetItem(m_hMusicCatalog, &tv_root);

        // if so then we add all the items under "All Tracks"
        if(tv_root.state & TVIS_SELECTED)
        {
            AddAllTrackURLs(urls);
        }
        else
        {
            TV_ITEM tv_all;

            // is the "All Tracks" item selected
            tv_all.hItem = TreeView_GetChild(m_hMusicCatalog, rootItem);
            tv_all.mask = TVIF_STATE;
            tv_all.stateMask = TVIS_SELECTED;
            tv_all.state = 0;

            TreeView_GetItem(m_hMusicCatalog, &tv_all);

            // if so then we add all the items under "All Tracks"
            if(tv_all.state & TVIS_SELECTED)
            {
                AddAllTrackURLs(urls);
            }
            else // if not we iterate the catalog for selected items
            {
                TV_ITEM tv_uncat;
                

                // is the "Uncatagorized" item selected
                tv_uncat.hItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_all.hItem);
                tv_uncat.mask = TVIF_STATE;
                tv_uncat.stateMask = TVIS_SELECTED;
                tv_uncat.state = 0;

                TreeView_GetItem(m_hMusicCatalog, &tv_uncat);

                HTREEITEM firstSearchItem = tv_uncat.hItem;

                // if so then we add all the items under "Uncatagorized"
                if(tv_all.state & TVIS_SELECTED)
                {
                    AddUncatagorizedTrackURLs(urls);

                    // wanna skip the uncated tracks if we just added them all
                    firstSearchItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_uncat.hItem);
                }

                if(firstSearchItem)
                    FindSelectedItems(m_hMusicCatalog, firstSearchItem, urls);
            }
        }
    }
}

#define IsCtrlDown()  (GetKeyState(VK_CONTROL) < 0)
#define IsShiftDown()  (GetKeyState(VK_SHIFT) < 0)

static BOOL TreeView_SetTree(HWND hwnd, TV_ITEM* root)
{
    BOOL result = FALSE;
    HTREEITEM childItem; 
    TV_ITEM tv_item = *root;

    do
    {
        result = TreeView_SetItem(hwnd, &tv_item);

        childItem = TreeView_GetChild(hwnd, tv_item.hItem);

        if(result && childItem)
        {
            TV_ITEM child_tv_item = tv_item;

            child_tv_item.hItem = childItem;
            result = TreeView_SetTree(hwnd, &child_tv_item);
        }

    }while(result && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

static BOOL TreeView_SetBranch(HWND hwnd, TV_ITEM* root)
{
    BOOL result = FALSE;
    TV_ITEM tv_item = *root;

    result = TreeView_SetItem(hwnd, &tv_item);

    tv_item.hItem = TreeView_GetChild(hwnd, tv_item.hItem);

    if(result && tv_item.hItem)
    {
        TreeView_SetTree(hwnd, &tv_item);
    }
    
    return result;
}

LRESULT WINAPI 
TreeViewWndProc(HWND hwnd, 
                UINT msg, 
                WPARAM wParam, 
                LPARAM lParam)
{
    MusicBrowserUI* ui = (MusicBrowserUI*)GetProp(hwnd, "this" );

    return ui->TreeViewWndProc(hwnd, msg, wParam, lParam);
}

LRESULT MusicBrowserUI::TreeViewWndProc(HWND hwnd, 
                                        UINT msg, 
                                        WPARAM wParam, 
                                        LPARAM lParam)
{
    WNDPROC lpOldProc = (WNDPROC)GetProp(hwnd, "oldproc" );

	switch(msg)
	{
		case WM_DESTROY:   
		{
			//  Put back old window proc and
			SetWindowLong( hwnd, GWL_WNDPROC, (DWORD)lpOldProc );

			// remove window property
			RemoveProp( hwnd, "oldproc" ); 
            RemoveProp( hwnd, "this" ); 

			break;
        }

        case WM_MOUSEMOVE:
        {
			break;
        }

        case WM_LBUTTONDOWN:
        {
            bool shiftKeyPressed = IsShiftDown();
            bool ctrlKeyPressed = IsCtrlDown();

            HTREEITEM item;
            TV_HITTESTINFO hti;

            hti.pt.x = LOWORD(lParam);
            hti.pt.y = HIWORD(lParam);
        
            item = TreeView_HitTest(hwnd, &hti);  

            if(item && (hti.flags & TVHT_ONITEM))
            {
                HTREEITEM focusItem = TreeView_GetSelection(hwnd);
                TV_ITEM tv_item;

                tv_item.hItem = item;
                tv_item.mask = TVIF_STATE;
                tv_item.stateMask = TVIS_SELECTED;

                TreeView_GetItem(hwnd, &tv_item);

                bool wasFocus = item == focusItem;
                bool wasSelected = (tv_item.state & TVIS_SELECTED) != 0;
                
                if(ctrlKeyPressed)
                {
                    /*TreeView_Select(hwnd, item, TVGN_CARET);

                    if(focusItem)
                    {
                        tv_item.hItem = focusItem;
                        tv_item.mask = TVIF_STATE;
                        tv_item.stateMask = TVIS_SELECTED;
                        tv_item.state = TVIS_SELECTED;

                        TreeView_SetItem(hwnd, &tv_item);
                    }*/

                    return TRUE;
                }
                else if(shiftKeyPressed)
                {
                    // need to iterate all the items and 
                    // make sure they aren't selected
                    HTREEITEM rootItem = TreeView_GetRoot(hwnd);

                    if(rootItem)
                    {
                        do
                        {
                            tv_item.hItem = rootItem;
                            tv_item.mask = TVIF_STATE;
                            tv_item.stateMask = TVIS_SELECTED | TVIS_EXPANDEDONCE;
                            tv_item.state = TVIS_EXPANDEDONCE;

                            TreeView_SetBranch(hwnd, &tv_item);
                            
                        }while(rootItem = TreeView_GetNextSibling(hwnd, rootItem));
                    }

                    HTREEITEM topItem = NULL;
                    HTREEITEM bottomItem = NULL;
                    HTREEITEM dummyItem = NULL;

                    dummyItem = focusItem;

                    // which item is above the other? search downward first
                    while(dummyItem = TreeView_GetNextSibling(hwnd, dummyItem))
                    {
                        if(dummyItem == item)
                        {
                            topItem = focusItem;
                            bottomItem = item;
                            break;
                        }
                    }
                    
                    // did we find out? no? search upward next
                    if(!topItem)
                    {
                        dummyItem = focusItem;

                        // which item is above the other? search downward first
                        while(dummyItem = TreeView_GetPrevSibling(hwnd, dummyItem))
                        {
                            if(dummyItem == item)
                            {
                                topItem = item;
                                bottomItem = focusItem;
                                break;
                            }
                        }
                    }

                    // if they are not siblings then we do not support shift
                    // selection so just pass it on
                    if(topItem)
                    {
                        // need to iterate all the items and 
                        // select them
                        
                        rootItem = topItem;

                        do
                        {
                            tv_item.hItem = rootItem;
                            tv_item.mask = TVIF_STATE;
                            tv_item.stateMask = TVIS_SELECTED;
                            tv_item.state = TVIS_SELECTED;

                            TreeView_SetBranch(hwnd, &tv_item);
                    
                        }while(rootItem != bottomItem && (rootItem = TreeView_GetNextSibling(hwnd, rootItem)));

                        
                        return TRUE;
                    }
                }
                else
                {
                    // need to iterate all the items and 
                    // make sure they aren't selected
                    HTREEITEM rootItem = TreeView_GetRoot(hwnd);

                    if(rootItem)
                    {
                        do
                        {
                            tv_item.hItem = rootItem;
                            tv_item.mask = TVIF_STATE;
                            tv_item.stateMask = TVIS_SELECTED | TVIS_EXPANDEDONCE;
                            tv_item.state = TVIS_EXPANDEDONCE;

                            TreeView_SetBranch(hwnd, &tv_item);
                            
                        }while(rootItem = TreeView_GetNextSibling(hwnd, rootItem));
                    }

                    // need to set this back cause windows won't
                    // set it if it is already the focus item and
                    // we just deselected it
                    if(wasSelected && wasFocus)
                    {
                        tv_item.hItem = focusItem;
                        tv_item.mask = TVIF_STATE;
                        tv_item.stateMask = TVIS_SELECTED;
                        tv_item.state = TVIS_SELECTED;

                        TreeView_SetItem(hwnd, &tv_item);
                    }

                    
                }
            }
                    
            break;
        }

        case WM_LBUTTONUP:
        {
            bool shiftKeyPressed = IsShiftDown();
            bool ctrlKeyPressed = IsCtrlDown();

            HTREEITEM item;
            TV_HITTESTINFO hti;

            hti.pt.x = LOWORD(lParam);
            hti.pt.y = HIWORD(lParam);
        
            item = TreeView_HitTest(hwnd, &hti);  

            if(item && (hti.flags & TVHT_ONITEM))
            {
                HTREEITEM focusItem = TreeView_GetSelection(hwnd);
                TV_ITEM tv_item;

                tv_item.hItem = focusItem;
                tv_item.mask = TVIF_STATE;
                tv_item.stateMask = TVIS_SELECTED;

                TreeView_GetItem(hwnd, &tv_item);

                bool wasFocusSelected = (tv_item.state & TVIS_SELECTED) != 0;

                tv_item.hItem = item;
                tv_item.mask = TVIF_STATE;
                tv_item.stateMask = TVIS_SELECTED;

                TreeView_GetItem(hwnd, &tv_item);

                bool wasFocus = item == focusItem;
                bool wasSelected = (tv_item.state & TVIS_SELECTED) != 0;
                
                if(ctrlKeyPressed)
                {
                    TreeView_Select(hwnd, item, TVGN_CARET);

                    if(wasSelected)
                    {
                        tv_item.hItem = item;
                        tv_item.mask = TVIF_STATE;
                        tv_item.stateMask = TVIS_SELECTED;
                        tv_item.state = 0;

                        TreeView_SetItem(hwnd, &tv_item);
                    }


                    if(!wasFocus && wasFocusSelected || !wasSelected)
                    {
                        tv_item.hItem = focusItem;
                        tv_item.mask = TVIF_STATE;
                        tv_item.stateMask = TVIS_SELECTED;
                        tv_item.state = TVIS_SELECTED;

                        TreeView_SetItem(hwnd, &tv_item);
                    }

                    return TRUE;
                }
            }
                    
            break;
        }

        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        {
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        case WM_NOTIFY:
        {
            

            break;
        }
    } 
	
	//  Pass all non-custom messages to old window proc
	return CallWindowProc((int (__stdcall *)(void))lpOldProc, hwnd, msg, wParam, lParam );
}