
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


/* Audio hardware handlers (SGI, OSS, Sun, NeXT, Mac, W95, Be, HPUX) */
/*
 * layout of this file:
 *    error handlers
 *    SGI new and old audio library (find "- SG")
 *    OSS ("- O")
 *    NeXT ("- NE")
 *    Sun ("- SU") (has switches for OPENBSD, but they're untested)
 *    Mac ("- M")
 *    Be ("- B")
 *    HPUX ("- H")
 *    W95 ("- WI")
 *    audio describers
 */

/* TODO: w95 input, read/write state
 *       sgi/w95/mac? also may have multiple systems/cards
 *       more machines!
 */
/* when reading device_field, put default input device first, or mark somehow */

/*
 * char *report_audio_state(void) returns the same information as a string.
 *
 * int open_audio_output(int dev, int srate, int chans, int format, int size)
 * int open_audio_input(int dev, int srate, int chans, int format, int size)
 * int write_audio(int line, char *buf, int bytes)
 * int close_audio(int line)
 * int read_audio(int line, char *buf, int bytes)
 *
 * int read_audio_state(int dev, int field, int chan, float *val)
 * int write_audio_state(int dev, int field, int chan, float *val)
 * void save_audio_state(void)
 * void restore_audio_state(void)
 *
 * int audio_error(void) returns the error code indicated by the immediately preceding audio call
 * int initialize_audio(void) does whatever is needed to get set up
 * char *audio_error_name(int err) gives string decription of error code
 *
 * int audio_systems(void) returns number of separate complete audio systems (soundcards essentially)
 * AUDIO_SYSTEM(n) selects the nth card (counting from 0), AUDIO_SYSTEM(0) is always the default
 * char *audio_system_name(int system) returns some user-recognizable (?) name for the given card
 */

#if defined(HAVE_CONFIG_H)
  #include "config.h"
#endif

#include <math.h>
#include <stdio.h>
#if (!defined(HAVE_CONFIG_H)) || (defined(HAVE_FCNTL_H))
  #include <fcntl.h>
#endif
#include <signal.h>
#if (!defined(HAVE_CONFIG_H)) || (defined(HAVE_LIMITS_H))
  #include <limits.h>
#endif
#include <errno.h>
#include <stdlib.h>

#if (defined(NEXT) || (defined(HAVE_LIBC_H) && (!defined(HAVE_UNISTD_H))))
  #include <libc.h>
#else
  #include <unistd.h>
#endif

#include "sndlib.h"

#define STRBUF_SIZE 4096

#if (defined(HAVE_CONFIG_H)) && (!defined(HAVE_STRERROR))
char *strerror(int errnum)
{
  char *strerrbuf;
  strerrbuf = (char *)CALLOC(16,sizeof(char));
  sprintf(strerrbuf,"io err %d",errnum);
  return(strerrbuf);
}
#else
  #include <string.h>
#endif
  

static int AUDIO_ERROR = NO_ERROR;
int audio_error(void) {return(AUDIO_ERROR);}
void set_audio_error(int err) {AUDIO_ERROR = err;}
static int audio_initialized = 0;

static char *audio_error_name_1(int err)
{
  switch (err)
    {
    case NO_ERROR:                    return("");                            break;
    case CHANNELS_NOT_AVAILABLE:      return("channel(s) not available");    break;
    case SRATE_NOT_AVAILABLE:         return("srate not available");         break;
    case FORMAT_NOT_AVAILABLE:        return("format not available");        break;
    case NO_INPUT_AVAILABLE:          return("no input available");          break;
    case NO_OUTPUT_AVAILABLE:         return("no output available");         break;
    case INPUT_BUSY:                  return("input busy");                  break;
    case OUTPUT_BUSY:                 return("output busy");                 break;
    case CONFIGURATION_NOT_AVAILABLE: return("configuration not available"); break;
    case INPUT_CLOSED:                return("input closed");                break;
    case OUTPUT_CLOSED:               return("output closed");               break;
    case IO_INTERRUPTED:              return("io interrupted");              break;
    case NO_LINES_AVAILABLE:          return("no lines available");          break;
    case WRITE_ERROR:                 return("write error");                 break;
    case SIZE_NOT_AVAILABLE:          return("size not available");          break;
    case DEVICE_NOT_AVAILABLE:        return("device not available");        break;
    case CANT_CLOSE:                  return("close failed");                break;
    case CANT_OPEN:                   return("open failed");                 break;
    case READ_ERROR:                  return("read error");                  break;
    case AMP_NOT_AVAILABLE:           return("amp control not available");   break;
    case AUDIO_NO_OP:                 return("unimplemented operation");     break;
    case CANT_WRITE:                  return("write failed");                break;
    case CANT_READ:                   return("read failed");                 break;
    case NO_READ_PERMISSION:          return("need read permission on /dev/dsp"); break;
    default: return("unknown error");                                        break;
    }
}

static char *strbuf = NULL;
static void pprint(char *str);

int device_channels(int dev);
int device_gains(int dev);

int device_channels(int dev)
{
  float val[1];
  read_audio_state(dev,CHANNEL_FIELD,0,val);
  return((int)val[0]);
}

int device_gains(int ur_dev)
{
  float val[1];
  int err;
  int dev;
  dev = SNDLIB_DEVICE(ur_dev);
  /* to get hardware gains, read device amp_field and error = none */
  if ((dev == DAC_FILTER_DEVICE) || (dev == MIXER_DEVICE)) 
    {
      err = read_audio_state(ur_dev,CHANNEL_FIELD,0,val);
      return((int)val[0]);
    }
  err = read_audio_state(ur_dev,AMP_FIELD,0,val);
  if (err != NO_ERROR) return(0);
  return(device_channels(ur_dev));
}



/* ------------------------------- SGI ----------------------------------------- */

#ifdef SGI
#define AUDIO_OK

#include <audio.h>

static char errstr[256];
static int al_err = 0;

int audio_systems(void) {return(1);} /* I think more than 1 is possible, but don't have a case to test with */
char *audio_system_name(int system) {return("SGI");}

#ifdef AL_RESOURCE

char *audio_error_name(int err)
{
  if (!al_err) return(audio_error_name_1(err));
  sprintf(errstr,"%s: %s",audio_error_name_1(err),alGetErrorString(al_err));
  return(errstr);
}

static int check_queue_size (int size, int chans) {return(size);}

#else

#define STEREO_QUEUE_MIN_SIZE 1024
#define STEREO_QUEUE_MIN_CHOICE 1024
/* docs say 510 or 512, but they die with "File size limit exceeded" %$@#!(& */
#define MONO_QUEUE_MIN_SIZE 1019
#define MONO_QUEUE_MIN_CHOICE 1024
#define STEREO_QUEUE_MAX_SIZE 131069
#define STEREO_QUEUE_MAX_CHOICE 65536
#define MONO_QUEUE_MAX_SIZE 262139
#define MONO_QUEUE_MAX_CHOICE 131072
/* if these limits are not followed, the damned thing dumps core and dies */

static int check_queue_size (int size, int chans)
{
  if ((chans == 1) && (size > MONO_QUEUE_MAX_SIZE)) return(MONO_QUEUE_MAX_CHOICE);
  if ((chans == 1) && (size < MONO_QUEUE_MIN_SIZE)) return(MONO_QUEUE_MIN_CHOICE);
  if ((chans > 1) && (size > STEREO_QUEUE_MAX_SIZE)) return(STEREO_QUEUE_MAX_CHOICE);
  if ((chans > 1) && (size < STEREO_QUEUE_MIN_SIZE)) return(STEREO_QUEUE_MIN_CHOICE);
  return(size);
}

static void check_quad (int device, int channels)
{
  long sr[2];
  /* if quad, make sure we are set up for it, else make sure we aren't (perhaps the latter is unnecessary) */
  /* in 4 channel mode, stereo mic and line-in are 4 inputs, headphones/speakers and stereo line-out are the 4 outputs */
  sr[0] = AL_CHANNEL_MODE;
  ALgetparams(device,sr,2);
  if ((channels == 4) && (sr[1] != AL_4CHANNEL))
    {
      sr[1] = AL_4CHANNEL;
      ALsetparams(device,sr,2);
    }
  else
    {
      if ((channels != 4) && (sr[1] != AL_STEREO))
        {
          sr[1] = AL_STEREO;
          ALsetparams(device,sr,2);
        }
    }
}

static char *AL_GetErrorString(int err)
{
  switch (err)
    {
    case AL_BAD_NOT_IMPLEMENTED:   return("not implemented yet"); break;
    case AL_BAD_PORT:              return("tried to use an invalid port"); break;
    case AL_BAD_CONFIG:            return("tried to use an invalid configuration"); break;
    case AL_BAD_DEVICE:            return("tried to use an invalid device"); break;
    case AL_BAD_DEVICE_ACCESS:     return("unable to access the device"); break;
    case AL_BAD_DIRECTION:         return("illegal direction given for port"); break;
    case AL_BAD_OUT_OF_MEM:        return("operation has run out of memory"); break;
    case AL_BAD_NO_PORTS:          return("not able to allocate a port"); break;
    case AL_BAD_WIDTH:             return("invalid sample width given"); break;
    case AL_BAD_ILLEGAL_STATE:     return("an illegal state has occurred"); break;
    case AL_BAD_QSIZE:             return("attempt to set an invalid queue size"); break;
    case AL_BAD_FILLPOINT:         return("attempt to set an invalid fillpoint"); break;
    case AL_BAD_BUFFER_NULL:       return("null buffer pointer"); break;
    case AL_BAD_COUNT_NEG:         return("negative count"); break;
    case AL_BAD_PVBUFFER:          return("param/val buffer doesn't make sense"); break;
    case AL_BAD_BUFFERLENGTH_NEG:  return("negative buffer length"); break;
    case AL_BAD_BUFFERLENGTH_ODD:  return("odd length parameter/value buffer"); break;
    case AL_BAD_CHANNELS:          return("invalid channel specifier"); break;
    case AL_BAD_PARAM:             return("invalid parameter"); break;
    case AL_BAD_SAMPFMT:           return("attempt to set invalid sample format"); break;
    case AL_BAD_RATE:              return("invalid sample rate token"); break;
    case AL_BAD_TRANSFER_SIZE:     return("invalid size for sample read/write"); break;
    case AL_BAD_FLOATMAX:          return("invalid size for floatmax"); break;
    case AL_BAD_PORTSTYLE:         return("invalid port style"); break;
    default:                       return("");
    }
}

char *audio_error_name(int err)
{
  if (!al_err) return(audio_error_name_1(err));
  sprintf(errstr,"%s: %s",audio_error_name_1(err),AL_GetErrorString(al_err));
  return(errstr);
}

#endif

#define IO_LINES 8
static ALconfig config[IO_LINES];
static ALport port[IO_LINES];
static int line_in_use[IO_LINES];
static int channels[IO_LINES];
static long device[IO_LINES];
static int datum_size[IO_LINES];
static int line_out[IO_LINES];

int initialize_audio(void)
{
  int i;
  if (!audio_initialized)
    {
      audio_initialized = 1;
      AUDIO_ERROR = NO_ERROR; al_err = 0;
      for (i=0;i<IO_LINES;i++) line_in_use[i]=0;
    }
  return(0);
}

static int error_exit(int error, int line)
{
  AUDIO_ERROR = error; 
#ifdef AL_RESOURCE
  if (line != -1) alFreeConfig(config[line]); 
#else
  if (line != -1) ALfreeconfig(config[line]); 
#endif
  return(-1);
}

#ifdef AL_RESOURCE
static int to_al_interface_or_device(int dev,int which)
{
  switch (dev)
    {
    case MICROPHONE_DEVICE: return(alGetResourceByName(AL_SYSTEM,"Microphone",which)); break;
    case DEFAULT_DEVICE:
    case READ_WRITE_DEVICE:
    case DAC_OUT_DEVICE:
    case SPEAKERS_DEVICE: return(alGetResourceByName(AL_SYSTEM,"Analog Out",which)); break;
    case ADAT_IN_DEVICE: return(alGetResourceByName(AL_SYSTEM,"ADAT In",which)); break;
    case AES_IN_DEVICE: return(alGetResourceByName(AL_SYSTEM,"AES In",which)); break;
    case ADAT_OUT_DEVICE: return(alGetResourceByName(AL_SYSTEM,"ADAT Out",which)); break;
    case DIGITAL_OUT_DEVICE:
    case AES_OUT_DEVICE: return(alGetResourceByName(AL_SYSTEM,"AES Out",which)); break;
    case LINE_IN_DEVICE: return(alGetResourceByName(AL_SYSTEM,"Line In",which)); break;
    case LINE_OUT_DEVICE: return(alGetResourceByName(AL_SYSTEM,"Line Out2",which)); break; /* ?? */
      /* case DIGITAL_IN_DEVICE: return(alGetResourceByName(AL_SYSTEM,"DAC2 In",which)); break; */ /* this is analog in ?? */
    }
  return(0);
}

static int to_al_device(int dev) {return(to_al_interface_or_device(dev,AL_DEVICE_TYPE));}
static int to_al_interface(int dev) {return(to_al_interface_or_device(dev,AL_INTERFACE_TYPE));}
#endif

#include <stdio.h>

/* just a placeholder for now */
int find_audio_output(int chans)
{
#ifdef AL_RESOURCE
  ALvalue x[32];
  ALpv y;
  int n,i;
  y.param = AL_INTERFACE;
  y.value.i = AL_DIGITAL_IF_TYPE;
  n = alQueryValues(AL_SYSTEM,AL_DEFAULT_OUTPUT,x,32,&y,1);
  for (i=0;i<n;i++) 
    {
      y.param = AL_CHANNELS;
      alGetParams(x[i].i,&y,1);
      if (chans <= y.value.i) return(x[i].i);
    }
#endif
  return(-1);
}

static int to_sgi_format(int frm)
{
  switch (frm)
    {
    case snd_8_linear: 
    case snd_16_linear: 
    case snd_24_linear: return(AL_SAMPFMT_TWOSCOMP); break;
    case snd_32_float: return(AL_SAMPFMT_FLOAT); break;
    case snd_64_double: return(AL_SAMPFMT_DOUBLE); break;
    }
  return(-1);
}

int open_audio_output(int ur_dev, int srate, int chans, int format, int requested_size)
{
#ifdef AL_RESOURCE
  ALpv pv;
  ALpv z[2];
#else
  long sr[2];
#endif
  int i,line,size,width,sgi_format,dev;
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  line = -1;
  for (i=0;i<IO_LINES;i++) if (line_in_use[i] == 0) {line=i; break;}
  if (line == -1) {AUDIO_ERROR = NO_LINES_AVAILABLE; return(-1);}
  channels[line] = chans;
  line_out[line] = 1;
  datum_size[line] = c_snd_datum_size(format);
  if (requested_size == 0) 
    size = 4096;
  else size = check_queue_size(requested_size,chans);
  if (datum_size[line] == 3) 
    width = AL_SAMPLE_24;
  else 
    {
      if (datum_size[line] == 1) 
        width = AL_SAMPLE_8;
      else width = AL_SAMPLE_16;
    }
  sgi_format = to_sgi_format(format);
#ifdef AL_RESOURCE
  if (dev == DEFAULT_DEVICE) dev = DAC_OUT_DEVICE;
  device[line] = to_al_device(dev);                     if (!(device[line])) return(error_exit(DEVICE_NOT_AVAILABLE,-1));
  if (chans>2) size = 65536;                            /* for temp adat code */
  if (device_channels(dev) < chans)                     /* look for some device that can play this file */
    device[line] = find_audio_output(chans);            if (device[line] == -1) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
  z[0].param = AL_RATE;
  z[0].value.ll = alDoubleToFixed((double)srate);
  z[1].param = AL_MASTER_CLOCK;
  z[1].value.i = AL_CRYSTAL_MCLK_TYPE;
  alSetParams(device[line],z, 2); 
  config[line] = alNewConfig();                         if (!(config[line])) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
  if ((sgi_format == -1) || 
      (alSetSampFmt(config[line],sgi_format) == -1) ||
      (alSetWidth(config[line],width) == -1))           /* this is a no-op in the float and double cases */
    return(error_exit(FORMAT_NOT_AVAILABLE,line));
  al_err = alSetChannels(config[line],chans);           if (al_err == -1) return(error_exit(CHANNELS_NOT_AVAILABLE,line));
  al_err = alSetQueueSize(config[line],size);           if (al_err == -1) return(error_exit(SIZE_NOT_AVAILABLE,line));
  al_err = alSetDevice(config[line],device[line]);      if (al_err == -1) return(error_exit(DEVICE_NOT_AVAILABLE,line));
  port[line] = alOpenPort("dac","w",config[line]);      if (!(port[line])) return(error_exit(CONFIGURATION_NOT_AVAILABLE,line));
#else
  device[line] = AL_DEFAULT_DEVICE;
  check_quad(device[line],chans);
  sr[0]=AL_OUTPUT_RATE;
  sr[1]=srate;
  al_err = ALsetparams(device[line],sr,2);              if (al_err == -1) return(error_exit(SRATE_NOT_AVAILABLE,-1));
  config[line] = ALnewconfig();                         if (!(config[line])) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
  if ((sgi_format == -1) || 
      (ALsetsampfmt(config[line],sgi_format) == -1) ||
      (ALsetwidth(config[line],width) == -1))           /* this is a no-op in the float and double cases */
    return(error_exit(FORMAT_NOT_AVAILABLE,line));
  al_err = ALsetchannels(config[line],chans);           if (al_err == -1) return(error_exit(CHANNELS_NOT_AVAILABLE,line));
  al_err = ALsetqueuesize(config[line],size);           if (al_err == -1) return(error_exit(SIZE_NOT_AVAILABLE,line));
  port[line] = ALopenport("dac","w",config[line]);      if (!(port[line])) return(error_exit(CONFIGURATION_NOT_AVAILABLE,line));
#endif
  line_in_use[line] = 1;
  return(line);
}

int write_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR; al_err = 0;
#ifdef AL_RESOURCE
  al_err = alWriteFrames(port[line],(short *)buf,bytes/(channels[line]*datum_size[line]));
#else
  al_err = ALwritesamps(port[line],(short *)buf,bytes/datum_size[line]);
#endif
  if (al_err) {AUDIO_ERROR = WRITE_ERROR; return(-1);}
  return(0);
}

int close_audio(int line)
{
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  if (line_in_use[line])
    {
      if (line_out[line])
        {
#ifdef AL_RESOURCE
          while (alGetFilled(port[line]) > 0) sginap(1);
#else
          while (ALgetfilled(port[line]) > 0) sginap(1);
#endif
        }
#ifdef AL_RESOURCE
      al_err = alClosePort(port[line]);
      if (al_err == 0) al_err = alFreeConfig(config[line]);
#else
      al_err = ALcloseport(port[line]);
      if (al_err == 0) al_err = ALfreeconfig(config[line]);
#endif
      line_in_use[line] = 0;
      if (al_err) {AUDIO_ERROR = CANT_CLOSE; return(-1);}
    }
  return(0);
}

int open_audio_input(int ur_dev, int srate, int chans, int format, int requested_size)
{
  int dev;
#ifdef AL_RESOURCE
  ALpv pv;
  ALpv x[2];
  int itf;
#else
  long sr[2];
  int resind;
#endif
  int i,line,size;
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  line = -1;
  for (i=0;i<IO_LINES;i++) if (line_in_use[i] == 0) {line=i; break;}
  if (line == -1) {AUDIO_ERROR = NO_LINES_AVAILABLE; return(-1);}
  channels[line] = chans;
  line_out[line] = 0;
  datum_size[line] = c_snd_datum_size(format);
  if (requested_size == 0) 
    size = 4096;
  else size = check_queue_size(requested_size,chans);
  /* there are lots of ways this may be called in terms of the desired "device" */
  /* in CLM, the caller specifies which device, in Snd we try to open everything available */
#ifdef AL_RESOURCE
  device[line] = to_al_device((dev == DEFAULT_DEVICE) ? MICROPHONE_DEVICE : dev);
  if (!(device[line])) return(error_exit(DEVICE_NOT_AVAILABLE,-1));
  itf = to_al_interface((dev == DEFAULT_DEVICE) ? MICROPHONE_DEVICE : dev);
  if (itf)
    { 
      pv.param = AL_INTERFACE;
      pv.value.i = itf;
      al_err = alSetParams(device[line],&pv,1);         if (al_err == -1) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
    }
  x[0].param = AL_RATE;
  x[0].value.ll = alDoubleToFixed((double)srate);
  x[1].param = AL_MASTER_CLOCK;
  x[1].value.i = AL_CRYSTAL_MCLK_TYPE;
  al_err = alSetParams(device[line],x,2);               if (al_err == -1) return(error_exit(SRATE_NOT_AVAILABLE,-1));
  config[line] = alNewConfig();                         if (!config[line]) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
  if ((to_sgi_format(format) == -1) || 
      (alSetSampFmt(config[line],to_sgi_format(format)) == -1) ||
      (alSetWidth(config[line],(datum_size[line] == 2) ? AL_SAMPLE_16 : AL_SAMPLE_8) == -1))
    return(error_exit(FORMAT_NOT_AVAILABLE,line));
  al_err = alSetChannels(config[line],chans);           if (al_err == -1) return(error_exit(CHANNELS_NOT_AVAILABLE,line));
  al_err = alSetQueueSize(config[line],size);           if (al_err == -1) return(error_exit(SIZE_NOT_AVAILABLE,line));
  al_err = alSetDevice(config[line],device[line]);      if (al_err == -1) return(error_exit(DEVICE_NOT_AVAILABLE,line));
  port[line] = alOpenPort("adc","r",config[line]);      if (!(port[line])) return(error_exit(CONFIGURATION_NOT_AVAILABLE,line));
#else
  switch (dev)
    {
    case DEFAULT_DEVICE: 
    case READ_WRITE_DEVICE: 
    case MICROPHONE_DEVICE: resind = AL_INPUT_MIC; break;
    case LINE_IN_DEVICE: resind = AL_INPUT_LINE; break;
    case DIGITAL_IN_DEVICE: resind = AL_INPUT_DIGITAL; break;
    default: return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1)); break;
    }
  device[line] = AL_DEFAULT_DEVICE;
  sr[0] = AL_INPUT_SOURCE;
  sr[1] = resind;
  al_err = ALsetparams(device[line],sr,2);              if (al_err == -1) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
  check_quad(device[line],chans);
  sr[0] = AL_INPUT_RATE;
  sr[1] = srate;
  al_err = ALsetparams(device[line],sr,2);              if (al_err == -1) return(error_exit(SRATE_NOT_AVAILABLE,-1));
  config[line] = ALnewconfig();                         if (!config[line]) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
  if ((to_sgi_format(format) == -1) || 
      (ALsetsampfmt(config[line],to_sgi_format(format)) == -1) ||
      (ALsetwidth(config[line],(datum_size[line] == 2) ? AL_SAMPLE_16 : AL_SAMPLE_8) == -1))
    return(error_exit(FORMAT_NOT_AVAILABLE,line));
  al_err = ALsetchannels(config[line],chans);           if (al_err == -1) return(error_exit(CHANNELS_NOT_AVAILABLE,line));
  al_err = ALsetqueuesize(config[line],size);           if (al_err == -1) return(error_exit(SIZE_NOT_AVAILABLE,line));
  port[line] = ALopenport("adc","r",config[line]);      if (!(port[line])) return(error_exit(CONFIGURATION_NOT_AVAILABLE,line));
#endif
  line_in_use[line] = 1;
  return(line);
}

int read_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR; al_err = 0;
#ifdef AL_RESOURCE
  al_err = alReadFrames(port[line],(short *)buf,bytes/(channels[line]*datum_size[line]));
#else
  al_err = ALreadsamps(port[line],(short *)buf,bytes/datum_size[line]);
#endif
  if (al_err) {AUDIO_ERROR = WRITE_ERROR; return(-1);}
  return(0);
}



#ifdef AL_RESOURCE
/* borrowed from /usr/share/src/dmedia/audio/printdevs.c with modifications */

#define MAX_CHANNELS 8

static float dB_to_linear(float val)
{
  if (val == 0.0) return(1.0);
  return(pow(10.0,val/20.0));
}

static float linear_to_dB(float val)
{
  return(20.0 * log10(val));
}

static float dB_to_normalized(float val, float lo, float hi)
{
  float linlo;
  if (hi <= lo) return(1.0);
  linlo = dB_to_linear(lo);
  return((dB_to_linear(val) - linlo) / (dB_to_linear(hi) - linlo));
}

static float normalized_to_dB(float val_norm, float lo_dB, float hi_dB)
{
  if (hi_dB <= lo_dB) return(0.0);
  return(lo_dB + (hi_dB - lo_dB)*val_norm);
}

int read_audio_state(int ur_dev, int field, int chan, float *val)
{
  ALpv x[4];
  ALparamInfo pinf;
  ALfixed g[MAX_CHANNELS];
  int rv,chans,i,dev;
  long fld;
  float amp;
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  if (field != DEVICE_FIELD)
    {
      rv = to_al_device(dev);                          if (!rv) return(error_exit(DEVICE_NOT_AVAILABLE,-1)); 
    }
  switch (field)
    {
    case DEVICE_FIELD:
      /* in this case, chan == length of incoming val array.  Number of devices is returned as val[0],
       * and the rest of the available area (if needed) is filled with the device ids.
       */

      /* TODO: get current AL_INPUT_SOURCE and output device and put these at the head of the list */

      i = 0;
      if (alGetResourceByName(AL_SYSTEM,"Microphone",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = MICROPHONE_DEVICE; i++;}
      if (alGetResourceByName(AL_SYSTEM,"Analog Out",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = DAC_OUT_DEVICE; i++;}
      if (alGetResourceByName(AL_SYSTEM,"ADAT In",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = ADAT_IN_DEVICE; i++;}
      if (alGetResourceByName(AL_SYSTEM,"AES In",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = AES_IN_DEVICE; i++;}
      if (alGetResourceByName(AL_SYSTEM,"ADAT Out",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = ADAT_OUT_DEVICE; i++;}
      if (alGetResourceByName(AL_SYSTEM,"AES Out",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = AES_OUT_DEVICE; i++;}
      if (alGetResourceByName(AL_SYSTEM,"Line In",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = LINE_IN_DEVICE; i++;}
      /* if (alGetResourceByName(AL_SYSTEM,"DAC2 In",AL_DEVICE_TYPE) != 0) {if ((i+1)<chan) val[i+1] = DIGITAL_IN_DEVICE; i++;} */
      val[0] = i;
      break;
    case FORMAT_FIELD:  val[0] = 1; if (chan>1) val[1] = snd_16_linear; break;
    case CHANNEL_FIELD:
      x[0].param = AL_CHANNELS;
      al_err = alGetParams(rv,x,1);                if (al_err == -1) return(error_exit(READ_ERROR,-1)); 
      val[0] = (float)(x[0].value.i);
      break;
    case SRATE_FIELD:
      x[0].param = AL_RATE;
      al_err = alGetParams(rv,x,1);                if (al_err == -1) return(error_exit(READ_ERROR,-1)); 
      val[0] = (float)(x[0].value.i);
      break;
    case AMP_FIELD:
      al_err = alGetParamInfo(rv,AL_GAIN,&pinf);   if (al_err == -1) return(error_exit(READ_ERROR,-1)); 
      if (pinf.min.ll == pinf.max.ll) return(error_exit(AMP_NOT_AVAILABLE,-1));
      /* this ridiculous thing is in dB with completely arbitrary min and max values */
      x[0].param = AL_GAIN;
      x[0].value.ptr = g;
      x[0].sizeIn = MAX_CHANNELS;
      al_err = alGetParams(rv,x,1);                if (x[0].sizeOut <= chan) return(error_exit(CHANNELS_NOT_AVAILABLE,-1));
      val[0] = dB_to_normalized(alFixedToDouble(g[chan]),alFixedToDouble(pinf.min.ll),alFixedToDouble(pinf.max.ll));
      break;
    default: AUDIO_ERROR = CANT_READ; break;
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val)
{
  /* each field coming in assumes 0.0 to 1.0 as the range */
  ALpv x[4];
  ALparamInfo pinf;
  ALfixed g[MAX_CHANNELS];
  int rv,dev;
  long fld;
  double amp;
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  rv = to_al_device(dev);                          if (!rv) return(error_exit(DEVICE_NOT_AVAILABLE,-1)); 
  switch (field)
    {
    case SRATE_FIELD:
      x[0].param = AL_RATE;
      x[0].value.i = (int)val[0];
      x[1].param = AL_MASTER_CLOCK;
      x[1].value.i = AL_CRYSTAL_MCLK_TYPE;
      alSetParams(rv,x,2); 
      break;
    case AMP_FIELD:
      /* need to change normalized linear value into dB between (dB) lo and hi */
      al_err = alGetParamInfo(rv,AL_GAIN,&pinf);  if (al_err == -1) return(error_exit(READ_ERROR,-1)); 
      /* I think we need to read all channels here, change the one we care about, then write all channels */
      x[0].param = AL_GAIN;
      x[0].value.ptr = g;
      x[0].sizeIn = MAX_CHANNELS;
      al_err = alGetParams(rv,x,1);               if (x[0].sizeOut <= chan) return(error_exit(CHANNELS_NOT_AVAILABLE,-1));
      g[chan] = alDoubleToFixed(normalized_to_dB(val[0],alFixedToDouble(pinf.min.ll),alFixedToDouble(pinf.max.ll)));
      al_err = alSetParams(rv,x,1);               if (al_err == -1) return(error_exit(WRITE_ERROR,-1));
      break;
    case DEVICE_FIELD:
      /* TODO: */
      /* how to set digital in??? */
      AUDIO_ERROR = CANT_WRITE;
      break;

    default: AUDIO_ERROR = CANT_WRITE; break;
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

#define STRING_SIZE 32
static void dump_resources(ALvalue *x,int rv)
{
  ALpv y[4];
  ALpv yy;
  ALparamInfo pinf;
  ALfixed g[MAX_CHANNELS];
  char dn[STRING_SIZE];
  char dl[STRING_SIZE];
  int i,k;
  ALvalue z[16];
  int nres;
  for (i=0;i<rv;i++)
    {
      y[0].param = AL_LABEL;
      y[0].value.ptr = dl;
      y[0].sizeIn = STRING_SIZE;
      y[1].param = AL_NAME;
      y[1].value.ptr = dn;
      y[1].sizeIn = STRING_SIZE;
      y[2].param = AL_CHANNELS;
      y[3].param = AL_RATE;
      alGetParams(x[i].i,y,5);
      if (alIsSubtype(AL_DEVICE_TYPE, x[i].i)) 
        {
          al_err = alGetParamInfo(x[i].i,AL_GAIN,&pinf);
          sprintf(strbuf,"\nDevice name: '%s', label: '%s', srate: %d, chans: %d",
                  dn,dl,
                  y[3].value.i,y[2].value.i); 
          pprint(strbuf);
          if (pinf.min.ll != pinf.max.ll)
            {
              yy.param = AL_GAIN;
              yy.value.ptr = g;
              yy.sizeIn = MAX_CHANNELS;
              alGetParams(x[i].i,&yy,1);
              pprint(" amps:[");
              for (k=0;k<yy.sizeOut;k++)
                {
                  sprintf(strbuf,"%.2f",dB_to_normalized(alFixedToDouble(g[k]),alFixedToDouble(pinf.min.ll),alFixedToDouble(pinf.max.ll)));
                  pprint(strbuf);
                  if (k<(yy.sizeOut-1)) pprint(" ");
                }
              pprint("]");
            }
          pprint("\n");
          if ((nres= alQueryValues(x[i].i,AL_INTERFACE,z,16,0,0))>=0) 
            dump_resources(z,nres);
          else sprintf(strbuf,"query failed: %s\n",alGetErrorString(oserror())); 
          pprint(strbuf);
        }
      else 
        {
          sprintf(strbuf,"      Interface name: '%s', label: '%s', chans: %d\n",dn,dl,y[2].value.i); 
          pprint(strbuf);
        }
    }
}

static void describe_audio_state_1(void)
{
  int rv;
  ALvalue x[16];
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  pprint("Devices and Interfaces on this system:\n"); 
  rv= alQueryValues(AL_SYSTEM,AL_DEVICES,x,16,0,0);
  if (rv>0) dump_resources(x,rv);
}


typedef struct {
  int dev,has_gains;
  ALfixed gains[MAX_CHANNELS];
  int srate;
} saved_info;

#define MAX_SAVED_DEVICES 16
static saved_info *si[MAX_SAVED_DEVICES];
static int saved_devices = 0;

static void save_devices(ALvalue *x,int rv)
{
  ALpv y;
  saved_info *sv;
  ALparamInfo pinf;
  ALfixed g[MAX_CHANNELS];
  int i,k;
  ALvalue z[16];
  int nres;
  if (saved_devices >= MAX_SAVED_DEVICES) return;
  for (i=0;i<rv;i++)
    {
      si[saved_devices] = (saved_info *)CALLOC(1,sizeof(saved_info));
      sv = si[saved_devices];
      sv->dev = x[i].i;
      y.param = AL_RATE;
      alGetParams(x[i].i,&y,1);
      sv->srate = y.value.i;
      saved_devices++;
      if (alIsSubtype(AL_DEVICE_TYPE, x[i].i)) 
        {
          alGetParamInfo(x[i].i,AL_GAIN,&pinf);
          if (pinf.min.ll != pinf.max.ll)
            {
              y.param = AL_GAIN;
              y.value.ptr = sv->gains;
              y.sizeIn = MAX_CHANNELS;
              alGetParams(x[i].i,&y,1);
              sv->has_gains = 1;
            }
          else sv->has_gains = 0;
          if ((nres = alQueryValues(x[i].i,AL_INTERFACE,z,16,0,0))>=0) save_devices(z,nres);
        }
    }
}

void save_audio_state (void) 
{
  int rv,i;
  ALvalue x[16];
  if (saved_devices>0)
    {
      for (i=0;i<saved_devices;i++)
        {
          if (si[i])
            {
              FREE(si[i]);
              si[i] = NULL;
            }
        }
    }
  saved_devices = 0;
  rv= alQueryValues(AL_SYSTEM,AL_DEVICES,x,16,0,0);
  if (rv>0) save_devices(x,rv);
}

void restore_audio_state (void) 
{
  int i;
  ALpv x;
  if (saved_devices > 0)
    {
      for (i=0;i<saved_devices;i++)
        {
          if (si[i])
            {
              if (si[i]->has_gains)
                {
                  x.param = AL_GAIN;
                  x.value.ptr = si[i]->gains;
                  x.sizeIn = MAX_CHANNELS;
                  alSetParams(si[i]->dev,&x,1);
                }
              x.param = AL_RATE;
              x.value.i = si[i]->srate;
              alSetParams(si[i]->dev,&x,1);
            }
        }
    }
}


#else

/* old audio library */

#define MAX_VOLUME 255

static int decode_field(int dev, int field, int chan)
{
  int fld = -1;
  switch (dev)
    {
    case DEFAULT_DEVICE: 
    case DAC_OUT_DEVICE:
    case READ_WRITE_DEVICE:
    case SPEAKERS_DEVICE:
      switch (field)
        {
        case AMP_FIELD:
          fld = ((chan == 0) ? AL_LEFT_SPEAKER_GAIN : AL_RIGHT_SPEAKER_GAIN);
          break;
        case SRATE_FIELD:
          fld = AL_OUTPUT_RATE;
          break;
        }
      break;
    case ADAT_IN_DEVICE: case ADAT_OUT_DEVICE:
    case AES_IN_DEVICE: case AES_OUT_DEVICE:
      AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE;
      break;
    case LINE_OUT_DEVICE:
      switch (field)
        {
        case SRATE_FIELD:
          fld = AL_OUTPUT_RATE; /* ? */
          break;
        }
      break;
    case DIGITAL_OUT_DEVICE:
      if (field == SRATE_FIELD)
        fld = AL_OUTPUT_RATE;
      else AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE;
      break;
    case DIGITAL_IN_DEVICE:
      if (field == SRATE_FIELD)
        fld = AL_INPUT_RATE;
      else AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE;
      break;
    case LINE_IN_DEVICE:
      if (field == AMP_FIELD)
        fld = ((chan == 0) ? AL_LEFT_INPUT_ATTEN : AL_RIGHT_INPUT_ATTEN);
      else
        {
          if (field == SRATE_FIELD)
            fld = AL_INPUT_RATE;
          else AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE;
        }
      break;
    case MICROPHONE_DEVICE:
      if (field == AMP_FIELD)
        fld = ((chan == 0) ? AL_LEFT2_INPUT_ATTEN : AL_RIGHT2_INPUT_ATTEN);
      else
        {
          if (field == SRATE_FIELD)
            fld = AL_INPUT_RATE;
          else AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE;
        }
      break;
    }
  return(fld);
}

int read_audio_state(int ur_dev, int field, int chan, float *val)
{
  long pb[4];
  long fld;
  int dev;
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  switch (field)
    {
    case CHANNEL_FIELD:
      val[0] = 2;
      break;
    case FORMAT_FIELD:  val[0] = 1; if (chan>1) val[1] = snd_16_linear; break;
    case DEVICE_FIELD:
      /* how to tell which machine we're on? */
      /* TODO: put default in/out devices first */
      val[0] = 4; 
      if (chan>1) val[1]=LINE_IN_DEVICE; 
      if (chan>2) val[2]=MICROPHONE_DEVICE; 
      if (chan>3) val[3]=DIGITAL_IN_DEVICE; 
      if (chan>4) val[4]=DAC_OUT_DEVICE;
      /* does this order work for digital input as well? (i.e. does it replace the microphone)? */
      break;
    case AMP_FIELD:
      fld = decode_field(dev,field,chan);
      if (fld != -1)
        {
          pb[0] = fld;
          al_err = ALgetparams(AL_DEFAULT_DEVICE,pb,2);
          if ((fld == AL_LEFT_SPEAKER_GAIN) || (fld == AL_RIGHT_SPEAKER_GAIN))
            val[0] = ((float)pb[1])/((float)MAX_VOLUME);
          else val[0] = 1.0 - ((float)pb[1])/((float)MAX_VOLUME);
          if (al_err) AUDIO_ERROR = READ_ERROR;    
        }
      break;
    case SRATE_FIELD:
      fld = decode_field(dev,field,chan);
      if (fld != -1)
        {
          pb[0] = fld;
          al_err = ALgetparams(AL_DEFAULT_DEVICE,pb,2);
          val[0] = pb[1];
          if (al_err) AUDIO_ERROR = READ_ERROR;    
        }
      break;
    default: AUDIO_ERROR = CANT_READ; break;
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val)
{
  long pb[4];
  long fld;
  int dev;
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  switch (field)
    {
    case DEVICE_FIELD:
      if (dev == DEFAULT_DEVICE)
        {
          pb[0] = AL_CHANNEL_MODE;
          pb[1] = ((chan == DIGITAL_IN_DEVICE) ? AL_STEREO : AL_4CHANNEL);
          pb[2] = AL_INPUT_SOURCE;
          pb[3] = ((chan == DIGITAL_IN_DEVICE) ? AL_INPUT_DIGITAL : AL_INPUT_MIC);
          al_err = ALsetparams(AL_DEFAULT_DEVICE,pb,4);
          if (al_err) AUDIO_ERROR = WRITE_ERROR;    
        }
      else AUDIO_ERROR = CANT_WRITE;
      break;
    case CHANNEL_FIELD:
      if (dev == MICROPHONE_DEVICE)
        {
          pb[0] =  AL_MIC_MODE;
          pb[1] = ((chan == 2) ? AL_STEREO : AL_MONO);
          al_err = ALsetparams(AL_DEFAULT_DEVICE,pb,2);
          if (al_err) AUDIO_ERROR = WRITE_ERROR;    
        }
      else
        {
          if (dev == DEFAULT_DEVICE)
            {
              pb[0] = AL_CHANNEL_MODE;
              pb[1] = ((chan == 4) ? AL_4CHANNEL : AL_STEREO);
              al_err = ALsetparams(AL_DEFAULT_DEVICE,pb,2);
              if (al_err) AUDIO_ERROR = WRITE_ERROR;    
            }
          else AUDIO_ERROR = CANT_WRITE;
        }
      break;
    case AMP_FIELD:
      fld = decode_field(dev,field,chan);
      if (fld != -1)
        {
          pb[0] = fld;
          if ((fld == AL_LEFT_SPEAKER_GAIN) || (fld == AL_RIGHT_SPEAKER_GAIN))
            pb[1] = val[0] * MAX_VOLUME;
          else pb[1] =  (1.0 - val[0])*MAX_VOLUME;
          al_err = ALsetparams(AL_DEFAULT_DEVICE,pb,2);
          if (al_err) AUDIO_ERROR = WRITE_ERROR;    
        }
      else AUDIO_ERROR = CANT_WRITE;
      break;
    case SRATE_FIELD:
      fld = decode_field(dev,field,chan);
      if (fld != -1)
        {
          pb[0] = fld;
          pb[1] = val[0];
          al_err = ALsetparams(AL_DEFAULT_DEVICE,pb,2);
          if (al_err) 
            AUDIO_ERROR = WRITE_ERROR;    
          else
            {
              if (fld == AL_INPUT_RATE)
                {
                  pb[0] = AL_OUTPUT_RATE;
                  pb[1] = val[0];
                  al_err = ALsetparams(AL_DEFAULT_DEVICE,pb,2);
                  if (al_err) AUDIO_ERROR = WRITE_ERROR;    
                }
            }
        }
      else AUDIO_ERROR = CANT_WRITE;
      break;
    default: AUDIO_ERROR = CANT_WRITE; break;
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

static void describe_audio_state_1(void) 
{
  float amps[1];
  int err;
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  err = read_audio_state(SPEAKERS_DEVICE,SRATE_FIELD,0,amps);
  if (err == NO_ERROR) {sprintf(strbuf,"srate: %.2f\n",amps[0]); pprint(strbuf);} else {fprintf(stdout,"err: %d!\n",err); fflush(stdout);}
  err = read_audio_state(SPEAKERS_DEVICE,AMP_FIELD,0,amps);
  if (err == NO_ERROR) {sprintf(strbuf,"speakers: %.2f",amps[0]); pprint(strbuf);}
  err = read_audio_state(SPEAKERS_DEVICE,AMP_FIELD,1,amps);
  if (err == NO_ERROR) {sprintf(strbuf," %.2f\n",amps[0]); pprint(strbuf);}
  err = read_audio_state(LINE_IN_DEVICE,AMP_FIELD,0,amps);
  if (err == NO_ERROR) {sprintf(strbuf,"line in: %.2f",amps[0]); pprint(strbuf);}
  err = read_audio_state(LINE_IN_DEVICE,AMP_FIELD,1,amps);
  if (err == NO_ERROR) {sprintf(strbuf," %.2f\n",amps[0]); pprint(strbuf);}
  err = read_audio_state(MICROPHONE_DEVICE,AMP_FIELD,0,amps);
  if (err == NO_ERROR) {sprintf(strbuf,"microphone: %.2f",amps[0]); pprint(strbuf);}
  err = read_audio_state(MICROPHONE_DEVICE,AMP_FIELD,1,amps);
  if (err == NO_ERROR) {sprintf(strbuf," %.2f\n",amps[0]); pprint(strbuf);}
  err = read_audio_state(LINE_OUT_DEVICE,AMP_FIELD,0,amps);
  if (err == NO_ERROR) {sprintf(strbuf,"line out: %.2f",amps[0]); pprint(strbuf);}
  err = read_audio_state(LINE_OUT_DEVICE,AMP_FIELD,1,amps);
  if (err == NO_ERROR) {sprintf(strbuf," %.2f\n",amps[0]); pprint(strbuf);}
  err = read_audio_state(DIGITAL_OUT_DEVICE,AMP_FIELD,0,amps);
  if (err == NO_ERROR) {sprintf(strbuf,"digital out: %.2f",amps[0]); pprint(strbuf);}
  err = read_audio_state(DIGITAL_OUT_DEVICE,AMP_FIELD,1,amps);
  if (err == NO_ERROR) {sprintf(strbuf," %.2f\n",amps[0]); pprint(strbuf);}
}

static long init_state[22];

void save_audio_state (void)
{
  /* save channel mode, input source, left/right atten (2), speaker gain, input/output rate, mic_mode */
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  init_state[0] = AL_CHANNEL_MODE;
  init_state[2] = AL_INPUT_SOURCE;
  init_state[4] = AL_LEFT_INPUT_ATTEN;
  init_state[6] = AL_RIGHT_INPUT_ATTEN;
  init_state[8] = AL_LEFT2_INPUT_ATTEN;
  init_state[10] = AL_RIGHT2_INPUT_ATTEN;
  init_state[12] = AL_MIC_MODE;
  init_state[14] = AL_LEFT_SPEAKER_GAIN;
  init_state[16] = AL_RIGHT_SPEAKER_GAIN;
  init_state[18] = AL_INPUT_RATE;
  init_state[20] = AL_OUTPUT_RATE;
  al_err = ALgetparams(AL_DEFAULT_DEVICE,init_state,22);
  if (al_err) AUDIO_ERROR = READ_ERROR;
}

void restore_audio_state (void)
{
  AUDIO_ERROR = NO_ERROR; al_err = 0;
  al_err = ALsetparams(AL_DEFAULT_DEVICE,init_state,22);
  if (al_err) AUDIO_ERROR = WRITE_ERROR;
}

 
#endif
/* new or old AL */

#endif
/* SGI */



/* ------------------------------- OSS ----------------------------------------- */

#if HAVE_OSS
#define AUDIO_OK

#include <sys/ioctl.h>

/* the system version of the soundcard header file may have no relation to the current OSS actually loaded */
/* sys/soundcard.h is usually just a pointer to linux/soundcard.h */

#if (USR_LIB_OSS)
  #include "/usr/lib/oss/include/sys/soundcard.h"
#else
  #if (USR_LOCAL_LIB_OSS)
    #include "/usr/local/lib/oss/include/sys/soundcard.h"
  #else
    #if defined(HAVE_SYS_SOUNDCARD_H) || defined(LINUX)
      #include <sys/soundcard.h>
    #else
      #if defined(HAVE_MACHINE_SOUNDCARD_H)
        #include <machine/soundcard.h>
      #else
        #include <soundcard.h>
      #endif
    #endif
  #endif
#endif

#if ((SOUND_VERSION > 360) && (defined(OSS_SYSINFO)))
/* apparently OSS is trying to hide its new structs from non-paying customers!
 * the OSS/Free version of soundcard.h has the current version number, but not the 
 * associated struct definitions.  This is not a good turn of events...
 */
  #define NEW_OSS
#endif

#define MAX_VOLUME 100

#define DAC_NAME "/dev/dsp"
#define MIXER_NAME "/dev/mixer"
#define SYNTH_NAME "/dev/music"

/* there can be more than one sound card installed, and a card can be handled through
 * more than one /dev/dsp device, so we can't use a global dac device here.
 * The caller has to keep track of the various cards (via AUDIO_SYSTEM) --
 * I toyed with embedding all that in open_audio_output and write_audio, but
 * decided it's better to keep them explicit -- the caller may want entirely
 * different (non-synchronous) streams going to the various cards.  This same
 * code (AUDIO_SYSTEM(n)->devn) should work in Windoze (see below), and
 * might work on the Mac and SGI -- something for a rainy day...
 */

static int FRAGMENTS = 4;
static int FRAGMENT_SIZE = 12;
static int fragments_locked = 0;

/* defaults here are FRAGMENTS 16 and FRAGMENT_SIZE 12; these values however
 * cause about a .5 second delay, which is not acceptable in "real-time" situations.
 */

void set_oss_buffers(int num,int size) {FRAGMENTS = num; FRAGMENT_SIZE = size; fragments_locked = 1;}

char *audio_error_name(int err) {return(audio_error_name_1(err));}

#define MAX_SOUNDCARDS 8
#define MAX_DSPS 8
#define MAX_MIXERS 8
/* there can be (apparently) any number of mixers and dsps per soundcard, but 8 is enough! */

static int audio_fd[MAX_SOUNDCARDS];
static int audio_open_ctr[MAX_SOUNDCARDS];
static int audio_dsp[MAX_SOUNDCARDS];
static int audio_mixer[MAX_SOUNDCARDS];
static int sound_cards = 0;
static int new_oss_running = 0;
static char dev_name[32];

int audio_systems(void) 
{
  return(sound_cards);
}

static char *mixer_name(int sys)
{
  if (sys < sound_cards)
    {
      sprintf(dev_name,"%s%d",MIXER_NAME,audio_mixer[sys]);
      return(dev_name);
    }
  return(DAC_NAME);
}

char *audio_system_name(int system) 
{
#ifdef NEW_OSS
  static mixer_info mixinfo;
  int status,ignored,fd;
  fd = open(mixer_name(system),O_RDONLY,0);
  if (fd != -1)
    {
      status = ioctl(fd,OSS_GETVERSION,&ignored);
      if (status == 0)
        {
          status = ioctl(fd,SOUND_MIXER_INFO,&mixinfo);
          if (status == 0) 
            {
              close(fd);
              return(mixinfo.name);
            }
        }
      close(fd);
    }
#endif
  return("OSS");
}

static char *dac_name(int sys, int offset)
{
  if ((sys < sound_cards) && (audio_mixer[sys] != -1))
    {
      sprintf(dev_name,"%s%d",DAC_NAME,audio_dsp[sys]+offset);
      return(dev_name);
    }
  return(DAC_NAME);
}

void setup_dsps(int cards, int *dsps, int *mixers)
{
  /* see note below -- there are still bugs in OSS multi-cards support, so sometimes the map has to be made by hand */
  int i;
  if (cards > MAX_SOUNDCARDS) cards = MAX_SOUNDCARDS;
  sound_cards = cards;
  for (i=0;i<cards;i++)
    {
      audio_dsp[i] = dsps[i];
      audio_mixer[i] = mixers[i];
    }
}

int initialize_audio(void) 
{
  /* here we need to set up the map of /dev/dsp and /dev/mixer to a given system */
  /* since this info is not passed to us by OSS, we have to work at it... */
  /* for the time being, I'll ignore auxiliary dsp and mixer ports (each is a special case) */
  int i,fd,md,err;
  char dname[16];
  int amp,old_mixer_amp,old_dsp_amp,new_mixer_amp;
  int devmask;
#ifdef NEW_OSS
  int status,ignored;
  oss_sysinfo sysinfo;
  int sysinfo_ok = 0;
#endif
  int num_mixers,num_dsps,nmix,ndsp;
  if (!audio_initialized)
    {
      audio_initialized = 1;
      AUDIO_ERROR = NO_ERROR; 
      for (i=0;i<MAX_SOUNDCARDS;i++)
        {
          audio_fd[i] = -1;
          audio_open_ctr[i] = 0;
          audio_dsp[i] = -1;
          audio_mixer[i] = -1;
        }
      num_mixers = MAX_MIXERS;
      num_dsps = MAX_DSPS;
#ifdef NEW_OSS
      fd = open(DAC_NAME,O_WRONLY,0);
      if (fd == -1) fd = open(SYNTH_NAME,O_RDONLY,0);
      if (fd == -1) fd = open(MIXER_NAME,O_RDONLY,0);
      if (fd != -1)
        {
          status = ioctl(fd,OSS_GETVERSION,&ignored);
          new_oss_running = (status == 0);
          if (new_oss_running)
            {
              status = ioctl(fd,OSS_SYSINFO,&sysinfo);
              sysinfo_ok = (status == 0);
            }
          if ((new_oss_running) && (sysinfo_ok))
            {
              num_mixers = sysinfo.nummixers;
              num_dsps = sysinfo.numaudios;
            }
        }
      close(fd);
#endif
      /* need to get which /dev/dsp lines match which /dev/mixer lines,
       *   find out how many separate systems (soundcards) are available,
       *   fill the audio_dsp and audio_mixer arrays with the system-related numbers,
       * since we have no way to tell from OSS info which mixers/dsps are the
       *   main ones, we'll do some messing aound to try to deduce this info.
       * for example, SB uses two dsp ports and two mixers pre card, whereas
       *  Ensoniq uses 2 dsps and 1 mixer.
       * 
       * the data we are gathering here:
       *   int audio_dsp[MAX_SOUNDCARDS] -> main_dsp_port[AUDIO_SYSTEM(n)] (-1 => no such system dsp)
       *   int audio_mixer[MAX_SOUNDCARDS] -> main_mixer_port[AUDIO_SYSTEM(n)]
       *   int sound_cards = 0 -> usable systems
       * all auxiliary ports are currently ignored (SB equalizer, etc)
       */
      sound_cards = 0;
      ndsp = 0;
      nmix = 0;
      while ((nmix<num_mixers) && (ndsp < num_dsps))
        {
          /* for each mixer, find associated main dsp (assumed to be first in /dev/dsp ordering) */
          /*   if mixer's dsp overlaps or we run out of dsps first, ignore it (aux mixer) */
          /* our by-guess-or-by-gosh method here is to try to open the mixer.
           *   if that fails, quit (if very first, try at least to get the dsp setup)
           *   find volume field, if none, go on, else read current volume
           *   open next unchecked dsp, try to set volume, read current, if different we found a match -- set and go on.
           *     if no change, move to next dsp and try again, if no more dsps, quit (checking for null case as before)
           */
          sprintf(dname,"%s%d",MIXER_NAME,nmix);
          md = open(dname,O_RDWR,0);
          if (md == -1) {if (errno == EBUSY) {sprintf(strbuf,"%s is busy: can't access it",dname); pprint(strbuf); break;}}
          sprintf(dname,"%s%d",DAC_NAME,ndsp);
          fd = open(dname,O_RDWR,0);
          if (fd == -1) {close(md); if (errno == EBUSY) fprintf(stderr,"%s is busy: can't access it\n",dname); break;}
          err = ioctl(md,SOUND_MIXER_READ_DEVMASK,&devmask);
          if ((!err) && (SOUND_MASK_VOLUME & devmask))
            {
              err = ioctl(md,MIXER_READ(SOUND_MIXER_VOLUME),&old_mixer_amp);
              if (!err)
                {
                  err = ioctl(fd,MIXER_READ(SOUND_MIXER_VOLUME),&old_dsp_amp);
                  if ((!err) && (old_dsp_amp == old_mixer_amp))
                    {
                      if (old_mixer_amp == 0) amp = 50; else amp = 0; /* 0..100 */
                      err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_VOLUME),&amp);
                      if (!err)
                        {
                          err = ioctl(md,MIXER_READ(SOUND_MIXER_VOLUME),&new_mixer_amp);
                          if (!err)
                            {
                              if (new_mixer_amp == amp)
                                {
                                  /* found one! */
                                  audio_dsp[sound_cards] = ndsp; ndsp++;
                                  audio_mixer[sound_cards] = nmix; nmix++;
                                  sound_cards++;
                                }
                              else ndsp++;
                              err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_VOLUME),&old_dsp_amp);
                            }
                          else nmix++;
                        }
                      else ndsp++;
                    }
                  else ndsp++;
                }
              else nmix++;
            }
          else nmix++;
          close(fd);
          close(md);
        }
      if (sound_cards == 0)
        {
          fd = open(DAC_NAME,O_WRONLY,0);
          if (fd != -1)
            {
              sound_cards = 1;
              audio_dsp[0] = 0;
              audio_mixer[0] = -1; /* hmmm -- need a way to see /dev/dsp as lonely outpost */
              close(fd);
            }
        }
    }
  return(0);
}

/* TODO: fix this to handle aux devices correctly */

static int linux_audio_open(const char *pathname, int flags, mode_t mode, int system)
{
  if (audio_fd[system] == -1) 
    {
      audio_fd[system] = open(pathname,flags,mode);
      audio_open_ctr[system] = 0;
    }
  else audio_open_ctr[system]++;
  return(audio_fd[system]);
}

static int find_system(int line)
{
  int i;
  for (i=0;i<MAX_SOUNDCARDS;i++)
    {
      if (line == audio_fd[i])
        return(i);
    }
  return(-1);
}

static int linux_audio_close(int fd)
{
  int err = 0,sys;
  if (fd != -1)
    {
      sys = find_system(fd);
      if (sys != -1)
        {
          if (audio_open_ctr[sys] > 0) 
            audio_open_ctr[sys]--;
          else 
            {
              err = close(fd);
              audio_open_ctr[sys] = 0;
              audio_fd[sys] = -1;
            }
        }
      else err = close(fd);
    }
  return(err);
}

static int error_exit(int error, int line)
{
  AUDIO_ERROR = error;
  if (line != -1) linux_audio_close(line);
  return(-1);
}

static int to_oss_format(int snd_format)
{
  switch (snd_format)
    {
    case snd_8_linear: return(AFMT_S8); break;
    case snd_16_linear: return(AFMT_S16_BE); break;
    case snd_8_unsigned: return(AFMT_U8); break;
    case snd_8_mulaw: return(AFMT_MU_LAW); break;
    case snd_8_alaw: return(AFMT_A_LAW); break;
    case snd_16_linear_little_endian: return(AFMT_S16_LE); break;
    case snd_16_unsigned: return(AFMT_U16_BE); break;
    case snd_16_unsigned_little_endian: return(AFMT_U16_LE); break;
    }
  return(-1);
}

int open_audio_output(int ur_dev, int srate, int chans, int format, int size)
{
  /* ur_dev is in general AUDIO_SYSTEM(n) | sndlib_DEVICE */
  int oss_format,stereo,buffer_info,audio_out,sys,dev;
  AUDIO_ERROR = NO_ERROR;
  sys = SNDLIB_SYSTEM(ur_dev);
  dev = SNDLIB_DEVICE(ur_dev);
  oss_format = to_oss_format(format); 
  if (oss_format == -1) return(error_exit(FORMAT_NOT_AVAILABLE,-1));
  if (chans == 2) stereo = 1; else stereo = 0;
  if (dev == DEFAULT_DEVICE)
    audio_out = linux_audio_open(dac_name(sys,0),O_WRONLY,0,sys);
  else audio_out = linux_audio_open(dac_name(sys,(dev == AUX_OUTPUT_DEVICE) ? 1 : 0),O_RDWR,0,sys);
  if (audio_out == -1) return(error_exit(CANT_OPEN,audio_out));
  /* ioctl(audio_out,SNDCTL_DSP_RESET,0); */ /* causes clicks */
  if ((dev == READ_WRITE_DEVICE) || (size != 0))
    {
      if (!fragments_locked) {if (srate > 30000) FRAGMENTS = 4; else FRAGMENTS = 2;}
      buffer_info = (FRAGMENTS<<16) | (FRAGMENT_SIZE);
      if (ioctl(audio_out,SNDCTL_DSP_SETFRAGMENT,&buffer_info) == -1)
        {
          /* older Linuces (or OSS's?) refuse to handle the fragment reset if O_RDWR used --
           * someone at OSS forgot to update the version number when this was fixed, so
           * I have no way to get around this except to try and retry...
           * 
           * But, if we hope to get input as well, we can't open this WRONLY.  This means
           * that in Linux, the real-time controls in CLM will be very sluggish if input is active.
           * We do what we can.
           */
          linux_audio_close(audio_out);
          audio_out = linux_audio_open(dac_name(sys,(dev == AUX_OUTPUT_DEVICE) ? 1 : 0),O_WRONLY,0,sys);
          if (audio_out == -1) return(error_exit(CANT_OPEN,audio_out));
          buffer_info = (FRAGMENTS<<16) | (FRAGMENT_SIZE);
          if (ioctl(audio_out,SNDCTL_DSP_SETFRAGMENT,&buffer_info) == -1) return(error_exit(CONFIGURATION_NOT_AVAILABLE,audio_out));
        }
    }
  if ((ioctl(audio_out,SNDCTL_DSP_SETFMT,&oss_format) == -1) || (oss_format != to_oss_format(format))) 
    return(error_exit(FORMAT_NOT_AVAILABLE,audio_out));
  if ((ioctl(audio_out,SNDCTL_DSP_STEREO,&stereo) == -1) || ((chans == 2) && (stereo == 0))) return(error_exit(CHANNELS_NOT_AVAILABLE,audio_out));
  if (ioctl(audio_out,SNDCTL_DSP_SPEED,&srate) == -1) return(error_exit(SRATE_NOT_AVAILABLE,audio_out));
  /* http://www.4front-tech.com/pguide/audio.html says this order has to be followed */
  return(audio_out);
}

int write_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR;
  write(line,buf,bytes);
  return(0);
}

int close_audio(int line)
{
  AUDIO_ERROR = NO_ERROR;
  return(linux_audio_close(line));
}

int read_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR;
  read(line,buf,bytes);
  return(0);
}

int open_audio_input(int ur_dev, int srate, int chans, int format, int requested_size)
{
  /* dev can be DEFAULT_DEVICE or READ_WRITE_DEVICE as well as the obvious others */
  int audio_fd,oss_format,stereo,buffer_info,sys,dev,srcbit,cursrc;
  AUDIO_ERROR = NO_ERROR;
  sys = SNDLIB_SYSTEM(ur_dev);
  dev = SNDLIB_DEVICE(ur_dev);
  oss_format = to_oss_format(format);
  if (oss_format == -1) return(error_exit(FORMAT_NOT_AVAILABLE,-1));
  audio_fd = linux_audio_open(dac_name(sys,(dev == AUX_INPUT_DEVICE) ? 1 : 0),O_RDWR,0,sys);
  if (audio_fd == -1)
    {
      if (dev == READ_WRITE_DEVICE) return(error_exit(CONFIGURATION_NOT_AVAILABLE,-1));
      if ((audio_fd = linux_audio_open(dac_name(sys,(dev == AUX_INPUT_DEVICE) ? 1 : 0),O_RDONLY,0,sys)) == -1)
        {
          if ((errno == EACCES) || (errno == ENOENT))
            {
              fprintf(stdout,"(to get input in Linux, we need read permission on /dev/dsp)");
              fflush(stdout);
              return(error_exit(NO_READ_PERMISSION,-1));
            }
          return(error_exit(NO_INPUT_AVAILABLE,-1));
        }
    }
  else ioctl(audio_fd,SNDCTL_DSP_SETDUPLEX,0); /* not always a no-op! */
  /* need to make sure the desired recording source is active -- does this actually have any effect? */
  switch (dev)
    {
    case MICROPHONE_DEVICE: srcbit = SOUND_MASK_MIC; break;
    case LINE_IN_DEVICE:    srcbit = SOUND_MASK_LINE; break;
    case LINE1_DEVICE:      srcbit = SOUND_MASK_LINE1; break;
    case LINE2_DEVICE:      srcbit = SOUND_MASK_LINE2; break;
    case LINE3_DEVICE:      srcbit = SOUND_MASK_LINE3; break; /* also digital1..3 */
    case READ_WRITE_DEVICE: 
    case DEFAULT_DEVICE:    srcbit = SOUND_MASK_LINE | SOUND_MASK_MIC; break;
    case CD_IN_DEVICE:      srcbit = SOUND_MASK_CD; break;
    default:                srcbit = 0; break;
      /* other possibilities: synth, radio, phonein */
    }
  ioctl(audio_fd,MIXER_READ(SOUND_MIXER_RECSRC),&cursrc);
  srcbit = (srcbit | cursrc);
  ioctl(audio_fd,MIXER_WRITE(SOUND_MIXER_RECSRC),&srcbit);
  if (chans == 2) stereo = 1; else stereo = 0;
  ioctl(audio_fd,SNDCTL_DSP_RESET,0);
  if (requested_size != 0)
    {
      buffer_info = (FRAGMENTS<<16) | (FRAGMENT_SIZE);
      ioctl(audio_fd,SNDCTL_DSP_SETFRAGMENT,&buffer_info);
    }
  if ((ioctl(audio_fd,SNDCTL_DSP_SETFMT,&oss_format) == -1) || (oss_format != to_oss_format(format)))
    return(error_exit(FORMAT_NOT_AVAILABLE,audio_fd));
  if ((ioctl(audio_fd,SNDCTL_DSP_STEREO,&stereo) == -1) || ((chans == 2) && (stereo == 0))) return(error_exit(CHANNELS_NOT_AVAILABLE,audio_fd));
  if (ioctl(audio_fd,SNDCTL_DSP_SPEED,&srate) == -1) return(error_exit(SRATE_NOT_AVAILABLE,audio_fd));
  return(audio_fd);
}


int read_audio_state(int ur_dev, int field, int chan, float *val)
{
  int fd,amp,channels,err,devmask,stereodevs,ind,formats,sys,dev;
  AUDIO_ERROR = NO_ERROR;
  sys = SNDLIB_SYSTEM(ur_dev);
  dev = SNDLIB_DEVICE(ur_dev);
  fd = linux_audio_open(mixer_name(sys),O_RDONLY | O_NONBLOCK,0,sys);
  if (fd == -1) return(error_exit(CANT_OPEN,-1));
  err = ioctl(fd,SOUND_MIXER_READ_DEVMASK,&devmask);
  if (err) return(error_exit(CONFIGURATION_NOT_AVAILABLE,fd));
  if ((dev == MIXER_DEVICE) || (dev == DAC_FILTER_DEVICE)) /* these give access to all the on-board analog input gain controls */
    {
      amp = 0;
      err = ioctl(fd,SOUND_MIXER_READ_DEVMASK,&devmask);
      switch (field)
        { 
          /* there's also a SOUND_MIXER_CD DIGITAL1..3 PHONEIN PHONEOUT VIDEO RADIO MONITOR -- except CD these skip the mixer */
          /* some of these are not in older OSS versions (350?) */
        case IMIX_FIELD:   if (SOUND_MASK_IMIX & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_IMIX),&amp);     break; /* recording monitor (soundcard.h) */
        case IGAIN_FIELD:  if (SOUND_MASK_IGAIN & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_IGAIN),&amp);   break;
        case RECLEV_FIELD: if (SOUND_MASK_RECLEV & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_RECLEV),&amp); break; /* recording level */
        case PCM_FIELD:    if (SOUND_MASK_PCM & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_PCM),&amp);       break;
        case PCM2_FIELD:   if (SOUND_MASK_ALTPCM & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_ALTPCM),&amp); break;
        case OGAIN_FIELD:  if (SOUND_MASK_OGAIN & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_OGAIN),&amp);   break;
        case LINE_FIELD:   if (SOUND_MASK_LINE & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE),&amp);     break;
        case MIC_FIELD:    if (SOUND_MASK_MIC & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_MIC),&amp);       break;
        case LINE1_FIELD:  if (SOUND_MASK_LINE1 & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE1),&amp);   break;
        case LINE2_FIELD:  if (SOUND_MASK_LINE2 & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE2),&amp);   break;
        case LINE3_FIELD:  if (SOUND_MASK_LINE3 & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE3),&amp);   break;
        case SYNTH_FIELD:  if (SOUND_MASK_SYNTH & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_SYNTH),&amp);   break;
        case BASS_FIELD:   if (SOUND_MASK_BASS & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_BASS),&amp);     break;
        case TREBLE_FIELD: if (SOUND_MASK_TREBLE & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_TREBLE),&amp); break;
        case CHANNEL_FIELD:
          if (dev == MIXER_DEVICE)
            {
              channels = 0;
              err = ioctl(fd,SOUND_MIXER_READ_STEREODEVS,&stereodevs);
              if (SOUND_MASK_IMIX & devmask) {if (SOUND_MASK_IMIX & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_IGAIN & devmask) {if (SOUND_MASK_IGAIN & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_RECLEV & devmask) {if (SOUND_MASK_RECLEV & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_PCM & devmask) {if (SOUND_MASK_PCM & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_ALTPCM & devmask) {if (SOUND_MASK_ALTPCM & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_OGAIN & devmask) {if (SOUND_MASK_OGAIN & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_LINE & devmask) {if (SOUND_MASK_LINE & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_MIC & devmask) {if (SOUND_MASK_MIC & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_LINE1 & devmask) {if (SOUND_MASK_LINE1 & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_LINE2 & devmask) {if (SOUND_MASK_LINE2 & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_LINE3 & devmask) {if (SOUND_MASK_LINE3 & stereodevs) channels += 2; else channels += 1;}
              if (SOUND_MASK_SYNTH & devmask) {if (SOUND_MASK_SYNTH & stereodevs) channels += 2; else channels += 1;}
            }
          else 
            if (SOUND_MASK_TREBLE & devmask) channels = 2; else channels = 0;
          val[0] = channels;
          linux_audio_close(fd);
          return(0);
          break;
        case FORMAT_FIELD: /* this is asking for configuration info -- we return an array with per-"device" channels */
          err = ioctl(fd,SOUND_MIXER_READ_STEREODEVS,&stereodevs);
          for (ind=0;ind<=SYNTH_FIELD;ind++) {if (chan>ind) val[ind]=0;}
          if (SOUND_MASK_IMIX & devmask) {if (chan>IMIX_FIELD) val[IMIX_FIELD] = ((SOUND_MASK_IMIX & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_IGAIN & devmask) {if (chan>IGAIN_FIELD) val[IGAIN_FIELD] = ((SOUND_MASK_IGAIN & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_RECLEV & devmask) {if (chan>RECLEV_FIELD) val[RECLEV_FIELD] = ((SOUND_MASK_RECLEV & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_PCM & devmask) {if (chan>PCM_FIELD) val[PCM_FIELD] = ((SOUND_MASK_PCM & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_ALTPCM & devmask) {if (chan>PCM2_FIELD) val[PCM2_FIELD] = ((SOUND_MASK_ALTPCM & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_OGAIN & devmask) {if (chan>OGAIN_FIELD) val[OGAIN_FIELD] = ((SOUND_MASK_OGAIN & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_LINE & devmask) {if (chan>LINE_FIELD) val[LINE_FIELD] = ((SOUND_MASK_LINE & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_MIC & devmask) {if (chan>MIC_FIELD) val[MIC_FIELD] = ((SOUND_MASK_MIC & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_LINE1 & devmask) {if (chan>LINE1_FIELD) val[LINE1_FIELD] = ((SOUND_MASK_LINE1 & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_LINE2 & devmask) {if (chan>LINE2_FIELD) val[LINE2_FIELD] = ((SOUND_MASK_LINE2 & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_LINE3 & devmask) {if (chan>LINE3_FIELD) val[LINE3_FIELD] = ((SOUND_MASK_LINE3 & stereodevs) ? 2 : 1);}
          if (SOUND_MASK_SYNTH & devmask) {if (chan>SYNTH_FIELD) val[SYNTH_FIELD] = ((SOUND_MASK_SYNTH & stereodevs) ? 2 : 1);}
          linux_audio_close(fd);
          return(0);
          break;
        default: AUDIO_ERROR = CANT_READ; break;
        }
      if (chan == 0)
        val[0] = ((float)(amp & 0xff))*0.01;
      else val[0] = (((float)((amp & 0xff00) >> 8))*0.01);
    }
  else
    {
      switch (field)
        {
        case DEVICE_FIELD:
          ind = 0;
          if ((SOUND_MASK_MIC | SOUND_MASK_LINE) & devmask) {ind++; if (chan>ind) val[ind] = LINE_IN_DEVICE;}
          /* problem here is that microphone and line_in are mixed before the ADC */
          if (SOUND_MASK_SPEAKER & devmask) {ind++; if (chan>ind) val[ind] = SPEAKERS_DEVICE;}
          if (SOUND_MASK_VOLUME & devmask) {ind++; if (chan>ind) val[ind] = DAC_OUT_DEVICE;}
          if (SOUND_MASK_IMIX & devmask) {ind++; if (chan>ind) val[ind] = MIXER_DEVICE;}
          if (SOUND_MASK_TREBLE & devmask) {ind++; if (chan>ind) val[ind] = DAC_FILTER_DEVICE;}
          /* DIGITAL1..3 as RECSRC(?) => DIGITAL_IN_DEVICE */
          val[0] = ind;
          break;
        case FORMAT_FIELD:
          err = ioctl(fd,SOUND_PCM_GETFMTS,&formats);
          ind = 0;
          if (formats & (to_oss_format(snd_8_linear))) {ind++; if (chan>ind) val[ind] = snd_8_linear;}
          if (formats & (to_oss_format(snd_16_linear))) {ind++; if (chan>ind) val[ind] = snd_16_linear;}
          if (formats & (to_oss_format(snd_8_unsigned))) {ind++; if (chan>ind) val[ind] = snd_8_unsigned;}
          if (formats & (to_oss_format(snd_8_mulaw))) {ind++; if (chan>ind) val[ind] = snd_8_mulaw;}
          if (formats & (to_oss_format(snd_8_alaw))) {ind++; if (chan>ind) val[ind] = snd_8_alaw;}
          if (formats & (to_oss_format(snd_16_linear_little_endian))) {ind++; if (chan>ind) val[ind] = snd_16_linear_little_endian;}
          if (formats & (to_oss_format(snd_16_unsigned))) {ind++; if (chan>ind) val[ind] = snd_16_unsigned;}
          if (formats & (to_oss_format(snd_16_unsigned_little_endian))) {ind++; if (chan>ind) val[ind] = snd_16_unsigned_little_endian;}
          val[0] = ind;
          break;
        case CHANNEL_FIELD:
          err = ioctl(fd,SOUND_MIXER_READ_STEREODEVS,&stereodevs);
          channels = 0;
          switch (dev)
            {
            case MICROPHONE_DEVICE: if (SOUND_MASK_MIC & devmask) {if (SOUND_MASK_MIC & stereodevs) channels = 2; else channels = 1;} break;
            case SPEAKERS_DEVICE: if (SOUND_MASK_SPEAKER & devmask) {if (SOUND_MASK_SPEAKER & stereodevs) channels = 2; else channels = 1;} break;
            case LINE_IN_DEVICE: if (SOUND_MASK_LINE & devmask) {if (SOUND_MASK_LINE & stereodevs) channels = 2; else channels = 1;} break;
            case LINE1_DEVICE: if (SOUND_MASK_LINE1 & devmask) {if (SOUND_MASK_LINE1 & stereodevs) channels = 2; else channels = 1;} break;
            case LINE2_DEVICE: if (SOUND_MASK_LINE2 & devmask) {if (SOUND_MASK_LINE2 & stereodevs) channels = 2; else channels = 1;} break;
            case LINE3_DEVICE: if (SOUND_MASK_LINE3 & devmask) {if (SOUND_MASK_LINE3 & stereodevs) channels = 2; else channels = 1;} break;
            case DAC_OUT_DEVICE: if (SOUND_MASK_VOLUME & devmask) {if (SOUND_MASK_VOLUME & stereodevs) channels = 2; else channels = 1;} break;
            case DEFAULT_DEVICE: if (SOUND_MASK_VOLUME & devmask) {if (SOUND_MASK_VOLUME & stereodevs) channels = 2; else channels = 1;} break;
            case READ_WRITE_DEVICE: 
              err = ioctl(fd,SNDCTL_DSP_GETCAPS,&ind);
              if (err != -1)
                channels = (ind & DSP_CAP_DUPLEX);
              else channels = 0;
              break;
            default: AUDIO_ERROR = DEVICE_NOT_AVAILABLE; break;
            }
          val[0] = channels;
          break;
        case AMP_FIELD:
          amp = 0;
          switch (dev)
            {
            case MICROPHONE_DEVICE: if (SOUND_MASK_MIC & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_MIC),&amp);       break;
            case SPEAKERS_DEVICE: if (SOUND_MASK_SPEAKER & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_SPEAKER),&amp); break; 
            case LINE_IN_DEVICE: if (SOUND_MASK_LINE & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE),&amp);        break; 
            case LINE1_DEVICE: if (SOUND_MASK_LINE1 & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE1),&amp);        break; 
            case LINE2_DEVICE: if (SOUND_MASK_LINE2 & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE2),&amp);        break; 
            case LINE3_DEVICE: if (SOUND_MASK_LINE3 & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_LINE3),&amp);        break; 
            case DAC_OUT_DEVICE: if (SOUND_MASK_VOLUME & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_VOLUME),&amp);    break;
            case DEFAULT_DEVICE: if (SOUND_MASK_VOLUME & devmask) err = ioctl(fd,MIXER_READ(SOUND_MIXER_VOLUME),&amp);    break;
            default: AUDIO_ERROR = DEVICE_NOT_AVAILABLE; break;
            }
          if (chan == 0)
            val[0] = ((float)(amp & 0xff))*0.01;
          else val[0] = (((float)((amp & 0xff00) >> 8))*0.01);
          break;
        default: AUDIO_ERROR = CANT_READ; break;
        }
    }
  linux_audio_close(fd);
  if (err) {AUDIO_ERROR = READ_ERROR; return(-1);}
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val)
{
  int fd,err,devmask,vol,sys,dev;
  float amp[1];
  AUDIO_ERROR = NO_ERROR;
  sys = SNDLIB_SYSTEM(ur_dev);
  dev = SNDLIB_DEVICE(ur_dev);
  fd = linux_audio_open(mixer_name(sys),O_RDWR | O_NONBLOCK,0,sys);
  if (fd == -1) return(error_exit(CANT_OPEN,-1));
  if ((dev == MIXER_DEVICE) || (dev == DAC_FILTER_DEVICE)) /* these give access to all the on-board analog input gain controls */
    {
      read_audio_state(dev,field,(chan == 0) ? 1 : 0,amp);
      if (val[0] >= 0.99) val[0] = 0.99;  if (val[0] < 0.0) val[0] = 0.0;
      if (amp[0] >= 0.99) amp[0] = 0.99;
      if (chan == 0)
        vol = (((int)(amp[0]*100)) << 8) + ((int)(val[0]*100));
      else vol = (((int)(val[0]*100)) << 8) + ((int)(amp[0]*100));
      err = ioctl(fd,SOUND_MIXER_READ_DEVMASK,&devmask);
      switch (field)
        {
        case IMIX_FIELD:   if (SOUND_MASK_IMIX & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_IMIX),&vol);     break;
        case IGAIN_FIELD:  if (SOUND_MASK_IGAIN & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_IGAIN),&vol);   break;
        case RECLEV_FIELD: if (SOUND_MASK_RECLEV & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_RECLEV),&vol); break;
        case PCM_FIELD:    if (SOUND_MASK_PCM & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_PCM),&vol);       break;
        case PCM2_FIELD:   if (SOUND_MASK_ALTPCM & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_ALTPCM),&vol); break;
        case OGAIN_FIELD:  if (SOUND_MASK_OGAIN & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_OGAIN),&vol);   break;
        case LINE_FIELD:   if (SOUND_MASK_LINE & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE),&vol);     break;
        case MIC_FIELD:    if (SOUND_MASK_MIC & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_MIC),&vol);       break;
        case LINE1_FIELD:  if (SOUND_MASK_LINE1 & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE1),&vol);   break;
        case LINE2_FIELD:  if (SOUND_MASK_LINE2 & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE2),&vol);   break;
        case LINE3_FIELD:  if (SOUND_MASK_LINE3 & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE3),&vol);   break;
        case SYNTH_FIELD:  if (SOUND_MASK_SYNTH & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_SYNTH),&vol);   break;
        case BASS_FIELD:   if (SOUND_MASK_BASS & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_BASS),&vol);     break;
        case TREBLE_FIELD: if (SOUND_MASK_TREBLE & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_TREBLE),&vol); break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
    }
  else
    {
      switch (field)
        {
        case AMP_FIELD:
          /* need to read both channel amps, then change the one we're concerned with */
          read_audio_state(dev,field,(chan == 0) ? 1 : 0,amp);
          if (val[0] >= 0.99) val[0] = 0.99;  if (val[0] < 0.0) val[0] = 0.0;
          if (amp[0] >= 0.99) amp[0] = 0.99;
          if (chan == 0)
            vol = (((int)(amp[0]*100)) << 8) + ((int)(val[0]*100));
          else vol = (((int)(val[0]*100)) << 8) + ((int)(amp[0]*100));
          err = ioctl(fd,SOUND_MIXER_READ_DEVMASK,&devmask);
          switch (dev)
            {
            case MICROPHONE_DEVICE: if (SOUND_MASK_MIC & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_MIC),&vol);       break;
            case SPEAKERS_DEVICE: if (SOUND_MASK_SPEAKER & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_SPEAKER),&vol); break;
            case LINE_IN_DEVICE: if (SOUND_MASK_LINE & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE),&vol);        break;
            case LINE1_DEVICE: if (SOUND_MASK_LINE1 & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE1),&vol);        break;
            case LINE2_DEVICE: if (SOUND_MASK_LINE2 & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE2),&vol);        break;
            case LINE3_DEVICE: if (SOUND_MASK_LINE3 & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_LINE3),&vol);        break;
            case DAC_OUT_DEVICE: if (SOUND_MASK_VOLUME & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_VOLUME),&vol);    break;
            case DEFAULT_DEVICE: if (SOUND_MASK_VOLUME & devmask) err = ioctl(fd,MIXER_WRITE(SOUND_MIXER_VOLUME),&vol);    break;
            default: AUDIO_ERROR = DEVICE_NOT_AVAILABLE; break;
            }
          break;
        case SRATE_FIELD:
          vol = (int)val[0];
          ioctl(fd,SNDCTL_DSP_RESET,0);  /* is this needed? */
          err = ioctl(fd,SNDCTL_DSP_SPEED,&vol);
          break;
        default: AUDIO_ERROR = CANT_WRITE; break;
          /* case FORMAT_FIELD: to force 16-bit input or give up */
          /* case CHANNEL_FIELD: to open as stereo if possible?? */
          /* case DEVICE_FIELD: to open digital out? */
        }
    }
  linux_audio_close(fd);
  if (err) {AUDIO_ERROR = WRITE_ERROR; return(-1);}
  return(0);
}

static char *synth_names[] = 
  {"",
   "Adlib","SoundBlaster","ProAudio Spectrum","Gravis UltraSound","MPU 401",
   "SoundBlaster 16","SoundBlaster 16 MIDI","6850 UART","Gravis UltraSound 16","Microsoft",
   "Personal sound system","Ensoniq Soundscape","Personal sound system + MPU","Personal/Microsoft",
   "Mediatrix Pro","MAD16","MAD16 + MPU","CS4232","CS4232 + MPU","Maui",
   "Pseudo-MSS","Gravis Ultrasound PnP","UART 401"};

static char *synth_name(int i)
{
  if ((i>0) && (i<=SNDCARD_UART401)) 
    return(synth_names[i]);
  return("unknown");
}

static char *device_types[] = {"FM","Sampling","MIDI"};

static char *device_type(int i)
{
  if ((i>=0) && (i<=2))
    return(device_types[i]);
  return("unknown");
}

static void yes_no (int condition)
{
  if (condition)  
    pprint("   yes    ");
  else pprint("   no     "); 
}

static int set_dsp(int fd, int channels, int bits, int *rate)
{
  int val;
  val = channels;
  ioctl(fd,SOUND_PCM_WRITE_CHANNELS,&val);
  if (val != channels) return(-1);
  val = bits;
  ioctl(fd,SOUND_PCM_WRITE_BITS,&val);
  if (val != bits) return(-1);
  ioctl(fd,SOUND_PCM_WRITE_RATE,rate);
  return(0);
}

static void describe_audio_state_1(void)
{
  /* this code taken largely from "Linux Multimedia Guide" by Jeff Tranter, (c) O'Reilly & Associates, Inc 1996 */
  /* it is explicitly released under the Gnu GPL, so I think I can use it here without elaborate disguises */
  int fd;
  int status,level,i,recsrc,devmask,recmask,stereodevs,caps,numdevs,rate,channels,bits,blocksize,formats,deffmt,min_rate,max_rate;
  struct synth_info sinfo;
  struct midi_info minfo;
  const char *sound_device_names[] = SOUND_DEVICE_LABELS;
  char dsp_name[16];
  char version[4];
  int dsp_num = 0;
#ifdef NEW_OSS
  mixer_info mixinfo;
  oss_sysinfo sysinfo;
#endif
  
  if (sound_cards <= 0) initialize_audio();

#ifdef NEW_OSS
  fd = open(DAC_NAME,O_WRONLY,0);
  if (fd == -1) fd = open(SYNTH_NAME,O_RDONLY,0);
  if (fd == -1) fd = open(MIXER_NAME,O_RDONLY,0);
  if (fd != -1)
    {
      status = ioctl(fd,OSS_GETVERSION,&level);
      new_oss_running = (status == 0);
      status = ioctl(fd,OSS_SYSINFO,&sysinfo);
      close(fd);
    }
#endif

  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  
  if (new_oss_running)
    {
#ifdef NEW_OSS
      if (status == 0)
        {
          sprintf(strbuf,"OSS version: %s\n",sysinfo.version);
          pprint(strbuf);
        }
      else
        {
          sprintf(strbuf,"OSS version: %x.%x.%x\n",(level>>16)&0xff,(level>>8)&0xff,level&0xff);
          pprint(strbuf);
        }
#else
      sprintf(strbuf,"OSS version: %x.%x.%x\n",(level>>16)&0xff,(level>>8)&0xff,level&0xff);
      pprint(strbuf);
#endif
    }
  else 
    {
      /* refers to the version upon compilation */
      sprintf(version,"%d",SOUND_VERSION);
      sprintf(strbuf,"OSS version: %c.%c.%c\n",version[0],version[1],version[2]);
      pprint(strbuf);
    }
  
  sprintf(strbuf,"%d card%s found",sound_cards,(sound_cards != 1) ? "s" : ""); pprint(strbuf);
  if (sound_cards > 1)
    {
      pprint(": ");
      for (i=0;i<sound_cards;i++)
        {
          sprintf(strbuf,"/dev/dsp%d with /dev/mixer%d%s",audio_dsp[i],audio_mixer[i],(i<(sound_cards-1)) ? ", " : "");
          pprint(strbuf);
        }
    }
  pprint("\n\n");

  fd = open(SYNTH_NAME,O_RDWR,0);
  if (fd == -1) fd = open(SYNTH_NAME,O_RDONLY,0);
  if (fd == -1) 
    {
      sprintf(strbuf,"%s: %s\n",SYNTH_NAME,strerror(errno)); pprint(strbuf); 
      pprint("no synth found\n"); 
    }
  else
    {
      /* ioctl(fd,SNDCTL_DSP_SETDUPLEX,0); */ /* try to enable "full duplex" mode -- appears to be a no-op */
      status = ioctl(fd,SNDCTL_SEQ_NRSYNTHS,&numdevs);
      if (status == -1) 
        {
          close(fd); 
          pprint("no sequencer?");
        }
      else
        {
          sprintf(strbuf,"/dev/sequencer: %d device%s installed\n",numdevs,(numdevs == 1) ? "" : "s"); 
          pprint(strbuf);
          for (i=0;i<numdevs;i++)
            {
              sinfo.device = i;
              status = ioctl(fd,SNDCTL_SYNTH_INFO,&sinfo);
              if (status != -1)
                {
                  sprintf(strbuf,"  device: %d: %s, %s, %d voices\n",i,sinfo.name,device_type(sinfo.synth_type),sinfo.nr_voices); 
                  pprint(strbuf);
                }
            }
          status = ioctl(fd,SNDCTL_SEQ_NRMIDIS,&numdevs);
          if (status == -1) 
            {
              close(fd); 
              pprint("no midi");
            }
          else
            {
              sprintf(strbuf,"  %d midi device%s installed\n",numdevs,(numdevs == 1) ? "" : "s"); 
              pprint(strbuf);
              for (i=0;i<numdevs;i++)
                {
                  minfo.device = i;
                  status = ioctl(fd,SNDCTL_MIDI_INFO,&minfo);
                  if (status != -1)
                    {
                      sprintf(strbuf,"  device %d: %s, %s\n",i,minfo.name,synth_name(minfo.dev_type)); 
                      pprint(strbuf);
                    }}}}}
  close(fd);
  pprint("--------------------------------\n");

MIXER_INFO:
  sprintf(dsp_name,"%s%d",MIXER_NAME,dsp_num);
  fd = linux_audio_open(dsp_name,O_RDWR,0,0);
  if (fd == -1) 
    {
      /* maybe output only */
      fd = linux_audio_open(dsp_name,O_WRONLY,0,0);
      if (fd == -1)
        {
          if (dsp_num == 0) 
            {
              /* is it possible to have /dev/dsp but not /dev/dsp0?? */
              sprintf(dsp_name,"%s",DAC_NAME);
              fd = linux_audio_open(dsp_name,O_RDWR,0,0);
              if (fd == -1) 
                {
                  /* maybe output only */
                  fd = linux_audio_open(dsp_name,O_WRONLY,0,0);
                  if (fd == -1)
                    {
                      pprint("no audio device found\n"); 
                      return;
                    }
                }
            }
          else goto AUDIO_INFO; /* no /dev/mixern */
        }
      else pprint("no audio input enabled\n"); 
    }

  status = ioctl(fd,SOUND_MIXER_READ_RECSRC,&recsrc);
  if (status == -1) 
    {
      linux_audio_close(fd); 
      pprint("no recsrc\n");
    }
  else
    {
      status = ioctl(fd,SOUND_MIXER_READ_DEVMASK,&devmask);
      if (status == -1) 
        {
          linux_audio_close(fd); 
          pprint("no devmask\n");
        }
      else
        {
          status = ioctl(fd,SOUND_MIXER_READ_RECMASK,&recmask);
          if (status == -1) 
            {
              linux_audio_close(fd); 
              pprint("no recmask\n");
            }
          else
            {
              status = ioctl(fd,SOUND_MIXER_READ_STEREODEVS,&stereodevs);
              if (status == -1) 
                {
                  linux_audio_close(fd); 
                  pprint("no stereodevs\n");
                }
              else
                {
                  status = ioctl(fd,SOUND_MIXER_READ_CAPS,&caps);
                  if (status == -1) 
                    {
                      linux_audio_close(fd); 
                      pprint("no caps\n");
                    }
                  else
                    {
#ifdef NEW_OSS
                      if (new_oss_running) status = ioctl(fd,SOUND_MIXER_INFO,&mixinfo);
#endif
                      sprintf(strbuf,"%s",dsp_name);
                      pprint(strbuf);
#ifdef NEW_OSS
                      if ((new_oss_running) && (status == 0)) 
                        {
                          sprintf(strbuf," (%s)",mixinfo.name);
                          pprint(strbuf);
                        }
#endif
                      pprint(":\n\n"
                             "  mixer     recording  active     stereo    current\n"
                             "  channel   source     source     device    level\n"
                             "  --------  --------   --------   --------   -------- \n"); 
                      for (i=0;i<SOUND_MIXER_NRDEVICES;i++)
                        {
                          if ((1<<i) & devmask)
                            {
                              sprintf(strbuf,"  %-10s",sound_device_names[i]); 
                              pprint(strbuf);
                              yes_no((1<<i) & recmask);
                              yes_no((1<<i) & recsrc);
                              yes_no((1<<i) & stereodevs);
                              status = ioctl(fd,MIXER_READ(i),&level);
                              if (status != -1)
                                {
                                  if ((1<<i) & stereodevs)
                                    sprintf(strbuf,"  %.2f %.2f",(float)(level&0xff) * 0.01,(float)((level&0xff00)>>8) * 0.01);
                                  else sprintf(strbuf,"  %.2f",(float)(level&0xff) * 0.01);
                                  /* can't use %% here because subsequent fprintf in pprint evaluates the %! #$@$! */
                                  pprint(strbuf);
                                }
                              pprint("\n"); 
                            }
                        }
                      pprint("--------------------------------\n");
                    }}}}}

AUDIO_INFO:
  linux_audio_close(fd);
  sprintf(dsp_name,"%s%d",DAC_NAME,dsp_num);
  fd = linux_audio_open(dsp_name,O_RDWR,0,0);
  if (fd == -1) return;
  sprintf(strbuf,"%s:\n\n",dsp_name); pprint(strbuf);
  if ((ioctl(fd,SOUND_PCM_READ_RATE,&rate) != -1) &&
      (ioctl(fd,SOUND_PCM_READ_CHANNELS,&channels) != -1) &&
      (ioctl(fd,SOUND_PCM_READ_BITS,&bits) != -1) &&
      (ioctl(fd,SNDCTL_DSP_GETBLKSIZE,&blocksize) != -1))
    {
      sprintf(strbuf,"  defaults:\n    sampling rate: %d, chans: %d, sample size: %d bits, block size: %d bytes\n",rate,channels,bits,blocksize); 
      pprint(strbuf);
      deffmt = AFMT_QUERY;
      if ((ioctl(fd,SOUND_PCM_SETFMT,&deffmt) != -1) &&
          (ioctl(fd,SOUND_PCM_GETFMTS,&formats) != -1))
        {
          pprint("  supported formats:\n"); 
          if (formats & AFMT_MU_LAW) {pprint("    mulaw"); if (deffmt == AFMT_MU_LAW) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_A_LAW)  {pprint("    alaw"); if (deffmt == AFMT_A_LAW) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_IMA_ADPCM) {pprint("    adpcm"); if (deffmt == AFMT_IMA_ADPCM) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_U8) {pprint("    unsigned byte"); if (deffmt == AFMT_U8) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_S16_LE) {pprint("    signed little-endian short"); if (deffmt == AFMT_S16_LE) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_S16_BE) {pprint("    signed big-endian short"); if (deffmt == AFMT_S16_BE) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_S8) {pprint("    signed byte"); if (deffmt == AFMT_S8) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_U16_LE) {pprint("    unsigned little-endian short"); if (deffmt == AFMT_U16_LE) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_U16_BE) {pprint("    unsigned big-endian short"); if (deffmt == AFMT_U16_BE) pprint(" (default)"); pprint("\n");}
          if (formats & AFMT_MPEG) {pprint("    mpeg 2"); if (deffmt == AFMT_MPEG) pprint(" (default)"); pprint("\n");}
          status = ioctl(fd,SNDCTL_DSP_GETCAPS,&caps);
          if (status != -1)
            {
              if (caps & DSP_CAP_DUPLEX) pprint("  full duplex\n");
              pprint("            sample        srate\n  channels   size      min      max\n");
              for (channels=1;channels<=2;channels++)
                {
                  for (bits=8;bits<=16;bits+=8)
                    {
                      min_rate = 1;
                      if (set_dsp(fd,channels,bits,&min_rate) == -1) continue;
                      max_rate = 100000;
                      if (set_dsp(fd,channels,bits,&max_rate) == -1) continue;
                      sprintf(strbuf,"  %4d  %8d  %8d  %8d\n",channels,bits,min_rate,max_rate); 
                      pprint(strbuf);
                    }}}}}
  pprint("--------------------------------\n");
  linux_audio_close(fd);
  dsp_num++; 
  if (dsp_num < 16)
    {
      sprintf(dsp_name,"%s%d",MIXER_NAME,dsp_num);
      goto MIXER_INFO;
    }
}

#define MIXER_SIZE SOUND_MIXER_NRDEVICES
static int mixer_state[MAX_SOUNDCARDS][MIXER_SIZE];
static int init_srate[MAX_SOUNDCARDS],init_chans[MAX_SOUNDCARDS],init_format[MAX_SOUNDCARDS];

void save_audio_state (void)
{
  int afd,i,devmask,err,level,system,systems;
  systems = audio_systems();
  for (system=0;system<systems;system++)
    {
      afd = linux_audio_open(mixer_name(system),O_RDONLY,0,0);
      if (afd == -1) {AUDIO_ERROR = CANT_OPEN; return;}
      ioctl(afd,SOUND_MIXER_READ_DEVMASK,&devmask);
      for (i=0;i<SOUND_MIXER_NRDEVICES;i++)
        {
          mixer_state[system][i] = 0;
          if ((1<<i) & devmask)
            {
              err = ioctl(afd,MIXER_READ(i),&level);
              if (err != -1) mixer_state[system][i] = level;
            }
        }
      ioctl(afd,SOUND_PCM_READ_RATE,&(init_srate[system]));
      ioctl(afd,SOUND_PCM_READ_CHANNELS,&(init_chans[system]));
      init_format[system] = AFMT_QUERY;
      ioctl(afd,SOUND_PCM_SETFMT,&(init_format[system]));
      linux_audio_close(afd);
    }
}

void restore_audio_state (void)
{
  int afd,i,level,devmask,system,systems;
  systems = audio_systems();
  for (system=0;system<systems;system++)
    {
      afd = linux_audio_open(mixer_name(system),O_RDWR,0,0);
      if (afd == -1) {AUDIO_ERROR = CANT_OPEN; return;}
      ioctl(afd,SOUND_PCM_WRITE_CHANNELS,&(init_chans[system]));
      ioctl(afd,SOUND_PCM_WRITE_RATE,&(init_srate[system]));
      ioctl(afd,SOUND_PCM_SETFMT,&(init_format[system]));
      ioctl(afd,SOUND_MIXER_READ_DEVMASK,&devmask);
      for (i=0;i<SOUND_MIXER_NRDEVICES;i++) 
        {
          if ((1<<i) & devmask)
            {
              level = mixer_state[system][i];
              ioctl(afd,MIXER_WRITE(i),&level);
            }
        }
      linux_audio_close(afd);
    }
}

void write_mixer_state(char *file)
{
  int fd,systems;
  fd = creat(file,0666);
  if (fd != -1)
    {
      systems = audio_systems();
      write(fd,(char *)mixer_state,MIXER_SIZE * sizeof(int) * systems);
      close(fd);
    }
}

void read_mixer_state(char *file)
{
  int fd,afd,i,level,devmask,system,systems;
  int vals[MAX_SOUNDCARDS][MIXER_SIZE];
  fd = open(file,O_RDONLY,0);
  if (fd != -1)
    {
      systems = audio_systems();
      read(fd,(char *)vals,MIXER_SIZE * sizeof(int) * systems);
      close(fd);
      for (system=0;system<systems;system++)
        {
          afd = linux_audio_open(mixer_name(system),O_RDONLY,0,0);
          if (afd != -1)
            {
              ioctl(afd,SOUND_MIXER_READ_DEVMASK,&devmask);
              for (i=0;i<SOUND_MIXER_NRDEVICES;i++) 
                {
                  if ((1<<i) & devmask)
                    {
                      level = vals[system][i];
                      ioctl(afd,MIXER_WRITE(i),&level);
                    }
                }
              linux_audio_close(afd);
            }
        }
    }
}

#endif



/* -------------------------------- NEXT -------------------------------- */

#ifdef NEXT
#define AUDIO_OK

#include <sound/sound.h>
#include <sound/sounddriver.h>
#include <sound/snddriver_client.h>
#include <mach/mach.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

static int low_water = 48*1024;
static int high_water = 64*1024;
static port_t dev_port = 0;
static port_t owner_port = 0;
static port_t write_port = 0;
static port_t read_port = 0;
static port_t reply_port = 0;
static short *rtbuf = NULL;
static int rtbuf_size = 0;
static char *readbuf;
#define WRITE_TAG 1
#define READ_TAG 1
#define BYTES_PER_SAMPLE 2
#define WRITE_COMPLETED_MSG 1
#define OUTPUT_LINE 1
#define INPUT_LINE 2

static int nxt_err = 0;
static char errstr[128];

char *audio_error_name(int err) 
{
  if (!nxt_err) return(audio_error_name_1(err));
  sprintf(errstr,"%s: %s",audio_error_name_1(err),SNDSoundError(nxt_err));
  return(errstr);
}

int audio_systems(void) {return(1);}
char *audio_system_name(int system) {return("NeXT");}

static int waiting = 0;

static void write_completed(void *arg, int tag) 
{
  if (tag == WRITE_TAG) waiting--;
}

int initialize_audio(void) {AUDIO_ERROR = NO_ERROR; return(0);}
static int output_chans = 0;
static snddriver_handlers_t replyHandlers;
static msg_header_t *reply_msg;

int open_audio_output(int ur_dev, int srate, int chans, int format, int size)
{
  int sr,i,dev;
  int protocol = 0;
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  if (format != COMPATIBLE_FORMAT) {AUDIO_ERROR = FORMAT_NOT_AVAILABLE; return(-1);}
  if (size == 0) size = 1024;
  /* large buffers (4096) can cause bus errors! */
  if (rtbuf == NULL) 
    {
      rtbuf = (short *)CALLOC(size,sizeof(short));
      rtbuf_size = size;
    }
  else 
    {
      if (size > rtbuf_size)
        {
          FREE(rtbuf);
          rtbuf = (short *)CALLOC(size,sizeof(short));
          rtbuf_size = size;
        }
      for (i=0;i<size;i++) rtbuf[i] = 0;
    }
  nxt_err = SNDAcquire(SND_ACCESS_OUT,0,0,0,NULL_NEGOTIATION_FUN,0,&dev_port,&owner_port); 
  if (nxt_err == 0)
    {
      if (srate > 30000) sr = SNDDRIVER_STREAM_TO_SNDOUT_44; else sr = SNDDRIVER_STREAM_TO_SNDOUT_22;
      nxt_err = snddriver_stream_setup(dev_port,owner_port,sr,size,BYTES_PER_SAMPLE,low_water,high_water,&protocol,&write_port);
      output_chans = chans;
      waiting = 0;
      if (nxt_err == 0)
        {
          reply_msg = (msg_header_t *)malloc(MSG_SIZE_MAX);
          nxt_err = port_allocate(task_self(),&reply_port); 
          if (nxt_err) {AUDIO_ERROR = CANT_OPEN; return(-1);}
          reply_msg->msg_size = MSG_SIZE_MAX;
          reply_msg->msg_local_port = reply_port;
          replyHandlers.completed = write_completed;
          for (sr=0;sr<4;sr++)
            {
              nxt_err = snddriver_stream_start_writing(write_port,rtbuf,size,WRITE_TAG,0,0,0,WRITE_COMPLETED_MSG,0,0,0,0,reply_port);
              if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
              waiting++;
            }
        }
    }
  if (nxt_err) {AUDIO_ERROR = CANT_OPEN; return(-1);}
  return(OUTPUT_LINE);
}

int write_audio(int line, char *buf, int bytes)
{
  int i,j,samps,err;
#ifdef CLM_LITTLE_ENDIAN
  char tmp;
#endif
  short *sbuf;
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  if (line != OUTPUT_LINE) {AUDIO_ERROR = CANT_WRITE; return(-1);}
  sbuf = (short *)buf;
  samps = (bytes>>1);
#ifdef CLM_LITTLE_ENDIAN
  /* "sound" format in NextStep is always big-endian */
  for (i=0;i<bytes;i+=2)
    {
      tmp = buf[i+1];
      buf[i+1] = buf[i];
      buf[i] = tmp;
    }
#endif
  reply_msg->msg_size = MSG_SIZE_MAX;
  reply_msg->msg_local_port = reply_port;
  if (output_chans == 2)
    {
      nxt_err = snddriver_stream_start_writing(write_port,sbuf,samps,WRITE_TAG,0,0,0,WRITE_COMPLETED_MSG,0,0,0,0,reply_port); 
      if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
      waiting++;
      nxt_err = msg_receive(reply_msg,RCV_TIMEOUT,0);
      if (nxt_err == RCV_SUCCESS) nxt_err = snddriver_reply_handler(reply_msg,&replyHandlers);
      if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
    }
  else
    {
      /* since output is always stereo on the Next, we have to fake up the second channel in the 1-channel case */
      j = 0;
      for (i=0;i<samps;i++)
        {
          rtbuf[j++] = sbuf[i];
          rtbuf[j++] = sbuf[i];
          if (j == rtbuf_size)
            {
              j = 0;
              nxt_err = snddriver_stream_start_writing(write_port,rtbuf,rtbuf_size,WRITE_TAG,0,0,0,WRITE_COMPLETED_MSG,0,0,0,0,reply_port); 
              if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
              waiting++;
              nxt_err = msg_receive(reply_msg,RCV_TIMEOUT,0);
              if (nxt_err == RCV_SUCCESS) nxt_err = snddriver_reply_handler(reply_msg,&replyHandlers);
              if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
            }
        }
      if (j > 0)
        {
          nxt_err = snddriver_stream_start_writing(write_port,rtbuf,j,WRITE_TAG,0,0,0,WRITE_COMPLETED_MSG,0,0,0,0,reply_port); 
          if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
          waiting++;
          nxt_err = msg_receive(reply_msg,RCV_TIMEOUT,0);
          if (nxt_err == RCV_SUCCESS) nxt_err = snddriver_reply_handler(reply_msg,&replyHandlers);
          if (nxt_err) {AUDIO_ERROR = CANT_WRITE; return(-1);}
        }
    }
  return(0);
}

int close_audio(int line)
{
  int ctr,oldwait;
  ctr = 0;
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  if (dev_port) 
    {
      if (line == OUTPUT_LINE)
          {
            while (waiting)
              {
                oldwait = waiting;
                nxt_err = msg_receive(reply_msg,RCV_TIMEOUT,1);
                if (nxt_err == RCV_SUCCESS) 
                  nxt_err = snddriver_reply_handler(reply_msg,&replyHandlers);
                else if (nxt_err == RCV_TIMEOUT) break;
                if (nxt_err) {AUDIO_ERROR = CANT_CLOSE; return(-1);}
                if (oldwait == waiting) ctr++; else ctr=0;
                /* messages seem to be dropped at random on the Next, so we need a fail-safe way to break out of this loop */
                if (ctr > 1000) break;
              }
            if (reply_msg) FREE(reply_msg);
            nxt_err = SNDRelease(SND_ACCESS_OUT,dev_port,owner_port);
            if (nxt_err) {AUDIO_ERROR = CANT_CLOSE; return(-1);}
            port_deallocate(task_self(),reply_port);
          }
      else
        {
          if (line == INPUT_LINE)
            {
              if (reply_msg) FREE(reply_msg);
              nxt_err = SNDRelease(SND_ACCESS_IN,dev_port,owner_port);
              if (nxt_err) {AUDIO_ERROR = CANT_CLOSE; return(-1);}
              port_deallocate(task_self(),reply_port);
            }
          else {AUDIO_ERROR = CANT_CLOSE; return(-1);}
        }
    }
  if (nxt_err) {AUDIO_ERROR = CANT_CLOSE; return(-1);}
  return(0);
}


/* here we'll block if data not ready for read */
/* also since it's the next built-in microphone, we get mulaw data at 8KHz */

static void read_completed(void *arg, int tag, void *inp, int bytes)
{
  int i;
  if (tag == READ_TAG)
    {
      for (i=0;i<bytes;i++) readbuf[i] = ((char *)inp)[i];
    }
}

/* as far as I can tell, you only get one read from the NeXT, so it better be a monster...*/

int open_audio_input(int ur_dev, int srate, int chans, int format, int size) 
{
  int protocol,dev;
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  if (format != snd_8_mulaw) {AUDIO_ERROR = FORMAT_NOT_AVAILABLE; return(-1);}
  if ((srate != 8000) && (srate != 8012)) {AUDIO_ERROR = SRATE_NOT_AVAILABLE; return(-1);}
  if (size == 0) size = 1024;
  nxt_err = SNDAcquire(SND_ACCESS_IN,0,0,0,NULL_NEGOTIATION_FUN,0,&dev_port,&owner_port); 
  if (nxt_err == 0)
    {
      nxt_err = snddriver_stream_setup(dev_port,owner_port,SNDDRIVER_STREAM_FROM_SNDIN,size,1,low_water,high_water,&protocol,&read_port);
      if (nxt_err == 0)
        {
          reply_msg = (msg_header_t *)malloc(MSG_SIZE_MAX);
          reply_msg->msg_size = MSG_SIZE_MAX;
          nxt_err = port_allocate(task_self(),&reply_port); 
          if (nxt_err) {AUDIO_ERROR = CANT_OPEN; return(-1);}
          reply_msg->msg_local_port = reply_port;
          replyHandlers.recorded_data = read_completed;
          nxt_err = snddriver_stream_start_reading(read_port,0,size,READ_TAG,0,0,0,0,0,0,reply_port);
        }
    }
  if (nxt_err) {AUDIO_ERROR = CANT_OPEN; return(-1);}
  return(INPUT_LINE);
}

int read_audio(int line, char *buf, int bytes) 
{
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  if (line != INPUT_LINE) {AUDIO_ERROR = CANT_READ; return(-1);}
  reply_msg->msg_size = MSG_SIZE_MAX;
  reply_msg->msg_local_port = reply_port;
  nxt_err = msg_receive(reply_msg,MSG_OPTION_NONE,0);
  if (nxt_err == RCV_SUCCESS) 
    {
      readbuf = buf;
      nxt_err = snddriver_reply_handler(reply_msg,&replyHandlers);
    }
  if (nxt_err) {AUDIO_ERROR = CANT_READ; return(-1);}
  return(0);
}

#define MAX_VOLUME 0x2b

int read_audio_state(int ur_dev, int field, int chan, float *val)
{
  int left,right,dev;
  int amps[2];
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  if ((dev == DAC_FILTER_DEVICE) && ((field == TREBLE_FIELD) || (field == BASS_FIELD)))
    {
      SNDGetFilter(&left);
      val[0] = left;
    }
  else
    {
      if ((dev == SPEAKERS_DEVICE) && (field == AMP_FIELD))
        {
          SNDGetMute(&left);
          val[0] = left;
        }
      else
        {
          switch (field)
            {
            case DEVICE_FIELD:  val[0] = 2; if (chan>1) val[1] = MICROPHONE_DEVICE; if (chan>2) val[2] = DAC_OUT_DEVICE; break;
            case FORMAT_FIELD:  val[0] = 1; if (chan>1) val[1] = snd_16_linear; break;
            case SRATE_FIELD:   val[0] = 3; val[1] = 8000; val[2] = 22050; val[3] = 44100; break;
            case CHANNEL_FIELD: val[0] = 2; break;
            case AMP_FIELD:
              if ((dev == DAC_OUT_DEVICE) || (dev == DEFAULT_DEVICE))
                {
                  SNDGetVolume(&left,&right);
                  /* lower level snddriver_get_device_parms doesn't work in NeXTStep/Intel */
                  if (chan == 0) 
                    val[0] = (float)left/(float)MAX_VOLUME;
                  else val[0] = (float)right/(float)MAX_VOLUME;
                }
              else AUDIO_ERROR = DEVICE_NOT_AVAILABLE;
              break;
            default: AUDIO_ERROR = CANT_READ; break;
            }
        }
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val)
{
  int left,right,dev;
  AUDIO_ERROR = NO_ERROR; nxt_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  if ((dev == DAC_FILTER_DEVICE) && ((field == TREBLE_FIELD) || (field == BASS_FIELD)))
    {
      SNDSetFilter(val[0]);
    }
  else
    {
      if ((dev == SPEAKERS_DEVICE) && (field == AMP_FIELD))
        {
          SNDSetMute(val[0]);
        }
      else
        {
          if (field != AMP_FIELD)
            AUDIO_ERROR = WRITE_ERROR;
          else
            {
              if ((dev == DAC_OUT_DEVICE) || (dev == DEFAULT_DEVICE))
                {
                  SNDGetVolume(&left,&right);
                  if (chan == 0)
                    left = (int)(val[0] * MAX_VOLUME);
                  else right = (int)(val[0] * MAX_VOLUME);
                  SNDSetVolume(left,right);
                }
              else AUDIO_ERROR = CANT_WRITE;
            }
        }
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

static void describe_audio_state_1(void)
{
  /* get volume -- not much else we can do here */
  float amps[1];
  float val;
  if (!strbuf) strbuf = (char *)CALLOC(1024,sizeof(char));
  read_audio_state(DEFAULT_DEVICE,AMP_FIELD,0,amps);
  val = amps[0];
  read_audio_state(DEFAULT_DEVICE,AMP_FIELD,1,amps);
  sprintf(strbuf,"speaker vols: %.2f %.2f\n",val,amps[0]);
  pprint(strbuf);
  read_audio_state(DAC_FILTER_DEVICE,TREBLE_FIELD,0,amps);
  pprint("dac lowpass: ");
  pprint((amps[0] == 0.0) ? "off\n" : "on\n");
  read_audio_state(SPEAKERS_DEVICE,AMP_FIELD,0,amps);
  pprint("speakers: ");
  pprint((amps[0] == 0) ? "off\n" : "on\n");
}

static int saved_left,saved_right,saved_mute,saved_filter;

void save_audio_state (void) 
{
  SNDGetVolume(&saved_left,&saved_right);
  SNDGetMute(&saved_mute);
  SNDGetFilter(&saved_filter);
}

void restore_audio_state (void) 
{
  SNDSetVolume(saved_left,saved_right);
  SNDSetMute(saved_mute);
  SNDSetFilter(saved_filter);
}

#endif



/* -------------------------------- SUN -------------------------------- */
/*
 * NetBSD and OpenBSD are apparently very similar.
 *
 * Thanks to Seppo Ingalsuo for several bugfixes.
 */

#if defined(SUN) || defined(SOLARIS) || defined(SUNOS) || defined(OPENBSD)
#define AUDIO_OK

#ifdef SUNOS
  #include <sun/audioio.h>
#else
  #include <sys/audioio.h>
#endif

int initialize_audio(void) {AUDIO_ERROR = NO_ERROR; return(0);}
char *audio_error_name(int err) {return(audio_error_name_1(err));}
int audio_systems(void) {return(1);}
char *audio_system_name(int system) {return("Sun");}

#ifdef OPENBSD
  #define DAC_NAME "/dev/sound"
#else
  #define DAC_NAME "/dev/audio"
#endif
#define AUDIODEV_ENV "AUDIODEV"

static int error_exit(int error, int line)
{
  AUDIO_ERROR = error;
  if (line != -1) close(line);
  return(-1);
}

static int to_sun_format(int format)
{
  switch (format)
    {
    case snd_16_linear: 
#ifdef OPENBSD
      return(AUDIO_ENCODING_PCM16); 
#else
      return(AUDIO_ENCODING_LINEAR); 
#endif
      break;
    case snd_8_linear: 
#if defined(SOLARIS) && defined(AUDIO_ENCODING_LINEAR8)
      return(AUDIO_ENCODING_LINEAR8); break;
#else
  #ifdef OPENBSD
      return(AUDIO_ENCODING_PCM8); 
  #else
      return(AUDIO_ENCODING_LINEAR);
  #endif 
      break;
#endif
    case snd_8_mulaw: return(AUDIO_ENCODING_ULAW); break;
    case snd_8_alaw: return(AUDIO_ENCODING_ALAW); break;
    }
  return(-1);
}

static int audio_fd = -1;

int open_audio_output(int ur_dev, int srate, int chans, int format, int size)
{
  /* may want to set play|record.buffer_size */
  struct audio_info info;
  char *dev_name;
  int encode,bits,dev;
  dev = SNDLIB_DEVICE(ur_dev);
  if (audio_fd == -1)
    {
      AUDIO_ERROR = NO_ERROR;
      errno = 0;
      encode = to_sun_format(format);
      if (encode == -1) return(error_exit(FORMAT_NOT_AVAILABLE,-1));
      if (getenv(AUDIODEV_ENV) != NULL) dev_name = getenv(AUDIODEV_ENV); else dev_name = DAC_NAME;
      if (dev != READ_WRITE_DEVICE)
        audio_fd = open(dev_name,O_WRONLY,0);
      else audio_fd = open(dev_name,O_RDWR,0);
      if (audio_fd == -1) 
        {
          if (errno == EBUSY) return(error_exit(OUTPUT_BUSY,-1));
          return(error_exit(CANT_OPEN,-1));
        }
      AUDIO_INITINFO(&info);
      if (dev == LINE_OUT_DEVICE)
        info.play.port = AUDIO_LINE_OUT;
      else
        if (dev == SPEAKERS_DEVICE)
          info.play.port = AUDIO_SPEAKER;
        else 
          if (dev != DEFAULT_DEVICE) 
            info.play.port = AUDIO_HEADPHONE;  /* add a headphone device choice? */
      info.play.sample_rate = srate; 
      info.play.channels = chans;
      bits = 8 * c_snd_datum_size(format);
      info.play.precision = bits;
      info.play.encoding = encode;
      ioctl(audio_fd,AUDIO_SETINFO,&info);
      if (info.play.channels != chans) return(error_exit(CHANNELS_NOT_AVAILABLE,audio_fd));
      if ((info.play.precision != bits) || (info.play.encoding != encode)) return(error_exit(FORMAT_NOT_AVAILABLE,audio_fd));
    }
  return(audio_fd);
}

int write_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR;
  write(line,buf,bytes);
  return(0);
}

int close_audio(int line)
{
  AUDIO_ERROR = NO_ERROR;
  close(line);
  audio_fd = -1;
  return(0);
}

int read_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR;
  read(line,buf,bytes);
  return(0);
}

int open_audio_input(int ur_dev, int srate, int chans, int format, int size)
{
  struct audio_info info;
  int indev,encode,bits,dev;
  char *dev_name;
  dev = SNDLIB_DEVICE(ur_dev);
  if (audio_fd == -1)
    {
      AUDIO_ERROR = NO_ERROR;
      errno = 0;
      encode = to_sun_format(format);
      if (encode == -1) return(error_exit(FORMAT_NOT_AVAILABLE,-1));
      if (getenv(AUDIODEV_ENV) != NULL) dev_name = getenv(AUDIODEV_ENV); else dev_name = DAC_NAME;
      if (dev != READ_WRITE_DEVICE)
        audio_fd = open(dev_name,O_RDONLY,0);
      else audio_fd = open(dev_name,O_RDWR,0);
      if (audio_fd == -1) 
        {
          if (errno == EBUSY) return(error_exit(INPUT_BUSY,-1));
          return(error_exit(CANT_OPEN,-1));
        }
      AUDIO_INITINFO(&info);
      info.record.sample_rate = srate;
      info.record.channels = chans;
      bits = 8 * c_snd_datum_size(format);
      info.play.precision = bits;
      info.play.encoding = encode;
      if (dev == LINE_IN_DEVICE) indev = AUDIO_LINE_IN; else indev = AUDIO_MICROPHONE;
      info.record.port = indev;
      ioctl(audio_fd, AUDIO_SETINFO, &info);
      if (info.record.port != indev) return(error_exit(DEVICE_NOT_AVAILABLE,audio_fd));
      if (info.record.channels != chans) return(error_exit(CHANNELS_NOT_AVAILABLE,audio_fd));
      if ((info.record.precision != bits) || (info.record.encoding != encode)) return(error_exit(FORMAT_NOT_AVAILABLE,audio_fd));
    }
  return(audio_fd);
}

int read_audio_state(int ur_dev, int field, int chan, float *val) 
{
  struct audio_info info;
  int dev;
  char *dev_name;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  AUDIO_INITINFO(&info);
  if (audio_fd != -1)
    ioctl(audio_fd,AUDIO_GETINFO,&info);
  else
    {
      if (getenv(AUDIODEV_ENV) != NULL) dev_name = getenv(AUDIODEV_ENV); else dev_name = DAC_NAME;
      audio_fd = open(dev_name,O_RDONLY,0);
      ioctl(audio_fd, AUDIO_GETINFO, &info);
      close_audio(audio_fd);
    }
  if (field == DEVICE_FIELD)
    {
      val[0] = 2; if (chan>1) val[1] = MICROPHONE_DEVICE; if (chan>2) val[2] = DAC_OUT_DEVICE;
      /* CD_INTERNAL ? */
    }
  else
    {
      if (field == FORMAT_FIELD)  /* this actually depends on the audio device */
        {
          val[0] = 3; 
          if (chan>1) val[1] = snd_16_linear;
          if (chan>2) val[2] = snd_8_mulaw;
          if (chan>3) val[3] = snd_8_alaw;
        }
      else
        {
          switch (dev)
            {
            case DEFAULT_DEVICE:
            case DAC_OUT_DEVICE:
            case SPEAKERS_DEVICE:
            case LINE_OUT_DEVICE:
              switch (field)
                {
                case AMP_FIELD: val[0] = ((float)info.play.gain / (float)AUDIO_MAX_GAIN); break;
                case CHANNEL_FIELD: val[0] = (float)info.play.channels; break;
                case SRATE_FIELD: val[0] = (float)info.play.sample_rate; break;
                default: AUDIO_ERROR = CANT_READ; break;
                }
              break;
            case MICROPHONE_DEVICE:
            case LINE_IN_DEVICE:
            case READ_WRITE_DEVICE:
              switch (field)
                {
                case AMP_FIELD: val[0] = ((float)info.record.gain / (float)AUDIO_MAX_GAIN); break;
                case CHANNEL_FIELD: val[0] = (float)info.record.channels; break;
                case SRATE_FIELD: val[0] = (float)info.record.sample_rate; break;
                default: AUDIO_ERROR = CANT_READ; break;
                }
              break;
            default: AUDIO_ERROR = CANT_READ; break;
            }
        }
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val) 
{
  struct audio_info info;
  int dev,need_close=0;
  char *dev_name;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  AUDIO_INITINFO(&info);
  if (audio_fd != -1)
    ioctl(audio_fd,AUDIO_GETINFO,&info);
  else 
    {
      if (getenv(AUDIODEV_ENV) != NULL) dev_name = getenv(AUDIODEV_ENV); else dev_name = DAC_NAME;
      audio_fd = open(dev_name,O_RDWR,0);
      ioctl(audio_fd, AUDIO_GETINFO, &info);
      need_close = 1;
    }
  switch (dev)
    {
    case DEFAULT_DEVICE:
    case DAC_OUT_DEVICE:
    case SPEAKERS_DEVICE:
    case LINE_OUT_DEVICE:
      switch (field)
        {
        case AMP_FIELD: info.play.gain  = (int)(val[0] * AUDIO_MAX_GAIN); break;
        case CHANNEL_FIELD: info.play.channels = (int)val[0]; break;
        case SRATE_FIELD: info.play.sample_rate = (int)val[0]; break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
      break;
    case MICROPHONE_DEVICE:
    case LINE_IN_DEVICE:
    case READ_WRITE_DEVICE:
      switch (field)
        {
        case AMP_FIELD: info.record.gain = (int)(val[0] * AUDIO_MAX_GAIN); break;
        case CHANNEL_FIELD: info.record.channels = (int)val[0]; break;
        case SRATE_FIELD: info.record.sample_rate = (int)val[0]; break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
      break;
    default: AUDIO_ERROR = CANT_WRITE; break;
    }
  ioctl(audio_fd,AUDIO_SETINFO,&info);
  if (need_close) close_audio(audio_fd);
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

/* pause can be implemented with play.pause and record.pause */

static void describe_audio_state_1(void) 
{
  struct audio_info info;
  struct audio_device ad;
  char *dev_name;
  AUDIO_INITINFO(&info);
  AUDIO_ERROR = NO_ERROR;
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  if (audio_fd != -1)
    {
      ioctl(audio_fd,AUDIO_GETINFO,&info);
      ioctl(audio_fd,AUDIO_GETDEV,&ad);
    }
  else 
    {
      if (getenv(AUDIODEV_ENV) != NULL) dev_name = getenv(AUDIODEV_ENV); else dev_name = DAC_NAME;
      audio_fd = open(dev_name,O_RDONLY,0);
      ioctl(audio_fd,AUDIO_GETINFO,&info);
      ioctl(audio_fd,AUDIO_GETDEV,&ad);
      close_audio(audio_fd);
    }
  sprintf(strbuf,"%s\n",ad.name);
  pprint(strbuf);
  sprintf(strbuf,"output: %s, srate: %d, vol: %.3f, chans: %d, format %d-bit %s\n",
          ((info.play.port == AUDIO_LINE_OUT) ? "line out" :
           (info.play.port == SPEAKERS_DEVICE) ? "speakers" : "headphones"),
          info.play.sample_rate,
          (float)info.play.gain / (float)AUDIO_MAX_GAIN,
          info.play.channels,
          info.play.precision,
          ((info.play.encoding == AUDIO_ENCODING_ALAW) ? "alaw" :
           (info.play.encoding == AUDIO_ENCODING_ULAW) ? "ulaw" : "linear"));
  pprint(strbuf);
  sprintf(strbuf,"intput: %s, srate: %d, vol: %.3f, chans: %d, format %d-bit %s\n",
          (info.record.port == AUDIO_LINE_IN) ? "line in" : "mic",
          info.record.sample_rate,
          (float)info.record.gain / (float)AUDIO_MAX_GAIN,
          info.record.channels,
          info.record.precision,
          ((info.record.encoding == AUDIO_ENCODING_ALAW) ? "alaw" :
           (info.record.encoding == AUDIO_ENCODING_ULAW) ? "ulaw" : "linear"));
  pprint(strbuf);
}

static struct audio_info saved_info;

void save_audio_state (void) 
{
  if (audio_fd != -1) ioctl(audio_fd,AUDIO_GETINFO,&saved_info);
}

void restore_audio_state (void) 
{
  if (audio_fd != -1) ioctl(audio_fd,AUDIO_SETINFO,&saved_info);
}
#endif



/* ------------------------------- MACOS ----------------------------------------- */

#ifdef MACOS
#define AUDIO_OK

#include <Resources.h>
#include <Sound.h>
#include <SoundInput.h>

char *audio_error_name(int err) {return(audio_error_name_1(err));}
int audio_systems(void) {return(1);} /* if Audiomedia, multiple? */
char *audio_system_name(int system) {return("Mac");}

static int available_input_devices(void)
{
  unsigned char *devname;
  OSErr err;
  int i;
  Handle h;
  devname = (unsigned char *)CALLOC(256,sizeof(char));
  for (i=1;i<16;i++)
    {
      err = SPBGetIndexedDevice(i,devname,&h);
      if (err != noErr) break;
    }
  FREE(devname);
  return(i-1);
}

static int input_device_is_connected(long refnum)
{
  OSErr err;
  short connected;
  err = SPBGetDeviceInfo(refnum,siDeviceConnected,&connected);
  return(connected);
}

static int input_device_get_source(long refnum)
{
  OSErr err;
  short source;
  err = SPBGetDeviceInfo(refnum,siInputSource,&source);
  return(source);
}

static int input_device_set_source(long refnum, short source)
{
  OSErr err;
  err = SPBSetDeviceInfo(refnum,siInputSource,&source);
  return((err == noErr) ? 0 : -1);
}

static int input_device_get_sources(long refnum, char **names)
{
  OSErr err;
  short sources;
  Handle h;
  err = SPBSetDeviceInfo(refnum,siInputSourceNames,&h);
  if (err == siUnknownInfoType) return(0);
  sources = (short)(*h);
  /* printf("%d sources: %s ",sources,p2cstr((unsigned char *)((*(h+2))))); */
  /* need an example to test this silly thing */
  return((err == noErr) ? sources : -1);
}

static int input_device_channels (long refnum)
{
  OSErr err;
  short chans;
  err = SPBGetDeviceInfo(refnum,siChannelAvailable,&chans);
  if (err == noErr) return(chans);
  return(-1);
}

static int input_device_get_async(long refnum)
{
  OSErr err;
  short async;
  err = SPBGetDeviceInfo(refnum,siAsync,&async);
  if (err == noErr) return(async);
  return(-1);
}

static char *input_device_name(long refnum)
{
  char *name;
  OSErr err;  
  name = (char *)CALLOC(256,sizeof(char));
  err = SPBGetDeviceInfo(refnum,siDeviceName,name);
  if (err == noErr) return(name);
  FREE(name);
  return(NULL);
}

static float input_device_get_gain(long refnum)
{
  OSErr err;
  unsigned long val;
  err = SPBGetDeviceInfo(refnum,siInputGain,&val);
  /* val is a "4 byte fixed value between .5 and 1.5"!! */
  if (err == noErr)
    return((float)val/65536.0);
  return(-1);
 }

static int input_device_set_gain(long refnum, float gain)
{
  OSErr err;
  int val;
  val = ((int)(gain*65536));
  err = SPBSetDeviceInfo(refnum,siInputGain,&val);
  return((err == noErr) ? 0 : -1); 
}

static int input_device_get_channels(long refnum)
{
  OSErr err;
  short chans;
  err = SPBGetDeviceInfo(refnum,siNumberChannels,&chans);
  return((err == noErr) ? chans : -1); 
}

static int input_device_set_channels(long refnum, short chans)
{
  OSErr err;
  err = SPBSetDeviceInfo(refnum,siNumberChannels,&chans);
  return((err == noErr) ? 0 : -1); 
}

static int input_device_get_quality(long refnum)
{
  OSErr err;
  OSType val;
  err = SPBGetDeviceInfo(refnum,siRecordingQuality,&val);
  if (err == noErr)
    {
      if (val == siCDQuality) return(3);
      if (val == siBestQuality) return(2);
      if (val == siBetterQuality) return(1);
      if (val == siGoodQuality) return(0);
    }
  return(-1);
}

static int input_device_set_quality(long refnum, int quality)
{
  OSErr err;
  OSType val;
  if (quality == 3) val = siCDQuality;
  else if (quality == 2) val = siBestQuality;
  else if (quality == 1) val = siBetterQuality;
  else val = siGoodQuality;
  err = SPBSetDeviceInfo(refnum,siRecordingQuality,&val);
  return((err == noErr) ? 0 : -1); 
}

static int input_device_get_srate(long refnum)
{
  OSErr err;
  unsigned long fixed_srate;
  err = SPBGetDeviceInfo(refnum,siSampleRate,&fixed_srate);
  if (err == noErr) return(fixed_srate>>16);
  return(-1);
}

static int input_device_set_srate(long refnum, int srate)
{
  OSErr err;
  unsigned long fixed_srate;
  fixed_srate = (unsigned long)(srate * 65536);
  err = SPBSetDeviceInfo(refnum,siSampleRate,&fixed_srate);
  return((err == noErr) ? 0 : -1); 
}

static int input_device_get_sample_size(long refnum)
{
  OSErr err;
  short size;
  err = SPBGetDeviceInfo(refnum,siSampleSize,&size);
  if (err == noErr) return(size);
  return(-1);
}

static int input_device_set_sample_size(long refnum, short size)
{
  OSErr err;
  err = SPBSetDeviceInfo(refnum,siSampleSize,&size);
  return((err == noErr) ? 0 : -1); 
}

static int input_device_get_signed(long refnum)
{ /* 0 = unsigned */
  OSErr err;
  short sign;
  err = SPBGetDeviceInfo(refnum,siSampleRate,&sign);
  if (err == noErr) return(sign);
  return(-1);
}

static int input_device_set_signed(long refnum, short sign)
{
  OSErr err;
  err = SPBSetDeviceInfo(refnum,siSampleRate,&sign);
  return((err == noErr) ? 0 : -1); 
}

static int input_device_sample_rates(long refnum, int *range, int *rates)
{
  unsigned short num;
  int i,j;
  unsigned long ptr;
  OSErr err;
  unsigned char pp[6];                                      /* can't depend on C compiler to pack a struct correctly here */
  num = 0;
  err = SPBGetDeviceInfo(refnum,siSampleRateAvailable,pp);
  if (err == noErr)
    {
      num = pp[1] + (pp[0]<<8);                             /* unsigned short is first element */
      if (num == 0) {(*range) = 1; num = 2;} else (*range) = 0;
      ptr = pp[5] + (pp[4]<<8) + (pp[3]<<16) + (pp[2]<<24); /* pointer to "fixed" table is second element */
      for (i=0,j=0;i<num;i++,j+=4)                          
        rates[i] = (*(unsigned short *)(j+(*(int *)ptr)));  /* ignore fraction -- this is dubious code */
    }
  return(num);
}

static int input_device_sample_sizes(long refnum, int *sizes)
{
  unsigned short num;
  int i,j;
  unsigned long ptr;
  OSErr err;
  unsigned char pp[6];
  num = 0;
  err = SPBGetDeviceInfo(refnum,siSampleSizeAvailable,pp);
  if (err == noErr)
    {
      num = pp[1] + (pp[0]<<8); 
      ptr = pp[5] + (pp[4]<<8) + (pp[3]<<16) + (pp[2]<<24);
      for (i=0,j=0;i<num;i++,j+=2) sizes[i] = (*(unsigned short *)(j+(*(int *)ptr)));
    }
  return(num);
}

static int input_device_get_gains(long refnum, float *gains)
{
  OSErr err;
  long ptr[2];
  err = SPBGetDeviceInfo(refnum,siStereoInputGain,ptr);
  if (err == noErr)
    {
      gains[0] = (float)ptr[0]/65536.0;
      gains[1] = (float)ptr[1]/65536.0;
    }
  else return(-1);
  return(0);
}

static int input_device_set_gains(long refnum, float *gains)
{
  OSErr err;
  long val[2];
  val[0] = gains[0]*65536;
  val[1] = gains[1]*65536;
  err = SPBSetDeviceInfo(refnum,siStereoInputGain,val);
  return((err == noErr) ? 0 : -1); 
}

static char *quality_names[5] = {"indescribable","bad","not bad","ok","good"};

static void describe_audio_state_1(void) 
{
  long response;
  NumVersion nv;
  OSErr err;
  int vals[64];
  float gains[2];
  int have_IO_mgr = 0, have_input_device = 0;
  unsigned char *devname = NULL;
  int i,j,devs,rates,range,sizes,connected;
  long refnum;
  Handle h;
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  nv = SndSoundManagerVersion();
  sprintf(strbuf,"Sound Manager: %d.%d.%d.%d\n",nv.majorRev,nv.minorAndBugRev,nv.stage,nv.nonRelRev); pprint(strbuf);
  err = Gestalt(gestaltSoundAttr,&response);
  have_IO_mgr = (response & gestaltSoundIOMgrPresent);
  have_input_device = (response & gestaltHasSoundInputDevice);
  if (have_IO_mgr)
    {
      nv = SPBVersion();
      sprintf(strbuf,"Sound Input Manager: %d.%d.%d.%d\n",nv.majorRev,nv.minorAndBugRev,nv.stage,nv.nonRelRev); pprint(strbuf);
     }
  if (!have_IO_mgr) pprint("Sound IO Manager absent!\n");
  if (!(response & gestaltBuiltInSoundInput)) pprint("no built-in input device!\n");
  if (!have_input_device) pprint("no input devices available!\n");
  if (!(response & gestaltSndPlayDoubleBuffer)) pprint("double buffering not supported!\n");
  if (response & gestalt16BitAudioSupport) pprint("has 16-bit audio ");
  if (response & gestalt16BitSoundIO) pprint("has 16-bit sound ");
  if (response & gestaltStereoInput) pprint("has stereo input\n");
  if (response & gestaltPlayAndRecord) pprint("can play and record simultaneously\n");
  if (response & gestaltMultiChannels) pprint("has multichannel support\n");
  GetSysBeepVolume(&response);
  sprintf(strbuf,"beep vol: %.3f %.3f\n",((float)(response>>16))/255.0,((float)(response&0xffff))/255.0); pprint(strbuf);
  GetDefaultOutputVolume(&response);
  sprintf(strbuf,"output vol: %.3f %.3f\n",((float)(response>>16))/255.0,((float)(response&0xffff))/255.0); pprint(strbuf);
  if ((have_IO_mgr) && (have_input_device))
    { 
      devs = available_input_devices();
      if (devs>0)
        {
          devname = (unsigned char *)CALLOC(256,sizeof(char));
          sprintf(strbuf,"input device%s:\n",(devs>1) ? "s" : ""); pprint(strbuf);
          for (i=1;i<=devs;i++)
            {
              for (i=1;i<=devs;i++)
                {
                  err = SPBGetIndexedDevice(i,devname,&h);
                  if (err == noErr)
                    {
                      err = SPBOpenDevice(devname,siWritePermission,&refnum);
                      if (err == noErr)
                        {
                          range = input_device_get_source(refnum);
                          connected = input_device_is_connected(refnum);
                          sprintf(strbuf,"  %s: %s%s",
                                  (*devname) ? devname : (unsigned char *)"un-named",
                                  ((input_device_get_async(refnum) == 1) ? "(async) " : ""),
                                  ((connected == siDeviceIsConnected) ? "" : 
                                   ((connected == siDeviceNotConnected) ? 
                                    "(not connected )" : "(might not be connected)")));
                        pprint(strbuf);
                        if (range == 0) pprint("\n");
                        else
                                {
                                sprintf(strbuf," (source: %d)\n",range);
                                pprint(strbuf);
                                }
                                sprintf(strbuf,"    %d chans available, %d active\n",
                                  input_device_channels(refnum),
                                  input_device_get_channels(refnum));
                          pprint(strbuf);

                          /* input_device_get_sources(refnum,NULL); */

                          range = 0;
                          rates = input_device_sample_rates(refnum,&range,vals);
                          if (rates>1)
                            {
                              sprintf(strbuf,"    srates available:"); 
                              pprint(strbuf);
                              if (range)
                                {sprintf(strbuf,"%d to %d",vals[0],vals[1]); pprint(strbuf);}
                              else
                                {for (j=0;j<rates;j++) {sprintf(strbuf," %d",vals[j]); pprint(strbuf);}}
                              sprintf(strbuf,", current srate: %d\n",
                                      input_device_get_srate(refnum));
                              pprint(strbuf);
                            }
                          else 
                            {
                              sprintf(strbuf,"    srate: %d\n",input_device_get_srate(refnum)); 
                              pprint(strbuf);
                            }
                          err = input_device_get_quality(refnum);
                          if (err != -1) 
                            {
                              sprintf(strbuf,"    quality: %s\n",quality_names[1+input_device_get_quality(refnum)]);
                              pprint(strbuf);
                            }
                          input_device_get_gains(refnum,gains);
                          sprintf(strbuf,"    gain: %.3f (%.3f %.3f)\n    sample: %s %d bits",
                                  input_device_get_gain(refnum),gains[0],gains[1],
                                  ((input_device_get_signed(refnum)) ? "signed" : "unsigned"),
                                  input_device_get_sample_size(refnum));
                          pprint(strbuf);
                          sizes = input_device_sample_sizes(refnum,vals);
                          if (sizes > 0)
                            {
                              sprintf(strbuf," (%d",vals[0]); pprint(strbuf);
                              for (j=1;j<sizes;j++) 
                                {sprintf(strbuf,", %d",vals[j]); pprint(strbuf);}
                              pprint(" bit samples available)");
                            }
                          pprint("\n");
                          SPBCloseDevice(refnum);
                        }
                    }
                }
            }
          FREE(devname);
        }
    }
}

#define BUFFER_FILLED 1
#define BUFFER_EMPTY 2

#define SOUND_UNREADY 0
#define SOUND_INITIALIZED 1
#define SOUND_RUNNING 2

#define INPUT_LINE 1
#define OUTPUT_LINE 2

static int buffer_size = 1024;
static SndDoubleBufferPtr *db = NULL;
static SndDoubleBufferHeader dh;
static SndChannelPtr chan;
static int *db_state = NULL;
static int sound_state = 0;
static int current_chans = 1;
static int current_datum_size = 2;
static int current_srate = 22050;
static int current_buf = 0;
static long in_ref = -1;
static SPB spb;

#define DATA_EMPTY 0
#define DATA_READY 1
#define DATA_WRITTEN 2
static int data_status = DATA_EMPTY;
static int data_bytes = 0;
static char *data = NULL;

#ifdef MCL_PPC
  void reset_db(void) 
    {
      db=NULL; 
      db_state=NULL;
      data = NULL;
      data_bytes = 0;
    }
#endif

static pascal void nextbuffer(SndChannelPtr cp, SndDoubleBufferPtr db)
{
  db_state[current_buf] = BUFFER_EMPTY;
}

int open_audio_output(int ur_dev, int srate, int chans, int format, int size) 
{
  OSErr err;
  int dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  if (!db) db = (SndDoubleBufferPtr *)CALLOC(2,sizeof(SndDoubleBufferPtr));
  if (!db_state) db_state = (int *)CALLOC(2,sizeof(int));
  chan = nil;
  err = SndNewChannel(&chan,sampledSynth,0,nil);
  if (err != noErr) {AUDIO_ERROR = DEVICE_NOT_AVAILABLE; return(-1);}
  dh.dbhNumChannels = chans;
  current_chans = chans;
  if (format == snd_8_unsigned) 
    {
      dh.dbhSampleSize = 8; 
      current_datum_size = 1;
    }
  else 
    {
      dh.dbhSampleSize = 16;
      current_datum_size = 2;
    }
  dh.dbhCompressionID = 0; 
  dh.dbhPacketSize = 0; 
  dh.dbhSampleRate = (srate << 16);
  dh.dbhDoubleBack = NewSndDoubleBackProc(nextbuffer);
  if (size <= 0) buffer_size = 1024; else buffer_size = size;
  db[0] = (SndDoubleBufferPtr)CALLOC(sizeof(SndDoubleBuffer) + buffer_size,sizeof(char));
  if ((db[0] == nil) || (MemError() != 0)) {AUDIO_ERROR = SIZE_NOT_AVAILABLE; SndDisposeChannel(chan,0); return(-1);}
  dh.dbhBufferPtr[0] = db[0];   
  db[0]->dbNumFrames = 0;
  db[0]->dbFlags = 0;
  db_state[0] = BUFFER_EMPTY;
  db[1] = (SndDoubleBufferPtr)CALLOC(sizeof(SndDoubleBuffer) + buffer_size,sizeof(char));
  if ((db[1] == nil) || (MemError() != 0)) {AUDIO_ERROR = SIZE_NOT_AVAILABLE; FREE(db[0]); SndDisposeChannel(chan,0); return(-1);}
  dh.dbhBufferPtr[1] = db[1];   
  db[1]->dbNumFrames = 0;
  db[1]->dbFlags = 0;
  db_state[1] = BUFFER_EMPTY;
  sound_state = SOUND_INITIALIZED;
  current_buf = 0;
  return(OUTPUT_LINE);
}

static OSErr fill_buffer(int dbi, char *inbuf, int instart, int bytes)
{
  int i,j;
  OSErr err;
  err = noErr;
  for (i=instart,j=0;j<bytes;j++,i++) db[dbi]->dbSoundData[j] = inbuf[i];
  db_state[dbi] = BUFFER_FILLED;
  db[dbi]->dbFlags = (db[dbi]->dbFlags | dbBufferReady);
  db[dbi]->dbNumFrames = (bytes / (current_chans * current_datum_size));
  if ((sound_state == SOUND_INITIALIZED) && (dbi == 1))
    {
      sound_state = SOUND_RUNNING;
      err = SndPlayDoubleBuffer(chan,&dh);
    }
  return(err);
}

static OSErr wait_for_empty_buffer(int buf)
{
  SCStatus Stats;
  OSErr err;
  err = noErr;
  while (db_state[buf] != BUFFER_EMPTY)
    {
      err = SndChannelStatus(chan,sizeof(Stats),&Stats);
      if ((err != noErr) || (!(Stats.scChannelBusy))) break;
    }
  return(err);
}

int write_audio(int line, char *buf, int bytes) 
{
  OSErr err;
  int lim,leftover,start;
  if (line != OUTPUT_LINE) {AUDIO_ERROR = CANT_WRITE; return(-1);}
  AUDIO_ERROR = NO_ERROR;
  leftover = bytes;
  start = 0;
  while (leftover > 0)
    {
      lim = leftover;
      if (lim > buffer_size) lim = buffer_size;
      leftover -= lim;
      err = wait_for_empty_buffer(current_buf);
      if (err != noErr) {AUDIO_ERROR = CANT_WRITE; return(-1);}
      err = fill_buffer(current_buf,buf,start,lim);
      if (err != noErr) {AUDIO_ERROR = CANT_WRITE; return(-1);}
      start += lim;
      current_buf++;
      if (current_buf>1) current_buf=0;
    }
  return(0);
}

int close_audio(int line) 
{
  OSErr err;
  int i;
  AUDIO_ERROR = NO_ERROR;
  if (line == OUTPUT_LINE)
    {
      /* fill with a few zeros, wait for empty flag */
      if (sound_state != SOUND_UNREADY)
        {
          wait_for_empty_buffer(current_buf);
          for (i=0;i<128;i++) db[current_buf]->dbSoundData[i] = 0;
          db[current_buf]->dbFlags = (db[current_buf]->dbFlags | dbBufferReady | dbLastBuffer);
          db[current_buf]->dbNumFrames = (128 / (current_chans * current_datum_size));
          wait_for_empty_buffer(current_buf);
          FREE(db[0]); db[0]=NULL;
          FREE(db[1]); db[1]=NULL;
          db_state[0] = BUFFER_EMPTY;
          db_state[1] = BUFFER_EMPTY;
          sound_state = SOUND_UNREADY;
          err = SndDisposeChannel(chan,0);
          /* this is the line that forced me to use FREE/CALLOC throughout! */
          if (err != noErr) {AUDIO_ERROR = CANT_CLOSE; return(-1);}
        }
    }
  else
    {
      if (line == INPUT_LINE)
        {
          if (in_ref != -1)
            {
              data_status = DATA_EMPTY;
              SPBStopRecording(in_ref);
              if (spb.bufferPtr) FREE(spb.bufferPtr);
              SPBCloseDevice(in_ref);
              in_ref = -1;
            }
        }
      else {AUDIO_ERROR = CANT_CLOSE; return(-1);}
    }
  return(0);
}

static void read_callback(SPB *spb)
{
  int i,lim;
  if (data_status != DATA_EMPTY)
    {
      if (data_bytes > spb->bufferLength) lim=spb->bufferLength; else lim=data_bytes;
      for (i=0;i<lim;i++) data[i] = spb->bufferPtr[i]; 
      spb->bufferLength = data_bytes;
      SPBRecord(spb,TRUE);
      data_status = DATA_WRITTEN;
    }
}

int open_audio_input(int ur_dev, int srate, int chans, int format, int size) 
{
  OSErr err;
  short source;
  int dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  data_status = DATA_EMPTY;
  if (size<=0) size = 1024;
  if (in_ref != -1) {AUDIO_ERROR = DEVICE_NOT_AVAILABLE; return(-1);}
  err = SPBOpenDevice((unsigned char *)"",siWritePermission,&in_ref);
  if (err != noErr) {AUDIO_ERROR = CANT_OPEN; return(-1);}
  spb.inRefNum = in_ref;
  spb.count = size;
  source = 3; /* the microphone ?? (2: CD, 4: modem, 0: none) -- nowhere is this documented! */
  input_device_set_source(in_ref,source);
  input_device_set_srate(in_ref,srate);
  input_device_set_channels(in_ref,(short)chans);
  input_device_set_sample_size(in_ref,(format == snd_16_linear) ? 2 : 1);
  input_device_set_signed(in_ref,(format == snd_16_linear) ? 1 : 0);
  spb.milliseconds = (int)((float)(size * 1000) / (float)(((format == snd_16_linear) ? 2 : 1) * srate));
  spb.bufferLength = size;
  spb.bufferPtr = (char *)CALLOC(size,sizeof(char));
  spb.completionRoutine = NewSICompletionProc(read_callback);
  err = SPBRecord(&spb,TRUE);
  return(INPUT_LINE);
}

int read_audio(int line, char *buf, int bytes) 
{
  OSErr err;
  unsigned long total_samps,num_samps,total_msecs,num_msecs;
  short level,status;
  if (line != INPUT_LINE) {AUDIO_ERROR = CANT_READ; return(-1);}
  AUDIO_ERROR = NO_ERROR;
  data_status = DATA_READY;
  data_bytes = bytes;
  data = buf;
  while (data_status == DATA_READY)
    {
      err = SPBGetRecordingStatus(in_ref,&status,&level,&total_samps,&num_samps,&total_msecs,&num_msecs);
      if ((err != noErr) || (status <= 0)) break; /* not necessarily an error */
    }
  return(0);
}

int read_audio_state(int ur_dev, int field, int chan, float *val) 
{
  OSErr err;
  long response;
  int dev;
  float in_val[2];
  int our_open = 0;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  switch (field)
    {
    case CHANNEL_FIELD:
      val[0] = 2;
      break;
    case DEVICE_FIELD:
      val[0] = 2; 
      if (chan>1) val[1] = MICROPHONE_DEVICE; 
      if (chan>2) val[2] = DAC_OUT_DEVICE;
      break;
    case FORMAT_FIELD:
      val[0] = 2; 
      if (chan>1) val[1] = snd_16_linear;
      if (chan>2) val[2] = snd_8_unsigned;
      break;
    default:
      switch (dev)
        {
        case DEFAULT_DEVICE:
        case DAC_OUT_DEVICE:
        case SPEAKERS_DEVICE:
        case LINE_OUT_DEVICE:
          switch (field)
            {
            case AMP_FIELD: 
              GetDefaultOutputVolume(&response);
              if (chan == 0)
                val[0] = ((float)(response>>16))/255.0;
              else val[0] = ((float)(response&0xffff))/255.0;
              break;
            case CHANNEL_FIELD: val[0] = 2; break;
            case SRATE_FIELD: val[0] = current_srate; break;
            }
          break;
        case MICROPHONE_DEVICE:
        case LINE_IN_DEVICE:
          if (in_ref == -1)
            {
              err = SPBOpenDevice((const unsigned char *)"",siWritePermission,&in_ref);
              if (err != noErr) {AUDIO_ERROR = CANT_OPEN; return(-1);}
              our_open = 1;
            }
          switch (field)
            {
            case AMP_FIELD:
              err = input_device_get_gains(in_ref,in_val);
              if (chan == 0) val[0] = in_val[0]; else val[0] = in_val[1];
              break;
            case CHANNEL_FIELD:
              err = input_device_get_channels(in_ref);
              break;
            case SRATE_FIELD:
              err = input_device_get_srate(in_ref);
              break;
            default: AUDIO_ERROR = CANT_READ; break;
            }
          if (err == -1) 
            AUDIO_ERROR = CANT_READ;
          else val[0] = err;
          if (our_open)
            {
              SPBCloseDevice(in_ref);
              in_ref = -1;
            }
          break;
        default: AUDIO_ERROR = CANT_READ; break;
        }
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val) 
{
  OSErr err;
  float out_val[2];
  long curval,newval;
  int amp,our_open,dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  switch (dev)
    {
    case DEFAULT_DEVICE:
    case DAC_OUT_DEVICE:
    case SPEAKERS_DEVICE:
    case LINE_OUT_DEVICE:
      switch (field)
        {
        case AMP_FIELD: 
          amp = (int)(255 * val[0]);
          GetDefaultOutputVolume(&curval);
          if (chan == 0)
            newval = ((curval & 0xffff0000) | (amp & 0xffff));
          else newval = (((amp<<16) & 0xffff0000) | (curval & 0xffff));
          SetDefaultOutputVolume(newval);
          break;
        case CHANNEL_FIELD: 
        case SRATE_FIELD: break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
      break;
    case MICROPHONE_DEVICE:
    case LINE_IN_DEVICE:
      if (in_ref == -1)
        {
          err = SPBOpenDevice((const unsigned char *)"",siWritePermission,&in_ref);
          if (err != noErr) {AUDIO_ERROR = CANT_OPEN; return(-1);}
          our_open = 1;
        }
      switch (field)
        {
        case AMP_FIELD:
          input_device_get_gains(in_ref,out_val);
          if (chan == 0) out_val[0] = val[0]; else out_val[1] = val[0];
          err = input_device_set_gains(in_ref,out_val);
          break;
        case CHANNEL_FIELD:
          err = input_device_set_channels(in_ref,(int)val[0]);
          break;
        case SRATE_FIELD:
          err = input_device_set_srate(in_ref,(int)val[0]);
          break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
      if (err == -1) 
        AUDIO_ERROR = CANT_READ;
      if (our_open)
        {
          SPBCloseDevice(in_ref);
          in_ref = -1;
        }
      break;
    default: AUDIO_ERROR = CANT_WRITE; break;
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

static long beep_vol = 0,out_vol = 0;

void save_audio_state(void) 
{
  GetDefaultOutputVolume(&out_vol);
  GetSysBeepVolume(&beep_vol);
}

void restore_audio_state(void) 
{
  SetDefaultOutputVolume(out_vol);
  SetSysBeepVolume(beep_vol);
}

int initialize_audio(void) 
{
  AUDIO_ERROR = NO_ERROR;
  return(0);
}

#ifdef MCL_PPC
  void reset_strbuf(void) {strbuf=NULL;}
#endif

#endif


/* ------------------------------- BEOS ----------------------------------------- */

#ifdef BEOS
#define AUDIO_OK

#include <Be.h>

#define OUTPUT_LINE 1
#define INPUT_LINE 2

char *audio_error_name(int err) {return(audio_error_name_1(err));}
int audio_systems(void) {return(1);}
char *audio_system_name(int system) {return("Be");}

static void describe_audio_state_1(void) 
{
  long chans;
  float g[1];
  float gleft,gright;
  bool enabled;
  BDACStream B_Stream;   
  BADCStream A_Stream;   
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  read_audio_state(DAC_OUT_DEVICE,AMP_FIELD,0,g); gleft=g[0];
  read_audio_state(DAC_OUT_DEVICE,AMP_FIELD,1,g); gright=g[0];
  sprintf(strbuf,"dac vol: %.3f %.3f",gleft,gright); pprint(strbuf);
  if ((gleft == 0.0) && (gright == 0.0) && (!(B_Stream.IsDeviceEnabled(B_DAC_OUT)))) pprint(" (muted)\n"); else pprint("\n");
  read_audio_state(LINE_OUT_DEVICE,AMP_FIELD,0,g); gleft=g[0];
  read_audio_state(LINE_OUT_DEVICE,AMP_FIELD,1,g); gright=g[0];
  sprintf(strbuf,"line out vol: %.3f %.3f",gleft,gright); pprint(strbuf);
  if ((gleft == 0.0) && (gright == 0.0) && (!(B_Stream.IsDeviceEnabled(B_MASTER_OUT)))) pprint(" (muted)\n"); else pprint("\n");
  read_audio_state(SPEAKERS_DEVICE,AMP_FIELD,0,g); gleft=g[0];
  read_audio_state(SPEAKERS_DEVICE,AMP_FIELD,1,g); gright=g[0];
  sprintf(strbuf,"speaker vol: %.3f %.3f",gleft,gright); pprint(strbuf);
  if ((gleft == 0.0) && (gright == 0.0) && (!(B_Stream.IsDeviceEnabled(B_SPEAKER_OUT)))) pprint(" (muted)\n"); else pprint("\n");
  if (A_Stream.IsMicBoosted()) pprint("mic boosted\n");
  read_audio_state(DAC_OUT_DEVICE,SRATE_FIELD,0,g);
  sprintf(strbuf,"srate: %d\n",(int)g[0]); pprint(strbuf);
  B_Stream.GetVolume(B_CD_THROUGH,&gleft,&gright,&enabled);
  if (!enabled)
    pprint("CD muted\n");
  else
    {
      sprintf(strbuf,"CD through vol: %.3f %.3f\n",gleft,gright); 
      pprint(strbuf);
    }
  B_Stream.GetVolume(B_LOOPBACK,&gleft,&gright,&enabled);
  if (!enabled)
    pprint("loopback muted\n");
  else
    {
      sprintf(strbuf,"loopback vol: %.3f %.3f\n",gleft,gright); 
      pprint(strbuf);
    }
  B_Stream.GetVolume(B_LINE_IN_THROUGH,&gleft,&gright,&enabled);
  if (!enabled)
    pprint("line in through muted\n");
  else
    {
      sprintf(strbuf,"line in through vol: %.3f %.3f\n",gleft,gright); 
      pprint(strbuf);
    }
  A_Stream.ADCInput(&chans);
  if (chans == B_CD_IN) pprint("CD in\n"); 
  else if (chans == B_LINE_IN) pprint("line in\n");
  else pprint("mic in\n");
  /* SetADCInput apparently to B_CD|LINE|MIC_IN */
}
  
static BSubscriber B_dac_sub; /* BAudioSubscriber? */
static BDACStream B_dac_stream;
static BSubscriber B_adc_sub;
static BADCStream B_adc_stream;

#define DATA_EMPTY 0
#define DATA_READY 1
#define DATA_WRITTEN 2
static int data_status;
static int data_bytes;
static char *data = NULL;

#define SOUND_UNREADY 0
#define SOUND_INITIALIZED 1
#define SOUND_RUNNING 2
static int sound_state = SOUND_UNREADY;

static bool read_callback(void *userData, char *buf, unsigned long count, void *hdr)
{
  int i,lim;
  short *sys,*loc;
  if (data_status != DATA_EMPTY)
    {
      if (data_bytes > count) lim=count; else lim=data_bytes;
      for (i=0;i<lim;i++) data[i] = buf[i]; 
      data_status = DATA_WRITTEN;
    }
  return(TRUE);
  /* return TRUE to continue writing, FALSE to exit stream */
}

static bool write_callback(void *userData, char *buf, unsigned long count, void *hdr)
{
  int i,lim;
  short *sys,*loc;
  if (data_status != DATA_EMPTY)
    {
      if (data_bytes > count) lim=count; else lim=data_bytes;
      lim = lim/2;
      sys = (short *)buf;
      loc = (short *)data;
      for (i=0;i<lim;i++) sys[i]+=loc[i];
      data_status = DATA_WRITTEN;
    }
  return(TRUE);
  /* return TRUE to continue writing, FALSE to exit stream */
}

int open_audio_output(int ur_dev, int srate, int chans, int format, int size) 
{
  status_t err;
  int dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  if (chans != 2) {AUDIO_ERROR = CHANNELS_NOT_AVAILABLE; return(-1);}
  if (format != snd_16_linear) {AUDIO_ERROR = FORMAT_NOT_AVAILABLE; return(-1);}
  if ((srate != 44100) && (srate != 22050)) {AUDIO_ERROR = SRATE_NOT_AVAILABLE; return(-1);}
  if ((B_dac_sub.Subscribe(&B_dac_stream)) == B_NO_ERROR)
    {
      err = B_dac_stream.SetSamplingRate(srate);
      if (err != B_NO_ERROR) fprintf(stdout,"can't reset srate ");
      err = B_dac_stream.SetStreamBuffers(size,8); /* size, count */
      if (err != B_NO_ERROR) fprintf(stdout,"can't reset size ");
      data_status = DATA_EMPTY;
      sound_state = SOUND_INITIALIZED;
    }
  else {AUDIO_ERROR = CANT_OPEN; return(-1);}
  return(OUTPUT_LINE);
}

int open_audio_input(int ur_dev, int srate, int chans, int format, int size) 
{
  status_t err;
  int dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  if ((chans != 2) || (format != snd_16_linear)) {AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE; return(-1);}
  if ((srate != 44100) && (srate != 22050)) {AUDIO_ERROR = SRATE_NOT_AVAILABLE; return(-1);}
  if ((B_adc_sub.Subscribe(&B_adc_stream)) == B_NO_ERROR)
    {
      err = B_adc_stream.SetSamplingRate(srate);
      if (err != B_NO_ERROR) fprintf(stdout,"can't reset srate ");
      err = B_adc_stream.SetStreamBuffers(size,8); /* size, count */
      if (err != B_NO_ERROR) fprintf(stdout,"can't reset size ");
      err = B_adc_sub.EnterStream(NULL,TRUE,NULL,read_callback,NULL,TRUE);
      if (err != B_NO_ERROR) fprintf(stdout,"can't enter stream ");
      data_status = DATA_EMPTY;
      /* no "neighbor", place at front, no user data, call read_callback, no exit function, execute in separate thread */
    }
  else {AUDIO_ERROR = CANT_OPEN; return(-1);}
  return(INPUT_LINE);
}

int write_audio(int line, char *buf, int bytes) 
{
  size_t size;
  status_t err;
  long count,subs;
  bool runs;
  if (line != OUTPUT_LINE) {AUDIO_ERROR = CANT_WRITE; return(-1);}
  AUDIO_ERROR = NO_ERROR;
  if (sound_state == SOUND_UNREADY) {AUDIO_ERROR = CANT_WRITE; return(-1);}
  if (sound_state == SOUND_INITIALIZED)
    {
      err = B_dac_sub.EnterStream(NULL,TRUE,NULL,write_callback,NULL,TRUE);
      if (err != B_NO_ERROR) fprintf(stdout,"can't enter stream ");
      /* no "neighbor", place at front, no user data, call write_callback, no exit function, execute in separate thread */
      sound_state = SOUND_RUNNING;
    }
  data_status = DATA_READY;
  data_bytes = bytes;
  data = buf;
  while (data_status == DATA_READY)
    {
      B_dac_stream.GetStreamParameters(&size,&count,&runs,&subs);
      if (!runs) {AUDIO_ERROR = CANT_WRITE; B_dac_sub.ExitStream(); sound_state = SOUND_UNREADY; return(-1);}
    }
  return(0);
}

int close_audio(int line) 
{
  AUDIO_ERROR = NO_ERROR;
  if (line == OUTPUT_LINE)
    {
      B_dac_sub.Unsubscribe();
      sound_state = SOUND_UNREADY;
    }
  else 
    {
      if (line == INPUT_LINE)
        B_adc_sub.Unsubscribe();
      else {AUDIO_ERROR = CANT_CLOSE; return(-1);}
    }
  return(0);
}

int read_audio(int line, char *buf, int bytes) 
{
  size_t size;
  long count,subs;
  bool runs;
  if (line != INPUT_LINE) {AUDIO_ERROR = CANT_READ; return(-1);}
  AUDIO_ERROR = NO_ERROR;
  data_status = DATA_READY;
  data_bytes = bytes;
  data = buf;
  while (data_status == DATA_READY)
    {
      B_adc_stream.GetStreamParameters(&size,&count,&runs,&subs);
      if (!runs) {AUDIO_ERROR = CANT_READ; B_adc_sub.ExitStream(); return(-1);}
    }
  return(0);
}

int read_audio_state(int ur_dev, int field, int chan, float *val) 
{
  /* also B_CD_THROUGH */
  float gleft,gright,g0,g1;
  bool enabled;
  int dev;
  status_t err;
  BDACStream B_stream;   
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  switch (field)
    {
    case AMP_FIELD:
      {
        switch (dev)
          {
          case DEFAULT_DEVICE:
          case DAC_OUT_DEVICE:
            err = B_stream.GetVolume(B_DAC_OUT,&gleft,&gright,&enabled);
            if (!enabled) 
              val[0] = 0.0;
            else
              {
                if (chan == 0) 
                  val[0] = gleft; 
                else val[0] = gright;
              }
            break;
          case SPEAKERS_DEVICE:
            err = B_stream.GetVolume(B_DAC_OUT,&g0,&g1,&enabled);
            if (!enabled)
              val[0] = 0.0;
            else
              {
                err = B_stream.GetVolume(B_MASTER_OUT,&gleft,&gright,&enabled);
                if (!enabled)
                  val[0] = 0.0;
                else
                  {
                    g0 *= gleft; g1 *= gright;
                    err = B_stream.GetVolume(B_SPEAKER_OUT,&gleft,&gright,&enabled);
                    if (!enabled)
                      val[0] = 0.0;
                    else
                      {
                        if (chan == 0) 
                          val[0] = g0 * gleft; 
                        else val[0] = g1 * gright;
                      }
                  }
              }
            break;
          case LINE_OUT_DEVICE:
            err = B_stream.GetVolume(B_DAC_OUT,&g0,&g1,&enabled);
            if (!enabled)
              val[0] = 0.0;
            else
              {
                err = B_stream.GetVolume(B_MASTER_OUT,&gleft,&gright,&enabled);
                if (!enabled)
                  val[0] = 0.0;
                else
                  {
                    if (chan == 0) 
                      val[0] = g0 * gleft; 
                    else val[0] = g1 * gright;
                  }
              }
            break;
          default: AUDIO_ERROR = CANT_READ; break;
          }
        break;
      }
    case SRATE_FIELD:
      err = B_stream.SamplingRate(&g0);
      val[0] = g0;
      break;
    case CHANNEL_FIELD:
      val[0] = 2;
      break;
    case DEVICE_FIELD:
      val[0] = 3; 
      if (chan>1) val[1] = MICROPHONE_DEVICE; 
      if (chan>2) val[2] = DAC_OUT_DEVICE;
      if (chan>3) val[3] = LINE_IN_DEVICE;
      break;
    case FORMAT_FIELD:
      val[0] = 1; 
      if (chan>1) val[1] = snd_16_linear;
      break;
    default: AUDIO_ERROR = CANT_READ; break;
    }
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val) 
{
  long srate;
  int dev;
  status_t err;
  BDACStream B_stream;   
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  switch (field)
    {
    case AMP_FIELD:
      {
        switch (dev)
          {
          case DEFAULT_DEVICE:
          case DAC_OUT_DEVICE:
            if ((val[0] > 0.0) && (!(B_stream.IsDeviceEnabled(B_DAC_OUT))))
              B_stream.EnableDevice(B_DAC_OUT,TRUE);
            if (chan == 0)
              err = B_stream.SetVolume(B_DAC_OUT,val[0],B_NO_CHANGE);
            else err = B_stream.SetVolume(B_DAC_OUT,B_NO_CHANGE,val[0]);
            break;
          case SPEAKERS_DEVICE:
            if ((val[0] > 0.0) && (!(B_stream.IsDeviceEnabled(B_SPEAKER_OUT))))
              B_stream.EnableDevice(B_SPEAKER_OUT,TRUE);
            if (chan == 0)
              err = B_stream.SetVolume(B_SPEAKER_OUT,val[0],B_NO_CHANGE);
            else err = B_stream.SetVolume(B_SPEAKER_OUT,B_NO_CHANGE,val[0]);
            break;
          case LINE_OUT_DEVICE:
            if ((val[0] > 0.0) && (!(B_stream.IsDeviceEnabled(B_MASTER_OUT))))
              B_stream.EnableDevice(B_MASTER_OUT,TRUE);
            if (chan == 0)
              err = B_stream.SetVolume(B_MASTER_OUT,val[0],B_NO_CHANGE);
            else err = B_stream.SetVolume(B_MASTER_OUT,B_NO_CHANGE,val[0]);
            break;
          default: AUDIO_ERROR = CANT_WRITE; break;
          }
        break;
      }
    case SRATE_FIELD:
      srate = val[0];
      err = B_stream.SetSamplingRate(srate);
      break;
    default: AUDIO_ERROR = CANT_WRITE; break;
    }
  return(0);
}

#define BE_DEVICES 6
/* B_ADC_IN exists but seems to return garbage */

static float be_gains[BE_DEVICES * 2];
static int be_mutes[BE_DEVICES];
static int be_srate;
static int be_devices[BE_DEVICES] = {B_CD_THROUGH,B_LINE_IN_THROUGH,B_LOOPBACK,B_DAC_OUT,B_MASTER_OUT,B_SPEAKER_OUT};

void save_audio_state(void) 
{
  int i,j;
  bool enabled;
  float gleft,gright;
  BDACStream B_Stream;   
  BADCStream A_Stream;   
  B_Stream.SamplingRate(&gleft);
  be_srate = gleft;
  for (i=0,j=0;i<BE_DEVICES;i++,j+=2)
    {
      B_Stream.GetVolume(be_devices[i],&gleft,&gright,&enabled);
      be_gains[j] = gleft;
      be_gains[j+1] = gright;
      be_mutes[i] = enabled;
    }
}

void restore_audio_state(void) 
{
  int i,j;
  BDACStream B_Stream;   
  BADCStream A_Stream;   
  B_Stream.SetSamplingRate(be_srate);
  for (i=0,j=0;i<BE_DEVICES;i++,j+=2)
    {
      B_Stream.EnableDevice(be_devices[i],be_mutes[i]);
      B_Stream.SetVolume(be_devices[i],be_gains[j],be_gains[j+1]);
    }
}

int initialize_audio(void) 
{
  AUDIO_ERROR = NO_ERROR;
  return(0);
}

#endif


/* ------------------------------- HPUX ----------------------------------------- */

#ifdef HPUX
#define AUDIO_OK
#include <sys/audio.h>

int open_audio_output(int ur_dev, int srate, int chans, int format, int size)
{
  int fd,i,dev;
  struct audio_describe desc;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  fd = open("/dev/audio",O_RDWR);
  if (fd == -1) {AUDIO_ERROR = CANT_OPEN; return(-1);}
  ioctl(fd,AUDIO_SET_CHANNELS,chans);
  if (dev == SPEAKERS_DEVICE)
    ioctl(fd,AUDIO_SET_OUTPUT,AUDIO_OUT_SPEAKER);
  else
    if (dev == LINE_OUT_DEVICE)
      ioctl(fd,AUDIO_SET_OUTPUT,AUDIO_OUT_LINE);
    else ioctl(fd,AUDIO_SET_OUTPUT,AUDIO_OUT_HEADPHONE);
  if (format == snd_16_linear)
    ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_LINEAR16BIT);
  else
    if (format == snd_8_mulaw)
      ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_ULAW);
  else 
    if (format == snd_8_alaw)
      ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_ALAW);
    else {AUDIO_ERROR = FORMAT_NOT_AVAILABLE; close(fd); return(-1);}
  ioctl(fd,AUDIO_DESCRIBE,&desc);
  for(i=0;i<desc.nrates;i++) if(srate == desc.sample_rate[i]) break;
  if (i == desc.nrates) {AUDIO_ERROR = SRATE_NOT_AVAILABLE; close(fd); return(-1);}
  ioctl(fd,AUDIO_SET_SAMPLE_RATE,srate);
  return(fd);
}

int write_audio(int line, char *buf, int bytes)
{
  AUDIO_ERROR = NO_ERROR;
  write(line,buf,bytes);
  return(0);
}

int close_audio(int line) 
{
  AUDIO_ERROR = NO_ERROR;
  close(line);
  return(0);
}

static void describe_audio_state_1(void)
{
  struct audio_describe desc;
  struct audio_gain gain;
  int mina,maxa,fd,tmp;
  int g[2];
  fd = open("/dev/audio",O_RDWR);
  if (fd == -1) return;
  ioctl(fd,AUDIO_GET_OUTPUT,&tmp);
  switch (tmp)
    {
    case AUDIO_OUT_SPEAKER: pprint("output: speakers\n"); break;
    case AUDIO_OUT_HEADPHONE: pprint("output: headphone\n"); break;
    case AUDIO_OUT_LINE: pprint("output: line out\n"); break;
    }
  ioctl(fd,AUDIO_GET_INPUT,&tmp);
  switch (tmp)
    {
    case AUDIO_IN_MIKE: pprint("input: mic\n"); break;
    case AUDIO_IN_LINE: pprint("input: line in\n"); break;
    }
  ioctl(fd,AUDIO_GET_DATA_FORMAT,&tmp);
  switch (tmp)
    {
    case AUDIO_FORMAT_LINEAR16BIT: pprint("format: 16-bit linear\n"); break;
    case AUDIO_FORMAT_ULAW: pprint("format: mulaw\n"); break;
    case AUDIO_FORMAT_ALAW: pprint("format: alaw\n"); break;
    }
  ioctl(fd,AUDIO_DESCRIBE,&desc);
  gain.channel_mask = (AUDIO_CHANNEL_LEFT | AUDIO_CHANNEL_RIGHT);
  ioctl(fd,AUDIO_GET_GAINS,&gain);
  close(fd);
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  g[0] = gain.cgain[0].transmit_gain; g[1] = gain.cgain[1].transmit_gain;
  mina = desc.min_transmit_gain;  maxa = desc.max_transmit_gain;
  sprintf(strbuf,"out vols: %.3f %.3f\n",(float)(g[0]-mina)/(float)(maxa-mina),(float)(g[1]-mina)/(float)(maxa-mina)); pprint(strbuf);
  g[0] = gain.cgain[0].receive_gain; g[1] = gain.cgain[1].receive_gain;
  mina = desc.min_receive_gain;  maxa = desc.max_receive_gain;
  sprintf(strbuf,"in vols: %.3f %.3f\n",(float)(g[0]-mina)/(float)(maxa-mina),(float)(g[1]-mina)/(float)(maxa-mina)); pprint(strbuf);
  g[0] = gain.cgain[0].monitor_gain; g[1] = gain.cgain[1].monitor_gain;
  mina = desc.min_monitor_gain;  maxa = desc.max_monitor_gain;
  sprintf(strbuf,"monitor vols: %.3f %.3f\n",(float)(g[0]-mina)/(float)(maxa-mina),(float)(g[1]-mina)/(float)(maxa-mina)); pprint(strbuf);
}

int read_audio_state(int ur_dev, int field, int chan, float *val) 
{
  struct audio_describe desc;
  struct audio_gain gain;
  int audio_fd,srate,g,maxa,mina,dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  if (field == DEVICE_FIELD)
    {
      val[0] = 4; 
      if (chan>1) val[1] = MICROPHONE_DEVICE; 
      if (chan>2) val[2] = DAC_OUT_DEVICE;
      if (chan>3) val[3] = LINE_OUT_DEVICE;
      if (chan>4) val[4] = LINE_IN_DEVICE;
    }
  else
    {
      if (field == FORMAT_FIELD)
        {
          val[0] = 3; 
          if (chan>1) val[1] = snd_16_linear;
          if (chan>2) val[2] = snd_8_mulaw;
          if (chan>3) val[3] = snd_8_alaw;
        }
      else
        {
          audio_fd = open("/dev/audio",O_RDWR);
          ioctl(audio_fd,AUDIO_DESCRIBE,&desc);
          switch (dev)
            {
            case DEFAULT_DEVICE:
            case DAC_OUT_DEVICE:
            case SPEAKERS_DEVICE:
            case LINE_OUT_DEVICE:
              switch (field)
                {
                case AMP_FIELD: 
                  ioctl(audio_fd,AUDIO_GET_GAINS,&gain);
                  if (chan == 0) g = gain.cgain[0].transmit_gain; else g = gain.cgain[1].transmit_gain;
                  mina = desc.min_transmit_gain;  maxa = desc.max_transmit_gain;
                  val[0] = (float)(g-mina)/(float)(maxa-mina);
                  break;
                case CHANNEL_FIELD: val[0] = 2; break;
                case SRATE_FIELD: 
                  ioctl(audio_fd,AUDIO_GET_SAMPLE_RATE,&srate); 
                  val[0] = srate; 
                  break;
                default: AUDIO_ERROR = CANT_READ; break;
                }
              break;
            case MICROPHONE_DEVICE:
            case LINE_IN_DEVICE:
            case READ_WRITE_DEVICE:
              switch (field)
                {
                case AMP_FIELD: 
                  ioctl(audio_fd,AUDIO_GET_GAINS,&gain);
                  if (chan == 0) g = gain.cgain[0].receive_gain; else g = gain.cgain[1].receive_gain;
                  mina = desc.min_receive_gain;  maxa = desc.max_receive_gain;
                  val[0] = (float)(g-mina)/(float)(maxa-mina);
                  break;
                case CHANNEL_FIELD: val[0] = 2; break;
                case SRATE_FIELD: 
                  ioctl(audio_fd,AUDIO_GET_SAMPLE_RATE,&srate); 
                  val[0] = srate; 
                  break;
                default: AUDIO_ERROR = CANT_READ; break;
                }
              break;
            default: AUDIO_ERROR = CANT_READ; break;
            }
          close(audio_fd);
        }
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

int write_audio_state(int ur_dev, int field, int chan, float *val) 
{
  struct audio_describe desc;
  struct audio_gain gain;
  int audio_fd,srate,g,maxa,mina,dev;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  audio_fd = open("/dev/audio",O_RDWR);
  ioctl(audio_fd,AUDIO_DESCRIBE,&desc);
  switch (dev)
    {
    case DEFAULT_DEVICE:
    case DAC_OUT_DEVICE:
    case SPEAKERS_DEVICE:
    case LINE_OUT_DEVICE:
      switch (field)
        {
        case AMP_FIELD: 
          mina = desc.min_transmit_gain;  maxa = desc.max_transmit_gain;
          ioctl(audio_fd,AUDIO_GET_GAINS,&gain);
          g = mina + val[0] * (maxa-mina);
          if (chan == 0) gain.cgain[0].transmit_gain = g; else gain.cgain[1].transmit_gain = g;
          ioctl(audio_fd,AUDIO_SET_GAINS,&gain);
          break;
        case SRATE_FIELD: 
          srate = val[0];
          ioctl(audio_fd,AUDIO_SET_SAMPLE_RATE,srate); 
          break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
      break;
    case MICROPHONE_DEVICE:
    case LINE_IN_DEVICE:
    case READ_WRITE_DEVICE:
      switch (field)
        {
        case AMP_FIELD: 
          mina = desc.min_receive_gain;  maxa = desc.max_receive_gain;
          ioctl(audio_fd,AUDIO_GET_GAINS,&gain);
          g = mina + val[0] * (maxa-mina);
          if (chan == 0) gain.cgain[0].receive_gain = g; else gain.cgain[1].receive_gain = g;
          ioctl(audio_fd,AUDIO_SET_GAINS,&gain);
          break;
        case SRATE_FIELD: 
          srate = val[0];
          ioctl(audio_fd,AUDIO_SET_SAMPLE_RATE,srate); 
          break;
        default: AUDIO_ERROR = CANT_WRITE; break;
        }
      break;
    default: AUDIO_ERROR = CANT_WRITE; break;
    }
  close(audio_fd);
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

static int saved_gains[6];

void save_audio_state(void)
{
  int fd;
  struct audio_gain gain;
  gain.channel_mask = (AUDIO_CHANNEL_LEFT | AUDIO_CHANNEL_RIGHT);
  fd = open("/dev/audio",O_RDWR);
  ioctl(fd,AUDIO_GET_GAINS,&gain);
  close(fd);
  saved_gains[0] = gain.cgain[0].transmit_gain;
  saved_gains[1] = gain.cgain[0].receive_gain;
  saved_gains[2] = gain.cgain[0].monitor_gain;
  saved_gains[3] = gain.cgain[1].transmit_gain;
  saved_gains[4] = gain.cgain[1].receive_gain;
  saved_gains[5] = gain.cgain[1].monitor_gain;
}

void restore_audio_state(void)
{
  int fd;
  struct audio_gain gain;
  gain.channel_mask = (AUDIO_CHANNEL_LEFT | AUDIO_CHANNEL_RIGHT);
  fd = open("/dev/audio",O_RDWR);
  ioctl(fd,AUDIO_GET_GAINS,&gain);
  gain.cgain[0].transmit_gain = saved_gains[0];
  gain.cgain[0].receive_gain = saved_gains[1];
  gain.cgain[0].monitor_gain = saved_gains[2];
  gain.cgain[1].transmit_gain = saved_gains[3];
  gain.cgain[1].receive_gain = saved_gains[4];
  gain.cgain[1].monitor_gain = saved_gains[5];
  ioctl(fd,AUDIO_SET_GAINS,&gain);
}

int initialize_audio(void) {AUDIO_ERROR = NO_ERROR; return(0);}

char *audio_error_name(int err) {return(audio_error_name_1(err));}
int audio_systems(void) {return(1);}
char *audio_system_name(int system) {return("HPUX");}

/* struct audio_status status_b;
 * ioctl(devAudio, AUDIO_GET_STATUS, &status_b)
 * not_busy = (status_b.transmit_status == AUDIO_DONE);
*/

int open_audio_input(int ur_dev, int srate, int chans, int format, int size) 
{
  int fd,i,dev;
  struct audio_describe desc;
  AUDIO_ERROR = NO_ERROR;
  dev = SNDLIB_DEVICE(ur_dev);
  fd = open("/dev/audio",O_RDWR);
  if (fd == -1) {AUDIO_ERROR = CANT_OPEN; return(-1);}
  ioctl(fd,AUDIO_SET_CHANNELS,chans);
  if (dev == MICROPHONE_DEVICE)
    ioctl(fd,AUDIO_SET_INPUT,AUDIO_IN_MIKE);
  else ioctl(fd,AUDIO_SET_INPUT,AUDIO_IN_LINE);
  if (format == snd_16_linear)
    ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_LINEAR16BIT);
  else
    if (format == snd_8_mulaw)
      ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_ULAW);
  else 
    if (format == snd_8_alaw)
      ioctl(fd,AUDIO_SET_DATA_FORMAT,AUDIO_FORMAT_ALAW);
    else {AUDIO_ERROR = FORMAT_NOT_AVAILABLE; close(fd); return(-1);}
  ioctl(fd,AUDIO_DESCRIBE,&desc);
  for(i=0;i<desc.nrates;i++) if(srate == desc.sample_rate[i]) break;
  if (i == desc.nrates) {AUDIO_ERROR = SRATE_NOT_AVAILABLE; close(fd); return(-1);}
  ioctl(fd,AUDIO_SET_SAMPLE_RATE,srate);
  return(fd);
}

int read_audio(int line, char *buf, int bytes) 
{
  AUDIO_ERROR = NO_ERROR;
  read(line,buf,bytes);
  return(0);
}

#endif


/* ------------------------------- WINDOZE ----------------------------------------- */

#ifdef WINDOZE
#define AUDIO_OK

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#define BUFFER_FILLED 1
#define BUFFER_EMPTY 2

#define OUTPUT_LINE 1
#define INPUT_LINE 2

#define SOUND_UNREADY 0
#define SOUND_INITIALIZED 1
#define SOUND_RUNNING 2

static int buffer_size = 1024;
static int db_state[2];
static int sound_state = 0;
static int current_chans = 1;
static int current_datum_size = 2;
static int current_buf = 0;
WAVEHDR wh[2];
HWAVEOUT fd;
HWAVEIN record_fd;
WAVEHDR rec_wh;
static int rec_state = SOUND_UNREADY;

static MMRESULT win_in_err = 0,win_out_err=0;
static char errstr[128],getstr[128];

char *audio_error_name(int err) 
{
  if ((win_in_err == 0) && (win_out_err == 0)) return(audio_error_name_1(err));
  if (win_in_err) 
    waveInGetErrorText(win_in_err,getstr,128);
  else waveOutGetErrorText(win_out_err,getstr,128);
  sprintf(errstr,"%s: %s",audio_error_name_1(err),getstr);
  return(errstr);
}

int audio_systems(void) 
{
  /* this number is available -- see below (user mixer number as in linux)->mixerGetNumDevs */
  return(1);
}
char *audio_system_name(int system) {return("Windoze");}

DWORD CALLBACK next_buffer(HWAVEOUT w, UINT msg, DWORD user_data, DWORD p1, DWORD p2)
{
  if (msg == WOM_DONE)
    {
      db_state[current_buf] = BUFFER_EMPTY;
    }
  return(0);
}

int open_audio_output(int ur_dev, int srate, int chans, int format, int size) 
{
  WAVEFORMATEX wf;
  int dev;
  AUDIO_ERROR = NO_ERROR; win_out_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  wf.nChannels = chans;
  current_chans = chans;
  wf.wFormatTag = WAVE_FORMAT_PCM;
  wf.cbSize = 0;
  if (format == snd_8_unsigned) 
    {
      wf.wBitsPerSample = 8;
      current_datum_size = 1;
    }
  else 
    {
      wf.wBitsPerSample = 16;
      current_datum_size = 2;
    }
  wf.nSamplesPerSec = srate;
  wf.nBlockAlign = chans * current_datum_size;
  wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;
  win_out_err = waveOutOpen(&fd,WAVE_MAPPER,&wf,(DWORD)next_buffer,0,CALLBACK_FUNCTION); /* 0 here = user_data above, other case = WAVE_FORMAT_QUERY */
  if (win_out_err) {AUDIO_ERROR = DEVICE_NOT_AVAILABLE; return(-1);}
  waveOutPause(fd);
  if (size <= 0) buffer_size = 1024; else buffer_size = size;
  wh[0].dwBufferLength = buffer_size * current_datum_size;
  wh[0].dwFlags = 0;
  wh[0].dwLoops = 0;
  wh[0].lpData = (char *)CALLOC(wh[0].dwBufferLength,sizeof(char));
  if ((wh[0].lpData) == 0) {AUDIO_ERROR = SIZE_NOT_AVAILABLE; waveOutClose(fd);  return(-1);}
  win_out_err = waveOutPrepareHeader(fd,&(wh[0]),sizeof(WAVEHDR));
  if (win_out_err) 
    {
      AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE; 
      FREE(wh[0].lpData); 
      waveOutClose(fd);  
      return(-1);
    }
  db_state[0] = BUFFER_EMPTY;
  wh[1].dwBufferLength = buffer_size * current_datum_size;
  wh[1].dwFlags = 0;
  wh[1].dwLoops = 0;
  wh[1].lpData = (char *)CALLOC(wh[0].dwBufferLength,sizeof(char));
  if ((wh[1].lpData) == 0) {AUDIO_ERROR = SIZE_NOT_AVAILABLE; FREE(wh[0].lpData); waveOutClose(fd);  return(-1);}
  win_out_err = waveOutPrepareHeader(fd,&(wh[1]),sizeof(WAVEHDR));
  if (win_out_err) 
    {
      AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE; 
      waveOutUnprepareHeader(fd,&(wh[0]),sizeof(WAVEHDR)); 
      FREE(wh[0].lpData); 
      FREE(wh[1].lpData); 
      waveOutClose(fd);  
      return(-1);
    }
  db_state[1] = BUFFER_EMPTY;
  sound_state = SOUND_INITIALIZED;
  current_buf = 0;
  return(OUTPUT_LINE);
}

static MMRESULT fill_buffer(int dbi, char *inbuf, int instart, int bytes)
{
  int i,j;
  win_out_err = 0;
  for (i=instart,j=0;j<bytes;j++,i++) wh[dbi].lpData[j] = inbuf[i];
  wh[dbi].dwBufferLength = bytes;
  db_state[dbi] = BUFFER_FILLED;
  if ((sound_state == SOUND_INITIALIZED) && (dbi == 1))
    {
      sound_state = SOUND_RUNNING;
      win_out_err = waveOutRestart(fd);
    }
  return(win_out_err);
}

static void wait_for_empty_buffer(int buf)
{
  while (db_state[buf] != BUFFER_EMPTY)
    {
      Sleep(1);      /* in millisecs, so even this may be too much if buf=256 bytes */
    }
}

int write_audio(int line, char *buf, int bytes) 
{
  int lim,leftover,start;
  if (line != OUTPUT_LINE) {AUDIO_ERROR = CANT_WRITE; return(-1);}
  AUDIO_ERROR = NO_ERROR; win_out_err = 0;
  leftover = bytes;
  start = 0;
  while (leftover > 0)
    {
      lim = leftover;
      if (lim > buffer_size) lim = buffer_size;
      leftover -= lim;
      wait_for_empty_buffer(current_buf);
      win_out_err = fill_buffer(current_buf,buf,start,lim);
      if (win_out_err) 
        {
          AUDIO_ERROR = CANT_WRITE;
          fprintf(stderr,"error %s upon fill",win_out_err);
        }
      win_out_err = waveOutWrite(fd,&wh[current_buf],sizeof(WAVEHDR));
      if (win_out_err)
        {
          AUDIO_ERROR = CANT_WRITE;
          fprintf(stderr,"error %s upon write",win_out_err);
        }
      start += lim;
      current_buf++;
      if (current_buf>1) current_buf=0;
    }
  if (AUDIO_ERROR != NO_ERROR) return(-1);
  return(0);
}

static int out_saved = 0, aux_saved = 0;
static DWORD *out_vols = NULL, *aux_vols = NULL;
static int *out_set = NULL, *aux_set = NULL;

void save_audio_state(void) 
{
  UINT dev;
  MMRESULT err;
  DWORD val;
  HWAVEOUT hd;
  WAVEOUTCAPS wocaps;
  AUXCAPS wacaps;
  WAVEFORMATEX pwfx;
  out_saved = waveOutGetNumDevs();
  if (out_vols) {FREE(out_vols); out_vols = NULL;}
  if (out_set) {FREE(out_set); out_set = NULL;}
  if (out_saved>0)
    {
      out_vols = (DWORD *)CALLOC(out_saved,sizeof(DWORD));
      out_set = (int *)CALLOC(out_saved,sizeof(int));
      for (dev=0;dev<out_saved;dev++)
        {
          err = waveOutGetDevCaps(dev,&wocaps,sizeof(wocaps));
          if ((!err) && (wocaps.dwSupport & WAVECAPS_VOLUME))
            {
              err = waveOutOpen(&hd,dev,&pwfx,NULL,NULL,WAVE_MAPPER);
              if (!err)
                {
                  err = waveOutGetVolume(hd,&val);
                  if (!err) 
                    {
                      out_vols[dev] = val;
                      out_set[dev] = 1;
                    }
                  waveOutClose(hd);
                }
            }
        }
    }
  aux_saved = auxGetNumDevs();
  if (aux_vols) {FREE(aux_vols); aux_vols = NULL;}
  if (aux_set) {FREE(aux_set); aux_set = NULL;}
  if (aux_saved>0)
    {
      aux_vols = (DWORD *)CALLOC(aux_saved,sizeof(unsigned long));
      aux_set = (int *)CALLOC(aux_saved,sizeof(int));
      for (dev=0;dev<aux_saved;dev++)
        {
          err = auxGetDevCaps(dev,&wacaps,sizeof(wacaps));
          if ((!err) && (wacaps.dwSupport & AUXCAPS_VOLUME))
            {
              err = auxGetVolume(dev,&val);
              if (!err) 
                {
                  aux_vols[dev] = val;
                  aux_set[dev] = 1;
                }
            }
        }
    }
  /* mixer state needs to be saved too, I suppose */
}

void restore_audio_state(void) 
{
  int i;
  HWAVEOUT hd;
  WAVEFORMATEX pwfx;
  MMRESULT err;
  for (i=0;i<out_saved;i++)
    if (out_set[i])
      {
        err = waveOutOpen(&hd,i,&pwfx,NULL,NULL,WAVE_MAPPER);
        if (!err)
          {
            waveOutSetVolume(hd,out_vols[i]);
            waveOutClose(hd);
          }
      }
  for (i=0;i<aux_saved;i++) if (aux_set[i]) auxSetVolume(i,aux_vols[i]);
}

static float unlog(unsigned short val)
{
  /* 1.0 linear is 0xffff, rest is said to be "logarithmic", whatever that really means here */
  if (val == 0) return(0.0);
  return((float)val / 65536.0);
  /* return(pow(2.0,amp) - 1.0); */ /* doc seems to be bogus */
}

#define SRATE_11025_BITS (WAVE_FORMAT_1S16 | WAVE_FORMAT_1S08 | WAVE_FORMAT_1M16 | WAVE_FORMAT_1M08)
#define SRATE_22050_BITS (WAVE_FORMAT_2S16 | WAVE_FORMAT_2S08 | WAVE_FORMAT_2M16 | WAVE_FORMAT_2M08)
#define SRATE_44100_BITS (WAVE_FORMAT_4S16 | WAVE_FORMAT_4S08 | WAVE_FORMAT_4M16 | WAVE_FORMAT_4M08)
#define SHORT_SAMPLE_BITS (WAVE_FORMAT_1S16 | WAVE_FORMAT_1M16 | WAVE_FORMAT_2S16 | WAVE_FORMAT_2M16 | WAVE_FORMAT_4S16 | WAVE_FORMAT_4M16)
#define BYTE_SAMPLE_BITS (WAVE_FORMAT_1S08 | WAVE_FORMAT_1M08 | WAVE_FORMAT_2S08 | WAVE_FORMAT_2M08 | WAVE_FORMAT_4S08 | WAVE_FORMAT_4M08)

static char *mfg(int mf)
{
  switch (mf)
    {
    case MM_MICROSOFT: return("Microsoft"); break;              case MM_CREATIVE: return("Creative Labs"); break; 
    case MM_MEDIAVISION: return("Media Vision"); break;         case MM_FUJITSU: return("Fujitsu Corp"); break;
    case MM_ARTISOFT: return("Artisoft"); break;                case MM_TURTLE_BEACH: return("Turtle Beach"); break;
    case MM_IBM: return("IBM"); break;                          case MM_VOCALTEC: return("Vocaltec"); break;
    case MM_ROLAND: return("Roland"); break;                    case MM_DSP_SOLUTIONS: return("DSP Solutions"); break;
    case MM_NEC: return("NEC"); break;                          case MM_ATI: return("ATI"); break;
    case MM_WANGLABS: return("Wang Laboratories"); break;       case MM_TANDY: return("Tandy"); break;
    case MM_VOYETRA: return("Voyetra"); break;                  case MM_ANTEX: return("Antex Electronics"); break;
    case MM_ICL_PS: return("ICL Personal Systems"); break;      case MM_INTEL: return("Intel"); break;
    case MM_GRAVIS: return("Advanced Gravis"); break;           case MM_VAL: return("Video Associates Labs"); break;
    case MM_INTERACTIVE: return("InterActive"); break;          case MM_YAMAHA: return("Yamaha"); break;
    case MM_EVEREX: return("Everex Systems"); break;            case MM_ECHO: return("Echo Speech"); break;
    case MM_SIERRA: return("Sierra Semiconductor"); break;      case MM_CAT: return("Computer Aided Technologies"); break;
    case MM_APPS: return("APPS Software"); break;               case MM_DSP_GROUP: return("DSP Group"); break;
    case MM_MELABS: return("microEngineering Labs"); break;     case MM_COMPUTER_FRIENDS: return("Computer Friends"); break;
    case MM_ESS: return("ESS Technology"); break;               case MM_AUDIOFILE: return("Audio"); break;
    case MM_MOTOROLA: return("Motorola"); break;                case MM_CANOPUS: return("Canopus"); break;
    case MM_EPSON: return("Seiko Epson"); break;                case MM_TRUEVISION: return("Truevision"); break;
    case MM_AZTECH: return("Aztech Labs"); break;               case MM_VIDEOLOGIC: return("Videologic"); break;
    case MM_SCALACS: return("SCALACS"); break;                  case MM_KORG: return("Korg"); break;
    case MM_APT: return("Audio Processing Technology"); break;  case MM_ICS: return("Integrated Circuit Systems"); break;
    case MM_ITERATEDSYS: return("Iterated Systems"); break;     case MM_METHEUS: return("Metheus"); break;
    case MM_LOGITECH: return("Logitech"); break;                case MM_WINNOV: return("Winnov"); break;
    case MM_NCR: return("NCR"); break;                          case MM_EXAN: return("EXAN"); break;
    case MM_AST: return("AST Research"); break;                 case MM_WILLOWPOND: return("Willow Pond"); break;
    case MM_SONICFOUNDRY: return("Sonic Foundry"); break;       case MM_VITEC: return("Vitec Multimedia"); break;
    case MM_MOSCOM: return("MOSCOM"); break;                    case MM_SILICONSOFT: return("Silicon Soft"); break;
    case MM_SUPERMAC: return("Supermac"); break;                case MM_AUDIOPT: return("Audio Processing Technology"); break;
    case MM_SPEECHCOMP: return("Speech Compression"); break;    case MM_DOLBY: return("Dolby Laboratories"); break;
    case MM_OKI: return("OKI"); break;                          case MM_AURAVISION: return("AuraVision"); break;
    case MM_OLIVETTI: return("Olivetti"); break;                case MM_IOMAGIC: return("I/O Magic"); break;
    case MM_MATSUSHITA: return("Matsushita Electric"); break;   case MM_CONTROLRES: return("Control Resources"); break;
    case MM_XEBEC: return("Xebec Multimedia Solutions"); break; case MM_NEWMEDIA: return("New Media"); break;
    case MM_NMS: return("Natural MicroSystems"); break;         case MM_LYRRUS: return("Lyrrus"); break;
    case MM_COMPUSIC: return("Compusic"); break;                case MM_OPTI: return("OPTi Computers"); break;
    case MM_DIALOGIC: return("Dialogic"); break;    
    }
  return("");
}

static char *mixer_status_name(int status)
{
  switch (status)
    {
    case MIXERLINE_LINEF_ACTIVE: return(", (active)"); break;
    case MIXERLINE_LINEF_DISCONNECTED: return(", (disconnected)"); break;
    case MIXERLINE_LINEF_SOURCE: return(", (source)"); break;
    default: return(""); break;
    }
}

static char *mixer_target_name(int type)
{
  switch (type)
    {
    case MIXERLINE_TARGETTYPE_UNDEFINED: return("undefined"); break;
    case MIXERLINE_TARGETTYPE_WAVEOUT: return("output"); break;
    case MIXERLINE_TARGETTYPE_WAVEIN: return("input"); break;
    case MIXERLINE_TARGETTYPE_MIDIOUT: return("midi output"); break;
    case MIXERLINE_TARGETTYPE_MIDIIN: return("midi input"); break;
    case MIXERLINE_TARGETTYPE_AUX: return("aux"); break;
    default: return(""); break;
    }
}

static char *mixer_component_name(int type)
{
  switch (type)
    {
    case MIXERLINE_COMPONENTTYPE_DST_UNDEFINED: return("undefined"); break;
    case MIXERLINE_COMPONENTTYPE_DST_DIGITAL: return("digital"); break;
    case MIXERLINE_COMPONENTTYPE_DST_LINE: return("line"); break;
    case MIXERLINE_COMPONENTTYPE_DST_MONITOR: return("monitor"); break;
    case MIXERLINE_COMPONENTTYPE_DST_SPEAKERS: return("speakers"); break;
    case MIXERLINE_COMPONENTTYPE_DST_HEADPHONES: return("headphones"); break;
    case MIXERLINE_COMPONENTTYPE_DST_TELEPHONE: return("telephone"); break;
    case MIXERLINE_COMPONENTTYPE_DST_WAVEIN: return("wave in"); break;
    case MIXERLINE_COMPONENTTYPE_DST_VOICEIN: return("voice in"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_UNDEFINED: return("undefined"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_DIGITAL: return("digital"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_LINE: return("line"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE: return("mic"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER: return("synth"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC: return("CD"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_TELEPHONE: return("telephone"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_PCSPEAKER: return("speaker"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT: return("wave out"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_AUXILIARY: return("aux"); break;
    case MIXERLINE_COMPONENTTYPE_SRC_ANALOG: return("analog"); break;
    default: return(""); break;
    }
}

#define MAX_DESCRIBE_CHANS 8
#define MAX_DESCRIBE_CONTROLS 16
/* these actually need to be big enough to handle whatever comes along, since we can't read partial states */
/*   or they need to be expanded as necessary */

static void describe_audio_state_1(void) 
{
  int devs,dev,srate,chans,format,need_comma,maker;
  MMRESULT err;
  unsigned long val,rate,pitch,version;
  WAVEOUTCAPS wocaps;
  WAVEINCAPS wicaps;
  AUXCAPS wacaps;
  HWAVEOUT hd;
  WAVEFORMATEX pwfx;
#ifdef MIXERR_BASE
  MIXERCAPS wmcaps;
  MIXERLINE mixline;
  MIXERLINECONTROLS linecontrols;
  MIXERCONTROL mc[MAX_DESCRIBE_CONTROLS];
  MIXERCONTROLDETAILS controldetails;
  MIXERCONTROLDETAILS_LISTTEXT clist[MAX_DESCRIBE_CHANS];
  MIXERCONTROLDETAILS_BOOLEAN cbool[MAX_DESCRIBE_CHANS];
  MIXERCONTROLDETAILS_UNSIGNED cline[MAX_DESCRIBE_CHANS];
  MIXERCONTROLDETAILS_SIGNED csign[MAX_DESCRIBE_CHANS];
  HMIXER mfd;
  int control,controls,dest,dests,source,happy,dest_time,chan,mina,maxa,ctype;
#endif
  need_comma = 1;
  chans=1;
  if (!strbuf) strbuf = (char *)CALLOC(STRBUF_SIZE,sizeof(char));
  devs = waveOutGetNumDevs();
  if (devs>0)
    {
      pprint("Output:\n");
      for (dev=0;dev<devs;dev++)
        {
          err = waveOutGetDevCaps(dev,&wocaps,sizeof(wocaps));
          if (!err)
            {
              version = wocaps.vDriverVersion;
              maker = wocaps.wMid;
              sprintf(strbuf,"  %s %s: version %d.%d\n",
                      mfg(maker),wocaps.szPname,
                      version>>8,version&0xff);
              pprint(strbuf);
              if (wocaps.wChannels == 2) {chans=2; pprint("    stereo");} else {chans=1; pprint("    mono");}
              if (wocaps.dwFormats & SRATE_11025_BITS)  {srate = 11025; if (need_comma) pprint(","); pprint(" 11025"); need_comma=1;}
              if (wocaps.dwFormats & SRATE_22050_BITS)  {srate = 22050; if (need_comma) pprint(","); pprint(" 22050"); need_comma=1;}
              if (wocaps.dwFormats & SRATE_44100_BITS)  {srate = 44100; if (need_comma) pprint(","); pprint(" 44100"); need_comma=1;}
              if (wocaps.dwFormats & BYTE_SAMPLE_BITS)  {format = 8; if (need_comma) pprint(","); pprint(" unsigned byte"); need_comma=1;}
              if (wocaps.dwFormats & SHORT_SAMPLE_BITS) {format = 16; if (need_comma) pprint(","); pprint(" little-endian short"); need_comma=1;}
              if (need_comma) pprint("\n");
              need_comma = 0;
              pwfx.wFormatTag = WAVE_FORMAT_PCM;
              pwfx.nChannels = chans;
              pwfx.nSamplesPerSec = srate;
              pwfx.nAvgBytesPerSec = srate;
              pwfx.nBlockAlign = 1;
              pwfx.wBitsPerSample = format;

              err = waveOutOpen(&hd,dev,&pwfx,NULL,NULL,WAVE_FORMAT_QUERY);

              if (wocaps.dwSupport & WAVECAPS_VOLUME)
                {
                  err = waveOutGetVolume(hd,&val);
                  if (!err)
                    {
                      if (wocaps.dwSupport & WAVECAPS_LRVOLUME)
                        sprintf(strbuf,"  vol: %.3f %.3f",unlog(val>>16),unlog(val&0xffff));
                      else sprintf(strbuf,"  vol: %.3f",unlog(val&0xffff));
                      pprint(strbuf);
                      need_comma = 1;
                    }
                }
              if (!err)
                {
                  /* this is just to get the hd data for subsequent info */
                  if (wocaps.dwSupport & WAVECAPS_PLAYBACKRATE)
                    {
                      err = waveOutGetPlaybackRate(hd,&rate);
                      if (!err)
                        {
                          sprintf(strbuf,"%s playback rate: %.3f",(need_comma ? "," : ""),(float)rate/65536.0);
                          pprint(strbuf);
                          need_comma = 1;
                        }
                    }
                  if (wocaps.dwSupport & WAVECAPS_PITCH)
                    {
                      err = waveOutGetPitch(hd,&pitch);
                      if (!err)
                        {
                          sprintf(strbuf,"%s pitch: %.3f",(need_comma ? "," : ""),(float)pitch/65536.0);
                          pprint(strbuf);
                          need_comma = 1;
                        }
                    }
                  waveOutClose(hd);
                }
              if (need_comma) {need_comma = 0; pprint("\n");}
            }
        }
    }
  devs = waveInGetNumDevs();
  if (devs>0)
    {
      pprint("Input:\n");
      for (dev=0;dev<devs;dev++)
        {
          err = waveInGetDevCaps(dev,&wicaps,sizeof(wicaps));
          if (!err)
            {
              sprintf(strbuf,"  %s%s",(wicaps.wMid != maker) ? mfg(wicaps.wMid) : "",wicaps.szPname);
              pprint(strbuf);
              if ((wicaps.wMid != maker) || (version != wicaps.vDriverVersion))
                {
                  sprintf(strbuf,": version %d.%d\n",(wicaps.vDriverVersion>>8),wicaps.vDriverVersion&0xff);
                  pprint(strbuf);
                }
              else pprint("\n");
              if (wicaps.wChannels == 2) pprint("    stereo"); else pprint("    mono");
              if (wicaps.dwFormats & SRATE_11025_BITS)  {pprint(", 11025"); need_comma=1;}
              if (wicaps.dwFormats & SRATE_22050_BITS)  {if (need_comma) pprint(","); pprint(" 22050"); need_comma=1;}
              if (wicaps.dwFormats & SRATE_44100_BITS)  {if (need_comma) pprint(","); pprint(" 44100"); need_comma=1;}
              if (wicaps.dwFormats & BYTE_SAMPLE_BITS)  {if (need_comma) pprint(","); pprint(" unsigned byte"); need_comma=1;}
              if (wicaps.dwFormats & SHORT_SAMPLE_BITS) {if (need_comma) pprint(","); pprint(" little-endian short");}
              pprint("\n");
            }
        }
    }
  devs = auxGetNumDevs();
  if (devs>0)
    {
      pprint("Auxiliary:\n");
      for (dev=0;dev<devs;dev++)
        {
          err = auxGetDevCaps(dev,&wacaps,sizeof(wacaps));
          if (!err)
            {
              sprintf(strbuf,"  %s%s",(wacaps.wMid != maker) ? mfg(wacaps.wMid) : "",wacaps.szPname);
              pprint(strbuf);
              if ((wacaps.wMid != maker) || (version != wacaps.vDriverVersion))
                sprintf(strbuf,": version %d.%d (%s)",
                        (wacaps.vDriverVersion>>8),wacaps.vDriverVersion&0xff,
                        (wacaps.wTechnology & AUXCAPS_CDAUDIO) ? "CD" : "");
              else sprintf(strbuf," (%s)\n",(wacaps.wTechnology & AUXCAPS_CDAUDIO) ? "CD" : "");
              pprint(strbuf);
              if (wacaps.dwSupport & AUXCAPS_VOLUME)
                {
                  err = auxGetVolume(dev,&val);
                  if (!err)
                    {
                      if (wacaps.dwSupport & AUXCAPS_LRVOLUME)
                        sprintf(strbuf,"  vol: %.3f %.3f\n",unlog(val>>16),unlog(val&0xffff));
                      else sprintf(strbuf,"  vol: %.3f\n",unlog(val&0xffff));
                      pprint(strbuf);
                    }
                }
            }
        }
    }
#ifdef MIXERR_BASE
  devs = mixerGetNumDevs();
  if (devs > 0)
    {
      pprint("Mixer:\n");
      for (dev=0;dev<devs;dev++)
        {
          err = mixerGetDevCaps(dev,&wmcaps,sizeof(wmcaps));
          if (!err)
            {
              sprintf(strbuf,"  %s%s",(wmcaps.wMid != maker) ? mfg(wmcaps.wMid) : "",wmcaps.szPname);
              pprint(strbuf);
              if ((wmcaps.wMid != maker) || (version != wmcaps.vDriverVersion))
                {
                  sprintf(strbuf,": version %d.%d\n",(wmcaps.vDriverVersion>>8),wmcaps.vDriverVersion&0xff);
                  pprint(strbuf);
                }
              else pprint("\n");
              dests = wmcaps.cDestinations;
              
              err = mixerOpen(&mfd,dev,NULL,NULL,CALLBACK_NULL);
              if (!err)
                {
                  dest=0;
                  source=0;
                  dest_time = 1;
                  happy = 1;
                  while (happy)
                    {
                      if (dest_time)
                        {
                          mixline.dwDestination = dest;
                          mixline.cbStruct = sizeof(MIXERLINE);
                          err = mixerGetLineInfo(mfd,&mixline,MIXER_GETLINEINFOF_DESTINATION);
                        }
                      else
                        {
                          mixline.dwSource = source;
                          mixline.cbStruct = sizeof(MIXERLINE);
                          err = mixerGetLineInfo(mfd,&mixline,MIXER_GETLINEINFOF_SOURCE);
                        }
                      if (!err)
                        {
                          if ((source == 0) && (!dest_time)) pprint("  Sources:\n");
                          if ((dest == 0) && (dest_time)) pprint("  Destinations:\n");
                          sprintf(strbuf,"    %s: %s (%s), %d chan%s",
                                  mixline.szName,
                                  mixer_component_name(mixline.dwComponentType),
                                  mixer_target_name(mixline.Target.dwType),
                                  
                                  mixline.cChannels,((mixline.cChannels != 1) ? "s" : ""));
                          pprint(strbuf);
                          if (mixline.cConnections > 0)
                                {
                                    sprintf(strbuf,", %d connection%s",
                                    mixline.cConnections,((mixline.cConnections != 1) ? "s" : ""));
                                    pprint(strbuf);
                                }
                          if (dest_time) 
                            {
                              sprintf(strbuf,"%s\n",mixer_status_name(mixline.fdwLine));
                              pprint(strbuf);
                            }
                          else pprint("\n");
                          if (mixline.cControls > 0)
                            {
                              linecontrols.cbStruct = sizeof(MIXERLINECONTROLS);
                              linecontrols.dwLineID = mixline.dwLineID;
                              linecontrols.dwControlID = MIXER_GETLINECONTROLSF_ONEBYID;
                              if (linecontrols.cControls > MAX_DESCRIBE_CONTROLS)
                                linecontrols.cControls = MAX_DESCRIBE_CONTROLS;
                              else linecontrols.cControls = mixline.cControls;
                              linecontrols.pamxctrl = mc;
                              linecontrols.cbmxctrl = sizeof(MIXERCONTROL);
                              err = mixerGetLineControls(mfd,&linecontrols,MIXER_GETLINECONTROLSF_ALL);
                              if (!err)
                                {
                                  sprintf(strbuf,"      %d control%s:\n",linecontrols.cControls,(linecontrols.cControls != 1) ? "s" : "");
                                  pprint(strbuf);
                                  controls = linecontrols.cControls;
                                  if (controls > MAX_DESCRIBE_CONTROLS) controls = MAX_DESCRIBE_CONTROLS;
                                  for (control=0;control<controls;control++)
                                    {

                                       sprintf(strbuf,"        %s",mc[control].szName);
                                       pprint(strbuf);
                                       controldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
                                       controldetails.dwControlID = mc[control].dwControlID;

                                       ctype = (mc[control].dwControlType);
                                       if ((ctype == MIXERCONTROL_CONTROLTYPE_EQUALIZER) ||
                                           (ctype == MIXERCONTROL_CONTROLTYPE_MUX) ||
                                           (ctype == MIXERCONTROL_CONTROLTYPE_MIXER) ||
                                           (ctype == MIXERCONTROL_CONTROLTYPE_SINGLESELECT) ||
                                           (ctype == MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT))
                                         {
                                           controldetails.cChannels = 1;
                                           controldetails.cMultipleItems = mc[control].cMultipleItems;
                                           controldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_LISTTEXT);
                                           controldetails.paDetails = clist;
                                           err = mixerGetControlDetails(mfd,&controldetails,MIXER_GETCONTROLDETAILSF_LISTTEXT);
                                           if (!err) 
                                             {
                                               for (chan=0;chan<mixline.cChannels;chan++) 
                                                 {
                                                   sprintf(strbuf," [%s]",clist[chan].szName);
                                                   pprint(strbuf);
                                                 }
                                             }
                                         }
                                       if (mixline.cChannels > MAX_DESCRIBE_CHANS)
                                         controldetails.cChannels = MAX_DESCRIBE_CHANS;
                                       else controldetails.cChannels = mixline.cChannels;
                                       controldetails.cMultipleItems = 0;
                                       err = 0;
                                       switch (mc[control].dwControlType & MIXERCONTROL_CT_UNITS_MASK)
                                         {
                                         case MIXERCONTROL_CT_UNITS_BOOLEAN:
                                           controldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
                                           controldetails.paDetails = cbool;
                                           break;
                                         case MIXERCONTROL_CT_UNITS_SIGNED: case MIXERCONTROL_CT_UNITS_DECIBELS:
                                           controldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_SIGNED);
                                           controldetails.paDetails = csign;
                                           break;
                                         case MIXERCONTROL_CT_UNITS_UNSIGNED: case MIXERCONTROL_CT_UNITS_PERCENT:
                                           controldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
                                           controldetails.paDetails = cline;
                                           break;
                                         default: err=1; break;
                                         }
                                       if (err) 
                                         pprint("\n");
                                       else
                                         {
                                           err = mixerGetControlDetails(mfd,&controldetails,MIXER_GETCONTROLDETAILSF_VALUE);
                                           if (!err)
                                             {
                                               chans = controldetails.cChannels;
                                               if (chans > MAX_DESCRIBE_CHANS) chans = MAX_DESCRIBE_CHANS;
                                               switch (mc[control].dwControlType & MIXERCONTROL_CT_UNITS_MASK)
                                                 {
                                                 case MIXERCONTROL_CT_UNITS_BOOLEAN:
                                                   for (chan=0;chan<chans;chan++)
                                                     {
                                                       sprintf(strbuf," %s",(cbool[chan].fValue) ? " on" : " off");
                                                       pprint(strbuf);
                                                     }
                                                   break;
                                                 case MIXERCONTROL_CT_UNITS_SIGNED: case MIXERCONTROL_CT_UNITS_DECIBELS:
                                                   mina = mc[control].Bounds.lMinimum;
                                                   maxa = mc[control].Bounds.lMaximum;
                                                   if (maxa > mina)
                                                     {
                                                       for (chan=0;chan<chans;chan++)
                                                         {
                                                           sprintf(strbuf," %.3f",(float)(csign[chan].lValue - mina)/(float)(maxa - mina));
                                                           pprint(strbuf);
                                                         }
                                                     }
                                                   break;
                                                 case MIXERCONTROL_CT_UNITS_UNSIGNED: case MIXERCONTROL_CT_UNITS_PERCENT:
                                                   mina = mc[control].Bounds.dwMinimum;
                                                   maxa = mc[control].Bounds.dwMaximum;
                                                   if (maxa > mina)
                                                     {
                                                       for (chan=0;chan<chans;chan++)
                                                         {
                                                           sprintf(strbuf," %.3f",(float)(cline[chan].dwValue - mina)/(float)(maxa - mina));
                                                           pprint(strbuf);
                                                         }
                                                     }
                                                   break;
                                                 default: break;
                                                 }
                                               pprint("\n");
                                             }
                                           else pprint("\n");
                                         }
                                    }
                                }
                            }
                        }
                      else if (!dest_time) happy = 0;
                      if (dest_time) dest++; else source++;
                      if (dest == dests) dest_time = 0;
                    }
                }
              mixerClose(mfd);
            }
        }
    }
#endif
}

int initialize_audio(void) 
{
  AUDIO_ERROR = NO_ERROR;
  return(0);
}

int close_audio(int line) 
{
  int i;
  AUDIO_ERROR = NO_ERROR; win_out_err = 0; win_in_err = 0;
  if (line == OUTPUT_LINE)
    {
      /* fill with a few zeros, wait for empty flag */
      if (sound_state != SOUND_UNREADY)
        {
          wait_for_empty_buffer(current_buf);
          for (i=0;i<128;i++) wh[current_buf].lpData[i] = 0;
          wait_for_empty_buffer(current_buf);
          win_out_err = waveOutClose(fd);
          while (win_out_err == WAVERR_STILLPLAYING)
            {
              win_out_err = waveOutClose(fd);
            }
          if (win_out_err) AUDIO_ERROR = CANT_CLOSE;
          waveOutUnprepareHeader(fd,&(wh[0]),sizeof(WAVEHDR));
          waveOutUnprepareHeader(fd,&(wh[1]),sizeof(WAVEHDR));
          FREE(wh[0].lpData);
          FREE(wh[1].lpData);
          db_state[0] = BUFFER_EMPTY;
          db_state[1] = BUFFER_EMPTY;
          sound_state = SOUND_UNREADY;
        }
    }
  else 
    {
      if (line == INPUT_LINE)
        {
          if (rec_state != SOUND_UNREADY)
            {
              waveInReset(record_fd);
              waveInClose(record_fd);
              waveInUnprepareHeader(record_fd,&rec_wh,sizeof(WAVEHDR));
              if (rec_wh.lpData) {FREE(rec_wh.lpData); rec_wh.lpData = NULL;}
              rec_state = SOUND_UNREADY;
            }
        }
      else AUDIO_ERROR = CANT_CLOSE;
    }
  return((AUDIO_ERROR == NO_ERROR) ? 0 : -1);
}

  /*
   * waveInAddBuffer sends buffer to get data
   * MM_WIM_DATA lParam->WAVEHDR dwBytesRecorded =>how much data actually in buffer
   */

static int current_record_chans = 0, current_record_datum_size = 0;

DWORD CALLBACK next_input_buffer(HWAVEIN w, UINT msg, DWORD user_data, DWORD p1, DWORD p2)
{
  if (msg == WIM_DATA)
    {
      /* grab data */
      /* p1->dwBytesRecorded */
    }
  return(0);
}

int open_audio_input(int ur_dev, int srate, int chans, int format, int size) 
{
  WAVEFORMATEX wf;
  int dev;
  AUDIO_ERROR = NO_ERROR; win_in_err = 0;
  dev = SNDLIB_DEVICE(ur_dev);
  wf.nChannels = chans;
  current_record_chans = chans;

  wf.wFormatTag = WAVE_FORMAT_PCM;
  wf.cbSize = 0;
  if (format == snd_8_unsigned) 
    {
      wf.wBitsPerSample = 8;
      current_record_datum_size = 1;
    }
  else 
    {
      wf.wBitsPerSample = 16;
      current_record_datum_size = 2;
    }
  wf.nSamplesPerSec = srate;
  wf.nBlockAlign = chans * current_datum_size;
  wf.nAvgBytesPerSec = wf.nBlockAlign * wf.nSamplesPerSec;

  rec_wh.dwBufferLength = size * current_record_datum_size;
  rec_wh.dwFlags = 0;
  rec_wh.dwLoops = 0;
  rec_wh.lpData = (char *)CALLOC(rec_wh.dwBufferLength,sizeof(char));
  if ((rec_wh.lpData) == 0) {AUDIO_ERROR = SIZE_NOT_AVAILABLE; return(-1);}
  win_in_err = waveInPrepareHeader(fd,&(rec_wh),sizeof(WAVEHDR));
  if (win_in_err) 
    {
      AUDIO_ERROR = CONFIGURATION_NOT_AVAILABLE; 
      FREE(rec_wh.lpData); 
      return(-1);
    }
  win_in_err = waveInOpen(&record_fd,WAVE_MAPPER,&wf,(DWORD)next_input_buffer,0,CALLBACK_FUNCTION);
  if (win_in_err) 
    {
      AUDIO_ERROR = DEVICE_NOT_AVAILABLE; 
      FREE(rec_wh.lpData);
      return(-1);
    }
  return(0);
}

int read_audio(int line, char *buf, int bytes) 
{
  AUDIO_ERROR = NO_ERROR;
  win_in_err = 0;
  return(-1);
}

int read_audio_state(int ur_dev, int field, int chan, float *val) 
{
  int dev;
  AUDIO_ERROR = CANT_READ; 
  dev = SNDLIB_DEVICE(ur_dev);
  /* how to map from device number to device function (i.e. microphone)? */
  /* is this where the mixer component field comes into play? */
  return(-1);
}

int write_audio_state(int ur_dev, int field, int chan, float *val) 
{
  int dev;
  AUDIO_ERROR = CANT_WRITE; 
  dev = SNDLIB_DEVICE(ur_dev);
  return(-1);
}

#endif


/* ------------------------------- STUBS ----------------------------------------- */

#ifndef AUDIO_OK
/* for dec alpha currently */
/* AudioFile? AFPlaySamples? -- need documentation */

static void describe_audio_state_1(void) {}
int open_audio_output(int dev, int srate, int chans, int format, int size) {return(-1);}
int open_audio_input(int dev, int srate, int chans, int format, int size) {return(-1);}
int write_audio(int line, char *buf, int bytes) {return(-1);}
int close_audio(int line) {return(-1);}
int read_audio(int line, char *buf, int bytes) {return(-1);}
int read_audio_state(int dev, int field, int chan, float *val) {return(-1);}
int write_audio_state(int dev, int field, int chan, float *val) {return(-1);}
void save_audio_state(void) {}
void restore_audio_state(void) {}
int initialize_audio(void) {return(-1);}
char *audio_error_name(int err) {return(audio_error_name_1(err));}
int audio_systems(void) {return(0);}
char *audio_system_name(int system) {return("unknown");}

#endif





static char *save_it = NULL;
static int print_it = 1;
static int save_it_len = 0;
static int save_it_loc = 0;

static void pprint(char *str)
{
  int i,len;
#ifdef MACOS
  char *ptr;
  int k;
#endif
  if ((str) && (*str))
    {
      if ((print_it) || (!(save_it)))
        {
#ifdef MACOS
          clm_printf(str);
#else
          fprintf(stdout,str);
          fflush(stdout);
#endif
        }
      else
        {
          len = strlen(str);
          for (i=0;i<len;i++,save_it_loc++)
            {
              if (save_it_loc == save_it_len)
                {
                  save_it_len += 1024;
#ifdef MACOS
                  ptr = (char *)CALLOC(save_it_len,sizeof(char));
                  if (save_it)
                    {
                      for (k=0;k<save_it_loc;k++) ptr[k] = save_it[k];
                      FREE(save_it);
                    }
                  save_it = ptr;
#else
                  save_it = (char *)REALLOC(save_it,save_it_len * sizeof(char));
#endif
                }
              save_it[save_it_loc] = str[i];
            }
        }
    }
}

char *report_audio_state(void)
{
  AUDIO_ERROR = NO_ERROR;
  if (!(save_it)) 
    {
      save_it_len = 1024;
      save_it = (char *)CALLOC(save_it_len,sizeof(char));
    }
  save_it_loc = 0;
  print_it = 0;
  describe_audio_state_1();
  return(save_it);
}

void describe_audio_state(void)
{
  AUDIO_ERROR = NO_ERROR;
  print_it = 1;
  describe_audio_state_1();
}


