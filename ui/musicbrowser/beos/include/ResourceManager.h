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

#ifndef INCLUDED_RESOURCE_MANAGER_H__
#define INCLUDED_RESOURCE_MANAGER_H__

// handles resource retrieval from add-on images.

#include <be/storage/Resources.h>

class BBitmap;

class ResourceManager
{
public:
                    ResourceManager( const char* imageName );
    virtual         ~ResourceManager();
    bool            InitCheck( void ) const { return m_initCheck; }
    BBitmap*        GetBitmap( uint32 type, int32 id );
    const char*     GetString( int32 id );

protected:
    status_t        FindPathFor( const char* imageName, char* path );

private:
    bool            m_initCheck;
    BResources      m_resources;
};

#endif // INCLUDED_RESOURCE_MANAGER_H__
