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

// Mpg123UI.h


#ifndef _Mpg123UI_H_
#define _Mpg123UI_H_

#include "ui.h"
#include "event.h"
#include "eventdata.h"
#include "playlist.h"

class FAContext;

class Mpg123UI : public UserInterface {
 public:
    Mpg123UI(FAContext *context);

    virtual int32 AcceptEvent(Event *);
    virtual void SetArgs(int argc, char **argv);
    virtual void SetTarget(EventQueue *);
    virtual Error Init(int32);
    virtual void SetPlayListManager(PlayListManager *);
    
    ~Mpg123UI();

    static EventQueue *m_playerEQ;
   virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }

 protected:
    FAContext *m_context;

 private:
    Properties *m_propManager;
    int32 m_argc;
    char **m_argv;
    int32 m_startupType;
    void ProcessArgs();

    PlayListManager *m_plm;
    void DisplayStuff();
    MediaInfoEvent m_mediaInfo;
    bool m_mediaInfo_set;
    MpegInfoEvent m_mpegInfo;
    bool m_mpegInfo_set;
    Id3TagInfo m_id3Tag;
    bool verboseMode;
    int32 totalFrames;
    float totalTime;
    int32 skipFirst;
    char fileName[512];
    float lastSeconds;
};


#endif // _Mpg123UI_H_
