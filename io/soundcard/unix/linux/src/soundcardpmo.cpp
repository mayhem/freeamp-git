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

/* system headers */
#include <stdlib.h>
#include <iostream.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/soundcard.h>
#include <errno.h>
#include <string.h>

/* project headers */
#include <config.h>
#include "soundcardpmo.h"
#include "log.h"

LogFile *g_Log;

#define PIECES 50

extern    "C"
{
   PhysicalMediaOutput *Initialize(LogFile *pLog)
   {
      g_Log = pLog;
      return new SoundCardPMO();
   }
}

static char *g_ErrorArray[8] =
{
   "Invalid Error Code",
   "dsp device open failed",
   "fcntl F_GETFL on /dev/dsp failed",
   "fcntl F_SETFL on /dev/dsp failed"
   "ioctl reset failed",
   "config of samplesize failed",
   "config of stereo failed",
   "config of speed failed"
};

const char *SoundCardPMO::
GetErrorString(int32 error)
{
   if ((error <= pmoError_MinimumError) || (error >= pmoError_MaximumError))
   {
      return g_ErrorArray[0];
   }
   return g_ErrorArray[error - pmoError_MinimumError];
}

SoundCardPMO::SoundCardPMO()
{
   // cout << "Creating scpmo" << endl;
   m_properlyInitialized = false;
   myInfo = new OutputInfo();
   // cout << "Done creation of the SoundCardPMO..." << endl;
}

SoundCardPMO::~SoundCardPMO()
{
   // RAK: I took out the reset here. According to the docs
   // (http://www.se.opensound.com/pguide/audio.html) close() does
   // a reset automagically.
   // Reset(false);

   // cout << "Deleting scpmo... " << endl;
   close(audio_fd);
   if (myInfo)
   {
      delete    myInfo;

      myInfo = NULL;
   }
   // cout << "Done deleting scpmo" << endl;
}

int       SoundCardPMO::audio_fd = -1;

Error     SoundCardPMO::
Pause()
{
   return Reset(true);
}

Error     SoundCardPMO::
Resume()
{
   return kError_NoErr;
}

Error     SoundCardPMO::
Init(OutputInfo * info)
{
   m_properlyInitialized = false;
   if (!info)
   {
      info = myInfo;
   }
   else
   {
      // got info, so this is the beginning...
      if ((audio_fd = open("/dev/dsp", O_WRONLY | O_SYNC, 0)) < 0)
      {
         if (errno == EBUSY)
         {
            g_Log->Error("Audio device busy!\n");
            return (Error) pmoError_DeviceOpenFailed;
         }
         else
         {
            g_Log->Error("Cannot open audio device!\n");
            return (Error) pmoError_DeviceOpenFailed;
         }
      }
   }

   int       fd = audio_fd;
   int       flags;

   if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
   {
      return (Error) pmoError_IOCTL_F_GETFL;
   }

   flags &= O_NDELAY;           // SYNC;

   if (fcntl(fd, F_SETFL, flags) < 0)
   {
      return (Error) pmoError_IOCTL_F_SETFL;
   }

   audio_fd = fd;

   channels = info->number_of_channels;

   for (int i = 0; i < info->number_of_channels; ++i)
      bufferp[i] = buffer + i;

   // configure the device:
   int       play_precision = 16;
   int       play_stereo = channels - 1;
   int       play_sample_rate = info->samples_per_second;

   int       junkvar = 0;

   if (ioctl(audio_fd, SNDCTL_DSP_RESET, &junkvar) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_RESET;
   }

   if (ioctl(audio_fd, SNDCTL_DSP_SAMPLESIZE, &play_precision) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_SAMPLESIZE;
   }

   if (ioctl(audio_fd, SNDCTL_DSP_STEREO, &play_stereo) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_STEREO;
   }
   if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &play_sample_rate) == -1)
   {
      return (Error) pmoError_IOCTL_SNDCTL_DSP_SPEED;
   }
   myInfo->bits_per_sample = info->bits_per_sample;
   myInfo->number_of_channels = info->number_of_channels;
   myInfo->samples_per_second = info->samples_per_second;
   myInfo->max_buffer_size = info->max_buffer_size;
   m_properlyInitialized = true;
   return kError_NoErr;
}

Error     SoundCardPMO::
Reset(bool user_stop)
{
   // cout << "Reset..." << endl;
   int       a;

   if (user_stop)
   {
      if (ioctl(audio_fd, SNDCTL_DSP_RESET, &a) == -1)
      {
         return (Error) pmoError_IOCTL_SNDCTL_DSP_RESET;
      }
      Init(NULL);
   }
   else
   {
      if (ioctl(audio_fd, SNDCTL_DSP_SYNC, &a) == -1)
      {
         return (Error) pmoError_IOCTL_SNDCTL_DSP_RESET;
      }
   }
   return kError_NoErr;
}

Error     SoundCardPMO::
Write(int32 & rtn, void *pBuffer, int32 length)
{
   int32     actualThisTime = 0;
   int32     writeBlockLength = length / PIECES;

   // cout << "length: " << length << endl;
   // cout << "writeBl" << endl;
   void     *pv = NULL;
   int32     gp = 0;

   do
   {
      rtn = write(audio_fd, pBuffer, length);
      if (rtn > 0) {
         pBuffer = (void *) ((char *) pBuffer + rtn);
	 length -= rtn;
      }
   }
   while (length != 0 && (errno == 0 || errno == EINTR));
   if (length != 0)
   {
      rtn = -1;
      return kError_OutputUnsuccessful;
   }
   return kError_NoErr;
}
