/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 1998-1999 EMusic.com

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

#ifndef INCLUDED_EVENTDATA_H_
#define INCLUDED_EVENTDATA_H_

#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
using namespace std;

#include "event.h"
#include "playlist.h"
#include "utility.h"

class     LogicalMediaConverter;

class     UserMessageEvent:public Event
{
   private:
   char     *m_info;

   public:

   virtual ~ UserMessageEvent()
   {
      if (m_info)
      {
         delete    m_info;
                   m_info = NULL;
      }
   }
   UserMessageEvent()
   {
      m_type = INFO_UserMessage;
      m_info = NULL;
   }
   UserMessageEvent(const char *info)
   {
      m_type = INFO_UserMessage;
      m_info = strdup_new(info);
   }
   const char *GetInfo()
   {
      return m_info;
   }
};

class StatusMessageEvent:public Event
{
   private:
   char     *m_info;

   public:
   virtual ~ StatusMessageEvent()
   {
      if (m_info)
      {
         free(m_info);
         m_info = NULL;
      }
   }
   StatusMessageEvent()
   {
      m_type = INFO_StatusMessage;
      m_info = "";
   }
   StatusMessageEvent(const char *info)
   {
      m_type = INFO_StatusMessage;
      m_info = strdup(info);
   }
   const char *GetStatusMessage()
   {
      return m_info;
   }
};

class ErrorMessageEvent:public Event
{
   private:
   char     *m_info;

   public:
   virtual ~ ErrorMessageEvent()
   {
      if (m_info)
      {
         free(m_info);
         m_info = NULL;
      }
   }
   ErrorMessageEvent()
   {
      m_type = INFO_ErrorMessage;
      m_info = "";
   }
   ErrorMessageEvent(const char *info)
   {
      m_type = INFO_ErrorMessage;
      m_info = strdup(info);
   }
   const char *GetErrorMessage()
   {
      return m_info;
   }
};


class     BrowserMessageEvent:public Event
{
   private:
   char     *m_info;

   public:
   virtual ~ BrowserMessageEvent()
   {
      if (m_info)
      {
         delete    m_info;
      }
   }
   BrowserMessageEvent()
   {
      m_type = INFO_BrowserMessage;
      m_info = "";
   }
   BrowserMessageEvent(const char *info)
   {
      m_type = INFO_BrowserMessage;
      m_info = strdup_new(info);
   }
   const char *GetBrowserMessage()
   {
      return m_info;
   }
};

class     MediaInfoEvent:public Event
{
   public:

   vector<Event*>*  m_childEvents;
   bool             m_filled;
   float            m_totalSeconds;
   int32            m_indexOfSong;
   int32            m_totalSongs;
   char             m_filename[512];
   uint32           m_plmID;

   virtual ~ MediaInfoEvent()
   {
      if (m_childEvents && (m_childEvents->size() != 0))
      {
            vector<Event *>::iterator i = m_childEvents->begin();

            for (; i != m_childEvents->end(); i++) 
                delete *i;
            delete m_childEvents;
            m_childEvents = NULL;
      }
   }

   MediaInfoEvent()
   {
      m_type = INFO_MediaInfo;
      m_filled = false;
      m_filename[0] = '\0';
      m_childEvents = new vector<Event *>;
   }

   MediaInfoEvent(MediaInfoEvent &other)
   {
      m_type = other.m_type;
      m_filled = other.m_filled;
      m_totalSeconds = other.m_totalSeconds;
      m_indexOfSong = other.m_indexOfSong;
      m_totalSongs = other.m_totalSongs;
      strcpy(m_filename, other.m_filename);

      m_childEvents = new vector <Event *>;

      m_childEvents->insert(m_childEvents->end(), 
                            other.m_childEvents->begin(),
                            other.m_childEvents->end());

      other.m_childEvents->clear();
   }

   MediaInfoEvent(const char *fn,
                  float ts)
   {
      m_childEvents = new vector<Event *>;
      m_filled = true;
      m_type = INFO_MediaInfo;
      m_totalSeconds = ts;
      m_indexOfSong = 0;
      m_totalSongs = 0;

      if (fn)
      {
         strncpy(m_filename, fn, 511);
         m_filename[511] = '\0';
      }
      else
      {
         m_filename[0] = '\0';
      }
   }
   void      AddChildEvent(Event * pE)
   {
      if (pE)
      {
         m_childEvents->push_back(pE);
      }
   }
};

class     MediaTimeInfoEvent:public Event
{
   public:
   int32 m_hours, m_minutes, m_seconds, m_milliseconds, m_frame;
   float     m_totalSeconds;
             MediaTimeInfoEvent(int32 h, int32 m, int32 s, int32 ms, float ts, int32 f)
   {
      m_type = INFO_MediaTimeInfo;
      m_totalSeconds = ts;
      m_hours = h;
      m_minutes = m;
      m_seconds = s;
      m_milliseconds = ms;
      m_frame = f;
   }
   virtual ~ MediaTimeInfoEvent()
   {
   }
};

class     VolumeEvent:public Event
{
   private:
   int32 m_volume;
   public:
   VolumeEvent(int32 t, int32 v = 0):m_volume(v)
   {
      m_type = t;
   }
   int32     GetVolume()
   {
      return m_volume;
   }
   virtual ~ VolumeEvent()
   {
   }
};

class     ChangePositionEvent:public Event
{
   private:
   int32 m_frame;
   public:
   ChangePositionEvent(int32 p)
   {
      m_type = CMD_ChangePosition;
      m_frame = p;
   }
   int32     GetPosition()
   {
      return m_frame;
   }
   virtual ~ ChangePositionEvent()
   {
   }
};

class     MpegInfoEvent:public Event
{
   public:
   enum
   {
      STEREO = 1, JOINT_STEREO = 2, DUAL = 3, MONO = 4
   };
   enum
   {
      LAYER_1 = 1, LAYER_2, LAYER_3
   };
   enum
   {
      MPEG_1 = 1, MPEG_2 = 2, MPEG_25 = 3
   };
             private:
             int32 m_totalFrames;
   float     m_secondsPerFrame;
   int32     m_bytesPerFrame;
   int32     m_bitrate;
   int32     m_sampleRate;
   int32     m_layer;
   int32     m_mpeg;
   int32     m_channels;
   int32     m_original;
   int32     m_copyright;
   int32     m_emphasis;
   int32     m_stereo;
   int32     m_modeExt;
   int32     m_crc;
             public:
             MpegInfoEvent()
   {
      m_type = INFO_MPEGInfo;
   }
   MpegInfoEvent(int32 tf, float spf, int32 bpf, int32 br, int32 sr, int32 layer, int32 mpeg, int32 chans, int32 orig, int32 copy, int32 emph, int32 stereo, int32 mode_ext)
   {
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
   int32     GetTotalFrames()
   {
      return m_totalFrames;
   }
   float     GetSecondsPerFrame()
   {
      return m_secondsPerFrame;
   }
   int32     GetBytesPerFrame()
   {
      return m_bytesPerFrame;
   }
   int32     GetBitRate()
   {
      return m_bitrate;
   }
   int32     GetSampleRate()
   {
      return m_sampleRate;
   }
   int32     GetLayer()
   {
      return m_layer;
   }
   int32     GetMpegVersion()
   {
      return m_mpeg;
   }
   int32     GetChannels()
   {
      return m_channels;
   }
   int32     GetOriginal()
   {
      return m_original;
   }
   int32     GetCopyright()
   {
      return m_copyright;
   }
   int32     GetEmphasis()
   {
      return m_emphasis;
   }
   int32     GetStereo()
   {
      return m_stereo;
   }
   int32     GetModeExt()
   {
      return m_modeExt;
   }
   int32     GetCRC()
   {
      return m_crc;
   }
   virtual ~MpegInfoEvent()
   {
   }

};

#define _EQUALIZER_ENABLE_
#ifdef  _EQUALIZER_ENABLE_
class     SetEqualizerDataEvent:public Event
{
   private:
   float    *m_eq;
   bool      m_enable;
   bool      m_IsEQData;
             public:
             SetEqualizerDataEvent(bool enable)
   {
      m_type = CMD_SetEQData;
      m_enable = enable;
      m_IsEQData = false;
   }
   SetEqualizerDataEvent(float *eq)
   {
      m_type = CMD_SetEQData;
      m_eq = eq;
      m_IsEQData = true;
   }
   float    *GetEQData()
   {
      return m_eq;
   }
   bool      IsEQData()
   {
      return m_IsEQData;
   }
   bool      GetEnableState()
   {
      return m_enable;
   }
   virtual ~ SetEqualizerDataEvent()
   {
   }
};
#endif // _EQUALIZER_ENABLE_
#undef  _EQUALIZER_ENABLE_

#define _VISUAL_ENABLE_
#ifdef  _VISUAL_ENABLE_
class     SendVisBufEvent:public Event
{
   private:
   int32 m_length;
   unsigned char *m_buf;
             public:
             SendVisBufEvent(int32 & wrote, void *pBuffer, int32 length)
   {
      m_type = CMD_SendVisBuf;
      m_buf = new unsigned char[length];
                m_length = length;
                memcpy(m_buf, pBuffer, length);
                wrote = length;
   }
   unsigned char *GetBuf()
   {
      return m_buf;
   }
   int32     GetLength()
   {
      return m_length;
   }
   virtual ~ SendVisBufEvent()
   {
      delete    m_buf;
   }
};
#endif // _VISUAL_ENABLE_
#undef  _VISUAL_ENABLE_

class     StreamBufferEvent:public Event
{
   private:
   bool      m_bBufferingUp;
   int32     m_iInputPercent, m_iOutputPercent;
   public:

   StreamBufferEvent(bool bBufferingUp, int32 iInputPercent, int iOutputPercent)
   {
      m_type = INFO_BufferStatus;
      m_bBufferingUp = bBufferingUp;
      m_iInputPercent = iInputPercent;
      m_iOutputPercent = iOutputPercent;
   };

   virtual ~ StreamBufferEvent()
   {
   };

   bool      IsBufferingUp()
   {
      return m_bBufferingUp;
   };
   int32     GetInputPercent()
   {
      return m_iInputPercent;
   };
   int32     GetOutputPercent()
   {
      return m_iOutputPercent;
   };
};

class     StreamInfoEvent:public Event
{
   

public:

   char     *m_streamURL;
   char     *m_streamTitle;

             StreamInfoEvent(char *szTitle, char *szURL)
   {
      m_type = INFO_StreamInfo;
      m_streamURL = strdup(szURL);
      m_streamTitle = strdup(szTitle);
   };

   virtual ~ StreamInfoEvent()
   {
      free(m_streamURL);
      free(m_streamTitle);
   };

   void      GetURL(char *szUrl, int iSize)
   {
      strncpy(szUrl, m_streamURL, iSize - 1);
      szUrl[iSize - 1] = 0;
   }
   void      GetTitle(char *szTitle, int iSize)
   {
      strncpy(szTitle, m_streamTitle, iSize - 1);
      szTitle[iSize - 1] = 0;
   }
};

class PlaylistItemAddedEvent : public Event {
private:
	const PlaylistItem* m_item;
public:
	PlaylistItemAddedEvent(const PlaylistItem* item) 
    { m_type = INFO_PlaylistItemAdded; m_item = item; }
	virtual ~PlaylistItemAddedEvent() {}

	const PlaylistItem* Item() { return m_item; }
};

class PlaylistItemRemovedEvent : public Event {
private:
	const PlaylistItem* m_item;
public:
	PlaylistItemRemovedEvent(const PlaylistItem* item) 
    { m_type = INFO_PlaylistItemRemoved; m_item = item; }
	virtual ~PlaylistItemRemovedEvent() {}

	const PlaylistItem* Item() { return m_item; }
};

class PlaylistItemUpdatedEvent : public Event {
private:
	const PlaylistItem* m_item;
public:
	PlaylistItemUpdatedEvent(const PlaylistItem* item) 
    { m_type = INFO_PlaylistItemUpdated; m_item = item; }
	virtual ~PlaylistItemUpdatedEvent() {}

	const PlaylistItem* Item() { return m_item; }
};

class PlaylistCurrentItemInfoEvent : public Event {
private:
	const PlaylistItem* m_item;
public:
	PlaylistCurrentItemInfoEvent(const PlaylistItem* item) 
    { m_type = INFO_PlaylistCurrentItemInfo; m_item = item; }
	virtual ~PlaylistCurrentItemInfoEvent() {}

	const PlaylistItem* Item() { return m_item; }
};


class PlaylistRepeatEvent : public Event {
private:
	RepeatMode m_rm;
public:
	PlaylistRepeatEvent(RepeatMode rm) 
    { m_type = INFO_PlaylistRepeat; m_rm = rm; }
	virtual ~PlaylistRepeatEvent() {}

	RepeatMode GetRepeatMode() { return m_rm; }
};

class PlaylistShuffleEvent : public Event {
private:
	bool m_sm;
public:
	PlaylistShuffleEvent(bool sm) 
    { m_type = INFO_PlaylistShuffle; m_sm = sm; }
	virtual ~PlaylistShuffleEvent() {}

	bool GetShuffleMode() { return m_sm; }
};

class DownloadItem;

class DownloadItemAddedEvent : public Event {
public:
    DownloadItemAddedEvent(DownloadItem* item) 
    { m_type = INFO_DownloadItemAdded; m_item = item; }
	virtual ~DownloadItemAddedEvent() {}

	DownloadItem* Item() { return m_item; }
private:
    DownloadItem* m_item;
};

class DownloadItemRemovedEvent : public Event {
public:
    DownloadItemRemovedEvent(DownloadItem* item) 
    { m_type = INFO_DownloadItemRemoved; m_item = item; }
	virtual ~DownloadItemRemovedEvent() {}

	DownloadItem* Item() { return m_item; }
private:
    DownloadItem* m_item;
};

class DownloadItemNewStateEvent : public Event {
public:
    DownloadItemNewStateEvent(DownloadItem* item) 
    { m_type = INFO_DownloadItemNewState; m_item = item; }
	virtual ~DownloadItemNewStateEvent() {}

	DownloadItem* Item() { return m_item; }
private:
    DownloadItem* m_item;
};

class DownloadItemProgressEvent : public Event {
public:
    DownloadItemProgressEvent(DownloadItem* item) 
    { m_type = INFO_DownloadItemProgress; m_item = item; }
	virtual ~DownloadItemProgressEvent() {}

	DownloadItem* Item() { return m_item; }
private:
    DownloadItem* m_item;
};

#endif /* _EVENTDATA_H_ */
