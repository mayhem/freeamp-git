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
#include "DataIndex.h"

class FAContext;

#define STATE_COLLAPSED 0
#define STATE_EXPANDED  1

bool operator<(const TreeData &A, const TreeData &b);
bool operator==(const TreeData &A, const TreeData &b);
void ClientToWindow(HWND hWnd, POINT *Pt); 
extern HINSTANCE g_hinst;

bool FileOpenDialog(HWND hwnd, 
                    const char* title,
                    const char* filter,
                    vector<char*>* fileList,
                    Preferences* prefs);

LRESULT WINAPI 
ListViewWndProc(HWND hwnd, 
                UINT msg, 
                WPARAM wParam, 
                LPARAM lParam);

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
    void   InitDialog(HWND hwnd);
    void   SetMinMaxInfo(void);

    BOOL   DialogProc(HWND hwnd, UINT msg, 
                      WPARAM wParam, LPARAM lParam);
 
 protected:
    FAContext *m_context;

 private:

    // Functions in Dialog.cpp
    void   ShowBrowser(bool bShowExpanded);
    void   HideBrowser(void);
    void   Close(void);
	void   ExpandCollapseEvent(void);
    void   GetMinMaxInfo(MINMAXINFO *pInfo);
    void   SizeWindow(int type, int iWidth, int iHeight);
    BOOL   SetCursor(int hitTest, int mouseMsg);
    void   MouseMove(uint32 uFlags, POINT &sPoint);
    void   MouseButtonDown(int keys, int x, int y);
    void   MouseButtonUp(int keys, int x, int y);
    BOOL   DrawItem(int32 controlId, DRAWITEMSTRUCT* dis);
    void   SetStatusText(const char *text);
    void   CreateToolbar(void);
    void   ToggleVisEvent(void);
    void   SetTitles(void);
    void   UpdateButtonStates(void);
    void   UpdateMenuItems(void);
    void   MoveControls(int iPixelsToMove);
    bool   CreateMainDialog(void);
    Error  CloseMainDialog(void);
    void   BeginDrag(NM_TREEVIEW *pTreeView);
    uint32 CalcStringEllipsis(HDC hdc, string& displayString, int32 columnWidth);

    // Functions in Event.cpp
    int   Notify(WPARAM wParam, NMHDR *pNMHDR);
    void  StartMusicSearch(void);
    void  MoveUpEvent(void);
    void  MoveDownEvent(void);
    void  AddEvent(void);
    void  EditEvent(void);
    void  DeleteEvent(void);
    void  DeleteListEvent(void);
    void  SortEvent(int id);
    void  EmptyDBCheck(void);
    void  RemoveEvent(void);
    void  RemoveFromDiskEvent(void);
    void  ImportEvent(void);
    void  MoveItemEvent(int source, int dest);

    // Functions in PlaylistView.cpp
    void  OpenPlaylist(void);
    void  NewPlaylist(void);
    void  WritePlaylist(void);
    void  SaveAsPlaylist(void);
    void  UpdatePlaylistList(void);
    void  InitList(void);
    void  AddPlaylist(const string &oName);
    void  LoadPlaylist(const string &oPlaylist);
    bool  SaveNewPlaylist(string &oName);
    void  EditPlaylist(const string &oList);

    // Functions in Win32MusicBrowser.cpp
    void  AddMusicBrowserWindow(MusicBrowserUI *pWindow);
    void  RemoveMusicBrowserWindow(MusicBrowserUI *pWindow);

    // Functions is MusicTree.cpp
    void      InitTree(void);
    void      FillArtists(void);
    void      FillAlbums(TV_ITEM *pItem);
    void      FillPlaylists(void);
    void      FillTracks(TV_ITEM *pItem);
    void      FillAllTracks(void);
    void      FillUncatTracks(void);
    int32     GetCurrentItemFromMousePos(void);
    int32     GetMusicTreeSelection(HTREEITEM hItem);

    // Data members
    EventQueue          *m_playerEQ;
    int32                m_state, m_startupType;
    int32                m_currentindex, m_currentplaying;
  	HWND                 m_hWnd, m_hStatus, m_hParent, m_hToolbar, m_hRebar;
    HWND                 m_hMusicCatalog, m_hPlaylistView;
    HWND                 m_hPlaylistTitle, m_hMusicCatalogTitle;
    PlaylistManager     *m_oPlm;
    bool                 m_initialized, isVisible, m_bListChanged, 
                         m_bSearchInProgress, m_bDragging;
    string               m_currentListName, m_activeListName;
    Thread              *m_uiThread;
    POINT                m_sMinSize;
    HTREEITEM	         m_hPlaylistItem, m_hCatalogItem, m_hDropTarget;
    HTREEITEM            m_hAllItem, m_hUncatItem;
    TV_ITEM              m_hTreeDragItem;
    TreeDataIndex        m_oTreeIndex;
    int                  m_iCollapseMoveAmount;
    HCURSOR              m_hSavedCursor, m_hDragCursor, m_hNoDropCursor;
    HCURSOR              m_hSplitterCursor, m_hPointerCursor, m_hCurrentCursor;
    MusicBrowserUI      *m_pParent;
    vector<MusicBrowserUI *> m_oWindowList;
    bool                 m_overSplitter;
    bool                 m_trackSplitter;
    RECT                 m_splitterRect;
    HBRUSH               m_splitterBrush;

    HWND                 m_hPlaylistHeader;
};

#endif

