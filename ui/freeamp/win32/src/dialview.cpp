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
#include "dialview.h"
#include "renderer.h"
#include "resource.h"
#include "freeampui.h"


DialView::
DialView(   HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* viewBitmap,
            int32 command):
View(hwnd, parent, viewRegion)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_command = command;
    m_pressed = false;

    HINSTANCE hinst = (HINSTANCE)GetWindowLong(Window(), GWL_HINSTANCE);
    m_dialCursor = LoadCursor(hinst, MAKEINTRESOURCE(IDC_DIAL));
    m_oldCursor = NULL;
}

DialView::
DialView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* viewBitmap,
            int32 command):
View(hwnd, parent, viewRect)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_offset = 0;
    m_command = command;
    m_pressed = false;


    HINSTANCE hinst = (HINSTANCE)GetWindowLong(Window(), GWL_HINSTANCE);
    m_dialCursor = LoadCursor(hinst, MAKEINTRESOURCE(IDC_DIAL));
    m_oldCursor = NULL;
}

DialView::
~DialView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;
}

void 
DialView::
MouseEntered()
{
    FreeAmpUI* ui = (FreeAmpUI*)GetWindowLong(Window(), GWL_USERDATA);

    m_oldCursor = ui->SetCursor(m_dialCursor);
}

void 
DialView::
MouseLeft()
{
    FreeAmpUI* ui = (FreeAmpUI*)GetWindowLong(Window(), GWL_USERDATA);

    ui->SetCursor(m_oldCursor);
}

void 
DialView::
MouseMove(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed)
    {
        int32 delta = 0;

        delta = m_clickPoint.y - y;

        if(delta > 0) // dial up
        {
            m_position += delta;
        }
        else if(delta < 0) // dial down
        {
            m_position += delta;
        }

        m_offset = abs(m_position % 5) * Width();

        /*if(y == 0 || y == Height() - 1)
            SetCursorPos(m_screenPoint.x, m_screenPoint.y);*/

        Invalidate();

        /*char buffer[256];

        wsprintf(   buffer, 
                    "y: %ld\r\n",
                    y);

        OutputDebugString(buffer);*/

        NMHDR nmhdr;

        nmhdr.hwndFrom = (HWND)this;
        nmhdr.idFrom = (UINT)m_command;
        nmhdr.code = (UINT)DIAL_MOVE;


        SendMessage(Window(), WM_NOTIFY, (WPARAM)m_command, (LPARAM) &nmhdr);
    }
}

void 
DialView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;
    ///m_clickPoint.x = x;
    //m_clickPoint.y = y;

    RECT rect;

    Bounds(&rect);

    m_clickPoint.x = m_screenPoint.x = rect.left + Width()/2;
    m_clickPoint.y = m_screenPoint.y = rect.top + Height()/2;

    m_position = 0;

    ClientToScreen(Window(), &m_screenPoint);
    SetCursorPos(m_screenPoint.x, m_screenPoint.y);
    ShowCursor(FALSE);

    MapWindowPoints(Window(), HWND_DESKTOP, (LPPOINT)&rect, 2);
    //ClipCursor(&rect);

    NMHDR nmhdr;

    nmhdr.hwndFrom = (HWND)this;
    nmhdr.idFrom = (UINT)m_command;
    nmhdr.code = (UINT)DIAL_BUTTON_DOWN;


    SendMessage(Window(), WM_NOTIFY, (WPARAM)m_command, (LPARAM) &nmhdr);
}

void 
DialView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    m_pressed = false;  
    SetCursorPos(m_screenPoint.x, m_screenPoint.y); 
    ShowCursor(TRUE);
    ClipCursor(NULL);

    NMHDR nmhdr;

    nmhdr.hwndFrom = (HWND)this;
    nmhdr.idFrom = (UINT)m_command;
    nmhdr.code = (UINT)DIAL_BUTTON_UP;


    SendMessage(Window(), WM_NOTIFY, (WPARAM)m_command, (LPARAM) &nmhdr);
}

void
DialView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}

void 
DialView::
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
                        src_x + m_offset,
                        src_y);

       
    }
}
