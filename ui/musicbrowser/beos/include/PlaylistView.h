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

#ifndef INCLUDED_PLAYLIST_VIEW_H__
#define INCLUDED_PLAYLIST_VIEW_H__

#include <be/interface/ListView.h>

class PlaylistView : public BListView
{
public:
                    PlaylistView( BRect frame, const char* name,
                                  uint32 resizingMode );
    virtual         ~PlaylistView();
    virtual void    Draw( BRect updateRect );
    virtual bool    InitiateDrag( BPoint point, int32 index, bool wasSelected );
    virtual void    MessageReceived( BMessage* message );
    virtual void    MouseMoved( BPoint point, uint32 transit,
                                const BMessage* message );
    virtual void    MouseUp( BPoint point );

    void            SetCurrentlyPlaying( int32 index );

protected:
    void            SetInserter( int32 index ); // if negative, removes it.

private:
    int32           m_currentIndex;
    int32           m_inserter;
};

#endif // INCLUDED_PLAYLIST_VIEW_H__
