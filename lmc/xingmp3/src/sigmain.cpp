/*____________________________________________________________________________
        
        FreeAmp - The Free MP3 Player

        MP3 Decoder originally Copyright (C) 1995-1997 Xing Technology
        Corp.  http://www.xingtech.com

        Portions Copyright (C) 1998-1999 EMusic.com

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

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h> 

#include "metadata.h"

#include "plm/metadata/id3v1/id3v1.h"
#include "plm/metadata/id3v2/id3v2.h"
#include "musicbrainz/mb_c.h"

extern "C"
{
   int       ff_decode(char *filename,
                    char *fileout,
                    int reduction_code,
                    int convert_code,
                    int decode8_flag,
                    int freq_limit,
                    int integer_decode);
};

/*------------------------------------------*/
// Silly defines to make this and that happy in this little hatchet job
const char* protocol = "file://";
const char* kWriteID3v1Pref = "foo!";
const char* kWriteID3v2Pref = "bar!";
/*------------------------------------------*/

Error URLToFilePath(const char* url, char* path, uint32* length)
{
    Error result = kError_InvalidParam;

    assert(path);
    assert(url);
    assert(length);

    if(path && url && length && !strncasecmp(url, protocol, strlen(protocol)))
    {
        result = kError_BufferTooSmall;

        if(*length >= strlen(url) - strlen(protocol) + 1)
        {
            strcpy(path, url + strlen(protocol));
#ifdef WIN32
            if(strlen(path) > 1 && path[1] == '|')
            {
                path[1] = ':';
            }

            for(int32 index = strlen(path) - 1; index >=0; index--)
            {
                if(path[index] == '/')
                    path[index] = '\\';
            }
#endif
            result = kError_NoErr;
        }

        *length = strlen(url) - strlen(protocol) + 1;
    }

    return result;
}   

bool get_metadata(char *file, MetaData *m)
{
   ID3v1    id31(NULL);
   ID3v2    id3(NULL); // NULL means we can't call the WriteMetaData function
   string   url("file://");

   url += string(file);
   id31.ReadMetaData(url.c_str(), m);
   return id3.ReadMetaData(url.c_str(), m);
}

void submit_metadata(MetaData *pmetaData)
{
   musicbrainz_t o;
   int    ret;
   char   *args[11];
   char    temp[255];
   int     i;

   if (pmetaData == NULL)
       return;

   if (pmetaData->Artist().length() == 0 || 
       pmetaData->Album().length() == 0 ||
       pmetaData->Title().length() == 0)
   {
       printf("A track needs to have album, artist and title metadata "
              "information in\n the id3v1/2 tag in order to be submitted "
              "using this program.\n\n");
       return;
   }

   o = mb_New();
   mb_SetServer(o, "musicbrainz.eorbit.net", 80);

   args[0] = strdup(pmetaData->Title().c_str());
   args[1] = strdup(pmetaData->GUID().c_str());
   args[2] = strdup(pmetaData->Artist().c_str());
   args[3] = strdup(pmetaData->Album().c_str());
   sprintf(temp, "%d", pmetaData->Track());
   args[4] = strdup(temp);
   sprintf(temp, "%d", pmetaData->Time());
   args[5] = strdup(temp);
   sprintf(temp, "%d", pmetaData->Year());
   args[6] = strdup(temp);
   args[7] = strdup(pmetaData->Genre().c_str());
   args[8] = strdup(pmetaData->Comment().c_str());
   args[9] = NULL;

   ret = mb_QueryWithArgs(o, MB_SubmitTrack, args);
   for(i = 0; i < 9; i++)
      free(args[i]);

   if (!ret)
   {
       char err[255];
       mb_GetQueryError(o, err, 255);
       printf("Data submit error: %s\n\n", err);
   }
   else
   {
       printf("Data submitted to server.\n\n");
   }

   mb_Delete(o);  
}

/*------------------------------------------*/

int main(int argc, char *argv[])
{
   char        sig[37];
   MetaData    m;
   struct stat s;

   if (argc < 2)
   {
       printf("Usage: sigapp <mp3 file>\n");
       exit(0);
   }

   if (get_metadata(argv[1], &m))
   {
       stat(argv[1], &s);
       m.SetSize(s.st_size);
       printf("    Title: %s\n", m.Title().c_str());
       printf("    Album: %s\n", m.Album().c_str());
       printf("   Artist: %s\n", m.Artist().c_str());
       printf("    Genre: %s\n", m.Genre().c_str());
       printf("  Comment: %s\n", m.Comment().c_str());
       printf("    Track: %d\n", m.Track());
       printf("     Time: %d\n", m.Time());
       printf("     Size: %d\n", m.Size());

       if (ff_decode(argv[1], sig, 0, 0, 0, 24000, 0))
       {
           m.SetGUID(sig);
           printf("Signature: %s\n", sig);
           submit_metadata(&m);
       }
       else
       {
           printf("Error calculating signature.\n");
       }
   }
   else
   {
       printf("Error getting metadata.\n");
   }

   return 0;
}

