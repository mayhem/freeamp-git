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

// The debugger can't handle symbols more than 255 characters long.
// STL often creates symbols longer than that.
// When symbols are longer than 255 characters, the warning is disabled.
#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <windows.h>
#include <windowsx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

#include <set>
#include <string>
#include <functional>
#include <algorithm>
#include <sstream>

using namespace std;

#include "config.h"
#include "utility.h"
#include "resource.h"
#include "Win32MusicBrowser.h"
#include "DropSource.h"
#include "DropObject.h"
#include "registrar.h"
#include "Http.h"

char* kMyMusic = "My Music";
char* kAllTracks = "All Tracks";
char* kUncatagorized = "Uncategorized Tracks";
char* kPlaylists = "My Playlists";
char* kStreams = "Streams";
char* kFavoriteStreams = "My Favorite Streams";
char* kPortables = "My Portables";
char* kNewPlaylist = "Create a New Playlist...";
char* kNewPortable = "Setup a Portable Player...";
char* kCDAudio = "CD Audio";
char* kWiredPlanet = "Wired Planet";
char* kShoutCast = "ShoutCast";
char* kIceCast = "IceCast";

void MusicBrowserUI::InitTree()
{
    if(m_hMyMusicItem)
        TreeView_DeleteItem(m_hMusicView, m_hMyMusicItem);
    if(m_hPlaylistItem)
        TreeView_DeleteItem(m_hMusicView, m_hPlaylistItem);
    if(m_hStreamsItem)
        TreeView_DeleteItem(m_hMusicView, m_hStreamsItem);
    if(m_hPortableItem)
        TreeView_DeleteItem(m_hMusicView, m_hPortableItem);
    if(m_hCDItem)
        TreeView_DeleteItem(m_hMusicView, m_hCDItem);

    TV_INSERTSTRUCT insert;
        
    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 
        
    insert.item.pszText = kMyMusic;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 0;
    insert.item.iSelectedImage = 0;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;            
    insert.hInsertAfter = TVI_FIRST;
    insert.hParent = NULL;
    m_hMyMusicItem = TreeView_InsertItem(m_hMusicView, &insert);

    insert.item.pszText = kPlaylists;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 1;
    insert.item.iSelectedImage = 1;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = NULL;
    m_hPlaylistItem = TreeView_InsertItem(m_hMusicView, &insert);

    insert.item.pszText = kCDAudio;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 13;
    insert.item.iSelectedImage = 13;
    insert.item.cChildren= 0;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = NULL;
    m_hCDItem = TreeView_InsertItem(m_hMusicView, &insert);

    insert.item.pszText = kStreams;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 8;
    insert.item.iSelectedImage = 8;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = NULL;
    m_hStreamsItem = TreeView_InsertItem(m_hMusicView, &insert);

    insert.item.pszText = kWiredPlanet;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 9;
    insert.item.iSelectedImage = 9;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = m_hStreamsItem;
    m_hWiredPlanetItem = TreeView_InsertItem(m_hMusicView, &insert);

    insert.item.pszText = kIceCast;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 10;
    insert.item.iSelectedImage = 10;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = m_hStreamsItem;
    m_hIceCastItem = TreeView_InsertItem(m_hMusicView, &insert);
   
    /*insert.item.pszText = kShoutCast;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 11;
    insert.item.iSelectedImage = 11;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = m_hStreamsItem;
    m_hShoutCastItem = TreeView_InsertItem(m_hMusicView, &insert);*/

    insert.item.pszText = kPortables;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 7;
    insert.item.iSelectedImage = 7;
    insert.item.cChildren= 1;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_LAST;
    insert.hParent = NULL;
    m_hPortableItem = TreeView_InsertItem(m_hMusicView, &insert);
}

void MusicBrowserUI::FillArtists()
{
    TV_INSERTSTRUCT                     insert;
    const vector<ArtistList*>*          artistList;
    vector<ArtistList*>::const_iterator artist;
    TreeData                            data;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    data.m_iLevel = 1;
    artistList = m_context->catalog->GetMusicList();
    for(artist = artistList->begin(); 
        artist != artistList->end(); 
        artist++)
    {
       data.m_pArtist = (*artist);
       
       insert.item.pszText = (char *)(*artist)->name.c_str();
       insert.item.cchTextMax = (*artist)->name.length();
       insert.item.iImage = 2;
       insert.item.iSelectedImage = 2;
       insert.item.cChildren= 1;
       insert.item.lParam = (LPARAM) new TreeData(data);
       insert.hInsertAfter = TVI_LAST;
       insert.hParent = m_hMyMusicItem;
       TreeView_InsertItem(m_hMusicView, &insert);
    }    

    TreeView_SortChildren(m_hMusicView, m_hMyMusicItem, 0);

    insert.item.pszText = kUncatagorized;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 6;
    insert.item.iSelectedImage = 6;
    insert.item.cChildren = (m_context->catalog->GetUnsortedMusic()->size() ? 1 : 0);
    insert.item.lParam = NULL;       
    insert.hInsertAfter = TVI_FIRST;
    insert.hParent = m_hMyMusicItem;
    m_hUncatItem = TreeView_InsertItem(m_hMusicView, &insert);

    insert.item.pszText = kAllTracks;
    insert.item.cchTextMax = lstrlen(insert.item.pszText);
    insert.item.iImage = 5;
    insert.item.iSelectedImage = 5;
    insert.item.cChildren = (artistList->size() ? 1 : 0);
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_FIRST;
    insert.hParent = m_hMyMusicItem;
    m_hAllItem = TreeView_InsertItem(m_hMusicView, &insert);
}

void MusicBrowserUI::FillAlbums(TV_ITEM *pItem)
{
    TV_INSERTSTRUCT                insert;
    vector<AlbumList*>::iterator   album;
    TreeData                       data;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    data.m_iLevel = 2;
    data.m_pArtist = ((TreeData*)pItem->lParam)->m_pArtist;

    for(album = data.m_pArtist->m_albumList->begin(); 
        album != data.m_pArtist->m_albumList->end(); 
        album++)
    {
        data.m_pAlbum = (*album);
        
        if ((*album)->name == string(" ") || 
            (*album)->name.length() == 0)
            insert.item.pszText = "Unknown";
        else    
            insert.item.pszText = (char *)(*album)->name.c_str();
            
        insert.item.cchTextMax = (*album)->name.length();
        insert.item.iImage = 3;
        insert.item.iSelectedImage = 3;
        insert.item.cChildren= 1;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_LAST;
        insert.hParent = pItem->hItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }

    TreeView_SortChildren(m_hMusicView, pItem->hItem, 0);
}

void MusicBrowserUI::FillTracks(TV_ITEM *pItem)
{
    TV_INSERTSTRUCT                 insert;
    vector<PlaylistItem*>::iterator track;
    TreeData                        data;
    MetaData                        metadata;
    //bool                          needToSort = false;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    data.m_iLevel = 3;
    data.m_pArtist = ((TreeData*)pItem->lParam)->m_pArtist;
    data.m_pAlbum = ((TreeData*)pItem->lParam)->m_pAlbum;

    stable_sort(data.m_pAlbum->m_trackList->begin(), 
                data.m_pAlbum->m_trackList->end(), 
                TrackSort());

    for(track = data.m_pAlbum->m_trackList->begin(); 
        track != data.m_pAlbum->m_trackList->end(); 
        track++)
    {
        data.m_pTrack = (*track);

        metadata = (*track)->GetMetaData();

        if(metadata.Track() == 0)
            insert.hInsertAfter = TVI_SORT;
        else
            insert.hInsertAfter = TVI_LAST;

        if (metadata.Title() == string(" ") || 
            metadata.Title().length() == 0)
            insert.item.pszText = "Unknown";
        else    
            insert.item.pszText = (char *)(metadata.Title().c_str());
            
        insert.item.cchTextMax = metadata.Title().length();
        insert.item.iImage = 4;
        insert.item.iSelectedImage = 4;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        //insert.hInsertAfter = TVI_LAST;
        insert.hParent = pItem->hItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }

    //if(needToSort)
        //TreeView_SortChildren(m_hMusicView, pItem->hItem, 0);
}


void MusicBrowserUI::RefreshCDList(vector<PlaylistItem*>* trackList)
{
    TV_INSERTSTRUCT insert;
    TreeData        data;
    MetaData        metadata;
    HTREEITEM       treeItem;

    while(treeItem = TreeView_GetChild(m_hMusicView, m_hCDItem))
    {
        TreeView_DeleteItem(m_hMusicView, treeItem);
    }

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    data.m_iLevel = 3;

    vector<PlaylistItem*>::iterator track;

    uint32 trackNumber = 1;

    for(track = trackList->begin();
        track != trackList->end();
        track++)
    {
        ostringstream ost;
        string trackString;

        data.m_pArtist = NULL;
        data.m_pAlbum = NULL;
        data.m_pTrack = (*track);

        metadata = (*track)->GetMetaData();        

        if(metadata.Title().size())
            insert.item.pszText = (char*)(metadata.Title().c_str());
        else
        {
            ost << "CD Audio Track " << trackNumber;

            trackString = ost.str();

            insert.item.pszText = (char*)(trackString.c_str());
        }        
            
        insert.item.cchTextMax = strlen(insert.item.pszText);
        insert.item.iImage = 4;
        insert.item.iSelectedImage = 4;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_LAST;
        insert.hParent = m_hCDItem;
        TreeView_InsertItem(m_hMusicView, &insert);

        trackNumber++;
    }

    string CD;

    if(trackList->size())
    {
        CD = (metadata.Album().size() ? metadata.Album() : "Unknown Album");
        CD += " by ";
        CD += (metadata.Artist().size() ? metadata.Artist() : "Unknown Artist");
    }
    else
    {
        CD = kCDAudio;
    }

    TV_ITEM tv_item;

    tv_item.mask = TVIF_CHILDREN|TVIF_TEXT;
    tv_item.hItem = m_hCDItem;
    tv_item.cchTextMax = strlen(insert.item.pszText);
    tv_item.pszText = (char*)(CD.c_str());
    tv_item.cChildren = (trackList->size() ? 1 : 0);

    TreeView_SetItem(m_hMusicView, &tv_item);
}

void
MusicBrowserUI::
fill_all_tracks(void* arg)
{
    MusicBrowserUI* ui = (MusicBrowserUI*) arg;

	ui->FillAllTracks();
}

void MusicBrowserUI::FillAllTracks()
{
    TV_INSERTSTRUCT                     insert;
    const vector<ArtistList*>*          artistList;
    vector<ArtistList*>::const_iterator artist;        
    TreeData                            data;
    MetaData                            metadata;
	bool								gotOne = false;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    data.m_iLevel = 3;
    artistList = m_context->catalog->GetMusicList();

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
                data.m_pArtist = (*artist);
                data.m_pAlbum = (*album);
                data.m_pTrack = (*track);
            
                metadata = (*track)->GetMetaData();
            
                if(metadata.Title() == string(" ") || 
                   metadata.Title().length() == 0)
                   insert.item.pszText = "Unknown";
                else    
                    insert.item.pszText = (char *)(metadata.Title().c_str());
                    
                insert.item.cchTextMax = metadata.Title().length();
                insert.item.iImage = 4;
                insert.item.iSelectedImage = 4;
                insert.item.cChildren= 0;
                insert.item.lParam = (LPARAM) new TreeData(data);
                insert.hInsertAfter = TVI_LAST;
                insert.hParent = m_hAllItem;
                TreeView_InsertItem(m_hMusicView, &insert);

				if(!gotOne)
				{
					gotOne = true;
					TreeView_Expand(m_hMusicView, m_hAllItem, TVE_EXPAND);
				}

            }
        }
    }       
    
    data.m_iLevel = 3;
    data.m_pArtist = NULL;
    data.m_pAlbum = NULL;

    const vector<PlaylistItem*>* trackList;
    vector<PlaylistItem*>::const_iterator track;
    
    trackList = m_context->catalog->GetUnsortedMusic();

    for(track = trackList->begin(); 
        track != trackList->end(); 
        track++)
    {
        data.m_pTrack = (*track);

        metadata = (*track)->GetMetaData();

        if (metadata.Title() == string(" ") || 
            metadata.Title().length() == 0)
            insert.item.pszText = "Unknown";
        else    
            insert.item.pszText = (char *)(metadata.Title().c_str());
            
        insert.item.cchTextMax = metadata.Title().length();
        insert.item.iImage = 4;
        insert.item.iSelectedImage = 4;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_SORT;
        insert.hParent = m_hAllItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }

    //TreeView_SortChildren(m_hMusicView, m_hAllItem, 0);
    delete m_fillAllThread;
}

void MusicBrowserUI::FillUncatTracks()
{
    TV_INSERTSTRUCT                         insert;
    const vector<PlaylistItem*>*            trackList;
    vector<PlaylistItem *>::const_iterator  track;
    TreeData                                data;
    MetaData                                metadata;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                 TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    data.m_iLevel = 3;
    data.m_pArtist = NULL;
    data.m_pAlbum = NULL;
    
    trackList = m_context->catalog->GetUnsortedMusic();

    for(track = trackList->begin(); 
        track != trackList->end(); 
        track++)
    {
        data.m_pTrack = (*track);
        metadata = (*track)->GetMetaData();

        if (metadata.Title() == string(" ") || 
            metadata.Title().length() == 0)
            insert.item.pszText = "Unknown";
        else    
            insert.item.pszText = (char *)(metadata.Title().c_str());
            
        insert.item.cchTextMax = metadata.Title().length();
        insert.item.iImage = 4;
        insert.item.iSelectedImage = 4;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_LAST;
        insert.hParent = m_hUncatItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }

    TreeView_SortChildren(m_hMusicView, m_hUncatItem, 0);
}

void MusicBrowserUI::FillPlaylists()
{
    TV_INSERTSTRUCT                 insert;
    vector<string>::const_iterator  url;
    const vector<string>*           playlists;
    char                            szBase[MAX_PATH];
    TreeData                        data;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    playlists = m_context->catalog->GetPlaylists(); 

    data.m_iLevel = 1;

    for(url = playlists->begin(); 
        url != playlists->end(); 
        url++)
    {
        _splitpath((char *)(*url).c_str(), NULL, NULL, szBase, NULL);   
        data.m_oPlaylistName = string(szBase);
        data.m_oPlaylistPath = *url;

        insert.item.pszText = szBase;
        insert.item.cchTextMax = strlen(szBase);
        insert.item.iImage = 1;
        insert.item.iSelectedImage = 1;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_SORT;
        insert.hParent = m_hPlaylistItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }
    
    insert.item.pszText = kNewPlaylist;
    insert.item.cchTextMax = strlen(kNewPlaylist);
    insert.item.iImage = 1;
    insert.item.iSelectedImage = 1;
    insert.item.cChildren= 0;
    insert.item.lParam = NULL;
    insert.hInsertAfter = TVI_FIRST;
    insert.hParent = m_hPlaylistItem;
    m_hNewPlaylistItem = TreeView_InsertItem(m_hMusicView, &insert);
}

void MusicBrowserUI::FillWiredPlanet()
{
    TV_INSERTSTRUCT insert;
    TreeData        data;

    char* stations[] = {
        "Alpha 2.0", // (Downtempo / Trip-Hop)",
		"Amplified", // (Punk, Garage, and Hard Alternative),
        "Axis", // (Jazz in its many forms)",
		"Butter", // (Melted grooves and dubbed out world-flavored Funk.),
		"DJ Mix",
		"Flipside",
		"Hearts of Space",
        "Nova Mundi", // (New Age / Ambient)",
        "Oscillations", // (Electronic / Trance / House)",
		"Pop 3",
        "Red Shift", // (Drum \'n Bass / Jungle)",
		"The Cut",
		"The Fringe",
		"World",
		"Radio EMusic",
		"Radio Epitaph",
		"Radio Radio",
        "Radio TMBG", // (They Might Be Giants)",
		"Noise Pop"
		
    };

    char* urls[] = {
        "http://ice1.wiredplanet.com:8001/alph",
		"http://ice1.wiredplanet.com:8001/ampp",
        "http://ice1.wiredplanet.com:8001/axis",
		"http://ice1.wiredplanet.com:8001/cjwp",
		"http://ice1.wiredplanet.com:8001/oaky",
		"http://ice1.wiredplanet.com:8001/flip",
		"http://ice1.wiredplanet.com:8001/hrts",
        "http://ice1.wiredplanet.com:8001/nova",
        "http://ice1.wiredplanet.com:8001/oscl",
		"http://ice1.wiredplanet.com:8001/popo",
        "http://ice1.wiredplanet.com:8001/reds",
		"http://ice1.wiredplanet.com:8001/tcut",
		"http://ice1.wiredplanet.com:8001/frng",
		"http://ice1.wiredplanet.com:8001/wrld",
		"http://ice1.wiredplanet.com:8001/emus",
		"http://ice1.wiredplanet.com:8001/punk",
		"http://ice1.wiredplanet.com:8001/elvi",
        "http://ice1.wiredplanet.com:8001/tmbg",
		"http://ice1.wiredplanet.com:8001/nois"
    };

    uint32 numStations = (sizeof(stations)/sizeof(char*));

    data.m_iLevel = 1;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

    for(uint32 i = 0; i < numStations; i++)
    {
        PlaylistItem* stream = new PlaylistItem;
        MetaData metadata;

        stream->SetURL(urls[i]);
        metadata.SetTitle(stations[i]);
        metadata.SetArtist("Wired Planet");
        stream->SetMetaData(&metadata);

        data.m_pStream = stream;

        insert.item.pszText = stations[i];
        insert.item.cchTextMax = strlen(insert.item.pszText);
        insert.item.iImage = 8;
        insert.item.iSelectedImage = 8;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_SORT;
        insert.hParent = m_hWiredPlanetItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }
}

void MusicBrowserUI::FillIceCast(vector<IcecastStreamInfo> &list)
{
    //FooCast* fooCast = new FooCast(m_hMusicView, m_hIceCastItem);

    HTREEITEM treeItem;

    while(treeItem = TreeView_GetChild(m_hMusicView, m_hIceCastItem))
    {
        TreeView_DeleteItem(m_hMusicView, treeItem);
    }

    TV_INSERTSTRUCT insert;
    TreeData        data;

    data.m_iLevel = 1;

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                        TVIF_SELECTEDIMAGE | TVIF_PARAM;


    vector<IcecastStreamInfo>::iterator i = list.begin();

    for(; i != list.end(); i++)
    {
        PlaylistItem* stream = new PlaylistItem;
        MetaData metadata;

        stream->SetURL(i->m_streamUrl.c_str());
        metadata.SetTitle(i->m_name.c_str());
        metadata.SetArtist(i->m_webUrl.c_str());
        metadata.SetComment(i->m_desc.c_str());
        stream->SetMetaData(&metadata);

        data.m_pStream = stream;

        insert.item.pszText = (char*)i->m_webUrl.c_str();
        insert.item.cchTextMax = strlen(insert.item.pszText);
        insert.item.iImage = 8;
        insert.item.iSelectedImage = 8;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_SORT;
        insert.hParent = m_hIceCastItem;
        TreeView_InsertItem(m_hMusicView, &insert);
    }

    if(m_fillIceCastThread)
    {
        delete m_fillIceCastThread;
        m_fillIceCastThread = NULL;
    }
}

void MusicBrowserUI::FillPortables()
{
    TV_INSERTSTRUCT         insert;
    char*                   buffer = NULL;  
    uint32                  size = 0;
    set<string>             portablePlayers;
    TreeData                data;
    uint32                  count = 0;
    HTREEITEM               item;

    // remove old ones if they exist
    while(item = TreeView_GetChild(m_hMusicView, m_hPortableItem))
    {
        TreeView_DeleteItem(m_hMusicView, item);
    }

    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
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

    while(IsntError(m_plm->GetSupportedPortables(&device, i++)))
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
                insert.item.pszText = (char*)displayString.c_str();
            }
            else
            {
               insert.item.pszText = (char*)device.GetPluginName();
            }

            data.m_pPortable = new DeviceInfo(device);
                        
            insert.item.cchTextMax = strlen(insert.item.pszText);
            insert.item.iImage = 7;
            insert.item.iSelectedImage = 7;
            insert.item.cChildren= 0;
            insert.item.lParam = (LPARAM) new TreeData(data);
            insert.hInsertAfter = TVI_SORT;
            insert.hParent = m_hPortableItem;
            TreeView_InsertItem(m_hMusicView, &insert);
            count++;
        }
    }

    if(!count)
    {
        insert.item.pszText = kNewPortable;
        insert.item.cchTextMax = strlen(insert.item.pszText);
        insert.item.iImage = 7;
        insert.item.iSelectedImage = 7;
        insert.item.cChildren= 0;
        insert.item.lParam = NULL;
        insert.hInsertAfter = TVI_FIRST;
        insert.hParent = m_hPortableItem;
        m_hNewPortableItem = TreeView_InsertItem(m_hMusicView, &insert);
    }
}

void MusicBrowserUI::icecast_timer(void *arg)
{
    MusicBrowserUI *ui = (MusicBrowserUI*)arg;
    ui->IceCastTimer();
}

void MusicBrowserUI::IceCastTimer()
{
    Error error;
    Http icecastDownload(m_context);
    string page;
    vector<IcecastStreamInfo> list;
    IcecastStreams icecastStream;
    const string icecastURL = "http://yp.icecast.org/yplist_long.xml";

    error = icecastDownload.DownloadToString(icecastURL, page);

    if(IsntError(error))
    {
        icecastStream.ParseStreamXML(page, list);

        if(list.size() > 0)
        {
            FillIceCast(list);
        }
    }
}

int32 MusicBrowserUI::GetCurrentItemFromMousePos()
{
    TV_ITEM tv_item;
    TV_HITTESTINFO tv_htinfo;

    GetCursorPos(&tv_htinfo.pt);
    ScreenToClient(m_hMusicView, &tv_htinfo.pt);
    if(TreeView_HitTest(m_hMusicView, &tv_htinfo))
    {
        tv_item.hItem = TreeView_GetSelection(m_hMusicView); 
        tv_item.mask = TVIF_PARAM | TVIF_HANDLE;
        TreeView_GetItem(m_hMusicView, &tv_item);
        
        return tv_item.lParam;
    }
    return -1;
}

HTREEITEM MusicBrowserUI::FindArtist(const ArtistList* artist)
{
    HTREEITEM result = NULL;
    HWND hwnd = m_hMusicView;

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
            TreeData* treedata = (TreeData*)tv_item.lParam;

            if(treedata && artist == treedata->m_pArtist)
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
    HWND hwnd = m_hMusicView;

    TV_ITEM tv_item;

    tv_item.hItem = NULL;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first artist
    tv_item.hItem = TreeView_GetChild(m_hMusicView, artistItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            TreeData* treedata = (TreeData*)tv_item.lParam;

            if(treedata && album == treedata->m_pAlbum)
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
    HWND hwnd = m_hMusicView;

    TV_ITEM tv_item;

    tv_item.hItem = NULL;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first artist
    tv_item.hItem = TreeView_GetChild(m_hMusicView, albumItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            TreeData* treedata = (TreeData*)tv_item.lParam;

            if(treedata && track == treedata->m_pTrack)
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
    HWND hwnd = m_hMusicView;

    TV_ITEM tv_item;

    tv_item.hItem = NULL;
    tv_item.mask = TVIF_PARAM;

    // this should retrieve the first playlist
    tv_item.hItem = TreeView_GetChild(m_hMusicView, m_hPlaylistItem);

    // but we want the second one... not the magic "Create New Playlist..."
    tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, tv_item.hItem);

    BOOL success;

    do
    {
        success = TreeView_GetItem(hwnd, &tv_item);

        if(success)
        {
            TreeData* treedata = (TreeData*)tv_item.lParam;

            if(treedata && playlist == treedata->m_oPlaylistPath)
            {
                result = tv_item.hItem;
                break;
            }
        }

    }while(success && (tv_item.hItem = TreeView_GetNextSibling(hwnd, tv_item.hItem)));
    
    return result;
}

void MusicBrowserUI::MusicCatalogCleared()
{
    if(m_initialized)
        InitTree();
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

            TreeView_GetItem(m_hMusicView, &tv_item);

            artistState = tv_item.state;

            albumItem = FindAlbum(artistItem, oldAlbum);

            if(albumItem)
            {
                tv_item.hItem = albumItem;
                tv_item.mask = TVIF_STATE;
                tv_item.stateMask = TVIS_SELECTED|TVIS_EXPANDED;
                tv_item.state = 0;

                TreeView_GetItem(m_hMusicView, &tv_item);

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
                TreeView_Expand(m_hMusicView, artistItem, TVE_EXPAND);

            albumItem = FindAlbum(artistItem, newAlbum);

            if(albumItem)
            {
                if(albumState & TVIS_EXPANDED)
                    TreeView_Expand(m_hMusicView, albumItem, TVE_EXPAND);

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
        TreeView_DeleteItem(m_hMusicView, playlistItem);
    }
}

void MusicBrowserUI::MusicCatalogPlaylistAdded(string item)
{
    // put it under playlists
    if(TreeView_GetChild(m_hMusicView, m_hPlaylistItem))
    {
        TV_INSERTSTRUCT insert;
        TreeData        data;
        MetaData        metadata;
        char            szBase[MAX_PATH];

        _splitpath((char *)item.c_str(), NULL, NULL, szBase, NULL);  

        insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                            TVIF_SELECTEDIMAGE | TVIF_PARAM; 

        data.m_iLevel = 1;

        data.m_oPlaylistName = string(szBase);
        data.m_oPlaylistPath = item;

        insert.item.pszText = szBase;
        insert.item.cchTextMax = strlen(szBase);
        insert.item.iImage = 1;
        insert.item.iSelectedImage = 1;
        insert.item.cChildren= 0;
        insert.item.lParam = (LPARAM) new TreeData(data);
        insert.hInsertAfter = TVI_SORT;
        insert.hParent = m_hPlaylistItem;
        TreeView_InsertItem(m_hMusicView, &insert);

        TreeView_DeleteItem(m_hMusicView, m_hNewPlaylistItem);
        insert.item.pszText = kNewPlaylist;
        insert.item.cchTextMax = strlen(kNewPlaylist);
        insert.item.iImage = 1;
        insert.item.iSelectedImage = 1;
        insert.item.cChildren= 0;
        insert.item.lParam = NULL;
        insert.hInsertAfter = TVI_FIRST;
        insert.hParent = m_hPlaylistItem;
        m_hNewPlaylistItem = TreeView_InsertItem(m_hMusicView, &insert);
    }
}

void MusicBrowserUI::MusicCatalogTrackRemoved(const ArtistList* artist,
                                              const AlbumList* album,
                                              const PlaylistItem* item)
{
    HTREEITEM artistItem = NULL;
    HTREEITEM albumItem = NULL;
    HTREEITEM trackItem = NULL;
    bool uncatagorized = false;
    

    // is this in the uncatagorized section?
    if(!artist) 
    {
        trackItem = FindTrack(m_hUncatItem, item);
        uncatagorized = true;
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
        TreeView_DeleteItem(m_hMusicView, trackItem);

        if(uncatagorized)
        {
            if(!TreeView_GetChild(m_hMusicView, m_hUncatItem))
            {
                TV_ITEM tv_item;

                tv_item.hItem = m_hUncatItem;
                tv_item.mask = TVIF_CHILDREN;
                tv_item.cChildren = 0;

                TreeView_SetItem(m_hMusicView, &tv_item);
            }
        }
    }

    if(albumItem && !album->m_trackList->size())
    {
        TreeView_DeleteItem(m_hMusicView, albumItem);
    }

    if(artistItem && !artist->m_albumList->size())
    {
        TreeView_DeleteItem(m_hMusicView, artistItem);
    }

    trackItem = FindTrack(m_hAllItem, item);

    if(trackItem)
    {
        TreeView_DeleteItem(m_hMusicView, trackItem);

        if(!TreeView_GetChild(m_hMusicView, m_hUncatItem))
        {
            TV_ITEM tv_item;

            tv_item.hItem = m_hAllItem;
            tv_item.mask = TVIF_CHILDREN;
            tv_item.cChildren = 0;

            TreeView_SetItem(m_hMusicView, &tv_item);
        }
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
        if(m_hUncatItem)
        {
            // nope, stick in uncatagorized
            TV_INSERTSTRUCT insert;
            TreeData        data;
            MetaData        metadata;
            TV_ITEM         tv_item;

            tv_item.hItem = m_hUncatItem;
            tv_item.mask = TVIF_CHILDREN;
            tv_item.cChildren = 1;

            TreeView_SetItem(m_hMusicView, &tv_item);

            insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                                TVIF_SELECTEDIMAGE | TVIF_PARAM; 

            data.m_iLevel = 3;
            data.m_pArtist = NULL;
            data.m_pAlbum = NULL;
    
        
            data.m_pTrack = (PlaylistItem*)item;
            metadata = item->GetMetaData();

            if (metadata.Title() == string(" ") || 
                metadata.Title().length() == 0)
                insert.item.pszText = "Unknown";
            else    
                insert.item.pszText = (char *)(metadata.Title().c_str());
        
            insert.item.cchTextMax = metadata.Title().length();
            insert.item.iImage = 4;
            insert.item.iSelectedImage = 4;
            insert.item.cChildren= 0;
            insert.item.lParam = (LPARAM) new TreeData(data);
            insert.hInsertAfter = TVI_SORT;
            insert.hParent = m_hUncatItem;
            newItem = TreeView_InsertItem(m_hMusicView, &insert);
        }
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
                if(TreeView_GetChild(m_hMusicView, albumItem))
                {
                    TV_INSERTSTRUCT insert;
                    TreeData        data;
                    MetaData        metadata;

                    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

                    data.m_iLevel = 3;
                    data.m_pArtist = (ArtistList*)artist;
                    data.m_pAlbum = (AlbumList*)album;
    
                    data.m_pTrack = (PlaylistItem*)item;
                    metadata = item->GetMetaData();

                    if (metadata.Title() == string(" ") || 
                        metadata.Title().length() == 0)
                        insert.item.pszText = "Unknown";
                    else    
                        insert.item.pszText = (char *)(metadata.Title().c_str());
    
                    insert.item.cchTextMax = metadata.Title().length();
                    insert.item.iImage = 4;
                    insert.item.iSelectedImage = 4;
                    insert.item.cChildren= 0;
                    insert.item.lParam = (LPARAM) new TreeData(data);
                    insert.hInsertAfter = TVI_LAST;
                    insert.hParent = albumItem;

                    TV_ITEM tv_item;
                    HTREEITEM sibling = NULL;

                    tv_item.mask = TVIF_PARAM;

                    if(tv_item.hItem = TreeView_GetChild(m_hMusicView, albumItem))
                    {
                        BOOL success;

                        do
                        {
                            success = TreeView_GetItem(m_hMusicView, &tv_item);

                            if(success)
                            {
                                TreeData* treedata = (TreeData*)tv_item.lParam;

                                if(treedata)
                                {
                                    PlaylistItem* track = treedata->m_pTrack;
                                    MetaData metadata = track->GetMetaData();
                                
                                    if(metadata.Track())
                                    {
                                        if(metadata.Track() > metadata.Track())
                                        {
                                            if(sibling)
                                                insert.hInsertAfter = sibling;
                                            else
                                                insert.hInsertAfter = TVI_FIRST;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        if(metadata.Track() || metadata.Title() >  metadata.Title())
                                        {
                                            if(sibling)
                                                insert.hInsertAfter = sibling;
                                            else
                                                insert.hInsertAfter = TVI_FIRST;
                                            break;
                                        }
                                    }
                                }
                            }

                            sibling = tv_item.hItem;
        
                        }while(success && 
                               (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
                                                                        tv_item.hItem)));
                    }

                    newItem = TreeView_InsertItem(m_hMusicView, &insert);
                }
            }
            else // might need to add the album
            {
                if(TreeView_GetChild(m_hMusicView, artistItem))
                {
                    TV_INSERTSTRUCT insert;
                    TreeData        data; 

                    insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                                        TVIF_SELECTEDIMAGE | TVIF_PARAM; 

                    data.m_iLevel = 2;
                    data.m_pArtist = (ArtistList*)artist;
                    data.m_pAlbum = (AlbumList*)album;

                    if (album->name == string(" ") || 
                        album->name.length() == 0)
                        insert.item.pszText = "Unknown";
                    else    
                        insert.item.pszText = (char *)album->name.c_str();
    
                    insert.item.cchTextMax = album->name.length();
                    insert.item.iImage = 3;
                    insert.item.iSelectedImage = 3;
                    insert.item.cChildren= 1;
                    insert.item.lParam = (LPARAM) new TreeData(data);
                    insert.hInsertAfter = TVI_SORT;
                    insert.hParent = artistItem;
                    newItem = TreeView_InsertItem(m_hMusicView, &insert);
                }
            }
        }
        else // might need to add the artist
        {
            HTREEITEM root = TreeView_GetRoot(m_hMusicView);

            TV_ITEM tv_item;

            tv_item.hItem = root;
            tv_item.mask = TVIF_STATE;
            tv_item.stateMask = TVIS_EXPANDEDONCE;
            tv_item.state = 0;

            TreeView_GetItem(m_hMusicView, &tv_item);

            if(tv_item.state & TVIS_EXPANDEDONCE)
            {
                TV_INSERTSTRUCT insert;
                TreeData        data;

                insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                             TVIF_SELECTEDIMAGE | TVIF_PARAM; 

                data.m_iLevel = 1;
                data.m_pArtist = (ArtistList*)artist;

                insert.item.pszText = (char*)artist->name.c_str();
                insert.item.cchTextMax = artist->name.length();
                insert.item.iImage = 2;
                insert.item.iSelectedImage = 2;
                insert.item.cChildren= 1;
                insert.item.lParam = (LPARAM) new TreeData(data);
                insert.hInsertAfter = TVI_LAST;
                insert.hParent = m_hMyMusicItem;

                TV_ITEM tv_item;
                HTREEITEM sibling = m_hUncatItem;

                tv_item.mask = TVIF_PARAM;

                if(tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, m_hUncatItem))
                {
                    BOOL success;

                    do
                    {
                        success = TreeView_GetItem(m_hMusicView, &tv_item);

                        if(success)
                        {
                            TreeData* treedata = (TreeData*)tv_item.lParam;

                            if(treedata)
                            {
                                ArtistList* artist2 = treedata->m_pArtist;
                            
                                if(lstrcmp(artist2->name.c_str(), artist->name.c_str()) > 0)
                                {
                                    insert.hInsertAfter = sibling;
                                    break;
                                }
                            }
                        }

                        sibling = tv_item.hItem;
    
                    }while(success && 
                           (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
                                                                    tv_item.hItem)));
                }

                newItem = TreeView_InsertItem(m_hMusicView, &insert);
            }       
        }
    }

    //TreeView_EnsureVisible(m_hMusicView, newItem);

    // put it under "All Tracks"
    if(TreeView_GetChild(m_hMusicView, m_hAllItem))
    {
        if(m_hAllItem)
        {
            TV_INSERTSTRUCT insert;
            TreeData        data;
            MetaData        metadata;
            TV_ITEM         tv_item;

            tv_item.hItem = m_hAllItem;
            tv_item.mask = TVIF_CHILDREN;
            tv_item.cChildren = 1;

            TreeView_SetItem(m_hMusicView, &tv_item);

            insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_CHILDREN |
                                TVIF_SELECTEDIMAGE | TVIF_PARAM;

            data.m_iLevel = 3;
            data.m_pArtist = (ArtistList*)artist;
            data.m_pAlbum = (AlbumList*)album;

            data.m_pTrack = (PlaylistItem*)item;
            metadata = item->GetMetaData();

            if (metadata.Title() == string(" ") || 
                metadata.Title().length() == 0)
                insert.item.pszText = "Unknown";
            else    
                insert.item.pszText = (char *)(metadata.Title().c_str());

            insert.item.cchTextMax = metadata.Title().length();
            insert.item.iImage = 4;
            insert.item.iSelectedImage = 4;
            insert.item.cChildren= 0;
            insert.item.lParam = (LPARAM) new TreeData(data);
            insert.hInsertAfter = TVI_SORT;
            insert.hParent = m_hAllItem;
            TreeView_InsertItem(m_hMusicView, &insert);
        }
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
        //GetSelectedStreamItems(urls);

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

    TreeData* treedata = (TreeData*)tv_item->lParam;

    if(treedata)
    {
        if(treedata->IsArtist())
        {
            ArtistList* artist = treedata->m_pArtist;
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
        else if(treedata->IsAlbum())
        {
            AlbumList* album = treedata->m_pAlbum;
            vector<PlaylistItem *>::iterator track;

            for(track = album->m_trackList->begin();
                track != album->m_trackList->end();
                track++)
            {
                items->push_back(*track);
            }
        }
        else if(treedata->IsTrack() || treedata->IsUncatagorized())
        {
            PlaylistItem* track = treedata->m_pTrack;

            items->push_back(track);
        }
    }
}

BOOL MusicBrowserUI::FindSelectedItems(HTREEITEM root, vector<PlaylistItem*>* items)
{
    BOOL result = FALSE;
    HWND hwnd = m_hMusicView;
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

void MusicBrowserUI::GetSelectedStreamItems(vector<string>* urls)
{
    TV_ITEM tv_root;

    tv_root.hItem = m_hWiredPlanetItem;
    tv_root.mask = TVIF_STATE;
    tv_root.stateMask = TVIS_SELECTED;
    tv_root.state = 0;

    TreeView_GetItem(m_hMusicView, &tv_root);

    // if selected then we add all the wired planet streams
    if(tv_root.state & TVIS_SELECTED)
    {
        TV_ITEM tv_item;

        // get the first playlist item
        tv_item.hItem = TreeView_GetChild(m_hMusicView, m_hWiredPlanetItem);
        tv_item.mask = TVIF_STATE|TVIF_PARAM;
        tv_item.stateMask = TVIS_SELECTED;
        tv_item.state = 0;

        if(tv_item.hItem)
        {
            BOOL result = FALSE;

            do
            {
                result = TreeView_GetItem(m_hMusicView, &tv_item);

                if(result)
                {
                    TreeData* treedata = (TreeData*)tv_item.lParam;

                    if(treedata)
                    {
                        PlaylistItem* stream = treedata->m_pStream;

                        urls->push_back(stream->URL());
                    }
                }
        
            }while(result && 
                   (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
                                                            tv_item.hItem)));
        }    
    }
}

void MusicBrowserUI::GetSelectedPlaylistItems(vector<string>* urls)
{
    TV_ITEM tv_root;

    tv_root.hItem = m_hPlaylistItem;
    tv_root.mask = TVIF_STATE;
    tv_root.stateMask = TVIS_SELECTED;
    tv_root.state = 0;

    TreeView_GetItem(m_hMusicView, &tv_root);

    // if selected then we add all the playlists
    if(tv_root.state & TVIS_SELECTED)
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

                if(result)
                {
                    TreeData* treedata = (TreeData*)tv_item.lParam;

                    if(treedata)
                    {
                        string playlist = treedata->m_oPlaylistPath;

                        urls->push_back(playlist);
                    }
                }
        
            }while(result && 
                   (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
                                                            tv_item.hItem)));
        }    
       
    }
    else // iterate the playlists and add selected ones
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
                    TreeData* treedata = (TreeData*)tv_item.lParam;

                    if(treedata)
                    {
                        string playlist = treedata->m_oPlaylistPath;

                        urls->push_back(playlist);
                    }
                }
        
            }while(result && 
                   (tv_item.hItem = TreeView_GetNextSibling(m_hMusicView, 
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
    HTREEITEM rootItem = TreeView_GetRoot(m_hMusicView);

    if(rootItem)
    {
        // is the "Music Catalog" item selected?
        TV_ITEM tv_root;

        tv_root.hItem = rootItem;
        tv_root.mask = TVIF_STATE;
        tv_root.stateMask = TVIS_SELECTED;
        tv_root.state = 0;

        TreeView_GetItem(m_hMusicView, &tv_root);

        // if so then we add all the items under "All Tracks"
        if(tv_root.state & TVIS_SELECTED)
        {
            AddAllTrackItems(items);
        }
        else
        {
            TV_ITEM tv_all;

            // is the "All Tracks" item selected
            tv_all.hItem = TreeView_GetChild(m_hMusicView, rootItem);
            tv_all.mask = TVIF_STATE;
            tv_all.stateMask = TVIS_SELECTED;
            tv_all.state = 0;

            TreeView_GetItem(m_hMusicView, &tv_all);

            // if so then we add all the items under "All Tracks"
            if(tv_all.state & TVIS_SELECTED)
            {
                AddAllTrackItems(items);
            }
            else // if not we iterate the catalog for selected items
            {
                HTREEITEM firstSearchItem = TreeView_GetChild(m_hMusicView, tv_all.hItem);

                if(firstSearchItem)
                    FindSelectedItems(firstSearchItem, items);

                TV_ITEM tv_uncat;
                
                // is the "Uncatagorized" item selected
                tv_uncat.hItem = TreeView_GetNextSibling(m_hMusicView, tv_all.hItem);
                tv_uncat.mask = TVIF_STATE;
                tv_uncat.stateMask = TVIS_SELECTED;
                tv_uncat.state = 0;

                TreeView_GetItem(m_hMusicView, &tv_uncat);

                firstSearchItem = tv_uncat.hItem;

                // if so then we add all the items under "Uncatagorized"
                if(tv_uncat.state & TVIS_SELECTED)
                {
                    AddUncatagorizedTrackItems(items);

                    // wanna skip the uncated tracks if we just added them all
                    firstSearchItem = TreeView_GetNextSibling(m_hMusicView, tv_uncat.hItem);
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
    HWND hwnd = m_hMusicView;
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
    HWND hwnd = m_hMusicView;
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
    HTREEITEM rootItem = TreeView_GetChild(m_hMusicView, m_hMyMusicItem);

    if(rootItem)
    {
        result = CountSelectedItems(rootItem);
    }

    return result;
}

uint32 MusicBrowserUI::GetSelectedPlaylistCount()
{
    uint32 result = 0;
    HTREEITEM rootItem = TreeView_GetChild(m_hMusicView, m_hPlaylistItem);

    if(rootItem)
    {
        result = CountSelectedItems(rootItem);
    }

    return result;
}

uint32 MusicBrowserUI::GetSelectedStreamCount()
{
    uint32 result = 0;
    HTREEITEM rootItem = TreeView_GetChild(m_hMusicView, m_hWiredPlanetItem);

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
