/*____________________________________________________________________________

   FreeAmp - The Free MP3 Player

   Copyright (C) 1999 EMusic

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

#include "BeOSFont.h"
#define DEBUG 0
#include <be/support/Debug.h>
#include <be/support/String.h>

#define CHECK_POINT CHECK_POINT_MSG("")
#define CHECK_POINT_MSG(a) PRINT(( "File %s Line %d: %s\n", __FILE__, __LINE__, a ))

BeOSFont::BeOSFont( string& oName, string& oFace, string& oFile,
                    string& oDefault )
:   Font( oName, oFace, oFile, oDefault )
{
    CHECK_POINT;
    PRINT(( "%s, %s, %s\n", oName.c_str(), oFace.c_str(), oDefault.c_str() ));

    BString families( oFace.c_str() );
    bool found = false;

    while ( families.Length() > 0 && !found )
    {
        BString familyToFind;
        int32 del = families.FindFirst( ',' );
        if ( del != B_ERROR )
        {
            families.MoveInto( familyToFind, 0, del );
            families.Remove( 0, 1 ); // remove first char ','
        }
        else
        {
            families.MoveInto( familyToFind, 0, families.Length() );
        }

        // find if it's installed
        int32 numFamilies = count_font_families();
        for ( int32 i = 0; i < numFamilies; i++ )
        {
            font_family family;
            uint32 flags;
            if ( get_font_family( i, &family, &flags ) == B_OK )
            {
                if ( oName == family )
                {
                    found = true;
                    break;
                }
            }
        }
    }

    // map the well known font names
    CHECK_POINT;
}

BeOSFont::~BeOSFont()
{
}
