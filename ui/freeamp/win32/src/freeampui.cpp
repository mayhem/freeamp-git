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
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <math.h>
#include <mmsystem.h >


/* project headers */
#include "config.h"
#include "thread.h"
#include "freeampui.h"
#include "event.h"
#include "eventdata.h"
#include "playlist.h"
#include "resource.h"
#include "font_width.h"

extern "C"
{

#include "drawplayer.h"

HINSTANCE g_hInst;		//  Program instance handle
extern ControlInfo g_buttonStateArray[kNumControls];
extern DisplayInfo g_displayInfo;

}



extern "C" FreeAmpUI *Initialize() 
{
    return new FreeAmpUI();
}


INT WINAPI DllMain (HINSTANCE hInst,
                    ULONG ul_reason_being_called,
	                LPVOID lpReserved)
{
	switch (ul_reason_being_called)
	{
		case DLL_PROCESS_ATTACH:
			g_hInst = hInst;
	    	break;

		case DLL_THREAD_ATTACH:
		    break;

		case DLL_THREAD_DETACH:
		    break;

		case DLL_PROCESS_DETACH:
		    break;
	}

    return 1;                 
}

FreeAmpUI *g_ui;

FreeAmpUI::
FreeAmpUI():
UserInterface()
{
    m_scrolling = false;

    m_uiSemaphore = new Semaphore();

    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(UIThreadFunc,this);

    m_uiSemaphore->Wait();

	g_ui = this;
	m_state = STATE_Stopped;
}

FreeAmpUI::
~FreeAmpUI()
{
    delete m_uiSemaphore;
}

void 
FreeAmpUI::
SetTarget(EventQueue* eq)
{
    m_target = eq;
}

int32 
FreeAmpUI::
AcceptEvent(Event* event)
{
    int32 result = 255;

    if (event) 
    {
        
        switch (event->Type()) 
        {
            case INFO_Playing: 
            {   
                EnableWindow(m_hwndPlay, FALSE);
				EnableWindow(m_hwndNext, TRUE);
				EnableWindow(m_hwndLast, TRUE);

				EnableWindow(m_hwndStop, TRUE);
				EnableWindow(m_hwndPause, TRUE);
				EnableWindow(m_hwndSlider, TRUE);
				m_state = STATE_Playing;
	            break; 
            }

            case INFO_Paused: 
            {
                EnableWindow(m_hwndPlay, TRUE);
				EnableWindow(m_hwndNext, FALSE);
				EnableWindow(m_hwndLast, FALSE);  

				EnableWindow(m_hwndStop, TRUE);
				EnableWindow(m_hwndPause, FALSE);
				EnableWindow(m_hwndSlider, TRUE);
				m_state = STATE_Paused;
	            break; 
            }

            case INFO_Stopped: 
            {
                EnableWindow(m_hwndPlay, TRUE);
				EnableWindow(m_hwndNext, TRUE);
				EnableWindow(m_hwndLast, TRUE);

				EnableWindow(m_hwndStop, FALSE);
				EnableWindow(m_hwndPause, FALSE);
				
                /*SendMessage(m_hwndSlider,
						    TBM_SETPOS,
						    (WPARAM)TRUE,
						    (LPARAM)0);*/

                EnableWindow(m_hwndSlider, FALSE);

                char timeString[256] = "00:00:00";

                SetWindowText(m_hwndCurrent, timeString);
				m_state = STATE_Stopped;
	            break; 
            }
			case INFO_PlayList: 
				{
					PlayListEvent *info = (PlayListEvent *)event;
					m_playList = info->GetPlayList();
					break;
				}
			case INFO_MPEGInfo: 
				{
					MpegInfoEvent *info = (MpegInfoEvent *)event;

					g_displayInfo.range = info->GetTotalFrames();

#if 0
					char szTemp[256];
					m_secondsPerFrame = info->GetSecondsPerFrame();
					sprintf(szTemp, "%d kbps",  info->GetBitRate()/1000);
					SendMessage(m_hwndStatus, 
								SB_SETTEXT, 
								0, 
								(LPARAM)szTemp);

					sprintf(szTemp, "%.1f kHz",  ((float)info->GetSampleRate())/1000);
					SendMessage(m_hwndStatus, 
								SB_SETTEXT, 
								1, 
								(LPARAM) szTemp);
					SendMessage(m_hwndSlider,
								TBM_SETRANGE,
								(WPARAM)TRUE,
								MAKELPARAM(0, info->GetTotalFrames()));
#endif					
					break;
				}
            case INFO_MediaInfo: 
            {
                MediaInfoEvent *info = (MediaInfoEvent*)event;
				int32 seconds = (int32)ceil(info->m_totalSeconds);
				int32 hours = seconds / 3600;
				int32 minutes = (seconds - (hours * 3600)) / 60;
				seconds = seconds - (hours * 3600) - (minutes * 60);

                strcpy(g_displayInfo.path,info->m_filename);
				g_displayInfo.totalhours = hours;
				g_displayInfo.totalminutes = minutes;
				g_displayInfo.totalseconds = seconds;
	            break; 
            }

            case INFO_MediaTimeInfo: 
            {
                MediaTimeInfoEvent* info = (MediaTimeInfoEvent*)event;
 
				int32 seconds = (int32)ceil(info->m_totalSeconds);
				int32 hours = seconds / 3600;
				int32 minutes = (seconds - (hours * 3600)) / 60;
				seconds = seconds - (hours * 3600) - (minutes * 60);
				
				g_displayInfo.hours = hours;
				g_displayInfo.minutes = minutes;
				g_displayInfo.seconds = seconds;
				g_displayInfo.frame = info->m_frame;
#if 0				
				char timeString[256] = "00:00:00";
                static int32 lastSeconds = 0, lastMinutes = 0, lastHours = 0;
	            
                if(lastSeconds != info->m_seconds ||
                    lastMinutes != info->m_minutes ||
                    lastHours != info->m_hours)
                {
                    lastSeconds = info->m_seconds;
                    lastMinutes = info->m_minutes;
                    lastHours = info->m_hours;

                    sprintf(timeString,"%02d:%02d:%02d",info->m_hours,
				                                    info->m_minutes,
				                                    info->m_seconds);

			        SetWindowText(m_hwndCurrent, timeString);
                }

                if(!m_scrolling)
                {

                    SendMessage(m_hwndSlider,
						        TBM_SETPOS,
						        (WPARAM)TRUE,
						        (LPARAM)info->m_frame);

                }
#endif
	            break; 
            }

            
            case INFO_DoneOutputting: 
            {
	            break; 
            }

            case INFO_PlayListDonePlay:
            {
                char timeString[256] = "00:00:00";
                char szTemp[256] = {0x00};

                SetWindowText(m_hwndCurrent, timeString);
			    SetWindowText(m_hwndTotal, timeString);


                SendMessage(m_hwndStatus, 
						    SB_SETTEXT, 
						    0, 
						    (LPARAM)szTemp);

			    SendMessage(m_hwndStatus, 
						    SB_SETTEXT, 
						    1, 
						    (LPARAM) szTemp);

			    SendMessage(m_hwndStatus, 
						    SB_SETTEXT, 
						    2, 
						    (LPARAM) szTemp);

             
                SetWindowText(m_hwnd, "FreeAmp");
                break;
            }

	        case CMD_Cleanup: 
            {
	            m_target->AcceptEvent(new Event(INFO_ReadyToDieUI));
	            break; 
            }

	        default:
	            break;
	    }

	    result = 0;

    } 

    return result;
}

void  
FreeAmpUI::
SetArgs(int32 argc, char** argv)
{
    PlayList* playlist = new PlayList;
    char *arg = NULL;
    bool shuffle = false;
    bool autoplay = false;
    int32 count = 0;

    for(int32 i = 1;i < argc; i++) 
    {
	    arg = argv[i];

	    if (arg[0] == '-') 
        {
	        switch (arg[1]) 
            {
		        case 's':
                {
                    shuffle = true;
		            break;
	            } 

                case 'p':
                {
                    autoplay = true;
		            break;
	            } 
            }
        }
        else 
        {
            playlist->Add(arg,0);
            count++;
	    }
    }

    playlist->SetFirst();

    if(shuffle) 
        playlist->SetOrder(PlayList::ORDER_SHUFFLED);
    
    m_target->AcceptEvent(new SetPlayListEvent(playlist));


    if(count)
    {
        EnableWindow(m_hwndPlay, TRUE);

        if(count > 1)
			EnableWindow(m_hwndNext, TRUE);
    }
    //if(autoplay)
       //m_target->AcceptEvent(m_target, new Event(CMD_Play));
}

void
FreeAmpUI::
CreateUI()
{
    WNDCLASS wc;
    MSG msg;
    HWND hwnd;
    	
    wc.style = 0;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInst;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hbrBackground = (HBRUSH)NULL;//( COLOR_WINDOW + 1 );
    wc.lpszMenuName = (LPSTR)0;
    wc.lpszClassName = "nonrect";

    if( RegisterClass( &wc ) )    
    {
        hwnd = CreateWindow("nonrect", 
                            "FreeAmp",
                            WS_POPUP | WS_VISIBLE | WS_SYSMENU, 
                            0, 
                            0, 
                            PLAYER_WINDOW_WIDTH, 
                            PLAYER_WINDOW_HEIGHT,
                            NULL, 
                            NULL, 
                            g_hInst, 
                            NULL );

        if( hwnd )
        {
      
            ShowWindow( hwnd, SW_NORMAL );
            UpdateWindow( hwnd );

            m_uiSemaphore->Signal();

            while( GetMessage( &msg, NULL, 0, 0 ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
    }

    m_target->AcceptEvent(new Event(CMD_QuitPlayer));
}

void 
FreeAmpUI::
UIThreadFunc(void* arg)
{
    FreeAmpUI* ui = (FreeAmpUI*)arg;

    ui->CreateUI();
}
