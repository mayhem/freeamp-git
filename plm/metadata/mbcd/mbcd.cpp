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
    metadata->SetTime(m_trackLens[track - 1]);

    if (track == numTracks)
    {
       mb_Delete(o);
       o = NULL;
    }

    return true;
}

bool MusicBrainzCD::LookupCD(void)
{
    char          error[iDataLen], trackLens[1024], *ptr, *result;
    char          diskId[64];
    int           ret;
    char         *tempDir = new char[_MAX_PATH];
    uint32        length = _MAX_PATH;
    Database     *db;
    string        rdf;

    m_context->prefs->GetPrefString(kDatabaseDirPref, tempDir, &length);

    string database = string(tempDir) + string(DIR_MARKER_STR) +
                      string("mbcd");


    mb_SetServer(o, "www.musicbrainz.org", 80);

    ret = mb_Query(o, MB_GetCDTOC);
    if (!ret)
    {
       mb_GetQueryError(o, error, iDataLen);
       //m_context->target->AcceptEvent(new BrowserMessage(error));
       printf("%s\n", error);
       return false;
    }

    if (!mb_Select(o, MB_SelectLocalQuery))
    {
       return false;
    }
    if (!mb_GetResultData(o, MB_LocalGetTrackLengths, trackLens, 1024))
    {
       mb_GetQueryError(o, error, iDataLen);
       printf("\n\nCannot get track lengths: %s\n", error);
       return false;
    }

    m_trackLens.clear();
    for(ptr = strtok(trackLens, " "); ptr; ptr = strtok(NULL, " "))
       m_trackLens.push_back(atoi(ptr));

    mb_GetResultData(o, MB_LocalGetId, diskId, 64);

    db = new Database(database.c_str());
    delete [] tempDir;

    if (ReadFromCache(db, diskId, m_trackLens, rdf) &&
        mb_SetResultRDF(o, (char *)rdf.c_str()))
    { 
        //printf("Found in cache\n");
    }
    else
    {
        ret = mb_Query(o, MB_GetCDInfo);
        if (!ret)
        {
           mb_GetQueryError(o, error, iDataLen);
           //m_context->target->AcceptEvent(new BrowserMessage(error));
           printf("%s\n", error);
           delete db;
           return false;
        }
    
        if (mb_GetNumItems(o) == 0)
        {
           //m_context->target->AcceptEvent(new 
           //          BrowserMessage("CD not found."));
           printf("CD not found\n");
           delete db;
           return false;
        }  
    
        ret = mb_GetResultRDFLen(o) + 1;
        result = new char[ret];
        if (mb_GetResultRDF(o, result, ret))
        {
            WriteToCache(db, diskId, m_trackLens, result); 
        }
        delete result;
    }
    delete db;

    return true;
}


bool MusicBrainzCD::WriteMetaData(const char* url, const MetaData& metadata)
{
    bool result = false;
    return result;
}

void MusicBrainzCD::WriteToCache(Database *db, char *diskId, 
                                 vector<int> &trackLens, char *rdf)
{
    char                  *data;
    vector<int>::iterator  i;

    if (db->Exists(diskId))
    {
       db->Remove(diskId);
    }

    data = new char[strlen(diskId) + strlen(rdf) + 1024];
    data[0] = 0;
    for(i = trackLens.begin(); i != trackLens.end(); i++)
       sprintf(data + strlen(data), "%d ", *i); 

    sprintf(data + strlen(data) - 1, "%s", rdf); 
    db->Insert(diskId, data);
    delete data;
}

bool MusicBrainzCD::ReadFromCache(Database *db, char *diskId, 
                                 vector<int> &trackLens, string &rdf)
{
    char *data, temp[1024], *ptr;
    bool  ret = false;

    if (db->Exists(diskId))
    {
        data = db->Value(diskId);    

        if (sscanf(data, "%[^<]", temp) > 0)
        {
           trackLens.clear();
           for(ptr = strtok(temp, " "); ptr; ptr = strtok(NULL, " "))
              trackLens.push_back(atoi(ptr));

           ptr = strchr(data, '<');
           if (ptr)
           {
               rdf = string(ptr);
               ret = true;
           }
        }

        delete data;
    }

    return ret;
}
