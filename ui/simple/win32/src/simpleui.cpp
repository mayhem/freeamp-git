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
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <math.h>


/* project headers */
#include "config.h"
#include "thread.h"
#include "simpleui.h"
#include "event.h"
#include "eventdata.h"
#include "playlist.h"
#include "about.h"
#include "resource.h"

HINSTANCE g_hInstance = NULL;

BOOL CALLBACK MainProc(	HWND hwnd, 
						UINT msg, 
						WPARAM wParam, 
						LPARAM lParam ); 

extern "C" SimpleUI *Initialize() 
{
    return new SimpleUI();
}


INT WINAPI DllMain (HINSTANCE hInst,
                    ULONG ul_reason_being_called,
	                LPVOID lpReserved)
{
	switch (ul_reason_being_called)
	{
		case DLL_PROCESS_ATTACH:
			g_hInstance = hInst;
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


SimpleUI::
SimpleUI():
UserInterface()
{
    m_scrolling = false;

    m_uiSemaphore = new Semaphore();

    m_uiThread = Thread::CreateThread();
    m_uiThread->Create(UIThreadFunc,this);

    m_uiSemaphore->Wait();
}

SimpleUI::
~SimpleUI()
{
    delete m_uiSemaphore;
}

void 
SimpleUI::
SetTarget(EventQueue* eq)
{
    m_target = eq;
}

void 
SimpleUI::
SetHwnd(HWND hwnd)
{
    m_hwnd = hwnd;

    m_hwndPlay	    = GetDlgItem(m_hwnd, IDC_PLAY);
	m_hwndStop	    = GetDlgItem(m_hwnd, IDC_STOP);
	m_hwndPause	    = GetDlgItem(m_hwnd, IDC_PAUSE);
	m_hwndNext	    = GetDlgItem(m_hwnd, IDC_NEXTSONG);
	m_hwndLast	    = GetDlgItem(m_hwnd, IDC_LASTSONG);
	m_hwndSlider    = GetDlgItem(m_hwnd, IDC_SLIDER);
    m_hwndCurrent   = GetDlgItem(m_hwnd, IDC_CURRENT_TIME);
    m_hwndTotal		= GetDlgItem(m_hwnd, IDC_TOTAL_TIME);
	m_hwndStatus	= GetDlgItem(m_hwnd, IDC_STATUS);
}

int32 
SimpleUI::
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
	            break; 
            }

            case INFO_Stopped: 
            {
                EnableWindow(m_hwndPlay, TRUE);
				EnableWindow(m_hwndNext, TRUE);
				EnableWindow(m_hwndLast, TRUE);

				EnableWindow(m_hwndStop, FALSE);
				EnableWindow(m_hwndPause, FALSE);
				
                SendMessage(m_hwndSlider,
						    TBM_SETPOS,
						    (WPARAM)TRUE,
						    (LPARAM)0);

                EnableWindow(m_hwndSlider, FALSE);

                char timeString[256] = "00:00:00";

                SetWindowText(m_hwndCurrent, timeString);

	            break; 
            }
			case INFO_MPEGInfo: 
				{
					MpegInfoEvent *info = (MpegInfoEvent *)event;
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
				
				break;
				}
            case INFO_MediaInfo: 
            {
                MediaInfoEvent *info = (MediaInfoEvent*)event;

                
                char timeString[256] = "00:00:00";
			    char szTemp[256];

                SetWindowText(m_hwndCurrent, timeString);


                int32 seconds = (int32)ceil(info->m_totalSeconds);
				m_totalSeconds = seconds;
			    int32 hours = seconds / 3600;
			    int32 minutes = seconds / 60 - hours * 60;
			    seconds = seconds - minutes * 60 - hours * 3600;

				
                sprintf(timeString,"%02d:%02d:%02d",hours,
				                                    minutes,
				                                    seconds);

			    SetWindowText(m_hwndTotal, timeString);



                sprintf(szTemp, "%d of %d", info->m_indexOfSong,info->m_totalSongs);
			    SendMessage(m_hwndStatus, 
						    SB_SETTEXT, 
						    2, 
						    (LPARAM) szTemp);

		
			    SendMessage(m_hwndSlider,
						    TBM_SETPOS,
						    (WPARAM)TRUE,
						    (LPARAM)0);

                SetWindowText(m_hwnd, info->m_filename);


		        /*totalFrames = info->totalFrames;
		        totalTime = info->totalTime;
		        char *path = info->filename;
		        char *pLastSlash = strrchr(path,'/');
		        char *dir = NULL;
		        char *fname = NULL;
		        if (pLastSlash) {
			    *pLastSlash = '\0';
			    fname = (char *)((int)pLastSlash + 1);
			    dir = path;
		        } else {
			    fname = path;
		        }
		        strncpy(fileName,fname,511);

		        fileName[511] = '\0';

		        if (info->tagInfo.contains_info) 
                {
			        fprintf(stderr,"Title  : %30s  Artist: %s\n",info->tagInfo.songname,info->tagInfo.artist);
			        fprintf(stderr,"Album  : %30s  Year: %4s, Genre: %d\n",info->tagInfo.album,info->tagInfo.year,(int)info->tagInfo.genre);
			        fprintf(stderr,"Comment: %30s \n",info->tagInfo.comment);
		        }

		        if (verboseMode == false) 
                {
                cerr << "Playing MPEG stream from " << fileName << " ..." << endl;
			    cerr << "MPEG 1.0, Layer: III, Freq: " << info->freq << ", mode: Joint-Stereo, modext: 3, BPF : " << info->bytesPerFrame  << endl;
			    cerr << "Channels: 2, copyright: No, original: Yes, CRC: No, emphasis: 0." << endl;
			    cerr << "Bitrate: " << info->bps/1000 << " KBits/s, Extension value: 0" << endl;
			    cerr << "Audio: 1:1 conversion, rate: " << info->freq << ", encoding: signed 16 bit, channels: 2" << endl;
		        }*/

	            break; 
            }

            case INFO_MediaTimeInfo: 
            {
                MediaTimeInfoEvent* info = (MediaTimeInfoEvent*)event;
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
SimpleUI::
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
SimpleUI::
CreateUI()
{
    InitCommonControls();

	DialogBoxParam( g_hInstance, 
                    MAKEINTRESOURCE(IDD_PLAYER),
                    NULL,
                    MainProc, 
                    (LPARAM)this);

    m_target->AcceptEvent(new Event(CMD_QuitPlayer));
}

void 
SimpleUI::
UIThreadFunc(void* arg)
{
    SimpleUI* ui = (SimpleUI*)arg;

    ui->CreateUI();
}


BOOL CALLBACK SimpleUI::MainProc(	HWND hwnd, 
						            UINT msg, 
						            WPARAM wParam, 
						            LPARAM lParam )
{
	BOOL result = FALSE;
    static SimpleUI* m_ui = NULL;

	switch(msg)
	{
		case WM_INITDIALOG:
		{
			HWND hwndStatus = NULL;
			HWND hwndPlay	= GetDlgItem(hwnd, IDC_PLAY);
			HWND hwndStop	= GetDlgItem(hwnd, IDC_STOP);
			HWND hwndPause	= GetDlgItem(hwnd, IDC_PAUSE);
			HWND hwndNext	= GetDlgItem(hwnd, IDC_NEXTSONG);
			HWND hwndLast	= GetDlgItem(hwnd, IDC_LASTSONG);
			HWND hwndSlider	= GetDlgItem(hwnd, IDC_SLIDER);

			EnableWindow(hwndPlay, FALSE);
			EnableWindow(hwndNext, FALSE);
			EnableWindow(hwndLast, FALSE);

			EnableWindow(hwndStop, FALSE);
			EnableWindow(hwndPause, FALSE);
			EnableWindow(hwndSlider, FALSE);

			hwndStatus= CreateStatusWindow(	WS_CHILD | WS_VISIBLE, 
											"", 
											hwnd, 
											IDC_STATUS);
			const int32 kNumPanes = 3;
			int32 panes[kNumPanes]= {60, 125,-1};

			SendMessage(hwndStatus, SB_SETPARTS, kNumPanes, (LPARAM) panes);

            m_ui = (SimpleUI*)lParam;

            m_ui->SetHwnd(hwnd);

			result = TRUE;

            m_ui->m_uiSemaphore->Signal();

			break;
		}

		case WM_COMMAND:
		{
            static bool isPaused = false;

			switch(wParam)
			{
				case IDC_PLAY:
				{
                    if(isPaused)
                    {
                        m_ui->m_target->AcceptEvent(new Event(CMD_TogglePause));
                        isPaused = false;
                    }
                    else
                    {
                        m_ui->m_target->AcceptEvent(new Event(CMD_Play));
                    }

					break;
				}

				case IDC_PAUSE:
				{
                    isPaused = true;
                    m_ui->m_target->AcceptEvent(new Event(CMD_TogglePause));
					break;
				}

				case IDC_STOP:
				{
                    m_ui->m_target->AcceptEvent(new Event(CMD_Stop));
					break;
				}

				case IDC_NEXTSONG:
				{
                    m_ui->m_target->AcceptEvent(new Event(CMD_NextMediaPiece));
					break;
				}

				case IDC_LASTSONG:
				{
                    m_ui->m_target->AcceptEvent(new Event(CMD_PrevMediaPiece));
					break;
				}

				case IDC_OPEN:
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
						PlayList* playlist;

						playlist = new PlayList;

						strcpy(file, filelist);
						strcat(file, "\\");

						cp = filelist + ofn.nFileOffset;

						while(*cp)
						{
							strcpy(file + ofn.nFileOffset, cp);

							playlist->Add(file,0);

							cp += strlen(cp) + 1;
						}

						m_ui->m_target->AcceptEvent(new SetPlayListEvent(playlist));
                        EnableWindow(m_ui->m_hwndPlay, TRUE);
					}

					delete [] filelist;

					break;
				}

				case IDC_ABOUT:
				{
					HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
					
					DialogBox(	hinst, 
								MAKEINTRESOURCE(IDD_ABOUT),
								NULL,
								AboutProc);
					break;
				}

				case IDC_EDIT_CURRENTPLAYLIST:
				{
					HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
					
					/*PlayList* playlist = player->CurrentPlayList();

					DialogBoxParam(	hinst, 
									MAKEINTRESOURCE(IDD_PLAYLISTEDITOR),
									NULL,
									PlayListProc,
									(LPARAM)playlist);*/
					break;
				}

				case IDC_EDIT_PLAYLISTFILE:
				{
					HINSTANCE hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
					
					/*PlayList* playlist = player->CurrentPlayList();

					DialogBoxParam(	hinst, 
									MAKEINTRESOURCE(IDD_PLAYLISTEDITOR),
									NULL,
									PlayListProc,
									(LPARAM)playlist);*/
					break;
				}

				case IDC_EXIT:
				{
					PostQuitMessage(0);
					break;
				}
			}

			result = TRUE;
			break;
		}

		case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP) wParam;
			int32 count;
			char szFile[MAX_PATH + 1];
			PlayList* playlist;

			count = DragQueryFile(	hDrop,
									-1L,
									szFile,
									sizeof(szFile));

			playlist = new PlayList;

			for(int32 i = 0; i < count; i++)
			{
				DragQueryFile(	hDrop,
								i,
								szFile,
								sizeof(szFile));

				playlist->Add(szFile,0);
			}

			m_ui->m_target->AcceptEvent(new SetPlayListEvent(playlist));
            EnableWindow(m_ui->m_hwndPlay, TRUE);

			break;
		}

		case WM_HSCROLL:
		{
			HWND hwndSlider = GetDlgItem(hwnd, IDC_SLIDER);
			LRESULT position;

			switch (LOWORD(wParam)) 
			{
				case TB_LINEUP:
				case TB_LINEDOWN:
				case TB_PAGEDOWN:
				case TB_PAGEUP:
				case TB_THUMBPOSITION:
				{
		  			position = SendMessage(	hwndSlider, 
											TBM_GETPOS, 
											0, 
											0);
                    

	                char timeString[256] = "00:00:00";

					int32 seconds = m_ui->m_totalSeconds;
					int32 hours = seconds / 3600;
					int32 minutes = seconds / 60 - hours * 60;
					seconds = seconds - minutes * 60 - hours * 3600;


					sprintf(timeString,"%02d:%02d:%02d",hours,
														minutes,
														seconds);

					SetWindowText(m_ui->m_hwndTotal, timeString);
                   
					m_ui->m_target->AcceptEvent(new ChangePositionEvent(position));
		  	
					m_ui->m_scrolling = false;
					break;
				}

				case TB_THUMBTRACK:
				{
					position = SendMessage( hwndSlider, TBM_GETPOS,0,0);

	                char timeString[256] = "00:00:00";

					int32 seconds = (int32)ceil(m_ui->m_secondsPerFrame * position);
					int32 hours = seconds / 3600;
					int32 minutes = seconds / 60 - hours * 60;
					seconds = seconds - minutes * 60 - hours * 3600;


					sprintf(timeString,"%02d:%02d:%02d",hours,
														minutes,
														seconds);

					SetWindowText(m_ui->m_hwndTotal, timeString);
 					
					
					m_ui->m_scrolling = true;
					break;
				}
			}

			break;
        }

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			result = TRUE;
			break;
		}
	
	}

	return result;
}