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

#include "ToolBar.h"
#include <be/interface/Control.h>
#define DEBUG 1
#include <be/support/Debug.h>

ToolBar::ToolBar( BRect frame, const char* name, uint32 resizingMode )
:   BView( frame, name, resizingMode, B_WILL_DRAW )
{
    SetViewColor( ui_color( B_PANEL_BACKGROUND_COLOR ) );
}

ToolBar::~ToolBar()
{
}

void
ToolBar::GetPreferredSize( float* width, float* height )
{
    // adjust the height only.
    *height = 0.0;

    for ( int32 i = 0; i < CountChildren(); i++ )
    {
        BView* v = ChildAt( i );
        *height = MAX( *height, v->Bounds().Height() );
    }
    *height += 4; // for fancy borders, 2 pixels each on top and bottom.
    PRINT(( "toolbar height = %f\n", *height ));

    *width = Bounds().Width();
}

void
ToolBar::Draw( BRect updateRect )
{
    BRect bounds( Bounds() );

    SetHighColor( tint_color( ui_color( B_PANEL_BACKGROUND_COLOR ),
                              B_LIGHTEN_2_TINT ) );
    MovePenTo( bounds.LeftTop() );
    StrokeLine( bounds.RightTop() );
    MovePenTo( bounds.LeftBottom() );
    StrokeLine( bounds.RightBottom() );
    SetHighColor( tint_color( ui_color( B_PANEL_BACKGROUND_COLOR ),
                              B_DARKEN_2_TINT ) );
    MovePenTo( 0.0, bounds.bottom - 1 );
    StrokeLine( BPoint( bounds.right, bounds.bottom - 1 ) );
}

void
ToolBar::AddItem( BControl* item )
{
    float w, h;
    const float MARGIN = 5;

    item->ResizeToPreferred();
    item->GetPreferredSize( &w, &h );

    BView* last = LastItem();
    float left = last ? ( last->Frame().right + MARGIN ) : MARGIN;
    item->MoveTo( left, 2 );

    AddChild( item );
}

BView*
ToolBar::LastItem( void )
{
    uint32 total = CountChildren();
    if ( total == 0 ) return NULL;

    return ChildAt( total - 1 );
}

// vi: set ts=4:
