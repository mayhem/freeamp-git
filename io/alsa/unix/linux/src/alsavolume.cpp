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

//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include <iostream.h>
#include <sys/asoundlib.h>

#include "alsavolume.h"

ALSAVolumeManager::ALSAVolumeManager(int iCard, int iDevice)
                  :VolumeManager()
{
   void *pMixer;
   char  mixer_id[13]=SND_MIXER_ID_MASTER; 

   this->iCard = iCard;
   this->iDevice = iDevice;
   switch (iDevice) 
   {
	    case 0: 
          strncpy(mixer_id,SND_MIXER_ID_PCM,sizeof(mixer_id));
          break;
	    case 1: 
          strncpy(mixer_id,SND_MIXER_ID_PCM1,sizeof(mixer_id));
          break;
   }

   snd_mixer_open(&pMixer, iCard, 0);
   iChannel = snd_mixer_channel(pMixer, mixer_id );
   if (iChannel < 0)
   {
       strncpy(mixer_id,SND_MIXER_ID_PCM,sizeof(mixer_id));
       iChannel = snd_mixer_channel(pMixer, mixer_id );
   }  
   snd_mixer_close(pMixer);
}

void ALSAVolumeManager::SetVolume(int32 iVolume) 
{
   int   err;
   void *pMixer;
   snd_mixer_channel_t channel;

   err = snd_mixer_open(&pMixer, iCard, 0);
   if (err < 0)
      return;

   if (iChannel >= 0) 
   {
      err = snd_mixer_channel_read(pMixer, iChannel, &channel);
      if (err < 0)
         return;

      channel.left=channel.right=iVolume;
      snd_mixer_channel_write(pMixer, iChannel, &channel);
   } 
   snd_mixer_close(pMixer);
}

int32 ALSAVolumeManager::GetVolume() 
{
   int   err;
   void *pMixer = NULL;
   snd_mixer_channel_t channel;

   err = snd_mixer_open(&pMixer, iCard, 0);
   if (err != 0)
   {
      return 0;
   }

   if (iChannel >= 0) 
   {
      err = snd_mixer_channel_read(pMixer, iChannel, &channel);
      if (err < 0)
         return 0;
   } 
   else
      return 0;

   snd_mixer_close(pMixer);
   return channel.left;
}
