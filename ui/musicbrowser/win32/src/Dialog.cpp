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
#include "DropSource.h"
#include "DropObject.h"
#include "debug.h"

#define WM_EMPTYDBCHECK WM_USER + 69

static int pControls[] =
{
   IDOK,
   IDC_PLAYLISTTEXT,
   IDC_MUSICCATALOGTEXT,
   IDC_MUSICTREE,
   IDC_PLAYLISTBOX,
   IDC_PLAYLISTCOMBO,
   IDC_SEARCH,        
   IDC_UP,            
   IDC_DELETE,        
   IDC_ADD,           
   IDC_DOWN,          
   IDC_NEWLIST,       
   IDC_CLEARLIST,
   -1     
};   

TBBUTTON tbButtons[] = {
    { 0, ID_FILE_NEWPLAYLIST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
    { 2, ID_FILE_SAVEPLAYLIST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
    { 1, ID_FILE_IMPORT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 12, ID_EDIT_REMOVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 5, ID_EDIT_EDIT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
    { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
    { 1, ID_EDIT_ADDTRACK, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
    { 1, ID_EDIT_ADDTRACK, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
    { 9, ID_EDIT_MOVEUP, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
    { 10, ID_EDIT_MOVEDOWN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
    { 11, ID_EDIT_CLEARPLAYLIST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0}
	
    
};

static BOOL CALLBACK MainDlgProc(HWND hwnd, UINT msg, 
                                 WPARAM wParam, LPARAM lParam )
{
    MusicBrowserUI* ui = (MusicBrowserUI*)GetWindowLong(hwnd, GWL_USERDATA);
    switch (msg)
    {
        case WM_INITDIALOG:
        {
            ui = (MusicBrowserUI*)lParam;
            assert(ui != NULL);
            SetWindowLong(hwnd, GWL_USERDATA, (LONG)ui);
            
        	ui->InitDialog(hwnd);
        	ui->SetMinMaxInfo();
            return 1;
        }
        default:
            return ui->DialogProc(hwnd, msg, wParam, lParam);
    }
}        

BOOL MusicBrowserUI::DialogProc(HWND hwnd, UINT msg, 
                                WPARAM wParam, LPARAM lParam )
{
    bool filesAreURLs = false;

    switch (msg)
    {
        case WM_DESTROY:
        {
            RevokeDragDrop(m_hPlaylistView);
            OleUninitialize(); 

            break;
        }

		case WM_CLOSE:
        {
			Close();
            return 1;
        }    

        case WM_DROPURLS:
            filesAreURLs = true;
        case WM_DROPFILES:
        {
            DropFiles((HDROP)wParam, filesAreURLs);
            return 0;
        }

		case WM_SIZE:
        {
            SizeWindow(wParam, LOWORD(lParam), HIWORD(lParam));
            return 1;
        } 
		case WM_NOTIFY:
            return Notify(wParam, (LPNMHDR)lParam);
            
		case WM_GETMINMAXINFO:
            GetMinMaxInfo((MINMAXINFO *)lParam);
            break;

        case WM_SETCURSOR:
            return SetCursor(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOUSEMOVE:
        {
            POINT sPoint;   
            sPoint.x = LOWORD(lParam);
            sPoint.y = HIWORD(lParam);  
            MouseMove((uint32)wParam, sPoint);
            return 1;
        }

        case WM_LBUTTONDOWN:
            MouseButtonDown(wParam, LOWORD(lParam), HIWORD(lParam));
            return 1;

        case WM_LBUTTONUP:
            MouseButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));
            return 1;
            
        case WM_EMPTYDBCHECK:
            EmptyDBCheck();
            return 1;

        case WM_COMMAND:
        {
        	switch(LOWORD(wParam))
            {
                case 2: // what is this? we get it when we press ESC.
                    if(m_bSearchInProgress)
                    {
                        StartStopMusicSearch();  
                    }
                    return 1;
                case ID_FILE_SEARCHFORMUSIC:
                case IDC_SEARCH:
                    StartStopMusicSearch();
                return 1;
                case ID_EDIT_MOVEUP:
                case IDC_UP:
                    MoveUpEvent();
                return 1;
                case ID_EDIT_MOVEDOWN:
                case IDC_DOWN:
                    MoveDownEvent();
                return 1;
                case IDC_DELETE:
                    DeleteEvent();
                return 1;
                case ID_EDIT_ADDTRACK:
                case IDC_ADD:
                    AddEvent();
                return 1;
                case ID_EDIT_CLEARPLAYLIST:
                case IDC_CLEARLIST:
                    DeleteListEvent();
                return 1;
                case ID_VIEW_MUSICCATALOG:
                    ExpandCollapseEvent();
                return 1;
                case ID_FILE_OPENPLAYLIST:
                    OpenPlaylist();
                return 1;
                case ID_FILE_EXIT:
                    Close();
                return 1;
                case ID_FILE_SAVEPLAYLIST:
                    WritePlaylist();
                return 1;
                case ID_FILE_SAVEASPLAYLIST:
                    SaveAsPlaylist();
                return 1;
                case IDC_NEWLIST:
                case ID_FILE_NEWPLAYLIST:
                    NewPlaylist();
                return 1;
                case ID_SORT_ARTIST:
                case ID_SORT_ALBUM:
                case ID_SORT_TITLE:
                case ID_SORT_LENGTH:
                case ID_SORT_YEAR:
                case ID_SORT_TRACK:
                case ID_SORT_GENRE:
                case ID_SORT_LOCATION:
                case ID_SORT_RANDOMIZE:
                case IDC_RANDOMIZE:
                    SortEvent(LOWORD(wParam));
                return 1;
                case ID_EDIT_EDIT:
                    EditEvent();
                return 1;
                case ID_EDIT_REMOVE:
                    RemoveEvent();
                return 1;
                case ID_EDIT_REMOVEFROMDISK:
                    RemoveFromDiskEvent();
                return 1;
                case ID_FILE_IMPORT:
                    ImportEvent();
                return 1;
            }    
        }     

        case WM_DRAWITEM:
        {
            return DrawItem(wParam, (DRAWITEMSTRUCT*) lParam);

            break;
        }
        
        case WM_MEASUREITEM:
        {
            MEASUREITEMSTRUCT* mis = (MEASUREITEMSTRUCT*) lParam;
            TEXTMETRIC tm;
            HDC hdc;
	        HFONT hFont;
	        HWND hwndLV;


            // Make sure the control is the listview control
            if (mis->CtlType != ODT_LISTVIEW || mis->CtlID != IDC_PLAYLISTBOX)
                return FALSE;

	        // Get the handle of the ListView control we're using
	        hwndLV = GetDlgItem(hwnd, mis->CtlID);

	        // Get the font the control is currently using
	        hFont = (HFONT)(DWORD) SendMessage(hwndLV, WM_GETFONT, 0, 0L);

	        // Set the font of the DC to the same font the control is using
            hdc = GetDC(hwndLV);
	        SelectObject(hdc, hFont);

            // Get the height of the font used by the control
            if (!GetTextMetrics(hdc, &tm))
                return FALSE;

            // Add a little extra space between items
            mis->itemHeight = tm.tmHeight + 1;

            // Make sure there is enough room for the images which are CY_SMICON high
            if (mis->itemHeight < 17)
    	        mis->itemHeight = 17;

	        // Clean up
	        ReleaseDC(hwndLV, hdc);

            return TRUE;

            break;
        }

    }
    
    return 0;
}

bool MusicBrowserUI::CreateMainDialog(void)
{
    MSG   msg;

	m_hWnd = CreateDialogParam( g_hinst, 
                    MAKEINTRESOURCE(IDD_MUSICBROWSER),
                    NULL,
                    (DLGPROC)MainDlgProc, 
                    (LPARAM)this);
    if(m_hWnd)
    {
        if (m_pParent)
        {
            RECT sRect;
            
            GetWindowRect(m_hParent, &sRect);
            SetWindowPos(m_hWnd, NULL, sRect.left + 20, sRect.top + 20,
                         0, 0, SWP_NOZORDER|SWP_NOSIZE);
            ShowWindow(m_hWnd, SW_NORMAL);
            UpdateWindow(m_hWnd);
        }    
        while(GetMessage(&msg,NULL,0,0))
        {
            if(!IsDialogMessage(m_hWnd, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
    
    ImageList_Destroy(TreeView_SetImageList(
                      m_hMusicCatalog,
                          NULL, TVSIL_NORMAL)); 
    ImageList_Destroy(ListView_SetImageList(
                      m_hPlaylistView,
                          NULL, LVSIL_SMALL)); 
    DestroyWindow(m_hWnd);
    m_hWnd = NULL;

    if (m_pParent)
    {
       m_pParent->RemoveMusicBrowserWindow(this);
       return true;
    }
    
    return false;
}

void MusicBrowserUI::UIThreadFunc(void* arg)
{
    MusicBrowserUI* ui = (MusicBrowserUI*)arg;

    if (ui->CreateMainDialog())
       delete ui;
}

Error MusicBrowserUI::CloseMainDialog(void)
{ 
    if (m_pParent)
    {
       if (m_bListChanged)
       {
           ShowWindow(m_hWnd, SW_RESTORE);
           SetForegroundWindow(m_hWnd);
           if (MessageBox(m_hWnd, "Would you like to save this playlist?",
                          BRANDING, MB_YESNO) == IDYES)
              WritePlaylist();
       }    
    }
    else      
       if (m_bListChanged)
           WritePlaylist();

    m_bListChanged = false;   
    PostMessage(m_hWnd, WM_QUIT, 0, 0);
       
    return kError_NoErr;
}

void MusicBrowserUI::ShowBrowser(bool bShowExpanded)
{
    PostMessage(m_hWnd, WM_EMPTYDBCHECK, 0, 0);
    ShowWindow(m_hWnd, SW_RESTORE);
	ShowWindow(m_hWnd, SW_SHOW);
    SetForegroundWindow(m_hWnd);
}

void MusicBrowserUI::HideBrowser(void)
{
	isVisible = false;
	ShowWindow(m_hWnd, SW_HIDE);
}

void MusicBrowserUI::Close(void)
{
    if (m_pParent == NULL)
    {
       HideBrowser();
    }   
    else
       CloseMainDialog();
}

void MusicBrowserUI::ExpandCollapseEvent(void)
{
    HMENU        hMenu;
    MENUITEMINFO sItem;
    RECT catalogRect, playlistRect, titleRect;

    GetWindowRect(m_hMusicCatalog, &catalogRect);
    GetWindowRect(m_hPlaylistView, &playlistRect);
    GetWindowRect(m_hPlaylistTitle, &titleRect);
    MapWindowPoints(NULL, m_hWnd, (LPPOINT)&catalogRect, 2);
    MapWindowPoints(NULL, m_hWnd, (LPPOINT)&playlistRect, 2);
    MapWindowPoints(NULL, m_hWnd, (LPPOINT)&titleRect, 2);

    if (m_state == STATE_COLLAPSED)
    {
       m_state = STATE_EXPANDED;
       SetWindowText(m_hWnd, BRANDING " - Music Browser");
       sItem.dwTypeData = "View &playlist only";

       ShowWindow(m_hMusicCatalog, SW_SHOW);
       ShowWindow(m_hMusicCatalogTitle, SW_SHOW);

       MoveWindow(m_hPlaylistView, 
                  catalogRect.left + m_iCollapseMoveAmount, 
                  catalogRect.top, 
        	      (playlistRect.right - playlistRect.left) - m_iCollapseMoveAmount,
                  playlistRect.bottom - playlistRect.top,
                  TRUE);

       MoveWindow(m_hPlaylistTitle, 
                  titleRect.left + m_iCollapseMoveAmount, 
                  titleRect.top, 
        	      (titleRect.right - titleRect.left) - m_iCollapseMoveAmount,
                  titleRect.bottom - titleRect.top,
                  TRUE);
    }            
    else
    {                
       m_state = STATE_COLLAPSED;
       SetWindowText(m_hWnd, BRANDING " - Playlist Manager");
       sItem.dwTypeData = "View &music browser";

       ShowWindow(m_hMusicCatalog, SW_HIDE);
       ShowWindow(m_hMusicCatalogTitle, SW_HIDE);

       m_iCollapseMoveAmount = playlistRect.left - catalogRect.left;
       
       MoveWindow(m_hPlaylistView, 
                  catalogRect.left, 
                  catalogRect.top, 
        	      (playlistRect.right - playlistRect.left) + m_iCollapseMoveAmount,
                  playlistRect.bottom - playlistRect.top,
                  TRUE);

       MoveWindow(m_hPlaylistTitle, 
                  titleRect.left - m_iCollapseMoveAmount, 
                  titleRect.top, 
        	      (titleRect.right - titleRect.left) + m_iCollapseMoveAmount,
                  titleRect.bottom - titleRect.top,
                  TRUE);
    }

    hMenu = GetMenu(m_hWnd);
    hMenu = GetSubMenu(hMenu, 2);
    sItem.cbSize = sizeof(MENUITEMINFO);
    sItem.fMask = MIIM_TYPE;
    sItem.fType = MFT_STRING;
    sItem.cch = strlen(sItem.dwTypeData);
    SetMenuItemInfo(hMenu, ID_VIEW_MUSICCATALOG, false, &sItem);
    
    SendMessage(m_hStatus, SB_SETTEXT, 0, (LPARAM)"");
    InvalidateRect(m_hWnd, NULL, true);
    UpdateWindow(m_hWnd);
}

void MusicBrowserUI::MoveControls(int iPixelsToMove)
{
	RECT  sLoc;
    POINT sCorner;
    int  *iIndex;
    
    for(iIndex = pControls; *iIndex >= 0; iIndex++)
    {
	    GetWindowRect(GetDlgItem(m_hWnd, *iIndex), &sLoc);
        sCorner.x = sLoc.left;
        sCorner.y = sLoc.top;
        ScreenToClient(m_hWnd, &sCorner);
    	SetWindowPos(GetDlgItem(m_hWnd, *iIndex), NULL,
                     sCorner.x + iPixelsToMove,
                     sCorner.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }                 
}

void MusicBrowserUI::SizeWindow(int iType, int iWidth, int iHeight)
{
    RECT windowRect;
    RECT clientRect;
    RECT controlRect;
    uint32 windowHeight, windowWidth;
    uint32 clientHeight, clientWidth;
    uint32 controlHeight, controlWidth;
    uint32 statusbarHeight;
    RECT oldListViewRect, newListViewRect;
    int32 headerResizeAmount;

    GetWindowRect(m_hWnd, &windowRect);
    windowHeight = windowRect.bottom - windowRect.top;
    windowWidth = windowRect.right - windowRect.left;

    GetClientRect(m_hWnd, &clientRect);
    clientHeight = clientRect.bottom - clientRect.top;
    clientWidth = clientRect.right - clientRect.left;

    HDWP hdwp = BeginDeferWindowPos(5);
    
    // Status Bar
    GetWindowRect(m_hStatus, &controlRect); 
    controlHeight = controlRect.bottom - controlRect.top;
    controlWidth = controlRect.right - controlRect.left;

    statusbarHeight = controlHeight;
    hdwp = DeferWindowPos(hdwp, m_hStatus, NULL,
                           windowRect.left, 
                           windowRect.bottom - controlHeight,
                           windowWidth,
                           controlHeight,
                           SWP_NOZORDER);

    // Tool Bar
    GetWindowRect(m_hRebar, &controlRect); 
    controlHeight = controlRect.bottom - controlRect.top;
    controlWidth = controlRect.right - controlRect.left;

    hdwp = DeferWindowPos(hdwp, m_hRebar, NULL,
                           0, 
                           0,
                           clientWidth,
                           controlHeight,
                           SWP_NOZORDER);

    // Playlist View
    GetClientRect(m_hPlaylistView, &oldListViewRect); 
    GetWindowRect(m_hPlaylistView, &controlRect); 
    MapWindowPoints(NULL, m_hWnd, (LPPOINT)&controlRect, 2);
    controlHeight = controlRect.bottom - controlRect.top;
    controlWidth = controlRect.right - controlRect.left;

    hdwp = DeferWindowPos(hdwp, m_hPlaylistView, NULL,
                           controlRect.left,
                           controlRect.top,
                           clientRect.right - controlRect.left,
                           clientRect.bottom - controlRect.top - statusbarHeight,
                           SWP_NOZORDER);


    // Music Catalog View
    GetWindowRect(m_hMusicCatalog, &controlRect); 
    MapWindowPoints(NULL, m_hWnd, (LPPOINT)&controlRect, 2);
    controlHeight = controlRect.bottom - controlRect.top;
    controlWidth = controlRect.right - controlRect.left;

    hdwp = DeferWindowPos(hdwp, m_hMusicCatalog, NULL,
                           controlRect.left,
                           controlRect.top,
                           controlWidth,
                           clientRect.bottom - controlRect.top - statusbarHeight,
                           SWP_NOZORDER);


    // Current Playlist Title
    GetWindowRect(m_hPlaylistTitle, &controlRect); 
    MapWindowPoints(NULL, m_hWnd, (LPPOINT)&controlRect, 2);
    controlHeight = controlRect.bottom - controlRect.top;
    controlWidth = controlRect.right - controlRect.left;

    hdwp = DeferWindowPos(hdwp, m_hPlaylistTitle, NULL,
                           controlRect.left,
                           controlRect.top,
                           clientRect.right - controlRect.left,
                           controlHeight,
                           SWP_NOZORDER);
                           
    EndDeferWindowPos(hdwp);

    // Resize the listview headers

#if 0

#define LENGTH_COLUMN_WIDTH 60
#define INDEX_COLUMN_WIDTH 25
#define FIXED_COLUMN_WIDTH (LENGTH_COLUMN_WIDTH + INDEX_COLUMN_WIDTH)

    RECT sRect;

    GetClientRect(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sRect);

    int32 remainder = (sRect.right-sRect.left - FIXED_COLUMN_WIDTH)%3;
    
    int32 width = (sRect.right-sRect.left - FIXED_COLUMN_WIDTH)/3;
    ListView_SetColumnWidth(m_hPlaylistView, 1, width);
    ListView_SetColumnWidth(m_hPlaylistView, 2, width);
    ListView_SetColumnWidth(m_hPlaylistView, 3, width);
    
    width = LENGTH_COLUMN_WIDTH + remainder;
    ListView_SetColumnWidth(m_hPlaylistView, 4, width);

#endif

    GetClientRect(m_hPlaylistView, &newListViewRect);
    
    headerResizeAmount = (newListViewRect.right - newListViewRect.left) - 
                         (oldListViewRect.right - oldListViewRect.left);

    
    int32 eachHeaderAmount = headerResizeAmount/3;
    int32 titleExtraAmount = headerResizeAmount%3;
    int32 width;

    if(eachHeaderAmount)
    {
        width = ListView_GetColumnWidth(m_hPlaylistView, 1);
        width += eachHeaderAmount;

        if(titleExtraAmount)
            width += titleExtraAmount;
    
        ListView_SetColumnWidth(m_hPlaylistView, 1, width);

        width = ListView_GetColumnWidth(m_hPlaylistView, 2);
        width += eachHeaderAmount;
        ListView_SetColumnWidth(m_hPlaylistView, 2, width);

        width = ListView_GetColumnWidth(m_hPlaylistView, 3);
        width += eachHeaderAmount;

        ListView_SetColumnWidth(m_hPlaylistView, 3, width);
    }
    else
    {
        static uint32 lastColumn = 1;

        width = ListView_GetColumnWidth(m_hPlaylistView, lastColumn);
       
        width += titleExtraAmount;

        ListView_SetColumnWidth(m_hPlaylistView, lastColumn, width);

        if(++lastColumn > 3)
            lastColumn = 1;
    }
}

void MusicBrowserUI::GetMinMaxInfo(MINMAXINFO *pInfo)
{
    pInfo->ptMinTrackSize = m_sMinSize;
}

void MusicBrowserUI::SetMinMaxInfo(void)
{
	RECT  sLoc, sLoc2;
    
	GetWindowRect(m_hMusicCatalog, &sLoc);
	GetWindowRect(m_hPlaylistView, &sLoc2);
    m_iCollapseMoveAmount = sLoc2.left - sLoc.left;
}

void MusicBrowserUI::InitDialog(HWND hWnd)
{
    HIMAGELIST      hList;
    const int32     kNumPanes = 1;
    int32           panes[kNumPanes]= {-1};

    m_hWnd = hWnd;
    m_hMusicCatalog = GetDlgItem(m_hWnd, IDC_MUSICTREE);
    m_hPlaylistView = GetDlgItem(m_hWnd, IDC_PLAYLISTBOX);
    m_hPlaylistTitle = GetDlgItem(m_hWnd, IDC_PLAYLISTTITLE);
    m_hMusicCatalogTitle = GetDlgItem(m_hWnd, IDC_MUSICCATALOGTEXT);

    HBITMAP bmp;
    
    hList = ImageList_Create(16, 16, ILC_COLOR24|ILC_MASK, 5, 0);

    bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_CATALOG));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
    bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_PLAYLIST));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
    bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_ARTIST));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
    bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_ALBUM));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
    bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_TRACK));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
 
    TreeView_SetImageList(m_hMusicCatalog, hList, TVSIL_NORMAL); 
    
    hList = ImageList_Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 0);

    bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_PLAYING));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
    DeleteObject(bmp);
 
    ListView_SetImageList(m_hPlaylistView, hList, LVSIL_SMALL); 

    if (m_pParent == NULL)
    {
//       string lastPlaylist = FreeampDir(m_context->prefs);
//       lastPlaylist += "\\currentlist.m3u";
//       LoadPlaylist(lastPlaylist);
        m_oPlm->SetActivePlaylist(kPlaylistKey_MasterPlaylist);
    }   
    else   
    {
        char   url[MAX_PATH];
        uint32 len = MAX_PATH;
        FilePathToURL(m_currentListName.c_str(), url, &len);
        m_oPlm->SetActivePlaylist(kPlaylistKey_ExternalPlaylist);
        LoadPlaylist(url);
    }   

    InitTree();                      
    InitList();
    SetTitles();
    CreateToolbar();

    // Subclass the listview
    m_hPlaylistHeader = FindWindowEx(m_hPlaylistView, NULL, WC_HEADER, NULL);

    if(m_hPlaylistHeader)
    {
        // Set the proc address as a property 
	    // of the window so it can get it
	    SetProp(m_hPlaylistView, 
			    "oldproc",
                (HANDLE)GetWindowLong(m_hPlaylistView, GWL_WNDPROC));

        SetProp(m_hPlaylistView, 
            "this",
            (HANDLE)this);
	    
	    // Subclass the window so we can draw it
	    SetWindowLong(m_hPlaylistView, 
                      GWL_WNDPROC, 
                      (DWORD)::ListViewWndProc );  

        HD_ITEM hd_item;
    
        hd_item.mask = HDI_FORMAT;
        hd_item.fmt = HDF_OWNERDRAW;

        Header_SetItem(m_hPlaylistHeader, 0, &hd_item);

        //hd_item.fmt = HDF_CENTER;
        //Header_SetItem(m_hPlaylistHeader, 1, &hd_item);
        //Header_SetItem(m_hPlaylistHeader, 2, &hd_item);
        //Header_SetItem(m_hPlaylistHeader, 3, &hd_item);
    }

    // Subclass the treeview

    // Set the proc address as a property 
	// of the window so it can get it
	SetProp(m_hMusicCatalog, 
            "oldproc",
            (HANDLE)GetWindowLong(m_hMusicCatalog, GWL_WNDPROC));

    SetProp(m_hMusicCatalog, 
            "this",
            (HANDLE)this);
	
	// Subclass the window so we can draw it
	SetWindowLong(m_hMusicCatalog, 
			      GWL_WNDPROC, 
                  (DWORD)::TreeViewWndProc );  

    // register our OLE drag and drop crap
    OleInitialize(NULL);

    int result;
    m_playlistDropTarget = new DropTarget(m_hPlaylistView);
    //CoLockObjectExternal ((IUnknown*)m_playlistDropTarget, TRUE, TRUE);
    result = RegisterDragDrop(m_hPlaylistView, m_playlistDropTarget);


    m_hStatus= CreateStatusWindow(WS_CHILD | WS_VISIBLE,
                                  "", m_hWnd, IDC_STATUS);

    SendMessage(m_hStatus, SB_SETPARTS, kNumPanes, (LPARAM) panes);
    m_currentindex = -1;
    UpdateButtonStates();

    m_sMinSize.x = 500;
    m_sMinSize.y = 300;
    
    if (m_pParent)
        ShowWindow(m_hWnd, SW_SHOW);
}

#define ID_TOOLBAR         13000
#define ID_REBAR           13001
#define TOOLBAR_INDENT	   8

void MusicBrowserUI::CreateToolbar(void)
{
    INITCOMMONCONTROLSEX icex;
    //REBARINFO rbi;
    REBARBANDINFO rbb;
    RECT rect;
    RECT dummyRect;
    HWND hwndDummy = GetDlgItem(m_hWnd, IDC_DUMMYCONTROL);

    GetWindowRect(hwndDummy, &dummyRect);
    int32 toolbarHeight = dummyRect.bottom - dummyRect.top;

    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_COOL_CLASSES ;	// load the rebar
	InitCommonControlsEx(&icex);

    GetClientRect(m_hWnd, &rect);

    // Create the REBAR control
	m_hRebar = CreateWindowEx( 
				0L,
				REBARCLASSNAME,
				NULL,
                WS_VISIBLE | WS_BORDER | WS_CHILD | 0 |
                  WS_CLIPSIBLINGS | CCS_NODIVIDER | RBS_VARHEIGHT | 
                  RBS_BANDBORDERS | CCS_NOPARENTALIGN,
				0, 0, rect.right - rect.left, toolbarHeight,
				m_hWnd,
				(HMENU)ID_REBAR,
				g_hinst,
				NULL );

    /*m_hToolbar = CreateToolbarEx(hwndRebar, 
                     WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT |
                     TBSTYLE_TOOLTIPS | TBSTYLE_LIST | WS_CLIPCHILDREN | 
                     WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE,
                     0, 8, g_hinst, IDB_TOOLBAR,
		             (LPCTBBUTTON)&tbButtons,
		             6, 44, 44, 16, 16, sizeof(TBBUTTON));*/

    m_hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, (LPSTR) NULL, 
                     WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT |
                     TBSTYLE_TOOLTIPS |  WS_CLIPCHILDREN | 
                     WS_CLIPSIBLINGS | CCS_NODIVIDER | CCS_NORESIZE, 
                     0, 0, 0, 0, m_hRebar, (HMENU) ID_TOOLBAR, g_hinst, NULL); 

    // Send the TB_BUTTONSTRUCTSIZE message, which is required for 
    // backward compatibility. 
    SendMessage(m_hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0); 

    SendMessage(m_hToolbar, TB_SETBITMAPSIZE, 0, MAKELPARAM(16, 16)); 

    TBADDBITMAP tbab; 

    // Add the bitmap containing button images to the toolbar.
    tbab.hInst = g_hinst; 
    tbab.nID   = IDB_TOOLBAR; 
    SendMessage(m_hToolbar, TB_ADDBITMAP, (WPARAM) 13, (WPARAM) &tbab);

    int32 index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"New Playlist");
    tbButtons[0].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Save Playlist");
    tbButtons[1].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Import Item");
    tbButtons[3].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Remove Item");
    tbButtons[4].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Edit Item");
    tbButtons[5].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Add Item");
    tbButtons[7].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Add File");
    tbButtons[8].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Move Up");
    tbButtons[9].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Move Down");
    tbButtons[10].iString = index;
    index = SendMessage(m_hToolbar, TB_ADDSTRING, (WPARAM) 0, (LPARAM)"Clear Playlist");
    tbButtons[11].iString = index;
    
    

    SendMessage(m_hToolbar, TB_ADDBUTTONS, (WPARAM) 12, (LPARAM) &tbButtons);
    SendMessage(m_hToolbar, TB_AUTOSIZE, (WPARAM) 0, (LPARAM) 0);

    //GetClientRect(m_hRebar, &dummyRect);
    SendMessage(m_hToolbar, TB_GETITEMRECT , (WPARAM) 0, (LPARAM) &dummyRect); 
    SendMessage(m_hToolbar, TB_SETBUTTONSIZE, 0, 
                    MAKELPARAM(dummyRect.right - dummyRect.left, toolbarHeight - TOOLBAR_INDENT)); 

    // Initialize REBARBANDINFO for all rebar bands
	rbb.cbSize = sizeof(REBARBANDINFO);
	rbb.fMask = RBBIM_COLORS |	// clrFore and clrBack are valid
		RBBIM_CHILD |				// hwndChild is valid
		RBBIM_CHILDSIZE |			// cxMinChild and cyMinChild are valid
		RBBIM_STYLE |				// fStyle is valid
		RBBIM_ID;					// wID is valid
	rbb.clrFore = GetSysColor(COLOR_BTNTEXT);
	rbb.clrBack = GetSysColor(COLOR_BTNFACE);
	rbb.fStyle = RBBS_NOVERT |	// do not display in vertical orientation
		            RBBS_CHILDEDGE;
	rbb.hbmBack = NULL;
	rbb.hwndChild = m_hToolbar;
	rbb.wID = ID_TOOLBAR;
	rbb.cxMinChild = rect.right - rect.left;
	rbb.cyMinChild = toolbarHeight - TOOLBAR_INDENT;

	// Insert band into rebar
	SendMessage(m_hRebar, RB_INSERTBAND, (UINT) -1, (LPARAM) &rbb);
}

void MusicBrowserUI::SetTitles(void)
{
    if (m_pParent == NULL)
    {
       SetWindowText(m_hPlaylistTitle, 
                     "Currently listening to:");
       SetWindowText(m_hWnd, 
                     BRANDING " - My Music");
    }   
    else
    {
       string oTitle, oName;
       
       if (m_currentListName.length() == 0)
          oName = "New Playlist";
       else
       {
          char szBase[MAX_PATH];
       
          _splitpath(m_currentListName.c_str(), NULL, NULL, szBase, NULL);
          oName = szBase;
       }
       
       oTitle = string("Editing playlist: ") + oName;
       SetWindowText(m_hPlaylistTitle, 
                     oTitle.c_str());
       oTitle = string(BRANDING " - Editing ") + oName;
       SetWindowText(m_hWnd, oTitle.c_str());
    }   
}

/*void MusicBrowserUI::BeginDrag(NM_TREEVIEW *pTreeView)
{
    POINT      sPoint;
    
    m_hTreeDragItem = pTreeView->itemNew;
    sPoint = pTreeView->ptDrag;

    m_hDragCursor = LoadCursor(g_hinst, MAKEINTRESOURCE(IDC_DRAG));
    m_hNoDropCursor = LoadCursor(g_hinst, MAKEINTRESOURCE(IDC_NODROP));
    m_hSavedCursor = ::SetCursor(m_hDragCursor);
    
    SetCapture(m_hWnd);
    m_bDragging = true;
}*/

void MusicBrowserUI::BeginDrag(HWND hwnd, NM_TREEVIEW* nmtv)
{
    /*HIMAGELIST himl;    // handle of image list 
    RECT rcItem;        // bounding rectangle of item 
    //DWORD dwLevel;      // heading level of item 
    DWORD dwIndent;     // amount that child items are indented 
 
    // Tell the tree-view control to create an image to use 
    // for dragging. 
    himl = TreeView_CreateDragImage(hwnd, nmtv->itemNew.hItem);
 
    // Get the bounding rectangle of the item being dragged. 
    TreeView_GetItemRect(hwnd, nmtv->itemNew.hItem, &rcItem, TRUE); 
 
    // Get the heading level and the amount that the child items are 
    // indented. 
    //dwLevel = nmtv->itemNew.lParam; 
    dwIndent = (DWORD) SendMessage(hwnd, TVM_GETINDENT, 0, 0);
 
    //MapWindowPoints(hwnd, GetParent(hwnd), &nmtv->ptDrag, 1);

    // Start the drag operation. 
    //ImageList_DragEnter(GetParent(hwnd), nmtv->ptDrag.x, nmtv->ptDrag.y);
    ImageList_BeginDrag(himl, 
                        0, 
                        nmtv->ptDrag.x - rcItem.left, 
                        nmtv->ptDrag.y - rcItem.top);
 
    POINT pt = nmtv->ptDrag;
    //ClientToScreen(hwnd, &pt);
    //MapWindowPoints(hwnd, GetParent(hwnd), &pt, 1);
    
    ClientToScreen(hwnd, &pt);

    //ImageList_DragEnter(NULL, pt.x, pt.y);
    ImageList_DragEnter(NULL, pt.x, pt.y);
 
    // Hide the mouse cursor, and direct mouse input to the 
    // parent window. 
    ShowCursor(FALSE); 
    SetCapture(GetParent(hwnd)); 
    m_bDragging = true; */
    
    vector<string>* urls = new vector<string>;

    GetSelectedMusicTreeItems(urls);
    //urls->push_back("file://c|/local/mpegs/311 - Beautiful Disaster.mp3");
    //urls->push_back("file://c|/local/mpegs/Age of Aquarius.mp3");
    //urls->push_back("file://c|/local/mpegs/B-52s - Loveshack.mp3");

    DataObject* data = new DataObject(urls);
    DropSource* src = new DropSource(hwnd, nmtv);
    DWORD dwEffect = 0;

    DoDragDrop(data, src, DROPEFFECT_COPY|DROPEFFECT_SCROLL, &dwEffect); 

    data->Release();
    src->Release();

}

void MusicBrowserUI::DropFiles(HDROP dropHandle, bool filesAreURLs)
{
    uint32 count;
    char url[1024];

    count = DragQueryFile(  dropHandle,
                            -1L,
                            url,
                            sizeof(url));

    for(uint32 i = 0; i < count; i++)
    {
        DragQueryFile(  dropHandle,
                        i,
                        url,
                        sizeof(url));

        //MessageBox(NULL, url, "url", MB_OK);
    }
}

BOOL MusicBrowserUI::SetCursor(int hitTest, int mouseMsg)
{
    BOOL result = FALSE;

    if((m_overSplitter || m_trackSplitter) && hitTest == HTCLIENT)
    {
        ::SetCursor(m_hSplitterCursor);
        result = TRUE;
    }
    return result;
}

void MusicBrowserUI::MouseMove(uint32 uFlags, POINT &sPoint)
{
    if(m_bDragging)
    {
        // Drag the item to the current position of the mouse cursor. 

        //ClientToScreen(m_hWnd, &sPoint);

        GetCursorPos(&sPoint);
        
        ImageList_DragMove(sPoint.x, sPoint.y); 
    }

    /*if(m_bDragging)
    {
        TV_HITTESTINFO sHit;
        
        sHit.flags = TVHT_ONITEM;
        
        ClientToWindow(m_hWnd, &sPoint); 
        sHit.pt = sPoint;
        m_hDropTarget = TreeView_HitTest(m_hMusicCatalog,
                                         &sHit);
        if (m_hDropTarget != NULL)
        {
            TV_ITEM sItem;
            
            sItem.hItem = m_hDropTarget;
            sItem.mask = TVIF_PARAM;
            TreeView_GetItem(m_hMusicCatalog, &sItem);
            if (sItem.lParam < 0)
                ::SetCursor(m_hDragCursor);
            else    
            {
                ::SetCursor(m_hNoDropCursor);
               m_hDropTarget = NULL;
            }
        }       
        else    
            ::SetCursor(m_hNoDropCursor);
    }*/
    else if(m_trackSplitter)
    {
        HDC hdc = GetDC(NULL);
        POINT pt = sPoint;

        MapWindowPoints(m_hWnd, NULL, (LPPOINT)&pt, 1);

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_splitterBrush);

        PatBlt(hdc,
               m_splitterRect.left, 
               m_splitterRect.top, 
               m_splitterRect.right - m_splitterRect.left, 
               m_splitterRect.bottom - m_splitterRect.top, 
               PATINVERT);

        OffsetRect(&m_splitterRect, (pt.x - m_splitterRect.left), 0);

        PatBlt(hdc,
               m_splitterRect.left, 
               m_splitterRect.top, 
               m_splitterRect.right - m_splitterRect.left, 
               m_splitterRect.bottom - m_splitterRect.top, 
               PATINVERT);

        SelectObject(hdc, oldBrush);

        ReleaseDC(m_hWnd, hdc);
    }
    else
    {
        RECT catalogRect,playlistRect;
        
        GetWindowRect(m_hMusicCatalog, &catalogRect);
        GetWindowRect(m_hPlaylistView, &playlistRect);
        MapWindowPoints(NULL, m_hWnd, (LPPOINT)&catalogRect, 2);
        MapWindowPoints(NULL, m_hWnd, (LPPOINT)&playlistRect, 2);

        m_splitterRect.top = playlistRect.top;
        m_splitterRect.bottom = playlistRect.bottom;
        m_splitterRect.left = catalogRect.right + 1;
        m_splitterRect.right = playlistRect.left - 1;

        if(PtInRect(&m_splitterRect, sPoint))
        {
            m_overSplitter = true;
        }
        else
        {
            m_overSplitter = false;
        }
   
    }
} 

void MusicBrowserUI::MouseButtonDown(int keys, int x, int y)
{
    RECT catalogRect,playlistRect;
    RECT clientRect;
    POINT pt;

    pt.x = x;
    pt.y = y;

    GetClientRect(m_hWnd, &clientRect);
    //InflateRect(&clientRect, -150, 0);
    clientRect.left += 125;
    clientRect.right -= 175;
    GetWindowRect(m_hMusicCatalog, &catalogRect);
    GetWindowRect(m_hPlaylistView, &playlistRect);
    MapWindowPoints(m_hWnd, NULL, (LPPOINT)&pt, 1);
    MapWindowPoints(m_hWnd, NULL, (LPPOINT)&clientRect, 2);

    m_splitterRect.top = playlistRect.top;
    m_splitterRect.bottom = playlistRect.bottom;
    m_splitterRect.left = catalogRect.right + 1;
    m_splitterRect.right = playlistRect.left - 1;

    if(PtInRect(&m_splitterRect, pt))
    {
        m_trackSplitter = true;
        SetCapture(m_hWnd);
        ClipCursor(&clientRect);
        m_hSavedCursor = ::SetCursor(m_hSplitterCursor);

        HDC hdc = GetDC(NULL);

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_splitterBrush);

        PatBlt(hdc,
               m_splitterRect.left, 
               m_splitterRect.top, 
               m_splitterRect.right - m_splitterRect.left, 
               m_splitterRect.bottom - m_splitterRect.top, 
               PATINVERT);

        SelectObject(hdc, oldBrush);

        ReleaseDC(m_hWnd, hdc);
    }
}

void MusicBrowserUI::MouseButtonUp(int keys, int x, int y)
{
    POINT pt;

    pt.x = x;
    pt.y = y;

    if(m_trackSplitter)
    {
        m_trackSplitter = false;
        ReleaseCapture();
        ClipCursor(NULL);
        ::SetCursor(m_hSavedCursor);

        HDC hdc = GetDC(NULL);

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_splitterBrush);

        PatBlt(hdc,
               m_splitterRect.left, 
               m_splitterRect.top, 
               m_splitterRect.right - m_splitterRect.left, 
               m_splitterRect.bottom - m_splitterRect.top, 
               PATINVERT);

        SelectObject(hdc, oldBrush);

        ReleaseDC(m_hWnd, hdc);


        RECT catalogRect, playlistRect, titleRect;
        int32 delta;

        GetWindowRect(m_hMusicCatalog, &catalogRect);
        GetWindowRect(m_hPlaylistView, &playlistRect);
        GetWindowRect(m_hPlaylistTitle, &titleRect);

        delta = (m_splitterRect.right + 1) - playlistRect.left;
        catalogRect.right = m_splitterRect.left - 1;
        playlistRect.left = m_splitterRect.right + 1;
        titleRect.left += delta;

        MapWindowPoints(NULL, m_hWnd, (LPPOINT)&catalogRect, 2);
        MapWindowPoints(NULL, m_hWnd, (LPPOINT)&playlistRect, 2);
        MapWindowPoints(NULL, m_hWnd, (LPPOINT)&titleRect, 2);

        
        MoveWindow(m_hMusicCatalog, 
                  catalogRect.left, 
                  catalogRect.top, 
        	      (catalogRect.right - catalogRect.left),
                  catalogRect.bottom - catalogRect.top,
                  TRUE);

        MoveWindow(m_hPlaylistView, 
                  playlistRect.left,
                  playlistRect.top, 
        	      (playlistRect.right - playlistRect.left),
                  playlistRect.bottom - playlistRect.top,
                  TRUE);

        MoveWindow(m_hPlaylistTitle, 
                  titleRect.left,
                  titleRect.top, 
        	      (titleRect.right - titleRect.left),
                  titleRect.bottom - titleRect.top,
                  TRUE);


    }
    else if(m_bDragging) 
    { 
        ImageList_EndDrag(); 
        ReleaseCapture(); 
        ShowCursor(TRUE); 
        m_bDragging = false; 
    } 
    /*
    else if(m_bDragging)
    {
        int i;

        ReleaseCapture();
        ::SetCursor(m_hSavedCursor);
        m_bDragging = false; 
        
        if (m_hDropTarget)
        {
            TV_ITEM sItem;
            PlaylistItem *item;
            
            sItem.hItem = m_hDropTarget;
            sItem.mask = TVIF_PARAM;
            TreeView_GetItem(m_hMusicCatalog, &sItem);

            assert(sItem.lParam < 0);
            
            i = -sItem.lParam;
                 
            if (i != 1)
            {
                const vector<string> *p;
                
                p = m_context->browser->m_catalog->GetPlaylists();
                LoadPlaylist((*p)[(-sItem.lParam) - 1]);
            }
            else
                LoadPlaylist(string(""));

            item = new PlaylistItem(*m_oTreeIndex.Data(m_hTreeDragItem.lParam).m_pTrack);
            
            m_oPlm->AddItem(item, false);
            UpdatePlaylistList();
            m_bListChanged = true;
                     
            SetFocus(m_hPlaylistView);
        }    
    }    */
}

void MusicBrowserUI::UpdateButtonStates()
{
    HMENU hMenu;
    
    hMenu = GetMenu(m_hWnd);
    hMenu = GetSubMenu(hMenu, 1);
    
    EnableWindow(GetDlgItem(m_hWnd, IDC_DELETE), m_currentindex != -1);

    EnableWindow(GetDlgItem(m_hWnd, IDC_UP), m_currentindex > 0);
    EnableMenuItem(hMenu, ID_EDIT_MOVEUP, 
                   (m_currentindex > 0) ? MF_ENABLED : MF_GRAYED);
    
    EnableWindow(GetDlgItem(m_hWnd, IDC_DOWN), 
                 m_currentindex < m_oPlm->CountItems() - 1);
    EnableMenuItem(hMenu, ID_EDIT_MOVEDOWN, 
                 (m_currentindex < m_oPlm->CountItems() - 1) ? 
                 MF_ENABLED : MF_GRAYED);

}

void MusicBrowserUI::UpdateMenuItems(void)
{
    HMENU        hMenu;
    MENUITEMINFO sMenuItem;
    string       oType(""), oText;
    int32        lParam;
    HTREEITEM    hDummy;

    lParam = GetMusicTreeSelection(hDummy);
    if (m_oTreeIndex.IsTrack(lParam))
       oType = " Track";
       
    if (m_oTreeIndex.IsPlaylist(lParam))
       oType = " Playlist";

    hMenu = GetMenu(m_hWnd);
    hMenu = GetSubMenu(hMenu, 1);
 
    sMenuItem.cbSize = sizeof(MENUITEMINFO);
    sMenuItem.fMask = MIIM_TYPE | MIIM_STATE;
    sMenuItem.fType = MFT_STRING;
    sMenuItem.fState = (oType.length() == 0) ? MFS_GRAYED : MFS_ENABLED;

    oText = string("&Edit") + oType + string("...");
    sMenuItem.dwTypeData = (char *)oText.c_str();
    sMenuItem.cch = strlen(sMenuItem.dwTypeData);
    SetMenuItemInfo(hMenu, ID_EDIT_EDIT, false, &sMenuItem);

    oText = string("&Remove") + oType + string(" from Catalog...");
    sMenuItem.dwTypeData = (char *)oText.c_str();
    sMenuItem.cch = strlen(sMenuItem.dwTypeData);
    SetMenuItemInfo(hMenu, ID_EDIT_REMOVE, false, &sMenuItem);
    
    oText = string("Remove") + oType + string(" from &Disk...");
    sMenuItem.dwTypeData = (char *)oText.c_str();
    sMenuItem.cch = strlen(sMenuItem.dwTypeData);
    SetMenuItemInfo(hMenu, ID_EDIT_REMOVEFROMDISK, false, &sMenuItem);
    
    SendMessage(m_hToolbar, TB_ENABLEBUTTON, ID_EDIT_EDIT, 
                MAKELONG(oType.length() != 0, 0));    
    SendMessage(m_hToolbar, TB_ENABLEBUTTON, ID_EDIT_REMOVE, 
                MAKELONG(oType.length() != 0, 0));    
}




static
UINT 
APIENTRY 
OpenFileHookProc(   HWND hwnd, 
                    UINT msg, 
                    WPARAM wParam, 
                    LPARAM lParam)
{
    UINT result = 0;
    static OPENFILENAME* ofn = NULL;

    switch(msg)
    {
        case WM_INITDIALOG:
        {
            ofn = (OPENFILENAME*)lParam;

            break;
        }

        case WM_COMMAND:
        {
            switch(wParam)
            {
                case IDC_OPEN_URL:
                {
                    char* url = new char[ofn->nMaxFile + 1];

                    if(GetDlgItemText(  hwnd,
                                        IDC_URL,
                                        url,
                                        ofn->nMaxFile))
                    {
                        *ofn->lpstrFile = 0x00;

                        if(!strstr(url, "://"))
                        {
                            strcpy(ofn->lpstrFile, "http://");
                        }

                        strcat(ofn->lpstrFile, url);

                        PostMessage(GetParent(hwnd), 
                                    WM_COMMAND, 
                                    IDCANCEL,
                                    0);

                        ofn->lCustData = 1;
                    }

                    delete url;
                    
                    result = 1;
                    break;
                }
            }

            break;
        }

        case WM_NOTIFY:
        {
            OFNOTIFY* notify = (OFNOTIFY*)lParam;

            switch(notify->hdr.code)
            {
                case CDN_FILEOK:
                {
                    
                    break;
                }                
            }

            break;
        }
    }

    return result;
}

bool 
FileOpenDialog(HWND hwnd, 
               const char* title,
               const char* filter,
               vector<char*>* fileList,
               Preferences* prefs)
{
    bool result = false;
    OPENFILENAME ofn;
    char szInitialDir[MAX_PATH + 1] = {0x00};
    uint32 initialDirSize = sizeof(szInitialDir);
    const int32 kBufferSize = MAX_PATH * 128;
    char* fileBuffer = new char[kBufferSize];

    *fileBuffer = 0x00;

    if(prefs)
    {
        prefs->GetOpenSaveDirectory( szInitialDir, &initialDirSize);
    }

    // Setup open file dialog box structure
    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
    ofn.hInstance         = (HINSTANCE)GetWindowLong(hwnd, 
                                                     GWL_HINSTANCE);
    ofn.lpstrFilter       = filter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = fileBuffer;
    ofn.nMaxFile          = kBufferSize;
    ofn.lpstrFileTitle    = NULL;
    ofn.nMaxFileTitle     = 0;
    ofn.lpstrInitialDir   = szInitialDir;
    ofn.lpstrTitle        = title;
    ofn.Flags             = OFN_FILEMUSTEXIST | 
					        OFN_PATHMUSTEXIST |
  	     			        OFN_HIDEREADONLY | 
					        OFN_ALLOWMULTISELECT |
					        OFN_EXPLORER |
                            OFN_ENABLEHOOK |
                            OFN_ENABLETEMPLATE;
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = "MP3";
    ofn.lCustData         = 0;
    ofn.lpfnHook          = OpenFileHookProc;
    ofn.lpTemplateName    = MAKEINTRESOURCE(IDD_OPENURL);

    if(GetOpenFileName(&ofn) || ofn.lCustData)
    {
        char file[MAX_PATH + 1];
        char* cp = NULL;

        if(ofn.lCustData) // URL
        {
            char* foo = new char[strlen(fileBuffer) + 1];

            strcpy(foo, fileBuffer);

            fileList->push_back(foo);
        }
          else // potential list of files
        {
            strncpy(file, fileBuffer, ofn.nFileOffset);

            if(*(fileBuffer + ofn.nFileOffset - 1) != DIR_MARKER)
                strcat(file, DIR_MARKER_STR);

            cp = fileBuffer + ofn.nFileOffset;

            while(*cp)
            {
	            strcpy(file + ofn.nFileOffset, cp);

                char* foo = new char[strlen(file) + 8];

                strcpy(foo, "file://");
                strcat(foo, file);

                fileList->push_back(foo);

	            cp += strlen(cp) + 1;
            }

            *(fileBuffer + ofn.nFileOffset - 1) = 0x00;

            if(prefs)
            {
                prefs->SetOpenSaveDirectory(fileBuffer);
            }
        }

        result = true;
    }

    delete [] fileBuffer;

    return result;
}

void ClientToWindow(HWND hWnd, POINT *Pt) 
{
    RECT wRect, cRect;
 
    GetWindowRect(hWnd, &wRect);
    GetClientRect(hWnd, &cRect);
    MapWindowPoints(hWnd, HWND_DESKTOP, (LPPOINT)&cRect, 2);
    Pt->x += (wRect.left - cRect.left);
    Pt->y += (wRect.top - cRect.top);
}

uint32 MusicBrowserUI::CalcStringEllipsis(HDC hdc, string& displayString, int32 columnWidth)
{
    const TCHAR szEllipsis[] = TEXT("...");
    SIZE   sizeString;
    SIZE   sizeEllipsis;
    string temp;
    
    // Adjust the column width to take into account the edges
    //columnWidth -= 4;

    temp = displayString;        

    GetTextExtentPoint32(hdc, temp.c_str(), temp.size(), &sizeString);       

    // If the width of the string is greater than the column width shave
    // the string and add the ellipsis
    if(sizeString.cx > columnWidth)
    {
        GetTextExtentPoint32(hdc, szEllipsis, strlen(szEllipsis), &sizeEllipsis);
       
        while(temp.size() > 1)
        {
            temp.erase(temp.size() - 1, 1);

            GetTextExtentPoint32(hdc, temp.c_str(), temp.size(), &sizeString);
            
            if ((uint32)(sizeString.cx + sizeEllipsis.cx) <= columnWidth)
            {
                // The string with the ellipsis finally fits                
                // Concatenate the two strings and break out of the loop
                temp += szEllipsis;
                displayString = temp;
                break;
            }
            else if(temp.size() == 1)
            {
                temp += szEllipsis;
                displayString = temp;
                break;
            }
        }
    }

    GetTextExtentPoint32(hdc, displayString.c_str(), displayString.size(), &sizeString);

    return sizeString.cx;
}
