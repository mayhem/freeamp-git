/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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

/* system headers */
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <stdlib.h>

/* project headers */
#include "config.h"
#include "view.h"

View::
View(   HWND hwnd,
        View* parent,
        HRGN viewRegion)
{
    assert(hwnd);
    assert(viewRegion);

    m_hwnd = hwnd;
    m_parent = parent;
    m_viewRegion = viewRegion;

    GetRgnBox(m_viewRegion, &m_viewRect);

    m_visible = true;
    m_enabled = true;
}

View::
View(   HWND hwnd,
        View* parent,
        RECT* viewRect)
{

    assert(hwnd);
    assert(viewRect);

    m_hwnd = hwnd;
    m_parent = parent;
    m_viewRegion = CreateRectRgn(   viewRect->left, 
                                    viewRect->top, 
                                    viewRect->right,
                                    viewRect->bottom);
    CopyRect(&m_viewRect, viewRect);

    m_visible = true;
    m_enabled = true;
}

View::
~View()
{
    m_hwnd          = NULL;
    m_parent        = NULL;
    m_viewRegion    = NULL;
}

bool 
View::
PointInView(int32 x, int32 y)
{
    assert(m_viewRegion);

    return ( PtInRegion(m_viewRegion, x, y) != 0 );
}

void 
View::
Show() 
{ 
    m_visible = true; 
    Invalidate();
}

void 
View::
Show(bool visible) 
{ 
    m_visible = visible; 
    Invalidate();
}

void 
View::
Hide() 
{ 
    m_visible = false; 
    Invalidate();
}

void 
View::
Enable(void)
{
    m_enabled = true;
}

void 
View::
Enable(bool enable)
{
    m_enabled = enable;
}

void 
View::
Disable(void)
{
    m_enabled = false;
}

void 
View::
MoveTo(int32 x, int32 y)
{
    RECT oldRect;

    CopyRect(&oldRect, &m_viewRect);

    int32 dx, dy;

    dx = x - m_viewRect.left;
    dy = y - m_viewRect.top;

    OffsetRect(&m_viewRect, dx, dy);
    OffsetRgn(m_viewRegion, dx, dy);

    Invalidate(&oldRect);
    Invalidate(&m_viewRect);
}

void 
View::
OffsetBy(int32 x, int32 y)
{
    RECT oldRect;

    CopyRect(&oldRect, &m_viewRect);

    OffsetRect(&m_viewRect, x, y);
    OffsetRgn(m_viewRegion, x, y);

    Invalidate(&oldRect);
    Invalidate(&m_viewRect);
}

void 
View::
SetRegion(HRGN viewRegion)
{
    if(m_viewRegion)
        DeleteObject(m_viewRegion);

    m_viewRegion = CreateRectRgn(0,0,0,0);

    CombineRgn( m_viewRegion,
                m_viewRegion,
                viewRegion,
                RGN_OR);

    GetRgnBox(viewRegion, &m_viewRect);

    Invalidate(m_viewRegion);
}

void 
View::
Invalidate() 
{ 
    InvalidateRect(m_hwnd, &m_viewRect, FALSE);
}


void 
View::
Invalidate(HRGN region) 
{ 
    InvalidateRgn(m_hwnd, region, FALSE);
}


void 
View::
Invalidate(RECT* rect) 
{ 
    InvalidateRect(m_hwnd, rect, FALSE);
}





