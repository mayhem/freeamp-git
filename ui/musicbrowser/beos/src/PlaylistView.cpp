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
#include "BeOSMusicBrowser.h"
#define DEBUG 1
#include <be/support/Debug.h>

PlaylistView::PlaylistView( BRect frame, const char* name,
                            uint32 resizingMode )
:   BListView( frame, name, B_SINGLE_SELECTION_LIST, resizingMode ),
    m_currentIndex( 0 ),
    m_inserter( -1 )
{
    SetSelectionMessage( new BMessage( MBMSG_SELECTION_CHANGED ) );
}

PlaylistView::~PlaylistView()
{
}

void
PlaylistView::Draw( BRect updateRect )
{
    BListView::Draw( updateRect );

    if ( m_inserter >= 0 )
    {
        BRect frame( ItemFrame( m_inserter ) );
        StrokeRect( frame );
    }
}

bool
PlaylistView::InitiateDrag( BPoint point, int32 index, bool wasSelected )
{
    CatalogItem* item = dynamic_cast<CatalogItem*>( ItemAt( index ) );
    if ( item )
    {
        PRINT(( "Drag begin\n" ));
        BMessage msg( B_SIMPLE_DATA );
        msg.AddPointer( "CatalogItem", item );
        BRect dragRect( ItemFrame( index ) );
        DragMessage( &msg, dragRect, NULL );
        if ( wasSelected )
        {
            BMessage sm( SelectionMessage() );
            sm.AddPointer( "source", this );
            Messenger().SendMessage( &sm );
        }
        return true;
    }
    return false;
}

void
PlaylistView::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        case B_SIMPLE_DATA:
            break;
        default:
            BListView::MessageReceived( message );
            break;
    }
}

void
PlaylistView::MouseMoved( BPoint point, uint32 transit,
                          const BMessage* message )
{
    if ( message )
    {
        SetInserter( IndexOf( point ) );
    }
}

void
PlaylistView::MouseUp( BPoint point )
{
    SetInserter( -1 );
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

void
PlaylistView::SetInserter( int32 index )
{
    PRINT(( "setting inserter at %d\n", index ));
    InvalidateItem( m_inserter );
    m_inserter = index;
    InvalidateItem( m_inserter );
}

// vi: set ts=4:
