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
#include <stdio.h>
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
    m_min = 0;
    m_max = 0;
    m_smallStep = 1;
    m_largeStep = 10;
    m_position = 0;

    m_target = NULL;

    RECT viewRect;
    Bounds(&viewRect);
    
    CopyRect(&m_thumbRect, &viewRect);
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
    m_min = 0;
    m_max = 0;
    m_smallStep = 1;
    m_largeStep = 10;
    m_position = 0;

    m_target = NULL;
    
    CopyRect(&m_thumbRect, viewRect);
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
        int32 delta;
        RECT viewRect;
        RECT rect;
        Bounds(&viewRect);

        rect = m_thumbRect;

        if(Orientation() == ScrollType_Vertical)
        {
            if( x <= m_thumbRect.right + SCROLL_SLOP &&
                x >= m_thumbRect.left - SCROLL_SLOP &&
                y <= viewRect.bottom + SCROLL_SLOP &&
                y >= viewRect.top - SCROLL_SLOP )
            {
                delta = y - m_lastPoint.y;
            }
            else
            {
                delta = m_pressedPoint.y - m_lastPoint.y;
            }

            OffsetRect(&rect, 0, delta);

            if(rect.top < viewRect.top)
            {
                delta += viewRect.top - rect.top;
                rect = m_thumbRect;
                OffsetRect(&rect, 0, delta);
            }
            else if(rect.bottom > viewRect.bottom)
            {
                delta -= rect.bottom - viewRect.bottom;
                rect = m_thumbRect;
                OffsetRect(&rect, 0, delta);
            }

            if(delta)
            {
                int32 total = m_max - m_min;

                m_lastPoint.y += delta;

                m_thumbRect = rect;

                float proportion = ((float)(rect.top - viewRect.top)/
                                    (float)(viewRect.bottom - viewRect.top));

                int32 index = (int32)((float)total * proportion);

                /*proportion = ((float)(rect.bottom - rect.top)/
                                    (float)(viewRect.bottom - viewRect.top));

                proportion /= ((float)(m_largeStep)/(float)(total));

                char buf[256];
                sprintf(buf, "%f\r\n", proportion);
                OutputDebugString(buf);

                index += proportion;*/

                m_position = index;

                m_target->ScrollTo(index);

                Invalidate();
            }
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


    if(PtInRect(&m_thumbRect, m_pressedPoint))
    {
        m_pressed = true;
    }
    else
    {
        if(y < m_thumbRect.top)
        {
            int32 index = Position() - m_largeStep;

            if(index < m_min)
                index = m_min;

            //m_target->ScrollTo(index);
            //SetPosition(index);

            SetPositionInternal(index, true);
        }
        else if(y > m_thumbRect.bottom)
        {
            int32 index = Position() + m_largeStep;

            if(index > m_max - (int32)m_largeStep)
                index = m_max - m_largeStep;

            //m_target->ScrollTo(index);
            //SetPosition(index);

            SetPositionInternal(index, true);
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
    SetPositionInternal(position, false);
}

void 
ScrollView::
SetPositionInternal(int32 position, bool notifyTarget)
{
    if(position < m_min)
        position = m_min;
    else if(position > m_max)
        position = m_max;

    if(m_position != position)
    {
        m_position = position;

        if(notifyTarget && m_target)
        {
            m_target->ScrollTo(position);
        }

        DetermineThumbRect();

        Invalidate();
    }
}

void 
ScrollView::
Range(int32* min, int32* max) const
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

        if(m_max < m_min)
        {
            m_max = min;
            m_min = max;
        }

        DetermineThumbRect();
       
        Invalidate();
    }
}

void 
ScrollView::
SetSteps(uint32 smallStep, uint32 largeStep)
{
    if(m_smallStep != smallStep || m_largeStep != largeStep)
    {
        m_smallStep = smallStep;
        m_largeStep = largeStep;

        DetermineThumbRect();
       
        Invalidate();
    }
}

void 
ScrollView::
Steps(uint32* smallStep, uint32* largeStep) const
{
    *smallStep = m_smallStep;
    *largeStep = m_largeStep;
}

int32 
ScrollView::
Orientation() const
{
    return m_orientation;
}


void 
ScrollView::
DetermineThumbRect()
{
    uint32 range = 0;
    float proportion = 0;
    RECT viewRect;

    Bounds(&viewRect);
    range = m_max - m_min;

    if(range <= m_largeStep)
    {
        CopyRect(&m_thumbRect, &viewRect);
    }
    else if(Orientation() == ScrollType_Vertical)
    {
        int32 thumb = 0;

        proportion = ((float)m_largeStep/(float)range);

        thumb = (int32)((float)(viewRect.bottom - viewRect.top) * proportion);

        if(thumb == 0)
            thumb = viewRect.bottom - viewRect.top;
        if(thumb < m_viewBitmap->Width())
            thumb = m_viewBitmap->Width();

        range = viewRect.bottom - viewRect.top - thumb;

        m_thumbRect.top = viewRect.top;

        float d = (float)(range * (m_position - m_min)) / (float)(m_max - m_min - m_largeStep);

        m_thumbRect.top += (int32)d;
		m_thumbRect.bottom = m_thumbRect.top + thumb;
    }
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
        int32 dest_x, dest_y;
        int32 length;

        dest_x = m_thumbRect.left;
        dest_y = m_thumbRect.top;
        length = m_thumbRect.bottom - m_thumbRect.top;

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
