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

#ifndef INCLUDED_BEOSCANVAS_H__
#define INCLUDED_BEOSCANVAS_H__

#include "Canvas.h"
#include "CanvasView.h"

class Font;
class BeOSBitmap;
class BeOSWindow;
class BView;

class BeOSCanvas : public Canvas
{
public:
                        BeOSCanvas( BeOSWindow* pParent );
    virtual             ~BeOSCanvas();
    virtual void        Init( void );
    virtual void        Erase( Rect& oPaintRect );
    virtual int         RenderText( int iFontHeight, Rect& oClipRect,
                                    string& oText, AlignEnum eAlign,
                                    Font* pFont, const Color& oColor,
                                    bool bBold, bool bItalic, bool bUnderline );
    virtual int         RenderOffsetText( int iFontHeight, Rect& oClipRect,
                                          string& oText, int iOffset,
                                          Font* pFont, const Color& oColor,
                                          bool bBold, bool bItalic,
                                          bool bUnderline );
    virtual Error       Invalidate( Rect& oRect );
    virtual Error       Update( void );
    virtual Error       BlitRect( Bitmap* pSrcBitmap, Rect& oSrcRect,
                                  Rect& oDestRec );
    virtual Error       MaskBlitRect( Bitmap* pSrcBitmap, Rect& oSrcRect,
                                      Rect& oDestRect );

    BView*              GetBView( void ) { return m_canvasView; }

protected:

private:
    BeOSWindow*         m_pParent;
    BeOSBitmap*         m_pBufferBitmap;
    CanvasView*         m_canvasView;
};

#endif // INCLUDED_BEOSCANVAS_H__
