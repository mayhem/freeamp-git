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

#include "PlaylistView.h"
#include "TrackItem.h"
#define DEBUG 1
#include <be/support/Debug.h>

PlaylistView::PlaylistView( BRect frame, const char* name,
                            uint32 resizingMode )
:   BListView( frame, name, B_SINGLE_SELECTION_LIST, resizingMode ),
    m_currentIndex( 0 )
{
}

PlaylistView::~PlaylistView()
{
}

bool
PlaylistView::InitiateDrag( BPoint point, int32 index, bool wasSelected )
{
    PRINT(( "InitiateDrag\n" ));
    return true;
}

void
PlaylistView::SetCurrentlyPlaying( int32 index )
{
    TrackItem* item;
    item = (TrackItem*)ItemAt( m_currentIndex );
    if ( item )
    {
        item->SetCurrentlyPlaying( false );
        InvalidateItem( m_currentIndex );
    }

    m_currentIndex = index;
    item = (TrackItem*)ItemAt( m_currentIndex );
    if ( item )
    {
        PRINT(( "current item = %s\n", item->Text() ));
        item->SetCurrentlyPlaying( true );
        InvalidateItem( m_currentIndex );
    }
}

// vi: set ts=4:
