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

// system header files
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

// project header files
#include "DropSource.h"
#include "resource.h"


// IUnknown Methods

STDMETHODIMP DropSource::QueryInterface(REFIID iid, void FAR* FAR* ppv) 
{
    if((iid == IID_IUnknown) || (iid == IID_IDropSource)) 
    {
        *ppv = this;
        ++m_refs;
        return NOERROR;
    }

    *ppv = NULL;

    return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) DropSource::AddRef(void) 
{
    return ++m_refs;
}


STDMETHODIMP_(ULONG) DropSource::Release(void) 
{
    if(--m_refs == 0) 
    {
        delete this;
        return 0;
    }
    return m_refs;
}  
 
// DropSource Constructor
 
DropSource::DropSource(HWND hwndTree, NM_TREEVIEW* nmtv) 
{
    /*HIMAGELIST			hIml;
    TL_DRAGBITMAP		tldb;

    m_refs = 1; 
    m_hwnd = hwnd;

    sCalcOffsets(hwnd, &m_ncxoffset, &m_ncyoffset);

    GetCursorPos(&tldb.ptCursorPos);
    ScreenToClient(hwndTree, &tldb.ptCursorPos);
    tldb.ulFlags = TLRB_TOPLEVELONLY;
    TreeList_RenderDragBitmap(hwndTree, &tldb);
    hIml = ImageList_Create(tldb.sizeDrag.cx, tldb.sizeDrag.cy, 
    ILC_MASK|ILC_COLORDDB, 1, 1);
    ImageList_AddMasked(hIml, tldb.hbmDrag, GetSysColor(COLOR_WINDOW));
    DeleteObject(tldb.hbmDrag);

    ImageList_BeginDrag(hIml, 0, tldb.ptHotSpot.x, tldb.ptHotSpot.y);
    ImageList_Destroy(hIml);

    GetCursorPos(&tldb.ptCursorPos);
    ScreenToClient(hwnd, &tldb.ptCursorPos);
    ImageList_DragEnter(hwnd, tldb.ptCursorPos.x + m_ncxoffset, 
                         tldb.ptCursorPos.y + m_ncyoffset);*/

    HIMAGELIST himl;    // handle of image list 
    RECT rcItem;        // bounding rectangle of item 
    //DWORD dwLevel;      // heading level of item 
    DWORD dwIndent;     // amount that child items are indented 

    m_hwnd = hwndTree;
    m_refs = 1; 
 
    // Tell the tree-view control to create an image to use 
    // for dragging. 
    himl = TreeView_CreateDragImage(hwndTree, nmtv->itemNew.hItem);
 
    // Get the bounding rectangle of the item being dragged. 
    TreeView_GetItemRect(hwndTree, nmtv->itemNew.hItem, &rcItem, TRUE); 
 
    // Get the heading level and the amount that the child items are 
    // indented. 
    //dwLevel = nmtv->itemNew.lParam; 
    dwIndent = (DWORD) SendMessage(hwndTree, TVM_GETINDENT, 0, 0);
 
    //MapWindowPoints(hwnd, GetParent(hwndTree), &nmtv->ptDrag, 1);

    // Start the drag operation. 
    //ImageList_DragEnter(GetParent(hwndTree), nmtv->ptDrag.x, nmtv->ptDrag.y);
    ImageList_BeginDrag(himl, 
                        0, 
                        0,
                        (rcItem.bottom - rcItem.top)/2);// + (rcItem.bottom - rcItem.top)/2);

                        //nmtv->ptDrag.x - rcItem.left, 
                        //nmtv->ptDrag.y - rcItem.top);

    ImageList_Destroy(himl);
 
    POINT pt = nmtv->ptDrag;
    
    ClientToScreen(hwndTree, &pt);

    ImageList_DragEnter(NULL, pt.x, pt.y);
 
    // Hide the mouse cursor, and direct mouse input to the 
    // parent window. 
    //ShowCursor(FALSE); 
    //SetCapture(GetParent(hwndTree)); 
}

DropSource::~DropSource() 
{
    ImageList_DragShowNolock(FALSE);
    ImageList_DragLeave(NULL);
    ImageList_EndDrag();
    //ShowCursor(TRUE); 
}


//	________________________________________
//
//	IDropSource Methods

STDMETHODIMP DropSource::QueryContinueDrag(BOOL	fEscapePressed, 
                                           DWORD grfKeyState) 
{  
    POINT	pt;

	GetCursorPos(&pt);

    ImageList_DragMove(pt.x , pt.y);

    if(fEscapePressed)
        return DRAGDROP_S_CANCEL;
    else if(!(grfKeyState & MK_LBUTTON) && !(grfKeyState & MK_RBUTTON))
        return DRAGDROP_S_DROP;
    else
        return NOERROR;
}

STDMETHODIMP DropSource::GiveFeedback(DWORD dwEffect) 
{
	//POINT	pt;
	//HWND	hwnd;
	//RECT	rc;

	//GetCursorPos(&pt);

	//GetWindowRect(m_hwnd, &rc);

	/*if(!PtInRect(&rc, pt)) {
		hwnd = NULL;
		//KMFindWindowFromPoint((PKEYMAN)m_pKeyMan, &pt, &hwnd);

		if(hwnd != m_hwnd) {
			ImageList_DragLeave(m_hwnd);
			m_hwnd = hwnd;
			if(m_hwnd) {
				sCalcOffsets(hwnd, &m_ncxoffset, &m_ncyoffset);
				ScreenToClient(m_hwnd, &pt);
				ImageList_DragEnter(m_hwnd, 
						pt.x + m_ncxoffset, pt.y + m_ncyoffset);
			}
		}
	}*/

	//if(!(dwEffect & DROPEFFECT_SCROLL)) {
	//	ImageList_DragMove(pt.x , pt.y);
	//}

	return DRAGDROP_S_USEDEFAULTCURSORS;
}