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
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>

/* project headers */
#include "config.h"
#include "resource.h"
#include "utility.h"

HRGN
DetermineRegion(HBITMAP bitmap, 
			    Color* color)
{
	HRGN result = NULL;
    HRGN scanline = NULL;
    COLORREF regionColor = RGB(color->r, color->g, color->b);
    HBITMAP oldBitmap;
    BITMAP bmp;
    HDC hdc;
    int32 width, height;

    GetObject(bitmap, sizeof(BITMAP), (LPSTR)&bmp);
    width = bmp.bmWidth; 
    height = bmp.bmHeight;

    hdc = CreateCompatibleDC(NULL);

    oldBitmap = SelectBitmap(hdc, bitmap);

    // empty region
    result = CreateRectRgn(0,0,0,0);

    for(int32 y = 0; y < height; y++)
    {
        bool scanning = false;
        int32 start;

        for(int32 x = 0; x < width; x++)
        {
            COLORREF pixel = GetPixel(hdc, x, y);

            if(pixel == regionColor)
            {
                if(scanning)
                    continue;
                else
                {
                    scanning = true;
                    start = x;
                }
            }
            else if(scanning)
            {
                scanning = false;
                // regions are non-inclusive of their bottom & right edges
                // so need to add one to values
                scanline = CreateRectRgn(start, y, x, y + 1);

                CombineRgn( result,
                            result,
                            scanline,
                            RGN_OR);

            }

        }

        // last pixel in scanline is included in region...
        if(scanning)
        {
            scanline = CreateRectRgn(start, y, x, y + 1);

            CombineRgn( result,
                        result,
                        scanline,
                        RGN_OR);
        }
    }

    SelectObject(hdc, oldBitmap);
    DeleteDC(hdc);

	return result;
}

void
DetermineControlRegions(HBITMAP bitmap, 
                        HRGN* controlRegions,
			            Color* controlColors,
                        int32 numControls)
{
    HRGN scanline = NULL;
    COLORREF* regionColors;// = RGB(color->r, color->g, color->b);
    HBITMAP oldBitmap;
    BITMAP bmp;
    HDC hdc;
    int32 width, height;
    int32 i;

    for(i = 0; i < numControls; i++)
    {
        // empty region
        controlRegions[i] = CreateRectRgn(0,0,0,0);
    }

    regionColors = new COLORREF[numControls];

    for(i = 0; i < numControls; i++)
    {
        regionColors[i] = RGB(controlColors[i].r, controlColors[i].g, controlColors[i].b);
    }

    GetObject(bitmap, sizeof(BITMAP), (LPSTR)&bmp);
    width = bmp.bmWidth; 
    height = bmp.bmHeight;

    hdc = CreateCompatibleDC(NULL);
    oldBitmap = SelectBitmap(hdc, bitmap);

    COLORREF black = RGB(0, 0, 0);
    COLORREF white = RGB(255, 255, 255);

    for(int32 y = 0; y < height; y++)
    {
        bool scanning = false;
        int32 scanIndex;
        int32 start;

        for(int32 x = 0; x < width; x++)
        {
            COLORREF pixel = GetPixel(hdc, x, y);

            if(scanning )
            {
                if(pixel == regionColors[scanIndex])
                    continue;
                else
                {
                    scanning = false;
                    // regions are non-inclusive of their bottom & right edges
                    // so need to add one
                    scanline = CreateRectRgn(start, y, x, y + 1);

                    CombineRgn( controlRegions[scanIndex],
                                controlRegions[scanIndex],
                                scanline,
                                RGN_OR);
                }

                if(pixel != black && pixel != white)
                {
                    for(i = 0; i < numControls; i++)
                    {
                        if(pixel == regionColors[i])
                        {
                            scanIndex = i;
                            scanning = true;
                            start = x;
                            break;
                        }
                    }
                }
            }
            else
            {
                if(pixel != black && pixel != white)
                {
                    for(i = 0; i < numControls; i++)
                    {
                        if(pixel == regionColors[i])
                        {
                            //int32 r = GetRValue(pixel);
                            //int32 g = GetGValue(pixel);
                            //int32 b = GetBValue(pixel);

                            scanIndex = i;
                            scanning = true;
                            start = x;
                            break;
                        }
                    }
                }
            }
        }

        // last pixel in scanline is included in region...
        if(scanning)
        {
            scanline = CreateRectRgn(start, y, x, y + 1);

            CombineRgn( controlRegions[scanIndex],
                        controlRegions[scanIndex],
                        scanline,
                        RGN_OR);
        }
    }

    SelectObject(hdc, oldBitmap);
    DeleteDC(hdc);

    delete [] regionColors;
}
