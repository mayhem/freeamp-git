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

/* project headers */
#include "config.h"
#include "control.h"

Control::
Control(HWND hwndParent,
        HRGN controlRegion,
        HBITMAP controlBitmap)
{
    m_hwndParent    = hwndParent;
    m_controlRegion = controlRegion;
    m_controlBitmap = controlBitmap;

    m_enabled = true;
}

Control::
~Control()
{
    m_hwndParent    = NULL;
    m_controlRegion = NULL;
    m_controlBitmap = NULL;
}

bool 
Control::
PointInControl(int32 x, int32 y)
{
    assert(m_controlRegion);
    return (PtInRegion(m_controlRegion, x, y) == TRUE);
}



