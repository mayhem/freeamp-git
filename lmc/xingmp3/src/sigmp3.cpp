/* (PD) 2001 The Bitzi Corporation
 * Please see file COPYING or http://bitzi.com/publicdomain 
 * for more info.
 *
 * $Id$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sigmp3.h"

static int mpeg1Bitrates[] = { 0, 32, 40, 48, 56, 64, 80, 96, 112, 
                               128, 160, 192, 224, 256, 320 };
static int mpeg2Bitrates[] = { 0, 8, 16, 24, 32, 40, 48, 56, 64, 
                               80, 96, 112, 128, 144, 160 };
static int mpeg1SampleRates[] = { 44100, 48000, 32000 };
static int mpeg2SampleRates[] = { 22050, 24000, 16000 };

static int bitrate(const char *header)
{
   int id, br;

   id = (header[1] & 0x8) >> 3;
   br = (header[2] & 0xF0) >> 4;

   // TODO: Add range checking 
   return id ? mpeg1Bitrates[br] : mpeg2Bitrates[br];
}

static int samplerate(const char *header)
{
   int id, sr;

   id = (header[1] & 0x8) >> 3;
   sr = (header[2] >> 2) & 0x3;

   // TODO: Add range checking 
   return id ? mpeg1SampleRates[sr] : mpeg2SampleRates[sr];
}

static int stereo(const char *header)
{
   return ((header[3] & 0xc0) >> 6) != 3;
}

static int mpeg_ver(const char *header)
{
   return (!((header[1] & 0x8) >> 3) + 1); 
}

static int padding(const char *header)
{
   return (header[2] >> 1) & 0x1;
}

void mp3_init(mp3_info *info)
{
   memset(info, 0, sizeof(mp3_info));
}

void mp3_final(mp3_info *info)
{
   /* If there are more bad bytes in a file, than there are good bytes,
      assume that the file is not an MP3 file and zero out all the values
      we've collected. Unfortunately there is no good way to detecting
      whether or not a file really is an MP3 file */
   if (info->badBytes > info->goodBytes)
   {
       memset(info, 0, sizeof(mp3_info));
   }
   else
   {
       if (info->mpegVer == 1)
          info->duration = info->frames * 1152 / (info->samplerate / 1000);
       else
          info->duration = info->frames * 576 / (info->samplerate / 1000);
       info->avgBitrate /= info->frames;
   }

   /*
   printf("du: %d\n", info->duration);
   printf("br: %d\n", info->bitrate);
   printf("sr: %d\n", info->samplerate);
   printf("fr: %d\n", info->frames);
   printf("vr: %d\n", info->mpegVer);
   printf("st: %d\n", info->stereo);
   */
}

void mp3_update(mp3_info      *info,
                unsigned char *buffer, 
                unsigned       len)
{
   unsigned       size, bytesLeft;
   unsigned char *ptr, *max;
   unsigned char *temp = NULL;

   /* If the a header spanned the last block and this block, then
      allocate a larger buffer and copy the last header plus the new
      block into the new buffer and work on it. This shouldn't happen
      very often. */
   if (info->spanningSize > 0)
   {
      temp = malloc(len + info->spanningSize);
      memcpy(temp, info->spanningHeader, info->spanningSize);
      memcpy(temp + info->spanningSize, buffer, len);
      len += info->spanningSize;
      buffer = temp;
   }

   /* Loop through the buffer trying to find frames */
   for(ptr = buffer + info->skipSize, max = buffer + len;
       ptr < max;)
   {
      /* printf("%02X%02X\n", ptr[0], ptr[1]);  */
      if ((unsigned int)max - (unsigned int)ptr < 4)
      {
         /* If we have a header that spans a block boundary, save
            up to 3 bytes and then return */
         info->spanningSize = (unsigned int)max - (unsigned int)ptr;
         memcpy(info->spanningHeader, ptr, info->spanningSize);
         info->skipSize = 0;

         if (temp)
            free(temp);

         return;
      }
 
      /* Find the frame marker */
      if (*ptr != 0xFF || ((*(ptr + 1) & 0xF0) != 0xF0 &&
                           (*(ptr + 1) & 0xF0) != 0xE0)) 
      {
          info->badBytes ++;
          ptr++;
          continue;
      }

      /* Check for invalid sample rates */
      if (samplerate(ptr) == 0)
      { 
          info->badBytes ++;
          ptr++;
          continue;
      }

      /* Calculate the size of the frame from the header components */
      if (mpeg_ver(ptr) == 1)
          size = (144000 * bitrate(ptr)) / samplerate(ptr) + padding(ptr);
      else
          size = (72000 * bitrate(ptr)) / samplerate(ptr) + padding(ptr);
      if (size <= 1 || size > 2048)
      {
          info->badBytes ++;
          ptr++;
          continue;
      }

      /* If this is the first frame, then tuck away important info */
      if (info->frames == 0)
      {
          info->samplerate = samplerate(ptr);
          info->bitrate = bitrate(ptr);
          info->mpegVer = mpeg_ver(ptr);
          info->stereo = stereo(ptr);
      }
      else
      {
          /* The sample rate inside of a file should never change. If the
             header says it did, then assume that we found a bad header 
             and skip past it. */
          if (info->samplerate != samplerate(ptr))
          {
             info->badBytes ++;
             ptr++;
             continue;
          }

          /* If the bitrate in subsequent frames is different from the
             first frame, then we have a VBR file */
          if (info->bitrate && info->bitrate != bitrate(ptr))
          {
             info->bitrate = 0;
          }
      }
  
      /* 
      printf("%08d: [%04d] %3d %5d %d %d\n", 
              (unsigned int)ptr - (unsigned int)buffer,
              info->frames, bitrate(ptr), 
              samplerate(ptr), size, padding(ptr)); 
      */ 

      bytesLeft = (unsigned int)max - (unsigned int)ptr;

      /* Move the memory pointer past the frame */
      info->frames++;
      info->goodBytes += size;
      info->avgBitrate += bitrate(ptr);
      ptr += size;
   }

   /* skipSize defines the number of bytes to skip in the next block,
      so that we're not searching for the frame marker inside of
      a frame, which can lead to false hits. Grrr. 
      Vielen Dank, Karl-Heinz Brandenburg! */
   info->skipSize = (unsigned int)ptr - (unsigned int)max;
   info->spanningSize = 0;
   if (temp)
      free(temp);
}
