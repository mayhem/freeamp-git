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

#ifndef _MEDIA_VITAL_INFO_H_
#define _MEDIA_VITAL_INFO_H_

#include <iostream.h>
#include <string.h>
#include "id3v1.h"

// Sent on INFO_MediaTimePosition

class MediaTimePositionInfo {
 public:
    MediaTimePositionInfo(float s,int32 f) {
	    m_seconds = s; m_frame = f;
    }
    float m_seconds;
    int32 m_frame;
};

// Sent on INFO_MediaVitalStats 

class MediaVitalInfo {
 public:
    int32 m_totalFrames;
    int32 m_bytesPerFrame;
    char m_songTitle[512];
    char m_filename[512];
    int32 m_bps;  // bits per second
    int32 m_freq; // is Hz
    float m_totalTime;
    Id3TagInfo m_tagInfo;
    int32 indexOfSong;
    int32 totalSongs;

    MediaVitalInfo( const char *t,
                    const char *fn, 
                    int32 tf, 
                    int32 bpf, 
                    int32 bitspersecond, 
                    int32 fr, 
                    float tt, 
                    Id3TagInfo &tag) 
    {
        m_tagInfo = tag;
        m_totalFrames = tf;
        m_bytesPerFrame = bpf;
        m_bps = bitspersecond;
        m_freq = fr;
        m_totalTime = tt;
        indexOfSong = 0;
        totalSongs = 0;

        if (t) {
	        strncpy(m_songTitle,t,511);
	        m_songTitle[511] = '\0';
        } else {
	        m_songTitle[0] = '\0';
        }

        if (fn) {
	        strncpy(m_filename,fn,511);
	        m_filename[511] = '\0';
        } else {
	        m_filename[0] = '\0';
        }
    }
    
};


#endif /* _MEDIA_VITAL_INFO_H_ */



