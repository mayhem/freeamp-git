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
#include "bitmapview.h"
#include "renderer.h"

BitmapView::
BitmapView( HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* viewBitmap,
            DIB* maskBitmap,
            int32 command):
View(hwnd, parent, viewRegion)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_maskBitmap = maskBitmap;

    m_command = command;
    m_pressed = false;
}

BitmapView::
BitmapView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* viewBitmap,
            DIB* maskBitmap,
            int32 command):
View(hwnd, parent, viewRect)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_maskBitmap = maskBitmap;

    m_command = command;
    m_pressed = false;
}

BitmapView::
~BitmapView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;

    if(m_maskBitmap)
    {
        delete m_maskBitmap;
        m_maskBitmap = NULL;
    }
}

void 
BitmapView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;  
}

void 
BitmapView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed && PtInRegion(Region(), x, y))
    {
        Invoke();
    }

    m_pressed = false;
}

void
BitmapView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}


void 
BitmapView::
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

    // make sure something weird did not happen above...
    if(!IsRectEmpty(&drawRect))
    {
        // render the draw rect to the canvas

        // first we need to translate the coordinates

        int32 src_x, src_y; // starting point in source
        int32 dest_x, dest_y; // starting point in destination
        int32 width, height; // lengths in source

        src_x = abs(drawRect.left - viewRect.left);
        src_y = abs(drawRect.top - viewRect.top);
        dest_x = drawRect.left;
        dest_y = drawRect.top;
        width = drawRect.right - drawRect.left;
        height = drawRect.bottom - drawRect.top;

        // second we need to blit the bits

        if(m_maskBitmap)
        {
            Renderer::Copy( canvas,
                            dest_x, 
                            dest_y,     
                            width,      
                            height, 
                            m_viewBitmap,   
                            m_maskBitmap,
                            src_x,
                            src_y);
        }
        else
        {
            Renderer::Copy( canvas,
                            dest_x, 
                            dest_y,     
                            width,      
                            height, 
                            m_viewBitmap,    
                            src_x,
                            src_y);
        }

       
    }
}
