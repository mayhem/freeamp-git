
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

#include "lmc.h"
#include "playlist.h"
#include "event.h"
#include "id3v1.h"


class SetPlayListEvent : public Event {
 public:
    SetPlayListEvent(PlayList *pl) { m_type = CMD_SetPlaylist; m_arg = pl; }
    PlayList *GetPlayList() { return m_arg; }
    virtual ~SetPlayListEvent() {};
 private:
    PlayList *m_arg;
};

class MediaInfoEvent : public Event {
 public:
    float m_totalSeconds;
    int32 m_indexOfSong;
    int32 m_totalSongs;
    char m_songTitle[512];
    char m_filename[512];
    virtual ~MediaInfoEvent() {}
    MediaInfoEvent( const char *t,
                    const char *fn, 
                    float ts)
    {
		m_type = INFO_MediaInfo;
        m_totalSeconds = ts;
        m_indexOfSong = 0;
        m_totalSongs = 0;

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

class MediaTimeInfoEvent : public Event {
 public:
    int32 m_hours, m_minutes, m_seconds, m_milliseconds, m_frame;
    float m_totalSeconds;
    MediaTimeInfoEvent(int32 h, int32 m, int32 s, int32 ms, float ts, int32 f) {
	m_type = INFO_MediaTimeInfo; m_totalSeconds = ts;
	m_hours = h; m_minutes = m; m_seconds = s; m_milliseconds = ms; m_frame = f;
    }
    virtual ~MediaTimeInfoEvent() {}
};

class ChangePositionEvent : public Event {
 private:
    int32 m_frame;
 public:
    ChangePositionEvent(int32 p) { m_type = CMD_ChangePosition; m_frame = p; }
    int32 GetPosition() { return m_frame; }
    virtual ~ChangePositionEvent() {}
};

class LMCErrorEvent : public Event {
 private:
    LogicalMediaConverter *m_lmc;
    Error m_error;
 public:
    LMCErrorEvent(LogicalMediaConverter *p, Error e) { m_type = INFO_LMCError; m_lmc = p; m_error = e; }
    LogicalMediaConverter *GetLMC() { return m_lmc; }
    Error GetError() { return m_error; }
    virtual ~LMCErrorEvent() {}
};

class MpegInfoEvent : public Event {
public:
	enum {LAYER_1=1,LAYER_2,LAYER_3};
	enum {MPEG_1=1,MPEG_2=2, MPEG_25=3};
private:
    int32 m_totalFrames;
    int32 m_bytesPerFrame;
    int32 m_bitrate;
    int32 m_sampleRate;
	int32 m_layer;
	int32 m_mpeg;
public:
	MpegInfoEvent(int32 tf, int32 bpf, int32 br,int32 sr,int32 layer,int32 mpeg) { 
		m_type = INFO_MPEGInfo; 
		m_totalFrames = tf;
		m_bytesPerFrame = bpf;
		m_bitrate = br;
		m_sampleRate = sr;
		m_layer = layer;
		m_mpeg = mpeg;
	}
	int32 GetTotalFrames() { return m_totalFrames; }
	int32 GetBytesPerFrame() { return m_bytesPerFrame; }
	int32 GetBitRate() { return m_bitrate; }
	int32 GetSampleRate() { return m_sampleRate; }
	int32 GetLayer() { return m_layer; }
	int32 GetMpegVersion() { return m_mpeg; }
	virtual ~MpegInfoEvent() {}

};

class ID3TagEvent : public Event {
private:
	Id3TagInfo m_tagInfo;
public:
	ID3TagEvent(Id3TagInfo &t) { m_type = INFO_ID3TagInfo; Id3TagInfo m_tagInfo = t; }
	virtual ~ID3TagEvent() {}
};
#endif /* _EVENTDATA_H_ */



