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

#include "CollectionItem.h"
#define DEBUG 1
#include <be/support/Debug.h>

CollectionItem::CollectionItem( const char* name, uint32 outlineLevel )
:   CatalogItem( CatalogItem::ITEM_COLLECTION ),
    BStringItem( name, outlineLevel, false )
{
}

CollectionItem::CollectionItem( const CollectionItem& rhs )
:   CatalogItem( CatalogItem::ITEM_COLLECTION ),
    BStringItem( rhs.Text(), rhs.OutlineLevel(), false )
{
}

CollectionItem::~CollectionItem()
{
}

void
CollectionItem::PrintToStream( void ) const
{
    printf( "Collections\n" );
}

bool
CollectionItem::AddItem( CatalogItem* item )
{
    if ( item->Type() != CatalogItem::ITEM_TRACK &&
         item->Type() != CatalogItem::ITEM_COLLECTION )
    {
        PRINT(( "CollectionItem only accepts COLLECTION and TRACK types\n" ));
        return false;
    }

    m_list.push_back( item );

    return true;
}

void
CollectionItem::GetURLs( vector<string>* urls )
{
}

// vi: set ts=4:
