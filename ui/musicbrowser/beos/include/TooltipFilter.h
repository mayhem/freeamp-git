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

#ifndef INCLUDED_TOOLTIP_FILTER_H__
#define INCLUDED_TOOLTIP_FILTER_H__

#include <be/app/MessageFilter.h>
#include <be/support/String.h>

class TooltipFilter : public BMessageFilter
{
public:
                            // takes ownerships of the "tooltipMessage"
                            TooltipFilter( BMessage* tooltipMessage,
                                           const char* text );
    virtual                 ~TooltipFilter();
    virtual filter_result   Filter( BMessage* message, BHandler** target );

protected:

private:
    BString                 m_text;
    BMessage*               m_template;
};

#endif // INCLUDED_TOOLTIP_FILTER_H__
