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
#include "currenttimeview.h"
#include "controlinfo.h"
#include "renderer.h"

CurrentTimeView::
CurrentTimeView(HWND hwnd,
                View* parent,
                HRGN viewRegion,
                DIB* viewBitmap,
                DIB* timeFontBitmap,
                int32 timeFontHeight,
                int32* timeFontWidths,
                DIB* descriptionFontBitmap,
                int32 descriptionFontHeight,
                int32* descriptionFontWidths):
TimeView(   hwnd,
            parent,
            viewRegion,
            viewBitmap,
            timeFontBitmap,
            timeFontHeight,
            timeFontWidths,
            descriptionFontBitmap,
            descriptionFontHeight,
            descriptionFontWidths,
            kSongInfoControl)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_pressed = false;
    m_command = command;

    m_hours = 0;
    m_minutes = 0;
    m_seconds = 0;

    m_description = NULL;

    m_timeFontBitmap = timeFontBitmap;
    m_timeFontWidths = timeFontWidths;
    m_timeFontHeight = timeFontHeight;

    m_descriptionFontBitmap = descriptionFontBitmap;
    m_descriptionFontWidths = descriptionFontWidths;
    m_descriptionFontHeight = descriptionFontHeight;

}

CurrentTimeView::
CurrentTimeView(   HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* viewBitmap,
            DIB* timeFontBitmap,
            int32 timeFontHeight,
            int32* timeFontWidths,
            DIB* descriptionFontBitmap,
            int32 descriptionFontHeight,
            int32* descriptionFontWidths,
            int32 command):
TimeView(hwnd, parent, viewRect)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_pressed = false;
    m_command = command;

    m_hours = 0;
    m_minutes = 0;
    m_seconds = 0;

    m_description = NULL;

    m_timeFontBitmap = timeFontBitmap;
    m_timeFontWidths = timeFontWidths;
    m_timeFontHeight = timeFontHeight;

    m_descriptionFontBitmap = descriptionFontBitmap;
    m_descriptionFontWidths = descriptionFontWidths;
    m_descriptionFontHeight = descriptionFontHeight;
}

CurrentTimeView::
~CurrentTimeView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;

    if(m_description)
        delete [] m_description;
}


void 
CurrentTimeView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;  
}

void 
CurrentTimeView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed && PtInRegion(Region(), x, y))
    {
        Invoke();
    }

    m_pressed = false;
}

void
CurrentTimeView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}

void 
CurrentTimeView::
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
