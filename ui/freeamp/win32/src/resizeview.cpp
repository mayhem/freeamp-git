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
#include "resizeview.h"
#include "renderer.h"
#include "resource.h"
#include "freeampui.h"


ResizeView::
ResizeView( HWND hwnd,
            View* parent,
            HRGN viewRegion,
            int32 command):
View(hwnd, parent, viewRegion)
{
    m_offset = 0;
    m_command = command;
    m_pressed = false;

    HINSTANCE hinst = (HINSTANCE)GetWindowLong(Window(), GWL_HINSTANCE);
    m_resizeCursor = LoadCursor(NULL, IDC_SIZEWE);
    m_oldCursor = NULL;

    GetWindowRect(Window(), &m_windowRect);
}

ResizeView::
ResizeView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            int32 command):
View(hwnd, parent, viewRect)
{
    m_offset = 0;
    m_command = command;
    m_pressed = false;


    HINSTANCE hinst = (HINSTANCE)GetWindowLong(Window(), GWL_HINSTANCE);
    m_resizeCursor = LoadCursor(NULL, IDC_SIZEWE);
    m_oldCursor = NULL;

    GetWindowRect(Window(), &m_windowRect);
}

ResizeView::
~ResizeView()
{
}

void 
ResizeView::
MouseEntered()
{
    FreeAmpUI* ui = (FreeAmpUI*)GetWindowLong(Window(), GWL_USERDATA);

    m_oldCursor = ui->SetCursor(m_resizeCursor);
}

void 
ResizeView::
MouseLeft()
{
    FreeAmpUI* ui = (FreeAmpUI*)GetWindowLong(Window(), GWL_USERDATA);

    ui->SetCursor(m_oldCursor);
}

void 
ResizeView::
MouseMove(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed)
    {
        int32 delta = 0;

        delta = x - m_clickPoint.x;
        m_clickPoint.x = x;

        RECT rect;

        GetWindowRect(Window(), &rect);

        char buf[256];

        wsprintf(buf, "l = %d; t = %d; r = %d; b = %d\r\n", rect.left, 
                                                            rect.top, 
                                                            rect.right, 
                                                            rect.bottom);
        OutputDebugString(buf);

        int32 height, width;

        height = rect.bottom - rect.top;
        width = rect.right - rect.left + delta;

        if(width < m_windowRect.right - m_windowRect.left)
        {
            width = m_windowRect.right - m_windowRect.left;
        }

        SetWindowPos(   Window(),
                        NULL,
                        rect.left,
                        rect.top,
                        width,
                        height,
                        SWP_NOZORDER | SWP_NOMOVE);
    }
}

void 
ResizeView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;
    m_clickPoint.x = x;
    m_clickPoint.y = y;
}

void 
ResizeView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    m_pressed = false;  
}

void
ResizeView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}
