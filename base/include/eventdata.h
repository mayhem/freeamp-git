
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

#ifndef _EVENTDATA_H_
#define _EVENTDATA_H_

#include <iostream.h>
#include <string.h>

class Id3TagInfo {
 public:
    bool contains_info;

    char songname[31];
    char artist[31];
    char album[31];
    char year[5];
    char comment[31];
    char genre;

    Id3TagInfo() {
	contains_info = false;
	nullinit();
    }

    Id3TagInfo(char *buffer) {
	if (buffer && (!strncmp(buffer,"TAG",3))) {
	    memset(this,0,sizeof(Id3TagInfo));
	    contains_info = true;
	    strncpy(songname, buffer + 3, 30);
	    strncpy(artist, buffer + 33, 30);
	    strncpy(album, buffer + 63, 30);
	    strncpy(year, buffer + 93, 4);
	    strncpy(comment, buffer + 97, 30);
	    genre = buffer[127];
	} else {
	    contains_info = false;
	    nullinit();
	}
    }

 private:
    void nullinit() {
	*songname = '\0';
	*artist = '\0';
	*album = '\0';
	*year = '\0';
	*comment = '\0';
	genre = '\0';
    }
};

// Sent on INFO_MediaTimePosition

class MediaTimePositionInfo {
 public:
    MediaTimePositionInfo(int32 s,int32 f) {
	seconds = s; frame = f;
    }
    float seconds;
    int32 frame;
};

// Sent on INFO_MediaVitalStats 

class MediaVitalInfo {
 public:
    int32 totalFrames;
    int32 bytesPerFrame;
    char songTitle[512];
    char filename[512];
    int32 bps;  // bits per second
    int32 freq; // is Hz
    float totalTime;
    Id3TagInfo tagInfo;

    MediaVitalInfo(const char *t,const char *fn, int32 tf, int32 bpf, int32 bitspersecond, int32 fr, float tt, Id3TagInfo &tag) {
	tagInfo = tag;
	totalFrames = tf;
	bytesPerFrame = bpf;
	bps = bitspersecond;
	freq = fr;
	totalTime = tt;
	if (t) {
	    strncpy(songTitle,t,511);
	    songTitle[511] = '\0';
	} else {
	    songTitle[0] = '\0';
	}
	if (fn) {
	    strncpy(filename,fn,511);
	    filename[511] = '\0';
	} else {
	    filename[0] = '\0';
	}
   }
    
};


#endif /* _EVENTDATA_H_ */



