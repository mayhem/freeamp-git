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

             MusicBrowserUI(FAContext      *context, 
                            MusicBrowserUI *parent,
                            HWND            hParent,
                            const string   &oPlaylistName);
    virtual ~MusicBrowserUI();

    virtual int32 AcceptEvent(Event *);
    virtual Error Init(int32);
						 
    static void UIThreadFunc(void* arg);

    void  InitDialog(HWND hwnd);
    void  ShowBrowser(bool bShowExpanded);
    void  HideBrowser(void);
    void  Close(void);
	void  ExpandCollapseEvent(void);
    void  GetMinMaxInfo(MINMAXINFO *pInfo);
    void  SetMinMaxInfo(void);
    void  SizeWindow(int iWidth, int iHeight);
    int   Notify(WPARAM wParam, NMHDR *pNMHDR);
    void  StartMusicSearch(void);
    void  MoveUpEvent(void);
    void  MoveDownEvent(void);
    void  AddEvent(void);
    void  EditEvent(void);
    void  DeleteEvent(void);
    void  DeleteListEvent(void);
    void  PlaylistComboChanged(void);
    void  OpenPlaylist(void);
    void  NewPlaylist(void);
    void  WritePlaylist(void);
    void  SaveAsPlaylist(void);
    void  SortEvent(int id);
    void  MouseMove(uint32 uFlags, POINT &sPoint);
    void  MouseButtonUp(void);
    void  EmptyDBCheck(void);
    void  UpdatePlaylistList(void);
    void  RemoveEvent(void);
    void  RemoveFromDiskEvent(void);
    void  ImportEvent(void);
    
    void  AddMusicBrowserWindow(MusicBrowserUI *pWindow);
    void  RemoveMusicBrowserWindow(MusicBrowserUI *pWindow);

    BOOL  DrawItem(int32 controlId, DRAWITEMSTRUCT* dis);
 
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
    void ImportPlaylist(char *path);
    void ReadPlaylist(char *filename, vector<PlaylistItem *> *plist);
    HTREEITEM GetMusicTreeIndices(int &iTrack, int &iPlaylist);

    void ToggleVisEvent(void);

    void InitList(void);
    void AddPlaylist(const string &oName);
    void LoadPlaylist(const string &oPlaylist);
    void SetActivePlaylistIcon(int iImage);
    bool SaveNewPlaylist(string &oName);
    void EditPlaylist(const string &oList);

    void InitTree(void);
    void SetTitles(void);
    void FillArtists(void);
    void FillAlbums(TV_ITEM *pItem);
    void FillPlaylists(void);
    void FillTracks(TV_ITEM *pItem);
    void FillAllTracks(void);
    void FillUncatTracks(void);

    void  UpdateButtonStates(void);
    void  UpdateMenuItems(void);
    void  MoveControls(int iPixelsToMove);
    bool  CreateMainDialog(void);
    Error CloseMainDialog(void);

    void  BeginDrag(NM_TREEVIEW *pTreeView);

    uint32 CalcStringEllipsis(HDC hdc, string& displayString, int32 columnWidth);


    EventQueue          *m_playerEQ;
    int32                m_state, m_startupType;
    int32                m_currentindex, m_currentplaying;
  	HWND                 m_hWnd, m_hStatus;		
    PlaylistManager     *m_oPlm;
    bool                 m_initialized, isVisible, m_bListChanged, 
                         m_bSearchInProgress, m_bDragging;
    string               m_currentListName, m_activeListName;
    Thread              *m_uiThread;
    POINT                m_sMinSize;
    HTREEITEM	         m_hPlaylistItem, m_hCatalogItem, m_hDropTarget;
    HTREEITEM            m_hAllItem, m_hUncatItem;
    TV_ITEM              m_hTreeDragItem;
    vector<TreeCrossRef> m_oMusicCrossRefs;
    int                  m_iCollapseMoveAmount;
    HCURSOR              m_hSavedCursor, m_hDragCursor, m_hNoDropCursor;
    MusicBrowserUI      *m_pParent;
    vector<MusicBrowserUI *> m_oWindowList;
    HWND                 m_hParent;
};

#endif

