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
	{ 7, ID_FILE_SEARCHFORMUSIC, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0},
	{ 0, ID_FILE_NEWPLAYLIST, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 1, ID_FILE_IMPORT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 3, ID_EDIT_REMOVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0},
	{ 4, ID_EDIT_EDIT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0L, 0}
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
    switch (msg)
    {
		case WM_CLOSE:
        {
			Close();
            return 1;
        }    

		case WM_SIZE:
        {
            SizeWindow(LOWORD(lParam), HIWORD(lParam));
            return 1;
        } 
		case WM_NOTIFY:
            return Notify(wParam, (LPNMHDR)lParam);
            
//		case WM_GETMINMAXINFO:
//            GetMinMaxInfo((MINMAXINFO *)lParam);
//            break;

        case WM_MOUSEMOVE:
        {
            POINT sPoint;   
            sPoint.x = LOWORD(lParam);
            sPoint.y = HIWORD(lParam);  
            MouseMove((uint32)wParam, sPoint);
            return 1;
        }
        case WM_LBUTTONUP:
            MouseButtonUp();
            return 1;
            
        case WM_EMPTYDBCHECK:
            EmptyDBCheck();
            return 1;
            
        case WM_COMMAND:
        {
        	switch(LOWORD(wParam))
            {
                case ID_FILE_SEARCHFORMUSIC:
                case IDC_SEARCH:
                   StartMusicSearch();
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
            if (mis->CtlType != ODT_LISTVIEW || mis->CtlID == IDC_PLAYLISTBOX)
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
                      GetDlgItem(m_hWnd, IDC_MUSICTREE),
                          NULL, TVSIL_NORMAL)); 
    ImageList_Destroy(ListView_SetImageList(
                      GetDlgItem(m_hWnd, IDC_PLAYLISTBOX),
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
    RECT         sWindow;
    HMENU        hMenu;
    MENUITEMINFO sItem;
    
    GetWindowRect(m_hWnd, &sWindow);
    if (m_state == STATE_COLLAPSED)
    {
       m_state = STATE_EXPANDED;
       SetWindowText(m_hWnd, BRANDING " - Music Browser");
       sItem.dwTypeData = "View &playlist only";
       
       SetWindowPos(m_hWnd, NULL, 0, 0, 
        			(sWindow.right - sWindow.left) + m_iCollapseMoveAmount,
                    sWindow.bottom - sWindow.top,
                    SWP_NOMOVE);
       
	   MoveControls(m_iCollapseMoveAmount);
    }            
    else
    {                
       m_state = STATE_COLLAPSED;
       SetWindowText(m_hWnd, BRANDING " - Playlist Manager");
       sItem.dwTypeData = "View &music browser";
       
       SetWindowPos(m_hWnd, NULL, 0, 0, 
        			(sWindow.right - sWindow.left) - m_iCollapseMoveAmount,
                    sWindow.bottom - sWindow.top,
                    SWP_NOMOVE);
       
	   MoveControls(-m_iCollapseMoveAmount);
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

void MusicBrowserUI::SizeWindow(int iWidth, int iHeight)
{
}

void MusicBrowserUI::GetMinMaxInfo(MINMAXINFO *pInfo)
{
    pInfo->ptMaxSize.x = 10000; 
    pInfo->ptMaxSize.y = 10000; 
    pInfo->ptMaxPosition.x = 10000; 
    pInfo->ptMaxPosition.y = 10000; 
    pInfo->ptMinTrackSize = m_sMinSize;
    pInfo->ptMaxTrackSize.x = 10000; 
    pInfo->ptMaxTrackSize.y = 10000; 
}

void MusicBrowserUI::SetMinMaxInfo(void)
{
	RECT  sLoc, sLoc2;
    
	GetWindowRect(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sLoc);
	GetWindowRect(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sLoc2);
    m_iCollapseMoveAmount = sLoc2.left - sLoc.left;
}

void MusicBrowserUI::InitDialog(HWND hWnd)
{
    HIMAGELIST      hList;
    HMODULE         hShell;
    DWORD           dwIcon;
    const int32     kNumPanes = 1;
    int32           panes[kNumPanes]= {-1};

    m_hWnd = hWnd;
    hShell = GetModuleHandle("SHELL32.DLL");
    
    hList = ImageList_Create(16, 16, ILC_COLOR24|ILC_MASK, 4, 0);
    dwIcon = 4;     // 'Closed folder' Icon
    ImageList_AddIcon(hList, LoadIcon(hShell, MAKEINTRESOURCE(dwIcon)));
    dwIcon = 5; 	   // 'Open folder' icon
    ImageList_AddIcon(hList, LoadIcon(hShell, MAKEINTRESOURCE(dwIcon)));
    dwIcon = 152; 	   // 'File folder' icon (hopefully)
    ImageList_AddIcon(hList, LoadIcon(hShell, MAKEINTRESOURCE(dwIcon)));
    ImageList_AddIcon(hList, LoadIcon(g_hinst, MAKEINTRESOURCE(IDI_ACTIVELIST)));
 
    TreeView_SetImageList(GetDlgItem(m_hWnd, IDC_MUSICTREE),
                          hList, TVSIL_NORMAL); 
	FreeLibrary(hShell);
    
    hList = ImageList_Create(16, 16, ILC_COLOR24|ILC_MASK, 1, 0);

    HBITMAP bmp = LoadBitmap(g_hinst, MAKEINTRESOURCE(IDB_PLAYING));
    ImageList_AddMasked(hList, bmp, RGB(255,0,0));
    DeleteObject(bmp);
 
    ListView_SetImageList(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX),
                          hList, LVSIL_SMALL); 

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
    
    m_hStatus= CreateStatusWindow(WS_CHILD | WS_VISIBLE,
                                  "", m_hWnd, IDC_STATUS);

    SendMessage(m_hStatus, SB_SETPARTS, kNumPanes, (LPARAM) panes);
    m_currentindex = -1;
    UpdateButtonStates();
    
    if (m_pParent)
        ShowWindow(m_hWnd, SW_SHOW);
}

void MusicBrowserUI::CreateToolbar(void)
{
	m_hToolbar = CreateToolbarEx(m_hWnd, 
                     WS_CHILD | WS_BORDER | WS_VISIBLE | 
                     TBSTYLE_TOOLTIPS | CCS_ADJUSTABLE,
                     0, 8, g_hinst, IDB_TOOLBAR,
		             (LPCTBBUTTON)&tbButtons,
		             6, 16,16, 16,16, sizeof(TBBUTTON));
}

void MusicBrowserUI::SetTitles(void)
{
    if (m_pParent == NULL)
    {
       SetWindowText(GetDlgItem(m_hWnd, IDC_PLAYLISTTITLE), 
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
       SetWindowText(GetDlgItem(m_hWnd, IDC_PLAYLISTTITLE), 
                     oTitle.c_str());
       oTitle = string(BRANDING " - Editing ") + oName;
       SetWindowText(m_hWnd, oTitle.c_str());
    }   
}

void MusicBrowserUI::BeginDrag(NM_TREEVIEW *pTreeView)
{
    POINT      sPoint;
    
    m_hTreeDragItem = pTreeView->itemNew;
    sPoint = pTreeView->ptDrag;

    m_hDragCursor = LoadCursor(g_hinst, MAKEINTRESOURCE(IDC_DRAG));
    m_hNoDropCursor = LoadCursor(g_hinst, MAKEINTRESOURCE(IDC_NODROP));
    m_hSavedCursor = SetCursor(m_hDragCursor);
    
    SetCapture(m_hWnd);
    m_bDragging = true;
}

void MusicBrowserUI::MouseMove(uint32 uFlags, POINT &sPoint)
{
    if (m_bDragging)
    {
        TV_HITTESTINFO sHit;
        
        sHit.flags = TVHT_ONITEM;
        
        ClientToWindow(m_hWnd, &sPoint); 
        sHit.pt = sPoint;
        m_hDropTarget = TreeView_HitTest(GetDlgItem(m_hWnd, IDC_MUSICTREE),
                                         &sHit);
        if (m_hDropTarget != NULL)
        {
            TV_ITEM sItem;
            
            sItem.hItem = m_hDropTarget;
            sItem.mask = TVIF_PARAM;
            TreeView_GetItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sItem);
            if (sItem.lParam < 0)
               SetCursor(m_hDragCursor);
            else    
            {
               SetCursor(m_hNoDropCursor);
               m_hDropTarget = NULL;
            }
        }       
        else    
            SetCursor(m_hNoDropCursor);
    }
} 

void MusicBrowserUI::MouseButtonUp(void)
{
    int i;
    
    if (m_bDragging)
    {
        ReleaseCapture();
        SetCursor(m_hSavedCursor);
        m_bDragging = false; 
        
        if (m_hDropTarget)
        {
            TV_ITEM sItem;
            PlaylistItem *item;
            
            sItem.hItem = m_hDropTarget;
            sItem.mask = TVIF_PARAM;
            TreeView_GetItem(GetDlgItem(m_hWnd, IDC_MUSICTREE), &sItem);

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
                     
            SetFocus(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX));
        }    
    }    
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
