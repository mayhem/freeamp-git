/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 2000 EMusic.com

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

#include <assert.h>
#include <string>
#include <errno.h>

using namespace std;

#include "config.h"
#include "errors.h"
#include "utility.h"

#include "mbcd.h"
#include "player.h"
#include "event.h"

const int iDataLen = 256;

extern "C"
{
   MetaDataFormat *Initialize(FAContext* context)
   {
      return new MusicBrainzCD(context);
   }
}

MusicBrainzCD::MusicBrainzCD(FAContext* context)
              :MetaDataFormat(context)
{
    o = NULL;
}

MusicBrainzCD::~MusicBrainzCD()
{
}

bool MusicBrainzCD::ReadMetaData(const char* url, MetaData* metadata)
{
    int  track, numTracks, i;
    char data[iDataLen];

    assert(url);
    assert(metadata);

    if(!url || !metadata)
       return false;

    char *ext = strrchr(url, '.');
    if (!ext)
       return false;

    if (strncasecmp(".CDA", ext, 4))
       return false;

    track = atoi(url + 7);
    if (track == 1)
    {
        if (o)
          mb_Delete(o);

        o = mb_New();
        if (!LookupCD())
        {
           mb_Delete(o);
           o = NULL; 
           return false;
        }
    }
    if (track > 1 && o == NULL)
       return false;

    mb_Select(o, MB_SelectFirstItem);  

    mb_GetResultData(o, MB_GetNumTracks, data, iDataLen);
    numTracks = atoi(data);

    mb_GetResultData(o, MB_GetAlbumName, data, iDataLen);
    metadata->SetAlbum(data);

    if (mb_DoesResultExist(o, MB_GetArtistName))
    {
        mb_GetResultData(o, MB_GetArtistName, data, iDataLen);
        metadata->SetArtist(data);

        mb_Select(o, MB_SelectFirstTrack);
        for(i = 0; i < track - 1; i++)
            mb_Select(o, MB_SelectNextTrack);

        mb_GetResultData(o, MB_GetTrackName, data, iDataLen);
        metadata->SetTitle(data);
    }
    else
    {
        mb_Select(o, MB_SelectFirstTrack);
        for(i = 0; i < track - 1; i++)
            mb_Select(o, MB_SelectNextTrack);

        mb_GetResultData(o, MB_GetArtistName, data, iDataLen);
        metadata->SetArtist(data);
        mb_GetResultData(o, MB_GetTrackName, data, iDataLen);
        metadata->SetTitle(data);
    }

    metadata->SetTrack(track);
    metadata->SetTime(0);

    if (track == numTracks)
    {
       mb_Delete(o);
       o = NULL;
    }

    return true;
}

bool MusicBrainzCD::LookupCD(void)
{
    char          error[iDataLen];
    int           ret;

    mb_SetServer(o, "www.musicbrainz.org", 80);
    ret = mb_Query(o, MB_GetCDInfo);
    if (!ret)
    {
       mb_GetQueryError(o, error, iDataLen);
       //m_context->target->AcceptEvent(new BrowserMessage(error));
       printf("%s\n", error);
       return false;
    }

    if (mb_GetNumItems(o) == 0)
    {
       //m_context->target->AcceptEvent(new BrowserMessage("CD not found."));
       printf("CD not found\n");
       return false;
    }  

    return true;
}


bool MusicBrainzCD::WriteMetaData(const char* url, const MetaData& metadata)
{
    bool result = false;
    return result;
}

