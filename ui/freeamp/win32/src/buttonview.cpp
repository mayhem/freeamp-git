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
#include "buttonview.h"
#include "renderer.h"

ButtonView::
ButtonView( HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* viewBitmap,
            int32 command,
            int32 flags):
View(hwnd, parent, viewRegion)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_state = Unpressed;
    m_command = command;
    m_flags = flags;
}

ButtonView::
ButtonView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* viewBitmap,
            int32 command,
            int32 flags):
View(hwnd, parent, viewRect)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_state = Unpressed;
    m_command = command;
    m_flags = flags;
}

ButtonView::
~ButtonView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;
}

void 
ButtonView::
MouseEntered()
{
    //OutputDebugString("Button: MouseEntered\r\n");
    if(m_state == Unpressed)
    {
        m_offset = Height();
        Invalidate();
    }
}

void 
ButtonView::
MouseLeft()
{
    //OutputDebugString("Button: MouseLeft\r\n");
    if(m_state == Unpressed)
    {
        m_offset = 0;
        Invalidate();
    }
}

void 
ButtonView::
MouseMove(int32 x, int32 y, int32 modifiers)
{

}

void 
ButtonView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    if(m_state == Unpressed)
    {
        m_offset = Height()*2;
        Invalidate();
    }
}

void 
ButtonView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(0)//m_flags & ButtonType_ToggleButton)
    {
        if(m_state == Unpressed)
        {
            if(PtInRegion(Region(), x, y))
            {
                m_offset = Height()*2;
                m_state = Pressed;
                Invoke();
            }
            else
            {
                m_offset = 0;
            }
        }
        else if(m_state == Pressed)
        {
            if(PtInRegion(Region(), x, y))
            {
                m_offset = Height();
                m_state = Unpressed;
                Invoke();
            }
            else
            {
                m_offset = 0;
            }
        }
    }
    else
    {
        if(PtInRegion(Region(), x, y))
        {
            m_offset = Height();
            m_state = Unpressed;
            Invoke();
        }
        else
        {
            m_offset = 0;
        }
    }

    Invalidate();
}

void 
ButtonView::
SetState(ButtonState state)
{
    if(m_state != state)
    {
        if(m_flags & ButtonType_ToggleButton)
        {
            m_state = state;

            if(m_state == Unpressed)
            {
                m_offset = 0;
            }
            else if(m_state == Pressed)
            {
                m_offset = Height()*2;
            }
        }

        Invalidate();
    }
}

void
ButtonView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}

void 
ButtonView::
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
