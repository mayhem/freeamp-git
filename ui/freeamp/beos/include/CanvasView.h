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

#ifndef INCLUDED_BEOSCANVASVIEW_H
#define INCLUDED_BEOSCANVASVIEW_H

#include <be/interface/View.h>

class BeOSWindow;

class CanvasView : public BView
{
public:
                        CanvasView(
                            BeOSWindow* parent,
                            BRect       frame,
                            const char* name,
                            uint32      resizingMode,
                            uint32      flags
                            );
    virtual             ~CanvasView();
    virtual void        AttachedToWindow( void );
    virtual void        Draw( BRect updateRect );
    virtual void        MouseDown( BPoint p );
    virtual void        MouseMoved(
                            BPoint          p,
                            uint32          transit,
                            const BMessage* message
                            );
    virtual void        MouseUp( BPoint p );

    void                SetCanvasBitmap( BBitmap* bitmap ) { m_bitmap = bitmap; }
    BBitmap*            CanvasBitmap( void ) { return m_bitmap; }

protected:

private:
    BeOSWindow*         m_parent;
    BBitmap*            m_bitmap;
    BPoint              m_movePos;
};

#endif // INCLUDED_BEOSCANVASVIEW_H
