/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player
        Driver for Advanced Linux Sound Architecture 
              http://www.alsa-project.org
 
        Portions Copyright (C) 1998-1999 EMusic.com

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

extern "C"
{
   PhysicalMediaOutput *Initialize(FAContext *context) {
      return new AlsaPMO(context);
   }
}

static int parse_gid(const char *str, snd_mixer_gid_t *gid)
{
    unsigned int size;
    unsigned char *ptr;

    bzero(gid, sizeof(*gid));
    ptr = gid->name;
    size = 0;
    while (*str) {
        if (size < sizeof(gid->name)) {
            *ptr++ = *str;
            size++;
        }
        str++;
    }
    return 0;
}

AlsaPMO::AlsaPMO(FAContext *context) :
	    PhysicalMediaOutput(context)
{
	uint32 deviceNameSize = 128;
   char scard[128];
   snd_mixer_t *pMixer;
   char  mixer_id[25]="Master";

   m_properlyInitialized = false;
   myInfo = new OutputInfo();
   memset(myInfo, 0, sizeof(OutputInfo));

   m_pBufferThread = NULL;

   m_iOutputBufferSize = 0;
   m_iBytesPerSample = 0;
   m_iBaseTime = -1;
   m_iDataSize = 0;

   if (!m_pBufferThread)
   {
      m_pBufferThread = Thread::CreateThread();
      assert(m_pBufferThread);
      m_pBufferThread->Create(AlsaPMO::StartWorkerThread, this);
   }

   ai = new struct audio_info_struct;
   ai->handle = NULL;
   ai->alsa_format.format = (unsigned)-1;
   ai->alsa_format.rate = (unsigned)-1;
   ai->alsa_format.channels = (unsigned)-1;
   ai->device = NULL;
   ai->format = -1;
   ai->channels = -1;
   ai->rate = -1;
   ai->mixer_handle=0;

	ai->device = (char *) malloc(deviceNameSize);
	m_pContext->prefs->GetPrefString(kALSADevicePref, ai->device,
					&deviceNameSize);

   if (ai->device)
   {
        if (sscanf(ai->device, "%[^:]: %d", scard, &m_iDevice) != 2)
        {
           ReportError("The ALSADevice statement in the preference file"
                       "is improperly formatted. Format: ALSADevice: "
                       "[card name/card number]:[device number]");
           return;
        }

        m_iCard = snd_card_name(scard);
        if (m_iCard < 0) 
        {
            ReportError("Invalid ALSA card name/number specified.");
            return;
        }
        if (m_iDevice < 0 || m_iDevice > 31) 
        {
            ReportError("Invalid ALSA device number specified.");
            return;
        }
   }
   else
       m_iCard = m_iDevice = 0;


   switch (m_iDevice)
   {
       case 0:
          strncpy(mixer_id,"PCM",sizeof(mixer_id));
          break;
       case 1:
          strncpy(mixer_id,"PCM1",sizeof(mixer_id));
          break;
   }

   parse_gid(mixer_id, &m_gid);

   snd_mixer_open(&pMixer, m_iCard, 0);

   bzero(&m_group, sizeof(m_group));
   m_group.gid = m_gid;

   if ((m_iChannel = snd_mixer_group_read(pMixer, &m_group )) < 0)
   {
       strncpy(mixer_id,"Master",sizeof(mixer_id));

       parse_gid(mixer_id, &m_gid);
       m_group.gid = m_gid;
       m_iChannel = snd_mixer_group_read(pMixer, &m_group );
   }
   snd_mixer_close(pMixer);

   delete ai->device;
}

AlsaPMO::~AlsaPMO() 
{
    m_bExit = true;
    m_pSleepSem->Signal();
    m_pPauseSem->Signal();

    if (m_pBufferThread)
    {
       m_pBufferThread->Join();
       delete m_pBufferThread;
    }

    snd_pcm_close(ai->handle);
    if (myInfo) {
        delete myInfo;
        myInfo = NULL;
    }
    if (ai) {
        delete ai;
        ai = NULL;
    }
}


void AlsaPMO::SetVolume(int32 iVolume)
{
   int   err;
   snd_mixer_t *pMixer;

   err = snd_mixer_open(&pMixer, m_iCard, 0);
   if (err < 0)
      return;

   if (m_iChannel >= 0)
   {
      err = snd_mixer_group_read(pMixer, &m_group);
      if (err < 0)
         return;

      int actualVolume = (int)((double)(m_group.max - m_group.min) * 
                          (double)iVolume * 0.01) + m_group.min;
      for (int chn = 0; chn <= SND_MIXER_CHN_LAST; chn++) {
          if (!(m_group.channels & (1<<chn)))
              continue;
          m_group.volume.values[chn] = actualVolume;
      }
      snd_mixer_group_write(pMixer, &m_group);
   }
   snd_mixer_close(pMixer);
} 

int32 AlsaPMO::GetVolume()
{
   int   err;
   snd_mixer_t *pMixer = NULL;

   err = snd_mixer_open(&pMixer, m_iCard, 0);
   if (err != 0)
   {
      return 0;
   }

   if (m_iChannel >= 0)
   {
      err = snd_mixer_group_read(pMixer, &m_group);
      if (err < 0)
         return 0;
   }
   else
      return 0;

   snd_mixer_close(pMixer);
   int actualVolume = (int)((double)(m_group.max - m_group.min) *
                       (double)m_group.volume.values[0] * 0.01) + 
                      m_group.min;
   return actualVolume;
} 

Error AlsaPMO::Init(OutputInfo* info) {
    int err;

    m_properlyInitialized = false;
    if (!info) {
        info = myInfo;
    } else {
        // got info, so this is the beginning...
        m_iDataSize = info->max_buffer_size;

        if((err=snd_pcm_open(&ai->handle, m_iCard, m_iDevice, SND_PCM_OPEN_PLAYBACK)) < 0 )
        {
            ReportError("Audio device is busy. Please make sure that "
                        "another program is not using the device.");
            return (Error)pmoError_DeviceOpenFailed;
        }
#define SND_PCM_BLOCK_MODE_NON_BLOCKING         (0)
#define SND_PCM_BLOCK_MODE_BLOCKING             (1)
        snd_pcm_block_mode(ai->handle,SND_PCM_BLOCK_MODE_NON_BLOCKING);
        if ((err=snd_pcm_playback_time(ai->handle,1))!=0) {
                ReportError("Can't enable playback time.");
            return (Error)pmoError_DeviceOpenFailed;
        }
    }

    channels = info->number_of_channels;
    
    // configure the device:
    ai->format=16;
    ai->channels=channels;
    ai->rate=info->samples_per_second;

    audio_set_all(ai);

    myInfo->bits_per_sample = info->bits_per_sample;
    myInfo->number_of_channels = info->number_of_channels;
    myInfo->samples_per_second = info->samples_per_second;
    myInfo->max_buffer_size = info->max_buffer_size;

    if (snd_pcm_playback_time(ai->handle, true))
        ReportError("Cannot set soundcard time playback mode.");

    snd_pcm_playback_status_t aInfo;
    snd_pcm_playback_status(ai->handle,&aInfo);
    m_iOutputBufferSize = aInfo.fragment_size * aInfo.fragments;
    m_iTotalFragments = aInfo.fragments;
    m_iBytesPerSample = info->number_of_channels * (info->bits_per_sample / 8);

    m_properlyInitialized = true;
    return kError_NoErr;
}

Error AlsaPMO::Reset(bool user_stop) {

    if (user_stop) 
        snd_pcm_drain_playback(ai->handle);
    else
        snd_pcm_flush_playback(ai->handle);

    return kError_NoErr;
}

void AlsaPMO::Pause(void)
{
    m_iBaseTime = -1;

    PhysicalMediaOutput::Pause();
}

bool AlsaPMO::WaitForDrain(void)
{
   snd_pcm_playback_status_t ainfo;

   snd_pcm_playback_status(ai->handle,&ainfo);
   for(; !m_bExit && !m_bPause; )
   {
       snd_pcm_playback_status(ai->handle,&ainfo);
       if (ainfo.underrun || ainfo.queue  < ainfo.fragment_size)
       {
           return true;
       }
       WasteTime();
   }
   return false;
} 

void AlsaPMO::HandleTimeInfoEvent(PMOTimeInfoEvent *pEvent)
{
   MediaTimeInfoEvent *pmtpi;
   int32               hours, minutes, seconds;
   int                 iTotalTime = 0;
   snd_pcm_playback_status_t ainfo;
   long long           llStart, llEnd;

   if (m_iBaseTime < 0)
   {
       m_iBaseTime = (pEvent->GetFrameNumber() * 
                      myInfo->samples_per_frame) / 
                      myInfo->samples_per_second;
   }

   snd_pcm_playback_status(ai->handle,&ainfo);
   llEnd = ((long long)ainfo.time.tv_sec * 100) + 
           ((long long)ainfo.time.tv_usec / 10000);
   llStart = ((long long)ainfo.stime.tv_sec * 100) + 
             ((long long)ainfo.stime.tv_usec / 10000);
   iTotalTime = ((llEnd - llStart) / 100) + m_iBaseTime;

   iTotalTime %= 86400;

   hours = iTotalTime / 3600;
   minutes = (iTotalTime / 60) % 60;
   seconds = iTotalTime % 60;

   if (hours < 0  ||
       minutes < 0 || minutes > 59 || 
       seconds < 0 || seconds > 59)
      return;

   pmtpi = new MediaTimeInfoEvent(hours, minutes, seconds, 0, 
                                  iTotalTime, 0);
   m_pTarget->AcceptEvent(pmtpi);
}

void AlsaPMO::StartWorkerThread(void *pVoidBuffer)
{
   ((AlsaPMO*)pVoidBuffer)->WorkerThread();
}

void AlsaPMO::WorkerThread(void)
{
   void                      *pBuffer;
   Error                      eErr;
   size_t                     iRet;
   Event                     *pEvent;
   snd_pcm_playback_status_t  ainfo;

   // Don't do anything until resume is called.
   m_pPauseSem->Wait();

   // Sleep for a pre buffer period
   PreBuffer();

   // The following should be abstracted out into the general thread
   // classes:
#ifdef __linux__
   struct sched_param sParam;

   sParam.sched_priority = sched_get_priority_max(SCHED_OTHER);
   pthread_setschedparam(pthread_self(), SCHED_OTHER, &sParam);
#endif

   for(; !m_bExit;)
   {
      if (m_bPause)
      {
          m_pPauseSem->Wait();
          continue;
      }

      // Loop until we get an Init event from the LMC
      if (!m_properlyInitialized)
      {
          pEvent = ((EventBuffer *)m_pInputBuffer)->GetEvent();

          if (pEvent == NULL)
          {
              m_pLmc->Wake();
              WasteTime();

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

      // Set up reading a block from the buffer. If not enough bytes are
      // available, sleep for a little while and try again.
      for(;;)
      {
          eErr = ((EventBuffer *)m_pInputBuffer)->BeginRead(pBuffer, 
                                                             m_iDataSize);
          if (eErr == kError_EndOfStream || eErr == kError_Interrupt)
             break;

          if (eErr == kError_NoDataAvail)
          {
              m_pLmc->Wake();

              WasteTime();
              continue;
          }

          // Is there an event pending that we need to take care of
          // before we play this block of samples?
          if (eErr == kError_EventPending)
          {
              pEvent = ((EventBuffer *)m_pInputBuffer)->GetEvent();

              if (pEvent->Type() == PMO_Init)
                  Init(((PMOInitEvent *)pEvent)->GetInfo());
    
              if (pEvent->Type() == PMO_Reset)
                  Reset(false);
    
              if (pEvent->Type() == PMO_Info) 
                  HandleTimeInfoEvent((PMOTimeInfoEvent *)pEvent);
    
              if (pEvent->Type() == PMO_Quit) 
              {
                  delete pEvent;
                  if (WaitForDrain())
                  {
                     Reset(true);
                     m_pTarget->AcceptEvent(new Event(INFO_DoneOutputting));
                  }
                  return;
              }
 
              delete pEvent;
    
              continue;
          }
          
          if (IsError(eErr))
          {
              ReportError("Internal error occured.");
              m_pContext->log->Error("Cannot read from buffer in PMO "
                                    "worker tread: %d\n", eErr);
              break;
          }
          break;
      }

      // Now write the block to the audio device. If the block doesn't
      // all fit, pause and loop until the entire block has been played.
      // This loop could be written using non-blocking io...
      for(;;)
      {
          if (m_bExit || m_bPause)
              break;

          iRet = snd_pcm_playback_status(ai->handle,&ainfo);
          if (ainfo.count < m_iDataSize)      
          {
              WasteTime();
              continue;
          }
          break;
      }        
      if (m_bExit || m_bPause)
      {
          m_pInputBuffer->EndRead(0);
          continue;
      }

      iRet = snd_pcm_write(ai->handle,pBuffer,m_iDataSize);
      if (iRet < 0)
      {
         m_pInputBuffer->EndRead(0);
         ReportError("Could not write sound data to the soundcard.");
         m_pContext->log->Error("Failed to write to the soundcard: %s\n", 
                               strerror(errno));
         break;
      }

      m_pInputBuffer->EndRead(iRet);
      m_pLmc->Wake();
      UpdateBufferStatus();
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
        if ((unsigned)pp.fragment_size > pi.max_fragment_size) 
            pp.fragment_size = pi.max_fragment_size;
        if ((unsigned)pp.fragment_size < pi.min_fragment_size) 
            pp.fragment_size = pi.min_fragment_size;
        pp.fragments_max = -1;
        pp.fragments_room = 1;

        if((err=snd_pcm_playback_params(ai->handle, &pp)) < 0 )
        {
                ReportError("Cannot set audio params.");
                return (Error)pmoError_ALSA_Playback_Params;
        }
        return 0;
}

