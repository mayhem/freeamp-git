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

#ifndef INCLUDED_COLLECTION_ITEM_H__
#define INCLUDED_COLLECTION_ITEM_H__

#include "CatalogItem.h"
#include <be/interface/ListItem.h>
#include <string>
#include <list>
#include <vector>
using namespace std;

class CollectionItem : public CatalogItem, public BStringItem
{
public:
                        CollectionItem( const char* name,
                                        uint32 outlineLevel = 0 );
                        CollectionItem( const CollectionItem& rhs );
    virtual             ~CollectionItem();
    virtual void        PrintToStream( void ) const;

    bool                AddItem( CatalogItem* item );
    void                GetURLs( vector<string>* urls );

protected:

private:
    list<CatalogItem*>  m_list;
};

#endif // INCLUDED_COLLECTION_ITEM_H__
