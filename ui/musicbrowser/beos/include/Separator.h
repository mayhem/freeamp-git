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

#ifndef INCLUDE_SEPARATOR_H__
#define INCLUDE_SEPARATOR_H__

#include <be/interface/View.h>
#include <vector>
using namespace std;

class Separator : public BView
{
public:
                    Separator( BRect frame, const char* name,
                               uint32 resizingMode );
    virtual         ~Separator();
    virtual void    Draw( BRect updateRect );
    virtual void    FrameResized( float width, float height );
    virtual void    GetPreferredSize( float* width, float* height );
    virtual void    MouseDown( BPoint p );
    virtual void    MouseMoved( BPoint p, uint32 transit,
                                const BMessage* message );
    virtual void    MouseUp( BPoint p );

    void            AddAttachment( BView* prev, BView* next ); // could be NULL

protected:

private:
    static uint8    s_cursorBits[68];
    bool            m_tracking;
    BPoint          m_hotSpot;
    vector<BView*>  m_prev;
    vector<BView*>  m_next;
};

#endif // INCLUDE_SEPARATOR_H__
