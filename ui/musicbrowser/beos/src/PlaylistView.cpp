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
#include "utility.h"
#define DEBUG 1
#include <be/support/Debug.h>
#include <be/storage/Entry.h>
#include <be/storage/Path.h>
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
    if ( item && wasSelected )
    {
        PRINT(( "Drag begin\n" ));
        BMessage msg( MBMSG_DRAGGED );
        msg.AddPointer( "CatalogItem", item );
        msg.AddPointer( "source", this );
        msg.AddInt32( "index", index );
        BRect dragRect( ItemFrame( index ) );
        DragMessage( &msg, dragRect, NULL );
        return true;
    }

    return BListView::InitiateDrag( point, index, wasSelected );
}

void
PlaylistView::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        case MBMSG_DRAGGED:
        {
            PRINT(( "inserter = %d\n", m_inserter ));
            int32 index = m_inserter + 1;
            CatalogItem* cti;
            if ( message->FindPointer( "CatalogItem", (void**)&cti ) == B_OK )
            {
                if ( cti->Type() == CatalogItem::ITEM_TRACK )
                {
                    TrackItem* ti = dynamic_cast<TrackItem*>( cti );
                    PlaylistView* src;
                    if ( message->FindPointer( "source", (void**)&src ) == B_OK
                         && src == this )
                    {
                        m_plm->MoveItem( message->FindInt32( "index" ),
                                         m_inserter );
                        Select( m_inserter );
                    }
                    else
                    {
                        m_plm->AddItem( ti->URL(), index );
                    }
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
        case B_SIMPLE_DATA:
        {
            int32 index = m_inserter + 1;
            entry_ref ref;
            int32 count = 0;
            while ( message->FindRef( "refs", count++, &ref ) == B_OK )
            {
                BPath path( &ref );
                if ( path.InitCheck() < B_OK )
                    continue;
                size_t urlLen = strlen( path.Path() ) + 10;
                char* url = new char[ urlLen ];
                if ( FilePathToURL( path.Path(), url, &urlLen )
                     == kError_BufferTooSmall )
                {
                    delete[] url;
                    url = new char[ urlLen ];
                    FilePathToURL( path.Path(), url, &urlLen );
                }
                PRINT(( "dropped: %s\n", url ));
                m_plm->AddItem( url, index );
                delete[] url;
            }
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
    PRINT(( "PlaylistView::SetInserter: inserter = %d\n", m_inserter ));
}

// vi: set ts=4:
