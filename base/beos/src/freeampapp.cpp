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

#include "config.h"
#include "freeampapp.h"
#include "facontext.h"
#include "player.h"
#include <be/storage/Entry.h>
#include <be/storage/Path.h>

FreeAmpApp::FreeAmpApp( const char* signature, FAContext* context )
:   BApplication( signature ),
    m_context( context )
{
}

FreeAmpApp::~FreeAmpApp()
{
}

void
FreeAmpApp::MessageReceived( BMessage* message )
{
    switch ( message->what )
    {
        default:
            BApplication::MessageReceived( message );
            break;
    }
}

void
FreeAmpApp::RefsReceived( BMessage* message )
{
    int32 i = 0;
    entry_ref ref;
    while ( message->FindRef( "refs", i, &ref ) == B_OK )
    {
        BEntry entry( &ref );
        if ( entry.InitCheck() == B_OK )
        {
            BPath path;
            entry.GetPath( &path );
            m_context->player->HandleSingleArg( (char*)path.Path() );
        }
        i++;
    }
}

// vi: set ts=4:
