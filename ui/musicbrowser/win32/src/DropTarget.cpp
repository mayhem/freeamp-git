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
}

DropTarget::~DropTarget() 
{
    
}

//	Enable/disable dropping
void DropTarget::Enable(BOOL bEnable) 
{
    m_bEnabled = bEnable;  
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

	return NOERROR;
}


STDMETHODIMP DropTarget::DragLeave() 
{   
    m_bAcceptFmt = FALSE;   
    //ImageList_DragLeave(m_hwnd);
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
            GlobalUnlock(hGlobal);

            SendMessage(m_hwnd, WM_DROPFILES, (WPARAM)hGlobal, 0);

            ReleaseStgMedium(&medium);
            *pdwEffect = DROPEFFECT_COPY;

            return NOERROR;
        }

    }
	return hr;      
}