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

#ifndef DIB_H
#define DIB_H

#include <windows.h>

#include "config.h"

typedef struct Color {
    short   r;
    short   g;
    short   b;
    short   a;

 public: 
     void Set(short rvalue, short gvalue, short bvalue, short avalue)
     {r = rvalue; g = gvalue, b = bvalue; a = avalue;}

     uint32 Pack()
     { return RGB(r, g ,b); }
}Color;


class DIB {

public:
	DIB();
	~DIB();

	virtual bool Create(uint32 width, 
						uint32 height,
						uint32 bitsPerPixel = 8);

	virtual bool Load(HANDLE module, LPCTSTR resource);

    void Pixel(int32 x, int32 y, Color* color);
    uint32 Pixel(int32 x, int32 y);

    BYTE* Bits() const { return m_bits; }
    BYTE* Bits(int32 x, int32 y);
    
    int32 Height() const { return m_height; }
    int32 Width() const { return m_width; }

    int32 BitsPerPixel() const { return m_bitCount; }
    int32 BytesPerLine() const { return m_bytesPerLine; }
    int32 BytesPerPixel() const { return (m_bitCount == 8) ? 1 : 3; }

    BITMAPINFO* BitmapInfo() { return m_bitmapInfo; }

    int32 NumberOfPaletteEntries() const;
    RGBQUAD* Palette();
    bool SetPalette(RGBQUAD* palette, int32 entries);

    bool ColorForIndex(int32 index, Color* color);
    int32 IndexForColor(Color* color);
    int32 IndexForColor(int32 r, int32 g, int32 b);

protected:
    BITMAPINFO* m_bitmapInfo;
    BYTE*       m_bits;
    int32       m_height;
    int32       m_width;
    int32       m_bytesPerLine;
    int32       m_bitCount;

private:
	

};

#endif /* DIB_H */
