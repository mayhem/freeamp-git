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


extern FreeAmpUI *g_ui;


#ifdef __cplusplus
extern "C" {
#endif


LRESULT WINAPI MainWndProc( HWND, UINT, WPARAM, LPARAM );
void ImmediateProcessMessage(HWND hWnd,UINT Msg, WPARAM wParam,LPARAM lParam);

extern HINSTANCE g_hInst;		//  Program instance handle


ControlInfo g_buttonStateArray[kNumControls];
DisplayInfo g_displayInfo;

static HBITMAP playerBitmap;
static HBITMAP leftBitmap;
static HBITMAP middleBitmap;
static HBITMAP rightBitmap;
static HBITMAP dialBitmap;
static HBITMAP playerButtonsBitmap,systemButtonsBitmap;
static HRGN closeRegion, displayRegion;
static HBITMAP playerMask;
static HBITMAP smallFont, largeFont;
static HDC tempDC;
static HCURSOR dialCursor, arrowCursor, currentCursor;
static HBITMAP logoBitmap;

void DrawPlayer(HDC hdc, ControlInfo* state)
{
    HDC memdc, bufferdc;
    HBITMAP oldMemBitmap, bufferBitmap, oldBufferBitmap;
    int width, height;
    BITMAP bmp;
    int32 i;


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
#define yIntroPos   16
#define xIntroPos   200
#define LogoHeight  18
#define FreeWidth   41
#define AmpWidth    45

    SelectClipRgn(bufferdc, displayRegion);

    switch(g_displayInfo.state)
    {
        case Intro: // hard coded hack just to look good <grin>
        {
            static int32 count = 0;
            static double multiplier = 0;

            int32 offset = (int32)(multiplier * count++);

            multiplier += .55;
            SelectObject(memdc, logoBitmap);
            
                                
            BitBlt( bufferdc, 
                    xIntroPos - offset, 
                    yIntroPos, 
                    FreeWidth, 
                    LogoHeight, 
                    memdc, 
                    0,
                    0,
                    SRCCOPY);

            BitBlt( bufferdc, 
                    xIntroPos + FreeWidth + offset, 
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

            int32 offset = 227;
    
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

            offset = 180 ;

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

            int32 offset = 180;
    
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
                offset = 250;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   g_displayInfo.minutes,
                                                        g_displayInfo.seconds);

                 offset = 260;
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

            int32 offset = 180;
    
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
                offset = 250;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   g_displayInfo.seekminutes,
                                                        g_displayInfo.seekseconds);

                 offset = 260;
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

            int32 offset = 180;
    
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
                offset = 250;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d", displayMinutes, displaySeconds);
				//OutputDebugString(valueString);
				//OutputDebugString("\n");
                 offset = 260;
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

            int32 offset = 180;
    
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
                offset = 250;
            }
            else
            {
                 wsprintf(valueString, "%.02d:%.02d",   g_displayInfo.totalminutes,
                                                        g_displayInfo.totalseconds);

                 offset = 260;
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
        int32 offset = 180;

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
                    RECT srcRect;

                    SelectObject(memdc, systemButtonsBitmap);
                    
                    CopyRect(&srcRect, &rect);

                    OffsetRect(&srcRect, -354, -18);

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
                            srcRect.left,
                            srcOffset,
                            SRCCOPY);

                    currentCursor = arrowCursor;
                    break;        
                }

                case kPlayControl:
                case kStopControl:
                case kNextControl:
                case kLastControl:
                {
                    int32 srcOffset = 0;
                    RECT srcRect;

                    SelectObject(memdc, playerButtonsBitmap);

                    CopyRect(&srcRect, &rect);

                    OffsetRect(&srcRect, -39, -15);

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

                    if(srcOffset)
                    BitBlt(bufferdc, 
                        rect.left, 
                        rect.top, 
                        rect.right - rect.left, 
                        rect.bottom - rect.top, 
                        memdc, 
                        srcRect.left,
                        srcOffset,
                        SRCCOPY);

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



LRESULT WINAPI MainWndProc( HWND hwnd, 
                            UINT msg, 
                            WPARAM wParam, 
                            LPARAM lParam )
{
    LRESULT result = FALSE;
    static POINT pressPt;
    static BOOL pressed = FALSE;
    

    switch( msg )
	{
		case WM_CREATE:
		{
			DragAcceptFiles(hwnd, TRUE);

			HRGN bodyRegion, capRegion, playerRegion, tempRegion;
            int32 i;
            int32 xPos, yPos;
   
            //  Create the region and apply it to the window

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

            //playerBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAYER));
            logoBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LOGO));
            leftBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_LEFT));
            middleBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MID));
            rightBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_RIGHT));
            dialBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DIALS));
            playerButtonsBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAYER_BUTTONS));
            systemButtonsBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SYSTEM_BUTTONS));
            playerMask = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_PLAYER_MASK));

            smallFont = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SMALL_FONT));
            largeFont = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIG_FONT));

            tempDC = CreateCompatibleDC(NULL);

            closeRegion = CreateEllipticRgn(0,0,14,14);

            OffsetRgn(closeRegion, PLAYER_WINDOW_WIDTH - 42, 18);

            /*HRGN          region;
            ControlState    state;
            BOOL            enabled;
            BOOL            dirty;
            HBITMAP         bitmap;*/

            for(i = 0; i < 4; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = FALSE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].state = Deactivated;
                g_buttonStateArray[i].bitmap = playerButtonsBitmap;
            } 

            for(i = 5; i < 8; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = FALSE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].state = Deactivated;
                g_buttonStateArray[i].bitmap = systemButtonsBitmap;
            } 

            for(i = 8; i < 10; i++)
            {
                g_buttonStateArray[i].control_id = i;
                g_buttonStateArray[i].dirty = FALSE;
                g_buttonStateArray[i].enabled = TRUE;
                g_buttonStateArray[i].state = Deactivated;
                g_buttonStateArray[i].bitmap = dialBitmap;
            } 

            tempRegion = CreateEllipticRgn( 39,15,60,36);
            g_buttonStateArray[0].region = tempRegion;
            tempRegion = CreateEllipticRgn( 66,15,86,36);
            g_buttonStateArray[1].region = tempRegion;
            tempRegion = CreateEllipticRgn( 92,15,112,36);
            g_buttonStateArray[2].region = tempRegion;
            tempRegion = CreateEllipticRgn( 119,15,139,36);
            g_buttonStateArray[3].region = tempRegion;

            tempRegion = CreateEllipticRgn( 354,18,367,31);
            g_buttonStateArray[5].region = tempRegion;
            tempRegion = CreateEllipticRgn( 370,18,383,31);
            g_buttonStateArray[6].region = tempRegion;
            tempRegion = CreateEllipticRgn( 386,18,399,31);
            g_buttonStateArray[7].region = tempRegion;

            tempRegion = CreateRectRgn( 148,0,158,50);
            g_buttonStateArray[8].region = tempRegion;
            tempRegion = CreateRectRgn( 334,0,344,50);
            g_buttonStateArray[9].region = tempRegion;

            displayRegion = CreateRectRgn( 177,11,316,38);

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
            static int32 count = 0;
            HDC hdc = GetDC(hwnd);

            switch (wParam)
            {
                case 0:
                {
                    if(count++ == INTRO_COUNT)
                    {
                        KillTimer(hwnd, 0x00);
                        g_displayInfo.state = TotalTime;
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

            //if(PtInRegion(closeRegion, xPos, yPos))
               // DestroyWindow( hwnd );

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
                        }
                    }
                    else
                    {
                        if( PtInRegion(g_buttonStateArray[i].region, xPos, yPos) &&
                            g_buttonStateArray[i].state != Activated)
                        {
                            g_buttonStateArray[i].state = Selected;
                            g_buttonStateArray[i].dirty = TRUE;
							ImmediateProcessMessage(hwnd,WM_COMMAND,g_buttonStateArray[i].control_id,0);
                            //PostMessage(hwnd, WM_COMMAND, g_buttonStateArray[i].control_id, 0);
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
                
            break;
        }

        /*case WM_MOUSEMOVE:
        {   
            WPARAM fwKeys = wParam;        // key flags 
            LPARAM xPos = LOWORD(lParam);  // horizontal position of cursor 
            LPARAM yPos = HIWORD(lParam);  // vertical position of cursor 
      
            break;
        }*/
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

				if (!g_ui->m_playList) {
					g_ui->m_playList = new PlayList();
					g_ui->m_playList->Add(szFile,0);
					g_ui->m_playList->SetFirst();
					g_ui->m_target->AcceptEvent(new SetPlayListEvent(g_ui->m_playList));
					g_ui->m_target->AcceptEvent(new Event(CMD_Play));
				} else {
					if (g_ui->m_playList->GetCurrent() == NULL) {
						g_ui->m_playList->Add(szFile,0);
						g_ui->m_target->AcceptEvent(new SetPlayListEvent(g_ui->m_playList));
						// first in playlist...
						g_ui->m_target->AcceptEvent(new Event(CMD_Play));
					} else {
						g_ui->m_playList->Add(szFile,0);
						g_ui->m_target->AcceptEvent(new SetPlayListEvent(g_ui->m_playList));
					}
				}
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
                result = HTCLIENT;            

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

                            //vErrorOut(fg_pink, "delta = %d\r\n", lParam);

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
							char foo[1024];
							sprintf(foo,"position: %d\n",pt.y);
							OutputDebugString(foo);
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

		case WM_COMMAND:
		{
			ImmediateProcessMessage(hwnd,WM_COMMAND,wParam,lParam);

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

void ImmediateProcessMessage(HWND hwnd,UINT Msg, WPARAM wParam,LPARAM lParam) {
	switch(wParam)
	{
		case kModeControl:
		{
			break;        
		}

		case kMinimizeControl:
		{
			break;        
		}

		case kCloseControl:
		{
			DestroyWindow( hwnd );
			break;        
		}

		case kPlayControl:
		{
			OutputDebugString("play clicked\n");
			if (g_ui->m_state == STATE_Stopped) {
				g_ui->m_target->AcceptEvent(new Event(CMD_Play));
			} else if (g_ui->m_state == STATE_Paused) {
				g_ui->m_target->AcceptEvent(new Event(CMD_TogglePause));
			} else if (g_ui->m_state == STATE_Playing) {
				g_ui->m_target->AcceptEvent(new Event(CMD_TogglePause));
			}
			break;        
		}
		case kStopControl:
		{
			OutputDebugString("stop clicked\n");
			g_ui->m_target->AcceptEvent(new Event(CMD_Stop));
			break;        
		}

		case kNextControl:
		{
			OutputDebugString("next clicked\n");
			g_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
			break;        
		}

		case kLastControl:
		{
			OutputDebugString("prev clicked\n");
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
			break;
		}
	}
}


#ifdef __cplusplus
}
#endif