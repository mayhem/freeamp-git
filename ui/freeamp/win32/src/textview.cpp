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
#include "textview.h"
#include "renderer.h"

TextView::
TextView(   HWND hwnd,
            View* parent,
            HRGN viewRegion,
            DIB* fontBitmap,
            int32 fontHeight,
            int32* fontWidths,
            int32 command,
            int32 flags):
View(hwnd, parent, viewRegion)
{
    assert(fontBitmap);
    assert(viewRegion);

    m_fontBitmap = fontBitmap;
    m_textBitmap = NULL;
    m_offset = 0;
    m_command = command;
    m_flags = flags;
    m_fontWidths = fontWidths;
    m_fontHeight = fontHeight;
    m_needToScroll = false;
    m_wiggle = false;
    m_viewDying = false;
    m_pressed   = false;

    InitializeCriticalSection(&m_criticalSection);

    // create the text bitmap
    m_textBitmap = new DIB;
    m_textBitmap->Create(   Width(), 
                            m_fontHeight, 
                            m_fontBitmap->BitsPerPixel());

    m_textLength = Width();
    m_bitmapLength = m_textLength;

    m_text = NULL;

    SetText("");

    m_thread = NULL;
    m_semaphore = new Semaphore(1);

    if(flags & TextType_Wiggle)
    {
        m_thread = Thread::CreateThread();
        m_thread->Create(thread_function, this);
        m_wiggle = true;
    }
}

TextView::
TextView(   HWND hwnd,
            View* parent,
            RECT* viewRect,
            DIB* fontBitmap,
            int32 fontHeight,
            int32* fontWidths,
            int32 command,
            int32 flags):
View(hwnd, parent, viewRect)
{
    assert(fontBitmap);
    assert(viewRect);

    m_fontBitmap = fontBitmap;
    m_textBitmap = NULL;
    m_offset = 0;
    m_command = command;
    m_flags = flags;
    m_fontWidths = fontWidths;
    m_fontHeight = fontHeight;
    m_needToScroll = false;
    m_wiggle = false;
    m_viewDying = false;
    m_pressed   = false;

    InitializeCriticalSection(&m_criticalSection);

    // create the text bitmap
    m_textBitmap = new DIB;
    m_textBitmap->Create(   Width(), 
                            m_fontHeight, 
                            m_fontBitmap->BitsPerPixel());

    m_textLength = Width();

    m_text = NULL;

    SetText("");

    m_thread = NULL;
    m_semaphore = new Semaphore(1);


    if(flags & TextType_Wiggle)
    {
        m_thread = Thread::CreateThread();
        m_thread->Create(thread_function, this);
        m_wiggle = true;
    }
}

TextView::
~TextView()
{
    m_viewDying = true;
    m_semaphore->Wait();

    if(m_semaphore)
        delete m_semaphore;

    if(m_textBitmap)
        delete m_textBitmap;

    if(m_text)
        delete [] m_text;

    
}

void 
TextView::
MouseEntered()
{
    if(m_needToScroll)
    {
        if(m_flags & TextType_MouseWiggle)
        {
            EnterCriticalSection(&m_criticalSection);

            if(!m_wiggle && !m_thread)
            {
                m_thread = Thread::CreateThread();
                m_thread->Create(thread_function, this);
                m_wiggle = true;
            }
            else if(!m_wiggle)
            {
                m_wiggle = true;
            }

            LeaveCriticalSection(&m_criticalSection);
        }
    }
}

void 
TextView::
MouseLeft()
{
    if(m_needToScroll)
    {
        if(m_flags & TextType_MouseWiggle)
        {
            EnterCriticalSection(&m_criticalSection);
            m_wiggle = false;
            LeaveCriticalSection(&m_criticalSection);
        }
    }
}

void 
TextView::
MouseMove(int32 x, int32 y, int32 modifiers)
{

}

void 
TextView::
LeftButtonDown(int32 x, int32 y, int32 modifiers)
{
    m_pressed = true;  
}

void 
TextView::
LeftButtonUp(int32 x, int32 y, int32 modifiers)
{
    if(m_pressed && PtInRegion(Region(), x, y))
    {
        Invoke();
    }

    m_pressed = false;
}

void
TextView::
Invoke()
{
    PostMessage(Window(), WM_COMMAND, (WPARAM)m_command, (LPARAM) this);
}

void 
TextView::
SetText(char* text)
{
    assert(text);

    if(m_text)
    {
        delete [] m_text;
    }

    m_text = new char[strlen(text) + 1];
    strcpy(m_text, text);

    int32 textLength = 0;
    int32 offset = 0;
    int32 i = 0;

    // calculate how long the bitmap needs to be
    // that will hold the pre-rendered string
    for(i = 0; m_text[i]; i++)
    {
        textLength += m_fontWidths[(BYTE)(m_text[i] - 32)];
        /*if(m_text[i] < 127 && m_text[i] > 31)
            textLength += m_fontWidths[m_text[i] - 32];
        else
            textLength += m_fontWidths[63 - 32];*/
    }

    m_needToScroll = (textLength > Width());

    EnterCriticalSection(&m_criticalSection);

    if(!(m_flags & TextType_Wiggle))
    {
        m_wiggle = false;
    }

    m_offset = 0;

    m_textLength = textLength;

    if(textLength > m_bitmapLength)
    {
        m_bitmapLength = textLength;

        if(m_textBitmap)
        {
            delete m_textBitmap;
        }

        // create new bitmap
        m_textBitmap = new DIB;
        m_textBitmap->Create(   m_bitmapLength, 
                                m_fontHeight, 
                                m_fontBitmap->BitsPerPixel());
    }


    // render the string
    for(i = 0; m_text[i]; i++)
    {
        int32 y;
        int32 width;

        y = ((BYTE)(m_text[i] - 32))*m_fontHeight;
        width = m_fontWidths[(BYTE)(m_text[i] - 32)];

        /*if(m_text[i] < 127 && m_text[i] > 31)
        {
            y = (m_text[i] - 32)*m_fontHeight;
            width = m_fontWidths[m_text[i] - 32];
        }
        else
        {
            y = (63 - 32)*m_fontHeight;
            width = m_fontWidths[63 - 32];
        }*/

        Renderer::Copy( m_textBitmap,
                        offset, 
                        0,     
                        width,   
                        m_fontHeight,
                        m_fontBitmap,    
                        0,
                        y);

        offset += width;
    }

    // see if we need to pad bitmap out to view length
    // this is a hack and we should use a masked blit so
    // we don't have to do this...
    if(textLength < Width())
    {
        int32 delta = Width() - textLength;
        int32 numSpaces = delta / m_fontWidths[0x20 - 32]; // 0x20 is hex for a space
        int32 remainder = delta % m_fontWidths[0x20 - 32]; // 0x20 is hex for a space

        // take care of the spaces
        for(i = 0; i < numSpaces; i++)
        {
            Renderer::Copy( m_textBitmap,
                            offset, 
                            0,     
                            m_fontWidths[0x20 - 32],   
                            m_fontHeight,
                            m_fontBitmap,    
                            0,
                            (0x20 - 32)*m_fontHeight);

            offset += m_fontWidths[0x20 - 32];
        }

        // take care of the remainder
        Renderer::Copy( m_textBitmap,
                        offset, 
                        0,     
                        remainder,   
                        m_fontHeight,
                        m_fontBitmap,    
                        0,
                        (0x20 - 32)*m_fontHeight);

    }

    LeaveCriticalSection(&m_criticalSection);

    Invalidate();
}

void 
TextView::
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

        EnterCriticalSection(&m_criticalSection);

        // second we need to blit the bits
        Renderer::Copy( canvas,
                        dest_x, 
                        dest_y,     
                        width,      
                        height, 
                        m_textBitmap,    
                        src_x + m_offset,
                        src_y);

        LeaveCriticalSection(&m_criticalSection);

       
    }
}

void
TextView::
ThreadFunction()
{
    static int32 delta = 1;
    bool loop = true;

    m_semaphore->Wait();

    do
    {
        EnterCriticalSection(&m_criticalSection);

        m_offset += delta;

        if( m_offset >= (m_textLength - Width()) || // reached the right edge of the bitmap
            m_offset <= 0)                          // reached the left edge of the bitmap
        {   
            if( m_offset > (m_textLength - Width()) )
                m_offset = m_textLength;
            else if( m_offset < 0 )
                m_offset = 0;

            delta *= -1; // reverse direction
        }


        /*char buffer[256];
        wsprintf(buffer, 
                 "m_offset: %d\r\n"
                 "m_textLength: %d\r\n"
                 "drawRect.right: %d\r\n"
                 "drawRect.bottom: %d\r\n",
                 drawRect.left,drawRect.top,drawRect.right,drawRect.bottom);

        OutputDebugString(buffer);*/

        Invalidate();

        LeaveCriticalSection(&m_criticalSection);

        Sleep(200);

        EnterCriticalSection(&m_criticalSection);

        if( !m_wiggle && !((m_flags & TextType_MouseWiggle) && m_offset))
        {
            delete m_thread;
            m_thread = NULL;
            loop = false;
        }

        LeaveCriticalSection(&m_criticalSection);
    }while(loop && !m_viewDying);

    m_semaphore->Signal();
}

void 
TextView::
thread_function(void* arg)
{
    TextView* view = (TextView*)arg;

    view->ThreadFunction();
}

