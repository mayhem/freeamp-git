/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#include "CanvasView.h"
#include "BeOSWindow.h"
#include "Types.h"
#define DEBUG 0
#include <be/support/Debug.h>
#include <be/interface/Window.h>

#define CHECK_POINT CHECK_POINT_MSG("")
#define CHECK_POINT_MSG(a) PRINT(( "File %s Line %d, %x: %s\n", __FILE__, __LINE__, this, a ))

CanvasView::CanvasView(
            BeOSWindow* parent,
            BRect       frame,
            const char* name,
            uint32      resizingMode,
            uint32      flags
            )
:   BView( frame, name, resizingMode, flags ),
    m_parent( parent ),
    m_bitmap( NULL )
{
}

CanvasView::~CanvasView()
{
}

void
CanvasView::AttachedToWindow( void )
{
    BView::AttachedToWindow();
    SetViewColor( B_TRANSPARENT_COLOR );
    PRINT(( "CanvasView %x attached to window %x\n", this, Window() ));
}

void
CanvasView::Draw( BRect updateRect )
{
    if ( !m_bitmap ) return;

    SetDrawingMode( B_OP_OVER );

    MovePenTo( B_ORIGIN );
    DrawBitmap( m_bitmap );
}

void
CanvasView::MouseDown( BPoint p )
{
    Window()->Activate( true );

    SetMouseEventMask( B_POINTER_EVENTS, B_NO_POINTER_HISTORY );

    ConvertToScreen( &p );

    Pos pos;
    pos.x = (int)p.x;
    pos.y = (int)p.y;
    m_parent->HandleMouseLButtonDown( pos );
}

void
CanvasView::MouseMoved(
        BPoint          p,
        uint32          transit,
        const BMessage* message
        )
{
    ConvertToScreen( &p );
    Pos pos;
    pos.x = int( p.x );
    pos.y = int( p.y );
    m_parent->HandleMouseMove( pos );
}

void
CanvasView::MouseUp( BPoint p )
{
    ConvertToScreen( &p );
    Pos pos;
    pos.x = int( p.x );
    pos.y = int( p.y );
    m_parent->HandleMouseLButtonUp( pos );
}
