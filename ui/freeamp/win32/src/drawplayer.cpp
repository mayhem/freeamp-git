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
#include <commctrl.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <math.h>
#include <mmsystem.h >


/* project headers */
#include "config.h"
#include "resource.h"
#define DEFINE_FONT_WIDTHS
#include "font_width.h"
#include "drawplayer.h"
#include "freeampui.h"
#include "playlist.h"
#include "eventdata.h"

#define INTRO_COUNT         13

extern FreeAmpUI *g_ui;


#ifdef __cplusplus
extern "C" {
#endif


LRESULT WINAPI MainWndProc( HWND, UINT, WPARAM, LPARAM );

extern HINSTANCE g_hInst;		//  Program instance handle


ControlInfo g_buttonStateArray[kNumControls];
DisplayInfo g_displayInfo;

static HBITMAP leftBitmap;
static HBITMAP middleBitmap;
static HBITMAP rightBitmap;
static HBITMAP dialBitmap;
static HBITMAP playButtonBitmap;
static HBITMAP stopButtonBitmap;
static HBITMAP pauseButtonBitmap;
static HBITMAP nextButtonBitmap;
static HBITMAP lastButtonBitmap;
static HBITMAP modeButtonBitmap;
static HBITMAP minimizeButtonBitmap;
static HBITMAP closeButtonBitmap;
static HRGN displayRegion;
static HBITMAP playerMask;
static HBITMAP smallFont, largeFont;
static HDC tempDC;
static HCURSOR dialCursor, arrowCursor, currentCursor;
static HBITMAP logoBitmap;
static HPALETTE palette = NULL;

void DrawPlayer(HDC hdc, ControlInfo* state)
{
    HDC memdc, bufferdc;
    HBITMAP oldMemBitmap, bufferBitmap, oldBufferBitmap;
    int width, height;
    BITMAP bmp;
    int32 i;

    if(palette)
    {
        SelectPalette (hdc, palette, FALSE);
        RealizePalette (hdc);
    }

    bufferBitmap = CreateCompatibleBitmap(  hdc, 
                                            PLAYER_WINDOW_WIDTH, 
                                            PLAYER_WINDOW_HEIGHT);

    memdc = CreateCompatibleDC(hdc);
    bufferdc = CreateCompatibleDC(hdc);
    oldMemBitmap = (HBITMAP)SelectObject(memdc, leftBitmap);
    oldBufferBitmap = (HBITMAP)SelectObject(bufferdc, bufferBitmap);

    GetObject(leftBitmap, sizeof(BITMAP), (LPSTR)&bmp);

    BitBlt( bufferdc, 
            0, 0, bmp.bmWidth, bmp.bmHeight,
            memdc, 
            0,0,
            SRCCOPY);

    width = bmp.bmWidth; 
    height = bmp.bmHeight;

    SelectObject(memdc, dialBitmap);

    GetObject(dialBitmap, sizeof(BITMAP), (LPSTR)&bmp);

    BitBlt( bufferdc, 
            width, 0, DIAL_SECTION, bmp.bmHeight,
            memdc, 
            0,0,
            SRCCOPY);

    width += DIAL_SECTION; 
    height = bmp.bmHeight;

    SelectObject(memdc, middleBitmap);

    GetObject(middleBitmap, sizeof(BITMAP), (LPSTR)&bmp);

    BitBlt( bufferdc, 
            width, 0, bmp.bmWidth, bmp.bmHeight,
            memdc, 
            0,0,
            SRCCOPY);

    width += bmp.bmWidth; 
    height = bmp.bmHeight;

    SelectObject(memdc, dialBitmap);

    GetObject(dialBitmap, sizeof(BITMAP), (LPSTR)&bmp);

    BitBlt( bufferdc, 
            width, 0, DIAL_SECTION, bmp.bmHeight,
            memdc, 
            0,0,
            SRCCOPY);

    width += DIAL_SECTION; 
    height = bmp.bmHeight;

    SelectObject(memdc,rightBitmap);

    GetObject(rightBitmap, sizeof(BITMAP), (LPSTR)&bmp);

    BitBlt( bufferdc, 
            width, 0, bmp.bmWidth, bmp.bmHeight,
            memdc, 
            0,0,
            SRCCOPY);

    width += bmp.bmWidth; 
    height = bmp.bmHeight;


    // Render Display
#define LogoHeight  18
#define FreeWidth   41
#define AmpWidth    45

    SelectClipRgn(bufferdc, displayRegion);

    RECT displayRect;
    GetRgnBox(displayRegion, &displayRect);

    switch(g_displayInfo.state)
    {
        case Intro: // hard coded hack just to look good <grin>
        {         
            int32 xIntroPos = displayRect.left + 32;
            int32 yIntroPos = displayRect.top + 5;

            SelectObject(memdc, logoBitmap);


            BitBlt( bufferdc, 
                    xIntroPos - g_displayInfo.introOffset, 
                    yIntroPos, 
                    FreeWidth, 
                    LogoHeight, 
                    memdc, 
                    0,
                    0,
                    SRCCOPY);

            BitBlt( bufferdc, 
                    xIntroPos + FreeWidth + g_displayInfo.introOffset, 
                    yIntroPos, 
                    AmpWidth, 
                    LogoHeight, 
                    memdc, 
                    0,
                    LogoHeight,
                    SRCCOPY);

            break;
        }

        /*
        strcpy(g_displayInfo.path, "Alienated - Free Return.mp3");
            g_displayInfo.frame = 0;
            g_displayInfo.range = 1200;
            g_displayInfo.hours = 0;
            g_displayInfo.minutes = 1;
            g_displayInfo.seconds = 26;
            g_displayInfo.volume = 85;*/

        case Volume:
        {
            char volString[] = "volume";
            char valueString[4];

            int32 offset = displayRect.left + 50;
    
            SelectObject(memdc, smallFont);

            for(i = 0; volString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        26, 
                        smallFontWidth[volString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (volString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[volString[i] - 32];

            }

            wsprintf(valueString, "%d%%", g_displayInfo.volume);

            offset = displayRect.left + 3;

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        25, 
                        largeFontWidth[valueString[i] - 32], 
                        12,
                        memdc, 
                        0,
                        (valueString[i] - 32)*12,
                        SRCCOPY);

                offset += largeFontWidth[valueString[i] - 32];

            }

            break;
        }

        case CurrentTime:
        {
            char labelString[] = "current time";
            char valueString[12];

            int32 offset = displayRect.left + 3;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        26, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }

            if(g_displayInfo.hours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", g_displayInfo.hours,
                                                        g_displayInfo.minutes,
                                                        g_displayInfo.seconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   g_displayInfo.minutes,
                                                        g_displayInfo.seconds);

                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        25, 
                        largeFontWidth[valueString[i] - 32], 
                        12,
                        memdc, 
                        0,
                        (valueString[i] - 32)*12,
                        SRCCOPY);

                offset += largeFontWidth[valueString[i] - 32];

            }
            break;
        }

        case SeekTime:
        {
            char labelString[] = "seek time";
            char valueString[12];

            int32 offset = displayRect.left + 3;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        26, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }

            if(g_displayInfo.hours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", g_displayInfo.seekhours,
                                                        g_displayInfo.seekminutes,
                                                        g_displayInfo.seekseconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   g_displayInfo.seekminutes,
                                                        g_displayInfo.seekseconds);

                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        25, 
                        largeFontWidth[valueString[i] - 32], 
                        12,
                        memdc, 
                        0,
                        (valueString[i] - 32)*12,
                        SRCCOPY);

                offset += largeFontWidth[valueString[i] - 32];

            }
            break;
        }

        case RemainingTime:
        {
            char labelString[] = "remaining time";
            char valueString[12];

            int32 offset = displayRect.left + 3;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        26, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }
			int32 totalSeconds = g_displayInfo.totalhours * 3600;
			totalSeconds += g_displayInfo.totalminutes * 60;
			totalSeconds += g_displayInfo.totalseconds;
			int32 currSeconds = g_displayInfo.hours * 3600;
			currSeconds += g_displayInfo.minutes * 60;
			currSeconds += g_displayInfo.seconds;

			int32 displaySeconds = totalSeconds - currSeconds;
			int32 displayHours = displaySeconds / 3600;
			int32 displayMinutes = (displaySeconds - (displayHours * 3600)) / 60;
			displaySeconds = displaySeconds - (displayHours * 3600) - (displayMinutes * 60);

            if(displayHours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", displayHours,displayMinutes,displaySeconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d", displayMinutes, displaySeconds);
				//OutputDebugString(valueString);
				//OutputDebugString("\n");
                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        25, 
                        largeFontWidth[valueString[i] - 32], 
                        12,
                        memdc, 
                        0,
                        (valueString[i] - 32)*12,
                        SRCCOPY);

                offset += largeFontWidth[valueString[i] - 32];

            }
            break;
        }

        case TotalTime:
        {
            char labelString[] = "total time";
            char valueString[12];

            int32 offset = displayRect.left + 3;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        26, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }

            if(g_displayInfo.hours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", g_displayInfo.totalhours,
                                                        g_displayInfo.totalminutes,
                                                        g_displayInfo.totalseconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   g_displayInfo.totalminutes,
                                                        g_displayInfo.totalseconds);

                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        25, 
                        largeFontWidth[valueString[i] - 32], 
                        12,
                        memdc, 
                        0,
                        (valueString[i] - 32)*12,
                        SRCCOPY);

                offset += largeFontWidth[valueString[i] - 32];

            }
            break;
        }
    }

    if(g_displayInfo.state != Intro)
    {
        int32 offset = displayRect.left + 3;

        SelectObject(memdc, smallFont);

        for(i = 0; g_displayInfo.path[i]; i++)
        {
            BitBlt( bufferdc, 
                    offset - g_displayInfo.scrollOffset, 
                    10, 
                    smallFontWidth[g_displayInfo.path[i] - 32], 
                    10,
                    memdc, 
                    0,
                    (g_displayInfo.path[i] - 32)*10,
                    SRCCOPY);

            offset += smallFontWidth[g_displayInfo.path[i] - 32];
        }

    }

    SelectClipRgn(bufferdc, NULL);

    // Render Controls...
    for(i = 0; i < kNumControls; i++)
    {
        RECT rect;

        GetRgnBox(g_buttonStateArray[i].region, &rect);

        if(1)//g_buttonStateArray[i].dirty)
        {
            switch(g_buttonStateArray[i].control_id)
            {
                case kModeControl:
                case kMinimizeControl:
                case kCloseControl:
                {
                    int32 srcOffset = 0;

                    SelectObject(memdc, g_buttonStateArray[i].bitmap);
                    
                    if( g_buttonStateArray[i].state == Selected )
                    {
                        srcOffset = 14;
                    }
                    else if( g_buttonStateArray[i].state == Pressed )
                    {
                        srcOffset = 28;
                    }

                    BitBlt( bufferdc, 
                            rect.left, 
                            rect.top, 
                            rect.right - rect.left, 
                            rect.bottom - rect.top, 
                            memdc, 
                            0,
                            srcOffset,
                            SRCCOPY);

                    currentCursor = arrowCursor;
                    break;        
                }

                case kPlayControl:
                case kPauseControl:
                case kStopControl:
                case kNextControl:
                case kLastControl:
                {
                    int32 srcOffset = 0;

                    SelectObject(memdc, g_buttonStateArray[i].bitmap);

                    if( g_buttonStateArray[i].state == Selected )
                    {
                        srcOffset = 20;
                    }
                    else if( g_buttonStateArray[i].state == Activated )
                    {
                        srcOffset = 40;
                    }
                    else if( g_buttonStateArray[i].state == Pressed )
                    {
                        srcOffset = 40;
                    }

                    if(g_buttonStateArray[i].shown)
                    {
                        BitBlt(bufferdc, 
                            rect.left, 
                            rect.top, 
                            rect.right - rect.left, 
                            rect.bottom - rect.top, 
                            memdc, 
                            0,
                            srcOffset,
                            SRCCOPY);
                    }

                    currentCursor = arrowCursor;
                    break;        
                }

                
                case kPlaylistControl:
                case kDisplayControl:
                {
                    currentCursor = arrowCursor;
                    break;
                }

                case kVolumeControl:
                case kSeekControl:
                {
                    if( g_buttonStateArray[i].state == Selected )
                    {
                        currentCursor = dialCursor;
                    }
                    else if( g_buttonStateArray[i].state == Pressed )
                    {
                        int32 srcOffset = 0;

                        SelectObject(memdc, dialBitmap);

                        /*vErrorOut(bg_blue|fg_white, 
                            "position = %d\toffset = %d\r\n", 
                            g_buttonStateArray[i].position, 
                            srcOffset);*/

                        srcOffset = g_buttonStateArray[i].position * 11;
                                            
                        BitBlt( bufferdc, 
                                rect.left, 
                                rect.top, 
                                rect.right - rect.left, 
                                rect.bottom - rect.top, 
                                memdc, 
                                srcOffset,
                                0,
                                SRCCOPY);
                    }

                    break;
                }


            }

            g_buttonStateArray[i].dirty = FALSE;
        }
    } 

    BitBlt( hdc, 
            0, 
            0, 
            PLAYER_WINDOW_WIDTH, 
            PLAYER_WINDOW_HEIGHT,
            bufferdc, 
            0,
            0,
            SRCCOPY);

    SelectObject(bufferdc, oldBufferBitmap); 
    SelectObject(memdc, oldMemBitmap); 
    DeleteDC(bufferdc);
    DeleteDC(memdc);
    DeleteObject(bufferBitmap);
}

static HPALETTE 
CreateDIBPalette (LPBITMAPINFO lpbmi, 
				  LPINT lpiNumColors) 
{
	LPBITMAPINFOHEADER lpbi;
	LPLOGPALETTE lpPal;
	HANDLE hLogPal;
	HPALETTE hPal = NULL;
	INT i;
 
	lpbi = (LPBITMAPINFOHEADER)lpbmi;
	if (lpbi->biBitCount <= 8) {
		*lpiNumColors = (1 << lpbi->biBitCount);
	}
	else
		*lpiNumColors = 0;  // No palette needed for 24 BPP DIB
 
	if (*lpiNumColors) {
		hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
                             sizeof (PALETTEENTRY) * (*lpiNumColors));
		lpPal = (LPLOGPALETTE) GlobalLock (hLogPal);
		lpPal->palVersion = 0x300;
		lpPal->palNumEntries = *lpiNumColors;
 
		for (i = 0;  i < *lpiNumColors;  i++) {
			lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
		}
		hPal = CreatePalette (lpPal);
		GlobalUnlock (hLogPal);
		GlobalFree (hLogPal);
   }
   return hPal;
}

static HBITMAP 
LoadResourceBitmap (HINSTANCE hInstance, 
					LPSTR lpString,
					HPALETTE* lphPalette) 
{
	HRSRC  hRsrc;
	HGLOBAL hGlobal;
	HBITMAP hBitmapFinal = NULL;
	LPBITMAPINFOHEADER lpbi;
	HDC hdc;
    INT iNumColors;
 
	if (hRsrc = FindResource (hInstance, lpString, RT_BITMAP)) {
		hGlobal = LoadResource (hInstance, hRsrc);
		lpbi = (LPBITMAPINFOHEADER)LockResource (hGlobal);
 
		hdc = GetDC(NULL);
		*lphPalette =  CreateDIBPalette ((LPBITMAPINFO)lpbi, &iNumColors);
		if (*lphPalette) {
			SelectPalette (hdc,*lphPalette,FALSE);
			RealizePalette (hdc);
		}
 
		hBitmapFinal = CreateDIBitmap (hdc,
                   (LPBITMAPINFOHEADER)lpbi,
                   (LONG)CBM_INIT,
                   (LPSTR)lpbi + lpbi->biSize + iNumColors * sizeof(RGBQUAD),
                   (LPBITMAPINFO)lpbi,
                   DIB_RGB_COLORS );
 
		ReleaseDC (NULL,hdc);
		UnlockResource (hGlobal);
		FreeResource (hGlobal);
	}
	return (hBitmapFinal);
}




LRESULT WINAPI MainWndProc( HWND hwnd, 
                            UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam )
{
    LRESULT result = FALSE;
    static POINT pressPt;
    static BOOL pressed = FALSE;
    static int32 seekSpeed = 0;
    static HWND hwndTooltip = NULL;
    

    switch( msg )
	{
		case WM_CREATE:
		{
			DragAcceptFiles(hwnd, TRUE);

            InitCommonControls();

			HRGN bodyRegion, capRegion, playerRegion, tempRegion;
            int32 i;
            int32 xPos, yPos;
   
            // Create the region and apply it to the window
            capRegion = CreateEllipticRgn(  0,
                                            0,
                                            CAP_CIRCUMFERENCE + 1,
                                            CAP_CIRCUMFERENCE + 1);

            bodyRegion = CreateRectRgn( 0,
                                        0,
                                        BODY_WIDTH,
                                        BODY_HEIGHT);

            playerRegion = bodyRegion;

            OffsetRgn(bodyRegion, CAP_CIRCUMFERENCE/2, 0);

            CombineRgn( playerRegion,
                        capRegion,
                        bodyRegion,
                        RGN_OR);

            OffsetRgn(capRegion, BODY_WIDTH, 0);

            CombineRgn( playerRegion,
                        playerRegion,
                        capRegion,
                        RGN_OR);


            SetWindowRgn( hwnd, playerRegion, TRUE );

            dialCursor = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_DIAL));
            arrowCursor = LoadCursor(NULL, IDC_ARROW);
            currentCursor = arrowCursor;

            HDC hdc = GetDC(hwnd);
            int32 numBits = 0;

            if(hdc)
			{
				numBits = GetDeviceCaps(hdc, BITSPIXEL) * 
					GetDeviceCaps(hdc, PLANES);

				ReleaseDC(hwnd, hdc);
			}

            if(numBits == 8) 
            {
                logoBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_LOGO_256), &palette);
                leftBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_LEFT_256), &palette);
                middleBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_MID_256), &palette);
                rightBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_RIGHT_256), &palette);
                dialBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_DIALS_256), &palette);
                smallFont = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_SMALL_FONT_256), &palette);
                largeFont = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIG_FONT_256), &palette);

                playButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAY_256), &palette);
                stopButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_STOP_256), &palette);
                pauseButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_PAUSE_256), &palette);
                nextButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_NEXT_256), &palette);
                lastButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_LAST_256), &palette);

                modeButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_MODE_256), &palette);
                minimizeButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINIMIZE_256), &palette);
                closeButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLOSE_256), &palette);

			}
            else
            {
                logoBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LOGO));
                leftBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LEFT));
                middleBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MID));
                rightBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_RIGHT));
                dialBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DIALS));
                smallFont = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SMALL_FONT));
                largeFont = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIG_FONT));

                playButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAY));
                stopButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_STOP));
                pauseButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PAUSE));
                nextButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_NEXT));
                lastButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LAST));

                modeButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MODE));
                minimizeButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MINIMIZE));
                closeButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_CLOSE));
            }
            
            playerMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAYER_MASK));

            tempDC = CreateCompatibleDC(NULL);

            for(i = 0; i < 5; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = FALSE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].shown = TRUE;
                g_buttonStateArray[i].state = Deactivated;

                if(i == kPlayControl)
                    g_buttonStateArray[i].bitmap = playButtonBitmap;
                else if(i == kPauseControl)
                    g_buttonStateArray[i].bitmap = pauseButtonBitmap;
                else if(i == kStopControl)
                    g_buttonStateArray[i].bitmap = stopButtonBitmap;
                else if(i == kNextControl)
                    g_buttonStateArray[i].bitmap = nextButtonBitmap;
                else if(i == kLastControl)
                    g_buttonStateArray[i].bitmap = lastButtonBitmap;

            } 

            // stop shown when play is pressed
            g_buttonStateArray[kStopControl].shown = FALSE;

            for(i = 5; i < 8; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = FALSE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].shown = TRUE;
                g_buttonStateArray[i].state = Deactivated;

                if(i == kModeControl)
                    g_buttonStateArray[i].bitmap = modeButtonBitmap;
                else if(i == kMinimizeControl)
                    g_buttonStateArray[i].bitmap = minimizeButtonBitmap;
                else if(i == kCloseControl)
                    g_buttonStateArray[i].bitmap = closeButtonBitmap;
            } 

            for(i = 8; i < 10; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = FALSE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].shown = TRUE;
                g_buttonStateArray[i].state = Deactivated;
                g_buttonStateArray[i].bitmap = dialBitmap;
            } 

            // play buttons
            tempRegion = CreateEllipticRgn( 29,15,50,36);
            g_buttonStateArray[0].region = tempRegion;
            tempRegion = CreateEllipticRgn( 29,15,50,36);
            g_buttonStateArray[4].region = tempRegion;
            tempRegion = CreateEllipticRgn( 56,15,77,36);
            g_buttonStateArray[1].region = tempRegion;
            tempRegion = CreateEllipticRgn( 82,15,103,36);
            g_buttonStateArray[2].region = tempRegion;
            tempRegion = CreateEllipticRgn( 109,15,130,36);
            g_buttonStateArray[3].region = tempRegion;

            // system buttons
            int32 systemButtonsOffset = LEFT_SECTION + 2*DIAL_SECTION + MID_SECTION;

            tempRegion = CreateEllipticRgn( systemButtonsOffset + 9,
                                            18,
                                            systemButtonsOffset + 9 + 13,
                                            33);
            g_buttonStateArray[5].region = tempRegion;

            tempRegion = CreateEllipticRgn( systemButtonsOffset + 25,
                                            18,
                                            systemButtonsOffset + 25 + 13,
                                            33);
            g_buttonStateArray[6].region = tempRegion;

            tempRegion = CreateEllipticRgn( systemButtonsOffset + 41,
                                            18,
                                            systemButtonsOffset + 41 + 13,
                                            33);
            g_buttonStateArray[7].region = tempRegion;

            // dials
            int32 dialOffset = LEFT_SECTION;

            tempRegion = CreateRectRgn( dialOffset,0,dialOffset + DIAL_SECTION,50);
            g_buttonStateArray[8].region = tempRegion;
            dialOffset = LEFT_SECTION + DIAL_SECTION + MID_SECTION;
            tempRegion = CreateRectRgn( dialOffset,
                                        0,
                                        dialOffset + DIAL_SECTION,
                                        50);
            g_buttonStateArray[9].region = tempRegion;

            hwndTooltip = CreateWindow( TOOLTIPS_CLASS, 
                                        NULL, 
                                        0, 
                                        CW_USEDEFAULT, 
                                        CW_USEDEFAULT, 
                                        CW_USEDEFAULT, 
                                        CW_USEDEFAULT, 
                                        NULL, 
                                        (HMENU) NULL, 
                                        g_hInst, 
                                        NULL);
            RECT toolRect;
            TOOLINFO ti;

            for(i = 0; i < kNumControls; i++)
            {
                GetRgnBox(g_buttonStateArray[i].region, &toolRect);

                ti.cbSize = sizeof(TOOLINFO); 
                ti.uFlags = 0; 
                ti.hwnd = hwnd; 
                ti.hinst = g_hInst; 
                ti.uId = (UINT) i; 
                ti.lpszText = (LPSTR) LPSTR_TEXTCALLBACK; 
                ti.rect.left = toolRect.left; 
                ti.rect.top = toolRect.top; 
                ti.rect.right = toolRect.right; 
                ti.rect.bottom = toolRect.bottom; 

                if(i != kStopControl)
                {
                    SendMessage(hwndTooltip, 
                            TTM_ADDTOOL, 
                            0, 
                            (LPARAM) &ti);
                }
            }


            


            // display
            int32 displayOffset = LEFT_SECTION + DIAL_SECTION + 19;

            displayRegion = CreateRectRgn(  displayOffset, 
                                            11,
                                            displayOffset + 139,
                                            38);

            g_displayInfo.state = Intro;
            strcpy(g_displayInfo.path, "Welcome to FreeAmp");
            g_displayInfo.frame = 0;
            g_displayInfo.range = 1200;
            g_displayInfo.hours = 0;
            g_displayInfo.minutes = 0;
            g_displayInfo.seconds = 0;

            waveOutGetVolume((HWAVEOUT)WAVE_MAPPER, (DWORD*)&g_displayInfo.volume);

            g_displayInfo.volume = (int32)(100 * ((float)LOWORD(g_displayInfo.volume)/(float)0xffff));

            xPos = (GetSystemMetrics (SM_CXFULLSCREEN) - (PLAYER_WINDOW_WIDTH)) / 2;
			yPos = (GetSystemMetrics (SM_CYFULLSCREEN) - PLAYER_WINDOW_HEIGHT) / 2;
				
			SetWindowPos (	hwnd,
							0, 
							xPos, 
							yPos, 
							0 , 
							0, 
							SWP_NOSIZE|SWP_NOZORDER);


            SetTimer(hwnd, 0x00, 100, NULL);
            SetTimer(hwnd, 0x01, 200, NULL);
			break;
		}

        case WM_TIMER:
        {
            HDC hdc = GetDC(hwnd);

            switch (wParam)
            {
                case 0:
                {
                    static int32 count = 0;

                    if(count == 15)
                    {
                        KillTimer(hwnd, 0x00);
                        g_displayInfo.state = TotalTime;
                    }
                    else
                    {
                        static double multiplier = 0;

                        g_displayInfo.introOffset = (int32)(multiplier * count++);

                        multiplier += .55;
                    }
                    break;
                }

                case 1:
                {
                    int32 length = 0;
                    int32 i;
                    RECT rect;
                    static int32 increment = 1;

                    GetRgnBox(displayRegion, &rect);

                    // first determine length
                    for(i = 0; g_displayInfo.path[i]; i++)
                    {
                        length += smallFontWidth[g_displayInfo.path[i] - 32];
                    }

                    // see if we need to scroll 
                    if(length > rect.right - rect.left)
                    {

                        if( increment > 0 &&
                            length - g_displayInfo.scrollOffset + 3< rect.right - rect.left)
                        {
                            increment = -1;
                        }
                        else if( increment < 0 &&
                                 g_displayInfo.scrollOffset <= 0)
                        {   
                            increment = 1;
                        }

                        g_displayInfo.scrollOffset += increment;

                    }
                    else
                    {
                        g_displayInfo.scrollOffset = 0;
                    }

                    break;
                }

                case 2:
                {
                    char foo[1024];

                    g_displayInfo.seekframe += seekSpeed*20*abs(seekSpeed);

                    if(g_displayInfo.seekframe < 0)
                        g_displayInfo.seekframe = 0;
                    else if(g_displayInfo.seekframe > g_displayInfo.range)
                        g_displayInfo.seekframe = g_displayInfo.range;

                    int32 position = g_displayInfo.seekframe;

                    sprintf(foo,"position: %d\r\n",position);
				    OutputDebugString(foo);

                    int32 seconds = (int32)ceil(g_ui->m_secondsPerFrame * position);
					int32 hours = seconds / 3600;
					int32 minutes = seconds / 60 - hours * 60;
					seconds = seconds - minutes * 60 - hours * 3600;

                    sprintf(foo,"seconds: %d\r\n",seconds);

                    g_displayInfo.seekhours = hours;
                    g_displayInfo.seekminutes = minutes;
                    g_displayInfo.seekseconds = seconds;


                    break;
                }
            } 

            DrawPlayer(hdc, g_buttonStateArray);

            ReleaseDC(hwnd, hdc);

            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            
            hdc = BeginPaint( hwnd, &ps );

            DrawPlayer(hdc, g_buttonStateArray);
            
            EndPaint( hwnd, &ps );          
            break;
        }

        case WM_DESTROY:
        {
            DeleteDC(tempDC);

            DeleteObject(leftBitmap);
            DeleteObject(middleBitmap);
            DeleteObject(rightBitmap);
            DeleteObject(dialBitmap);
            DeleteObject(playerMask);
            DeleteObject(smallFont); 
            DeleteObject(largeFont);
            DeleteObject(logoBitmap);

            DeleteObject(displayRegion);


            if(palette)
                DeleteObject(palette);

            PostQuitMessage( 0 );             
            break;
        }

        case WM_SETCURSOR:
        {
            SetCursor(currentCursor);
            break;
        }

        case WM_LBUTTONUP:
        {   
            WPARAM fwKeys = wParam;        // key flags 
            LPARAM xPos = LOWORD(lParam);  // horizontal position of cursor 
            LPARAM yPos = HIWORD(lParam);  // vertical position of cursor 
            HDC hdc;
            int32 i;

            if(pressed)
            {
                for(i = 0; i < kNumControls; i++)
                {
                    if(i == kVolumeControl || i == kSeekControl )
                    {

                        if(PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y))
                        {
                            g_buttonStateArray[i].state = Deactivated;
                            g_buttonStateArray[i].dirty = TRUE;
                            g_buttonStateArray[i].position = 0;
                            g_displayInfo.state = g_displayInfo.oldstate;

                            if(i == kSeekControl)
                            {
                                KillTimer(hwnd, 0x02);

                                if(g_ui->m_state == STATE_Playing)
                                {
                                    g_ui->m_target->AcceptEvent(new ChangePositionEvent(g_displayInfo.seekframe));
                                }
                                else
                                {
                                    SendMessage(hwnd,WM_COMMAND,kPlayControl,0);
                                    g_ui->m_target->AcceptEvent(new ChangePositionEvent(g_displayInfo.seekframe));
                                }
                            }
                        }
                    }
                    else
                    {
                        if( PtInRegion(g_buttonStateArray[i].region, xPos, yPos) &&
                            PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) &&
                            g_buttonStateArray[i].state != Activated &&
                            g_buttonStateArray[i].shown)
                        {
                            g_buttonStateArray[i].state = Selected;
                            g_buttonStateArray[i].dirty = TRUE;
							SendMessage(hwnd,WM_COMMAND,g_buttonStateArray[i].control_id,0);
                        }
                        else if(g_buttonStateArray[i].state != Activated)
                        {
                            g_buttonStateArray[i].state = Deactivated;
                            g_buttonStateArray[i].dirty = TRUE;
                        }
                    }
                }

                // check on display area
                if( PtInRegion(displayRegion, xPos, yPos) )
                {
                    if(g_displayInfo.state == TotalTime)
                        g_displayInfo.state = CurrentTime;
                    else if(g_displayInfo.state == CurrentTime)
                        g_displayInfo.state = RemainingTime;
                    else if(g_displayInfo.state == RemainingTime)
                        g_displayInfo.state = TotalTime;
                }

            }

            hdc = GetDC(hwnd);

            DrawPlayer(hdc, g_buttonStateArray);

            ReleaseDC(hwnd, hdc);

            pressed = FALSE;
            ReleaseCapture();

            MSG relayMsg; 
 
            relayMsg.lParam = lParam; 
            relayMsg.wParam = wParam; 
            relayMsg.message = msg; 
            relayMsg.hwnd = hwnd; 
            SendMessage(hwndTooltip, 
                        TTM_RELAYEVENT, 
                        0, 
                        (LPARAM) &relayMsg); 
            break;
        }

        case WM_LBUTTONDOWN:
        {   
            WPARAM fwKeys = wParam;        // key flags 
            LPARAM xPos = LOWORD(lParam);  // horizontal position of cursor 
            LPARAM yPos = HIWORD(lParam);  // vertical position of cursor 
            HDC hdc;
            int32 i;

            pressPt.x = xPos;
            pressPt.y = yPos;

            if(g_displayInfo.state == Intro)
                g_displayInfo.state = TotalTime;

            for(i = 0; i < kNumControls; i++)
            {
                if( PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) )
                {
                    pressed = TRUE;
                    SetCapture(hwnd);

                    if(i == kVolumeControl)
                    {
                        g_displayInfo.oldstate = g_displayInfo.state;
                        g_displayInfo.state = Volume;
                    }
                    else if(i == kSeekControl)
                    {
                        g_displayInfo.oldstate = g_displayInfo.state;
                        g_displayInfo.state = SeekTime;
                        g_displayInfo.seekframe = g_displayInfo.frame;
                        SetTimer(hwnd, 0x02, 100, NULL);
                    }

                    g_buttonStateArray[i].oldstate = g_buttonStateArray[i].state;
                    g_buttonStateArray[i].state = Pressed;
                    g_buttonStateArray[i].dirty = TRUE;

                    hdc = GetDC(hwnd);

                    DrawPlayer(hdc, g_buttonStateArray);

                    ReleaseDC(hwnd, hdc);

                }
            } 

            // check on display area
            if( PtInRegion(displayRegion, pressPt.x, pressPt.y) )
            {
                pressed = TRUE;
                SetCapture(hwnd);
            }

            MSG relayMsg; 
 
            relayMsg.lParam = lParam; 
            relayMsg.wParam = wParam; 
            relayMsg.message = msg; 
            relayMsg.hwnd = hwnd; 
            SendMessage(hwndTooltip, 
                        TTM_RELAYEVENT, 
                        0, 
                        (LPARAM) &relayMsg); 
                
            break;
        }

        //  Tell Windows everything is the caption bar
        //  so the window can be dragged from anywhere
        case WM_MOUSEMOVE:
        case WM_NCHITTEST:   
        {
            LPARAM xPos = LOWORD(lParam);  // horizontal position of cursor 
            LPARAM yPos = HIWORD(lParam);  // vertical position of cursor 
            POINT pt;
            COLORREF color;
            HBITMAP oldBitmap;
            int32 i;
            HDC hdc;

            pt.x = xPos;
            pt.y = yPos;

            if(WM_NCHITTEST == msg)
                ScreenToClient(hwnd, &pt);

            /*vErrorOut(bg_blue|fg_pink, "yPos = %d\r\n", pt.y);*/

            oldBitmap = (HBITMAP)SelectObject(tempDC, playerMask);

            color = GetPixel(tempDC, pt.x, pt.y);

            if(color == RGB(0,0,0) && !pressed)
            {
                currentCursor = arrowCursor;
                result = HTCAPTION;
            }
            else
            {
                result = HTCLIENT;

                MSG relayMsg; 
 
                relayMsg.lParam = MAKELPARAM(pt.x, pt.y); 
                relayMsg.wParam = wParam; 
                relayMsg.message = WM_MOUSEMOVE; 
                relayMsg.hwnd = hwnd; 

                SendMessage(hwndTooltip, 
                            TTM_RELAYEVENT, 
                            0, 
                            (LPARAM) &relayMsg); 
                
            }
            

            for(i = 0; i < kNumControls; i++)
            {
                if(pressed)
                {
                    if(i == kVolumeControl || i == kSeekControl )
                    {
                        short delta = (short)(pt.y - pressPt.y);

                        delta = delta/-5;

                        if(delta == 0)
                        {
                            g_buttonStateArray[i].position = 0;
                        }
                        else if(delta > 0)
                        {
                            g_buttonStateArray[i].position = delta % 5;
                        }
                        else if(delta < 0)
                        {
                            g_buttonStateArray[i].position = ((delta % 5) + 5)  % 5;
                        }

                        if(i == kVolumeControl &&
							PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y))
                        {
                            static int32 lastDelta = 0;
                            int32 change;

                            if(lastDelta > delta)
                                change = -1;
                            else if(delta > lastDelta)
                                change = 1;
                            else
                                change = 0;

                            lastDelta = delta;

                            char foo[1024];
							sprintf(foo,"position: %d\n",delta);
							OutputDebugString(foo);

                            SendMessage(hwnd, WM_COMMAND, kVolumeControl, change);
                        }
                        else if(i == kSeekControl &&
							PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y))
                        {
                            SendMessage(hwnd, WM_COMMAND, kSeekControl, delta);
                        }

                        g_buttonStateArray[i].dirty = TRUE;

                    }
                    else
                    {
                        if( PtInRegion(g_buttonStateArray[i].region, pt.x, pt.y) &&
                            PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) )
                        {
                            g_buttonStateArray[i].state = Pressed;
                            g_buttonStateArray[i].dirty = TRUE;
                        }
                        else if( PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) )
                        {
                            g_buttonStateArray[i].state = g_buttonStateArray[i].oldstate;
                            g_buttonStateArray[i].dirty = TRUE;
                        }
                    }
                }
                else
                {
                    if( PtInRegion(g_buttonStateArray[i].region, pt.x, pt.y) &&
                        g_buttonStateArray[i].state != Activated)
                    {
                        g_buttonStateArray[i].state = Selected;
                        g_buttonStateArray[i].dirty = TRUE;

                    }
                    else if(g_buttonStateArray[i].state != Activated)
                    {
                        g_buttonStateArray[i].state = Deactivated;
                        g_buttonStateArray[i].dirty = TRUE;
                    }
                }
            } 

            hdc = GetDC(hwnd);

            DrawPlayer(hdc, g_buttonStateArray);

            ReleaseDC(hwnd, hdc);
           
            SelectObject(tempDC, oldBitmap); 

            break;
        }

        case WM_NOTIFY:
        {
            LPTOOLTIPTEXT lpttt; 
            int32 idCtrl; 

            if((((LPNMHDR) lParam)->code) == TTN_NEEDTEXT)
            {
                idCtrl = ((LPNMHDR) lParam)->idFrom;
                lpttt = (LPTOOLTIPTEXT) lParam; 

                switch (idCtrl) 
                { 
                    case kModeControl:
                        lpttt->lpszText = "Change Player Mode (disabled)"; 
                        break;
                    case kMinimizeControl:
                        lpttt->lpszText = "Minimize."; 
                        break;
                    case kCloseControl:
                        lpttt->lpszText = "Close"; 
                        break;
                    case kPlayControl:
                        if(g_ui->m_state != STATE_Playing)
                            lpttt->lpszText = "Play"; 
                        else
                            lpttt->lpszText = "Stop";
                        break;
                    case kPauseControl:
                        lpttt->lpszText = "Pause"; 
                        break;
                    case kNextControl:
                        lpttt->lpszText = "Next Song"; 
                        break;
                    case kLastControl:
                        lpttt->lpszText = "Previous Song."; 
                        break;
                    case kPlaylistControl:
                        lpttt->lpszText = "Display Playlist (disabled)"; 
                        break;
                    case kDisplayControl:
                        lpttt->lpszText = "Change Display Mode"; 
                        break;
                    case kVolumeControl:
                        lpttt->lpszText = "Volume"; 
                        break;
                    case kSeekControl:
                        lpttt->lpszText = "Seek"; 
                        break;
                }
            }

            break;
        }

        case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP) wParam;
			int32 count;
			char szFile[MAX_PATH + 1];

			count = DragQueryFile(	hDrop,
									-1L,
									szFile,
									sizeof(szFile));


			for(int32 i = 0; i < count; i++)
			{
				DragQueryFile(	hDrop,
								i,
								szFile,
								sizeof(szFile));

				g_ui->m_plm->Add(szFile,0);
				g_ui->m_plm->SetFirst();
			}
			if (count) {
				g_ui->m_target->AcceptEvent(new Event(CMD_Play));
                SendMessage(hwnd, WM_COMMAND, kPlayControl,0);
			}
			break;
		}

		case WM_COMMAND:
		{
			switch(wParam)
	        {
		        case kModeControl:
		        {
                    OPENFILENAME ofn;
					char szTitle[] = "Open Audio File";
					char szFilter[] =
					"MPEG Audio Streams (.mp1, .mp2, .mp3, .mpp)\0"
					"*.mp1;*.mp2;*.mp3;*.mpp\0"
					//"Playlists (.txt, .lst, .m3u)\0"
					//"*.lst;*.m3u;*.txt\0"
					"All Files (*.*)\0"
					"*.*\0";
					const int32 kBufferSize = MAX_PATH * 128;
					char* filelist = new char[kBufferSize];
					
					*filelist = 0x00;

					// Setup open file dialog box structure
					ofn.lStructSize       = sizeof(OPENFILENAME);
					ofn.hwndOwner         = hwnd;
					ofn.hInstance         = (HINSTANCE)GetWindowLong(hwnd, 
															GWL_HINSTANCE);
					ofn.lpstrFilter       = szFilter;
					ofn.lpstrCustomFilter = NULL;
					ofn.nMaxCustFilter    = 0;
					ofn.nFilterIndex      = 1;
					ofn.lpstrFile         = filelist;
					ofn.nMaxFile          = kBufferSize;
					ofn.lpstrFileTitle    = NULL;
					ofn.nMaxFileTitle     = 0;
					ofn.lpstrInitialDir   = "";
					ofn.lpstrTitle        = szTitle;
					ofn.Flags             = OFN_FILEMUSTEXIST | 
											OFN_PATHMUSTEXIST |
  	     				   					OFN_HIDEREADONLY | 
											OFN_ALLOWMULTISELECT |
											OFN_EXPLORER;
					ofn.nFileOffset       = 0;
					ofn.nFileExtension    = 0;
					ofn.lpstrDefExt       = "MP*";
					ofn.lCustData         = 0;
					ofn.lpfnHook          = NULL;
					ofn.lpTemplateName    = NULL;

					if(GetOpenFileName(&ofn))
					{
						char file[MAX_PATH + 1];
						char* cp = NULL;

						strcpy(file, filelist);
						strcat(file, "\\");

						cp = filelist + ofn.nFileOffset;

						while(*cp)
						{
							strcpy(file + ofn.nFileOffset, cp);

					        g_ui->m_plm->Add(file,0);

					        cp += strlen(cp) + 1;
						}

                        if(g_ui->m_state == STATE_Playing)
                        {
                            g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
                            g_ui->m_target->AcceptEvent(new Event(CMD_Play));
                        }
                        else
                        {
							;
                        }
                        //EnableWindow(m_ui->m_hwndPlay, TRUE);
					}

                    g_ui->m_plm->SetFirst();

					delete [] filelist;

                    g_buttonStateArray[kModeControl].state = Deactivated;

			        break;        
		        }

		        case kMinimizeControl:
		        {
                    ShowWindow(hwnd, SW_MINIMIZE);
                    g_buttonStateArray[kMinimizeControl].state = Deactivated;
			        break;        
		        }

		        case kCloseControl:
		        {
			        DestroyWindow( hwnd );
			        break;        
		        }

                case kPauseControl:
                {
                    g_ui->m_target->AcceptEvent(new Event(CMD_TogglePause));
                    break;
                }

		        case kPlayControl:
		        {
		            g_ui->m_target->AcceptEvent(new Event(CMD_Play));
			        break;        
		        }

		        case kStopControl:
		        {
			        g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
			        break;        
		        }

		        case kNextControl:
		        {
			        g_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
			        break;        
		        }

		        case kLastControl:
		        {
			        g_ui->m_target->AcceptEvent(new Event(CMD_PrevMediaPiece));
			        break;        
		        }
    
		        case kPlaylistControl:
		        {
			        break;        
		        }

		        case kDisplayControl:
		        {
			        break;        
		        }

		        case kVolumeControl:
		        {
		           //vErrorOut(fg_pink, "delta = %d\r\n", lParam);
			        float percent;
			        g_displayInfo.volume+=lParam;

			        if(g_displayInfo.volume > 100)
				        g_displayInfo.volume = 100;
			        else if(g_displayInfo.volume < 0)
				        g_displayInfo.volume = 0;
        
			        percent = (float)g_displayInfo.volume/(float)100;

			        waveOutSetVolume( (HWAVEOUT)WAVE_MAPPER , MAKELPARAM(0xFFFF*percent, 0xFFFF*percent));

			        break;        
		        }

		        case kSeekControl:
		        {
                    if(lParam > 5)
                        lParam = 5;
                    else if(lParam < -5)
                        lParam = -5;

                    seekSpeed = lParam;
			        break;
		        }
	        }

			break;
		}

        default:
        {
            result = DefWindowProc( hwnd, msg, wParam, lParam );
            break;
        }
	}

	return result;
}                                                        


#ifdef __cplusplus
}
#endif