
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


DIB::DIB()
{
    m_handle = NULL;
    m_bits = NULL;
    m_height = 0;
    m_width = 0;
    m_bytesPerLine = 0;
    m_bitCount = 0;
}

DIB::~DIB()
{
    
}


bool DIB::Create(uint32 width, 
				 uint32 height, 
				 uint32 bitsPerPixel)
{
	bool result = false;

    m_width = width;
    m_height = height;
    m_bitCount = bitsPerPixel;

    HDC hdc = CreateCompatibleDC(NULL);

	m_bitmapInfo.bmiHeader.biSize = sizeof BITMAPINFOHEADER;
    m_bitmapInfo.bmiHeader.biWidth = m_width;
    m_bitmapInfo.bmiHeader.biHeight = m_height;
    m_bitmapInfo.bmiHeader.biPlanes = 1;
    m_bitmapInfo.bmiHeader.biBitCount = m_bitCount;
    m_bitmapInfo.bmiHeader.biCompression = BI_RGB;
    m_bitmapInfo.bmiHeader.biSizeImage = 0;    
    m_bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    m_bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    m_bitmapInfo.bmiHeader.biClrUsed = 0;
    m_bitmapInfo.bmiHeader.biClrImportant = 0;

    
    m_handle = CreateDIBSection(   
                        hdc, 
                        &m_bitmapInfo,
                        DIB_RGB_COLORS,
                        &m_bits,
                        NULL,
                        NULL);

    GetObject(m_handle, sizeof DIBSECTION, &m_dibSection);

    m_bytesPerLine = m_dibSection.dsBm.bmWidthBytes;

    DeleteDC(hdc);

	return result;
}


bool DIB::Load(HANDLE module, LPCTSTR resource)
{
    bool result = false;

    m_handle = LoadImage(   module,
                            resource,
                            IMAGE_BITMAP, 
							0, 0, 
							LR_CREATEDIBSECTION);

    if(m_handle)
    {
        GetObject(m_handle, sizeof DIBSECTION, &m_dibSection);

	    m_bits = (BYTE*)m_dibSection.dsBm.bmBits;
	    m_height = m_dibSection.dsBmih.biHeight;
	    m_width = m_dibSection.dsBmih.biWidth;
	    //m_bytesPerLine = m_dibSection.dsBm.bmWidthBytes;
	    m_bitCount = m_dibSection.dsBmih.biBitCount;
        m_bytesPerLine = (m_width * BytesPerPixel()) + (m_width%4);

        memcpy(&m_bitmapInfo, &m_dibSection.dsBmih, sizeof BITMAPINFOHEADER);

	    result = true;
    }

    return result;
}

BYTE* DIB::Bits(int32 x, int32 y)
{
    BYTE* result = (BYTE*) m_bits;

    result += m_bytesPerLine*(m_height - y - 1) + x*BytesPerPixel();

    return result;
}
