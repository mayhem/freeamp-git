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
#include "volumeview.h"
#include "renderer.h"

VolumeView::
VolumeView( HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* backgroundBitmap,
            DIB* volFontBitmap,
            int32 volFontHeight,
            int32* volFontWidths,
            DIB* labelFontBitmap,
            int32 labelFontHeight,
            int32* labelFontWidths,
            int32 command):
View(hwnd, parent, viewRegion)
{
    assert(backgroundBitmap);

    m_backgroundBitmap = backgroundBitmap;
    m_offset = 0;
    m_pressed = false;
    m_command = command;

    m_volLength = 0;

    m_volFontBitmap = volFontBitmap;
    m_volFontWidths = volFontWidths;
    m_volFontHeight = volFontHeight;

    m_labelFontBitmap = labelFontBitmap;
    m_labelFontWidths = labelFontWidths;
    m_labelFontHeight = labelFontHeight;

    m_viewBitmap = new DIB;
    m_viewBitmap->Create(   Width(),
                            Height(),
                            backgroundBitmap->BitsPerPixel());

    m_volume = 0;

    strcpy(m_label, "volume");

    m_volLength = 0;

    for(int32 i = 0; m_label[i]; i++)
    {
        m_volLength += m_labelFontWidths[m_label[i] - 32];
    }
}

VolumeView::
VolumeView( HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* backgroundBitmap,
            DIB* volFontBitmap,
            int32 volFontHeight,
            int32* volFontWidths,
            DIB* labelFontBitmap,
            int32 labelFontHeight,
            int32* labelFontWidths,
            int32 command):
View(hwnd, parent, viewRect)
{
    assert(backgroundBitmap);

    m_backgroundBitmap = backgroundBitmap;
    m_offset = 0;
    m_pressed = false;
    m_command = command;

    m_volLength = 0;

    m_volFontBitmap = volFontBitmap;
    m_volFontWidths = volFontWidths;
    m_volFontHeight = volFontHeight;

    m_labelFontBitmap = labelFontBitmap;
    m_labelFontWidths = labelFontWidths;
    m_labelFontHeight = labelFontHeight;

    m_viewBitmap = new DIB;
    m_viewBitmap->Create(   Width(),
                            Height(),
                            backgroundBitmap->BitsPerPixel());

    m_volume = 0;

    strcpy(m_label, "volume");

    m_volLength = 0;

    for(int32 i = 0; m_label[i]; i++)
    {
        m_volLength += m_labelFontWidths[m_label[i] - 32];
    }
}

VolumeView::
~VolumeView()
{
    delete m_viewBitmap;
    m_viewBitmap = NULL;
}


void 
VolumeView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;  
}

void 
VolumeView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed && PtInRegion(Region(), x, y))
    {
        Invoke();
    }

    m_pressed = false;
}

void
VolumeView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}


void 
VolumeView::
SetVolume(int32 volume)
{
    if(volume != m_volume)
    {
        m_volume = volume;

        Invalidate();
    }
}

void 
VolumeView::
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
    if(m_backgroundBitmap->Width() < Width() ||
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
    }
   
    int32 i = 0;
    int32 offset = 1;
    char volume[5];

    wsprintf(volume, "%d%%", m_volume);

    // copy the volume
    for(i = 0; volume[i]; i++)
    {
        Renderer::Copy( m_viewBitmap,
                        offset, 
                        2,     
                        m_volFontWidths[volume[i] - 32],      
                        m_volFontHeight, 
                        m_volFontBitmap,    
                        0,
                        (volume[i] - 32)*m_volFontHeight);
       
        offset += m_volFontWidths[volume[i] - 32];
    }

    offset = Width() - m_volLength - 1;

    // copy the label
    for(i = 0; m_label[i]; i++)
    {

        Renderer::Copy( m_viewBitmap,
                        offset, 
                        3,     
                        m_labelFontWidths[m_label[i] - 32],      
                        m_labelFontHeight, 
                        m_labelFontBitmap,    
                        0,
                        (m_label[i] - 32)*m_labelFontHeight);
       
        offset += m_labelFontWidths[m_label[i] - 32];
    }

    // make sure something weird did not happen above...
    if(!IsRectEmpty(&drawRect))
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
    }
}
