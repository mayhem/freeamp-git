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

#include "ToolBarButton.h"
#include <be/interface/Bitmap.h>
#define DEBUG 0
#include <be/support/Debug.h>

const float MARGIN = 3.0;

ToolBarButton::ToolBarButton( BBitmap* bitmap, const char* label,
                              BMessage* message )
:   BControl( bitmap->Bounds(), label, label, message, B_FOLLOW_NONE,
              B_WILL_DRAW ),
    m_bitmapEnabledOn( bitmap ),
    m_mouseOver( false ),
    m_tracking( false )
{
}

ToolBarButton::~ToolBarButton()
{
}

void
ToolBarButton::Draw( BRect updateRect )
{
    BRect bounds( Bounds() );

    if ( m_mouseOver && Value() == B_CONTROL_OFF )
    {
        SetHighColor( 0, 0, 255, 255 );
        StrokeRect( Bounds() );
    }
    else if ( Value() == B_CONTROL_ON )
    {
        SetHighColor( 0, 0, 0, 255 );
        StrokeRect( Bounds() );
    }

    MovePenTo( 0.5 * ( bounds.Width() - m_bitmapEnabledOn->Bounds().Width() ),
               bounds.top + MARGIN );
    SetDrawingMode( B_OP_ALPHA );
    DrawBitmap( m_bitmapEnabledOn );

    // Label.
    font_height fontHeight;
    GetFontHeight( &fontHeight );
    MovePenTo( bounds.left + MARGIN, bounds.bottom - MARGIN );
    SetHighColor( 0, 0, 0, 255 );
    DrawString( Label() );
	SetDrawingMode( B_OP_COPY );
}

void
ToolBarButton::GetPreferredSize( float* width, float* height )
{
    font_height fontHeight;
    GetFontHeight( &fontHeight );

    *width = MAX( StringWidth( Label() ),
                  m_bitmapEnabledOn->Bounds().Width() ) + MARGIN * 2;
    *height = m_bitmapEnabledOn->Bounds().Height() + fontHeight.ascent
              + fontHeight.descent + MARGIN * 2;
}

void
ToolBarButton::MouseDown( BPoint point )
{
    SetMouseEventMask( B_POINTER_EVENTS );
    m_tracking = true;
    SetValue( B_CONTROL_ON );
    Invalidate();
}

void
ToolBarButton::MouseMoved( BPoint point, uint32 transit,
                           const BMessage* message )
{
    bool old = m_mouseOver;
    bool needToInvalidate = false;

    m_mouseOver = ( transit == B_INSIDE_VIEW || transit == B_ENTERED_VIEW );
    needToInvalidate = ( old != m_mouseOver );

    if ( transit == B_ENTERED_VIEW && m_tracking )
    {
        // re-entered while mouse down.
        SetValue( B_CONTROL_ON );
        needToInvalidate = true;
    }
    else if ( transit == B_EXITED_VIEW && m_tracking )
    {
        // exited while mouse down.
        SetValue( B_CONTROL_OFF );
        needToInvalidate = true;
    }

    if ( needToInvalidate ) Invalidate();
}

void
ToolBarButton::MouseUp( BPoint point )
{
    if ( Bounds().Contains( point ) && m_tracking )
    {
        PRINT(( "Invoke\n" ));
        Invoke();
    }
    SetValue( B_CONTROL_OFF );
    Invalidate();

    m_tracking = false;
}

// vi: set ts=4:
