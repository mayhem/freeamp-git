/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1998 GoodNoise

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


#ifndef _SOUNDCARDPMO_H_
#define _SOUNDCARDPMO_H_

/* system headers */
#include <stdlib.h>

/* project headers */
#include <config.h>
#include "pmo.h"


#define BIT_SELECT  0x1f
#define SLEEPTIME   256

static const uint32 OBUFFERSIZE = 2 * 1152;

enum {
    pmoError_MinimumError = 4000,
    pmoError_DeviceOpenFailed,
    pmoError_IOCTL_F_GETFL,
    pmoError_IOCTL_F_SETFL,
    pmoError_IOCTL_SNDCTL_DSP_RESET,
    pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE,
    pmoError_IOCTL_SNDCTL_DSP_STEREO,
    pmoError_IOCTL_SNDCTL_DSP_SPEED,
    pmoError_MaximumError
};

class SoundCardPMO : public PhysicalMediaOutput{

public:
    SoundCardPMO();
    virtual ~SoundCardPMO();
    
    virtual Error Init(OutputInfo* info);
    virtual Error Reset(bool user_stop);
    virtual Error Write(int32 &,void *,int32);
    virtual Error Pause();
    virtual Error Resume();
    virtual const char *GetErrorString(int32);
   virtual Error SetPropManager(Properties *p) { m_propManager = p; if (p) return kError_NoErr; else return kError_UnknownErr; }
 private:
   Properties *m_propManager;
    bool m_properlyInitialized;
    int16 buffer[OBUFFERSIZE];
    int16 *bufferp[MAXCHANNELS];
    uint32 channels;
    static int audio_fd;
    OutputInfo *myInfo;
    int32 getprocessed(void);
    
};

#endif /* _SOUNDCARDPMO_H_ */



