
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
//#include "playlist.h"
#include "event.h"
#include "id3v1.h"
#include "vector.h"

class MediaInfoEvent : public Event {
 public:
    Vector<Event *> *m_childEvents;
    bool m_filled;
    float m_totalSeconds;
    int32 m_indexOfSong;
    int32 m_totalSongs;
    char m_filename[512];
    uint32 m_plmID;
    virtual ~MediaInfoEvent() {
	if (m_childEvents) {
	    m_childEvents->DeleteAll();
	    delete m_childEvents;
	    m_childEvents = NULL;
	}
    }
    MediaInfoEvent() { 
	m_type = INFO_MediaInfo; m_filled = false; m_filename[0] = '\0'; 
	m_childEvents = new Vector<Event *>();
    }
    MediaInfoEvent( const char *fn, 
                    float ts)
    {
	m_childEvents = new Vector<Event *>();
	m_filled = true;
	m_type = INFO_MediaInfo;
        m_totalSeconds = ts;
        m_indexOfSong = 0;
        m_totalSongs = 0;

        if (fn) {
	    strncpy(m_filename,fn,511);
	    m_filename[511] = '\0';
        } else {
	    m_filename[0] = '\0';
        }
    }
    void AddChildEvent(Event *pE) {
	if (pE) {
	    m_childEvents->Insert(pE);
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

#define	_EQUALIZER_ENABLE_
#ifdef	_EQUALIZER_ENABLE_
class SetEqualizerDataEvent : public Event {
private:
	float *m_eq;
	bool m_enable;
	bool m_IsEQData;
public:
	SetEqualizerDataEvent(bool enable) { m_type = CMD_SetEQData; m_enable = enable; m_IsEQData = false; }
	SetEqualizerDataEvent(float *eq) {	m_type = CMD_SetEQData; m_eq = eq; m_IsEQData = true; }
	float *GetEQData() { return m_eq; }
	bool IsEQData() { return m_IsEQData; }
	bool GetEnableState() { return m_enable; }
	virtual ~SetEqualizerDataEvent() {}
};
#endif	//_EQUALIZER_ENABLE_
#undef	_EQUALIZER_ENABLE_

#define	_VISUAL_ENABLE_
#ifdef	_VISUAL_ENABLE_
class SendVisBufEvent : public Event {
private:
		int32 m_length;
		unsigned char *m_buf;
public:
	SendVisBufEvent(int32& wrote, void *pBuffer,int32 length) {
		m_type = CMD_SendVisBuf;
		m_buf = new unsigned char[length];
		m_length = length;
		memcpy(m_buf, pBuffer, length);
		wrote = length;
	}
	unsigned char *GetBuf() { return m_buf; }
	int32 GetLength() { return m_length; }
	virtual ~SendVisBufEvent() { delete m_buf; }
};
#endif	//_VISUAL_ENABLE_
#undef	_VISUAL_ENABLE_

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
    enum { STEREO=1,JOINT_STEREO=2,DUAL=3,MONO=4};
    enum {LAYER_1=1,LAYER_2,LAYER_3};
    enum {MPEG_1=1,MPEG_2=2, MPEG_25=3};
 private:
    int32 m_totalFrames;
	float m_secondsPerFrame;
    int32 m_bytesPerFrame;
    int32 m_bitrate;
    int32 m_sampleRate;
    int32 m_layer;
    int32 m_mpeg;
    int32 m_channels;
    int32 m_original;
    int32 m_copyright;
    int32 m_emphasis;
    int32 m_stereo;
    int32 m_modeExt;
    int32 m_crc;
 public:
    MpegInfoEvent() { m_type = INFO_MPEGInfo; }
    MpegInfoEvent(int32 tf, float spf, int32 bpf, int32 br,int32 sr,int32 layer,int32 mpeg,int32 chans,int32 orig, int32 copy,int32 emph,int32 stereo, int32 mode_ext) { 
	m_type = INFO_MPEGInfo; 
	m_totalFrames = tf;
	m_secondsPerFrame = spf;
	m_bytesPerFrame = bpf;
	m_bitrate = br;
	m_sampleRate = sr;
	m_layer = layer;
	m_mpeg = mpeg;
	m_channels = chans;
	m_original = orig & 0x1;
	m_copyright = copy & 0x1;
	m_emphasis = emph;
	m_stereo = stereo;
	m_modeExt = mode_ext;
    }
    int32 GetTotalFrames() { return m_totalFrames; }
	float GetSecondsPerFrame() { return m_secondsPerFrame; }
    int32 GetBytesPerFrame() { return m_bytesPerFrame; }
    int32 GetBitRate() { return m_bitrate; }
    int32 GetSampleRate() { return m_sampleRate; }
    int32 GetLayer() { return m_layer; }
    int32 GetMpegVersion() { return m_mpeg; }
    int32 GetChannels() { return m_channels; }
    int32 GetOriginal() { return m_original; }
    int32 GetCopyright() { return m_copyright; }
    int32 GetEmphasis() { return m_emphasis; }
    int32 GetStereo() { return m_stereo; }
    int32 GetModeExt() { return m_modeExt; }
    int32 GetCRC() { return m_crc; }
    virtual ~MpegInfoEvent() {}
    
};

class ID3TagEvent : public Event {
private:
	Id3TagInfo m_tagInfo;
public:
	ID3TagEvent(Id3TagInfo &t) { m_type = INFO_ID3TagInfo; m_tagInfo = t; }
	Id3TagInfo GetId3Tag() { return m_tagInfo; }
	virtual ~ID3TagEvent() {}
};
#endif /* _EVENTDATA_H_ */



