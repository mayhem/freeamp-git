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

#ifndef INCLUDED_PLAYLIST_LIST_ITEM_H__
#define INCLUDED_PLAYLIST_LIST_ITEM_H__

#include "CatalogItem.h"
#include <be/interface/ListItem.h>
#include <string>

using namespace std;

class PlaylistListItem : public CatalogItem, public BStringItem
{
public:
                    PlaylistListItem( string url );
    virtual         ~PlaylistListItem();
    virtual void    PrintToStream( void ) const {}
    const char*     URL( void ) const { return m_url.c_str(); }

protected:

private:
    string          m_url;
};

#endif // INCLUDED_PLAYLIST_LIST_ITEM_H__
