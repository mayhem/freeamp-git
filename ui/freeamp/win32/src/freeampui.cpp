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

FreeAmpUI *g_ui;


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

FreeAmpUI::
FreeAmpUI():
UserInterface()
{
    g_ui = this;

    m_scrolling = false;

    m_uiSemaphore = new Semaphore();

    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(UIThreadFunc,this);

    m_uiSemaphore->Wait();

	m_state = STATE_Stopped;

    g_displayInfo.hours = 0;
	g_displayInfo.minutes = 0;
	g_displayInfo.seconds = 0;
	g_displayInfo.frame = 0;
    m_secondsPerFrame = 0;
	m_plm = NULL;
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

void 
FreeAmpUI::
SetHwnd(HWND hwnd)
{
    m_hwnd = hwnd;
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
		    case INFO_PlayListRepeat:
			{
				PlayListRepeatEvent *plre = (PlayListRepeatEvent *)event;
				switch (plre->GetRepeatMode()) 
                {
				    case REPEAT_CURRENT:
					    g_displayInfo.repeat = true;
					    g_displayInfo.repeatAll = false;
					    break;
				    case REPEAT_ALL:
					    g_displayInfo.repeat = true;
					    g_displayInfo.repeatAll = true;
					    break;
				    case REPEAT_NOT:
				    default:
					    g_displayInfo.repeat = false;
					    g_displayInfo.repeatAll = false;
					    break;
				}

                UpdateControls(m_hwnd);
				break;
			}

		    case INFO_PlayListShuffle:
			{
				PlayListShuffleEvent *plse = (PlayListShuffleEvent *)event;
                
				switch (plse->GetShuffleMode()) 
                {
				    case SHUFFLE_NOT_SHUFFLED:
					    g_displayInfo.shuffled = false;
					    break;
				    case SHUFFLE_SHUFFLED:
					    g_displayInfo.shuffled = true;
					    break;
				    default:
					    g_displayInfo.shuffled = false;
				};

                UpdateControls(m_hwnd);
				break;
			}

            case INFO_Playing: 
            {   
				m_state = STATE_Playing;
                g_buttonStateArray[kPlayControl].shown = FALSE;
                g_buttonStateArray[kStopControl].shown = TRUE;
                g_buttonStateArray[kStopControl].dirty = TRUE;
                g_buttonStateArray[kPauseControl].state = Deactivated;

                if(g_displayInfo.state == TotalTime)
                    g_displayInfo.state = CurrentTime;

                UpdateControls(m_hwnd);

	            break; 
            }

            case INFO_Paused: 
            {
				m_state = STATE_Paused;
                g_buttonStateArray[kPauseControl].state = Activated;
                UpdateControls(m_hwnd);
	            break; 
            }

            case INFO_Stopped: 
            {
                g_displayInfo.hours = 0;
				g_displayInfo.minutes = 0;
				g_displayInfo.seconds = 0;
				g_displayInfo.frame = 0;

                g_displayInfo.state = TotalTime;

				m_state = STATE_Stopped;
                g_buttonStateArray[kPauseControl].state = Deactivated;
                g_buttonStateArray[kPlayControl].shown = TRUE;
                g_buttonStateArray[kStopControl].shown = FALSE;
                UpdateControls(m_hwnd);
	            break; 
            }


			case INFO_MPEGInfo: 
			{
				MpegInfoEvent *info = (MpegInfoEvent *)event;

				g_displayInfo.range = info->GetTotalFrames();

                m_secondsPerFrame = info->GetSecondsPerFrame();

#if 0
				char szTemp[256];
				
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

			case INFO_ID3TagInfo:
			{
				ID3TagEvent *info = (ID3TagEvent *)event;

				if (info->GetId3Tag().m_containsInfo) 
                {
					char foo[1024];
					strncpy(foo,info->GetId3Tag().m_artist,sizeof(foo)-1);
					//kill trailing spaces
					char *pFoo = &(foo[strlen(foo)-1]);

					while ((pFoo >= foo) && pFoo && (*pFoo == ' ')) 
                    {
						*pFoo = '\0';
						pFoo--;
					}

					strncat(foo," - ",sizeof(foo)-strlen(foo));

					strncat(foo,info->GetId3Tag().m_songName,sizeof(foo)-strlen(foo));
					// kill trailing spaces
					pFoo = &(foo[strlen(foo)-1]);

					while ((pFoo >= foo) && pFoo && (*pFoo == ' ')) 
                    {
						*pFoo = '\0';
						pFoo--;
					}

					strncpy(g_displayInfo.path,foo,sizeof(g_displayInfo.path)-1);
				}

                if(NeedToScroll())
                    SetTimer(m_hwnd, 0x01, 200, NULL);
                else
                    KillTimer(m_hwnd, 0x01);
                

                UpdateDisplay(m_hwnd);
				break;
			}

            case INFO_MediaInfo: 
            {
                MediaInfoEvent *info = (MediaInfoEvent*)event;
				int32 seconds = (int32)ceil(info->m_totalSeconds);
				int32 hours = seconds / 3600;
				int32 minutes = (seconds - (hours * 3600)) / 60;
				seconds = seconds - (hours * 3600) - (minutes * 60);
				char *pFoo = strrchr(info->m_filename,'\\');
				if (pFoo) {
					strncpy(g_displayInfo.path,++pFoo,sizeof(g_displayInfo.path)-1);
				} else {
					strncpy(g_displayInfo.path,info->m_filename,sizeof(g_displayInfo.path)-1);
				}
				g_displayInfo.totalhours = hours;
				g_displayInfo.totalminutes = minutes;
				g_displayInfo.totalseconds = seconds;
				g_displayInfo.scrollOffset = 0;
				g_displayInfo.indexOfSong = info->m_indexOfSong;
				g_displayInfo.totalSongs = info->m_totalSongs;

                if(NeedToScroll())
                    SetTimer(m_hwnd, 0x01, 200, NULL);
                else
                    KillTimer(m_hwnd, 0x01);

                UpdateDisplay(m_hwnd);
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

                UpdateDisplay(m_hwnd);
	            break; 
            }

            
            case INFO_DoneOutputting: 
            {
	            break; 
            }

            case INFO_PlayListDonePlay:
            {
                g_displayInfo.hours = 0;
				g_displayInfo.minutes = 0;
				g_displayInfo.seconds = 0;
				g_displayInfo.frame = 0;
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
            m_plm->Add(arg,0);
            count++;
	    }
    }

    m_plm->SetFirst();

    if(shuffle) 
        m_plm->SetShuffle(SHUFFLE_SHUFFLED);
    
    //if(autoplay)
       //m_target->AcceptEvent(m_target, new Event(CMD_Play));
}

void
FreeAmpUI::
SetPlayListManager(PlayListManager *plm) {
	m_plm = plm;
}

void
FreeAmpUI::
CreateUI()
{
    WNDCLASS wc;
    MSG msg;
    HWND hwnd;
    	
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInst;
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon = LoadIcon( g_hInst, MAKEINTRESOURCE(IDI_EXE_ICON) );
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
