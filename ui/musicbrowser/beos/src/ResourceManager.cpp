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

#include "ResourceManager.h"
#include <be/storage/Resources.h>
#include <be/storage/File.h>
#include <be/translation/TranslationUtils.h>
#include <be/support/TypeConstants.h>
#include <be/kernel/image.h>
#include <string.h>
#define DEBUG 1
#include <be/support/Debug.h>

#define REPORT_ERROR(e) PRINT(( "File %s,Line %d: %s\n", __FILE__, __LINE__, strerror(e) ))

ResourceManager::ResourceManager( const char* imageName )
:   m_initCheck( false )
{
    status_t err;
    char path[ MAXPATHLEN ];

    if ( FindPathFor( imageName, path ) < B_OK ) return;

    BFile file( path, B_READ_ONLY );
    if ( ( err = file.InitCheck() ) < B_OK )
    {
        REPORT_ERROR( err );
        return;
    }

    if ( ( err = m_resources.SetTo( &file ) ) < B_OK )
    {
        REPORT_ERROR( err );
    }

    m_initCheck = true;
}

ResourceManager::~ResourceManager()
{
}

BBitmap*
ResourceManager::GetBitmap( uint32 type, int32 id )
{
    size_t size;
    const void* data;

    data = m_resources.LoadResource( type, id, &size );
    if ( data == NULL ) return NULL;

    BMemoryIO resIO( data, size );
    return BTranslationUtils::GetBitmap( &resIO );
}

const char*
ResourceManager::GetString( int32 id )
{
    size_t size;
    const void* data;

    return m_resources.LoadResource( B_STRING_TYPE, id, &size );
}

status_t
ResourceManager::FindPathFor( const char* imageName, char* path )
{
    status_t   err;
    image_info info;
    int32      cookie = 0;

    while ( ( err = get_next_image_info( 0, &cookie, &info ) ) == B_OK )
    {
        if ( strstr( info.name, imageName ) != NULL )
        {
            strcpy( path, info.name );
            PRINT(( "found %s matching %s\n", path, imageName ));
            return B_OK;
        }
    }

    REPORT_ERROR( err );
    return err;
}

// vi: set ts=4:
