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

#ifndef _SCROLLVIEW_H_
#define _SCROLLVIEW_H_

#include "config.h"
#include "view.h"


#define ScrollType_Horizontal       0       
#define ScrollType_Vertical         2



class ScrollView : public View {
 public:
    ScrollView( HWND hwnd, 
                View* parent, 
                HRGN viewRegion, 
                DIB* viewBitmap,
                int32 flags = 0);

    ScrollView( HWND hwnd, 
                View* parent, 
                RECT* viewRect, 
                DIB* viewBitmap,
                int32 flags = 0);

    virtual ~ScrollView();

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect);
    virtual void MouseEntered(void);
    virtual void MouseLeft(void);
    virtual void MouseMove(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers);
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers);

            View* Target() const;
            void SetTarget(View* target);

            int32 Position() const;
            void SetPosition(int32 position);

            void Range(int32* min, int32* max) const;
            void SetRange(int32 min, int32 max);

		    void Steps(uint32* smallStep, uint32* largeStep) const;
            void SetSteps(uint32 smallStep, uint32 largeStep);

            int32 Proportion() const;
            void SetProportion(int32 visible);

            int32 Orientation() const;

 protected:
     void SetPositionInternal(int32 position, bool notifyTarget);
     void DetermineThumbRect();
    
 private:
    DIB*            m_viewBitmap;
    bool            m_pressed;
    int32           m_flags;

    int32           m_orientation;
    int32           m_min;
    int32           m_max;
    int32           m_position;
    uint32          m_smallStep;
    uint32          m_largeStep;

    
    View*           m_target;
    RECT            m_thumbRect;
    POINT           m_pressedPoint;
    POINT           m_lastPoint;

    int32           m_visible;
};

#endif /* _SCROLLVIEW_H_ */