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

#ifndef INCLUDED_TOOL_BAR_BUTTON_H__
#define INCLUDED_TOOL_BAR_BUTTON_H__

#include <be/interface/Control.h>

class BBitmap;

class ToolBarButton : public BControl
{
public:
                    // takes ownership of bitmaps.
                    ToolBarButton( BBitmap* bitmap, const char* label,
                                   BMessage* message );
    virtual         ~ToolBarButton();
    virtual void    Draw( BRect updateRect );
    virtual void    GetPreferredSize( float* width, float* height );
    virtual void    MouseDown( BPoint point );
    virtual void    MouseMoved( BPoint point, uint32 transit,
                                const BMessage* message );
    virtual void    MouseUp( BPoint point );

protected:

private:
    BBitmap*        m_bitmapEnabledOn;
    BBitmap*        m_bitmapEnabledOff;
    bool            m_mouseOver;
    bool            m_tracking;
};

#endif // INCLUDED_TOOL_BAR_BUTTON_H__
