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

#define IsCtrlDown()  (GetKeyState(VK_CONTROL) < 0)
#define IsShiftDown()  (GetKeyState(VK_SHIFT) < 0)

static HTREEITEM g_editItem = NULL;


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

static
LRESULT WINAPI 
EditLabelWndProc(HWND hwnd, 
                 UINT msg, 
                 WPARAM wParam, 
                 LPARAM lParam)
{
    WNDPROC lpOldProc = (WNDPROC)GetProp(hwnd, "oldproc" );
    BOOL isPlaylist = (BOOL)GetProp(hwnd, "playlist" );

    switch(msg)
	{
		case WM_DESTROY:   
		{
			//  Put back old window proc and
			SetWindowLong( hwnd, GWL_WNDPROC, (DWORD)lpOldProc );

			// remove window property
			RemoveProp( hwnd, "oldproc" ); 
            RemoveProp( hwnd, "playlist" ); 

			break;
        }

        case WM_GETDLGCODE:
        {
            return DLGC_WANTALLKEYS;
        }

        case WM_CHAR:
        {
            if(wParam == VK_ESCAPE)
            {
                SendMessage(GetParent(hwnd), TVM_ENDEDITLABELNOW, TRUE, 0); 
                return 1;
            }
            else if(wParam == VK_RETURN)
            {
                SendMessage(GetParent(hwnd), TVM_ENDEDITLABELNOW, FALSE, 0); 
                return 1;
            }

            if( isPlaylist &&
               (wParam == '\\'  ||
                wParam == '/'   ||
                wParam == ':'   ||
                wParam == '*'   ||
                wParam == '?'   ||
                wParam == '\"'  ||
                wParam == '<'   ||
                wParam == '>'   ||
                wParam == '|' ) )
            {
                return 1;
            }

            break;
        }


    }

    return CallWindowProc((int (__stdcall *)(void))lpOldProc, hwnd, msg, wParam, lParam );
}

void MusicBrowserUI::EditItemLabel(HWND hwnd, HTREEITEM item)
{
    g_editItem = item;
    SetFocus(hwnd);

    HWND hwndEdit = TreeView_EditLabel(hwnd, item);

    if(hwndEdit)
    {
        bool isPlaylist = false;
        TV_ITEM tv_item;

        tv_item.hItem = item;
        tv_item.mask = TVIF_PARAM;

        TreeView_GetItem(hwnd, &tv_item);

        SetProp(hwndEdit, 
                "oldproc",
                (HANDLE)GetWindowLong(hwndEdit, GWL_WNDPROC));

        SetProp(hwndEdit, 
                "playlist",
                (HANDLE)m_oTreeIndex.IsPlaylist(tv_item.lParam));

	    // Subclass the window so we can handle multi-select
	    SetWindowLong(hwndEdit, 
			          GWL_WNDPROC, 
                      (DWORD)::EditLabelWndProc);  
    }
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
    static bool dragging = false;
    static RECT dragRect;
    static HTREEITEM dragItem = NULL;
    static bool selectedOnMouseDown = false;

    //return CallWindowProc((int (__stdcall *)(void))lpOldProc, hwnd, msg, wParam, lParam );

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
            if(dragging)
            {
                if(wParam & MK_LBUTTON)
                {
                    POINT dragPt;

                    dragPt.x = LOWORD(lParam);
                    dragPt.y = HIWORD(lParam);

                    if( !PtInRect(&dragRect, dragPt) )
                    {
                        SetCapture(NULL);
                        dragging = false;

                        NM_TREEVIEW nm_treeview;

                        memset(&nm_treeview, 0x00, sizeof(NM_TREEVIEW));
                        
                        nm_treeview.hdr.code = TVN_BEGINDRAG;
                        nm_treeview.hdr.hwndFrom = hwnd;
                        nm_treeview.hdr.idFrom = GetWindowLong(hwnd, GWL_ID);

                        nm_treeview.itemNew.hItem = dragItem;
                        nm_treeview.itemNew.mask = TVIF_STATE|TVIF_PARAM;
                        nm_treeview.itemNew.stateMask = TVIS_BOLD|TVIS_CUT|TVIS_DROPHILITED|TVIS_EXPANDED|TVIS_SELECTED|TVIS_EXPANDEDONCE;
                        
                        TreeView_GetItem(hwnd, &nm_treeview.itemNew);

                        nm_treeview.ptDrag.x = LOWORD(lParam);
                        nm_treeview.ptDrag.y = HIWORD(lParam);

                        SendMessage(GetParent(hwnd), 
                                    WM_NOTIFY, 
                                    (WPARAM)nm_treeview.hdr.idFrom,
                                    (LPARAM)&nm_treeview);

                        return TRUE;
                    }
                }
            }

            break;
        }

        case WM_RBUTTONDOWN:
        {
            SetFocus(hwnd);

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

                if(!wasSelected)
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
                            tv_item.stateMask = TVIS_SELECTED;
                            tv_item.state = 0;

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

                TreeView_Select(hwnd, item, TVGN_CARET);

                if(!wasFocus && wasFocusSelected && wasSelected)
                {
                    tv_item.hItem = focusItem;
                    tv_item.mask = TVIF_STATE;
                    tv_item.stateMask = TVIS_SELECTED;
                    tv_item.state = TVIS_SELECTED;

                    TreeView_SetItem(hwnd, &tv_item);
                }
            }

            //return TRUE;

            break;
        }

        case WM_RBUTTONUP:
        {
            break;
        }

        case WM_LBUTTONDOWN:
        {
            bool shiftKeyPressed = IsShiftDown();
            bool ctrlKeyPressed = IsCtrlDown();

            selectedOnMouseDown = false;

            SetFocus(hwnd);

            HTREEITEM item;
            TV_HITTESTINFO hti;

            hti.pt.x = LOWORD(lParam);
            hti.pt.y = HIWORD(lParam);

            int dx = GetSystemMetrics(SM_CXDRAG);
            int dy = GetSystemMetrics(SM_CYDRAG);

            dragRect.top = hti.pt.y - dy;
            dragRect.bottom = hti.pt.y + dy;
            dragRect.left = hti.pt.x - dx;
            dragRect.right = hti.pt.x + dx;
        
            item = TreeView_HitTest(hwnd, &hti);  

            if(item && (hti.flags & TVHT_ONITEM))
            {
                dragItem = item;

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

                selectedOnMouseDown = wasSelected;
                
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
                            tv_item.stateMask = TVIS_SELECTED;
                            tv_item.state = 0;

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
                    if(!wasSelected)
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
                                tv_item.stateMask = TVIS_SELECTED;
                                tv_item.state = 0;

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

                    TreeView_Select(hwnd, item, TVGN_CARET);

                    if(!wasFocus && wasFocusSelected && wasSelected)
                    {
                        tv_item.hItem = focusItem;
                        tv_item.mask = TVIF_STATE;
                        tv_item.stateMask = TVIS_SELECTED;
                        tv_item.state = TVIS_SELECTED;

                        TreeView_SetItem(hwnd, &tv_item);
                    }
                    
                    
                }

                SetCapture(hwnd);
                dragging = true;

                return TRUE;
            }
                    
            break;
        }

        case WM_LBUTTONUP:
        {
            bool shiftKeyPressed = IsShiftDown();
            bool ctrlKeyPressed = IsCtrlDown();

            SetCapture(NULL);
            dragging = false;

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
                            tv_item.stateMask = TVIS_SELECTED;
                            tv_item.state = 0;

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
                    
                    if(selectedOnMouseDown)
                    {
                        if(g_editItem != item)
                        {
                            // i should do this in the notify but it is ignoring me
                            if(item != m_hCatalogItem &&
                               item != m_hPlaylistItem &&
                               item != m_hAllItem &&
                               item != m_hUncatItem &&
                               item != m_hNewPlaylistItem)
                            {
                                // pause a half sec so this does not
                                // look so jarring
                                Sleep(500);

                                EditItemLabel(hwnd, item);
                            }
                        }
                        else
                        {
                            g_editItem = NULL;
                        }
                    }
                }
            }

                    
            break;
        }

        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        {
            UpdateButtonMenuStates();
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