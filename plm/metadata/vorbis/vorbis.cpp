/*____________________________________________________________________________
    
    FreeAmp - The Free MP3 Player

    Portions Copyright (C) 1999 EMusic.com

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
#include <math.h>
#include <assert.h>
#include <string>
#include <errno.h>

using namespace std;

#include "config.h"
#include "errors.h"
#include "utility.h"

#include "vorbis.h"

#define CHUNK 4096

extern    "C"
{
   MetaDataFormat *Initialize(FAContext * context)
   {
      return new Vorbis(context);
   }
}

Vorbis::Vorbis(FAContext * context):MetaDataFormat(context)
{
}

Vorbis::~Vorbis()
{

}

bool Vorbis::WriteMetaData(const char *url, const MetaData & metadata)
{
   return false;
}

bool Vorbis::ReadMetaData(const char *url, MetaData * metadata)
{
   vorbis_comment vc;
   char           path[_MAX_PATH];
   uint32         length = sizeof(path);
   int            i;

   URLToFilePath(url, path, &length);
   if (ReadHeader(path, vc))
   {
       for(i=0;i<vc.comments;i++)
       {
           if (strncmp(vc.user_comments[i], "title=", 6) == 0)
           {
               metadata->SetTitle(vc.user_comments[i] + 6);
               continue;
           }
           if (strncmp(vc.user_comments[i], "artist=", 7) == 0)
           {
               metadata->SetArtist(vc.user_comments[i] + 7);
               continue;
           }
           if (strncmp(vc.user_comments[i], "album=", 6) == 0)
           {
               metadata->SetAlbum(vc.user_comments[i] + 6);
               continue;
           }
       }  
       metadata->SetTrack(0);
       return true;
   }

   return false;
}

bool Vorbis::ReadHeader(const char *url, vorbis_comment &vc)
{
   char     *buffer;
   int       bytes;
   int       i;
   int       serial;
   FILE     *input = NULL;
   bool      ret = false;

   vorbis_info vi;
   ogg_sync_state oy;
   ogg_stream_state os;
   ogg_page  og;

   ogg_packet *header;
   ogg_packet header_main;
   ogg_packet header_comments;
   ogg_packet header_codebooks;

   ogg_sync_init(&oy);

   input = fopen(url, "rb");
   if (input == NULL)
      goto cleanup;

   buffer = ogg_sync_buffer(&oy, CHUNK);
   bytes = fread(buffer, 1, CHUNK, input);
   ogg_sync_wrote(&oy, bytes);

   if (ogg_sync_pageout(&oy, &og) != 1)
      goto cleanup;

   serial = ogg_page_serialno(&og);
   ogg_stream_init(&os, serial);

   vorbis_info_init(&vi);
   vorbis_comment_init(&vc);

   if (ogg_stream_pagein(&os, &og) < 0)
      goto cleanup;

   if (ogg_stream_packetout(&os, &header_main) != 1)
      goto cleanup;

   if (vorbis_synthesis_headerin(&vi, &vc, &header_main) < 0)
      goto cleanup;

   i = 0;
   header = &header_comments;
   while (i < 2)
   {
      while (i < 2)
      {
         int  result = ogg_sync_pageout(&oy, &og);

         if (result == 0)
            break;
         else if (result == 1)
         {
            ogg_stream_pagein(&os, &og);
            while (i < 2)
            {
               result = ogg_stream_packetout(&os, header);
               if (result == 0)
                  break;
               if (result == -1)
               {
                  goto cleanup;
               }
               vorbis_synthesis_headerin(&vi, &vc, header);
               i++;
               header = &header_codebooks;
            }
         }
      }
      buffer = ogg_sync_buffer(&oy, CHUNK);
      bytes = fread(buffer, 1, 4096, input);
      if (bytes == 0 && i < 2)
         goto cleanup;

      ogg_sync_wrote(&oy, bytes);
   }

   ret = true;

   cleanup:

   ogg_sync_clear(&oy);
   ogg_stream_clear(&os);
   vorbis_info_clear(&vi);

   if (input)
     fclose(input);

   return ret;
}

#if 0
bool Vorbis::WriteHeader(const char *url, vorbis_comment &vcnew)
{
   unsigned char *buffer;
   int       bytes;
   int       i;
   int       serial;
   int       eosin = 0, eosout = 0;
   FILE     *input = NULL, *output;
   bool      ret = false;

   vorbis_comment vc;
   vorbis_info vi;
   vorbis_dsp_state vd;
   ogg_sync_state oy;
   ogg_stream_state os;
   ogg_stream_state out;
   ogg_page  og;
   ogg_page  outpage;
   ogg_packet op;

   ogg_packet *header;
   ogg_packet header_main;
   ogg_packet header_comments;
   ogg_packet header_codebooks;

   input = fopen(argv[2], "rb");
   if (input == NULL)
      goto cleanup;

   ogg_sync_init(&oy);

   buffer = ogg_sync_buffer(&oy, CHUNK);
   bytes = fread(buffer, 1, CHUNK, input);
   ogg_sync_wrote(&oy, bytes);

   if (ogg_sync_pageout(&oy, &og) != 1)
      goto cleanup;

   serial = ogg_page_serialno(&og);
   ogg_stream_init(&os, serial);

   vorbis_info_init(&vi);
   vorbis_comment_init(&vc);

   if (ogg_stream_pagein(&os, &og) < 0)
      goto cleanup;

   if (ogg_stream_packetout(&os, &header_main) != 1)
      goto cleanup;

   if (vorbis_synthesis_headerin(&vi, &vc, &header_main) < 0)
      goto cleanup;

   i = 0;
   header = &header_comments;
   while (i < 2)
   {
      while (i < 2)
      {
         int  result = ogg_sync_pageout(&oy, &og);

         if (result == 0)
            break;
         else if (result == 1)
         {
            ogg_stream_pagein(&os, &og);
            while (i < 2)
            {
               result = ogg_stream_packetout(&os, header);
               if (result == 0)
                  break;
               if (result == -1)
               {
                  goto cleanup;
               }
               vorbis_synthesis_headerin(&vi, &vc, header);
               i++;
               header = &header_codebooks;
            }
         }
      }
      buffer = ogg_sync_buffer(&oy, CHUNK);
      bytes = fread(buffer, 1, 4096, input);
      if (bytes == 0 && i < 2)
         goto cleanup;

      ogg_sync_wrote(&oy, bytes);
   }

   vorbis_comment_clear(&vc);

   /* The following needs some explanation - the API doesn't currently give
      us * a `clean' way of doing this. * * We need to call
      vorbis_analysis_headerout() to build the header packets, * but the
      first argument to that is a vorbis_dsp_state *. * So, we need to
      initialise this, for which we have vorbis_analysis_init(), * but this
      doesn't work - because our vorbis_info struct is set up for  * DECODE, 
      and so is missing the encode-only parameters, so we crash. * * To
      avoid this, we take a look inside block.c, figure out what  *
      vorbis_analysis_init() does, and what we actually NEED to do - it
      turns * out that it's reasonably simple, we just need to clear
      everything else, * and set the vorbis_info pointer to the one we
      decoded from the other * header packets. */

   // vorbis_analysis_init(&vd,&vi);
   memset(&vd, 0, sizeof(vd));
   vd.vi = &vi;              /* Ugly hack? Hell yeah! */

   /* End ugly hack */

   ogg_stream_init(&out, serial);

   vorbis_analysis_headerout(&vd, &vcnew, &header_main, &header_comments, &header_codebooks);

   ogg_stream_packetin(&out, &header_main);
   ogg_stream_packetin(&out, &header_comments);
   ogg_stream_packetin(&out, &header_codebooks);

   /* Hard bit - stream ogg packets out, ogg packets in */

   while (!(eosin && eosout))
   {
      while (!(eosin && !eosout))
      {

         int result = ogg_sync_pageout(&oy, &og);

         if (result == 0)
            break;
         else 
         if (result == -1)
         {
             ; 
         }
         else
         {
            ogg_stream_pagein(&os, &og);

            while (1)
            {
               result = ogg_stream_packetout(&os, &op);
               if (result == 0)
                  break;
               else if (result == -1)
               {
               }
               else
               {
                  ogg_stream_packetin(&out, &op);

                  while (!(eosin && eosout))
                  {
                     int       result = ogg_stream_pageout(&out, &outpage);

                     if (result == 0)
                        break;
                     fwrite(outpage.header, 1, outpage.header_len, output);
                     fwrite(outpage.body, 1, outpage.body_len, output);

                     if (ogg_page_eos(&outpage))
                        eosout = 1;
                  }
               }
            }
            if (ogg_page_eos(&og))
               eosin = 1;
         }
      }
      if (!(eosin && eosout))
      {
         buffer = ogg_sync_buffer(&oy, CHUNK);
         bytes = fread(buffer, 1, CHUNK, input);
         ogg_sync_wrote(&oy, bytes);
         if (bytes == 0)
            eosin = 1;
      }
   }

   ret = true;

   cleanup:

   ogg_stream_clear(&os);
   ogg_stream_clear(&out);
   ogg_sync_clear(oy);
   vorbis_info_clear(&vi);

   if (input)
     fclose(input);

   if (output)
     fclose(output);

   return ret;
}
#endif
