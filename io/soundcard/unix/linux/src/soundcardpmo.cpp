
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
#include <stdlib.h>
#include <iostream.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <errno.h>
#include <string.h>


/* project headers */
#include <config.h>
#include "soundcardpmo.h"

#define PIECES 50

extern "C" {
PhysicalMediaOutput *Initialize() {
    return new SoundCardPMO();
}
	   }

SoundCardPMO::SoundCardPMO() {
    //cout << "Creating scpmo" << endl;
    myInfo = new OutputInfo();
    if ((audio_fd = open("/dev/dsp", O_WRONLY | O_SYNC, 0)) < 0) {
	if (errno == EBUSY) {
	    cerr << "Audio device busy!" << endl;
	    exit(1);
	} else {
	    cout << "Can't open /dev/dsp for writing" << endl;
	    exit(1);
	}
    }
    //cout << "Done creation of the SoundCardPMO..." << endl;
}

SoundCardPMO::~SoundCardPMO() {
    Reset(false);
    //cout << "Deleting scpmo... " << endl;
    close(audio_fd);
    if (myInfo) {
	delete myInfo;
	myInfo = NULL;
    }
    //cout << "Done deleting scpmo" << endl;
}

int SoundCardPMO::audio_fd = -1;


void SoundCardPMO::Pause() {
    Reset(true);
}

void SoundCardPMO::Resume() {

}

bool SoundCardPMO::Init(OutputInfo* info) {
    //cout << "initialize..." << endl;
//    cout << "OI: bits_per_sample: " << info->bits_per_sample << endl;
//    cout << "OI: number_of_channels: " << info->number_of_channels << endl;
//    cout << "OI: samples_per_second: " << info->samples_per_second << endl;
//    cout << "OI: max_buffer_size: " << info->max_buffer_size << endl;
    if (!info) {
	info = myInfo;
    }
    
    

    int fd = audio_fd;
    int flags;
    if ((flags = fcntl(fd,F_GETFL,0)) < 0) {
	cerr << "fcntl F_GETFL on /dev/dsp failed" << endl;
	exit(1);
    }

    flags &= O_NDELAY; //SYNC;
    
    if (fcntl(fd, F_SETFL, flags) < 0) {
	cerr << "fcntl F_SETFL on /dev/dsp failed" << endl;
	exit(1);
    }

    audio_fd = fd;
    
    channels = info->number_of_channels;
    
    for (int i = 0; i < info->number_of_channels; ++i)
	bufferp[i] = buffer + i;
    
    // configure the device:
    int play_precision = 16;
    int play_stereo = channels-1;
    int play_sample_rate = info->samples_per_second;

    int junkvar = 0;
    if (ioctl(audio_fd, SNDCTL_DSP_RESET, &junkvar) == -1) {
	cerr << "hosed during ioctl reset" << endl;
	exit(1);
    } 
    
    if(ioctl(audio_fd, SNDCTL_DSP_SAMPLESIZE, &play_precision) == -1) {
	cerr << "config of samplesize failed" << endl;
	exit(1);
    }

    if(ioctl(audio_fd, SNDCTL_DSP_STEREO, &play_stereo) == -1) {
	cerr << "config of stereo failed" << endl;
	exit(1);
    }
    if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &play_sample_rate) == -1) {
	cerr << "config of speed failed" << endl;
	exit(1);
    }
    myInfo->bits_per_sample = info->bits_per_sample;
    myInfo->number_of_channels = info->number_of_channels;
    myInfo->samples_per_second = info->samples_per_second;
    myInfo->max_buffer_size = info->max_buffer_size;
    return true;
}

bool SoundCardPMO::Reset(bool user_stop) {
    //cout << "Reset..." << endl;
    int a;
    ioctl(audio_fd,SNDCTL_DSP_RESET,&a);
    if (user_stop) {
	Init(NULL);
    }
}

int32 SoundCardPMO::Write(void *pBuffer,int32 length) {
    int32 actual = 0;
    int32 actualThisTime = 0;
    int32 writeBlockLength = length / PIECES;
    //cout << "length: " << length << endl;
    //cout << "writeBl" << endl;
    void *pv = NULL;
    int32 gp = 0;
    do {
	actual = write(audio_fd,pBuffer,length);
    } while ((actual != length) && (errno == EINTR));
    if(actual != length) {
	cerr << "Wrote " << length << ", and got " << actual << " back" << endl;
	cerr << "errno: " << errno << endl << "str: " << strerror(errno) << endl;
	actual = -1;
    }
    return actual;
}











