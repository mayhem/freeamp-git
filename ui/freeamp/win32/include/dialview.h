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

#ifndef _DialView_H_
#define _DialView_H_

#include "config.h"
#include "view.h"

#define DIAL_BUTTON_DOWN    0
#define DIAL_BUTTON_UP      1
#define DIAL_MOVE           2


class DialView : public View {
 public:
    DialView( HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* viewBitmap,
                int32 command = -1);

    DialView( HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* viewBitmap,
                int32 command = -1);

    virtual ~DialView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);
    virtual void MouseEntered(void);
    virtual void MouseLeft(void);
    virtual void MouseMove(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

    virtual void Invoke(void);

    int32 Position() const { return m_position; }
    void SetPosition(int32 position);

 protected:
    
 private:
    DIB*            m_viewBitmap;
    int32           m_position;
    int32           m_offset;
    bool            m_pressed;
    int32           m_command;
    POINT           m_clickPoint;
    POINT           m_screenPoint;

    HCURSOR         m_dialCursor;
    HCURSOR         m_oldCursor;
};

#endif /* _DialView_H_ */