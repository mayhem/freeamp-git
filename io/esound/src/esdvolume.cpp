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

ESDVolumeManager::ESDVolumeManager(int iStream)
                 :VolumeManager()
{
   this->iStream = iStream;
   this->iVolume = ESD_VOLUME_BASE;
}

void ESDVolumeManager::SetVolume(int32 v)
{
    int esd = esd_open_sound(NULL);
    iVolume = v;
    if (esd > 0)
    {
/* do nothing for now...
        v = ESD_VOLUME_BASE * (v & 0xff) / 50;
        esd_set_stream_pan(esd, iStream, v, v); */
        close(esd);
    }

}

int32 ESDVolumeManager::GetVolume()
{
    return iVolume;
}

