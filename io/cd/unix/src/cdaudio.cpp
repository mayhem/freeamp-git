/*____________________________________________________________________________
 
        FreeAmp - The Free MP3 Player
 
        Portions Copyright (C) 1999 EMusic.com
        Portions Copyright (C) 1998-1999 Tony Arcieri
        Portions Copyright (C) 1999 Quinton Dolan
  
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


/* hacked from libcdaudio */

#include <config.h>
#undef socklen_t

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cdaudio.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>

/* We can check to see if the CD-ROM is mounted if this is available */
#ifdef HAVE_MNTENT_H
#include <mntent.h>
#endif

#ifdef HAVE_SYS_MNTENT_H
#include <sys/mntent.h>
#endif

#ifdef HAVE_SYS_UCRED_H
#include <sys/ucred.h>
#endif

#ifdef HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif

/* For Linux */
#ifdef HAVE_LINUX_CDROM_H
#include <linux/cdrom.h>
#define NON_BLOCKING
#endif

#ifdef HAVE_LINUX_UCDROM_H
#include <linux/ucdrom.h>
#endif

/* For FreeBSD, OpenBSD, and Solaris */
#ifdef HAVE_SYS_CDIO_H
#include <sys/cdio.h>
#endif

/* For Digital UNIX */
#ifdef HAVE_IO_CAM_CDROM_H
#include <io/cam/cdrom.h>
#endif

#include "compat.h"


#ifndef IRIX_CDAUDIO
/*
Because of Irix's different interface, most of this program is
completely ignored when compiling under Irix.
*/
#include <iostream.h>

/* Return the version of libcdaudio */
long cdaudio_getversion(void)
{
   return LIBCDAUDIO_VERSION;
}

/* Initialize the CD-ROM for playing audio CDs */
int
#if __STDC__
cd_init_device(char *device_name)
#else
cd_init_device(device_name)
   char *device_name
#endif
{
   int cd_desc;

#ifdef HAVE_GETMNTENT
   FILE *mounts;
   struct mntent *mnt;
#endif
#ifdef HAVE_GETMNTINFO
   int mounts;
   struct statfs *mnt;
#endif
   char devname[255];
   struct stat st;
   int len = 0;

   if(lstat(device_name, &st) < 0)
     return -1;
   
   if(S_ISLNK(st.st_mode)) {
      /* Some readlink implementations don't put a \0 on the end */
      len = readlink(device_name, devname, 255);
      devname[len] = '\0';
   } else {
      strncpy(devname, device_name, 255);
      len = strlen(devname);
   }
#ifdef HAVE_GETMNTENT
   if((mounts = setmntent(MOUNTED, "r")) == NULL)
      return -1;
      
   while((mnt = getmntent(mounts)) != NULL) {
      if(strncmp(mnt->mnt_fsname, devname, len) == 0) {
	 endmntent(mounts);
	 errno = EBUSY;
	 return -1;
      }
   }
   endmntent(mounts);
#endif
#ifdef HAVE_GETMNTINFO
   for ( (mounts = getmntinfo(&mnt, 0)); mounts > 0;)
   {
      mounts--;
      if (strncmp(mnt[mounts].f_mntfromname, devname, len) == 0)
      {
         errno = EBUSY;
         return -1;
      }
   }
#endif   

#ifdef NON_BLOCKING
   if((cd_desc = open(device_name, O_RDONLY | O_NONBLOCK)) < 0)
#else
   if((cd_desc = open(device_name, O_RDONLY)) < 0)
#endif
     return -1;
	
   return cd_desc;
}

/* Close a device handle and free its resources. */
int
#if __STDC__
cd_finish(int cd_desc)
#else
cd_finish(cd_desc)
   int cd_desc;
#endif
{
   close(cd_desc);
   return 0;
}

/* Update a CD status structure... because operating system interfaces vary
   so does this function. */
int
#if __STDC__
cd_stat(int cd_desc, struct disc_info *disc)
#else
cd_stat(cd_desc, disc)
   int cd_desc;
   struct disc_info *disc;
#endif
{
   /* Since every platform does this a little bit differently this gets pretty
      complicated... */

   struct CDAUDIO_TOCHEADER cdth;
   struct CDAUDIO_TOCENTRY cdte;
#ifdef CDAUDIO_TOCENTRY_DATA
   struct CDAUDIO_TOCENTRY_DATA cdte_buffer[MAX_TRACKS];
#endif

   struct disc_status status;
   int readtracks, pos;
   
   if(cd_poll(cd_desc, &status) < 0)
     return -1;
 
   if(!status.status_present) {
      disc->disc_present = 0;
      return 0;
   }
   
   /* Read the Table Of Contents header */
   if(ioctl(cd_desc, CDAUDIO_READTOCHEADER, &cdth) < 0)
     return -1;

   disc->disc_first_track = cdth.CDTH_STARTING_TRACK;
   disc->disc_total_tracks = cdth.CDTH_ENDING_TRACK;

#ifdef CDAUDIO_READTOCENTRYS

   /* Read the Table Of Contents */
   cdte.CDTE_ADDRESS_FORMAT = CDAUDIO_MSF_FORMAT;
   cdte.CDTE_STARTING_TRACK = 0;
   cdte.CDTE_DATA = cdte_buffer;
   cdte.CDTE_DATA_LEN = sizeof(cdte_buffer);

   if(ioctl(cd_desc, CDAUDIO_READTOCENTRYS, &cdte) < 0)
     return -1;

   for(readtracks = 0; readtracks <= disc->disc_total_tracks; readtracks++) {
      disc->disc_track[readtracks].track_pos.minutes = cdte.CDTE_DATA[readtracks].CDTE_MSF_M;
      disc->disc_track[readtracks].track_pos.seconds = cdte.CDTE_DATA[readtracks].CDTE_MSF_S;
      disc->disc_track[readtracks].track_pos.frames = cdte.CDTE_DATA[readtracks].CDTE_MSF_F;
      disc->disc_track[readtracks].track_type = (cdte.CDTE_DATA[readtracks].CDTE_CONTROL & CDROM_DATA_TRACK) ? CDAUDIO_TRACK_DATA : CDAUDIO_TRACK_AUDIO;
      disc->disc_track[readtracks].track_lba = cd_msf_to_lba(disc->disc_track[readtracks].track_pos);
   }

#else /* CDAUDIO_READTOCENTRYS */

   for(readtracks = 0; readtracks <= disc->disc_total_tracks; readtracks++) {
      cdte.CDTE_STARTING_TRACK = (readtracks == disc->disc_total_tracks) ? CDROM_LEADOUT : readtracks + 1;
      cdte.CDTE_ADDRESS_FORMAT = CDAUDIO_MSF_FORMAT;
      if(ioctl(cd_desc, CDAUDIO_READTOCENTRY, &cdte) < 0)
      return -1;

      disc->disc_track[readtracks].track_pos.minutes = cdte.CDTE_MSF_M;
      disc->disc_track[readtracks].track_pos.seconds = cdte.CDTE_MSF_S;
      disc->disc_track[readtracks].track_pos.frames = cdte.CDTE_MSF_F;
      disc->disc_track[readtracks].track_type = (cdte.CDTE_CONTROL & CDROM_DATA_TRACK) ? CDAUDIO_TRACK_DATA : CDAUDIO_TRACK_AUDIO;
      disc->disc_track[readtracks].track_lba = cd_msf_to_lba(disc->disc_track[readtracks].track_pos);
   }
#endif /* CDAUDIO_READTOCENTRY */
   for(readtracks = 0; readtracks <= disc->disc_total_tracks; readtracks++) {
      if(readtracks > 0) {
         pos = cd_msf_to_frames(disc->disc_track[readtracks].track_pos) -
         	cd_msf_to_frames(disc->disc_track[readtracks - 1].track_pos);
	 cd_frames_to_msf(&disc->disc_track[readtracks - 1].track_length, pos);
      }
   }
            
   disc->disc_length.minutes = disc->disc_track[disc->disc_total_tracks].track_pos.minutes;
   disc->disc_length.seconds = disc->disc_track[disc->disc_total_tracks].track_pos.seconds;
   disc->disc_length.frames = disc->disc_track[disc->disc_total_tracks].track_pos.frames;
   
   cd_update(disc, status);
 
   return 0;
}

int
#if __STDC__
cd_poll(int cd_desc, struct disc_status *status)
#else
cd_poll(cd_desc, status)
   int cd_desc;
   struct disc_status *status
#endif
{
   struct CDAUDIO_SUBCHANNEL cdsc;
#ifdef CDAUDIO_SUBCHANNEL_DATA
   struct CDAUDIO_SUBCHANNEL_DATA cdsc_data;
#endif


#ifdef CDAUDIO_SUBCHANNEL_DATA
   memset(&cdsc_data, '\0', sizeof(cdsc_data));
   cdsc.CDSC_DATA = &cdsc_data;
   cdsc.CDSC_DATA_LEN = sizeof(cdsc_data);
   cdsc.CDSC_DATA_FORMAT = CDAUDIO_CDSC_DATA_FORMAT;
#endif
   cdsc.CDSC_ADDRESS_FORMAT = CDAUDIO_MSF_FORMAT;
   
   if(ioctl(cd_desc, CDAUDIO_READSUBCHANNEL, &cdsc) < 0)
     {
	status->status_present = 0;
	
	return 0;
     }
   
   status->status_present = 1;
   
#ifdef CDAUDIO_SUBCHANNEL_DATA
   status->status_disc_time.minutes = cdsc_data.CDSC_DATA_ABS_MSF_M;
   status->status_disc_time.seconds = cdsc_data.CDSC_DATA_ABS_MSF_S;
   status->status_disc_time.frames = cdsc_data.CDSC_DATA_ABS_MSF_F;
   status->status_track_time.minutes = cdsc_data.CDSC_DATA_REL_MSF_M;
   status->status_track_time.seconds = cdsc_data.CDSC_DATA_REL_MSF_S;
   status->status_track_time.frames = cdsc_data.CDSC_DATA_REL_MSF_F;
   status->status_current_track = cdsc_data.CDSC_DATA_TRK;
   switch(cdsc_data.CDSC_AUDIO_STATUS) {
#else
   status->status_disc_time.minutes = cdsc.CDSC_ABS_MSF_M;
   status->status_disc_time.seconds = cdsc.CDSC_ABS_MSF_S;
   status->status_disc_time.frames = cdsc.CDSC_ABS_MSF_F;
   status->status_track_time.minutes = cdsc.CDSC_REL_MSF_M;
   status->status_track_time.seconds = cdsc.CDSC_REL_MSF_S;
   status->status_track_time.frames = cdsc.CDSC_REL_MSF_F;
   status->status_current_track = cdsc.CDSC_TRK;
   switch(cdsc.CDSC_AUDIO_STATUS) {
#endif
    case CDAUDIO_AS_PLAY_IN_PROGRESS:
      status->status_mode = CDAUDIO_PLAYING;
      break;
    case CDAUDIO_AS_PLAY_PAUSED:
      status->status_mode = CDAUDIO_PAUSED;
      break;
    case CDAUDIO_AS_PLAY_COMPLETED:
      status->status_mode = CDAUDIO_COMPLETED;
      break;
    default:
      status->status_mode = CDAUDIO_NOSTATUS;
   }
   
   return 0;
}

/* Play frames from CD */
int
#if __STDC__
cd_play_frames(int cd_desc, int startframe, int endframe)
#else
cd_play_frames(cd_desc, startframe, endframe)
   int cd_desc;
   int startframe;
   int endframe;
#endif
{
   struct CDAUDIO_MSF cdmsf;

   cdmsf.CDMSF_START_M = startframe / 4500;
   cdmsf.CDMSF_START_S = (startframe % 4500) / 75;
   cdmsf.CDMSF_START_F = startframe % 75;
   cdmsf.CDMSF_END_M = endframe / 4500;
   cdmsf.CDMSF_END_S = (endframe % 4500) / 75;
   cdmsf.CDMSF_END_F = endframe % 75;

#ifdef CDAUDIO_START
   /* Some CDROM drives don't support this so lets try things without it */
   /* ioctl(cd_desc, CDAUDIO_START); */
#endif
   
   if(ioctl(cd_desc, CDAUDIO_PLAY_MSF, &cdmsf) < 0)
     return -2;

   return 0;
}

/* Play starttrack at position pos to endtrack */
int
#if __STDC__
cd_play_track_pos(int cd_desc, int starttrack, int endtrack, int startpos)
#else
cd_play_track_pos(cd_desc, starttrack, endtrack, startpos)
   int cd_desc;
   int starttrack;
   int endtrack;
   int startpos;
#endif
{
   struct disc_timeval time;
   
   time.minutes = startpos / 60;
   time.seconds = startpos % 60;
   time.frames = 0;
   
   return cd_playctl(cd_desc, PLAY_END_TRACK | PLAY_START_POSITION, starttrack, endtrack, &time);
}

/* Play starttrack to endtrack */
int
#if __STDC__
cd_play_track(int cd_desc, int starttrack, int endtrack)
#else
cd_play_track(cd_desc, starttrack, endtrack)
   int cd_desc;
   int starttrack;
   int endtrack;
#endif
{
   return cd_playctl(cd_desc, PLAY_END_TRACK, starttrack, endtrack);
}

/* Play starttrack at position pos to end of CD */
int
#if __STDC__
cd_play_pos(int cd_desc, int track, int startpos)
#else
cd_play_pos(cd_desc, track, startpos)
   int cd_desc;
   int track;
   int startpos;
#endif
{
   struct disc_timeval time;
   
   time.minutes = startpos / 60;
   time.seconds = startpos * 60;
   time.frames = 0;
   
   return cd_playctl(cd_desc, PLAY_START_POSITION, track, &time);
}

/* Play starttrack to end of CD */
int
#if __STDC__
cd_play(int cd_desc, int track)
#else
cd_play(cd_desc, track)
   int cd_desc;
   int track;
#endif
{
   return cd_playctl(cd_desc, PLAY_START_TRACK, track);
}

/* Stop the CD, if it is playing */
int
#if __STDC__
cd_stop(int cd_desc)
#else
cd_stop(cd_desc)
   int cd_desc;
#endif
{
   if(ioctl(cd_desc, CDAUDIO_STOP) < 0)
     return -1;
   
   return 0;
}

/* Pause the CD */
int
#if __STDC__
cd_pause(int cd_desc)
#else
cd_pause(cd_desc)
   int cd_desc;
#endif
{
   if(ioctl(cd_desc, CDAUDIO_PAUSE) < 0)
     return -1;
   
   return 0;
}

/* Resume playing */
int
#if __STDC__
cd_resume(int cd_desc)
#else
cd_resume(cd_desc)
   int cd_desc;
#endif
{
   if(ioctl(cd_desc, CDAUDIO_RESUME) < 0)
     return -1;
   
   return 0;
}

/* Eject the tray */
int
#if __STDC__
cd_eject(int cd_desc)
#else
cd_eject(cd_desc)
   int cd_desc;
#endif
{  
   if(ioctl(cd_desc, CDAUDIO_EJECT) < 0)
     return -1;
   
   return 0;
}

/* Close the tray */
int
#if __STDC__
cd_close(int cd_desc)
#else
cd_close(cd_desc)
   int cd_desc;
#endif
{
   if(ioctl(cd_desc, CDAUDIO_CLOSE) < 0)
     return -1;
   
   return 0;
}

/* Return the current volume setting */
int
#if __STDC__
cd_get_volume(int cd_desc, struct disc_volume *vol)
#else
cd_get_volume(cd_desc, vol)
   int cd_desc;
   struct disc_volume *vol;
#endif
{
   struct CDAUDIO_VOLSTAT cdvol;
#ifdef CDAUDIO_VOLSTAT_DATA
   struct CDAUDIO_VOLSTAT_DATA cdvol_data;
   cdvol.CDVOLSTAT_DATA = &cdvol_data;
   cdvol.CDVOLSTAT_DATA_LEN = sizeof(cdvol_data);
#endif

   if(ioctl(cd_desc, CDAUDIO_GET_VOLUME, &cdvol) < 0)
     return -1;
   
   vol->vol_front.left = cdvol.CDVOLSTAT_FRONT_LEFT;
   vol->vol_front.right = cdvol.CDVOLSTAT_FRONT_RIGHT;
   vol->vol_back.left = cdvol.CDVOLSTAT_BACK_LEFT;
   vol->vol_back.right = cdvol.CDVOLSTAT_BACK_RIGHT;

#ifdef CDAUDIO_VOLSTAT_DATA
   vol->vol_front.left = cdvol_data.CDVOLSTAT_FRONT_LEFT;
   vol->vol_front.right = cdvol_data.CDVOLSTAT_FRONT_RIGHT;
   vol->vol_back.left = cdvol_data.CDVOLSTAT_BACK_LEFT;
   vol->vol_back.right = cdvol_data.CDVOLSTAT_BACK_RIGHT;
#endif
   
   return 0;
}

/* Set the volume */
int
#if __STDC__
cd_set_volume(int cd_desc, struct disc_volume vol)
#else
cd_set_volume(cd_desc, vol)
   int cd_desc;
   struct disc_volume vol;
#endif
{
   struct CDAUDIO_VOLCTRL cdvol;
#ifdef CDAUDIO_VOLCTRL_DATA
   struct cd_playback cdvol_data;
   cdvol.CDVOLCTRL_DATA = &cdvol_data;
   cdvol.CDVOLCTRL_DATA_LEN = sizeof(cdvol_data);
#endif
   
   if(vol.vol_front.left > 255 || vol.vol_front.left < 0 || vol.vol_front.right > 255 || vol.vol_front.right < 0 || vol.vol_back.left > 255 || vol.vol_back.left < 0 || vol.vol_back.right > 255 || vol.vol_back.right < 0)
     return -1;

   cdvol.CDVOLCTRL_FRONT_LEFT = vol.vol_front.left;
   cdvol.CDVOLCTRL_FRONT_RIGHT = vol.vol_front.right;
   cdvol.CDVOLCTRL_BACK_LEFT = vol.vol_back.left;
   cdvol.CDVOLCTRL_BACK_RIGHT = vol.vol_back.right;

#ifdef CDAUDIO_VOLCTRL_SELECT
   cdvol_data.CDVOLCTRL_FRONT_LEFT_SELECT = CDAUDIO_MAX_VOLUME;
   cdvol_data.CDVOLCTRL_FRONT_RIGHT_SELECT = CDAUDIO_MAX_VOLUME;
   cdvol_data.CDVOLCTRL_BACK_LEFT_SELECT = CDAUDIO_MAX_VOLUME;
   cdvol_data.CDVOLCTRL_BACK_RIGHT_SELECT = CDAUDIO_MAX_VOLUME;
#endif
   
   if(ioctl(cd_desc, CDAUDIO_SET_VOLUME, &cdvol) < 0)
     return -1;

   return 0;
}

#endif  /* IRIX_CDAUDIO */

/*
Because all these functions are solely mathematical and/or only make callbacks
to previously existing functions they can be used for any platform.
 */

/* Convert frames to a logical block address */
int
cd_frames_to_lba(int frames)
{
   if(frames >= 150)
     return frames - 150;
   
   return 0;
}

/* Convert a logical block address to frames */
int
cd_lba_to_frames(int lba)
{
   return lba + 150;
}

/* Convert disc_timeval to frames */
int
cd_msf_to_frames(struct disc_timeval time)
{
   return time.minutes * 4500 + time.seconds * 75 + time.frames;
}

/* Convert disc_timeval to a logical block address */
int
cd_msf_to_lba(struct disc_timeval time)
{
   if(cd_msf_to_frames(time) > 150)
     return cd_msf_to_frames(time) - 150;
   
   return 0;
}

/* Convert frames to disc_timeval */
void
cd_frames_to_msf(struct disc_timeval *time, int frames)
{
   time->minutes = frames / 4500;
   time->seconds = (frames % 4500) / 75;
   time->frames = frames % 75;
}

/* Convert a logical block address to disc_timeval */
void
cd_lba_to_msf(struct disc_timeval *time, int lba)
{
   cd_frames_to_msf(time, lba + 150);
}

/* Internal advance function */
int
#if __STDC__
__internal_cd_track_advance(int cd_desc, struct disc_info disc, int endtrack, struct disc_timeval time)
#else
__internal_cd_track_advance(int cd_desc, struct disc_info disc, endtrack, time)
   int cd_desc;
   struct disc_info disc;
   int endtrack;
   struct disc_timeval time;
#endif
{ 
   disc.disc_track_time.minutes += time.minutes;
   disc.disc_track_time.seconds += time.seconds;
   disc.disc_track_time.frames += time.frames;
   
   if(disc.disc_track_time.frames > 74) {
      disc.disc_track_time.frames -= 74;
      disc.disc_track_time.seconds++;
   }
   
   if(disc.disc_track_time.frames < 0) {
      disc.disc_track_time.frames += 75;
      disc.disc_track_time.seconds--;
   }
   
   if(disc.disc_track_time.seconds > 59) {
      disc.disc_track_time.seconds -= 59;
      disc.disc_track_time.minutes++;
   }
   if(disc.disc_track_time.seconds < 0) {
      disc.disc_track_time.seconds += 60;
      disc.disc_track_time.minutes--;
   }

   if(disc.disc_track_time.minutes < 0) {
      disc.disc_current_track--;
      if(disc.disc_current_track == 0)
        disc.disc_current_track = 1;

      return cd_play_track(cd_desc, disc.disc_current_track, endtrack);
   }

   if((disc.disc_track_time.minutes == disc.disc_track[disc.disc_current_track].track_pos.minutes && disc.disc_track_time.seconds > disc.disc_track[disc.disc_current_track].track_pos.seconds) || disc.disc_track_time.minutes > disc.disc_track[disc.disc_current_track].track_pos.minutes) {
      disc.disc_current_track++;
      if(disc.disc_current_track > endtrack)
        disc.disc_current_track = endtrack;

      return cd_play_track(cd_desc, disc.disc_current_track, endtrack);
   }

   return cd_play_track_pos(cd_desc, disc.disc_current_track, endtrack, disc.disc_track_time.minutes * 60 + disc.disc_track_time.seconds);
}

/* Advance the position within a track */
int
#if __STDC__
cd_track_advance(int cd_desc, int endtrack, struct disc_timeval time)
#else
cd_track_advance(cd_desc, endtrack, time)
   int cd_desc;
   int endtrack;
   struct disc_timeval time;
#endif
{
   struct disc_info disc;
   
   if(cd_stat(cd_desc, &disc) < 0)
     return -1;
   
   if(!disc.disc_present)
     return -1;
   
   if(__internal_cd_track_advance(cd_desc, disc, endtrack, time) < 0)
     return -1;
   
   return 0;
}

/* Advance the position within the track without preserving an endtrack */
int
#if __STDC__
cd_advance(int cd_desc, struct disc_timeval time)
#else
cd_advance(cd_desc, time)
    int cd_desc;
    struct disc_timeval time;
#endif
{
   struct disc_info disc;

   if(cd_stat(cd_desc, &disc) < 0)
     return -1;
   
   if(__internal_cd_track_advance(cd_desc, disc, disc.disc_total_tracks, time) < 0)
     return -1;
   
   return 0;
}

/* Update information in a disc_info structure using a disc_status structure */
int
#if __STDC__
cd_update(struct disc_info *disc, struct disc_status status)
#else
cd_update(disc, status)
   struct disc_info *disc;
   struct disc_status status;
#endif
{
   if(!(disc->disc_present = status.status_present))
     return -1;
   
   disc->disc_mode = status.status_mode;
   memcpy(&disc->disc_time, &status.status_disc_time, sizeof(struct disc_timeval));
   memcpy(&disc->disc_track_time, &status.status_track_time, sizeof(struct disc_timeval));
   
   disc->disc_current_track = 0;
   while(disc->disc_current_track < disc->disc_total_tracks && 
          cd_msf_to_frames(disc->disc_time) >= cd_msf_to_frames(disc->disc_track[disc->disc_current_track].track_pos) )
     disc->disc_current_track++;
   
   return 0;
}

/* Universal play control function */
int
#if __STDC__
cd_playctl(int cd_desc, int options, int start_track, ...)
#else
cd_playctl(cd_desc, options, ...)
   int cd_desc;
   int options;
#endif
{
   int end_track;
   struct disc_info disc;
   struct disc_timeval *startpos, *endpos, start_position, end_position;
   va_list arglist;

   va_start(arglist, start_track);
   if(cd_stat(cd_desc, &disc) < 0)
     return -1;

   if(options & PLAY_END_TRACK)
     end_track = va_arg(arglist, int);
   else
     end_track = disc.disc_total_tracks;
   
   if(options & PLAY_START_POSITION)
     startpos = va_arg(arglist, struct disc_timeval *);
   else
     startpos = 0;
   
   if(options & PLAY_END_POSITION)
     endpos = va_arg(arglist, struct disc_timeval *);
   else
     endpos = 0;
   
   va_end(arglist);
   
   if(options & PLAY_START_POSITION) {
      start_position.minutes = disc.disc_track[start_track - 1].track_pos.minutes + startpos->minutes;
      start_position.seconds = disc.disc_track[start_track - 1].track_pos.seconds + startpos->seconds;
      start_position.frames = disc.disc_track[start_track - 1].track_pos.frames + startpos->frames;
   } else {
      start_position.minutes = disc.disc_track[start_track - 1].track_pos.minutes;
      start_position.seconds = disc.disc_track[start_track - 1].track_pos.seconds;
      start_position.frames = disc.disc_track[start_track - 1].track_pos.frames;
   }

   if(options & PLAY_END_TRACK) {
      if(options & PLAY_END_POSITION) {
	 end_position.minutes = disc.disc_track[end_track].track_pos.minutes + endpos->minutes;
	 end_position.seconds = disc.disc_track[end_track].track_pos.seconds + endpos->seconds;
	 end_position.frames = disc.disc_track[end_track].track_pos.frames + endpos->frames;
      } else {
	 end_position.minutes = disc.disc_track[end_track].track_pos.minutes;
	 end_position.seconds = disc.disc_track[end_track].track_pos.seconds;
	 end_position.frames = disc.disc_track[end_track].track_pos.frames;
      }
   } else {
      end_position.minutes = disc.disc_track[disc.disc_total_tracks].track_pos.minutes;
      end_position.seconds = disc.disc_track[disc.disc_total_tracks].track_pos.seconds;
      end_position.frames = disc.disc_track[disc.disc_total_tracks].track_pos.frames;
   }
   
   return cd_play_frames(cd_desc, cd_msf_to_frames(start_position), cd_msf_to_frames(end_position));
}
