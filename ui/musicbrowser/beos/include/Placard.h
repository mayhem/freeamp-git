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

#ifndef INCLUDED_PLACARD_H__
#define INCLUDED_PLACARD_H__

#include <be/interface/View.h>
#include <be/support/String.h>

class Placard : public BView
{
public:
                    Placard( BRect frame, const char* name,
                             uint32 resizingMode = B_FOLLOW_ALL,
                             uint32 flags = B_WILL_DRAW );
    virtual         ~Placard();
    virtual void    Draw( BRect updateRect );

    void            SetText( const char* text );

protected:

private:
    BString         m_text;
};

#endif // INCLUDED_PLACARD_H__
