/*____________________________________________________________________________
	
	FreeAmp - The Free MP3 Player

	Portions Copyright (C) 2000 EMusic.com
        Portions Copyright (C) 1998-1999 Tony  Arcieri

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

#ifndef INCLUDED_CDDB_PROTO_H
#define INCLUDED_CDDB_PROTO_H

#define MAX_TRACKS                              100

/* CDDB defaults */
#define CDDB_PROTOCOL_LEVEL                     3
#define CDDBP_DEFAULT_PORT                      888
#define HTTP_DEFAULT_PORT                       80
#define CDDB_MAX_SERVERS                        128
#define MAX_INEXACT_MATCHES                     16
#define EXTENDED_DATA_SIZE                      4096
#define DISC_ART_SIZE                           32768
#define CDINDEX_ID_SIZE                         30

/* Connection modes */
#define CDDB_MODE_CDDBP                         0
#define CDDB_MODE_HTTP                          1
#define CDINDEX_MODE_HTTP                       2
#define COVERART_MODE_HTTP                      3
#define CDDB_ACCESS_LOCAL                       0
#define CDDB_ACCESS_REMOTE                      1
#define CDDB_PROXY_DISABLED                     0
#define CDDB_PROXY_ENABLED                      1

/* CDDB genres */
#define CDDB_UNKNOWN                            0
#define CDDB_BLUES                              1
#define CDDB_CLASSICAL                          2
#define CDDB_COUNTRY                            3
#define CDDB_DATA                               4
#define CDDB_FOLK                               5
#define CDDB_JAZZ                               6
#define CDDB_MISC                               7
#define CDDB_NEWAGE                             8
#define CDDB_REGGAE                             9
#define CDDB_ROCK                               10
#define CDDB_SOUNDTRACK                         11

#ifndef __INCLUDED_CDPMO_H
/* Used for keeping track of times */
struct disc_timeval {
   int minutes;
   int seconds;
   int frames;
};

/* Track specific information */
struct track_info {
   struct disc_timeval track_length;            /* Length of track */
   struct disc_timeval track_pos;               /* Position of track */
   int track_lba;                               /* Logical Block Address */
   int track_type;                              /* CDAUDIO_TRACK_AUDIO or CDAUDIO_TRACK_DATA */
};

/* Disc information such as current track, amount played, etc */
struct disc_info {
   int disc_present;                            /* Is disc present? */
   int disc_mode;                               /* Current disc mode */
   struct disc_timeval disc_track_time;         /* Current track time */
   struct disc_timeval disc_time;               /* Current disc time */
   struct disc_timeval disc_length;             /* Total disc length */
   int disc_current_track;                      /* Current track */
   int disc_first_track;                        /* First track on the disc */
   int disc_total_tracks;                       /* Number of tracks on disc */
   struct track_info disc_track[MAX_TRACKS];    /* Track specific information */
};
#endif

/* CDDB entry */
struct cddb_entry {
   int entry_present;                           /* Is there an entry? */
   long entry_timestamp;                        /* Last modification time */
   unsigned long entry_id;                      /* CDDB ID of entry */
   int entry_genre;                             /* CDDB genre of entry */
};

/* CDDB configuration */
struct cddb_conf {
   int conf_access;                             /* CDDB_ACCESS_LOCAL or CDDB_ACCESS_REMOTE */
   int conf_proxy;                              /* Is a proxy being used? */
};

/* Server retrieval structure */
struct cddb_server {
   char server_name[256];                       /* Server name */
   int server_port;                             /* Server port */
};

/* CDDB server list structure */
struct cddb_host {
   struct cddb_server host_server;
   char host_addressing[256];
   int host_protocol;
};

/* CDDB server list structure */
struct cddb_serverlist {
   int list_len;
   struct cddb_host list_host[CDDB_MAX_SERVERS];
};

/* CDDB hello structure */
struct cddb_hello {
   char hello_program[256];                     /* Program name*/
   char hello_version[256];                     /* Version */
};

/* An entry in the query list */
struct query_list_entry {
   int list_genre;                              /* CDDB genre of entry */
   unsigned long list_id;                       /* CDDB ID of entry */
   char list_title[64];                         /* Title of entry */
   char list_artist[64];                        /* Entry's artist */
};

/* TOC data for CDDB query */
struct cddb_direct_query_toc {
   unsigned long toc_id;
   int tracks;
   int positions[MAX_TRACKS];
};

/* CDDB query structure */
struct cddb_query_t {
#define QUERY_NOMATCH                           0
#define QUERY_EXACT                             1
#define QUERY_INEXACT                           2
   int query_match;                             /* Uses above definitions */
   int query_matches;                           /* Number of matches */
   struct query_list_entry query_list[MAX_INEXACT_MATCHES];
};

/* Track database structure */
struct track_data {
   char track_name[256];                        /* Track name */
   char track_artist[256];                      /* Track specific artist */
   char track_extended[EXTENDED_DATA_SIZE];     /* Extended information */
};

/* Disc database structure */
struct disc_data {
   unsigned long data_id;                       /* CDDB ID */
   int data_revision;                           /* CDDB revision (incremented with each submit) */
   char data_title[256];                        /* Disc title */
   char data_artist[256];                       /* Album artist */
   char data_extended[EXTENDED_DATA_SIZE];      /* Extended information */
   int data_genre;                              /* Disc genre */
   int data_artist_type;                        /* Single or multiple artist CD */
   struct track_data data_track[MAX_TRACKS];    /* Track names */
};

int cddb_read_disc_data(int cd_desc, unsigned long discid, 
                        struct disc_data *outdata);

#define MAX_EXTEMPORANEOUS_LINES                  6
#define MAX_EXTENDED_LINES                        64

/* Internal CDDB structures used for preprocessing */
struct __unprocessed_track_data {
   int track_name_index;
   char track_name[MAX_EXTEMPORANEOUS_LINES][80];
   int track_extended_index;
   char track_extended[MAX_EXTENDED_LINES][80];
};

struct __unprocessed_disc_data {
   unsigned long data_id;
   int data_revision;
   int data_title_index;
   char data_title[MAX_EXTEMPORANEOUS_LINES][80];
   int data_extended_index;
   char data_extended[MAX_EXTENDED_LINES][80];
   int data_genre;
   struct __unprocessed_track_data data_track[MAX_TRACKS];
};

#endif // INCLUDED_CDDB_PROTO_H

