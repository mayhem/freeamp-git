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

#ifndef INCLUDED_WIN32MUSICBROWSER_H_
#define INCLUDED_WIN32MUSICBROWSER_H_

#include <windows.h>
#include <commctrl.h>

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "config.h"
#include "ui.h"
#include "event.h"
#include "thread.h"
#include "playlist.h"
#include "musicbrowser.h"

class FAContext;

#define STATE_COLLAPSED 0
#define STATE_EXPANDED  1

struct TreeCrossRef
{
    int32         iLevel;
    ArtistList   *pArtist;
    AlbumList    *pAlbum;
    PlaylistItem *pTrack;
    
    TreeCrossRef(void)
    {
       iLevel = -1;
       pArtist = NULL;
       pAlbum = NULL;
       pTrack = NULL;
    };
};


bool operator<(const TreeCrossRef &A, const TreeCrossRef &b);
bool operator==(const TreeCrossRef &A, const TreeCrossRef &b);

class MusicBrowserUI : public UserInterface 
{
 public:

             MusicBrowserUI(FAContext *);
    virtual ~MusicBrowserUI();

    virtual int32 AcceptEvent(Event *);
    virtual Error Init(int32);
						 
    static void UIThreadFunc(void* arg);

    void  InitDialog(HWND hwnd);
    void  ShowBrowser(bool bShowExpanded);
    void  HideBrowser(void);
	void  ExpandCollapseEvent(void);
    void  GetMinMaxInfo(MINMAXINFO *pInfo);
    void  SetMinMaxInfo(void);
    void  SizeWindow(int iWidth, int iHeight);
    int   Notify(WPARAM wParam, NMHDR *pNMHDR);
    void  StartMusicSearch(void);
    void  MoveUpEvent(void);
    void  MoveDownEvent(void);
    void  AddEvent(void);
    void  DeleteEvent(void);
    void  DeleteListEvent(void);
    void  PlaylistComboChanged(void);
    void  OpenPlaylist(void);
    void  NewPlaylist(void);
    void  WritePlaylist(void);
    void  SortEvent(int id);
    void  MouseMove(uint32 uFlags, POINT &sPoint);
    void  MouseButtonUp(void);
    void  EmptyDBCheck(void);
 
 protected:
    FAContext *m_context;

 private:

    void SetStatusText(const char *text);
    void MoveItemEvent(int source, int dest);
    void AddTrackPlaylistEvent(char *path);
    void AddTrackPlaylistEvent(PlaylistItem *newitem);
    void AddTracksPlaylistEvent(vector<PlaylistItem *> *newlist);
    void PlayEvent(void);
    void SortPlaylistEvent(PlaylistSortKey order, PlaylistSortType type);
    void PopUpInfoEditor(void);
    void SaveCurrentPlaylist(char *path);
    void UpdateCatalog(void);
    void UpdatePlaylistList(void);
    void ImportPlaylist(char *path);
    void ReadPlaylist(char *filename, vector<PlaylistItem *> *plist);

    void ToggleVisEvent(void);

    void InitList(void);
    void LoadPlaylist(string &oPlaylist);
    void ActivePlaylistCleared(void);
    void SetActivePlaylist(void);
    void SetActivePlaylistIcon(int iImage);

    void InitTree(void);
    void FillArtists(void);
    void FillAlbums(TV_ITEM *pItem);
    void FillPlaylists(void);
    void FillTracks(TV_ITEM *pItem);
//    void FillPlaylistCombo(void);

    void  UpdateButtonStates(void);
    void  MoveControls(int iPixelsToMove);
    Error CreateMainDialog(void);
    Error CloseMainDialog(void);

    void  BeginDrag(NM_TREEVIEW *pTreeView);


    EventQueue          *m_playerEQ;
    int32                m_state, m_startupType;
    int32                m_currentindex, m_currentplaying;
  	HWND                 m_hWnd, m_hStatus;		
    PlaylistManager     *m_plm;
    bool                 m_initialized, isVisible, m_bListChanged, 
                         m_bSearchInProgress, m_bDragging;
    string               m_currentListName, m_activeListName;
    Thread              *m_uiThread;
    POINT                m_sMinSize;
    HTREEITEM	         m_hPlaylistItem, m_hCatalogItem, m_hDropTarget;
    TV_ITEM              m_hTreeDragItem;
    vector<TreeCrossRef> m_oMusicCrossRefs;
    int                  m_iCollapseMoveAmount;
    HCURSOR              m_hSavedCursor, m_hDragCursor, m_hNoDropCursor;
};

#endif

