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

#include "MusicTreeView.h"
#include "BeOSMusicBrowser.h"
#include "CatalogItem.h"
#include "TrackItem.h"
#include "CollectionItem.h"
#include "PlaylistListItem.h"
#include <be/interface/PopUpMenu.h>
#include <be/interface/MenuItem.h>
#include <be/interface/Window.h>
#define DEBUG 1
#include <be/support/Debug.h>

MusicTreeView::MusicTreeView( BRect frame, const char* name,
                              list_view_type type, uint32 resizingMode )
:   BOutlineListView( frame, name, type, resizingMode ),
    m_myMusicGroup( NULL ),
    m_allTracksGroup( NULL ),
    m_uncategorizedGroup( NULL ),
    m_playlistGroup( NULL ),
    m_popUp( NULL )
{
    m_myMusicGroup = new CollectionItem( "My Music" );
    AddItem( m_myMusicGroup );
    m_uncategorizedGroup = new CollectionItem( "Uncategorized Tracks" );
    AddUnder( m_uncategorizedGroup, m_myMusicGroup );
    m_allTracksGroup = new CollectionItem( "All Tracks" );
    AddUnder( m_allTracksGroup, m_myMusicGroup );
    m_playlistGroup = new CollectionItem( "My Playlists" );
    AddItem( m_playlistGroup );
}

MusicTreeView::~MusicTreeView()
{
    // Empty list
    for ( int i = CountItems() - 1; i >= 0; i-- )
    {
        delete ItemAt( i );
    }
}

void
MusicTreeView::AttachedToWindow( void )
{
    BOutlineListView::AttachedToWindow();
}

bool
MusicTreeView::InitiateDrag( BPoint point, int32 index, bool wasSelected )
{
    CatalogItem* item = dynamic_cast<CatalogItem*>( ItemAt( index ) );
    if ( item )
    {
        BMessage dragMsg( 'drag' );
        puts( "Drag begin" );
        return true;
    }

    return false;
}

void
MusicTreeView::MouseDown( BPoint point )
{
    uint32 buttons, index;

    BMessage* cm = Window()->CurrentMessage();
    cm->FindInt32( "buttons", (int32*)&buttons );

    if ( buttons & B_SECONDARY_MOUSE_BUTTON &&
         ( index = IndexOf( point ) ) >= 0 )
    {
        // Context-sensitive pop-up menu.
        if ( !m_popUp )
        {
            m_popUp = new BPopUpMenu( "CatalogPopUp", false, false );
            m_popUp->SetFont( be_plain_font );
            m_editMenuItem = new BMenuItem( "Edit", NULL );
            m_editMenuItem->SetTarget( Window(), Window() );
            m_popUp->AddItem( m_editMenuItem );
        }
        PRINT(( "index = %d\n", index ));
        CatalogItem* item = dynamic_cast<CatalogItem*>( ItemAt( index ) );
        if ( item )
        {
            PRINT(( "item type %d\n", item->Type() ));
            BMessage* editMsg = new BMessage( MBMSG_EDIT_CATALOG_ITEM );
            editMsg->AddPointer( "item", item );
            m_editMenuItem->SetMessage( editMsg );
        }
        ConvertToScreen( &point );
        BRect clickToOpenRect( -10, -10, 10, 10 );
        clickToOpenRect.OffsetTo( point );
        m_popUp->Go( point, true, true, clickToOpenRect, true );
    }

    BOutlineListView::MouseDown( point );
}

void
MusicTreeView::ClearItemsUnder( CollectionItem* group )
{
    LockLooper();

    vector<BListItem*> itemsToBeDeleted;
    int32 count = CountItemsUnder( group, false );
    for ( int32 i = 0; i < count; i++ )
    {
        itemsToBeDeleted.push_back( ItemUnderAt( group, false, i ) );
    }

    int32 index = FullListIndexOf( group );
    RemoveItem( group ); // this removes all the subitems as well.
    AddItem( group, index );

    // then dispose of the items themselves.
    vector<BListItem*>::iterator i;
    for ( i = itemsToBeDeleted.begin(); i != itemsToBeDeleted.end(); i++ )
    {
        delete *i;
    }

    UnlockLooper();
}

void
MusicTreeView::ClearMyMusicGroup( void )
{
    ClearItemsUnder( m_myMusicGroup );

    m_uncategorizedGroup = new CollectionItem( "Uncategorized Tracks" );
    AddUnder( m_uncategorizedGroup, m_myMusicGroup );
    m_allTracksGroup = new CollectionItem( "All Tracks" );
    AddUnder( m_allTracksGroup, m_myMusicGroup );
}

void
MusicTreeView::ClearPlaylistGroup( void )
{
    ClearItemsUnder( m_playlistGroup );
}

void
MusicTreeView::AddUnderAllTracksGroup( TrackItem* item )
{
    AddUnder( item, m_allTracksGroup );
}

void
MusicTreeView::AddUnderUncategorizedGroup( TrackItem* item )
{
    AddUnder( item, m_uncategorizedGroup );
}

void
MusicTreeView::AddArtistItem( CollectionItem* item )
{
    int32 index = FullListIndexOf( m_playlistGroup );
    AddItem( item, index );
}

void
MusicTreeView::AddPlaylistListItem( PlaylistListItem* item )
{
    AddUnder( item, m_playlistGroup );
}

TrackItem*
MusicTreeView::FindTrackItemUnder( PlaylistItem* item, CollectionItem* group  )
{
    TrackItem* ti = NULL;
    bool found = false;
    int32 count = CountItemsUnder( group, false );
    PRINT(( "there're %d items under this\n", count ));
    for ( int32 i = 0; i < count; i++ )
    {
        BListItem* li = ItemUnderAt( group, false, i );
        CatalogItem* ci = dynamic_cast<CatalogItem*>( li );
        if ( ci->Type() == CatalogItem::ITEM_TRACK )
        {
            ti = dynamic_cast<TrackItem*>( ci );
            if ( ti->Item() == item )
            {
                PRINT(( "Found in group: %s\n", ti->URL() ));
                found = true;
                break;
            }
        }
    }

    return found ? ti : NULL;
}

TrackItem*
MusicTreeView::FindTrackItemInArtistGroup( PlaylistItem* item )
{
    // Uncategorized Tracks Group is the group immediately before artists.
    int32 head = FullListIndexOf( m_uncategorizedGroup );
    head += CountItemsUnder( m_uncategorizedGroup, false ) + 1;
    // At this point, head is the index to the very first artist group.

    // To get the the last index of all the artist groups, use the index to
    // the last item under My Music group. (Artist groups are the last group
    // in My Music.)
    int32 tail = CountItemsUnder( m_myMusicGroup, false ) + 1;

    TrackItem* ti = NULL;
    bool found = false;
    for ( int32 i = head; i <= tail; i++ )
    {
        BListItem* li = FullListItemAt( i );
        CatalogItem* ci = dynamic_cast<CatalogItem*>( li );
        if ( ci->Type() == CatalogItem::ITEM_TRACK )
        {
            ti = dynamic_cast<TrackItem*>( ci );
            if ( ti->Item() == item )
            {
                PRINT(( "Found in artist group: %s\n", ti->URL() ));
                found = true;
                break;
            }
        }
    }

    return found ? ti : NULL;
}

bool
MusicTreeView::RemoveTrack( const char* artist, const char* album,
                            const PlaylistItem* item )
{
    TrackItem* ti;
    bool found = false;

    ti = FindTrackItemUnder( item, AllTracksGroup() );
    if ( ti )
    {
        PRINT(( "found at %d in all track group\n", FullListIndexOf( ti ) ));
        found = true;
        RemoveItem( ti );
    }

    ti = FindTrackItemInArtistGroup( item );
    if ( ti )
    {
        PRINT(( "found at %d in artist group\n", FullListIndexOf( ti ) ));
        found = true;
        RemoveItem( ti );
    }

    return found;
}

void
MusicTreeView::UpdateTrack( PlaylistItem* oldItem, PlaylistItem* newItem )
{
    TrackItem* ti;

    ti = FindTrackItemUnder( oldItem, AllTracksGroup() );
    if ( ti )
    {
        ASSERT( oldItem == ti->Item() );
        PRINT(( "Updating in AllTracksGroup %s\n", ti->URL() ));
        ti->SetPlaylistItem( newItem );
    }

    ti = FindTrackItemInArtistGroup( oldItem );
    if ( ti )
    {
        ASSERT( oldItem == ti->Item() );
        PRINT(( "Updating in ArtistGroups %s\n", ti->URL() ));
        ti->SetPlaylistItem( newItem );
    }
}

// vi: set ts=4:
