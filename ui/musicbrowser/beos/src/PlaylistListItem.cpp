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

#include "PlaylistListItem.h"
#include <string.h>

PlaylistListItem::PlaylistListItem( string url )
:   CatalogItem( CatalogItem::ITEM_PLAYLIST ),
    BStringItem( "", 0, false ),
    m_url( url )
{
    const char* leaf = strrchr( m_url.c_str(), '/' );
    if ( leaf )
    {
        leaf++;
    }
    else
    {
        leaf = m_url.c_str();
    }
    SetText( leaf );
}

PlaylistListItem::~PlaylistListItem()
{
}

// vi: set ts=4:
