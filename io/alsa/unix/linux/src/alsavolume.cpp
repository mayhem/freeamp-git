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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/asoundlib.h>

#include "alsavolume.h"

ALSAVolumeManager::ALSAVolumeManager(int iCard, int iDevice)
                  :VolumeManager()
{
   this->iCard = iCard;
   this->iDevice = iDevice;
}

void ALSAVolumeManager::SetVolume(int32 iVolume) 
{
   int   err, mixer_channel;
   void *pMixer;
   char  mixer_id[13]=SND_MIXER_ID_MASTER; 
   snd_mixer_channel_t channel;

   err = snd_mixer_open(&pMixer, iCard, 0);
   if (err < 0)
      return;

   mixer_channel = snd_mixer_channel(pMixer, SND_MIXER_ID_PCM);   
   if (mixer_channel >= 0) 
   {
      err = snd_mixer_channel_read(pMixer, mixer_channel, &channel);
      if (err < 0)
         return;

      channel.left=channel.right=iVolume;
      snd_mixer_channel_write(pMixer, mixer_channel, &channel);
   } 

   snd_mixer_close(pMixer);
}

int32 ALSAVolumeManager::GetVolume() 
{
   int   err, mixer_channel;
   void *pMixer = NULL;
   snd_mixer_channel_t channel;

   err = snd_mixer_open(&pMixer, iCard, 0);
   if (err != 0)
   {
      return 0;
   }

   mixer_channel = snd_mixer_channel(pMixer, SND_MIXER_ID_PCM);   
   if (mixer_channel >= 0) 
   {
      err = snd_mixer_channel_read(pMixer, mixer_channel, &channel);
      if (err < 0)
         return 0;
   } 
   else
      return 0;

   snd_mixer_close(pMixer);

   return channel.left;
}
