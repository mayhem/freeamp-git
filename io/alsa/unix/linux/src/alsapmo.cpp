/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player
	Driver for Advanced Linux Sound Architecture, http://alsa.jcu.cz
 
	Portions Copyright (C) 1998 GoodNoise

	alsapmo.cpp by Fleischer Gabor <flocsy@usa.net>
	uses code by Anders Semb Hermansen <ahermans@vf.telia.no>
	and Jaroslav Kysela <perex@jcu.cz>

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

 *  You can use -a <soundcard #>:<device #>...
 *  For example: mpg123 -a 1:0 aaa.mpg
 *               mpg123 -a guspnp:1 aaa.mpg

____________________________________________________________________________*/

/*
To the developers:
I have left a lot of "#ifdef DEBUG" parts in to make it easier to debug.
You should put "#define DEBUG" either here or into alsapmo.h.

I also left the code from soundcardpmo. They are not needed, just for fun.
You will find them in the "#else" part of "#ifdef ALSA".
*/

/* project headers */
#include <config.h>
#include "alsapmo.h"

/* system headers */
#include <stdlib.h>
#include <iostream.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#ifdef ALSA
#include <sys/asoundlib.h>
#include <ctype.h>
#else
#include <sys/soundcard.h>
#endif

#include <errno.h>
#include <string.h>

#define PIECES 50

extern "C" {
PhysicalMediaOutput *Initialize() {
    return new AlsaPMO();
}
	   }

static char * g_ErrorArray[pmoError_MaximumError] = {
    "Invalid Error Code",
    "dsp device open failed",
    "fcntl F_GETFL on /dev/dsp failed",
    "fcntl F_SETFL on /dev/dsp failed"
    "ioctl reset failed",
    "config of samplesize failed",
    "config of stereo failed",
    "config of speed failed"
#ifdef ALSA
    ,
    "wrong card number",
    "wrong device number",
    "getting playback info",
    "getting playback params",
    "alsa device close failed"
#endif
};

const char *AlsaPMO::GetErrorString(int32 error) {
    if ((error <= pmoError_MinimumError) || (error >= pmoError_MaximumError)) {
	return g_ErrorArray[0];
    }
    return g_ErrorArray[error - pmoError_MinimumError];
}

AlsaPMO::AlsaPMO() {
#ifdef DEBUG
    cout << "Creating scpmo" << endl;
#endif
    m_properlyInitialized = false;
    myInfo = new OutputInfo();

#ifdef ALSA
    ai = new struct audio_info_struct;
    ai->rate = -1;
//   ai->gain = -1;
//   ai->output = -1;
    ai->handle = NULL;
    ai->alsa_format.format = (unsigned)-1;
    ai->alsa_format.rate = (unsigned)-1;
    ai->alsa_format.channels = (unsigned)-1;
    ai->device = NULL;
    ai->channels = -1;
    ai->format = -1;

    // ai->device=strdup(ALSA_DEVICE);  // JDW - moved to Init so we can grabp the ALSA-device Property
#endif

#ifdef DEBUG
    cout << "Done creation of the AlsaPMO..." << endl;
#endif
}

AlsaPMO::~AlsaPMO() {
#ifdef DEBUG
    cout << "Deleting scpmo... " << endl;
#endif
    Reset(false);
#ifdef ALSA
    int err;
    if ((err=snd_pcm_close(ai->handle)) < 0) {
#ifdef DEBUG
	    cout<<"close failed: "<<snd_strerror(err)<<endl;
#endif
	    (Error)pmoError_ALSA_DeviceCloseFailed;
    }
#else
    close(audio_fd);
#endif // ALSA
    if (myInfo) {
	delete myInfo;
	myInfo = NULL;
    }
#ifdef ALSA
    if (ai) {
	delete ai;
	ai = NULL;
    }
#endif
#ifdef DEBUG
    cout << "Done deleting scpmo" << endl;
#endif
}

int AlsaPMO::audio_fd = -1;

Error AlsaPMO::Pause() {
//cout<<"Pause"<<endl;
    return Reset(true);
}

Error AlsaPMO::Resume() {
    return kError_NoErr;
}

Error AlsaPMO::Init(OutputInfo* info) {
#ifdef ALSA
    int card=0,device=0;
    char scard[128], sdevice[128];
    int err;
#endif

#ifdef DEBUG
    cout << "initialize..." << endl;
    if (info) {
        cout << "OI: bits_per_sample: " << info->bits_per_sample << endl;
        cout << "OI: number_of_channels: " << info->number_of_channels << endl;
        cout << "OI: samples_per_second: " << info->samples_per_second << endl;
        cout << "OI: max_buffer_size: " << info->max_buffer_size << endl;
    }
#endif

    m_properlyInitialized = false;
    if (!info) {
	info = myInfo;
    } else {
	// got info, so this is the beginning...
#ifdef ALSA

	PropValue *pv = NULL;
	m_propManager->GetProperty("ALSA-device",&pv);
	if (pv) {
	    const char *pChar = ((StringPropValue *)pv)->GetString();
	    if (pChar) {
		ai->device = strdup(pChar);
	    } else {
		ai->device=strdup(ALSA_DEVICE);
	    }
	} else {
	    ai->device=strdup(ALSA_DEVICE);  // use default
	}

	if(ai->device) {	/* parse ALSA device name */
#ifdef DEBUG
cout<<"device="<<ai->device<<endl;
#endif
		if(strchr(ai->device,':')) {	/* card with device */
			strncpy(scard, ai->device, sizeof(scard)-1);
			scard[sizeof(scard)-1] = '\0';
			if (strchr(scard,':')) *strchr(scard,':') = '\0';
			card = snd_card_name(scard);
			if (card < 0) {
#ifdef DEBUG
				cerr<<"wrong Alsa number: "<<scard<<endl;
#endif
				return (Error)pmoError_ALSA_CardNumber;
			}
			strncpy(sdevice, strchr(ai->device, ':') + 1, sizeof(sdevice)-1);
		} else {
			strncpy(sdevice, ai->device, sizeof(sdevice)-1);
		}
		sdevice[sizeof(sdevice)-1] = '\0';
		device = atoi(sdevice);
		if (!isdigit(sdevice[0]) || device < 0 || device > 31) {
#ifdef DEBUG
			cerr<<"wrong device number: "<<sdevice<<endl;
#endif
			return (Error)pmoError_ALSA_DeviceNumber;
		}
	}
	if((err=snd_pcm_open(&ai->handle, card, device, SND_PCM_OPEN_PLAYBACK)) < 0 )
	{
#ifdef DEBUG
	    cout<<"open failed: "<<snd_strerror(err)<<endl;
#endif
	    return (Error)pmoError_DeviceOpenFailed;
	}

#else //ALSA

	if ((audio_fd = open("/dev/dsp", O_WRONLY | O_SYNC, 0)) < 0) {
	    if (errno == EBUSY) {
#ifdef DEBUG
		cerr << "Audio device busy!" << endl;
#endif
		return (Error)pmoError_DeviceOpenFailed;
	    } else {
#ifdef DEBUG
		cout << "Can't open /dev/dsp for writing" << endl;
#endif
		return (Error)pmoError_DeviceOpenFailed;
	    }
	}
#endif
    }

#ifdef ALSA
    channels = info->number_of_channels;
    for (int i = 0; i < info->number_of_channels; ++i)
        bufferp[i] = buffer + i;
    
    // configure the device:
    ai->format=16;
    ai->channels=channels;
    ai->rate=info->samples_per_second;

//cout<<"Before all"<<endl;
//	audio_set_format(ai);
//cout<<"Before channels"<<endl;
//	audio_set_channels(ai);
//cout<<"Before rate"<<endl;
//	audio_set_rate(ai);
    audio_set_all(ai);
//cout<<"After all"<<endl;

#else // ALSA

    int fd = audio_fd;
    int flags;
    if ((flags = fcntl(fd,F_GETFL,0)) < 0) {
	return (Error)pmoError_IOCTL_F_GETFL;
    }

    flags &= O_NDELAY; //SYNC;
    
    if (fcntl(fd, F_SETFL, flags) < 0) {
	return (Error)pmoError_IOCTL_F_SETFL;
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
	return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
    } 
    
    if(ioctl(audio_fd, SNDCTL_DSP_SAMPLESIZE, &play_precision) == -1) {
	return (Error)pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE;
    }

    if(ioctl(audio_fd, SNDCTL_DSP_STEREO, &play_stereo) == -1) {
	return (Error)pmoError_IOCTL_SNDCTL_DSP_STEREO;
    }
    if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &play_sample_rate) == -1) {
	return (Error)pmoError_IOCTL_SNDCTL_DSP_SPEED;
    }
#endif

    myInfo->bits_per_sample = info->bits_per_sample;
    myInfo->number_of_channels = info->number_of_channels;
    myInfo->samples_per_second = info->samples_per_second;
    myInfo->max_buffer_size = info->max_buffer_size;
    m_properlyInitialized = true;
    return kError_NoErr;
}

Error AlsaPMO::Reset(bool user_stop) {
#ifdef DEBUG
    cout << "Reset..." << endl;
#endif

#ifdef ALSA
//    audio_set_format(ai);
//    audio_set_channels(ai);
//    audio_set_rate(ai);
///    audio_set_all(ai);
#else
    int a;
    if (ioctl(audio_fd,SNDCTL_DSP_RESET,&a) == -1) {
	return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
    }
#endif

    if (user_stop) {
	Init(NULL);
    }
    return kError_NoErr;
}

Error AlsaPMO::Write(int32&rtn,void *pBuffer,int32 length) {
    int32 actualThisTime = 0;
    int32 writeBlockLength = length / PIECES;
#ifdef DEBUG
//    cout << "length: " << length << endl;
//    cout << "writeBl" << endl;
#endif
    void *pv = NULL;
    int32 gp = 0;
#ifdef ALSA
    rtn=snd_pcm_write(ai->handle,pBuffer,length);
#else
    do {
	rtn = write(audio_fd,pBuffer,length);
    } while ((rtn != length) && (errno == EINTR));
#endif

    if(rtn != length) {
//#ifdef DEBUG
	cerr << "Wrote " << length << ", and got " << rtn << " back" << endl;
	cerr << "errno: " << errno << endl << "str: " << strerror(errno) << endl;
//#endif
	rtn = -1;
	return kError_OutputUnsuccessful;
    }
    return kError_NoErr;
}

#ifdef ALSA
/*
int AlsaPMO::audio_set_playback_params2(struct audio_info_struct *ai)
{
	int err;
	snd_pcm_playback_info_t pi;
	snd_pcm_playback_params_t pp;

#ifdef DEBUG
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv,&tz);
	cout<<"Before allA"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
	if((err=snd_pcm_playback_info(ai->handle, &pi)) < 0 )
	{
#ifdef DEBUG
		cerr<<"audio_set_playback_params: playback info failed: "<<snd_strerror(err)<<endl;
#endif
		return (Error)pmoError_ALSA_Playback_Info;
	}
#ifdef DEBUG
	gettimeofday(&tv,&tz);
	cout<<"Before allB"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif

	bzero(&pp, sizeof(pp));
	pp.fragment_size = pi.buffer_size/4;
	if (pp.fragment_size > pi.max_fragment_size) pp.fragment_size = pi.max_fragment_size;
	if (pp.fragment_size < pi.min_fragment_size) pp.fragment_size = pi.min_fragment_size;
	pp.fragments_max = -1;
	pp.fragments_room = 1;

	if((err=snd_pcm_playback_params(ai->handle, &pp)) < 0 )
	{
#ifdef DEBUG
		cerr<<"audio_set_playback_params: playback params failed: "<<snd_strerror(err)<<endl;
#endif
		return (Error)pmoError_ALSA_Playback_Params;
	}
#ifdef DEBUG
	gettimeofday(&tv,&tz);
	cout<<"Before allC"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
	return 0;
}

int AlsaPMO::audio_set_format(struct audio_info_struct *ai)
{
	int ret;

	if(ai->format == -1)
		return 0;
/*
	switch(ai->format)
	{
		case AUDIO_FORMAT_SIGNED_16:
		default:
			ai->alsa_format.format=SND_PCM_SFMT_S16_LE;
			break;
		case AUDIO_FORMAT_UNSIGNED_8:
			ai->alsa_format.format=SND_PCM_SFMT_U8;
			break;
		case AUDIO_FORMAT_SIGNED_8:
			ai->alsa_format.format=SND_PCM_SFMT_S8;
			break;
		case AUDIO_FORMAT_ULAW_8:
			ai->alsa_format.format=SND_PCM_SFMT_MU_LAW;
			break;
		case AUDIO_FORMAT_ALAW_8:
			ai->alsa_format.format=SND_PCM_SFMT_A_LAW;
			break;
		case AUDIO_FORMAT_UNSIGNED_16:
			ai->alsa_format.format=SND_PCM_SFMT_U16_LE;
			break;
	}
//
	ai->alsa_format.format=SND_PCM_SFMT_S16_LE;

	if((ret=snd_pcm_playback_format(ai->handle, &ai->alsa_format)) < 0 ) {
#ifdef DEBUG
	    cout<<"audio_set_format failed: "<<snd_strerror(ret)<<endl;
#endif
	    return (Error)pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE;
	}
	audio_set_playback_params(ai);
	return 0;
}

int AlsaPMO::audio_set_rate(struct audio_info_struct *ai)
{
	int ret;

	if(!ai || ai->rate < 0)
		return -1;

	ai->alsa_format.rate=ai->rate;
	if((ret=snd_pcm_playback_format(ai->handle, &ai->alsa_format)) < 0 ) {
#ifdef DEBUG
	    cout<<"audio_set_rate failed: "<<snd_strerror(ret)<<endl;
#endif
	    return (Error)pmoError_IOCTL_SNDCTL_DSP_SPEED;
	}
	audio_set_playback_params(ai);
	return 0;
}

int AlsaPMO::audio_set_channels(struct audio_info_struct *ai)
{
	int ret;

	if(ai->alsa_format.channels < 0)
		return 0;

	ai->alsa_format.channels = ai->channels;
	if((ret=snd_pcm_playback_format(ai->handle, &ai->alsa_format)) < 0 ) {
#ifdef DEBUG
	    cout<<"audio_set_channels failed: "<<snd_strerror(ret)<<endl;
#endif
	    return (Error)pmoError_IOCTL_SNDCTL_DSP_STEREO;
	}
	audio_set_playback_params(ai);
	return 0;
}
*/

int AlsaPMO::audio_set_all(struct audio_info_struct *ai)
{
	int err;

	if(ai->format == -1)
		return 0;
	if(!ai || ai->rate < 0)
		return -1;
	if(ai->alsa_format.channels < 0)
		return 0;

	switch(ai->format)
	{
		case AUDIO_FORMAT_SIGNED_16:
		default:
			ai->alsa_format.format=SND_PCM_SFMT_S16_LE;
			break;
		case AUDIO_FORMAT_UNSIGNED_8:
			ai->alsa_format.format=SND_PCM_SFMT_U8;
			break;
		case AUDIO_FORMAT_SIGNED_8:
			ai->alsa_format.format=SND_PCM_SFMT_S8;
			break;
		case AUDIO_FORMAT_ULAW_8:
			ai->alsa_format.format=SND_PCM_SFMT_MU_LAW;
			break;
		case AUDIO_FORMAT_ALAW_8:
			ai->alsa_format.format=SND_PCM_SFMT_A_LAW;
			break;
		case AUDIO_FORMAT_UNSIGNED_16:
			ai->alsa_format.format=SND_PCM_SFMT_U16_LE;
			break;
	}
//	ai->alsa_format.format=SND_PCM_SFMT_S16_LE;
	ai->alsa_format.rate=ai->rate;
	ai->alsa_format.channels = ai->channels;

#ifdef DEBUG
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv,&tz);
	cout<<"Before all2"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
	if((err=snd_pcm_playback_format(ai->handle, &ai->alsa_format)) < 0 ) {
#ifdef DEBUG
	    cout<<"audio_set_all failed: "<<snd_strerror(err)<<endl;
#endif
	    return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
	}
#ifdef DEBUG
	gettimeofday(&tv,&tz);
	cout<<"Before all3"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
//	audio_set_playback_params2(ai);
//	int err;
	snd_pcm_playback_info_t pi;
	snd_pcm_playback_params_t pp;

#ifdef DEBUG
//	struct timeval tv;
//	struct timezone tz;
	gettimeofday(&tv,&tz);
	cout<<"Before allA"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
	if((err=snd_pcm_playback_info(ai->handle, &pi)) < 0 )
	{
#ifdef DEBUG
		cerr<<"audio_set_playback_params: playback info failed: "<<snd_strerror(err)<<endl;
#endif
		return (Error)pmoError_ALSA_Playback_Info;
	}
#ifdef DEBUG
	gettimeofday(&tv,&tz);
	cout<<"Before allB"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif

	bzero(&pp, sizeof(pp));
	pp.fragment_size = pi.buffer_size/4;
	if (pp.fragment_size > pi.max_fragment_size) pp.fragment_size = pi.max_fragment_size;
	if (pp.fragment_size < pi.min_fragment_size) pp.fragment_size = pi.min_fragment_size;
	pp.fragments_max = -1;
	pp.fragments_room = 1;

	if((err=snd_pcm_playback_params(ai->handle, &pp)) < 0 )
	{
#ifdef DEBUG
		cerr<<"audio_set_playback_params: playback params failed: "<<snd_strerror(err)<<endl;
#endif
		return (Error)pmoError_ALSA_Playback_Params;
	}
#ifdef DEBUG
	gettimeofday(&tv,&tz);
	cout<<"Before allC"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
//	return 0;
#ifdef DEBUG
	gettimeofday(&tv,&tz);
	cout<<"Before all4"<<tv.tv_sec<<","<<tv.tv_usec<<endl;
#endif
	return 0;
}
#endif
