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

#include "cddb.h"
#include "cdpmo.h"
#include "cddb_proto.h"

#include "player.h"

extern "C"
{
   MetaDataFormat *Initialize(FAContext* context)
   {
      return new CDDB(context);
   }
}

CDDB::CDDB(FAContext* context):MetaDataFormat(context)
{
    m_context = context;
    use_cddb_message = true;
    parse_disc_artist = true;
    parse_track_artist = true;
}

CDDB::~CDDB()
{
}

bool CDDB::ReadMetaData(const char* url, MetaData* metadata)
{
    bool retvalue = false;

    assert(url);
    assert(metadata);

    if(url && metadata)
    {
        Registry *pmoRegistry = m_context->player->GetPMORegistry();
        RegistryItem *pmo_item = NULL;
        int32 i = 0;

        while (NULL != (pmo_item = pmoRegistry->GetItem(i++))) {
            if (!strcmp("cd.pmo", pmo_item->Name())) {
                break;
            }
        }

        if (!pmo_item)
            return retvalue;

        PhysicalMediaOutput *pmo;
        pmo = (PhysicalMediaOutput *)pmo_item->InitFunction()(m_context);
        pmo->SetPropManager((Properties *)(m_context->player));
        pmo->Init(NULL);

        m_discinfo = ((CDPMO*)pmo)->GetDiscInfo();

        if (!m_discinfo.disc_present)
            return retvalue;

        m_discid = ((CDPMO*)pmo)->GetCDDBDiscID();

        m_total_tracks = m_discinfo.disc_total_tracks;

        delete pmo;

        struct disc_data data;
        if (cddb_read_disc_data(&data) < 0)
            return retvalue;

        char *tracknumber = strrchr(url, DIR_MARKER);
        if (!tracknumber)
            tracknumber = (char *)url;
        else
            tracknumber++;

        int m_track = atoi(tracknumber);

        if (m_track < 1)
            return retvalue;

        m_track--;
        metadata->SetAlbum(data.data_title);
        if (strlen(data.data_track[m_track].track_artist) > 0) 
            metadata->SetArtist(data.data_track[m_track].track_artist);
        else
            metadata->SetArtist(data.data_artist);
        metadata->SetTitle(data.data_track[m_track].track_name);
        metadata->SetComment(data.data_track[m_track].track_extended);
        metadata->SetTrack(m_track + 1);
        metadata->SetGenre(cddb_genre(data.data_genre));
        metadata->SetTime(m_discinfo.disc_track[m_track].track_length.minutes *
                          60 +  
                          m_discinfo.disc_track[m_track].track_length.seconds);

        retvalue = true;
    }
    return retvalue;
}

bool CDDB::WriteMetaData(const char* url, const MetaData& metadata)
{
    bool result = false;

    return result;
}
