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

#ifndef INCLUDED_CDDB_H
#define INCLUDED_CDDB_H

#include <string>

using namespace std;

#include <assert.h>

#if defined(solaris)
#include <stdarg.h>
#endif

#include "config.h"
#include "errors.h"
#include "facontext.h"

#include "metadata.h"
#include "cddb_proto.h"

class CDDB : public MetaDataFormat {
 public:
    CDDB(FAContext *context);
    virtual ~CDDB();

    virtual bool ReadMetaData(const char* url, MetaData* metadata);
    virtual bool WriteMetaData(const char* url, const MetaData& metadata);
    virtual MetaDataOrder Order() { return MetaDataOrder_Low; }

 private:
    unsigned long     m_discid;
    int               m_total_tracks;
    struct disc_info *m_discinfo;

    int cddb_read_disc_data(struct disc_data *outdata);
    int cddb_generate_unknown_entry(struct disc_data *outdata);
    int cddb_process_line(char *line, struct __unprocessed_disc_data *data);
    int cddb_read_data(struct disc_data *data);
    int cddb_quit(int sock);
    int cddb_read(int sock, int mode, struct cddb_entry *entry, 
                  struct disc_data *data, char *http_string = NULL);
    const char *cddb_genre(int genre);
    int cddb_genre_value(char *genre);
    int cddb_read_line(int sock, char *inbuffer, int len);
    int cddb_read_token(int sock, int token[3]);
    int cddb_skip_http_header(int sock);
    int cddb_generate_http_request(char *outbuffer, const char *cmd, 
                                   char *http_string, int outbuffer_len);
    int cddb_connect_server(struct cddb_host host, struct cddb_server *proxy,
                            struct cddb_hello *hello, ...);
    int cddb_connect(struct cddb_server *server);
    int cddb_process_url(struct cddb_host *host, const char *url);  
    int cddb_read_serverlist(struct cddb_conf *conf, 
                             struct cddb_serverlist *list,
                             struct cddb_server *proxy);
    int cddb_query(int sock, int mode, struct cddb_query_t *query, ...);
    int cddb_write_data(struct disc_data *indata);
 
    int data_process_control_codes(char *outbuffer, char *inbuffer, int len);
    int data_process_block(char *outbuffer, int len, char (*block)[80],
                           int bounds); 
    int data_format_line(char *outbuffer, char *inbuffer, int len);
    int data_format_block(char (*block)[80], int arraylen, char *inbuffer);
    int data_format_input(struct disc_data *outdata,
                          struct __unprocessed_disc_data *indata);
    int data_format_output(struct __unprocessed_disc_data *outdata,
                           struct disc_data *indata);

    char cddb_message[256];
    bool use_cddb_message;
    bool parse_disc_artist;
    bool parse_track_artist;
};



#endif // INCLUDED_CDDB_H

