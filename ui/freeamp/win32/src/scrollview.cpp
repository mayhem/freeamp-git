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

/* system headers */
#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdlib.h>
#include <assert.h>

/* project headers */
#include "config.h"
#include "scrollview.h"
#include "renderer.h"

ScrollView::
ScrollView( HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* viewBitmap,
            int32 flags):
View(hwnd, parent, viewRegion)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_pressed = false;
    m_flags = flags;

    if(flags & ScrollType_Horizontal)
        m_orientation = ScrollType_Horizontal;
    else
        m_orientation = ScrollType_Vertical;

    m_min = 0;
    m_max = 0;
    m_position = 0;
    m_visible = 0;
    m_target = NULL;
    
    SetRect(&m_scrollRect, 0,0,0,0);
}

ScrollView::
ScrollView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* viewBitmap,
            int32 flags):
View(hwnd, parent, viewRect)
{
    assert(viewBitmap);

    m_viewBitmap = viewBitmap;
    m_pressed = false;
    m_flags = flags;

    if(flags & ScrollType_Horizontal)
        m_orientation = ScrollType_Horizontal;
    else
        m_orientation = ScrollType_Vertical;

    m_min = 0;
    m_max = 0;
    m_position = 0;
    m_visible = 0;
    m_target = NULL;

    SetRect(&m_scrollRect, 0,0,0,0);
}

ScrollView::
~ScrollView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;
}

void 
ScrollView::
MouseEntered()
{
   
}

void 
ScrollView::
MouseLeft()
{
    
}

#define SCROLL_SLOP 5

void 
ScrollView::
MouseMove(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed)
    {
        int32 dy;
        RECT viewRect;
        Bounds(&viewRect);

        if( x <= m_scrollRect.right + SCROLL_SLOP &&
            x >= m_scrollRect.left - SCROLL_SLOP &&
            y <= viewRect.bottom + SCROLL_SLOP &&
            y >= viewRect.top - SCROLL_SLOP )
        {
            dy = y - m_lastPoint.y;
        }
        else
        {
            dy = m_pressedPoint.y - m_lastPoint.y;
        }

        RECT rect = m_scrollRect;
       
        OffsetRect(&rect, 0, dy);

        if(rect.top < viewRect.top)
        {
            dy += viewRect.top - rect.top;
            rect = m_scrollRect;
            OffsetRect(&rect, 0, dy);
        }
        else if(rect.bottom > viewRect.bottom)
        {
            dy -= rect.bottom - viewRect.bottom;
            rect = m_scrollRect;
            OffsetRect(&rect, 0, dy);
        }
    
        if(dy)
        {
            m_scrollRect = rect;

            m_lastPoint.y += dy;

            int32 total = m_max - m_min;

            dy = m_scrollRect.top - viewRect.top;
            float proportion = ((float)(m_scrollRect.top - viewRect.top)/
                                (float)(viewRect.bottom - viewRect.top));


            int32 index = (int32)((float)total * proportion);

            m_target->ScrollTo(index);

            //m_position = index;

            Invalidate();
        }
    }
}

void 
ScrollView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressedPoint.x = x;
    m_pressedPoint.y = y;

    m_lastPoint = m_pressedPoint;


    if(PtInRect(&m_scrollRect, m_pressedPoint))
    {
        m_pressed = true;
    }
    else
    {
        if(y < m_scrollRect.top)
        {
            int32 index = Position() - Proportion();

            if(index < m_min)
                index = m_min;

            m_target->ScrollTo(index);

            SetPosition(index);
        }
        else if(y > m_scrollRect.bottom)
        {
            int32 index = Position() + Proportion();

            if(index > m_max - Proportion())
                index = m_max - Proportion();

            m_target->ScrollTo(index);

            SetPosition(index);
        }
    }
}

void 
ScrollView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    m_pressed = false;
}

View* 
ScrollView::
Target() const
{
    return m_target;
}

void 
ScrollView::
SetTarget(View* target)
{
    if(m_target)
        m_target->TargetedByScrollView(NULL);

    m_target = target;

    if(m_target)
        m_target->TargetedByScrollView(this);
}

int32 
ScrollView::
Position() const
{
    return m_position;
}

void 
ScrollView::
SetPosition(int32 position)
{
    /*if(position < m_min)
        position = m_min;
    else if(position > m_max)
        position = m_max;*/

    if( m_position != position &&
        position >= m_min &&
        position <= m_max)
    {
        m_position = position;

        int32 length;
        RECT viewRect;
        int32 total = m_max - m_min;
        float proportion = 0;

        Bounds(&viewRect);
        
        if(Orientation() == ScrollType_Horizontal)
        {
            length = m_scrollRect.right - m_scrollRect.left;
        }
        else
        {
            length = m_scrollRect.bottom - m_scrollRect.top;

            int32 dest_y = viewRect.top;

            if(total)
            {
                proportion = ((float)m_position/(float)total);
            
                dest_y += (int32)((float)(viewRect.bottom - viewRect.top) * proportion);
            }

            SetRect(&m_scrollRect, viewRect.left, dest_y, viewRect.right, dest_y + length);
        }

        Invalidate();
    }
}

void 
ScrollView::
Range(int32* min, int32* max)
{
    *min = m_min;
    *max = m_max;
}

void 
ScrollView::
SetRange(int32 min, int32 max)
{
    if(m_min != min || m_max != max)
    {
        m_min = min;
        m_max = max;

        int32 length;
        RECT viewRect;

        Bounds(&viewRect);

        // calculate length of scrollbar
        int32 total = m_max - m_min;
        float proportion = 0;

        if(total)
        {
            proportion = ((float)m_visible/(float)total);
        }

        if(Orientation() == ScrollType_Horizontal)
        {
            length = (int32)((float)(viewRect.right - viewRect.left) * proportion);

            if(length == 0)
                length = viewRect.right - viewRect.left;
            else if(length < m_viewBitmap->Width())
                length = m_viewBitmap->Width();

        }
        else
        {
            length = (int32)((float)(viewRect.bottom - viewRect.top) * proportion);

            if(length == 0)
                length = viewRect.bottom - viewRect.top;
            else if(length < m_viewBitmap->Height())
                length = m_viewBitmap->Height();

            int32 dest_y;

            dest_y = viewRect.top;

            if(total)
            {
                proportion = ((float)m_position/(float)total);
            
                dest_y += (int32)((float)(viewRect.bottom - viewRect.top) * proportion);
            }

            SetRect(&m_scrollRect, viewRect.left, dest_y, viewRect.right, dest_y + length);
        }

        Invalidate();
    }
}


int32 
ScrollView::
Proportion() const
{
    return m_visible;
}

void 
ScrollView::
SetProportion(int32 visible)
{
    if(m_visible != visible)
    {
        m_visible = visible;

        int32 length;
        RECT viewRect;

        Bounds(&viewRect);

        // calculate length of scrollbar
        int32 total = m_max - m_min;
        float proportion = 0;

        if(total)
        {
            proportion = ((float)m_visible/(float)total);
        }

        if(Orientation() == ScrollType_Horizontal)
        {
            length = (int32)((float)(viewRect.right - viewRect.left) * proportion);

            if(length == 0)
                length = viewRect.right - viewRect.left;
            else if(length < m_viewBitmap->Width())
                length = m_viewBitmap->Width();

        }
        else
        {
            length = (int32)((float)(viewRect.bottom - viewRect.top) * proportion);

            if(length == 0)
                length = viewRect.bottom - viewRect.top;
            else if(length < m_viewBitmap->Height())
                length = m_viewBitmap->Height();

            int32 dest_y;

            dest_y = viewRect.top;

            if(total)
            {
                proportion = ((float)m_position/(float)total);
            
                dest_y += (int32)((float)(viewRect.bottom - viewRect.top) * proportion);
            }

            SetRect(&m_scrollRect, viewRect.left, dest_y, viewRect.right, dest_y + length);
        }

        Invalidate();
    }
}

int32 
ScrollView::
Orientation() const
{
    return m_orientation;
}


void 
ScrollView::
Draw(DIB* canvas, RECT* invalidRect)
{
    RECT tempRect, viewRect, parentRect, drawRect;

    assert(canvas);

    // what is our rect?
    Bounds(&viewRect);

    if(invalidRect && !IsRectEmpty(&drawRect))
    {
        // what is the smallest portion of us inside the dirty rect
        IntersectRect(&tempRect, invalidRect, &viewRect);
    }
    else
    {
        CopyRect(&tempRect, &viewRect);
    }

    if(Parent())
    {
        Parent()->Bounds(&parentRect);

        // clip ourselves to our parent view
        IntersectRect(&drawRect, &tempRect, &parentRect);
    }
    else
    {
        CopyRect(&drawRect, &tempRect);
    }

    // calculate length of scrollbar
    /*int32 total = m_max - m_min;
    float proportion = 0;

    if(total)
    {
        proportion = ((float)m_visible/(float)total);
    }

    */

    if(Orientation() == ScrollType_Horizontal)
    {
        /*length = (int32)((float)(viewRect.right - viewRect.left) * proportion);

        if(length == 0)
            length = viewRect.right - viewRect.left;
        else if(length < m_viewBitmap->Width())
            length = m_viewBitmap->Width();*/

    }
    else
    {
        /*length = (int32)((float)(viewRect.bottom - viewRect.top) * proportion);

        if(length == 0)
            length = viewRect.bottom - viewRect.top;
        else if(length < m_viewBitmap->Height())
            length = m_viewBitmap->Height();

        int32 dest_x, dest_y;

        dest_x = viewRect.left;
        dest_y = viewRect.top;

        if(total)
        {
            proportion = ((float)m_position/(float)total);
            
            dest_y += (int32)((float)(viewRect.bottom - viewRect.top) * proportion);
        }*/

        int32 dest_x, dest_y;
        int32 length;

        dest_x = m_scrollRect.left;
        dest_y = m_scrollRect.top;
        length = m_scrollRect.bottom - m_scrollRect.top;

        Renderer::Copy( canvas,
                        dest_x, 
                        dest_y,     
                        m_viewBitmap->Width(),      
                        m_viewBitmap->Height(), 
                        m_viewBitmap,    
                        0,
                        0);

        length -= m_viewBitmap->Height();

        dest_y += m_viewBitmap->Height() - 2;

        for(int32 i = 0; i < length; i++)
        {
            Renderer::Copy( canvas,
                            dest_x, 
                            dest_y++,     
                            m_viewBitmap->Width(),      
                            3, 
                            m_viewBitmap,    
                            0,
                            m_viewBitmap->Height() - 3);
        }

    }
}
