/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 1998 GoodNoise

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

#ifndef _BITMAPVIEW_H_
#define _BITMAPVIEW_H_

#include "config.h"
#include "view.h"

class BitmapView : public View {
 public:
    BitmapView( HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* viewBitmap,
                DIB* maskBitmap = NULL,
                int32 command = -1);

    BitmapView( HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* viewBitmap,
                DIB* maskBitmap = NULL,
                int32 command = -1);

    ~BitmapView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);

    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

    virtual void Invoke(void);

 protected:
    
 private:
    DIB*    m_viewBitmap;
    DIB*    m_maskBitmap;

    int32   m_command;
    bool    m_pressed;
    
};

#endif /* _BITMAPVIEW_H_ */