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

#ifndef _BUTTONVIEW_H_
#define _BUTTONVIEW_H_

#include "config.h"
#include "view.h"

typedef enum{
    Unpressed,
    Pressed
}ButtonState;

#define ButtonType_PushButton       0       
#define ButtonType_ToggleButton     2



class ButtonView : public View {
 public:
    ButtonView( HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* viewBitmap,
                int32 command = -1,
                int32 flags = 0);

    ButtonView( HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* viewBitmap,
                int32 command = -1,
                int32 flags = 0);

    virtual ~ButtonView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);
    virtual void MouseEntered(void);
    virtual void MouseLeft(void);
    virtual void MouseMove(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

    virtual void Invoke(void);

 protected:
    
 private:
    DIB*            m_viewBitmap;
    int32           m_offset;
    ButtonState     m_state;
    int32           m_command;
    int32           m_flags;
};

#endif /* _BUTTONVIEW_H_ */