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

// system header files
#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

// project header files
#include "DropTarget.h"
#include "DropObject.h"


// IUnknown Methods

STDMETHODIMP DropTarget::QueryInterface(REFIID iid, void FAR* FAR* ppv) 
{
    if((iid == IID_IUnknown) ||(iid == IID_IDropTarget)) 
    {
        *ppv = this;
        ++m_refs;
        return NOERROR;
    }

    *ppv = NULL;

    return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) DropTarget::AddRef(void) 
{
    return ++m_refs;
}


STDMETHODIMP_(ULONG) DropTarget::Release(void) 
{
    if(--m_refs == 0) 
    {
        delete this;
        return 0;
    }
    return m_refs;
}  

// DropTarget Constructor
 
DropTarget::DropTarget(HWND	hwnd) 
{
    m_refs = 1;  
	m_hwnd = hwnd;
	m_bAcceptFmt = FALSE;
	m_bEnabled = TRUE;
    m_scroll = true;
    m_scrolling = true;

    short pattern[8];
    HBITMAP bmp;

	for (int i = 0; i < 8; i++)
		pattern[i] = (WORD)(0x5555 << (i & 1));

	bmp = CreateBitmap(8, 8, 1, 1, &pattern);

    m_insertBrush = CreatePatternBrush(bmp);
    DeleteObject(bmp);
}

DropTarget::~DropTarget() 
{
    DeleteObject(m_insertBrush);
}

//	Enable/disable dropping
void DropTarget::Enable(BOOL bEnable) 
{
    m_bEnabled = bEnable;  
}

#define SCROLL_OFF 0
#define SCROLL_UP 1
#define SCROLL_DOWN -1
#define SCROLL_TIMER 92173

void DropTarget::CheckAutoScroll(POINT pt)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE;

    BOOL hasScroll = GetScrollInfo(m_hwnd, SB_VERT, &si);

    if(hasScroll && si.nMin != si.nMax)
    {
        RECT rect;

        GetClientRect(m_hwnd, &rect);

        char buf[256];
        sprintf(buf, "%d, %d\r\n", pt.x, pt.y);

        //sprintf(buf, "%d, %d, %d, %d\r\n", 
        //    itemRect.top, itemRect.left, itemRect.bottom, itemRect.right);
        OutputDebugString(buf);


        if(PtInRect(&rect, pt))
        {
            if(pt.y < rect.top + GetSystemMetrics(SM_CYHSCROLL))
            {
                OutputDebugString("scroll up\r\n");
                // scroll up
                AutoScroll(SCROLL_UP);
            }
            else if(pt.y > rect.bottom - GetSystemMetrics(SM_CYHSCROLL))
            {
                // scroll down
                OutputDebugString("scroll down\r\n");
                AutoScroll(SCROLL_DOWN);
            }
            else
            {
                OutputDebugString("scroll off\r\n");
                AutoScroll(SCROLL_OFF);
            }
        }
    }
}

unsigned long __stdcall DropTarget::scrollThreadFunction(void* arg)
{
    DropTarget* me = (DropTarget*) arg;

    return me->ScrollThreadFunction();
}

unsigned long DropTarget::ScrollThreadFunction()
{
    

    do
    {
        ImageList_DragShowNolock(FALSE);
        SendMessage(m_hwnd, WM_VSCROLL, MAKELONG(m_scrollCode, 0), NULL);
        ImageList_DragShowNolock(TRUE);

        Sleep(500);
    }while(m_scroll);

    CloseHandle(m_threadHandle);

    m_scrolling = false;
    return 0;
}

int g_scrollCode;

static void CALLBACK ScrollProc(HWND hwnd, UINT msg, UINT id, DWORD ticks)
{
    ImageList_DragShowNolock(FALSE);
    SendMessage(hwnd, WM_VSCROLL, MAKELONG(g_scrollCode, 0), NULL);
    ImageList_DragShowNolock(TRUE);
}

void DropTarget::AutoScroll(int scrollCode)
{
    static int timer = 0;

    g_scrollCode = (scrollCode == SCROLL_UP ? SB_LINEUP : SB_LINEDOWN);

    if(scrollCode == SCROLL_OFF && m_scrolling)
    {
        KillTimer(NULL, timer);
        m_scrolling = false;
    }
    else if(!m_scrolling)
    {
        m_scrolling = true;
        //m_scroll = true;

        //OutputDebugString("set timer\r\n");

        

        //SetWindowLong(m_hwnd, GWL_USERDATA, (LONG) scrollCode);

        /*m_threadHandle = ::CreateThread(
									NULL,
									0,
									scrollThreadFunction,
									this,
									0,
									&m_threadId);*/

        timer = SetTimer(m_hwnd, SCROLL_TIMER, 250, (int (__stdcall *)(void))ScrollProc);
    }
}
//	________________________________________
//
//	IDropTarget Methods

STDMETHODIMP DropTarget::DragEnter(LPDATAOBJECT pDataObj, 
                                   DWORD grfKeyState, 
                                   POINTL pt, 
                                   LPDWORD pdwEffect) 
{  
    FORMATETC fmtetc;

    // default to not allowing drop
    m_bAcceptFmt = FALSE;
    *pdwEffect = DROPEFFECT_NONE;

    if(!m_bEnabled) 
    {
        return NOERROR;
    }

    // Does the drag source provide our private format?     
    fmtetc.cfFormat = RegisterClipboardFormat(CFSTR_FREEAMP_MUSICBROWSERITEM);
    fmtetc.ptd      = NULL;
    fmtetc.dwAspect = DVASPECT_CONTENT;  
    fmtetc.lindex   = -1;
    fmtetc.tymed    = TYMED_HGLOBAL; 

    if(pDataObj->QueryGetData(&fmtetc) == NOERROR) 
    {
        m_bAcceptFmt = TRUE;
        *pdwEffect = DROPEFFECT_COPY;
    }

    // Does the drag source provide CF_HDROP?     
    fmtetc.cfFormat = CF_HDROP;
    fmtetc.ptd      = NULL;
    fmtetc.dwAspect = DVASPECT_CONTENT;  
    fmtetc.lindex   = -1;
    fmtetc.tymed    = TYMED_HGLOBAL; 

    if(pDataObj->QueryGetData(&fmtetc) == NOERROR) 
    {
        m_bAcceptFmt = TRUE;
        *pdwEffect = DROPEFFECT_COPY;
    }

    // draw a line to indicate insertion point in list
    LV_HITTESTINFO hti;
    RECT itemRect;

    hti.pt.x = pt.x;
    hti.pt.y = pt.y;

    MapWindowPoints(NULL, m_hwnd, (LPPOINT)&hti.pt, 1);

    m_oldItem = ListView_HitTest(m_hwnd, &hti);

    if(m_oldItem < 0)
    {
        itemRect.top = 0;
        itemRect.bottom = 0;
        itemRect.left = 0;
        itemRect.right = 0;
    }
    else
    {   
        int middle;

        ListView_GetItemRect(m_hwnd, hti.iItem, &itemRect, LVIR_BOUNDS);

        middle = itemRect.top + (itemRect.bottom - itemRect.top)/2;

        if(pt.y < middle)
        {
            itemRect.top -= 2;
            itemRect.bottom = itemRect.top + 1;
        }
        else
        {
            itemRect.bottom += 2;
            itemRect.top = itemRect.bottom - 1;
        }
    }

    HDC hdc = GetDC(m_hwnd);

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_insertBrush);

    // draw new
    PatBlt(hdc,
           itemRect.left, 
           itemRect.top, 
           itemRect.right - itemRect.left, 
           itemRect.bottom - itemRect.top, 
           PATINVERT);

    SelectObject(hdc, oldBrush);

    ReleaseDC(m_hwnd, hdc);

    m_insertRect = itemRect;
   
	return NOERROR;
}

STDMETHODIMP DropTarget::DragOver(DWORD grfKeyState, 
                                  POINTL pt, 
                                  LPDWORD pdwEffect) 
{
    if(m_bAcceptFmt && m_bEnabled) 
        *pdwEffect = DROPEFFECT_COPY; 
	else 
        *pdwEffect = DROPEFFECT_NONE; 

    // draw a line to indicate insertion point in list
    LV_HITTESTINFO hti;
    RECT itemRect;

    hti.pt.x = pt.x;
    hti.pt.y = pt.y;

    MapWindowPoints(NULL, m_hwnd, (LPPOINT)&hti.pt, 1);

    m_oldItem = ListView_HitTest(m_hwnd, &hti);

    if(m_oldItem < 0)
    {
        itemRect = m_insertRect;
    }
    else
    {   
        int middle;

        ListView_GetItemRect(m_hwnd, hti.iItem, &itemRect, LVIR_BOUNDS);

        middle = itemRect.top + (itemRect.bottom - itemRect.top)/2;

        if(hti.pt.y < middle)
        {
            if(m_oldItem == 0)
            {
                itemRect.bottom = itemRect.top + 2;
            }
            else
            {
                itemRect.bottom = itemRect.top;
                itemRect.top -= 2;
            }
            //OutputDebugString("top\r\n");
        }
        else
        {
            itemRect.top = itemRect.bottom - 2;
            //itemRect.bottom;
            //OutputDebugString("bottom\r\n");
        }
    }

    //char buf[256];
    //sprintf(buf, "%d, %d\r\n", pt.x, pt.y);

    //sprintf(buf, "%d, %d, %d, %d\r\n", 
    //    itemRect.top, itemRect.left, itemRect.bottom, itemRect.right);
    //OutputDebugString(buf);

    HDC hdc = GetDC(m_hwnd);

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_insertBrush);

    // erase old
    /*PatBlt(hdc,
           m_insertRect.left, 
           m_insertRect.top, 
           m_insertRect.right - m_insertRect.left, 
           m_insertRect.bottom - m_insertRect.top, 
           PATINVERT);*/

    SCROLLINFO si;
    UINT columnWidth = ListView_GetColumnWidth(m_hwnd, 0);

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;

    GetScrollInfo(m_hwnd, SB_HORZ, &si);
    
    RECT rectColumn;
    RECT rectClient;

    if(si.nPos < columnWidth)
    {
        rectClient = rectColumn = m_insertRect;
        rectColumn.right = rectColumn.left + columnWidth - si.nPos - 1;
        rectClient.left = rectColumn.right;
        FillRect(hdc, &rectColumn, (HBRUSH)(COLOR_INFOBK + 1));
    }

    FillRect(hdc, &rectClient, (HBRUSH)GetClassLong(m_hwnd, GCL_HBRBACKGROUND));

    // draw new
    PatBlt(hdc,
           itemRect.left, 
           itemRect.top, 
           itemRect.right - itemRect.left, 
           itemRect.bottom - itemRect.top, 
           PATINVERT);

    SelectObject(hdc, oldBrush);

    ReleaseDC(m_hwnd, hdc);

    m_insertRect = itemRect;

    CheckAutoScroll(hti.pt);

	return NOERROR;
}


STDMETHODIMP DropTarget::DragLeave() 
{   
    m_bAcceptFmt = FALSE;  

    //ListView_RedrawItems(m_hwnd, m_oldItem - 1, m_oldItem + 1);
    
    HDC hdc = GetDC(m_hwnd);

    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_insertBrush);

    // erase old
    SCROLLINFO si;
    UINT columnWidth = ListView_GetColumnWidth(m_hwnd, 0);

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;

    GetScrollInfo(m_hwnd, SB_HORZ, &si);

    RECT rectColumn;
    RECT rectClient;

    if(si.nPos < columnWidth)
    {
        rectClient = rectColumn = m_insertRect;
        rectColumn.right = rectColumn.left + columnWidth - si.nPos - 1;
        rectClient.left = rectColumn.right;
        FillRect(hdc, &rectColumn, (HBRUSH)(COLOR_INFOBK + 1));
    }

    FillRect(hdc, &rectClient, (HBRUSH)GetClassLong(m_hwnd, GCL_HBRBACKGROUND));

    SelectObject(hdc, oldBrush);

    ReleaseDC(m_hwnd, hdc);

    AutoScroll(SCROLL_OFF);

    return NOERROR;
}

STDMETHODIMP DropTarget::Drop(LPDATAOBJECT pDataObj, 
                              DWORD grfKeyState, 
                              POINTL pt, 
                              LPDWORD pdwEffect) 
{   
    FORMATETC fmtetc;   
    STGMEDIUM medium;   
    HGLOBAL hGlobal;
    HRESULT hr;
 
    *pdwEffect = DROPEFFECT_NONE;
    hr = NOERROR;

    if(m_bAcceptFmt && m_bEnabled) 
    {   
        AutoScroll(SCROLL_OFF);

        /*HDC hdc = GetDC(m_hwnd);

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, m_insertBrush);

        // erase old
        SCROLLINFO si;
        UINT columnWidth = ListView_GetColumnWidth(m_hwnd, 0);

        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;

        GetScrollInfo(m_hwnd, SB_HORZ, &si);
    
        RECT rectColumn;
        RECT rectClient;

        if(si.nPos < columnWidth)
        {
            rectClient = rectColumn = m_insertRect;
            rectColumn.right = rectColumn.left + columnWidth - si.nPos - 1;
            rectClient.left = rectColumn.right;
            FillRect(hdc, &rectColumn, (HBRUSH)(COLOR_INFOBK + 1));
        }

        FillRect(hdc, &rectClient, (HBRUSH)GetClassLong(m_hwnd, GCL_HBRBACKGROUND));

        SelectObject(hdc, oldBrush);

        ReleaseDC(m_hwnd, hdc);*/

        if(m_oldItem > 0)
            m_oldItem--;

        ListView_RedrawItems(m_hwnd, m_oldItem, m_oldItem + 1);

        // User has dropped on us. First, try getting data in the 
        // private FreeAmp format

        fmtetc.cfFormat = RegisterClipboardFormat(CFSTR_FREEAMP_MUSICBROWSERITEM);
        fmtetc.ptd = NULL;
        fmtetc.dwAspect = DVASPECT_CONTENT;  
        fmtetc.lindex = -1;
        fmtetc.tymed = TYMED_HGLOBAL;       
        
        hr = pDataObj->GetData(&fmtetc, &medium);
        if(!FAILED(hr)) 
        {
            // Import the data and release it.
            hGlobal = medium.hGlobal;

            // need to update the point so we know 
            // where it was droppped in the window
            DROPFILES* df = (DROPFILES*)GlobalLock(hGlobal);
            df->pt.x = pt.x;
            df->pt.y = pt.y;

            MapWindowPoints(NULL, m_hwnd, (LPPOINT)&df->pt, 1);

            GlobalUnlock(hGlobal);

            // our format is the same as the DROPFILES format
            // except we pass URLs not paths... 
            SendMessage(m_hwnd, WM_DROPURLS, (WPARAM)hGlobal, 0);
           
            ReleaseStgMedium(&medium);
            *pdwEffect = DROPEFFECT_COPY;

            return NOERROR;
        }
        
        // if not our format, try getting CF_HDROP data from drag source
        fmtetc.cfFormat = CF_HDROP;
        fmtetc.ptd = NULL;
        fmtetc.dwAspect = DVASPECT_CONTENT;  
        fmtetc.lindex = -1;
        fmtetc.tymed = TYMED_HGLOBAL;       
    
        hr = pDataObj->GetData(&fmtetc, &medium);

        if(!FAILED(hr)) 
        {
            // Import the data and release it.
            hGlobal = medium.hGlobal;

            // need to update the point so we know 
            // where it was droppped in the window
            DROPFILES* df = (DROPFILES*)GlobalLock(hGlobal);
            df->pt.x = pt.x;
            df->pt.y = pt.y;

            MapWindowPoints(NULL, m_hwnd, (LPPOINT)&df->pt, 1);

            GlobalUnlock(hGlobal);

            SendMessage(m_hwnd, WM_DROPFILES, (WPARAM)hGlobal, 0);

            ReleaseStgMedium(&medium);
            *pdwEffect = DROPEFFECT_COPY;

            return NOERROR;
        }
    }
	return hr;      
}