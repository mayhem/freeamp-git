/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Copyright (C) 1999 Stephan Auerhahn

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
        
----------------------------------------------------------------------------
ncurses user interface by stephan auerhahn (palpatine@midi.net)

this is a hacked up version of cmdlineUI.cpp
____________________________________________________________________________*/

#include <iostream.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>
#include <termios.h>
#include <signal.h>

/* ncurses / curses include */
#include <curses.h>
#ifndef NCURSES_VERSION
#define color_set(a,b) ;
#endif

#include "config.h"
#include "ncursesUI.h"
#include "event.h"
#include "playlist.h"
#include "thread.h"
#include "eventdata.h"


#define stdinfd 0

extern "C" {

UserInterface *Initialize(FAContext *context) {
    return new ncursesUI(context);
}

           }
static struct termios normalTTY;
static struct termios rawTTY;
int getKey() {
    fd_set rdfs;

    FD_ZERO(&rdfs);
    FD_SET(stdinfd, &rdfs);
    if (select (stdinfd+1,&rdfs,NULL,NULL,NULL) == 1) {
        if (FD_ISSET(stdinfd, &rdfs)) return 1;
    }
    return 0;
}

void ncursesUI::SetPlayListManager(PlayListManager *plm) {
    m_plm = plm;
}

ncursesUI::ncursesUI(FAContext *context) {
    m_context = context;
    m_plm = NULL;
    m_playerEQ = NULL;
    m_lastIndexPlayed = -1;
    m_id3InfoPrinted = false;
    m_mediaInfo_set = false;
    m_mpegInfo_set = false;
    totalFrames = 0;
    
    keyboardListenThread = NULL;
}

Error ncursesUI::Init(int32 startup_level) {
    if ((m_startupLevel = startup_level) == PRIMARY_UI) {
        tcgetattr(stdinfd, &::normalTTY);
        ::rawTTY = ::normalTTY;
        ::rawTTY.c_lflag &= ~ICANON;
        ::rawTTY.c_lflag &= ~ECHO;
        tcsetattr(stdinfd, TCSANOW, &rawTTY);
        

        keyboardListenThread = Thread::CreateThread();
        keyboardListenThread->Create(ncursesUI::keyboardServiceFunction,this);

        /* curses initialization */
        initscr();
        cbreak();
        noecho();
        nonl();
        intrflush( stdscr, false );
        keypad( stdscr, true );
        start_color();

        init_pair(1, COLOR_BLUE, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);
        init_pair(4, COLOR_CYAN, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_YELLOW, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
        init_pair(8, COLOR_BLACK, COLOR_BLACK);
        curs_set(0);

        showInfo();

        ProcessArgs();
    }
    return kError_NoErr;
}

ncursesUI::~ncursesUI() {
    if (m_startupLevel == PRIMARY_UI) {
        tcsetattr(stdinfd, TCSANOW, &normalTTY);
    }
    //cout << "ncursesUI: begin deleted..." << endl;
    if (keyboardListenThread) {
        keyboardListenThread->Destroy();
        delete keyboardListenThread;
        keyboardListenThread = NULL;
    }
}

void ncursesUI::keyboardServiceFunction(void *pclcio) {
    ncursesUI *pMe = (ncursesUI *)pclcio;
    //char *pkey = new char[1];
    char chr;
    //size_t rtn;
    //int fn = STDIN_FILENO;
    for (;;) {
        ::getKey();
        read(stdinfd,&chr,1);
        switch (chr) {
            case 'p':
            case 'P': {
                Event *e = new Event(CMD_TogglePause);
                pMe->m_playerEQ->AcceptEvent(e);
                break;
            }
            case '-': {
                Event *e = new Event(CMD_PrevMediaPiece);
                pMe->m_playerEQ->AcceptEvent(e);
                break;
            }
            case '=':
            case '+':
            case 'n':
            case 'N': {
                Event *e = new Event(CMD_NextMediaPiece);
                pMe->m_playerEQ->AcceptEvent(e);
                break; }
            case 'q':
            case 'Q': {
                Event *e = new Event(CMD_QuitPlayer);
                pMe->m_playerEQ->AcceptEvent(e);
                clear();
                move(0,0);
                endwin();
                break; }
            case 's':
            case 'S': {
                if (pMe->m_plm) {
                    pMe->m_plm->SetShuffle(SHUFFLE_RANDOM);
                    pMe->m_plm->SetFirst();
                }
                Event *e = new Event(CMD_Stop);
                pMe->m_playerEQ->AcceptEvent(e);
                e = new Event(CMD_Play);
                pMe->m_playerEQ->AcceptEvent(e);
                break;}
//          case 'f':{
//              Event *e = new Event(CMD_ChangePosition,(void *)200);
//              pMe->m_playerEQ->AcceptEvent(pMe->m_playerEQ,e);
//          }
            default:
                break;
        }
    }

}

int32 ncursesUI::AcceptEvent(Event *e) {
    if (e) {
        switch (e->Type()) {
            case INFO_PlayListDonePlay: {
                Event *e = new Event(CMD_QuitPlayer);
                m_playerEQ->AcceptEvent(e);
                break; }
            case CMD_Cleanup: {
                Event *e = new Event(INFO_ReadyToDieUI);
                m_playerEQ->AcceptEvent(e);
                break; }
            case INFO_MPEGInfo: {
                MpegInfoEvent *mie = (MpegInfoEvent *)e;
                if (mie) {
                    m_mpegInfo = *mie;
                    m_mpegInfo_set = true;
                }
                totalFrames = m_mpegInfo.GetTotalFrames();
                break;
            }
            case INFO_MediaInfo: {
                MediaInfoEvent *pmvi = (MediaInfoEvent *)e;
                if (pmvi)
                {
                    totalTime = pmvi->m_totalSeconds;
                    m_mediaInfo_set = true;
                }
                if (pmvi && m_lastIndexPlayed != pmvi->m_indexOfSong) {
                    m_lastIndexPlayed = pmvi->m_indexOfSong;
                    m_id3InfoPrinted = false;

                    clear();
                    move(0,0);
                    showInfo();
                    move(2, 0);
                    addstr("Title  : ");
                    addstr(pmvi->m_filename);
                    addstr("\nArtist : ");
                    addstr("\nAlbum  : ");
                    addstr("\nYear   : ");
                    addstr("\nComment: ");
                    addstr("\n");
//                    refresh();

                    counter = 0;
                    sprintf( title, "Freeamp - [%s]", pmvi->m_filename);
                    if ( 12 + strlen(pmvi->m_filename) > (unsigned)COLS - 13 )
                    {
                        titleStart = 0;
                        titleDir = -1;
                        move( 0, 6 );
                        color_set(7, NULL);
                        attron(A_REVERSE);
                        for (int i = 6; i < COLS - 7; i++)
                            addch(title[i-6]);
                        attroff(A_REVERSE);
                    }
                    else
                    {
                        move(0, (COLS / 2) - ( 12 + strlen(pmvi->m_filename) ) / 2 );
                        color_set(7, NULL);
                        attron(A_REVERSE);
                        addstr(title);
                        attroff(A_REVERSE);
//                        refresh();
                    }
                }
                break; }
            case INFO_StreamInfo:
            {
                char szTitle[100];
                int  i;
            
                StreamInfoEvent *pInfo = (StreamInfoEvent *)e;
           
                pInfo->GetTitle(szTitle, 100);
                move(2, 0);
                addstr("Title  : ");
                addstr(szTitle);
                for(i = 9 + strlen(szTitle); i < COLS; i++)
                    addch(' ');
                      
                break;
            } 
            case INFO_MediaTimeInfo: {
                MediaTimeInfoEvent *pmtp = (MediaTimeInfoEvent *)e;
                float percentAmount;
                int i=0, hours, minutes, seconds;
                char buf[1024];

                if (title && ( strlen(title) > (unsigned)COLS - 13 ) )
                   counter++;
                
                if ( counter > 50
                   || (counter > 4 && (titleDir == 0 || titleDir == 1)))
                {
                    int i;
                    counter = 0;
                    if (titleDir == -1)
                       titleDir = 0;
                    else if (titleDir == 2)
                       titleDir = 1;
                    if (titleDir == 0)
                        titleStart++;
                    else
                        titleStart--;
                    move( 0, 6 );
                    color_set(7, NULL);
                    attron(A_REVERSE);
                    for (i = 6; i < COLS - 7; i++)
                        addch(title[i-6+titleStart]);
                    attroff(A_REVERSE);
                    if ( titleStart <= 0 )
                       titleDir = -1;
                    else if ( i - 6 + (unsigned)titleStart >= strlen(title))
                       titleDir = 2;
                }

                
                percentAmount = ( totalTime / ( COLS + 1));
                move(LINES-2, 0);
                sprintf(buf, "%02d:%02d:%02d", pmtp->m_hours, pmtp->m_minutes,
                                               pmtp->m_seconds);
                addstr(buf);
                move(LINES-2, COLS-8);
                hours = (int)totalTime / 60 / 60;
                minutes = (int)totalTime / 60;
                seconds = (int)totalTime;
                seconds -= minutes * 60;
                minutes -= hours * 60;                

                sprintf(buf, "%02d:%02d:%02d", hours, minutes,
                                               seconds);
                addstr(buf);                
                move(LINES-1, 0);
                for (i=0; i < pmtp->m_totalSeconds / percentAmount; i++)
                    addch(ACS_BLOCK);
//                color_set(3, NULL);
                for (; i < COLS; i++)
                {
                    addch(ACS_BOARD);
                }
//                color_set(7, NULL);
                refresh();
                lastSeconds = pmtp->m_totalSeconds;
                break;}

            case INFO_ID3TagInfo: {
                ID3TagEvent *ite = (ID3TagEvent *)e;
                if (ite) {
                    Id3TagInfo ti = ite->GetId3Tag();
                    if (ti.m_containsInfo && !m_id3InfoPrinted) {
                        m_id3InfoPrinted = true;
                        move(2, 0);
                        addstr("Title  : ");
                        addstr(ti.m_songName);
                        addstr("\nArtist : ");
                        addstr(ti.m_artist);
                        addstr("\nAlbum  : ");
                        addstr(ti.m_album);
                        addstr("\nYear   : ");
                        addstr(ti.m_year);
                        addstr("\nComment: ");
                        addstr(ti.m_comment);
                        addstr("\n\n");
                        refresh();
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    return 0;
}

void ncursesUI::SetArgs(int argc, char **argv) {
    m_argc = argc; m_argv = argv;
}
void ncursesUI::ProcessArgs() {
    char *pc = NULL;
    for(int i=1;i<m_argc;i++) {
        //cout << "Adding arg " << i << ": " << argv[i] << endl;
        pc = m_argv[i];
        if (pc[0] == '-') {
            processSwitch(&(pc[0]));
        } else {
            m_plm->AddItem(pc,0);
        }
    }
    m_plm->SetFirst();
    Event *e = new Event(CMD_Play);
    m_playerEQ->AcceptEvent(e);
}

void ncursesUI::processSwitch(char *pc) {
    return;
}

void ncursesUI::showInfo() {
    move(0,0);
    color_set(7, NULL);
    attron(A_REVERSE);
    for (int i=0; i < COLS; i++)
        addstr(" ");
    move(0, (COLS/2) - 4);
    addstr("Freeamp");
    move(0, COLS-6);
    addstr("_ o x");
    attroff(A_REVERSE);
//    refresh();
    color_set(7, NULL);
    move(8,0);
    addstr("       Keystroke Help     \n");
    addstr("  ( q )    Quit           \n");
    addstr("  ( + )    Next Song      \n");
    addstr("  ( - )    Prev Song      \n");
    addstr("  ( p )    Pause / UnPause\n");
    addstr("  ( s )    Shuffle        \n");
    move(LINES - 2, 0);
    addstr("00:00:00");
    move(LINES - 2, COLS - 8);
    addstr("00:00:00");
    move(LINES - 1, 0);
    for (int i = 0; i < COLS; i++)
    {
         addch(ACS_BOARD);
    }
    
//    refresh();
    return;
}

