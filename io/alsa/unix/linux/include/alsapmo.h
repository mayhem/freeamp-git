/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Portions copyright (C) 1998-1999 EMusic.com

	alsapmo.h by Gabor Fleischer <flocsy@usa.net>

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


#ifndef INCLUDED_ALSAPMO_H_
#define INCLUDED_ALSAPMO_H_

/* system headers */
#include <stdlib.h>

#include <sys/asoundlib.h>

#define AUDIO_FORMAT_SIGNED_16    0x110
#define AUDIO_FORMAT_UNSIGNED_16  0x120
#define AUDIO_FORMAT_UNSIGNED_8   0x1
#define AUDIO_FORMAT_SIGNED_8     0x2
#define AUDIO_FORMAT_ULAW_8       0x4
#define AUDIO_FORMAT_ALAW_8       0x8

/* project headers */
#include <config.h>
#include "thread.h"
#include "mutex.h"
#include "pmo.h"
#include "pmoevent.h"
#include "eventbuffer.h"

struct audio_info_struct
{
    snd_pcm_t *handle;
    snd_pcm_format_t alsa_format;

    char *device;
    int format;
    int channels;
    long rate;
    void * mixer_handle;
    snd_mixer_channel_t channel;
};

#define BIT_SELECT  0x1f
#define SLEEPTIME   256

static const uint32 OBUFFERSIZE = 2 * 1152;

enum {
    pmoError_MinimumError = 0x00010000,
    pmoError_DeviceOpenFailed,
    pmoError_IOCTL_F_GETFL,
    pmoError_IOCTL_F_SETFL,
    pmoError_IOCTL_SNDCTL_DSP_RESET,
    pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE,
    pmoError_IOCTL_SNDCTL_DSP_STEREO,
    pmoError_IOCTL_SNDCTL_DSP_SPEED,
    pmoError_ALSA_CardNumber,
    pmoError_ALSA_DeviceNumber,
    pmoError_ALSA_Playback_Info,
    pmoError_ALSA_Playback_Params,
    pmoError_ALSA_DeviceCloseFailed,
    pmoError_ALSA_MixerOpenFailed,
    pmoError_ALSA_Mixer_Info,
    pmoError_MaximumError
};

class AlsaPMO : public PhysicalMediaOutput
{
public:
    AlsaPMO(FAContext *context);
    virtual ~AlsaPMO();
    
    virtual Error Init(OutputInfo* info);
    static void   StartWorkerThread(void *);
    virtual void  Pause(void);
            void  SetVolume(int32);
            int32 GetVolume(void);

 private:

    void          WorkerThread(void); 
    virtual Error Reset(bool user_stop);
    void          HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent);
    bool          WaitForDrain(void);  

    bool	         m_properlyInitialized;
    uint32	      channels;
#ifdef SOUNDCARD
    static int audio_fd;
#endif
    OutputInfo  *myInfo;
    Thread      *m_pBufferThread;
    int          m_iOutputBufferSize, m_iBytesPerSample;
    int          m_iTotalFragments, m_iBaseTime;
    int          m_iDataSize;
    int          m_iCard, m_iDevice;

    int		 m_iChannel;
    snd_mixer_gid_t m_gid;
    snd_mixer_group_t m_group;
 
    struct audio_info_struct *ai;
    int audio_set_all(struct audio_info_struct *);
};

#endif /* _ALSAPMO_H_ */
