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

typedef struct Color{
    short   r;
    short   g;
    short   b;
    short   a;
}Color;

Color g_buttonColorArray[] = {
        {0,   0,   255, 0}, // kPlayControl
        {255, 255, 0,   0}, // kPauseControl
        {0,   255, 0,   0}, // kLastControl
        {0,   255, 255, 0}, // kNextControl
        {0,   0,   255, 0}, // kStopControl
        {128, 128, 0,   0}, // kModeControl
        {128, 0,   128, 0}, // kMinimizeControl
        {0,   0,   128, 0}, // kCloseControl
        {0,   128, 128, 0}, // kRepeatControl
        {0,   128, 0,   0}, // kShuffleControl
        {255, 0,   0,   0}, // kOpenControl
        {255, 0,   255, 0}, // kVolumeControl
        {128, 0,   0,   0}, // kSeekControl
        {203, 203, 203, 0}, // kPlaylistControl
        {128, 128, 128, 0}, // kDisplayControl
};

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
static HBITMAP repeatButtonBitmap;
static HBITMAP shuffleButtonBitmap;
static HBITMAP openButtonBitmap;
static HBITMAP playerMask;
static HBITMAP oldPlayerMaskBitmap;
static HBITMAP smallFont, largeFont;
static HBITMAP logoBitmap;
static HBITMAP shuffledIconBitmap;
static HBITMAP repeatIconBitmap;
static HBITMAP allIconBitmap;

static HRGN g_displayRegion;
static HRGN g_playerRegion;
static HRGN* g_controlRegions = NULL;

static HDC playerMaskDC;
static HCURSOR dialCursor, arrowCursor, currentCursor;
static HPALETTE palette = NULL;


bool NeedToScroll()
{
    int32 length = 0;
    int32 i;
    RECT rect;
    bool result = false;

    GetRgnBox(g_displayRegion, &rect);

    // first determine length
    for(i = 0; g_displayInfo.path[i]; i++)
    {
        length += smallFontWidth[g_displayInfo.path[i] - 32];
    }

    // see if we need to scroll 
    if(length > rect.right - rect.left)
        result = true;

    return result;
}

static void DrawPlayer(HDC hdc, ControlInfo* state, bool repaintAll)
{
    HDC memdc, bufferdc;
    HBITMAP oldMemBitmap, bufferBitmap, oldBufferBitmap;
    int width, height;
    BITMAP bmp;
    int32 i;
    HRGN clipRegion = CreateRectRgn(0,0,0,0);

    
    
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

    if(repaintAll)
    {
        CombineRgn( clipRegion,
                    clipRegion,
                    g_playerRegion,
                    RGN_OR);

        GetObject(leftBitmap, sizeof(BITMAP), (LPSTR)&bmp);

        OutputDebugString("left side\r\n");
        BitBlt( bufferdc, 
                0, 0, bmp.bmWidth, bmp.bmHeight,
                memdc, 
                0,0,
                SRCCOPY);

        width = bmp.bmWidth; 
        height = bmp.bmHeight;

        SelectObject(memdc, dialBitmap);

        GetObject(dialBitmap, sizeof(BITMAP), (LPSTR)&bmp);

        OutputDebugString("volume dial\r\n");
        BitBlt( bufferdc, 
                width, 0, DIAL_SECTION, bmp.bmHeight,
                memdc, 
                0,0,
                SRCCOPY);

        width += DIAL_SECTION; 
        height = bmp.bmHeight;

        SelectObject(memdc, middleBitmap);

        GetObject(middleBitmap, sizeof(BITMAP), (LPSTR)&bmp);
        OutputDebugString("middle side\r\n");
        BitBlt( bufferdc, 
                width, 0, bmp.bmWidth, bmp.bmHeight,
                memdc, 
                0,0,
                SRCCOPY);

        width += bmp.bmWidth; 
        height = bmp.bmHeight;

        SelectObject(memdc, dialBitmap);

        GetObject(dialBitmap, sizeof(BITMAP), (LPSTR)&bmp);
        OutputDebugString("seek dial\r\n");
        BitBlt( bufferdc, 
                width, 0, DIAL_SECTION, bmp.bmHeight,
                memdc, 
                0,0,
                SRCCOPY);

        width += DIAL_SECTION; 
        height = bmp.bmHeight;

        SelectObject(memdc,rightBitmap);

        GetObject(rightBitmap, sizeof(BITMAP), (LPSTR)&bmp);
        OutputDebugString("right side\r\n");
        BitBlt( bufferdc, 
                width, 0, bmp.bmWidth, bmp.bmHeight,
                memdc, 
                0,0,
                SRCCOPY);

        width += bmp.bmWidth; 
        height = bmp.bmHeight;
    }


    // Render Controls...
    for(i = 0; i < kNumControls; i++)
    {
        RECT rect;

        GetRgnBox(g_buttonStateArray[i].region, &rect);

        if(g_buttonStateArray[i].dirty)
        {
            CombineRgn( clipRegion,
                        clipRegion,
                        g_buttonStateArray[i].region,
                        RGN_OR);

            switch(g_buttonStateArray[i].control_id)
            {
                case kModeControl:
                case kMinimizeControl:
                case kCloseControl:
                case kRepeatControl:
                case kShuffleControl:
                case kOpenControl:
                {
                    int32 srcOffset = 0;
                    
                    int32 id = g_buttonStateArray[i].control_id;

                    if(id == kModeControl)
                        OutputDebugString("Drawing kModeControl\r\n");
                    else if(id == kMinimizeControl)
                        OutputDebugString("Drawing kMinimizeControl\r\n");
                    else if(id == kCloseControl)
                        OutputDebugString("Drawing kCloseControl\r\n");
                    else if(id == kRepeatControl)
                        OutputDebugString("Drawing kRepeatControl\r\n");
                    else if(id == kShuffleControl)
                        OutputDebugString("Drawing kShuffleControl\r\n");
                    else if(id == kOpenControl)
                        OutputDebugString("Drawing kOpenControl\r\n");
                    else
                        OutputDebugString("UNKNOWN BLIT!!!!!!!!\r\n");
                   
                    SelectObject(memdc, g_buttonStateArray[i].bitmap);
                    
                    if( g_buttonStateArray[i].state == Selected )
                    {
                        srcOffset = 13;
                    }
                    else if( g_buttonStateArray[i].state == Pressed )
                    {
                        srcOffset = 26;
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

                    //currentCursor = arrowCursor;
                    break;        
                }

                case kPlayControl:
                case kPauseControl:
                case kStopControl:
                case kNextControl:
                case kLastControl:
                {
                    int32 srcOffset = 0;

                    int32 id = g_buttonStateArray[i].control_id;

                    if(id == kPlayControl)
                        OutputDebugString("Drawing kPlayControl\r\n");
                    else if(id == kPauseControl)
                        OutputDebugString("Drawing kPauseControl\r\n");
                    else if(id == kStopControl)
                        OutputDebugString("Drawing kStopControl\r\n");
                    else if(id == kNextControl)
                        OutputDebugString("Drawing kNextControl\r\n");
                    else if(id == kLastControl)
                        OutputDebugString("Drawing kLastControl\r\n");
                    else
                        OutputDebugString("UNKNOWN BLIT!!!!!!!!\r\n");

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

                    //currentCursor = arrowCursor;
                    break;        
                }

                
                case kPlaylistControl:
                case kDisplayControl:
                {
                    SelectObject(memdc, middleBitmap);

                    int32 id = g_buttonStateArray[i].control_id;

                    if(id == kPlaylistControl)
                        OutputDebugString("Drawing kPlaylistControl\r\n");
                    else if(id == kDisplayControl)
                        OutputDebugString("Drawing kDisplayControl\r\n");
                    else
                        OutputDebugString("UNKNOWN BLIT!!!!!!!!\r\n");

                    GetObject(middleBitmap, sizeof(BITMAP), (LPSTR)&bmp);

                    int32 offset = LEFT_SECTION + DIAL_SECTION; 

                    BitBlt( bufferdc, 
                            offset, 0, bmp.bmWidth, bmp.bmHeight,
                            memdc, 
                            0,0,
                            SRCCOPY);

                    //currentCursor = arrowCursor;
                    break;
                }

                case kVolumeControl:
                case kSeekControl:
                {
                    int32 srcOffset = 0;

                    int32 id = g_buttonStateArray[i].control_id;

                    if(id == kVolumeControl)
                        OutputDebugString("Drawing kVolumeControl\r\n");
                    else if(id == kSeekControl)
                        OutputDebugString("Drawing kSeekControl\r\n");
                    else
                        OutputDebugString("UNKNOWN BLIT!!!!!!!!\r\n");

                    if( g_buttonStateArray[i].state == Selected )
                    {
                        //currentCursor = dialCursor;
                    }
                    else if( g_buttonStateArray[i].state == Pressed )
                    {
                        /*vErrorOut(bg_blue|fg_white, 
                            "position = %d\toffset = %d\r\n", 
                            g_buttonStateArray[i].position, 
                            srcOffset);*/

                        srcOffset = g_buttonStateArray[i].position * 11;
                    }

                    SelectObject(memdc, dialBitmap);

                    BitBlt( bufferdc, 
                            rect.left, 
                            rect.top, 
                            11, 
                            rect.bottom - rect.top, 
                            memdc, 
                            srcOffset,
                            0,
                            SRCCOPY);

                    break;
                }


            }

            g_buttonStateArray[i].dirty = FALSE;
        }
    } 

    // this should not be here but deadline is approaching...
    RECT displayRect;
    GetRgnBox(g_displayRegion, &displayRect);

    if(g_displayInfo.shuffled)
    {
        int32 offset = displayRect.right + 3;

        SelectObject(memdc, shuffledIconBitmap);

        OutputDebugString("Drawing Shuffle Icon\r\n");
   
        BitBlt( bufferdc, 
                offset, 
                12, 
                11, 
                6,
                memdc, 
                0,
                0,
                SRCCOPY);
    }

    

    if(g_displayInfo.repeat)
    {
        int32 offset = displayRect.right + 2;

        SelectObject(memdc, repeatIconBitmap);
   
        OutputDebugString("Drawing Repeat Icon\r\n");
        BitBlt( bufferdc, 
                offset, 
                22, 
                12, 
                7,
                memdc, 
                0,
                0,
                SRCCOPY);

        if(g_displayInfo.repeatAll)
        {
            offset = displayRect.right + 3;

            SelectObject(memdc, allIconBitmap);
   
            OutputDebugString("Drawing Repeat All Icon\r\n");

            BitBlt( bufferdc, 
                    offset, 
                    29, 
                    11, 
                    8,
                    memdc, 
                    0,
                    0,
                    SRCCOPY);
        }
    }

    SelectClipRgn(hdc, clipRegion);

    OutputDebugString("Drawing To Screen\r\n");

    BitBlt( hdc, 
            0, 
            0, 
            PLAYER_WINDOW_WIDTH, 
            PLAYER_WINDOW_HEIGHT,
            bufferdc, 
            0,
            0,
            SRCCOPY);

    SelectClipRgn(hdc, NULL);

    DeleteObject(clipRegion);

    SelectObject(bufferdc, oldBufferBitmap); 
    SelectObject(memdc, oldMemBitmap); 
    DeleteDC(bufferdc);
    DeleteDC(memdc);
    DeleteObject(bufferBitmap);
}

void DrawDisplay(HDC hdc, DisplayInfo* state)
{
    HDC memdc, bufferdc;
    HBITMAP oldMemBitmap, bufferBitmap, oldBufferBitmap;
    int width;
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
    oldMemBitmap = (HBITMAP)SelectObject(memdc, middleBitmap);
    oldBufferBitmap = (HBITMAP)SelectObject(bufferdc, bufferBitmap);

    GetObject(middleBitmap, sizeof(BITMAP), (LPSTR)&bmp);

    width = LEFT_SECTION + DIAL_SECTION; 

    BitBlt( bufferdc, 
            width, 0, bmp.bmWidth, bmp.bmHeight,
            memdc, 
            0,0,
            SRCCOPY);

    // Render Display
#define LogoHeight  18
#define FreeWidth   41
#define AmpWidth    45


    RECT displayRect;
    GetRgnBox(g_displayRegion, &displayRect);

    switch(state->state)
    {
        case Intro: // hard coded hack just to look good <grin>
        {         
            int32 xIntroPos = displayRect.left + 32;
            int32 yIntroPos = displayRect.top + 6;

            SelectObject(memdc, logoBitmap);


            BitBlt( bufferdc, 
                    xIntroPos - state->introOffset, 
                    yIntroPos, 
                    FreeWidth, 
                    LogoHeight, 
                    memdc, 
                    0,
                    0,
                    SRCCOPY);

            BitBlt( bufferdc, 
                    xIntroPos + FreeWidth + state->introOffset, 
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


#define descriptionY    26
#define valueY          25    
#define descriptionX    

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
                        descriptionY, 
                        smallFontWidth[volString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (volString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[volString[i] - 32];

            }

            wsprintf(valueString, "%d%%", state->volume);

            offset = displayRect.left + 3;

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        valueY, 
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

            int32 offset = displayRect.left + 1;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        descriptionY, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }

            if(state->hours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", state->hours,
                                                        state->minutes,
                                                        state->seconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   state->minutes,
                                                        state->seconds);

                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        valueY, 
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

            int32 offset = displayRect.left + 1;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        descriptionY, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }

            if(state->hours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", state->seekhours,
                                                        state->seekminutes,
                                                        state->seekseconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   state->seekminutes,
                                                        state->seekseconds);

                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        valueY, 
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

            int32 offset = displayRect.left + 1;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        descriptionY, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }
			int32 totalSeconds = state->totalhours * 3600;
			totalSeconds += state->totalminutes * 60;
			totalSeconds += state->totalseconds;
			int32 currSeconds = state->hours * 3600;
			currSeconds += state->minutes * 60;
			currSeconds += state->seconds;

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
                        valueY, 
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

            int32 offset = displayRect.left + 1;
    
            SelectObject(memdc, smallFont);

            for(i = 0; labelString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        descriptionY, 
                        smallFontWidth[labelString[i] - 32], 
                        10,
                        memdc, 
                        0,
                        (labelString[i] - 32)*10,
                        SRCCOPY);

                offset += smallFontWidth[labelString[i] - 32];

            }

            if(state->hours)
            {
                wsprintf(valueString, "%d:%.02d:%.02d", state->totalhours,
                                                        state->totalminutes,
                                                        state->totalseconds);
                offset = displayRect.left + 73;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   state->totalminutes,
                                                        state->totalseconds);

                 offset = displayRect.left + 83;
            }

            SelectObject(memdc, largeFont);

            for(i = 0; valueString[i]; i++)
            {
                BitBlt( bufferdc, 
                        offset, 
                        valueY, 
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

    if(state->state != Intro )
    {
        int32 offset = displayRect.left + 1;

        SelectObject(memdc, smallFont);

        for(i = 0; state->path[i]; i++)
        {
            BitBlt( bufferdc, 
                    offset - state->scrollOffset, 
                    10, 
                    smallFontWidth[state->path[i] - 32], 
                    10,
                    memdc, 
                    0,
                    (state->path[i] - 32)*10,
                    SRCCOPY);

            offset += smallFontWidth[state->path[i] - 32];
        }

    }

    SelectClipRgn(hdc, g_displayRegion);

    BitBlt( hdc, 
            0, 
            0, 
            PLAYER_WINDOW_WIDTH, 
            PLAYER_WINDOW_HEIGHT,
            bufferdc, 
            0,
            0,
            SRCCOPY);

    SelectClipRgn(hdc, NULL);

    SelectObject(bufferdc, oldBufferBitmap); 
    SelectObject(memdc, oldMemBitmap); 
    DeleteDC(bufferdc);
    DeleteDC(memdc);
    DeleteObject(bufferBitmap);
}

void UpdateControls(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);

    DrawPlayer(hdc, g_buttonStateArray, false);

    ReleaseDC(hwnd, hdc);
}

void UpdateDisplay(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);

    DrawDisplay(hdc, &g_displayInfo);

    ReleaseDC(hwnd, hdc);
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

static HRGN
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

    oldBitmap = (HBITMAP)SelectObject(hdc, bitmap);

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

static HRGN*
DetermineRegions(   HBITMAP bitmap, 
			        HBITMAP colors,
                    int32 numColors)
{
	HRGN* result = NULL;
    HRGN scanline = NULL;
    COLORREF* regionColors;// = RGB(color->r, color->g, color->b);
    HBITMAP oldBitmap;
    BITMAP bmp;
    HDC hdc;
    int32 width, height;
    int32 i;

    hdc = CreateCompatibleDC(NULL);

    oldBitmap = (HBITMAP)SelectObject(hdc, colors);

    regionColors = new COLORREF[numColors];

    for(i = 0; i < numColors; i++)
    {
        regionColors[i] = GetPixel(hdc, i, 0);
    }

    result = new HRGN[numColors];

    for(i = 0; i < numColors; i++)
    {
        // empty region
        result[i] = CreateRectRgn(0,0,0,0);
    }

    GetObject(bitmap, sizeof(BITMAP), (LPSTR)&bmp);
    width = bmp.bmWidth; 
    height = bmp.bmHeight;

    SelectObject(hdc, bitmap);

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

                    CombineRgn( result[scanIndex],
                                result[scanIndex],
                                scanline,
                                RGN_OR);
                }
            }
            else
            {
                if(pixel != black && pixel != white)
                {
                    for(i = 0; i < numColors; i++)
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
        }

        // last pixel in scanline is included in region...
        if(scanning)
        {
            scanline = CreateRectRgn(start, y, x, y + 1);

            CombineRgn( result[scanIndex],
                        result[scanIndex],
                        scanline,
                        RGN_OR);
        }
    }

    SelectObject(hdc, oldBitmap);
    DeleteDC(hdc);

    delete [] regionColors;

	return result;
}

LRESULT WINAPI MainWndProc( HWND hwnd, 
                            UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam )
{
    LRESULT result = FALSE;
    static POINT pressPt;
    static BOOL pressed = FALSE;
    static int32 pressedIndex = 0;
    static int32 seekSpeed = 0;
    static HWND hwndTooltip = NULL;
    

    switch( msg )
	{
		case WM_CREATE:
		{
            g_ui->SetHwnd(hwnd);

			DragAcceptFiles(hwnd, TRUE);

            InitCommonControls();

			HRGN bodyRegion, capRegion, playerRegion;
            int32 i;
            int32 xPos, yPos;
   
            // Create the region and apply it to the window

            // empty region to begin with...
            playerRegion = CreateRectRgn(0,0,0,0);
            g_playerRegion = CreateRectRgn(0,0,0,0);

            capRegion = CreateEllipticRgn(  0,
                                            0,
                                            CAP_CIRCUMFERENCE + 1,
                                            CAP_CIRCUMFERENCE + 1);

            bodyRegion = CreateRectRgn( 0,
                                        0,
                                        BODY_WIDTH,
                                        BODY_HEIGHT);

            OffsetRgn(bodyRegion, CAP_CIRCUMFERENCE/2, 0);

            CombineRgn( playerRegion,
                        capRegion,
                        bodyRegion,
                        RGN_OR);

            CombineRgn( g_playerRegion,
                        capRegion,
                        bodyRegion,
                        RGN_OR);

            OffsetRgn(capRegion, BODY_WIDTH, 0);

            CombineRgn( playerRegion,
                        playerRegion,
                        capRegion,
                        RGN_OR);
            CombineRgn( g_playerRegion,
                        g_playerRegion,
                        capRegion,
                        RGN_OR);
            
            SetWindowRgn( hwnd, playerRegion, TRUE );

            dialCursor = LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_DIAL));
            arrowCursor = LoadCursor(NULL, IDC_ARROW);
            //currentCursor = arrowCursor;

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
                
                repeatButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_REPEAT_256), &palette);
                shuffleButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_SHUFFLE_256), &palette);
                openButtonBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_OPEN_256), &palette);
                
                shuffledIconBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_SHUFFLED_ICON_256), &palette);
                repeatIconBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_REPEAT_ICON_256), &palette);
                allIconBitmap = LoadResourceBitmap(g_hInst, MAKEINTRESOURCE(IDB_REPEAT_ALL_ICON_256), &palette);
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

                repeatButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_REPEAT));
                shuffleButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SHUFFLE));
                openButtonBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_OPEN));

                shuffledIconBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SHUFFLED_ICON));
                repeatIconBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_REPEAT_ICON));
                allIconBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_REPEAT_ALL_ICON));

            }
            
            playerMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAYER_MASK));
            HBITMAP colors = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_COLORS));

            g_controlRegions = DetermineRegions(    playerMask,
                                                    colors,
                                                    kFinalControl);

            DeleteObject(colors);

            playerMaskDC = CreateCompatibleDC(NULL);

            oldPlayerMaskBitmap = (HBITMAP)SelectObject(playerMaskDC, playerMask);

            for(i = 0; i < kNumControls; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = TRUE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].shown = TRUE;
                g_buttonStateArray[i].state = Deactivated;
                g_buttonStateArray[i].region = g_controlRegions[i];
                    //DetermineRegion(playerMask, &(g_buttonColorArray[i]));

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
                else if(i == kModeControl)
                    g_buttonStateArray[i].bitmap = modeButtonBitmap;
                else if(i == kMinimizeControl)
                    g_buttonStateArray[i].bitmap = minimizeButtonBitmap;
                else if(i == kCloseControl)
                    g_buttonStateArray[i].bitmap = closeButtonBitmap;
                else if(i == kRepeatControl)
                    g_buttonStateArray[i].bitmap = repeatButtonBitmap;
                else if(i == kShuffleControl)
                    g_buttonStateArray[i].bitmap = shuffleButtonBitmap;
                else if(i == kOpenControl)
                    g_buttonStateArray[i].bitmap = openButtonBitmap;
                else if(i == kVolumeControl || i == kSeekControl )
                    g_buttonStateArray[i].bitmap = dialBitmap;

            } 

            // stop shown when play is pressed
            g_buttonStateArray[kStopControl].shown = FALSE;
            g_buttonStateArray[kStopControl].region = g_buttonStateArray[kPlayControl].region;

            // mode changing is currently disabled
            g_buttonStateArray[kModeControl].enabled = FALSE;

            // display
            int32 displayOffset = LEFT_SECTION + DIAL_SECTION + 19;

            g_displayRegion = g_controlRegions[kDisplayControl];

            // player
            //Color black = {0,0,0,0};
            //playerRegion = DetermineRegion(playerMask, &black);
            
            CombineRgn( g_playerRegion,
                        g_playerRegion,
                        g_displayRegion,
                        RGN_DIFF);
             
            g_displayInfo.state = Intro;
            strcpy(g_displayInfo.path, "Welcome to FreeAmp");
            g_displayInfo.frame = 0;
            g_displayInfo.range = 1200;
            g_displayInfo.hours = 0;
            g_displayInfo.minutes = 0;
            g_displayInfo.seconds = 0;
            g_displayInfo.shuffled = false;
            g_displayInfo.repeat = false;
            g_displayInfo.repeatAll = false;


            waveOutGetVolume((HWAVEOUT)WAVE_MAPPER, (DWORD*)&g_displayInfo.volume);

            g_displayInfo.volume = (int32)(100 * ((float)LOWORD(g_displayInfo.volume)/(float)0xffff));

            // tooltip support
            hwndTooltip = CreateWindowEx(WS_EX_TOPMOST,
                                        TOOLTIPS_CLASS, 
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
                ti.uFlags =  TTF_SUBCLASS; 
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

            // don't forget display area!
            GetRgnBox(g_displayRegion, &toolRect);
            ti.cbSize = sizeof(TOOLINFO); 
            ti.uFlags =  TTF_SUBCLASS; 
            ti.hwnd = hwnd; 
            ti.hinst = g_hInst; 
            ti.uId = (UINT) kDisplayControl; 
            ti.lpszText = (LPSTR) LPSTR_TEXTCALLBACK; 
            ti.rect.left = toolRect.left; 
            ti.rect.top = toolRect.top; 
            ti.rect.right = toolRect.right; 
            ti.rect.bottom = toolRect.bottom; 

            SendMessage(hwndTooltip, 
                        TTM_ADDTOOL, 
                        0, 
                        (LPARAM) &ti);

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
            //SetTimer(hwnd, 0x01, 200, NULL);
			break;
		}

        case WM_TIMER:
        {
            
            switch (wParam)
            {
                case 0:
                {
                    static int32 count = 0;
                    HDC hdc = GetDC(hwnd);

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

                    DrawDisplay(hdc, &g_displayInfo);

                    ReleaseDC(hwnd, hdc);

                    break;
                }

                case 1:
                {
                    int32 length = 0;
                    int32 i;
                    RECT rect;
                    static int32 increment = 1;
                    HDC hdc = GetDC(hwnd);

                    GetRgnBox(g_displayRegion, &rect);

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

                    DrawDisplay(hdc, &g_displayInfo);

                    ReleaseDC(hwnd, hdc);

                    break;
                }

                case 2:
                {
                    //char foo[1024];
                    HDC hdc = GetDC(hwnd);

                    g_displayInfo.seekframe += seekSpeed*20*abs(seekSpeed);

                    if(g_displayInfo.seekframe < 0)
                        g_displayInfo.seekframe = 0;
                    else if(g_displayInfo.seekframe > g_displayInfo.range)
                        g_displayInfo.seekframe = g_displayInfo.range;

                    int32 position = g_displayInfo.seekframe;

                    //sprintf(foo,"position: %d\r\n",position);
				    //OutputDebugString(foo);

                    int32 seconds = (int32)ceil(g_ui->m_secondsPerFrame * position);
					int32 hours = seconds / 3600;
					int32 minutes = seconds / 60 - hours * 60;
					seconds = seconds - minutes * 60 - hours * 3600;

                    //sprintf(foo,"seconds: %d\r\n",seconds);

                    g_displayInfo.seekhours = hours;
                    g_displayInfo.seekminutes = minutes;
                    g_displayInfo.seekseconds = seconds;

                    DrawDisplay(hdc, &g_displayInfo);

                    ReleaseDC(hwnd, hdc);

                    break;
                }
            } 

            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc;
            
            hdc = BeginPaint( hwnd, &ps );

            DrawPlayer(hdc, g_buttonStateArray, true);
            DrawDisplay(hdc, &g_displayInfo);
            
            EndPaint( hwnd, &ps );          
            break;
        }

        case WM_DESTROY:
        {
            SelectObject(playerMaskDC, oldPlayerMaskBitmap); 

            DeleteDC(playerMaskDC);

            DeleteObject(leftBitmap);
            DeleteObject(middleBitmap);
            DeleteObject(rightBitmap);
            DeleteObject(dialBitmap);
            DeleteObject(playButtonBitmap);
            DeleteObject(stopButtonBitmap);
            DeleteObject(pauseButtonBitmap);
            DeleteObject(nextButtonBitmap);
            DeleteObject(lastButtonBitmap);
            DeleteObject(modeButtonBitmap);
            DeleteObject(minimizeButtonBitmap);
            DeleteObject(closeButtonBitmap);
            DeleteObject(repeatButtonBitmap);
            DeleteObject(shuffleButtonBitmap);
            DeleteObject(openButtonBitmap);
            DeleteObject(playerMask);
            DeleteObject(smallFont); 
            DeleteObject(largeFont);
            DeleteObject(logoBitmap);

            DeleteObject(g_displayRegion);

            DeleteObject(palette);

            if(g_controlRegions)
            {
                for(int32 i = 0; i < kFinalControl; i++)
                    DeleteObject(g_controlRegions[i]);

                delete [] g_controlRegions;
            }

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
                //for(i = 0; i < kNumControls; i++)
                //{

                i = pressedIndex;

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

                //}

                // check on display area
                if( PtInRegion(g_displayRegion, xPos, yPos) )
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

            DrawPlayer(hdc, g_buttonStateArray, false);
            DrawDisplay(hdc, &g_displayInfo);

            ReleaseDC(hwnd, hdc);

            pressed = FALSE;
            ReleaseCapture();

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
                if( PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) &&
                    g_buttonStateArray[i].shown &&
                    g_buttonStateArray[i].enabled)
                {
                    pressed = TRUE;
                    pressedIndex = i;
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

                    DrawPlayer(hdc, g_buttonStateArray, false);

                    ReleaseDC(hwnd, hdc);

                }
            } 

            // check on display area
            if( PtInRegion(g_displayRegion, pressPt.x, pressPt.y) )
            {
                pressed = TRUE;
                SetCapture(hwnd);
            }

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
            bool dirtyPlayer = false;
            bool dirtyDisplay = false;
            
            int32 i;

            pt.x = xPos;
            pt.y = yPos;

            if(WM_NCHITTEST == msg)
                ScreenToClient(hwnd, &pt);

            /*vErrorOut(bg_blue|fg_pink, "yPos = %d\r\n", pt.y);*/

            color = GetPixel(playerMaskDC, pt.x, pt.y);

            if(color == RGB(0,0,0) && !pressed)
            //if(PtInRegion(playerRegion, pt.x, pt.y))
            {
                result = HTCAPTION;
            }
            else
            {
                result = HTCLIENT;
            }

            currentCursor = arrowCursor;

            if(pressed)
            {
                i = pressedIndex;

                if(i == kVolumeControl || i == kSeekControl )
                {
                    currentCursor = dialCursor;

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

                        SendMessage(hwnd, WM_COMMAND, kVolumeControl, change);
                    }
                    else if(i == kSeekControl &&
						PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y))
                    {
                        SendMessage(hwnd, WM_COMMAND, kSeekControl, delta);
                    }

                    g_buttonStateArray[i].dirty = TRUE;
                    dirtyPlayer = true;
                    dirtyDisplay = true;

                }
                else
                {
                    if( PtInRegion(g_buttonStateArray[i].region, pt.x, pt.y) &&
                        PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) )
                    {
                        g_buttonStateArray[i].state = Pressed;
                        g_buttonStateArray[i].dirty = TRUE;
                        dirtyPlayer = true;
                    }
                    else if( PtInRegion(g_buttonStateArray[i].region, pressPt.x, pressPt.y) )
                    {
                        g_buttonStateArray[i].state = g_buttonStateArray[i].oldstate;
                        g_buttonStateArray[i].dirty = TRUE;
                        dirtyPlayer = true;
                    }
                }
            }
            else
            {
                for(i = 0; i < kNumControls; i++)
                {
                    if( PtInRegion(g_buttonStateArray[i].region, pt.x, pt.y) &&
                        g_buttonStateArray[i].state != Activated &&
                        g_buttonStateArray[i].enabled)
                    {
                        if(g_buttonStateArray[i].state != Selected )
                        {
                            g_buttonStateArray[i].state = Selected;
                            g_buttonStateArray[i].dirty = TRUE;
                            dirtyPlayer = true;
                        }

                        if(i == kVolumeControl || i == kSeekControl )
                            currentCursor = dialCursor;
                    }
                    else if(g_buttonStateArray[i].state == Selected)
                    {
                        g_buttonStateArray[i].state = Deactivated;
                        g_buttonStateArray[i].dirty = TRUE;
                        dirtyPlayer = true;
                    }
                }
            }

            if(dirtyPlayer || dirtyDisplay)
            {
                HDC hdc = GetDC(hwnd);

                if(dirtyDisplay)
                    DrawDisplay(hdc, &g_displayInfo);

                if(dirtyPlayer)
                    DrawPlayer(hdc, g_buttonStateArray, false);

                ReleaseDC(hwnd, hdc);
            }
           
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
                    case kRepeatControl:
                        lpttt->lpszText = "Repeat"; 
                        break;
                    case kShuffleControl:
                        lpttt->lpszText = "Shuffle"; 
                        break;
                    case kOpenControl:
                        lpttt->lpszText = "Open..."; 
                        break;
                    case kModeControl:
                        lpttt->lpszText = "Change Player Mode (disabled)"; 
                        break;
                    case kMinimizeControl:
                        lpttt->lpszText = "Minimize"; 
                        break;
                    case kCloseControl:
                        lpttt->lpszText = "Close"; 
                        break;
                    case kPlayControl:
                        if(g_ui->m_state == STATE_Stopped)
                            lpttt->lpszText = "Play"; 
                        else
                            lpttt->lpszText = "Stop";
                        break;
                    case kPauseControl:
                        if(g_ui->m_state == STATE_Paused)
                            lpttt->lpszText = "Continue Playing"; 
                        else
                            lpttt->lpszText = "Pause"; 
                        break;
                    case kNextControl:
                        lpttt->lpszText = "Next Song"; 
                        break;
                    case kLastControl:
                        lpttt->lpszText = "Previous Song"; 
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

			g_ui->m_plm->RemoveAll();
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
				//g_ui->m_target->AcceptEvent(new Event(CMD_Play));
                //SendMessage(hwnd, WM_COMMAND, kPlayControl,0);
			}
			break;
		}

		case WM_COMMAND:
		{
			switch(wParam)
	        {
                case kRepeatControl:
                {
					//OutputDebugString("Repeat Control\n");
					g_ui->m_plm->ToggleRepeat();
			        break;        
		        }

                case kShuffleControl:
                {
					g_ui->m_plm->ToggleShuffle();
			        break;        
		        }

		        case kOpenControl:
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
						g_ui->m_plm->RemoveAll();
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

                    g_buttonStateArray[kOpenControl].state = Deactivated;

			        break;        
		        }

                case kModeControl:
                {
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

		        case kLastControl:
		        {
					if ((g_displayInfo.seconds < 2) &&
						(g_displayInfo.hours == 0) &&
						(g_displayInfo.minutes == 0)) {
						if (g_displayInfo.indexOfSong > 1) {
							g_ui->m_target->AcceptEvent(new Event(CMD_PrevMediaPiece));
						} else {
							if (g_ui->m_plm->GetRepeat() != REPEAT_NOT) {
								g_ui->m_target->AcceptEvent(new Event(CMD_PrevMediaPiece));
							} else {
								if (g_buttonStateArray[kPlayControl].shown == FALSE) {
									g_ui->m_target->AcceptEvent(new Event(CMD_Play));
								}
							}
						}
					} else {
						g_ui->m_target->AcceptEvent(new Event(CMD_Play));
					}
					g_displayInfo.seconds = 0;
					g_displayInfo.minutes = 0;
					g_displayInfo.hours = 0;
			        break;        
		        }

		        case kNextControl:
		        {
					if ((g_displayInfo.indexOfSong != g_displayInfo.totalSongs)) {
						g_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
						g_displayInfo.seconds = 0;
						g_displayInfo.minutes = 0;
						g_displayInfo.hours = 0;
					} else if ((g_ui->m_plm->GetRepeat() != REPEAT_NOT)) {
						g_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
						g_displayInfo.seconds = 0;
						g_displayInfo.minutes = 0;
						g_displayInfo.hours = 0;
					}
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