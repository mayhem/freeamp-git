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

#ifndef _VIEW_H_
#define _VIEW_H_

class ScrollView;

#include "config.h"
#include "dib.h"

class View {
 public:
    View(HWND hwnd, View* parent, HRGN viewRegion);
    View(HWND hwnd, View* parent, RECT* viewRect);

    virtual ~View();

    // Deal with mouse stuff
    virtual void MouseEntered(void){}
    virtual void MouseLeft(void){}
    virtual void MouseMove(int32 x, int32 y, int32 modifiers){}
    virtual void LeftButtonDown(int32 x, int32 y, int32 modifiers){}
    virtual void LeftButtonUp(int32 x, int32 y, int32 modifiers){}
    virtual void LeftButtonDoubleClick(int32 x, int32 y, int32 modifiers){}
    virtual void MiddleButtonDown(int32 x, int32 y, int32 modifiers){}
    virtual void MiddleButtonUp(int32 x, int32 y, int32 modifiers){}
    virtual void MiddleButtonDoubleClick(int32 x, int32 y, int32 modifiers){}
    virtual void RightButtonDown(int32 x, int32 y, int32 modifiers){}
    virtual void RightButtonUp(int32 x, int32 y, int32 modifiers){}
    virtual void RightButtonDoubleClick(int32 x, int32 y, int32 modifiers){}

    // Tell the view to render itself into a bitmap
    virtual void Draw(DIB* canvas, RECT* invalidRect){}

    // Allow the view to deal with key presses
    virtual	void KeyPressed(char byte){}

    // Allow the view to get timer messages
    virtual	void Pulse(){}

    // used for scrolling a view
    virtual	void TargetedByScrollView(ScrollView *scroller){}
    virtual	void ScrollTo(int32 index){}

    // Is the point within the view's region?
    bool PointInView(int32 x, int32 y);

    // Moving function
    void MoveTo(int32 x, int32 y);
    void OffsetBy(int32 x, int32 y);

    // Housekeeping functions
    void Show(void);
    void Show(bool visible);
    void Hide(void);
    void Enable(void);
    void Enable(bool enable);
    void Disable(void);
    void Invalidate(void);
    void Invalidate(HRGN region);
    void Invalidate(RECT* rect);
    bool Visible(void) const{return m_visible;}
    bool Enabled(void) const{return m_enabled;}


    void Bounds(RECT* bounds) const{ CopyRect(bounds, &m_viewRect); }
    View* Parent() const { return m_parent; }
    HWND Window() const { return m_hwnd; }
    HRGN Region() const { return m_viewRegion; }
    void SetRegion(HRGN viewRegion);
    int32 Width() { return m_viewRect.right - m_viewRect.left;}
    int32 Height() { return m_viewRect.bottom - m_viewRect.top;}


 protected:
    
 private:
    View*       m_parent;
    HWND        m_hwnd;
    HRGN        m_viewRegion;
    RECT        m_viewRect;
    
    bool        m_visible;
    bool        m_enabled;
};

#endif /* _VIEW_H_ */