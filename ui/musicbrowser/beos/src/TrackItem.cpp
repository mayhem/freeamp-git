/*____________________________________________________________________________

        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1999, 2000 EMusic.com

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

#include "TrackItem.h"
#include "metadata.h"
#include "playlist.h"
#include <be/interface/View.h>
#define DEBUG 1
#include <be/support/Debug.h>
#include <stdio.h>

TrackItem::TrackItem( PlaylistItem* item )
:   CatalogItem( CatalogItem::ITEM_TRACK ),
    BStringItem( "", 0, false ),
    m_isPlaying( false ),
    m_myMusic( false ),
    m_item( item ),
    m_artist( item->GetMetaData().Artist() ),
    m_album( item->GetMetaData().Album() ),
    m_title( item->GetMetaData().Title() ),
    m_url( item->URL() )
{
    string text = m_title + " - " + m_artist;
    SetText( text.c_str() );
}

TrackItem::TrackItem( const TrackItem& rhs )
:   CatalogItem( CatalogItem::ITEM_TRACK ),
    BStringItem( rhs.Text(), 0, false ),
    m_isPlaying( false ),
    m_myMusic( rhs.IsMyMusicItem() ),
    m_item( rhs.Item() ),
    m_artist( rhs.Artist() ),
    m_album( rhs.Album() ),
    m_title( rhs.Title() ),
    m_url( rhs.URL() )
{
}

TrackItem::~TrackItem()
{
}

void
TrackItem::DrawItem( BView* owner, BRect frame, bool complete )
{
    if ( m_myMusic )
    {
        BStringItem::DrawItem( owner, frame, complete );
        return;
    }

    owner->PushState();

    if ( m_isPlaying )
        owner->SetFont( be_bold_font );

    if ( IsSelected() )
    {
        owner->SetHighColor( 150, 150, 150, 255 );
        owner->FillRect( frame );
        owner->SetLowColor( 150, 150, 150, 255 );
    }
    else if ( complete )
    {
        owner->SetHighColor( owner->ViewColor() );
        owner->FillRect( frame );
    }

    font_height fontHeight;
    owner->GetFontHeight( &fontHeight );

    owner->SetHighColor( 0, 0, 0, 255 );
    owner->MovePenTo( frame.left, frame.top + fontHeight.ascent );
    owner->DrawString( Text() );

    owner->PopState();
}

void
TrackItem::SetCurrentlyPlaying( bool flag )
{
    PRINT(( "%s, m_isPlaying = %d\n", Text(), flag ));
    m_isPlaying = flag;
}

void
TrackItem::PrintToStream( void ) const
{
    printf( "TrackItem: %s\n", URL() );
    printf( "\tArtist: %s\n\tAlbum: %s\n\tTitle: %s\n",
            Artist(), Album(), Title() );
}

bool
TrackItem::operator==( const TrackItem& rhs ) const
{
    return ( m_artist == rhs.m_artist &&
             m_album == rhs.m_album &&
             m_title == rhs.m_title );
}

bool
TrackItem::operator!=( const TrackItem& rhs ) const
{
    return ! ( *this == rhs );
}

bool
TrackItem::operator==( const PlaylistItem& rhs ) const
{
    return ( m_artist == rhs.GetMetaData().Artist() &&
             m_album == rhs.GetMetaData().Album() &&
             m_title == rhs.GetMetaData().Title() );
}

bool
TrackItem::operator!=( const PlaylistItem& rhs ) const
{
    return ! ( *this == rhs );
}

// vi: set ts=4:
