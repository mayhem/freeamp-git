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

#ifndef INCLUDED_TRACK_ITEM_H__
#define INCLUDED_TRACK_ITEM_H__

#include "CatalogItem.h"
#include "playlist.h"
#include "metadata.h"
#include <be/interface/ListItem.h>
#include <string>

using namespace std;

class MetaData;
class PlaylistItem;

class TrackItem : public CatalogItem, public BStringItem
{
public:
                    TrackItem( PlaylistItem* item );
                    TrackItem( const TrackItem& rhs );
    virtual         ~TrackItem();
    virtual void    DrawItem( BView* owner, BRect frame, bool complete );

    virtual void    PrintToStream( void ) const;

    PlaylistItem*   Item( void ) const { return m_item; }
    const char*     Artist( void ) const { return m_item->GetMetaData().Artist().c_str(); }
    const char*     Album( void ) const { return m_item->GetMetaData().Album().c_str(); }
    const char*     Title( void ) const { return m_item->GetMetaData().Title().c_str(); }
    const char*     URL( void ) const { return m_item->URL().c_str(); }
    bool            IsMyMusicItem( void ) const { return m_myMusic; }
    void            SetCurrentlyPlaying( bool flag );
    void            SetPlaylistItem( PlaylistItem* item ) { m_item = item; }

    bool            operator==( const TrackItem& rhs ) const;
    bool            operator!=( const TrackItem& rhs ) const;
    bool            operator==( const PlaylistItem& rhs ) const;
    bool            operator!=( const PlaylistItem& rhs ) const;

protected:

private:
    bool            m_isPlaying;// FIXME: get rid of this
    bool            m_myMusic;
    PlaylistItem*   m_item;
    string          m_artist, m_album, m_title, m_url;
};

#endif // INCLUDED_TRACK_ITEM_H__
