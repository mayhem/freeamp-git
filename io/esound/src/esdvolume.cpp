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
#include <linux/soundcard.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <esd.h>

#include "esdvolume.h"

ESDVolumeManager::ESDVolumeManager()
                 :VolumeManager()
{

}

void ESDVolumeManager::SetVolume(int32 v) 
{ // This OSS code will work for the moment.
    int mixFd = open("/dev/mixer",O_RDWR);
    if (mixFd != -1) 
    {
        v |= (v << 8);
        ioctl(mixFd, SOUND_MIXER_WRITE_PCM, &v);
        close(mixFd);
    }
}

int32 ESDVolumeManager::GetVolume() 
{
    int mixFd = open("/dev/mixer",O_RDWR);
    int volume = 0;
    if (mixFd != -1) 
    {
        	ioctl(mixFd, SOUND_MIXER_READ_PCM, &volume);
        	volume &= 0xFF;
        	close(mixFd);
    }
    return volume;
}

