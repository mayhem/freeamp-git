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
#include "CollectionItem.h"
#include "PlaylistListItem.h"
#include "BeOSMusicBrowser.h"
#include "playlist.h"
#define DEBUG 1
#include <be/support/Debug.h>
#include <vector>
using namespace std;

PlaylistView::PlaylistView( BeOSMusicBrowser* browser,
                            BRect frame, const char* name,
                            uint32 resizingMode )
:   BListView( frame, name, B_SINGLE_SELECTION_LIST, resizingMode ),
    m_browser( browser ),
    m_plm( m_browser->PLM() ),
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
        MovePenTo( frame.LeftBottom() );
        StrokeLine( frame.RightBottom() );
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
            BMessage sm( *SelectionMessage() );
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
        {
            PRINT(( "inserter = %d\n", m_inserter ));
            int32 index = m_inserter + 1;
            CatalogItem* cti;
            if ( message->FindPointer( "CatalogItem", (void**)&cti ) == B_OK )
            {
                if ( cti->Type() == CatalogItem::ITEM_TRACK )
                {
                    TrackItem* ti = dynamic_cast<TrackItem*>( cti );
                    m_plm->AddItem( ti->URL(), index );
                }
                else if ( cti->Type() == CatalogItem::ITEM_COLLECTION )
                {
                    CollectionItem* cli = dynamic_cast<CollectionItem*>( cti );
                    vector<string> urls;
                    m_browser->GetURLsUnder( cli, &urls );
                    m_plm->AddItems( urls, index );
                }
                else if ( cti->Type() == CatalogItem::ITEM_PLAYLIST )
                {
                    PlaylistListItem* pli =
                        dynamic_cast<PlaylistListItem*>( cti );
                    m_plm->AddItem( pli->URL(), index );
                }
            }
            SetInserter( -1 );
            break;
        }
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
        // if the user is draggin something over, activate the inserter.
        SetInserter( IndexOf( point ) );
    }

    if ( transit == B_EXITED_VIEW )
    {
        SetInserter( -1 );
    }
}

void
PlaylistView::MouseUp( BPoint point )
{
    // inserter is reset after the message was delivered to MessageReceived.
    // so nothing is done here.
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
    if ( m_inserter == index ) return;

    InvalidateItem( m_inserter );
    m_inserter = index;
    InvalidateItem( m_inserter );
}

// vi: set ts=4:
