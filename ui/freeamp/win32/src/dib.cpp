
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

#include <assert.h>

#include "dib.h"


DIB::
DIB()
{
    m_bits = NULL;
    m_bitmapInfo = NULL;
    m_height = 0;
    m_width = 0;
    m_bytesPerLine = 0;
    m_bitCount = 0;
}

DIB::
~DIB()
{
    if(m_bits)
    {
        free(m_bits);
        m_bits = NULL;
    }

    if(m_bitmapInfo)
    {
        free(m_bitmapInfo);
        m_bitmapInfo = NULL;
    }
}


bool 
DIB::
Create( uint32 width, 
	    uint32 height, 
	    uint32 bitsPerPixel)
{
	bool result = false;

    if(m_bits)
    {
        free(m_bits);
        m_bits = NULL;
    }

    if(m_bitmapInfo)
    {
        free(m_bitmapInfo);
        m_bitmapInfo = NULL;
    }

    m_width = width;
    m_height = height;
    m_bitCount = bitsPerPixel;

    int32 imageSize = ((((m_width * m_bitCount) + 31) & ~31) >> 3) * m_height;

    m_bytesPerLine = imageSize / m_height;

    /*LEAK*/m_bits = (BYTE*)malloc(imageSize);

    /*LEAK*/m_bitmapInfo = (BITMAPINFO*)malloc( sizeof BITMAPINFO + 
                                        sizeof RGBQUAD *
                                        NumberOfPaletteEntries());

    if(m_bits && m_bitmapInfo)
    {
        result = true;

        // fill in the important header information
	    m_bitmapInfo->bmiHeader.biSize = sizeof BITMAPINFOHEADER;
        m_bitmapInfo->bmiHeader.biWidth = m_width;
        m_bitmapInfo->bmiHeader.biHeight = m_height;
        m_bitmapInfo->bmiHeader.biPlanes = 1;
        m_bitmapInfo->bmiHeader.biBitCount = m_bitCount;
        m_bitmapInfo->bmiHeader.biCompression = BI_RGB;
        m_bitmapInfo->bmiHeader.biSizeImage = 0;    
        m_bitmapInfo->bmiHeader.biXPelsPerMeter = 0;
        m_bitmapInfo->bmiHeader.biYPelsPerMeter = 0;
        m_bitmapInfo->bmiHeader.biClrUsed = 0;
        m_bitmapInfo->bmiHeader.biClrImportant = 0;

        // do we need a palette?
        if(NumberOfPaletteEntries())
        {
            int32 multiple = 256/NumberOfPaletteEntries();

            // create a simple gray scale
            for(int32 i = 0; i < NumberOfPaletteEntries(); i++)
            {
                m_bitmapInfo->bmiColors[i].rgbBlue = i * multiple;
                m_bitmapInfo->bmiColors[i].rgbGreen = i * multiple;
                m_bitmapInfo->bmiColors[i].rgbRed = i * multiple;
                m_bitmapInfo->bmiColors[i].rgbReserved = 0;
            }
        }
    }
    else
    {
        if(m_bits)
        {
            free(m_bits);
            m_bits = NULL;
        }

        if(m_bitmapInfo)
        {
            free(m_bitmapInfo);
            m_bitmapInfo = NULL;
        }
    }
    

    
	return result;
}

bool 
DIB::
Load(HANDLE module, LPCTSTR resource) 
{
    bool result = false;
    HRSRC rsrcHandle = NULL;
	HGLOBAL globalHandle = NULL;
    BITMAPINFOHEADER* bitmapInfoHeader = NULL;

    if(m_bits)
    {
        free(m_bits);
        m_bits = NULL;
    }

    if(m_bitmapInfo)
    {
        free(m_bitmapInfo);
        m_bitmapInfo = NULL;
    }

    if(rsrcHandle = FindResource(module, resource, RT_BITMAP))
    {
        if(globalHandle = LoadResource(module, rsrcHandle))
        {
            bitmapInfoHeader = (BITMAPINFOHEADER*)LockResource(globalHandle);

            if(bitmapInfoHeader)
            {
                m_width = bitmapInfoHeader->biWidth;
                m_height = bitmapInfoHeader->biHeight;
                m_bitCount = bitmapInfoHeader->biBitCount;
                m_bitmapInfo = (BITMAPINFO*)bitmapInfoHeader; // just temporary...
                // so we can calculate this...
                int32 numEntries = NumberOfPaletteEntries();

                int32 imageSize = ((((m_width * m_bitCount) + 31) & ~31) >> 3) * m_height;

                m_bytesPerLine = imageSize / m_height;

				//LEAK
                m_bits = (BYTE*)malloc(imageSize);

				// LEAK
                m_bitmapInfo = (BITMAPINFO*)malloc( sizeof BITMAPINFO + 
                                                    sizeof RGBQUAD *
                                                    numEntries);

                if(m_bits && m_bitmapInfo)
                {
                    result = true;

                    memcpy( m_bitmapInfo, 
                            bitmapInfoHeader,   
                            sizeof BITMAPINFO + 
                            sizeof RGBQUAD *
                            numEntries);

                    BYTE* bits = ((BYTE*)bitmapInfoHeader + 
                                    bitmapInfoHeader->biSize + 
                                    numEntries * 
                                    sizeof RGBQUAD);

                    memcpy(m_bits, bits, m_bytesPerLine * m_height);
                }
                else
                {
                   if(m_bits)
                    {
                        free(m_bits);
                        m_bits = NULL;
                    }

                    if(m_bitmapInfo)
                    {
                        free(m_bitmapInfo);
                        m_bitmapInfo = NULL;
                    }
                }
                
                UnlockResource(globalHandle);
            }
        }
    }

    return result;
}


int32
DIB::
NumberOfPaletteEntries() const
{
    int32 result = 0;

    if(m_bitmapInfo && m_bitmapInfo->bmiHeader.biClrUsed)
        result =  m_bitmapInfo->bmiHeader.biClrUsed;
    else
    {
	    /* return number of colors based on bits per pixel */
	    switch (m_bitCount)
	    {
		    case 1:
			    result = 2;

		    case 4:
			    result = 16;

		    case 8:
			    result = 256;
	    }
    }

    return result;
}

BYTE* 
DIB::
Bits(int32 x, int32 y)
{
    BYTE* result = (BYTE*) m_bits;

    int32 temp = x*BytesPerPixel();

    result += m_bytesPerLine*(m_height - y - 1) + temp;

    return result;
}

void 
DIB::
Pixel(int32 x, int32 y, Color* color)
{
    BYTE* bits = Bits(x,y);

    if(BitsPerPixel() == 24)
    {
        color->b = bits[0];
        color->g = bits[1];
        color->r = bits[2];
        color->a = 0;
    }
    else
    {
        int32 index = *bits;
        
        color->b = BitmapInfo()->bmiColors[index].rgbBlue;
        color->g = BitmapInfo()->bmiColors[index].rgbGreen;
        color->r = BitmapInfo()->bmiColors[index].rgbRed;
        color->a = BitmapInfo()->bmiColors[index].rgbReserved;
    }
}

uint32 
DIB::
Pixel(int32 x, int32 y)
{
    Color color;

    Pixel(x, y, &color);

    return color.Pack();
}

RGBQUAD* 
DIB::
Palette()
{
    RGBQUAD* result = NULL;

    if(BitsPerPixel() == 8)
    {
        BITMAPINFO* info = BitmapInfo();

        result = info->bmiColors;
    }

    return result;
}

bool
DIB::
SetPalette(RGBQUAD* palette, int32 entries)
{
    bool result = false;

    if(BitsPerPixel() == 8)
    {
        memcpy(Palette(), palette, entries*sizeof(RGBQUAD));

        result = true;
    }

    return result;
}


bool 
DIB::
ColorForIndex(int32 index, Color* color)
{
    bool result = false;
    
    if( BitsPerPixel() == 8 &&
        index >= 0 && index < 256)
    {
        RGBQUAD* palette = Palette();

        color->r = palette[index].rgbRed;
        color->g = palette[index].rgbGreen;
        color->b = palette[index].rgbBlue;
        color->a = palette[index].rgbReserved;

        result = true;
    }

    return result;   
}

int32 
DIB::
IndexForColor(Color* color)
{
    return IndexForColor(color->r, color->g, color->b);
}

int32 
DIB::
IndexForColor(int32 r, int32 g, int32 b)
{
    int32 result = -1;
    
    if(BitsPerPixel() == 8)
    {
        uint32 p, d, bestCandidate = 0, leastDifference = 0xFFFFFFFF;
        RGBQUAD* palette = Palette();

        for(p = 0; p < 256; p++)
        {
            d =  abs(palette[p].rgbRed - r)^2;
            d += abs(palette[p].rgbGreen - g)^2;
            d += abs(palette[p].rgbBlue - b)^2;

            if(d < leastDifference)
            {
                leastDifference = d;
                bestCandidate = p;
            }
        }

        result = bestCandidate;
    }

    return result;
}