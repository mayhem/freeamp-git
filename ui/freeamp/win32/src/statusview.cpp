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
#include <stdlib.h>
#include <assert.h>

/* project headers */
#include "config.h"
#include "statusview.h"
#include "renderer.h"


StatusView::
StatusView(   HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* viewBitmap):
View(hwnd, parent, viewRegion)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_on = false;

}

StatusView::
StatusView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* viewBitmap):
View(hwnd, parent, viewRect)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_on = false;
}

StatusView::
~StatusView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;
}

void 
StatusView::
On(void)
{ 
    m_on = true;
    m_offset = Height();
    Invalidate();
}

void 
StatusView::
On(bool on)
{ 
    m_on = on;
    m_offset = (m_on) ? Height() : 0;
    Invalidate();
}

void 
StatusView::
Off(void)
{ 
    m_on = false;
    m_offset = 0;
    Invalidate();
}

void 
StatusView::
Draw(DIB* canvas, RECT* invalidRect)
{
    RECT tempRect, viewRect, parentRect, drawRect;

    assert(canvas);

    // what is our rect?
    Bounds(&viewRect);

    if(invalidRect && !IsRectEmpty(&drawRect))
    {
        // what is the smallest portion of us inside the dirty rect
        IntersectRect(&tempRect, invalidRect, &viewRect);
    }
    else
    {
        CopyRect(&tempRect, &viewRect);
    }

    if(Parent())
    {
        Parent()->Bounds(&parentRect);

        // clip ourselves to our parent view
        IntersectRect(&drawRect, &tempRect, &parentRect);
    }
    else
    {
        CopyRect(&drawRect, &tempRect);
    }

    /*char buffer[256];
    wsprintf(buffer, 
             "drawRect.left: %d\r\n"
             "drawRect.top: %d\r\n"
             "drawRect.right: %d\r\n"
             "drawRect.bottom: %d\r\n",
             drawRect.left,drawRect.top,drawRect.right,drawRect.bottom);

    OutputDebugString(buffer);*/

    // make sure something weird did not happen above...
    if(!IsRectEmpty(&drawRect))
    {
        // render the draw rect to the canvas

        // first we need to translate the coordinates

        int32 src_x, src_y;     // starting point in source
        int32 dest_x, dest_y;   // starting point in destination
        int32 width, height;    // lengths in source

        src_x = abs(drawRect.left - viewRect.left);
        src_y = abs(drawRect.top - viewRect.top);
        dest_x = drawRect.left;
        dest_y = drawRect.top;
        width = drawRect.right - drawRect.left;
        height = drawRect.bottom - drawRect.top;

        // second we need to blit the bits
        Renderer::Copy( canvas,
                        dest_x,
                        dest_y,
                        width,
                        height,
                        m_viewBitmap,
                        src_x,
                        src_y + m_offset);

       
    }
}
