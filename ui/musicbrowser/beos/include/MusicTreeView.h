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

#ifndef INCLUDED_MUSIC_TREE_VIEW_H__
#define INCLUDED_MUSIC_TREE_VIEW_H__

#define TREEVIEWTEST 1

#include <be/interface/OutlineListView.h>
#include "CollectionItem.h"

class TrackItem;
class PlaylistListItem;
class PlaylistItem;
class BPopUpMenu;
class BMenuItem;

class MusicTreeView : public BOutlineListView
{
public:
                    MusicTreeView( BRect frame, const char* name,
                                   list_view_type type
                                       = B_SINGLE_SELECTION_LIST,
                                   uint32 resizingMode = B_FOLLOW_ALL );
	virtual         ~MusicTreeView();
    virtual void    AttachedToWindow( void );
    virtual bool    InitiateDrag( BPoint point, int32 index, bool wasSelected );
    virtual void    MouseDown( BPoint point );

    void            ClearItemsUnder( CollectionItem* group );
    void            ClearMyMusicGroup( void );
    void            ClearPlaylistGroup( void );
    void            AddUnderAllTracksGroup( TrackItem* item );
    void            AddUnderUncategorizedGroup( TrackItem* item );
    void            AddArtistItem( CollectionItem* item );
    void            AddPlaylistListItem( PlaylistListItem* item );

    void            AddTrack( const char* artist, const char* album,
                              PlaylistItem* item );
    bool            RemoveTrack( const char* artist, const char* album,
                                 const PlaylistItem* item );
    void            UpdateTrack( PlaylistItem* oldItem, PlaylistItem* newItem );
    TrackItem*      FindTrackItemUnder( PlaylistItem* item,
                                        CollectionItem* group );
    TrackItem*      FindTrackItemInArtistGroup( PlaylistItem* item );

    uint32          ArtistOutlineLevel( void ) const;
    CollectionItem* MyMusicGroup( void ) const { return m_myMusicGroup; }
    CollectionItem* AllTracksGroup( void ) const { return m_allTracksGroup; }
    CollectionItem* PlaylistGroup( void ) const { return m_playlistGroup; }

protected:

private:
    CollectionItem* m_myMusicGroup;
    CollectionItem* m_allTracksGroup;
    CollectionItem* m_uncategorizedGroup;
    CollectionItem* m_playlistGroup;
    BPopUpMenu*     m_popUp;
    BMenuItem*      m_editMenuItem;
};

inline uint32
MusicTreeView::ArtistOutlineLevel( void ) const
{
    return m_myMusicGroup->OutlineLevel() + 1;
}

#endif // INCLUDED_MUSIC_TREE_VIEW_H__
