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


#include "Mpg123UI.h"
#include "event.h"
#include "eventdata.h"
#include "playlist.h"

void mysigterm(int);
void mysigint(int);


extern "C" {
    UserInterface *Initialize() {
	return new Mpg123UI();
    }
	   }

EventQueue *Mpg123UI::m_playerEQ = NULL;

Mpg123UI::Mpg123UI() {
    m_mediaInfo_set = false;
    m_mpegInfo_set = false;
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
    Mpg123UI::m_playerEQ->AcceptEvent(e);
    e = new Event(CMD_QuitPlayer);
    Mpg123UI::m_playerEQ->AcceptEvent(e);
}

void mysigint(int f) {
    mysigterm(f);
}
Mpg123UI::~Mpg123UI() {
    //cout << "deleting Mpg123UI" << endl;
    // NOTE: we don't call cleanup on this EventQueueRef because it is pointing to the player here...
    if (m_playerEQ) {
	delete m_playerEQ;
	m_playerEQ = NULL;
    }
}


int32 Mpg123UI::AcceptEvent(Event *e) {
    if (e) {
	//cerr << "Mpg123COO: processing event " << e->GetEvent() << endl;
	switch (e->Type()) {
	    case INFO_PlayListDonePlay: {
		Event *e = new Event(CMD_QuitPlayer);
		m_playerEQ->AcceptEvent(e);
		break; }
	    case INFO_ID3TagInfo: {
		ID3TagEvent *ite = (ID3TagEvent *)e;
		if (ite) {
		    m_id3Tag = ite->GetId3Tag();
		}
		break;
	    }
	    case INFO_MPEGInfo: {
		MpegInfoEvent *mie = (MpegInfoEvent *)e;
		if (mie) {
		    m_mpegInfo = *mie;
		    m_mpegInfo_set = true;
		    DisplayStuff();
		}
		totalFrames = m_mpegInfo.GetTotalFrames();
		break;
	    }
	    case INFO_MediaInfo: {
		MediaInfoEvent *pmvi = (MediaInfoEvent *)e;
		if (pmvi) {
		    m_mediaInfo = *pmvi;
		    m_mediaInfo_set = true;
		    DisplayStuff();
		}
		totalTime = m_mediaInfo.m_totalSeconds;
		break;
	    }
	    case INFO_MediaTimeInfo: {
		MediaTimeInfoEvent *pmtp = (MediaTimeInfoEvent *)e;
		if (verboseMode == true) {
//		    cout << "foo: " << pmtp->m_frame << "  " << pmtp->m_seconds << endl;
		    fprintf(stderr,"\rFrame# %5d [%5d], ",pmtp->m_frame,totalFrames-pmtp->m_frame);
		    fprintf(stderr,"Time: %3.2f [%3.2f], ",pmtp->m_totalSeconds,totalTime-pmtp->m_totalSeconds);
		}
		lastSeconds = pmtp->m_totalSeconds;
		break;}
	    case INFO_Stopped: {
		int m = (int)lastSeconds / 60;
		int s = (int)lastSeconds % 60;
		fprintf(stderr,"\n[%d:%02d] Decoding of %s finished.\n",m,s,fileName);
		break;
	    }
	    case CMD_Cleanup: {
		Event *e = new Event(INFO_ReadyToDieUI);
		//cout << "I'm gonna ACTUALLY kill myself" << endl;
		m_playerEQ->AcceptEvent(e);
		break; }
	    default:
		break;
	}
    }
//cerr << "Done cmdlinecoo: accpet event" << endl;
    return 0;
}

void Mpg123UI::DisplayStuff() {
    if (!m_mpegInfo_set) return;
    if (!m_mediaInfo_set) return;
    char *path = m_mediaInfo.m_filename;
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
    if (m_id3Tag.m_containsInfo) {
	fprintf(stderr,"Title  : %30s  Artist: %s\n",m_id3Tag.m_songName,m_id3Tag.m_artist);
	fprintf(stderr,"Album  : %30s  Year: %4s, Genre: %d\n",m_id3Tag.m_album,m_id3Tag.m_year,(int)m_id3Tag.m_genre);
	fprintf(stderr,"Comment: %30s \n",m_id3Tag.m_comment);
    }
    cerr << endl;
    
    static char *mpeg_string[4] = { "1.0","1.0","2.0","2.5"};
    static char *mpeg_layer[4] = { "Unknown","I", "II", "III"};
    static char *mpeg_stereo[4] = { "Stereo", "Joint-Stereo", "Dual-Channel", "Single-Channel" };

    if (verboseMode == false) {
	// NORMAL MODE
	if (dir) {
	    cerr << "Directory: " << dir << "/" << endl;
	}
	cerr << "Playing MPEG stream from " << fileName << " ..." << endl;
	cerr << "MPEG " << mpeg_string[m_mpegInfo.GetMpegVersion()] << " layer "<< mpeg_layer[m_mpegInfo.GetLayer()]<< ", " << m_mpegInfo.GetBitRate()/1000 << " KBit/s" << ", " << m_mpegInfo.GetSampleRate() << " Hz" << " " << mpeg_stereo[m_mpegInfo.GetStereo()] << endl;
    } else {
	// VERBOSE MODE
	if (dir) {
	    cerr << "Directory: " << dir << "/" << endl;
	}
	cerr << "Playing MPEG stream from " << fileName << " ..." << endl;
	cerr << "MPEG " << mpeg_string[m_mpegInfo.GetMpegVersion()];
	cerr << ", Layer: " << mpeg_layer[m_mpegInfo.GetLayer()];
	cerr << ", Freq: " << m_mpegInfo.GetSampleRate();
	cerr << ", mode: " << mpeg_stereo[m_mpegInfo.GetStereo()] << ", modext: 3, BPF : " << m_mpegInfo.GetBytesPerFrame() << endl;
	cerr << "Channels: 2, copyright: No, original: Yes, CRC: No, emphasis: 0." << endl;
	cerr << "Bitrate: " << m_mpegInfo.GetBitRate()/1000 << " KBits/s, Extension value: 0" << endl;
	cerr << "Audio: 1:1 conversion, rate: " << m_mpegInfo.GetSampleRate() << ", encoding: signed 16 bit, channels: 2" << endl;
    }
}


void Mpg123UI::SetArgs(int argc, char **argv) {
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
		    pl->SetOrder(PlayList::ORDER_SHUFFLED);
		    //cout << "shuffle play (with wildcards) " << endl;
		    break;
		case 'Z':
		    pl->SetOrder(PlayList::ORDER_RANDOM);
		    //cout << "Random Play" << endl;
		    break;
		default:
		    cout << "Unknown option:  " << pc << endl;
		    break;
	    }
	} else {
	    //ut << "Adding: " <<argv[i] << endl;
	    pl->Add(argv[i],0);
	}
    }


    pl->SetFirst();
    pl->SetSkip(skipFirst);
    Event *e = new SetPlayListEvent(pl);
    m_playerEQ->AcceptEvent(e);
    e = new Event(CMD_Play);
    m_playerEQ->AcceptEvent(e);
}

void Mpg123UI::SetTarget(EventQueue *eq) {
    m_playerEQ = eq;
}

#if 0

void Mpg123UI::SetRef(UIRef ui) {

}

#ifdef __cplusplus
extern "C" {
#endif


    void SetArgs(UI *ref, int32 c, char **v) {
	UserInterface *ui = (UserInterface *)ref->ref;
	ui->SetArgs(c,v);
    }
    
    int32 AcceptEvent(UI *ref,Event *e) {
	UserInterface *ui = (UserInterface *)ref->ref;
	return ui->AcceptEvent(e);
    }
    
    void SetTarget(UI *ref, EventQueue *eq) {
	UserInterface *ui = (UserInterface *)ref->ref;
	ui->SetTarget(eq);
    }
	
    void Cleanup(UI *ref) {
	UserInterface *ui = (UserInterface *)ref->ref;
	delete ui;
    }

void Initialize(UI *ref)
{
    if(ref)
    {
        UserInterface *ui = new Mpg123UI();
        ref->ref = ui;

	ref->SetArgs = SetArgs;
	ref->AcceptEvent = AcceptEvent;
	ref->SetTarget = SetTarget;
	ref->Cleanup = Cleanup;
    }
}

#ifdef __cplusplus
	   }
#endif

#endif
