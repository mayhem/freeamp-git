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
#include <stdlib.h>
#include <assert.h>

/* project headers */
#include "config.h"
#include "stringitem.h"
#include "renderer.h"

StringItem::
StringItem( char* text,
            DIB* fontBitmap,
            int32 fontHeight,
            int32* fontWidths):
ListItem()
{
    assert(text);
    assert(fontBitmap);

    m_fontBitmap = fontBitmap;
    m_fontWidths = fontWidths;
    m_fontHeight = fontHeight;

    m_text = new char[strlen(text) + 1];
    strcpy(m_text, text);

    int32 textLength = 0;
    int32 offset = 0;
    int32 i = 0;

    // calculate how long the bitmap needs to be
    // that will hold the pre-rendered string
    for(i = 0; m_text[i]; i++)
    {
        if(m_text[i] < 127 && m_text[i] > 31)
            textLength += m_fontWidths[m_text[i] - 32];
        else
            textLength += m_fontWidths[63 - 32];

    }

    // create text bitmap
    m_textBitmap = new DIB;
    m_textBitmap->Create(   textLength, 
                            m_fontHeight, 
                            m_fontBitmap->BitsPerPixel());

    // render the string
    for(i = 0; m_text[i]; i++)
    {
        int32 y;
        int32 width;

        if(m_text[i] < 127 && m_text[i] > 31)
        {
            y = (m_text[i] - 32)*m_fontHeight;
            width = m_fontWidths[m_text[i] - 32];
        }
        else
        {
            y = (63 - 32)*m_fontHeight;
            width = m_fontWidths[63 - 32];
        }

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

    SetHeight(fontHeight);
    SetWidth(textLength);
}

StringItem::
~StringItem()
{
    if(m_textBitmap)
        delete m_textBitmap;

    if(m_text)
        delete [] m_text;
}

void
StringItem::
DrawItem(DIB* canvas, RECT* bounds)
{
    int32 width = bounds->right - bounds->left;
    int32 height = bounds->bottom - bounds->top;

    if(width > Width())
        width = Width();

    Renderer::Copy( canvas,
                    bounds->left, 
                    bounds->top,     
                    width,      
                    height, 
                    m_textBitmap,    
                    0,
                    0);

    if(IsSelected())
    {
       Renderer::Fill(  canvas,
                        bounds->left, 
                        bounds->top,     
                        bounds->right - bounds->left,      
                        bounds->bottom - bounds->top, 
                        121,    
                        131,
                        153,
                        100);
    }
}
