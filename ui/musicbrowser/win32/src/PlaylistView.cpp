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

void MusicBrowserUI::NewPlaylist(void)
{
    MusicBrowserUI *pNew;
    
    if (m_pParent)
    {
       pNew = new MusicBrowserUI(m_context, m_pParent, m_hWnd, string(""));
       m_pParent->AddMusicBrowserWindow(pNew);
    }   
    else   
    {
       pNew = new MusicBrowserUI(m_context, this, m_hWnd, string(""));
       AddMusicBrowserWindow(pNew);
    }   
       
    pNew->Init(SECONDARY_UI_STARTUP);
}

void MusicBrowserUI::EditPlaylist(const string &oList)
{
    MusicBrowserUI *pNew;
    
    if (m_pParent)
    {
       pNew = new MusicBrowserUI(m_context, m_pParent, m_hWnd, oList);
       m_pParent->AddMusicBrowserWindow(pNew);
    }   
    else   
    {
       pNew = new MusicBrowserUI(m_context, this, m_hWnd, oList);
       AddMusicBrowserWindow(pNew);
    }   
       
    pNew->Init(SECONDARY_UI_STARTUP);
}


#define kPrePadding 5

BOOL MusicBrowserUI::DrawItem(int32 controlId, DRAWITEMSTRUCT* dis)
{
    BOOL result = TRUE;

    switch(controlId)
    {
        case IDC_PLAYLISTBOX:
        {
            uint32 uiFlags = ILD_TRANSPARENT;
            RECT rcClip;
            HIMAGELIST himl;
            HFONT boldFont = NULL, oldFont = NULL;
            int32 cxImage = 0, cyImage = 0;

            // Get Image List
            himl = ListView_GetImageList(dis->hwndItem, LVSIL_SMALL);
            ImageList_GetIconSize(himl, &cxImage, &cyImage);

            rcClip = dis->rcItem;   
        
            HWND hwndList = GetDlgItem(m_hWnd, IDC_PLAYLISTBOX);
            LV_ITEM lv_item;
            char* buf = new char[1024]; // MS is so dumb they do not give you a way to query text length
            string displayString;

            lv_item.mask = LVIF_TEXT;
            lv_item.state = 0;
            lv_item.stateMask = 0;
            lv_item.iItem = dis->itemID;
            lv_item.iSubItem = 0;
            lv_item.pszText = buf;
            lv_item.cchTextMax = 2;
            lv_item.lParam = NULL;

            // is this the current index? if so make it bold ...
            if(dis->itemID == m_oPlm->GetCurrentIndex())
            {
                LOGFONT lf;

                GetObject(GetWindowFont(hwndList), sizeof(LOGFONT), &lf);

                lf.lfWeight = FW_BOLD;

                boldFont = CreateFontIndirect(&lf);

                oldFont = (HFONT)SelectObject(dis->hDC, boldFont);
            }

            // Item index
            //ListView_GetItemText(hwndList, dis->itemID, 0, buf, 1024);
            sprintf(buf, "%d", dis->itemID + 1);
            displayString = buf;

            
            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 0) /*- (cxImage + 1)*/);

            UINT oldAlign;

            oldAlign = SetTextAlign(dis->hDC, TA_CENTER | TA_TOP );

            UINT left = rcClip.left + (ListView_GetColumnWidth(hwndList, 0)/2);
            RECT indexColumnRect;

            GetClientRect(hwndList, &indexColumnRect);
            indexColumnRect.top = rcClip.top;
            indexColumnRect.right = indexColumnRect.left + ListView_GetColumnWidth(hwndList, 0);
            
            SetTextColor(dis->hDC, GetSysColor(COLOR_INFOTEXT));
            SetBkColor(dis->hDC, GetSysColor(COLOR_INFOBK ));

            ExtTextOut( dis->hDC, 
                        left, rcClip.top + 1,
                        ETO_CLIPPED | ETO_OPAQUE,
                        &indexColumnRect, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            SetTextAlign(dis->hDC, oldAlign);

            /*ExtTextOut( dis->hDC, 
                        rcClip.left + cxImage + 2, rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);
            
            // Draw the icon. Drawing it after the first item allows us
            // to let ExtTextOut draw the correct background 
            if(himl && dis->itemID == m_oPlm->GetCurrentIndex())
            {
                uint32 top = rcClip.top + ((rcClip.bottom - rcClip.top) - cyImage)/2;
                ImageList_Draw( himl, 0, dis->hDC, 
                                rcClip.left, top,
                                uiFlags);
            }*/

            // Move over to the next column
            rcClip.left += ListView_GetColumnWidth(hwndList, 0);

            // Check to see if this item is selected
            if(dis->itemState & ODS_SELECTED)
            {
                // Set the text background and foreground colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_HIGHLIGHT));

		        // Also add the ILD_BLEND50 so the images come out selected
		        //uiFlags |= ILD_BLEND50;
            }
            else
            {
                // Set the text background and foreground colors to the
                // standard window colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_WINDOWTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_WINDOW));
            }

            

            // Title
            ListView_GetItemText(hwndList, dis->itemID, 1, buf, 1024);
            displayString = buf;

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 1) - kPrePadding);

            ExtTextOut( dis->hDC, 
                        rcClip.left + kPrePadding, rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);            

            // Move over to the next column
            rcClip.left += ListView_GetColumnWidth(hwndList, 1);

            // Artist
            ListView_GetItemText(hwndList, dis->itemID, 2, buf, 1024);
            displayString = buf;

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 2) - kPrePadding);

            ExtTextOut( dis->hDC, 
                        rcClip.left + kPrePadding, rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            // Move over to the next column
            rcClip.left += ListView_GetColumnWidth(hwndList, 2);

            // Album
            ListView_GetItemText(hwndList, dis->itemID, 3, buf, 1024);

            displayString = buf;

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 3) - kPrePadding);

            ExtTextOut( dis->hDC, 
                        rcClip.left + kPrePadding, rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            // Move over to the next column
            rcClip.left += ListView_GetColumnWidth(hwndList, 3);

            // Length
            ListView_GetItemText(hwndList, dis->itemID, 4, buf, 1024);

            displayString = buf;

            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 4) - kPrePadding);

            ExtTextOut( dis->hDC, 
                        rcClip.left + kPrePadding, rcClip.top + 1, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            // Move over to the next column
            rcClip.left += ListView_GetColumnWidth(hwndList, 4);

            // If we changed font undo it
            if(dis->itemID == m_oPlm->GetCurrentIndex())
            {
                SelectObject(dis->hDC, oldFont);
                DeleteObject(boldFont);
            }

            // If we changed the colors for the selected item, undo it
            if(dis->itemState & ODS_SELECTED)
            {
                // Set the text background and foreground colors
                SetTextColor(dis->hDC, GetSysColor(COLOR_WINDOWTEXT));
                SetBkColor(dis->hDC, GetSysColor(COLOR_WINDOW));
            }

            // If the item is focused draw a focus rect around the entire row
            if(dis->itemState & ODS_FOCUS)
            {
                // Draw the focus rect
                DrawFocusRect(dis->hDC, &dis->rcItem);
            }

            break;
        }


    }

    return result;
}

#define LENGTH_COLUMN_WIDTH 60
#define INDEX_COLUMN_WIDTH 25
#define FIXED_COLUMN_WIDTH (LENGTH_COLUMN_WIDTH + INDEX_COLUMN_WIDTH)

void MusicBrowserUI::InitList(void)
{
    LV_COLUMN lvc;
    RECT      sRect;
    
    ListView_DeleteAllItems(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX));
    GetClientRect(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sRect);

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.fmt = LVCFMT_LEFT; // left align column

    lvc.pszText = "#";
    lvc.cchTextMax = strlen(lvc.pszText);
    lvc.iSubItem = 0;
    lvc.cx = INDEX_COLUMN_WIDTH; // width of column in pixels
    ListView_InsertColumn(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), 0, &lvc);

    int32 remainder = (sRect.right-sRect.left - FIXED_COLUMN_WIDTH)%3;

    lvc.pszText = "Title";
    lvc.cchTextMax = strlen(lvc.pszText);
    lvc.iSubItem = 1;
    lvc.cx = (sRect.right-sRect.left - FIXED_COLUMN_WIDTH)/3; // width of column in pixels
    ListView_InsertColumn(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), 1, &lvc);
    
    lvc.pszText = "Artist";
    lvc.cchTextMax = strlen(lvc.pszText);
    lvc.iSubItem = 2;
    ListView_InsertColumn(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), 2, &lvc);

    lvc.pszText = "Album";
    lvc.cchTextMax = strlen(lvc.pszText);
    lvc.iSubItem = 3;
    ListView_InsertColumn(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), 3, &lvc);

    
    lvc.pszText = "Length";
    lvc.cx = LENGTH_COLUMN_WIDTH + remainder;//((sRect.right-sRect.left)/4) - 3; // width of column in pixels
    lvc.cchTextMax = strlen(lvc.pszText);
    lvc.iSubItem = 4;
    ListView_InsertColumn(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), 4, &lvc);
}


void MusicBrowserUI::AddPlaylist(const string &oName)
{
    vector<PlaylistItem *>            oList;
    vector<PlaylistItem *>::iterator  j;
    int                               i;
    char                              url[MAX_PATH];
    uint32                            len = MAX_PATH;

    FilePathToURL(oName.c_str(), url, &len);
    m_oPlm->SetActivePlaylist(kPlaylistKey_ExternalPlaylist);
    m_oPlm->SetExternalPlaylist(url);
    for(i = 0; i < m_oPlm->CountItems(); i++)
        oList.push_back(m_oPlm->ItemAt(i));

    m_oPlm->SetActivePlaylist(kPlaylistKey_MasterPlaylist);
    for(j = oList.begin(); j != oList.end(); j++)
        m_oPlm->AddItem(new PlaylistItem(*(*j)), true);

    m_bListChanged = true;

    UpdatePlaylistList();
}

void MusicBrowserUI::LoadPlaylist(const string &oPlaylist)
{
    m_oPlm->RemoveAll();
    m_oPlm->ReadPlaylist((char *)oPlaylist.c_str());
       
    m_currentindex = 0;
    UpdatePlaylistList();
}

void MusicBrowserUI::WritePlaylist(void)
{
    PlaylistFormatInfo     oInfo;              
    char                   ext[MAX_PATH];
    Error                  eRet = kError_NoErr;
    vector<PlaylistItem *> oTempList;
    int                    i;
    bool                   bNewList = false;

    if (!m_bListChanged)
        return;

    if (!m_pParent)
    {
//        string lastPlaylist = FreeampDir(m_context->prefs);
//        lastPlaylist += "\\currentlist.m3u";
    
//        SaveCurrentPlaylist((char *)lastPlaylist.c_str());  

        m_bListChanged = false;
        return;
    }
  
    Debug_v("items: %d", m_oPlm->CountItems());
    if (m_currentListName.length() == 0)
    {
        if (SaveNewPlaylist(m_currentListName))
           bNewList = true;
        else
        {
           return;
        }   
        Debug_v("items: %d", m_oPlm->CountItems());
        SetTitles();
        Debug_v("items: %d", m_oPlm->CountItems());
    }

    Debug_v("items: %d", m_oPlm->CountItems());
    _splitpath(m_currentListName.c_str(), NULL, NULL, NULL, ext);
    for(i = 0; ; i++)
    {
        eRet = m_oPlm->GetSupportedPlaylistFormats(&oInfo, i);
        if (IsError(eRet))
           break;

        if (strcasecmp(oInfo.GetExtension(), ext + 1) == 0)
           break;   
    }
    Debug_v("items: %d", m_oPlm->CountItems());
    if (!IsError(eRet))
    {
        char   url[MAX_PATH];
        uint32 len = MAX_PATH;
        
        FilePathToURL(m_currentListName.c_str(), url, &len);
        Debug_v("items: %d", m_oPlm->CountItems());
        Debug_v("Writing playlist to: %s", url);
        eRet = m_oPlm->WritePlaylist(url, &oInfo);   
        if (IsError(eRet))
        {
           MessageBox(m_hWnd, "Cannot save playlist to disk. Out of disk space?", BRANDING, MB_OK);                              
           return;
        }
    }
    if (bNewList)
    {
        m_context->browser->m_catalog->AddPlaylist(m_currentListName.c_str());
    }

    m_bListChanged = false;
}

void MusicBrowserUI::SaveAsPlaylist(void)
{
    string oName;
    
    if (SaveNewPlaylist(oName))
    {
       m_currentListName = oName;
       SetTitles();
       WritePlaylist();
    }   
}

void MusicBrowserUI::UpdatePlaylistList(void)
{
    LV_ITEM       sItem;
    int           i;
    PlaylistItem *pItem;
    char          szText[100];

    ListView_DeleteAllItems(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX));
    sItem.state = sItem.stateMask = 0;
    for(i = 0; pItem = m_oPlm->ItemAt(i); i++)
    {
        sprintf(szText, "%d", i + 1);

        sItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
        sItem.pszText = szText;
        sItem.cchTextMax = strlen(szText);
        sItem.iSubItem = 0;
        sItem.iItem = i;
        sItem.lParam = i;
        sItem.iImage = 0;

        ListView_InsertItem(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sItem);

        sItem.mask = LVIF_TEXT;
        sItem.pszText = (char *)pItem->GetMetaData().Title().c_str();
        sItem.cchTextMax = pItem->GetMetaData().Title().length();
        sItem.iSubItem = 1;

        ListView_SetItem(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sItem);

        sItem.mask = LVIF_TEXT;
        sItem.pszText = (char *)pItem->GetMetaData().Artist().c_str();
        sItem.cchTextMax = pItem->GetMetaData().Artist().length();
        sItem.iSubItem = 2;
        ListView_SetItem(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sItem);

        sItem.mask = LVIF_TEXT;
        sItem.pszText = (char *)pItem->GetMetaData().Album().c_str();
        sItem.cchTextMax = pItem->GetMetaData().Album().length();
        sItem.iSubItem = 3;
        ListView_SetItem(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sItem);

        if (pItem->GetMetaData().Time() != 0)
        {
           int iTime = pItem->GetMetaData().Time();
           if (iTime > 3600)
              sprintf(szText, "%d:%02d:%02d", iTime / 3600, 
                                          (iTime % 3600) / 60,
                                          iTime % 60);
           else   
               sprintf(szText, "%d:%02d", (iTime % 3600) / 60,
                                        iTime % 60);
        }      
        else    
            strcpy(szText, "Unknown");
        sItem.pszText = szText;
        sItem.cchTextMax = strlen(szText);
        sItem.iSubItem = 4;
        ListView_SetItem(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), &sItem);
    }

    if (m_currentindex >= i)
       m_currentindex = i - 1;
        
    ListView_SetItemState(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX), 
                    m_currentindex, 
                    LVIS_FOCUSED|LVIS_SELECTED,
                    LVIS_FOCUSED|LVIS_SELECTED);
    SetFocus(GetDlgItem(m_hWnd, IDC_PLAYLISTBOX));
    UpdateButtonStates();
}

bool MusicBrowserUI::SaveNewPlaylist(string &oName)
{
    int32              i, iOffset = 0;
    uint32             size;
    PlaylistFormatInfo format;
    char               szFilter[512];
    OPENFILENAME       sOpen;
    char               szFile[MAX_PATH], szInitialDir[MAX_PATH];
        
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

    size = MAX_PATH;
    if (!IsError(m_context->prefs->GetInstallDirectory(szInitialDir, &size)))
    {
        struct _stat buf;
        
        strcat(szInitialDir, "\\Playlists");
        if (_stat(szInitialDir, &buf) != 0)
           _mkdir(szInitialDir);
           
        sOpen.lpstrInitialDir = szInitialDir;
    }
    else
        sOpen.lpstrInitialDir = NULL;
    
    szFile[0] = 0;
    sOpen.lStructSize = sizeof(OPENFILENAME);
    sOpen.hwndOwner = m_hWnd;
    sOpen.hInstance = NULL;
    sOpen.lpstrFilter = szFilter;
    sOpen.lpstrCustomFilter = NULL;
    sOpen.nMaxCustFilter = 0;
    sOpen.nFilterIndex = 1;
    sOpen.lpstrFile = szFile;
    sOpen.nMaxFile = MAX_PATH;
    sOpen.lpstrFileTitle = NULL;
    sOpen.lpstrTitle = "Save Playlist as";
    sOpen.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY |
                  OFN_PATHMUSTEXIST;
    sOpen.lpstrDefExt = "m3u";
      
    if (GetSaveFileName(&sOpen))
    {
        oName = sOpen.lpstrFile;
        return true;
    }
    return false;
}

void MusicBrowserUI::OpenPlaylist(void)
{
    int32              i, iOffset = 0;
    PlaylistFormatInfo format;
    char               szFilter[512];
    OPENFILENAME       sOpen;
    char               szFile[MAX_PATH];
        
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
    
    szFile[0] = 0;
    sOpen.lStructSize = sizeof(OPENFILENAME);
    sOpen.hwndOwner = m_hWnd;
    sOpen.hInstance = NULL;
    sOpen.lpstrFilter = szFilter;
    sOpen.lpstrCustomFilter = NULL;
    sOpen.nMaxCustFilter = 0;
    sOpen.nFilterIndex = 1;
    sOpen.lpstrFile = szFile;
    sOpen.nMaxFile = MAX_PATH;
    sOpen.lpstrFileTitle = NULL;
    sOpen.lpstrInitialDir = NULL;
    sOpen.lpstrTitle = "Open Playlist";
    sOpen.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    sOpen.lpstrDefExt = "m3u";
      
    if (GetOpenFileName(&sOpen))
    {
        string playlist = sOpen.lpstrFile;
        
        m_context->browser->m_catalog->AddPlaylist(sOpen.lpstrFile);
        InitTree();
        //FillPlaylistCombo();
        LoadPlaylist(playlist);
    }
}

LRESULT WINAPI 
ListViewWndProc(HWND hwnd, 
                UINT msg, 
                WPARAM wParam, 
                LPARAM lParam)
{
    WNDPROC lpOldProc = (WNDPROC)GetProp( hwnd, "oldproc" );

	switch(msg)
	{
		case WM_DESTROY:   
		{
			//  Put back old window proc and
			SetWindowLong( hwnd, GWL_WNDPROC, (DWORD)lpOldProc );

			// remove window property
			RemoveProp( hwnd, "oldproc" ); 

			break;
		}

        case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*) lParam;

            if(dis->CtlType == ODT_HEADER)
            {
                RECT rcClip = rcClip = dis->rcItem;
                UINT oldAlign;

                oldAlign = SetTextAlign(dis->hDC, TA_CENTER | TA_TOP );

                UINT left = rcClip.left + ((rcClip.right - rcClip.left)/2);
                UINT top = rcClip.top + 2; // + ((rcClip.bottom - rcClip.top)/2);

                ExtTextOut( dis->hDC,left, top, 
                        ETO_CLIPPED | ETO_OPAQUE,
                        &rcClip, 
                        "#",
                        strlen("#"),
                        NULL);

                SetTextAlign(dis->hDC, oldAlign);
            }

            break;
        }

        case WM_ERASEBKGND:
        {
            HDC hdc = (HDC) wParam;

            //HWND hwndHeader = FindWindowEx(hwnd, NULL, WC_HEADER, NULL);

            //if(hwndHeader /* && !ListView_GetItemCount(hwnd) */)
            {
                //RECT headerRect;
                
                //GetWindowRect(hwndHeader, &headerRect);

                RECT rectClient, rectColumn;

                GetClientRect(hwnd, &rectClient);
                rectColumn = rectClient;

                //rectColumn.top += (headerRect.bottom - headerRect.top);
                rectColumn.right = rectColumn.left + ListView_GetColumnWidth(hwnd, 0);
                rectClient.left = rectColumn.right;

                FillRect(hdc, &rectClient, (HBRUSH)GetClassLong(hwnd, GCL_HBRBACKGROUND));

                FillRect(hdc, &rectColumn, (HBRUSH)(COLOR_INFOBK + 1));
            }

            return TRUE;
            break;
        }
#if 0
        case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hwnd, &ps);

            LRESULT result = CallWindowProc((int (__stdcall *)(void))lpOldProc, hwnd, msg, (WPARAM)hdc, lParam );
            
            HWND hwndHeader = FindWindowEx(hwnd, NULL, WC_HEADER, NULL);

            if(hwndHeader /* && !ListView_GetItemCount(hwnd) */)
            {
                RECT headerRect;
                
                GetWindowRect(hwndHeader, &headerRect);
        
                //HDC hdc;

                hdc = GetDC(hwnd);

                RECT rect;

                GetClientRect(hwnd, &rect);

                rect.top += (headerRect.bottom - headerRect.top);
                rect.right = rect.left + ListView_GetColumnWidth(hwnd, 0);
            
                FillRect(hdc, &rect, (HBRUSH)(COLOR_INFOBK + 1));

                //ReleaseDC(hwnd, hdc);
               
            }

            EndPaint(hwnd, &ps);
            
            return result;
            break;
        }
#endif
        case WM_NOTIFY:
        {
            int idCtrl = wParam; 
            HD_NOTIFY* hdn = (HD_NOTIFY*) lParam; 

            if(hdn->hdr.code == HDN_BEGINTRACKW)
            {
                if(hdn->iItem == 0)
                    return TRUE; 
            }

            break;
        }

        
    } 
	
	//  Pass all non-custom messages to old window proc
	return CallWindowProc((int (__stdcall *)(void))lpOldProc, hwnd, msg, wParam, lParam );
}
