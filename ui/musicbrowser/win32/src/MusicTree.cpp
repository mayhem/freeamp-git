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

#include <set>
#include <string>
#include <functional>
#include <algorithm>

using namespace std;

#include "config.h"
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"
#include "DropSource.h"
#include "DropObject.h"
#include "registrar.h"

char* kMusicCatalog = "My Music";
char* kAllTracks = "<All>";
char* kUncatagorized = "<Uncategorized>";
char* kPlaylists = "My Playlists";
char* kStreams = "My Favorite Streams";
char* kPortables = "My Portables";
char* kNewPlaylist = "Create a New Playlist...";
char* kNewPortable = "Setup a Portable Player...";

void MusicBrowserUI::InitTree(void)
{                          
    TV_ITEM         sItem;
    TV_INSERTSTRUCT sInsert;
    
    TreeView_DeleteAllItems(m_hMusicCatalog);
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
    m_hCatalogItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);

    /*sItem.pszText = kAllTracks;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 5;
    sItem.iSelectedImage = 5;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_FIRST;
    sInsert.hParent = m_hCatalogItem;
    m_hAllItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);

    sItem.pszText = kUncatagorized;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 6;
    sItem.iSelectedImage = 6;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_LAST;
    sInsert.hParent = m_hCatalogItem;
    m_hUncatItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);*/

    sItem.pszText = kPlaylists;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 1;
    sItem.iSelectedImage = 1;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_LAST;
    sInsert.hParent = NULL;
    m_hPlaylistItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);

    /*sItem.pszText = kPortables;
    sItem.cchTextMax = lstrlen(sItem.pszText);
    sItem.iImage = 7;
    sItem.iSelectedImage = 7;
    sItem.cChildren= 1;
    sItem.lParam = -1;
        
    sInsert.item = sItem;
    sInsert.hInsertAfter = TVI_LAST;
    sInsert.hParent = NULL;
    m_hPortableItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);*/
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
    pList = (vector<ArtistList *> *)m_context->catalog->GetMusicList();
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
       TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }    

    TreeView_SortChildren(m_hMusicCatalog, m_hCatalogItem, 0);
    
    /*TV_ITEM tv_item;


    tv_item.mask = 
    tv_item.hItem = m_hUncatItem;
    TreeView_SetItem(m_hMusicCatalog, &tv_item);

    tv_item.hItem = m_hAllItem
    TreeView_SetItem(m_hMusicCatalog, &tv_item);*/

    sInsert.item.pszText = kUncatagorized;
    sInsert.item.cchTextMax = lstrlen(sInsert.item.pszText);
    sInsert.item.iImage = 6;
    sInsert.item.iSelectedImage = 6;
    sInsert.item.cChildren= 1;
    sInsert.item.lParam = -1;
        
    sInsert.hInsertAfter = TVI_FIRST;
    sInsert.hParent = m_hCatalogItem;
    m_hUncatItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);

    sInsert.item.pszText = kAllTracks;
    sInsert.item.cchTextMax = lstrlen(sInsert.item.pszText);
    sInsert.item.iImage = 5;
    sInsert.item.iSelectedImage = 5;
    sInsert.item.cChildren= 1;
    sInsert.item.lParam = -1;
    sInsert.hInsertAfter = TVI_FIRST;
    sInsert.hParent = m_hCatalogItem;
    m_hAllItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);

    

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
        sInsert.hInsertAfter = TVI_LAST;
        sInsert.hParent = pItem->hItem;
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }

    TreeView_SortChildren(m_hMusicCatalog, pItem->hItem, 0);
}

void MusicBrowserUI::FillTracks(TV_ITEM *pItem)
{
    TV_INSERTSTRUCT                   sInsert;
    vector<PlaylistItem *>::iterator  i;
    int                               iIndex;
    TreeData                          oCrossRef;
    MetaData                          oData;
    //bool                              needToSort = false;

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    oCrossRef.m_iLevel = 3;
    oCrossRef.m_pArtist = m_oTreeIndex.Data(pItem->lParam).m_pArtist;
    oCrossRef.m_pAlbum = m_oTreeIndex.Data(pItem->lParam).m_pAlbum;

    stable_sort(oCrossRef.m_pAlbum->m_trackList->begin(), 
                oCrossRef.m_pAlbum->m_trackList->end(), 
                TrackSort());

    for(i = oCrossRef.m_pAlbum->m_trackList->begin(), iIndex = 0; 
        i != oCrossRef.m_pAlbum->m_trackList->end(); i++, iIndex++)
    {
        oCrossRef.m_pTrack = (*i);

        oData = (*i)->GetMetaData();

        if(oData.Track() == 0)
            sInsert.hInsertAfter = TVI_SORT;
        else
            sInsert.hInsertAfter = TVI_LAST;

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
        //sInsert.hInsertAfter = TVI_LAST;
        sInsert.hParent = pItem->hItem;
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }

    //if(needToSort)
        //TreeView_SortChildren(m_hMusicCatalog, pItem->hItem, 0);
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
    pArtistList = (vector<ArtistList *>*)m_context->catalog->GetMusicList();
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
                sInsert.hInsertAfter = TVI_LAST;
                sInsert.hParent = m_hAllItem;
                TreeView_InsertItem(m_hMusicCatalog, &sInsert);
            }
        }
    }       
    
    oCrossRef.m_iLevel = 3;
    oCrossRef.m_pArtist = NULL;
    oCrossRef.m_pAlbum = NULL;
    
    pList = (vector<PlaylistItem *>*)m_context->catalog->GetUnsortedMusic();
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
        sInsert.hInsertAfter = TVI_LAST;
        sInsert.hParent = m_hAllItem;
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }

    TreeView_SortChildren(m_hMusicCatalog, m_hAllItem, 0);
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
    
    pList = (vector<PlaylistItem *>*)m_context->catalog->GetUnsortedMusic();
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
        sInsert.hInsertAfter = TVI_LAST;
        sInsert.hParent = m_hUncatItem;
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }

    TreeView_SortChildren(m_hMusicCatalog, m_hUncatItem, 0);
}

int32 MusicBrowserUI::GetMusicTreeSelection(HTREEITEM* hItem)
{
    TV_ITEM sItem;
    
    sItem.mask = TVIF_PARAM;
    sItem.hItem = TreeView_GetSelection(m_hMusicCatalog);
    if (sItem.hItem)
    {
       *hItem = sItem.hItem;
       TreeView_GetItem(m_hMusicCatalog, &sItem);
       return sItem.lParam;   
    }
    else
    {
       *hItem = NULL;
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

    pList = (vector<string>*)m_context->catalog->GetPlaylists(); 

    oData.m_iLevel = 1;

    for(i = pList->begin(), iIndex = 0; i != pList->end(); i++,iIndex++)
    {
        _splitpath((char *)(*i).c_str(), NULL, NULL, szBase, NULL);   
        oData.m_oPlaylistName = string(szBase);
        oData.m_oPlaylistPath = *i;

        sInsert.item.pszText = szBase;
        sInsert.item.cchTextMax = strlen(szBase);
        sInsert.item.iImage = 1;
        sInsert.item.iSelectedImage = 1;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = m_oTreeIndex.Add(oData);
        sInsert.hInsertAfter = TVI_SORT;
        sInsert.hParent = m_hPlaylistItem;
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }
    
    sInsert.item.pszText = kNewPlaylist;
    sInsert.item.cchTextMax = strlen(kNewPlaylist);
    sInsert.item.iImage = 1;
    sInsert.item.iSelectedImage = 1;
    sInsert.item.cChildren= 0;
    sInsert.item.lParam = -1;
    sInsert.hInsertAfter = TVI_FIRST;
    sInsert.hParent = m_hPlaylistItem;
    m_hNewPlaylistItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
}

void MusicBrowserUI::FillPortables(void)
{
    TV_INSERTSTRUCT         sInsert;
    char*                   buffer = NULL;  
    uint32                  size = 0;
    set<string>             portablePlayers;
    TreeData                oData;
    uint32                  count = 0;
    HTREEITEM               item;

    // remove old ones if they exist
    while(item = TreeView_GetChild(m_hMusicCatalog, m_hPortableItem))
    {
        TreeView_DeleteItem(m_hMusicCatalog, item);
    }

    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    if(kError_BufferTooSmall == m_context->prefs->GetUsersPortablePlayers(buffer, &size))
    {
        buffer = (char*)malloc(size);

        if(buffer)
        {
            m_context->prefs->GetUsersPortablePlayers(buffer, &size);
            char* cp = buffer;
            char* name = cp;

            while(cp = strchr(cp, ';'))
            {
                *cp = 0x00;
                portablePlayers.insert(string(name));

                cp++;
                name = cp;
            }

            if(*name)
            {
                portablePlayers.insert(string(name));
            }

            free(buffer);
        }
    }
   
    uint32 i = 0;
    DeviceInfo device;

    while(IsntError(m_oPlm->GetSupportedPortables(&device, i++)))
    {

        if( portablePlayers.end() != 
            portablePlayers.find(string(device.GetPluginName())) )
        {
            string displayString;

            displayString = device.GetManufacturer();
            displayString += " ";
            displayString += device.GetDevice();

            if(displayString.size())
            {
                sInsert.item.pszText = (char*)displayString.c_str();
            }
            else
            {
               sInsert.item.pszText = (char*)device.GetPluginName();
            }

            oData.m_pPortable = new DeviceInfo(device);
                        
            sInsert.item.cchTextMax = strlen(sInsert.item.pszText);
            sInsert.item.iImage = 7;
            sInsert.item.iSelectedImage = 7;
            sInsert.item.cChildren= 0;
            sInsert.item.lParam = m_oTreeIndex.Add(oData);
            sInsert.hInsertAfter = TVI_SORT;
            sInsert.hParent = m_hPortableItem;
            TreeView_InsertItem(m_hMusicCatalog, &sInsert);
            count++;
        }
    }

    if(!count)
    {
        sInsert.item.pszText = kNewPortable;
        sInsert.item.cchTextMax = strlen(sInsert.item.pszText);
        sInsert.item.iImage = 7;
        sInsert.item.iSelectedImage = 7;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = -1;
        sInsert.hInsertAfter = TVI_FIRST;
        sInsert.hParent = m_hPortableItem;
        m_hNewPortableItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }
}

int32 MusicBrowserUI::GetCurrentItemFromMousePos(void)
{
    TV_ITEM sItem;
    TV_HITTESTINFO tv_htinfo;

    GetCursorPos(&tv_htinfo.pt);
    ScreenToClient(m_hMusicCatalog, &tv_htinfo.pt);
    if(TreeView_HitTest(m_hMusicCatalog, &tv_htinfo))
    {
        sItem.hItem = TreeView_GetSelection(m_hMusicCatalog); 
        sItem.mask = TVIF_PARAM | TVIF_HANDLE;
        TreeView_GetItem(m_hMusicCatalog, &sItem);
        
        return sItem.lParam;
    }
    return -1;
}

HTREEITEM MusicBrowserUI::FindArtist(const ArtistList* artist)
{
    HTREEITEM result = NULL;
    HWND hwnd = m_hMusicCatalog;

    TV_ITEM tv_item;

    tv_item.hItem = m_hUncatItem;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first artist
    tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            if(artist == m_oTreeIndex.Data(tv_item.lParam).m_pArtist)
            {
                result = tv_item.hItem;
                break;
            }
        }

    }while(success && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

HTREEITEM MusicBrowserUI::FindAlbum(HTREEITEM artistItem, const AlbumList* album)
{
    HTREEITEM result = NULL;
    HWND hwnd = m_hMusicCatalog;

    TV_ITEM tv_item;

    tv_item.hItem = NULL;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first artist
    tv_item.hItem = TreeView_GetChild(m_hMusicCatalog, artistItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            if(album == m_oTreeIndex.Data(tv_item.lParam).m_pAlbum)
            {
                result = tv_item.hItem;
                break;
            }
        }

    }while(success && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

HTREEITEM MusicBrowserUI::FindTrack(HTREEITEM albumItem, const PlaylistItem* track)
{
    HTREEITEM result = NULL;
    HWND hwnd = m_hMusicCatalog;

    TV_ITEM tv_item;

    tv_item.hItem = NULL;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first artist
    tv_item.hItem = TreeView_GetChild(m_hMusicCatalog, albumItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            if(track == m_oTreeIndex.Data(tv_item.lParam).m_pTrack)
            {
                result = tv_item.hItem;
                break;
            }
        }

    }while(success && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

HTREEITEM MusicBrowserUI::FindPlaylist(const string playlist)
{
    HTREEITEM result = NULL;
    HWND hwnd = m_hMusicCatalog;

    TV_ITEM tv_item;

    tv_item.hItem = NULL;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first playlist
    tv_item.hItem = TreeView_GetChild(m_hMusicCatalog, m_hPlaylistItem);

    // but we want the second one... not the magic "Create New Playlist..."
    tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_item.hItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            if(playlist == m_oTreeIndex.Data(tv_item.lParam).m_oPlaylistPath)
            {
                result = tv_item.hItem;
                break;
            }
        }

    }while(success && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}



void MusicBrowserUI::MusicCatalogTrackChanged(const ArtistList *oldArtist,
                                              const ArtistList *newArtist,
                                              const AlbumList *oldAlbum,
                                              const AlbumList *newAlbum,
                                              const PlaylistItem *oldItem,
                                              const PlaylistItem *newItem)
{
    HTREEITEM artistItem = NULL;
    HTREEITEM albumItem = NULL;
    HTREEITEM trackItem = NULL;
    UINT artistState = 0;
    UINT albumState = 0;
    
    // is this in the uncatagorized section?
    if(!oldArtist) 
    {
        trackItem = FindTrack(m_hUncatItem, oldItem);
    }
    else
    {
        artistItem = FindArtist(oldArtist);

        if(artistItem)
        {
            TV_ITEM tv_item;

            tv_item.hItem = artistItem;
            tv_item.mask = TVIF_STATE;
            tv_item.stateMask = TVIS_SELECTED|TVIS_EXPANDED;
            tv_item.state = 0;

            TreeView_GetItem(m_hMusicCatalog, &tv_item);

            artistState = tv_item.state;

            albumItem = FindAlbum(artistItem, oldAlbum);

            if(albumItem)
            {
                tv_item.hItem = albumItem;
                tv_item.mask = TVIF_STATE;
                tv_item.stateMask = TVIS_SELECTED|TVIS_EXPANDED;
                tv_item.state = 0;

                TreeView_GetItem(m_hMusicCatalog, &tv_item);

                albumState = tv_item.state;

                //trackItem = FindTrack(albumItem, oldItem);
            }
        }
    }   

    MusicCatalogTrackRemoved(oldArtist, oldAlbum, oldItem);

    MusicCatalogTrackAdded(newArtist,newAlbum, newItem);   

    // is this in the uncatagorized section?
    if(!oldArtist) 
    {
        //trackItem = FindTrack(m_hUncatItem, newItem);
    }
    else
    {
        artistItem = FindArtist(newArtist);

        if(artistItem)
        {
            if(artistState & TVIS_EXPANDED)
                TreeView_Expand(m_hMusicCatalog, artistItem, TVE_EXPAND);

            albumItem = FindAlbum(artistItem, newAlbum);

            if(albumItem)
            {
                if(albumState & TVIS_EXPANDED)
                    TreeView_Expand(m_hMusicCatalog, albumItem, TVE_EXPAND);

                //trackItem = FindTrack(albumItem, oldItem);
            }
        }
    }   
}

void MusicBrowserUI::MusicCatalogPlaylistRemoved(string item)
{
    HTREEITEM playlistItem = NULL;

    playlistItem = FindPlaylist(item);

    if(playlistItem)
    {
        TreeView_DeleteItem(m_hMusicCatalog, playlistItem);
    }
}

void MusicBrowserUI::MusicCatalogPlaylistAdded(string item)
{
    // put it under playlists
    if(TreeView_GetChild(m_hMusicCatalog, m_hPlaylistItem))
    {
        TV_INSERTSTRUCT sInsert;
        TreeData        oCrossRef;
        MetaData        oData;
        char            szBase[MAX_PATH];

        _splitpath((char *)item.c_str(), NULL, NULL, szBase, NULL);  

        sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                            TVIF_SELECTEDIMAGE | TVIF_PARAM; 

        oCrossRef.m_iLevel = 1;

        oCrossRef.m_oPlaylistName = string(szBase);
        oCrossRef.m_oPlaylistPath = item;

        sInsert.item.pszText = szBase;
        sInsert.item.cchTextMax = strlen(szBase);
        sInsert.item.iImage = 1;
        sInsert.item.iSelectedImage = 1;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
        sInsert.hInsertAfter = TVI_SORT;
        sInsert.hParent = m_hPlaylistItem;
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);

        TreeView_DeleteItem(m_hMusicCatalog, m_hNewPlaylistItem);
        sInsert.item.pszText = kNewPlaylist;
        sInsert.item.cchTextMax = strlen(kNewPlaylist);
        sInsert.item.iImage = 1;
        sInsert.item.iSelectedImage = 1;
        sInsert.item.cChildren= 0;
        sInsert.item.lParam = -1;
        sInsert.hInsertAfter = TVI_FIRST;
        sInsert.hParent = m_hPlaylistItem;
        m_hNewPlaylistItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }
}

void MusicBrowserUI::MusicCatalogTrackRemoved(const ArtistList* artist,
                                              const AlbumList* album,
                                              const PlaylistItem* item)
{
    HTREEITEM artistItem = NULL;
    HTREEITEM albumItem = NULL;
    HTREEITEM trackItem = NULL;
    

    // is this in the uncatagorized section?
    if(!artist) 
    {
        trackItem = FindTrack(m_hUncatItem, item);
    }
    else
    {
        artistItem = FindArtist(artist);

        if(artistItem)
        {
            albumItem = FindAlbum(artistItem, album);

            if(albumItem)
            {
                trackItem = FindTrack(albumItem, item);
            }
        }
    }

    if(trackItem)
    {
       TreeView_DeleteItem(m_hMusicCatalog, trackItem);
    }

    if(albumItem && !album->m_trackList->size())
    {
        TreeView_DeleteItem(m_hMusicCatalog, albumItem);
    }

    if(artistItem && !artist->m_albumList->size())
    {
        TreeView_DeleteItem(m_hMusicCatalog, artistItem);
    }

    trackItem = FindTrack(m_hAllItem, item);

    if(trackItem)
    {
        TreeView_DeleteItem(m_hMusicCatalog, trackItem);
    }

}

void MusicBrowserUI::MusicCatalogTrackAdded(const ArtistList* artist,
                                            const AlbumList* album,
                                            const PlaylistItem* item)
{
    const MetaData meta = item->GetMetaData();
    HTREEITEM newItem = NULL;

    // can we catagorize this track?
    if(!artist) 
    {
        // nope, stick in uncatagorized
        TV_INSERTSTRUCT sInsert;
        TreeData        oCrossRef;
        MetaData        oData;

        sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                            TVIF_SELECTEDIMAGE | TVIF_PARAM; 

        oCrossRef.m_iLevel = 3;
        oCrossRef.m_pArtist = NULL;
        oCrossRef.m_pAlbum = NULL;
    
        
        oCrossRef.m_pTrack = (PlaylistItem*)item;
        oData = item->GetMetaData();

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
        newItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }
    else 
    {
        // yep, search for it
        HTREEITEM artistItem, albumItem;

        artistItem = FindArtist(artist);

        if(artistItem)
        {
            albumItem = FindAlbum(artistItem, album);

            if(albumItem)
            {
                if(TreeView_GetChild(m_hMusicCatalog, albumItem))
                {
                    TV_INSERTSTRUCT sInsert;
                    TreeData        oCrossRef;
                    MetaData        oData;

                    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

                    oCrossRef.m_iLevel = 3;
                    oCrossRef.m_pArtist = (ArtistList*)artist;
                    oCrossRef.m_pAlbum = (AlbumList*)album;
    
                    oCrossRef.m_pTrack = (PlaylistItem*)item;
                    oData = item->GetMetaData();

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
                    sInsert.hInsertAfter = TVI_LAST;
                    sInsert.hParent = albumItem;

                    TV_ITEM tv_item;
                    HTREEITEM sibling = NULL;

                    if(tv_item.hItem = TreeView_GetChild(m_hMusicCatalog, albumItem))
                    {
                        BOOL success;

                        do
                        {
                            success = TreeView_GetItem(m_hMusicCatalog, &tv_item);

                            if(success)
                            {
                                PlaylistItem* track = m_oTreeIndex.Data(tv_item.lParam).m_pTrack;
                                MetaData metadata = track->GetMetaData();
                                
                                if(oData.Track())
                                {
                                    if(metadata.Track() > oData.Track())
                                    {
                                        if(sibling)
                                            sInsert.hInsertAfter = sibling;
                                        else
                                            sInsert.hInsertAfter = TVI_FIRST;
                                        break;
                                    }
                                }
                                else
                                {
                                    if(metadata.Track() || metadata.Title() >  oData.Title())
                                    {
                                        if(sibling)
                                            sInsert.hInsertAfter = sibling;
                                        else
                                            sInsert.hInsertAfter = TVI_FIRST;
                                        break;
                                    }
                                }
                            }

                            sibling = tv_item.hItem;
        
                        }while(success && 
                               (tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, 
                                                                        tv_item.hItem)));
                    }

                    newItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
                }
            }
            else // might need to add the album
            {
                if(TreeView_GetChild(m_hMusicCatalog, artistItem))
                {
                    TV_INSERTSTRUCT sInsert;
                    TreeData        oCrossRef; 

                    sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

                    oCrossRef.m_iLevel = 2;
                    oCrossRef.m_pArtist = (ArtistList*)artist;
                    oCrossRef.m_pAlbum = (AlbumList*)album;

                    if (album->name == string(" ") || 
                        album->name.length() == 0)
                        sInsert.item.pszText = "Unknown";
                    else    
                        sInsert.item.pszText = (char *)album->name.c_str();
    
                    sInsert.item.cchTextMax = album->name.length();
                    sInsert.item.iImage = 3;
                    sInsert.item.iSelectedImage = 3;
                    sInsert.item.cChildren= 1;
                    sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
                    sInsert.hInsertAfter = TVI_SORT;
                    sInsert.hParent = artistItem;
                    newItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
                }
            }
        }
        else // might need to add the artist
        {
            if(TreeView_GetNextSibling(m_hMusicCatalog, m_hUncatItem))
            {
                TV_INSERTSTRUCT sInsert;
                TreeData        oCrossRef;

                sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                             TVIF_SELECTEDIMAGE | TVIF_PARAM; 

                oCrossRef.m_iLevel = 1;
                oCrossRef.m_pArtist = (ArtistList*)artist;

                sInsert.item.pszText = (char*)artist->name.c_str();
                sInsert.item.cchTextMax = artist->name.length();
                sInsert.item.iImage = 2;
                sInsert.item.iSelectedImage = 2;
                sInsert.item.cChildren= 1;
                sInsert.item.lParam = m_oTreeIndex.Add(oCrossRef);
                sInsert.hInsertAfter = TVI_LAST;
                sInsert.hParent = m_hCatalogItem;

                TV_ITEM tv_item;
                HTREEITEM sibling = m_hUncatItem;

                if(tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, m_hUncatItem))
                {
                    BOOL success;

                    do
                    {
                        success = TreeView_GetItem(m_hMusicCatalog, &tv_item);

                        if(success)
                        {
                            ArtistList* artist2 = m_oTreeIndex.Data(tv_item.lParam).m_pArtist;
                            
                            if(lstrcmp(artist2->name.c_str(), artist->name.c_str()) > 0)
                            {
                                sInsert.hInsertAfter = sibling;
                                break;
                            }
                        }

                        sibling = tv_item.hItem;
    
                    }while(success && 
                           (tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, 
                                                                    tv_item.hItem)));
                }

                newItem = TreeView_InsertItem(m_hMusicCatalog, &sInsert);
            }       
        }
    }

    //TreeView_EnsureVisible(m_hMusicCatalog, newItem);

    // put it under "All Tracks"
    if(TreeView_GetChild(m_hMusicCatalog, m_hAllItem))
    {
        TV_INSERTSTRUCT sInsert;
        TreeData        oCrossRef;
        MetaData        oData;

        sInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                            TVIF_SELECTEDIMAGE | TVIF_PARAM;

        oCrossRef.m_iLevel = 3;
        oCrossRef.m_pArtist = (ArtistList*)artist;
        oCrossRef.m_pAlbum = (AlbumList*)album;

        oCrossRef.m_pTrack = (PlaylistItem*)item;
        oData = item->GetMetaData();

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
        TreeView_InsertItem(m_hMusicCatalog, &sInsert);
    }
}

void MusicBrowserUI::TVBeginDrag(HWND hwnd, NM_TREEVIEW* nmtv)
{
    if(nmtv->itemNew.hItem != m_hNewPlaylistItem && 
       nmtv->itemNew.hItem != m_hPortableItem &&
       nmtv->itemNew.hItem != m_hNewPortableItem)
    {
        vector<PlaylistItem*> items;
        vector<string>* urls = new vector<string>;

        GetSelectedMusicTreeItems(&items); 

        vector<PlaylistItem*>::iterator i;

        for(i = items.begin(); i != items.end(); i++)
        {
            urls->push_back((*i)->URL().c_str());
        }

        GetSelectedPlaylistItems(urls);

        HIMAGELIST himl;
        RECT rcItem;
        POINT hotspot;

        himl = TreeView_CreateDragImage(hwnd, nmtv->itemNew.hItem);

        TreeView_GetItemRect(hwnd, nmtv->itemNew.hItem, &rcItem, TRUE); 

        hotspot.x = 0;
        hotspot.y = (rcItem.bottom - rcItem.top)/2;

        DataObject* data = new DataObject(CFSTR_FREEAMP_CATALOGITEM, urls);
        DropSource* src = new DropSource(hwnd, himl, hotspot, nmtv->ptDrag);
        DWORD dwEffect = 0;

        DoDragDrop(data, src, DROPEFFECT_COPY|DROPEFFECT_SCROLL, &dwEffect); 

        data->Release();
        src->Release();
    }
}

void MusicBrowserUI::AddAllTrackItems(vector<PlaylistItem*>* items)
{
    vector<ArtistList*>*            artistList;
    vector<ArtistList*>::iterator   artist;    

    artistList = (vector<ArtistList*>*)
            m_context->catalog->GetMusicList();

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
                items->push_back(*track);
            }
        }
    }

    AddUncatagorizedTrackItems(items);
}

void MusicBrowserUI::AddUncatagorizedTrackItems(vector<PlaylistItem*>* items)
{
    vector<PlaylistItem*>*          trackList;
    vector<PlaylistItem*>::iterator track;

    trackList = (vector<PlaylistItem*>*)
            m_context->catalog->GetUnsortedMusic();

    for(track = trackList->begin(); 
        track != trackList->end(); 
        track++)
    {
        items->push_back(*track);
    }
}

void MusicBrowserUI::AddTrackItems(TV_ITEM* tv_item, 
                                   vector<PlaylistItem*>* items)
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
                items->push_back(*track);
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
            items->push_back(*track);
        }
    }
    else if(m_oTreeIndex.IsTrack(tv_item->lParam) || m_oTreeIndex.IsUncatagorized(tv_item->lParam))
    {
        PlaylistItem* track = m_oTreeIndex.Data(tv_item->lParam).m_pTrack;

        items->push_back(track);
    }
}

BOOL MusicBrowserUI::FindSelectedItems(HTREEITEM root, vector<PlaylistItem*>* items)
{
    BOOL result = FALSE;
    HWND hwnd = m_hMusicCatalog;
    TV_ITEM tv_item;

    tv_item.hItem = root;
    tv_item.mask = TVIF_STATE|TVIF_PARAM;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    do
    {
        result = TreeView_GetItem(hwnd, &tv_item);

        HTREEITEM childItem = TreeView_GetChild(hwnd, tv_item.hItem);

        if(result && (tv_item.state & TVIS_SELECTED))
        {
            AddTrackItems(&tv_item, items);
        }
        else if(result && childItem)
        {
            FindSelectedItems(childItem, items);        
        }

    }while(result && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

void MusicBrowserUI::GetSelectedPlaylistItems(vector<string>* urls)
{
    TV_ITEM tv_root;

    tv_root.hItem = m_hPlaylistItem;
    tv_root.mask = TVIF_STATE;
    tv_root.stateMask = TVIS_SELECTED;
    tv_root.state = 0;

    TreeView_GetItem(m_hMusicCatalog, &tv_root);

    // if selected then we add all the playlists
    if(tv_root.state & TVIS_SELECTED)
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

                if(result)
                {
                    string playlist;

                    playlist = m_oTreeIndex.Data(tv_item.lParam).m_oPlaylistPath;

                    urls->push_back(playlist);
                }
        
            }while(result && 
                   (tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, 
                                                            tv_item.hItem)));
        }    
       
    }
    else // iterate the playlists and add selected ones
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
                    string playlist;

                    playlist = m_oTreeIndex.Data(tv_item.lParam).m_oPlaylistPath;

                    urls->push_back(playlist);
                }
        
            }while(result && 
                   (tv_item.hItem = TreeView_GetNextSibling(m_hMusicCatalog, 
                                                            tv_item.hItem)));
        }
    }
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

void MusicBrowserUI::GetSelectedMusicTreeItems(vector<PlaylistItem*>* items)
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
            AddAllTrackItems(items);
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
                AddAllTrackItems(items);
            }
            else // if not we iterate the catalog for selected items
            {
                HTREEITEM firstSearchItem = TreeView_GetChild(m_hMusicCatalog, tv_all.hItem);

                if(firstSearchItem)
                    FindSelectedItems(firstSearchItem, items);

                TV_ITEM tv_uncat;
                
                // is the "Uncatagorized" item selected
                tv_uncat.hItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_all.hItem);
                tv_uncat.mask = TVIF_STATE;
                tv_uncat.stateMask = TVIS_SELECTED;
                tv_uncat.state = 0;

                TreeView_GetItem(m_hMusicCatalog, &tv_uncat);

                firstSearchItem = tv_uncat.hItem;

                // if so then we add all the items under "Uncatagorized"
                if(tv_uncat.state & TVIS_SELECTED)
                {
                    AddUncatagorizedTrackItems(items);

                    // wanna skip the uncated tracks if we just added them all
                    firstSearchItem = TreeView_GetNextSibling(m_hMusicCatalog, tv_uncat.hItem);
                }

                if(firstSearchItem)
                    FindSelectedItems(firstSearchItem, items);
            }
        }
    }
}

bool MusicBrowserUI::IsItemSelected(HTREEITEM item)
{
    bool result = false;
    BOOL success = FALSE;
    HWND hwnd = m_hMusicCatalog;
    TV_ITEM tv_item;

    tv_item.hItem = item;
    tv_item.mask = TVIF_STATE;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    success = TreeView_GetItem(hwnd, &tv_item);

    if(success && (tv_item.state & TVIS_SELECTED))
    {
        result = true;
    }

    return result;
}

uint32 MusicBrowserUI::CountSelectedItems(HTREEITEM root)
{
    uint32 result = 0;
    BOOL success = FALSE;
    HWND hwnd = m_hMusicCatalog;
    TV_ITEM tv_item;

    tv_item.hItem = root;
    tv_item.mask = TVIF_STATE;
    tv_item.stateMask = TVIS_SELECTED;
    tv_item.state = 0;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        HTREEITEM childItem = TreeView_GetChild(hwnd, tv_item.hItem);

        if(success && (tv_item.state & TVIS_SELECTED))
        {
            result++;
        }
        
        if(success && childItem)
        {
            result += CountSelectedItems(childItem);        
        }

    }while(success && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

uint32 MusicBrowserUI::GetSelectedTrackCount()
{
    uint32 result = 0;
    HTREEITEM rootItem = TreeView_GetChild(m_hMusicCatalog, m_hCatalogItem);

    if(rootItem)
    {
        result = CountSelectedItems(rootItem);
    }

    return result;
}

uint32 MusicBrowserUI::GetSelectedPlaylistCount()
{
    uint32 result = 0;
    HTREEITEM rootItem = TreeView_GetChild(m_hMusicCatalog, m_hPlaylistItem);

    if(rootItem)
    {
        result = CountSelectedItems(rootItem);
    }

    return result;
}


void MusicBrowserUI::UpdateUncatagorizedTrackName(PlaylistItem* track, 
                                                  const char* name)
{
    MetaData metadata = track->GetMetaData();

    metadata.SetTitle(name);

    track->SetMetaData(&metadata);

    m_context->catalog->UpdateSong(track);
}


void MusicBrowserUI::UpdateTrackName(PlaylistItem* track, 
                                     const char* name)
{
    MetaData metadata = track->GetMetaData();

    metadata.SetTitle(name);

    track->SetMetaData(&metadata);

    m_context->catalog->UpdateSong(track);
}

const char* kFileExists = "There is already a playlist with that name.";
                          
void MusicBrowserUI::UpdatePlaylistName(const string& playlist, 
                                        const char* name)
{
    char path[MAX_PATH];
    char orig[MAX_PATH];
    uint32 len = sizeof(orig);

    URLToFilePath(playlist.c_str(), orig, &len);

    strcpy(path, orig);

    char* cp = strrchr(path, '\\');

    if(cp)
    {
        strcpy(cp + 1, name);

        cp = strrchr(playlist.c_str(), '.');

        if(cp)
        {
            strcat(path, cp);
        }
    }

    if(strcmp(orig, path))
    {
        HANDLE findFileHandle = NULL;
        WIN32_FIND_DATA findData;

        findFileHandle = FindFirstFile(path, &findData);

        if(findFileHandle == INVALID_HANDLE_VALUE)
        {
            rename(orig, path);

            char url[MAX_PATH + 7];
            uint32 len = sizeof(url);

            FilePathToURL(path, url, &len);

            m_context->catalog->RemovePlaylist(playlist.c_str());
            m_context->catalog->AddPlaylist(url);
        }
        else
        {
            MessageBox(m_hWnd, 
                       kFileExists, 
                       "Unable to Rename Playlist", 
                       MB_OK|MB_ICONERROR);

            FindClose(findFileHandle);
        }        
    }
}

void MusicBrowserUI::UpdateAlbumName(AlbumList* album, 
                                     const char* name)      
{
    vector<PlaylistItem *>::reverse_iterator track;

    for(track = album->m_trackList->rbegin();
        track != album->m_trackList->rend();
        track++)
    {
        MetaData metadata = (*track)->GetMetaData();

        metadata.SetAlbum(name);

        (*track)->SetMetaData(&metadata);

        m_context->catalog->UpdateSong(*track);
    }
}

void MusicBrowserUI::UpdateArtistName(ArtistList* artist, 
                                      const char* name)
{
    vector<AlbumList*>::reverse_iterator album;
        
    for(album = artist->m_albumList->rbegin();
        album != artist->m_albumList->rend();
        album++)
    {
        vector<PlaylistItem*>::reverse_iterator track;

        for(track = (*album)->m_trackList->rbegin();
            track != (*album)->m_trackList->rend();
            track++)
        {
            MetaData metadata = (*track)->GetMetaData();

            metadata.SetArtist(name);

            (*track)->SetMetaData(&metadata);

            m_context->catalog->UpdateSong(*track);
        }

    }
}

// Function object used for sorting PlaylistItems in PlaylistManager
bool TrackSort::operator() (PlaylistItem* item1, 
                            PlaylistItem* item2) const
{
    bool result = true;

    assert(item1);
    assert(item2);

    if(item1 && item2)
    {
        MetaData m1 = item1->GetMetaData();
        MetaData m2 = item2->GetMetaData();

        result = m1.Track() < m2.Track();        
    }

    return result;
}
