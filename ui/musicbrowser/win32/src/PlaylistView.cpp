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
            PlaylistItem* item;
            LV_ITEM lv_item;
            string displayString;

            lv_item.mask = LVIF_PARAM;
            lv_item.iItem = dis->itemID;
            lv_item.iSubItem = 0;
            lv_item.lParam = NULL;

            ListView_GetItem(hwndList, &lv_item);

            //item = (PlaylistItem*) lv_item.lParam;
            item = m_oPlm->ItemAt(dis->itemID);

            if(item == NULL)
                return FALSE;

            // is this the current index? if so make it bold ...
            // btw, we only do this if it is the primary browser
            if(dis->itemID == m_oPlm->GetCurrentIndex() && !m_pParent)
            {
                LOGFONT lf;

                GetObject(GetWindowFont(hwndList), sizeof(LOGFONT), &lf);

                lf.lfWeight = FW_BOLD;

                boldFont = CreateFontIndirect(&lf);

                oldFont = (HFONT)SelectObject(dis->hDC, boldFont);
            }

            // Item index
            char buf[64];
            sprintf(buf, "%d", dis->itemID + 1);
            displayString = buf;
            
            CalcStringEllipsis(dis->hDC, 
                               displayString, 
                               ListView_GetColumnWidth(hwndList, 0) /*- (cxImage + 1)*/);

            UINT oldAlign;

            oldAlign = SetTextAlign(dis->hDC, TA_CENTER | TA_TOP );

            RECT indexRect = rcClip;

            indexRect.right = indexRect.left + ListView_GetColumnWidth(hwndList, 0) - 1;

            UINT left = indexRect.left + (ListView_GetColumnWidth(hwndList, 0)/2);
            
            SetTextColor(dis->hDC, GetSysColor(COLOR_INFOTEXT));
            SetBkColor(dis->hDC, GetSysColor(COLOR_INFOBK ));

            ExtTextOut( dis->hDC, 
                        left, indexRect.top + 1,
                        ETO_CLIPPED | ETO_OPAQUE,
                        &indexRect, 
                        displayString.c_str(),
                        displayString.size(),
                        NULL);

            SetTextAlign(dis->hDC, oldAlign);

            
            // Move over to the next column
            rcClip.left += ListView_GetColumnWidth(hwndList, 0);

            // Check to see if this item is selected
            if(dis->itemState & ODS_SELECTED && GetFocus() == hwndList)
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
            //ListView_GetItemText(hwndList, dis->itemID, 1, buf, 1024);
            displayString = item->GetMetaData().Title();

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
            //ListView_GetItemText(hwndList, dis->itemID, 2, buf, 1024);
            displayString = item->GetMetaData().Artist();

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
            //ListView_GetItemText(hwndList, dis->itemID, 3, buf, 1024);
            displayString = item->GetMetaData().Album();

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
            //ListView_GetItemText(hwndList, dis->itemID, 4, buf, 1024);

            if(item->GetMetaData().Time() != 0)
            {
                int32 seconds = item->GetMetaData().Time();
                int32 hours = seconds / 3600;
		        int32 minutes = seconds / 60 - hours * 60;
                seconds = seconds - minutes * 60 - hours * 3600;

                if(hours)
                    sprintf(buf, "%d:%02d:%02d", hours, minutes, seconds);
                else
                    sprintf(buf, "%d:%02d", minutes, seconds);

                displayString = buf;
            }
            else    
                displayString = "Unknown";

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
            if(dis->itemID == m_oPlm->GetCurrentIndex() && !m_pParent)
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

void MusicBrowserUI::PlaylistListItemMoved(const PlaylistItem* item, 
                                           uint32 oldIndex, 
                                           uint32 newIndex)
{
    HWND    hwnd = GetDlgItem(m_hWnd, IDC_PLAYLISTBOX);
    uint32  index = m_oPlm->IndexOf(item);

    if(index != kInvalidIndex)
    {
        LV_ITEM sItem;

        uint32 state = ListView_GetItemState(m_hPlaylistView, 
                                             oldIndex, 
                                             LVIS_SELECTED|LVIS_FOCUSED);

        sItem.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
        sItem.iSubItem = 0;
        sItem.iItem = index;
        sItem.lParam = (LPARAM)item;
        sItem.iImage = 0;
        sItem.stateMask = LVIS_FOCUSED|LVIS_SELECTED;
        sItem.state = state;

        ListView_DeleteItem(hwnd, oldIndex);

        ListView_InsertItem(hwnd, &sItem);
    }
}

void MusicBrowserUI::PlaylistListItemRemoved(const PlaylistItem* item, 
                                             uint32 oldIndex)
{
    // item has already been deleted when we get this 
    // msg so don't access it. only use it for comparison

    if(oldIndex != kInvalidIndex)
    {
        LV_ITEM sItem;
        
        sItem.mask = LVIF_PARAM;
        sItem.iItem = oldIndex;
        sItem.iSubItem = 0;
        sItem.lParam = 0;

        ListView_GetItem(m_hPlaylistView, &sItem);
        
        ListView_DeleteItem(m_hPlaylistView, oldIndex);

        if(oldIndex >= ListView_GetItemCount(m_hPlaylistView))
            oldIndex = ListView_GetItemCount(m_hPlaylistView) - 1;

        ListView_SetItemState(m_hPlaylistView, oldIndex, LVIS_SELECTED, LVIS_SELECTED);
        ListView_RedrawItems(m_hPlaylistView, oldIndex, ListView_GetItemCount(m_hPlaylistView) - 1);

        m_bListChanged = true;
        UpdateButtonMenuStates();
    }
}

void MusicBrowserUI::PlaylistListSorted(void)
{
    ListView_RedrawItems(m_hPlaylistView, 0, ListView_GetItemCount(m_hPlaylistView) - 1);
    m_bListChanged = true;
    UpdateButtonMenuStates();
}

void MusicBrowserUI::PlaylistListItemUpdated(const PlaylistItem* item)
{
    uint32        index = m_oPlm->IndexOf(item);
    HWND          hwnd = GetDlgItem(m_hWnd, IDC_PLAYLISTBOX);

    if(index != kInvalidIndex)
    {
        ListView_RedrawItems(hwnd, index, index);
    }
}

void MusicBrowserUI::PlaylistListItemAdded(const PlaylistItem* item)
{
    LV_ITEM       sItem;
    uint32        index = m_oPlm->IndexOf(item);
    HWND          hwnd = GetDlgItem(m_hWnd, IDC_PLAYLISTBOX);

    if(index != kInvalidIndex)
    {
        sItem.mask = 0;
        sItem.iSubItem = 0;
        sItem.iItem = index;

        ListView_InsertItem(hwnd, &sItem);

        // this skips change notification
        // for initial loading of list for
        // editing. a hack pretty much but
        // i can't think of a better way
        if(m_initialCount)
            m_initialCount--;
        else
        {
            m_bListChanged = true;
            UpdateButtonMenuStates();
        }
    }
}

LRESULT WINAPI 
ListViewWndProc(HWND hwnd, 
                UINT msg, 
                WPARAM wParam, 
                LPARAM lParam)
{
    MusicBrowserUI* ui = (MusicBrowserUI*)GetProp(hwnd, "this" );

    return ui->ListViewWndProc(hwnd, msg, wParam, lParam);
}


LRESULT MusicBrowserUI::ListViewWndProc(HWND hwnd, 
                                        UINT msg, 
                                        WPARAM wParam, 
                                        LPARAM lParam)
{
    WNDPROC lpOldProc = (WNDPROC)GetProp( hwnd, "oldproc" );
    bool filesAreURLs = false;

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

        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            UpdateButtonMenuStates();
            break;
        
        case WM_DROPURLS:
            filesAreURLs = true;
        case WM_DROPFILES:
        {
            HDROP dropHandle = (HDROP)wParam;
            uint32 count;
            char url[1024];
            char path[MAX_PATH];
            vector<string> fileList;

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

                if(!filesAreURLs)
                {
                    uint32 length = sizeof(url);
                    strcpy(path, url);
                    FilePathToURL(path, url, &length);
                }

                fileList.push_back(url);

                //MessageBox(NULL, url, "url", MB_OK);
            }

            // we know that we are gonna be adding a 
            // bunch of items so let windows know.
            // it will make the adds more efficient
            uint32 newSize = ListView_GetItemCount(hwnd);
            newSize += fileList.size();
            ListView_SetItemCount(hwnd, newSize);

            LV_HITTESTINFO hti;
            RECT itemRect;

            DragQueryPoint(dropHandle, &hti.pt);
            int32 index = ListView_HitTest(hwnd, &hti);

            if(index < 0)
            {
                m_oPlm->AddItems(fileList);
            }
            else
            {   
                int32 middle;

                ListView_GetItemRect(hwnd, hti.iItem, &itemRect, LVIR_BOUNDS);

                middle = itemRect.top + (itemRect.bottom - itemRect.top)/2;

                if(hti.pt.y < middle)
                {
                    //index 
                }
                else
                {
                    index++; 
                }

                m_oPlm->AddItems(fileList, index);
            }

            //char buf[256];
            //sprintf(buf, "x: %d   y: %d\r\n", pt.x, pt.y);
            //OutputDebugString(buf);
   
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

            SCROLLINFO si;
            uint32 columnWidth = ListView_GetColumnWidth(hwnd, 0);

            si.cbSize = sizeof(SCROLLINFO);
            si.fMask = SIF_ALL;

            GetScrollInfo(hwnd, SB_HORZ, &si);
            
            RECT rectClient, rectColumn;

            GetClientRect(hwnd, &rectClient);

            if(si.nPos < columnWidth)
            {
                rectColumn = rectClient;
                rectColumn.right = rectColumn.left + columnWidth - si.nPos - 1;
                rectClient.left = rectColumn.right;
                FillRect(hdc, &rectColumn, (HBRUSH)(COLOR_INFOBK + 1));
            }

            FillRect(hdc, &rectClient, (HBRUSH)GetClassLong(hwnd, GCL_HBRBACKGROUND));

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
