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

#ifndef INCLUDED_TOOL_BAR_H__
#define INCLUDED_TOOL_BAR_H__

#include <be/interface/View.h>

class BControl;

class ToolBar : public BView
{
public:
                    ToolBar( BRect frame, const char* name,
                             uint32 resizingMode );
    virtual         ~ToolBar();
    // adjusts the height only.
    virtual void    GetPreferredSize( float* width, float* height );
    virtual void    Draw( BRect updateRect );

    virtual void    AddItem( BControl* item );

protected:
    BView*          LastItem( void );

private:
};

#endif // INCLUDED_TOOL_BAR_H__
