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

#include <iostream.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>

#include <curses.h>
#include <termios.h>


#include "Mpg123UI.h"
#include "event.h"
#include "eventdata.h"
#include "player.h"
#include "playlist.h"

void mysigterm(int);
void mysigint(int);


Mpg123UI::Mpg123UI() {

    //cout << endl << "mpg123 0.59k command line compatability mode" << endl << endl;
    cerr << "High Performance MPEG 1.0 Audio Player for Layer 2, 3" << endl;
    cerr << "Version 0.05 (1998/Oct/06).  Written by Jason Woodward, Mark Elrod, others." << endl;
    cerr << "Copyrights GoodNoise, XingTech. See 'README' for more!" << endl;
    cerr << "This software is distributed under the GNU GPL." << endl;

    verboseMode = false;
    totalFrames = 0;

    signal(SIGTERM,mysigterm);
    signal(SIGINT,mysigint);
    
}


void mysigterm(int f) {
    Event *e = new Event(CMD_Stop);
    Player::GetPlayer()->AcceptEvent(e);
    e = new Event(CMD_QuitPlayer);
    Player::GetPlayer()->AcceptEvent(e);
}

void mysigint(int f) {
    mysigterm(f);
}
Mpg123UI::~Mpg123UI() {
    //cout << "deleting Mpg123UI" << endl;
}


int32 Mpg123UI::acceptCIOEvent(Event *e) {
    if (e) {
	//cerr << "Mpg123UI: processing event " << e->getEvent() << endl;
	switch (e->getEvent()) {
	    case INFO_PlayListDonePlay: {
		Event *e = new Event(CMD_QuitPlayer);
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    case CMD_Cleanup: {
		Event *e = new Event(INFO_ReadyToDieCIO,this);
		//cout << "I'm gonna ACTUALLY kill myself" << endl;
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    default:
		break;
	}
    }
    return 0;
}

int32 Mpg123UI::acceptCOOEvent(Event *e) {
    if (e) {
	//cerr << "Mpg123COO: processing event " << e->getEvent() << endl;
	switch (e->getEvent()) {
	    case INFO_MediaVitalStats: {
		MediaVitalInfo *pmvi = (MediaVitalInfo *)e->getArgument();
		if (pmvi) {
		    totalFrames = pmvi->totalFrames;
		    totalTime = pmvi->totalTime;
		    char *path = pmvi->filename;
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
		    if (pmvi->tagInfo.contains_info) {
			fprintf(stderr,"Title  : %30s  Artist: %s\n",pmvi->tagInfo.songname,pmvi->tagInfo.artist);
			fprintf(stderr,"Album  : %30s  Year: %4s, Genre: %d\n",pmvi->tagInfo.album,pmvi->tagInfo.year,(int)pmvi->tagInfo.genre);
			fprintf(stderr,"Comment: %30s \n",pmvi->tagInfo.comment);
		    }
		    cerr << endl;

		    if (verboseMode == false) {
			// NORMAL MODE
			if (dir) {
			    cerr << "Directory: " << dir << "/" << endl;
			}
			cerr << "Playing MPEG stream from " << fileName << " ..." << endl;
			cerr << "MPEG 1.0 layer III, " << pmvi->bps/1000 << " KBit/s" << ", " << pmvi->freq << " Hz" << " joint-stereo" << endl;
		    } else {
			// VERBOSE MODE
			if (pmvi->tagInfo.contains_info) {
			    fprintf(stderr,"Title  : %30s  Artist: %s\n",pmvi->tagInfo.songname,pmvi->tagInfo.artist);
			    fprintf(stderr,"Album  : %30s  Year: %4s, Genre: %d\n",pmvi->tagInfo.album,pmvi->tagInfo.year,(int)pmvi->tagInfo.genre);
			    fprintf(stderr,"Comment: %30s \n",pmvi->tagInfo.comment);
			}

			if (dir) {
			    cerr << "Directory: " << dir << "/" << endl;
			}
			cerr << "Playing MPEG stream from " << fileName << " ..." << endl;
			cerr << "MPEG 1.0, Layer: III, Freq: " << pmvi->freq << ", mode: Joint-Stereo, modext: 3, BPF : " << pmvi->bytesPerFrame  << endl;
			cerr << "Channels: 2, copyright: No, original: Yes, CRC: No, emphasis: 0." << endl;
			cerr << "Bitrate: " << pmvi->bps/1000 << " KBits/s, Extension value: 0" << endl;
			cerr << "Audio: 1:1 conversion, rate: " << pmvi->freq << ", encoding: signed 16 bit, channels: 2" << endl;
		    }
		}
		break; }
	    case INFO_MediaTimePosition: {
		MediaTimePositionInfo *pmtp = (MediaTimePositionInfo *)(e->getArgument());
		if (verboseMode == true) {
//		    cout << "foo: " << pmtp->frame << "  " << pmtp->seconds << endl;
		    fprintf(stderr,"\rFrame# %5d [%5d], ",pmtp->frame,totalFrames-pmtp->frame);
		    fprintf(stderr,"Time: %3.2f [%3.2f], ",pmtp->seconds,totalTime-pmtp->seconds);
		}
		lastSeconds = pmtp->seconds;
		break;}
	    case INFO_Stopped: {
		int m = (int)lastSeconds / 60;
		int s = (int)lastSeconds % 60;
		fprintf(stderr,"\n[%d:%02d] Decoding of %s finished.\n",m,s,fileName);
		break;
	    }
	    case CMD_Cleanup: {
		Event *e = new Event(INFO_ReadyToDieCOO);
		//cout << "i'm gonna NOT kill myself..." << endl;
		Player::GetPlayer()->AcceptEvent(e);
		break; }
	    default:
		break;
	}
    }
    //cerr << "Done cmdlinecoo: accpet event" << endl;
    return 0;
}


void Mpg123UI::setArgs(int argc, char **argv) {
    PlayList *pl = new PlayList();
    char *pc = NULL;
    for(int i=1;i<argc;i++) {
	pc = argv[i];
	if (pc[0] == '-') {
	    switch (pc[1]) {
		case 't': 
		    cout << "test mode" << endl;
		    break;
		case 'v': 
		    verboseMode = true;
		    //cout << "verbose mode" << endl;
		    break;
		case 'k': 
		    i++;
		    pc = argv[i];
		    skipFirst = atoi(pc);
		    //cout << "skiping first " << skipFirst << " frames" << endl;
		    break;
		case 'c':
		    cout << "check range violations" << endl;
		    break;
		case 'b':
		    i++;
		    pc = argv[i];
		    cout << "output buffer: " << pc << "Kbytes" << endl;
		    break;
		case 'r':
		    i++;
		    pc = argv[i];
		    cout << "override samplerate: " << pc << endl;
		    break;
		case 'o': {
		    switch (pc[2]) {
			case 's':
			    cout << "output to builtin speaker" << endl;
			    break;
			case 'l':
			    cout << "output to line connector" << endl;
			    break;
			case 'h':
			    cout << "output to headphones" << endl;
			    break;
		    }
		}
		case '2':
		    cout << "downsample 1:2 (22 kHz)" << endl;
		    break;
		case 'd':
		    i++;
		    pc = argv[i];
		    cout << "play only every nth frame" << endl;
		    break;
		case '0':
		    cout << "decode channel 0 (left) only" << endl;
		    break;
		case 'm':
		    cout << "mix both channels (mono)" << endl;
		    break;
		case '@':
		    i++;
		    pc = argv[i];
		    cout << "read filenames/URLs from " << pc << endl;
		    break;
		case 'q':
		    cout << "quiet" << endl;
		    break;
		case 's':
		    cout << "write to standard out" << endl;
		    break;
		case 'n':
		    i++;
		    pc = argv[i];
		    cout << "decode only " << pc << " frames" << endl;
		    break;
		case 'y':
		    cout << "disable resync on errors" << endl;
		    break;
		case 'f':
		    i++;
		    pc = argv[i];
		    cout << "change scalefactor to " << pc << endl;
		    break;
		case 'g':
		    i++;
		    pc = argv[i];
		    cout << "set audio output hardware gain to " << pc << endl;
		    break;
		case 'a':
		    i++;
		    pc = argv[i];
		    cout << "set audio device " << pc << endl;
		    break;
		case '4':
		    cout << "downsample 1:4 (11 kHz)" << endl;
		    break;
		case 'h':
		    i++;
		    pc = argv[i];
		    cout << " play every frame " << pc << " times" << endl;
		    break;
		case '1':
		    cout << "decode channel 1 only (right) " << endl;
		    break;
		case 'p':
		    i++;
		    pc = argv[i];
		    cout << "use HTTP proxy " << pc << endl;
		    break;
		case 'z':
		    cout << "shuffle play (with wildcards) " << endl;
		    break;
		case 'Z':
		    cout << "Random Play" << endl;
		    break;
		default:
		    cout << "Unknown option:  " << pc << endl;
		    break;
	    }
	} else {
	    pl->add(argv[i],0);
	}
    }


    pl->setFirst();
    pl->setSkip(skipFirst);
    Event *e = new Event(CMD_SetPlaylist,pl);
    Player::GetPlayer()->AcceptEvent(e);
    e = new Event(CMD_Play);
    Player::GetPlayer()->AcceptEvent(e);
}







