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

/* system headers */
#include <stdlib.h>
#include <iostream.h>
#include <sys/asoundlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

/* project headers */
#include <config.h>
#include "alsapmo.h"
#include "facontext.h"
#include "log.h"

#define DB printf("%s:%d\n", __FILE__, __LINE__);

const int iDefaultBufferSize = 512 * 1024;
const int iOrigBufferSize = 64 * 1024;
const int iOverflowSize = 0;
const int iWriteTriggerSize = 8 * 1024;

extern "C"
{
   PhysicalMediaOutput *Initialize(FAContext *context) {
      return new AlsaPMO(context);
   }
}

/*
static char * g_ErrorArray[pmoError_MaximumError] = {
    "Invalid Error Code",
    "dsp device open failed",
    "fcntl F_GETFL on /dev/dsp failed",
    "fcntl F_SETFL on /dev/dsp failed"
    "ioctl reset failed",
    "config of samplesize failed",
    "config of stereo failed",
    "config of speed failed",

    "wrong card number",
    "wrong device number",
    "getting playback info",
    "getting playback params",
    "alsa device close failed"
};

const char *AlsaPMO::GetErrorString(int32 error) {
    if ((error <= pmoError_MinimumError) || (error >= pmoError_MaximumError)) {
        return g_ErrorArray[0];
    }
    return g_ErrorArray[error - pmoError_MinimumError];
}
*/

AlsaPMO::AlsaPMO(FAContext *context) :
        EventBuffer(iOrigBufferSize, iOverflowSize,
		    iWriteTriggerSize, context)
{
   m_properlyInitialized = false;
   myInfo = new OutputInfo();
   memset(myInfo, 0, sizeof(OutputInfo));

   m_pBufferThread = NULL;

   m_pPauseMutex = new Mutex();
   m_iOutputBufferSize = 0;
   m_iTotalBytesWritten = 0;
   m_iBytesPerSample = 0;
   m_iLastFrame = -1;
   m_iDataSize = 0;

   if (!m_pBufferThread)
   {
      m_pBufferThread = Thread::CreateThread();
      assert(m_pBufferThread);
      m_pBufferThread->Create(AlsaPMO::StartWorkerThread, this);
   }

    ai = new struct audio_info_struct;
//   ai->gain = -1;
//   ai->output = -1;
    ai->handle = NULL;
    ai->alsa_format.format = (unsigned)-1;
    ai->alsa_format.rate = (unsigned)-1;
    ai->alsa_format.channels = (unsigned)-1;
    ai->device = NULL;
    ai->format = -1;
    ai->channels = -1;
    ai->rate = -1;
    ai->mixer_handle=0;
    ai->pcm=-1;
//    ai->mixer_id=SND_MIXER_ID_MASTER;
//    ai->channel;

}

AlsaPMO::~AlsaPMO() 
{
   m_bExit = true;
   m_pWriteSem->Signal();
   m_pReadSem->Signal();
   m_pPauseMutex->Release();

   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
   }

   if (m_pPauseMutex)
   {
      delete m_pPauseMutex;
      m_pPauseMutex = NULL;
   }

    int err;
    if ((err=snd_pcm_close(ai->handle)) < 0) {
            (Error)pmoError_ALSA_DeviceCloseFailed;
    }
    if (myInfo) {
        delete myInfo;
        myInfo = NULL;
    }
    if (ai) {
        delete ai;
        ai = NULL;
    }
}

void AlsaPMO::WaitToQuit()
{
   if (m_pBufferThread)
   {
      m_pBufferThread->Join();
      delete m_pBufferThread;
      m_pBufferThread = NULL;
   }
}

Error AlsaPMO::SetPropManager(Properties * p)
{
   m_propManager = p;
   return kError_NoErr;
}

/*
int AlsaPMO::OpenMixer()
{
   int err;
   err = snd_mixer_open(&ai->mixer_handle,ai->card,ai->device);
   if (err < 0) {
#ifdef DEBUG
cout<<"snd_mixer_open err"<<endl;
#endif
      return -1;
   }
//in fact i think the next step is not needed all time, just at the first
//to get the channel into info. This could be done in constructor of
//VolumeManager.
   err = snd_mixer_info( ai->mixer_handle, &info );
   if (err < 0) {
#ifdef DEBUG
cout<<"snd_mixer_info err"<<endl;
#endif
      snd_mixer_close( mixer_handle );
      return -1;
   }
   pcm = snd_mixer_channel( mixer_handle, mixer_id );
   return 0;
}
*/

int AlsaPMO::GetVolume()
{
   int volume = 0;
   int err;
//   OpenMixer();
   if (ai->pcm >= 0) {
      err = snd_mixer_channel_read(ai->mixer_handle,ai->pcm,&ai->channel);
      if (err < 0) {
         snd_mixer_close(ai->mixer_handle);
         return volume;
      }
      volume=ai->channel.left;
   }
//   snd_mixer_close(mixer_handle);
   return volume;   
}

void AlsaPMO::SetVolume(int32 iVolume)
{
   int err;
//   OpenMixer();
   if (ai->pcm >= 0) {
      err = snd_mixer_channel_read(ai->mixer_handle,ai->pcm,&ai->channel);
      if (err < 0) {
         snd_mixer_close(ai->mixer_handle);
         return;
      }
      ai->channel.left=ai->channel.right=iVolume;
      err = snd_mixer_channel_write(ai->mixer_handle,ai->pcm,&ai->channel);
      if (err < 0) {
         snd_mixer_close(ai->mixer_handle);
         return;
      }
   }
//   snd_mixer_close(mixer_handle);
}

int AlsaPMO::GetBufferPercentage()
{
   return PullBuffer::GetBufferPercentage();
}

Error AlsaPMO::Pause() 
{
   m_pPauseMutex->Acquire();
//   if (m_properlyInitialized)
//       Reset(true);
}

Error AlsaPMO::Resume() {
   m_pPauseMutex->Release();
    return kError_NoErr;
}

Error AlsaPMO::Break()
{
   m_bExit = true;
   PullBuffer::BreakBlocks();

   return kError_NoErr;
}

Error AlsaPMO::Init(OutputInfo* info) {
    int card=0,device=0;
    char scard[128], sdevice[128];
    int err;
    char mixer_id[13]=SND_MIXER_ID_MASTER;

    m_properlyInitialized = false;
    if (!info) {
        info = myInfo;
    } else {
        // got info, so this is the beginning...
        PropValue *pv = NULL, *pProp;
        int32      iNewSize = iDefaultBufferSize;
        Error      result;
	uint32	   deviceNameSize = 128;

	ai->device = (char *) malloc(deviceNameSize);
	m_context->prefs->GetPrefString(kALSADevicePref, ai->device,
					&deviceNameSize);

   m_iDataSize = info->max_buffer_size;
	m_context->prefs->GetOutputBufferSize(&iNewSize);
	iNewSize *= 1024;

	// cerr << "Using ALSA device: " << ai->device << endl;
	// cerr << "Using output buffer size: " << iNewSize << endl;

        iNewSize -= iNewSize % m_iDataSize;
        result = Resize(iNewSize, 0, m_iDataSize);
        if (IsError(result))
        {
           ReportError("Internal buffer sizing error occurred.");
           m_context->log->Error("Resize output buffer failed.");
           return result;
        }

        if (ai->device)
        {
            if (sscanf(ai->device, "%[^:]: %d", scard, &device) != 2)
            {
               ReportError("The ALSADevice statement in the preference file"
                           "is improperly formatted. Format: ALSADevice: "
                           "[card name/card number]:[device number]");
               return (Error)pmoError_ALSA_CardNumber;
            }

            card = snd_card_name(scard);
            if (card < 0) 
            {
                ReportError("Invalid ALSA card name/number specified.");
                return (Error)pmoError_ALSA_CardNumber;
            }
            if (device < 0 || device > 31) 
            {
                ReportError("Invalid ALSA device number specified.");
                return (Error)pmoError_ALSA_DeviceNumber;
            }
        }
        if((err=snd_pcm_open(&ai->handle, card, device, SND_PCM_OPEN_PLAYBACK)) < 0 )
        {
            ReportError("Audio device is busy. Please make sure that "
                        "another program is not using the device.");
//            ReportError("Cannot open audio device. Please make sure that "
//                        "the audio device is properly configured.");
            return (Error)pmoError_DeviceOpenFailed;
        }
#define SND_PCM_BLOCK_MODE_NON_BLOCKING         (0)
#define SND_PCM_BLOCK_MODE_BLOCKING             (1)
        snd_pcm_block_mode(ai->handle,SND_PCM_BLOCK_MODE_NON_BLOCKING);
        if ((err=snd_pcm_playback_time(ai->handle,1))!=0) {
                ReportError("Can't enable playback time.");
            return (Error)pmoError_DeviceOpenFailed;
        }
//Here comes the VolumeManager stuff's setup
        switch (device) {
            case 0 : strncpy(mixer_id,SND_MIXER_ID_PCM,sizeof(mixer_id));break;
            case 1 : strncpy(mixer_id,SND_MIXER_ID_PCM1,sizeof(mixer_id));break;
        }
//      err = snd_mixer_open(&ai->mixer_handle,card,device);
        err = snd_mixer_open(&ai->mixer_handle,0,0);
        if (err < 0) {
                ReportError("Cannot open mixer device.");
                return (Error)pmoError_ALSA_MixerOpenFailed;
        }
//in fact i think the next step is not needed all time, just at the first
//to get the channel into info. This could be done in constructor of
//VolumeManager.
        snd_mixer_info_t mixer_info;
        err = snd_mixer_info( ai->mixer_handle, &mixer_info );
        if (err < 0) {
                ReportError("Cannot get mixer info.");
                snd_mixer_close( ai->mixer_handle );
                return (Error)pmoError_ALSA_Mixer_Info;
        }

        ai->pcm = snd_mixer_channel( ai->mixer_handle, mixer_id );
    }

    channels = info->number_of_channels;
    for (int i = 0; i < info->number_of_channels; ++i)
        bufferp[i] = buffer + i;
    
    // configure the device:
    ai->format=16;
    ai->channels=channels;
    ai->rate=info->samples_per_second;

    audio_set_all(ai);

    myInfo->bits_per_sample = info->bits_per_sample;
    myInfo->number_of_channels = info->number_of_channels;
    myInfo->samples_per_second = info->samples_per_second;
    myInfo->max_buffer_size = info->max_buffer_size;

   snd_pcm_playback_status_t aInfo;
   snd_pcm_playback_status(&ai->handle,&aInfo);
   m_iOutputBufferSize = aInfo.fragment_size * aInfo.fragments;
   m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

    m_properlyInitialized = true;
    return kError_NoErr;
}

Error AlsaPMO::Reset(bool user_stop) {

    if (user_stop) {
        Init(NULL);
    }
    return kError_NoErr;
}

Error AlsaPMO::BeginWrite(void *&pBuffer, size_t &iBytesToWrite)
{
   return EventBuffer::BeginWrite(pBuffer, iBytesToWrite);
}

Error AlsaPMO::EndWrite(size_t iNumBytesWritten)
{
   return EventBuffer::EndWrite(iNumBytesWritten);
}

Error AlsaPMO::AcceptEvent(Event *pEvent)
{
   return EventBuffer::AcceptEvent(pEvent);
}

Error AlsaPMO::Clear()
{
   return EventBuffer::Clear();
}

void AlsaPMO::HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent)
{
   MediaTimeInfoEvent *pmtpi;
   int32               hours, minutes, seconds;
   int                 iTotalTime = 0;

   if (pEvent->GetFrameNumber() != m_iLastFrame + 1)
   {
       m_iTotalBytesWritten = 1152 * pEvent->GetFrameNumber() * 
                              m_iBytesPerSample; 
   }
   m_iLastFrame = pEvent->GetFrameNumber();

   if (myInfo->samples_per_second <= 0 || pEvent->GetFrameNumber() < 3)
      return;

   snd_pcm_playback_status_t ainfo;
   snd_pcm_playback_status(&ai->handle,&ainfo);
   iTotalTime = (m_iTotalBytesWritten) /
                (m_iBytesPerSample * myInfo->samples_per_second);

   hours = iTotalTime / 3600;
   minutes = (iTotalTime / 60) % 60;
   seconds = iTotalTime % 60;

   if (hours < 0 || hours > 23 ||
       minutes < 0 || minutes > 59 || 
       seconds < 0 || seconds > 59)
      return;

   pmtpi = new MediaTimeInfoEvent(hours, minutes, seconds, 0, 
                                  iTotalTime, 0);
   m_target->AcceptEvent(pmtpi);
}

void AlsaPMO::StartWorkerThread(void *pVoidBuffer)
{
   ((AlsaPMO*)pVoidBuffer)->WorkerThread();
}

void AlsaPMO::WorkerThread(void)
{
   void                      *pBuffer;
   size_t                     iToCopy, iCopied;
   Error                      eErr;
   size_t                     iRet;
   Event                     *pEvent;
   OutputInfo                *pInfo;
   snd_pcm_playback_status_t  ainfo;

   for(; !m_bExit;)
   {
      if (!m_properlyInitialized)
      {
          pEvent = GetEvent();

          if (pEvent == NULL)
          {
              m_pReadSem->Wait();
              continue;
          }

          if (pEvent->Type() == PMO_Init)
          {
              if (IsError(Init(((PMOInitEvent *)pEvent)->GetInfo())))
              {
                  delete pEvent;
                  break;
              }
          }
          delete pEvent;

          continue;
      }

      iToCopy = m_iDataSize;
      m_pPauseMutex->Acquire();

      eErr = BeginRead(pBuffer, iToCopy);
      if (eErr == kError_InputUnsuccessful || eErr == kError_NoDataAvail)
      {
          m_pPauseMutex->Release();
          m_pReadSem->Wait();
          continue;
      }
      if (iToCopy < m_iDataSize)
      {
          EndRead(0);
          m_pPauseMutex->Release();
          m_pReadSem->Wait();
          continue;
      }
      if (eErr == kError_Interrupt)
      {
          m_pPauseMutex->Release();
          break;
      }
          
      if (eErr == kError_EventPending)
      {
          m_pPauseMutex->Release();

          pEvent = GetEvent();

          if (pEvent->Type() == PMO_Init)
              Init(((PMOInitEvent *)pEvent)->GetInfo());

          if (pEvent->Type() == PMO_Reset)
              Reset(false);

          if (pEvent->Type() == PMO_Info) 
              HandleTimeInfoEvent((PMOTimeInfoEvent *)pEvent);

          if (pEvent->Type() == PMO_Quit) 
          {
              Reset(false);
              delete pEvent;
              break;
          }
 
          delete pEvent;

          continue;
      }
          
      if (IsError(eErr))
      {
          m_pPauseMutex->Release();

          ReportError("Internal error occured.");
          m_context->log->Error("Cannot read from buffer in PMO worker tread: %d\n",
              eErr);
          break;
      }

      for(;;)
      {
          iRet = snd_pcm_playback_status(ai->handle,&ainfo);
          if (ainfo.count < iToCopy)
          {
              EndRead(0);
              m_pPauseMutex->Release();

              usleep(10000);

              for(;!m_bExit;)
                 if (m_pPauseMutex->Acquire(10000))
                     break;
              if (m_bExit)
                 iToCopy = 0;

              // If beginread returns an error, break out...
              if (BeginRead(pBuffer, iToCopy) != kError_NoErr)
                 iToCopy = 0;

              continue;
          }
          break;
      }

      iCopied = snd_pcm_write(ai->handle,pBuffer,iToCopy);
      if (iCopied > 0)
          m_iTotalBytesWritten += iCopied;

      // Was iToCopy set to zero? If so, we got cleared and should
      // start from the top
      if (iToCopy == 0)
      {
         m_pPauseMutex->Release();
         continue;
      }

      if (iCopied < iToCopy)
      {
         EndRead(0);
         m_pPauseMutex->Release();
         ReportError("Could not write sound data to the soundcard.");
         m_context->log->Error("Failed to write to the soundcard\n");
         break;
      }

      EndRead(iCopied);
      m_pPauseMutex->Release();
   }
}

int AlsaPMO::audio_set_all(struct audio_info_struct *ai)
{
        int err;

        if(ai->format == -1) {
#ifdef DEBUG
cout<<"AlsaPMO::audio_set_all:ai->format error"<<endl;
#endif
                return 0;
        }
        if(!ai || ai->rate < 0) {
#ifdef DEBUG
cout<<"AlsaPMO::audio_set_all:ai->rate error"<<endl;
#endif
                return -1;
        }
        if(ai->alsa_format.channels < 0) {
#ifdef DEBUG
cout<<"AlsaPMO::audio_set_all:ai->channels error"<<endl;
#endif
                return 0;
        }
#ifdef DEBUG
cout<<"AlsaPMO::audio_set_all: format="<<ai->format<<",rate="<<ai->rate<<",channels="<<ai->channels<<endl;
#endif

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
//      ai->alsa_format.format=SND_PCM_SFMT_S16_LE;
        ai->alsa_format.rate=ai->rate;
        ai->alsa_format.channels = ai->channels;

        if((err=snd_pcm_playback_format(ai->handle, &ai->alsa_format)) < 0 ) {
//          ReportError("Cannot reset the soundcard.");
            ReportError("Cannot get audio format.");
            return (Error)pmoError_IOCTL_SNDCTL_DSP_RESET;
        }
//      audio_set_playback_params2(ai);
//      int err;
        snd_pcm_playback_info_t pi;
        snd_pcm_playback_params_t pp;

        if((err=snd_pcm_playback_info(ai->handle, &pi)) < 0 )
        {
                ReportError("Cannot get audio info.");
                return (Error)pmoError_ALSA_Playback_Info;
        }

        bzero(&pp, sizeof(pp));
        pp.fragment_size = pi.buffer_size/4;
        if (pp.fragment_size > pi.max_fragment_size) pp.fragment_size = pi.max_fragment_size;
        if (pp.fragment_size < pi.min_fragment_size) pp.fragment_size = pi.min_fragment_size;
        pp.fragments_max = -1;
        pp.fragments_room = 1;

        if((err=snd_pcm_playback_params(ai->handle, &pp)) < 0 )
        {
                ReportError("Cannot set audio params.");
                return (Error)pmoError_ALSA_Playback_Params;
        }
        return 0;
}
