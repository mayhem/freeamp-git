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

#include "TooltipFilter.h"
#include <be/app/Message.h>
#include <be/app/Looper.h>
#include <be/interface/View.h>
#include <stdio.h>

TooltipFilter::TooltipFilter( BMessage* tooltipMessage, const char* text )
:   BMessageFilter( B_ANY_DELIVERY, B_ANY_SOURCE ),
    m_text( text ),
    m_template( tooltipMessage )
{
}

TooltipFilter::~TooltipFilter()
{
    delete m_template;
}

filter_result
TooltipFilter::Filter( BMessage* message, BHandler** target )
{
    if ( message->what != B_MOUSE_MOVED ) return B_DISPATCH_MESSAGE;

    uint32 transit;
    if ( message->FindInt32( "be:transit", (int32*)&transit ) < B_OK )
    {
        return B_DISPATCH_MESSAGE;
    }

    switch ( transit )
    {
        case B_ENTERED_VIEW:
        {
            BMessage tip( *m_template );
            tip.AddString( "text", m_text.String() );
            Looper()->PostMessage( &tip );
            break;
        }
        case B_EXITED_VIEW:
        {
            BMessage tip( *m_template );
            tip.AddString( "text", "" );
            Looper()->PostMessage( &tip );
            break;
        }
        default:
            break;
    }

    return B_DISPATCH_MESSAGE;
}

// vi: set ts=4:
