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

#include "Separator.h"
#include <be/app/Application.h>
#include <be/app/Cursor.h>
#define DEBUG 1
#include <be/support/Debug.h>

uint8
Separator::s_cursorBits[68] = {
    16, 1, 7, 7,
    // Cursor
    0x00, 0x00,
    0x01, 0x00,
    0x01, 0x00,
    0x01, 0x00,
    0x05, 0x40,
    0x0d, 0x60,
    0x1d, 0x70,
    0x3d, 0x78,
    0x1d, 0x70,
    0x0d, 0x60,
    0x05, 0x40,
    0x01, 0x00,
    0x01, 0x00,
    0x01, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    // Mask
    0x03, 0x80,
    0x03, 0x80,
    0x03, 0x80,
    0x07, 0xc0,
    0x0f, 0xe0,
    0x1f, 0xf0,
    0x3f, 0xf8,
    0x7f, 0xfc,
    0x3f, 0xf8,
    0x1f, 0xf0,
    0x0f, 0xe0,
    0x07, 0xc0,
    0x03, 0x80,
    0x03, 0x80,
    0x03, 0x80,
    0x00, 0x00,
};

BCursor*
Separator::s_cursor = new BCursor( Separator::s_cursorBits );

Separator::Separator( BRect frame, const char* name, uint32 resizingMode )
:   BView( frame, name, resizingMode, B_WILL_DRAW | B_FRAME_EVENTS ),
    m_tracking( false ),
    m_hotSpot( B_ORIGIN )
{
    SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
}

Separator::~Separator()
{
}

void
Separator::Draw( BRect updateRect )
{
    BRect bounds( Bounds() );
    float x1 = 0.5 * bounds.Width();
    float y1 = 0.5 * bounds.Height() - 15;
    SetHighColor( tint_color( ViewColor(), B_DARKEN_3_TINT ) );
    MovePenTo( x1, y1 );
    StrokeLine( BPoint( x1, y1 + 30 ) );
    SetHighColor( tint_color( ViewColor(), B_LIGHTEN_2_TINT ) );
    MovePenTo( x1 + 1, y1 + 1 );
    StrokeLine( BPoint( x1 + 1, y1 + 30 ) );
}

void
Separator::FrameResized( float width, float height )
{
    Invalidate();
}

void
Separator::GetPreferredSize( float* width, float* height )
{
    *width = 4.0;
    *height = Bounds().Height();
}

void
Separator::MouseDown( BPoint p )
{
    m_tracking = true;
    m_hotSpot = p;
    SetMouseEventMask( B_POINTER_EVENTS, B_NO_POINTER_HISTORY );
}

void
Separator::MouseMoved( BPoint p, uint32 transit, const BMessage* message )
{
    if ( transit == B_ENTERED_VIEW )
    {
        SetViewCursor( s_cursor );
    }

    if ( !m_tracking ) return;

    ConvertToParent( &p );

    // check for limit.
    BView* prevCheck = m_prev.front();
    BView* nextCheck = m_next.front();
    if ( prevCheck && prevCheck->Frame().left >= p.x ) return;
    if ( nextCheck && nextCheck->Frame().right <= p.x ) return;

    MoveTo( p.x - m_hotSpot.x, Frame().top );

    // Resize the neighbors.
    BRect frame( Frame() ); // my frame is fixed by now, so cache it.
    vector<BView*>::iterator i;
    for ( i = m_prev.begin(); i != m_prev.end(); i++ )
    {
        (*i)->ResizeTo( frame.left - (*i)->Frame().left - 1,
                        (*i)->Frame().Height() );
    }
    for ( i = m_next.begin(); i != m_next.end(); i++ )
    {
        (*i)->ResizeTo( (*i)->Frame().right - frame.right - 1,
                        (*i)->Frame().Height() );
        (*i)->MoveTo( frame.right + 1, (*i)->Frame().top );
    }
}

void
Separator::MouseUp( BPoint p )
{
    m_tracking = false;
}

void
Separator::AddAttachment( BView* prev, BView* next )
{
    if ( prev )
    {
        m_prev.push_back( prev );
    }
    if ( next )
    {
        m_next.push_back( next );
    }
}

// vi: set ts=4:
