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
#include "timeview.h"
#include "renderer.h"

TimeView::
TimeView(   HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* timeFontBitmap,
            int32 timeFontHeight,
            int32* timeFontWidths,
            DIB* labelFontBitmap,
            int32 labelFontHeight,
            int32* labelFontWidths,
            int32 command,
            int32 flags):
View(hwnd, parent, viewRegion)
{
    //assert(backgroundBitmap);

    //m_backgroundBitmap = backgroundBitmap;
    m_offset = 0;
    m_pressed = false;
    m_command = command;
    m_flags = flags;

    m_timeLength = 0;

    m_timeFontBitmap = timeFontBitmap;
    m_timeFontWidths = timeFontWidths;
    m_timeFontHeight = timeFontHeight;

    m_labelFontBitmap = labelFontBitmap;
    m_labelFontWidths = labelFontWidths;
    m_labelFontHeight = labelFontHeight;


    /*m_viewBitmap = new DIB;
    m_viewBitmap->Create(   Width(),
                            Height(),
                            backgroundBitmap->BitsPerPixel());*/

    m_label = NULL;

    SetCurrentTime(0,0,0);
    SetTotalTime(0,0,0);
    SetSeekTime(0,0,0);

    SetDisplay(DisplayCurrentTime);
}

TimeView::
TimeView(   HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* timeFontBitmap,
            int32 timeFontHeight,
            int32* timeFontWidths,
            DIB* labelFontBitmap,
            int32 labelFontHeight,
            int32* labelFontWidths,
            int32 command,
            int32 flags):
View(hwnd, parent, viewRect)
{
    //assert(backgroundBitmap);

    //m_backgroundBitmap = backgroundBitmap;
    m_offset = 0;
    m_pressed = false;
    m_command = command;
    m_flags = flags;

    m_timeLength = 0;

    m_timeFontBitmap = timeFontBitmap;
    m_timeFontWidths = timeFontWidths;
    m_timeFontHeight = timeFontHeight;

    m_labelFontBitmap = labelFontBitmap;
    m_labelFontWidths = labelFontWidths;
    m_labelFontHeight = labelFontHeight;

    /*m_viewBitmap = new DIB;
    m_viewBitmap->Create(   Width(),
                            Height(),
                            backgroundBitmap->BitsPerPixel());*/

	m_label = NULL;

    SetCurrentTime(0,0,0);
    SetTotalTime(0,0,0);

    SetDisplay(DisplayCurrentTime);
}

TimeView::
~TimeView()
{
    //delete m_viewBitmap;
    //m_viewBitmap = NULL;

    if(m_label)
        delete [] m_label;
}


void 
TimeView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;  
}

void 
TimeView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed && PtInRegion(Region(), x, y))
    {
        Invoke();
    }

    m_pressed = false;
}

void
TimeView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}

void 
TimeView::
SetLabel(char* label)
{
    assert(label);

    if(m_label)
    {
        delete [] m_label;
    }

    m_label = new char[strlen(label) + 1];
    strcpy(m_label, label);

    Invalidate();
}

void 
TimeView::
SetCurrentTime( int32 hours, 
                int32 minutes, 
                int32 seconds)
{
    if( m_currentHours != hours ||
        m_currentMinutes != minutes ||
        m_currentSeconds != seconds)
    {
        m_currentHours = hours;
        m_currentMinutes = minutes;
        m_currentSeconds = seconds;

        CreateTimeString();

        Invalidate();
    }
}

void 
TimeView::
SetTotalTime(   int32 hours, 
                int32 minutes, 
                int32 seconds)
{
    if( m_totalHours != hours ||
        m_totalMinutes != minutes ||
        m_totalSeconds != seconds)
    {
        m_totalHours = hours;
        m_totalMinutes = minutes;
        m_totalSeconds = seconds;

        CreateTimeString();

        Invalidate();
    }
}

void 
TimeView::
SetSeekTime(int32 hours, 
			int32 minutes, 
			int32 seconds)
{
    if( m_seekHours != hours ||
        m_seekMinutes != minutes ||
        m_seekSeconds != seconds)
    {
        m_seekHours = hours;
        m_seekMinutes = minutes;
        m_seekSeconds = seconds;

        CreateTimeString();

        Invalidate();
    }

}

void 
TimeView::
CurrentTime(int32* hours, 
            int32* minutes, 
            int32* seconds)
{
    *hours = m_currentHours;
    *minutes = m_currentMinutes;
    *seconds = m_currentSeconds;
}

void 
TimeView::
TotalTime(  int32* hours, 
            int32* minutes, 
            int32* seconds)
{
    *hours = m_totalHours;
    *minutes = m_totalMinutes;
    *seconds = m_totalSeconds;
}
void 
TimeView::
SetDisplay(TimeDisplay display) 
{ 
	if(display != m_display)
	{
		m_display = display; 

        switch(m_display)
        {
            case DisplayCurrentTime:
                SetLabel("current");
                break;

            case DisplayTotalTime:
                SetLabel("total");
                break;

            case DisplayRemainingTime:
                SetLabel("remaining");
                break;

            case DisplaySeekTime:
                SetLabel("seek");
                break;
        
        }

        CreateTimeString();
		Invalidate();
	}
}

void
TimeView::
ToggleDisplay()
{
    int32 foo = (int32)m_display;

    m_display = (TimeDisplay)++foo;

	if(m_display == DisplayLastToggleTime)
		m_display = DisplayFirstToggleTime;

    switch(m_display)
    {
        case DisplayCurrentTime:
            SetLabel("current");
            break;

        case DisplayTotalTime:
            SetLabel("total");
            break;

        case DisplayRemainingTime:
            SetLabel("remaining");
            break;
    
    }

    CreateTimeString();
	Invalidate();
}

void 
TimeView::
CreateTimeString()
{
    int32 hours = 0;
    int32 minutes = 0;
    int32 seconds = 0;

    switch(m_display)
    {
        case DisplayCurrentTime:
            hours = m_currentHours;
            minutes = m_currentMinutes;
            seconds = m_currentSeconds;
            break;

        case DisplayTotalTime:
            hours = m_totalHours;
            minutes = m_totalMinutes;
            seconds = m_totalSeconds;
            break;

        case DisplayRemainingTime:
        {
            int32 totalTimeInSeconds = m_totalHours * 3600;
			totalTimeInSeconds += m_totalMinutes * 60;
			totalTimeInSeconds += m_totalSeconds;

			int32 currentTimeInSeconds = m_currentHours * 3600;
			currentTimeInSeconds += m_currentMinutes * 60;
			currentTimeInSeconds += m_currentSeconds;

			int32 remainingTimeInSeconds = totalTimeInSeconds - 
											currentTimeInSeconds;

			hours = remainingTimeInSeconds / 3600;
			minutes = (remainingTimeInSeconds - 
						(hours * 3600)) / 60;
			seconds = remainingTimeInSeconds - 
						(hours * 3600) - 
						(minutes * 60);
            break;
        }
    
        case DisplaySeekTime:
            hours = m_seekHours;
            minutes = m_seekMinutes;
            seconds = m_seekSeconds;
            break;
    }

    if(hours)
    {
        wsprintf(m_time, "%d:%.02d:%.02d",  hours,
                                            minutes,
                                            seconds);
    }
    else
    {
         wsprintf(m_time, "%.02d:%.02d",    minutes,
                                            seconds);
    }

    // calculate how long the string is in pixels

    m_timeLength = 0;

    for(int32 i = 0; m_time[i]; i++)
    {
        m_timeLength += m_timeFontWidths[m_time[i] - 32];
    }
}

void 
TimeView::
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

    /*char buffer[256];
    wsprintf(buffer, 
             "drawRect.left: %d\r\n"
             "drawRect.top: %d\r\n"
             "drawRect.right: %d\r\n"
             "drawRect.bottom: %d\r\n",
             drawRect.left,drawRect.top,drawRect.right,drawRect.bottom);

    OutputDebugString(buffer);*/


    // prepare the view bitmap
    /*if(m_backgroundBitmap->Width() < Width() ||
        m_backgroundBitmap->Height() < Height() )
    {
        Renderer::Tile( m_viewBitmap,
                        0, 
                        0,     
                        m_viewBitmap->Width(),      
                        m_viewBitmap->Height(), 
                        m_backgroundBitmap);
    }
    else
    {
        Renderer::Copy( m_viewBitmap,
                        0, 
                        0,     
                        m_viewBitmap->Width(),      
                        m_viewBitmap->Height(), 
                        m_backgroundBitmap,    
                        0,
                        0);
    }*/

    int32 i = 0;
    int32 offset = viewRect.left + 1;

    // copy the label
    for(i = 0; m_label[i]; i++)
    {

        Renderer::Copy( canvas,
                        offset, 
                        viewRect.top + 3,     
                        m_labelFontWidths[m_label[i] - 32],      
                        m_labelFontHeight, 
                        m_labelFontBitmap,    
                        0,
                        (m_label[i] - 32)*m_labelFontHeight);
       
        offset += m_labelFontWidths[m_label[i] - 32];
    }

    offset = viewRect.left + Width() - m_timeLength - 1;

    for(i = 0; m_time[i]; i++)
    {
        Renderer::Copy( canvas,
                        offset, 
                        viewRect.top + 2,     
                        m_timeFontWidths[m_time[i] - 32],      
                        m_timeFontHeight, 
                        m_timeFontBitmap,    
                        0,
                        (m_time[i] - 32)*m_timeFontHeight);
       
        offset += m_timeFontWidths[m_time[i] - 32];
    }

    // make sure something weird did not happen above...
    /*if(!IsRectEmpty(&drawRect))
    {
        // render the draw rect to the canvas

        // first we need to translate the coordinates

        int32 src_x, src_y; // starting point in source
        int32 dest_x, dest_y; // starting point in destination
        int32 width, height; // lengths in source

        src_x = abs(drawRect.left - viewRect.left);
        src_y = abs(drawRect.top - viewRect.top);
        dest_x = drawRect.left;
        dest_y = drawRect.top;
        width = drawRect.right - drawRect.left;
        height = drawRect.bottom - drawRect.top;

        // second we need to blit the bits
        Renderer::Copy( canvas,
                        dest_x, 
                        dest_y,     
                        width,      
                        height, 
                        m_viewBitmap,    
                        src_x,
                        src_y + m_offset);
    }*/
}
