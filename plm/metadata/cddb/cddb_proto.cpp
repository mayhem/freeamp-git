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

#include "config.h"
#undef socklen_t

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#ifndef INADDR_NONE
#define INADDR_NONE 0xFFFFFFFF
#endif

#include "database.h"
#include "cddb.h"
#include "cddb_proto.h"

#ifndef HAVE_SNPRINTF
int snprintf(char *dest, int size, const char *format, ...)
{
    va_list ap;
    int result;

    va_start(ap, format);
    result = vsprintf(dest, format, ap);
    va_end(ap);
    return result;
}
#endif  /* HAVE_SNPRINTF */

char *CDDB::cddb_genre(int genre)
{
   switch(genre) {
      case CDDB_BLUES: return "blues";
      case CDDB_CLASSICAL: return "classical";
      case CDDB_COUNTRY: return "country";
      case CDDB_DATA: return "data";
      case CDDB_FOLK: return "folk";
      case CDDB_JAZZ: return "jazz";
      case CDDB_MISC: return "misc";
      case CDDB_NEWAGE: return "newage";
      case CDDB_REGGAE: return "reggae";
      case CDDB_ROCK: return "rock";
      case CDDB_SOUNDTRACK: return "soundtrack";
      default: return "unknown";
   }
}

int CDDB::cddb_genre_value(char *genre)
{
   if (!strcmp(genre, "blues"))
       return CDDB_BLUES;
   else if (!strcmp(genre, "classical"))
       return CDDB_CLASSICAL;
   else if (!strcmp(genre, "country"))
       return CDDB_COUNTRY;
   else if (!strcmp(genre, "data"))
       return CDDB_DATA;
   else if (!strcmp(genre, "folk"))
       return CDDB_FOLK;
   else if (!strcmp(genre, "jazz"))
       return CDDB_JAZZ;
   else if (!strcmp(genre, "misc"))
       return CDDB_MISC;
   else if (!strcmp(genre, "newage"))
       return CDDB_NEWAGE;
   else if (!strcmp(genre, "reggae"))
       return CDDB_REGGAE;
   else if (!strcmp(genre, "rock"))
       return CDDB_ROCK;
   else if (!strcmp(genre, "soundtrack"))
       return CDDB_SOUNDTRACK;
   else
       return CDDB_UNKNOWN;
}

int CDDB::cddb_query(int sock, int mode, struct cddb_query_t *query, ...) 
{
   int index, slashed = 0, token[3];
   struct disc_info disc = m_discinfo;
   char *outbuffer, *outtemp, *inbuffer, *origchunk, *http_string;
   va_list arglist;


   va_start(arglist, query);
   query->query_matches = 0;

   outbuffer = new char[1024];
   outtemp = new char[1024];

   if(mode == CDDB_MODE_HTTP) {
      http_string = va_arg(arglist, char *);

      snprintf(outbuffer, 1024, "%d", m_total_tracks);
      for(index = 0; index < m_total_tracks; index++) {
         snprintf(outtemp, 1024, "%s+%d", outbuffer, (disc.disc_track[index].track_pos.minutes * 60 + disc.disc_track[index].track_pos.seconds) * 75 + disc.disc_track[index].track_pos.frames);
         strncpy(outbuffer, outtemp, 1024);
      }
      snprintf(outtemp, 1024, "cddb+query+%08lx+%s+%d", m_discid, outbuffer, disc.disc_length.minutes * 60 + disc.disc_length.seconds);
      cddb_generate_http_request(outbuffer, outtemp, http_string, 1024);
   } else {
      snprintf(outbuffer, 1024, "%d", m_total_tracks);
      for(index = 0; index < m_total_tracks; index++) {
         snprintf(outtemp, 1024, "%s %d", outbuffer, (disc.disc_track[index].track_pos.minutes * 60 + disc.disc_track[index].track_pos.seconds) * 75 + disc.disc_track[index].track_pos.frames);
         strncpy(outbuffer, outtemp, 1024);
      }
      strncpy(outtemp, outbuffer, 1024);
      snprintf(outbuffer, 1024, "cddb query %08lx %s %d\n", m_discid, outtemp, disc.disc_length.minutes * 60 + disc.disc_length.seconds);
   }

   delete outtemp;
   va_end(arglist);

   if(write(sock, outbuffer, strlen(outbuffer)) < 0) {
      delete outbuffer;
      return -1;
   }

   delete outbuffer;

   if(mode == CDDB_MODE_HTTP)
     cddb_skip_http_header(sock);

   inbuffer = new char[256];
   origchunk = inbuffer;

   if(cddb_read_line(sock, inbuffer, 256) < 0) {
      delete inbuffer;
      return -1;
   }

   if(strncmp(inbuffer, "<!DOC", 5) == 0) {
      if(use_cddb_message)
        strncpy(cddb_message, "404 CDDB CGI not found", 256);
      delete inbuffer;
      return -1;
   }

   token[0] = inbuffer[0] - 48;
   token[1] = inbuffer[1] - 48;
   token[2] = inbuffer[2] - 48;

   if(use_cddb_message)
     strncpy(cddb_message, inbuffer + 4, 256);

   if(token[0] != 2) {
      delete inbuffer;
      return -1;
   }

   if(token[1] == 0) {
      if(token[2] != 0) {
         query->query_match = QUERY_NOMATCH;
         delete inbuffer;
         return 0;
      }

      query->query_match = QUERY_EXACT;
      query->query_matches = 1;
      slashed = 0;
      if(strchr(inbuffer, '/') != NULL && parse_disc_artist) {
         index = 0;
         while(inbuffer[index] != '/' && inbuffer[index] != '\0')
           index++;
         inbuffer[index - 1] = '\0';
         strncpy(query->query_list[0].list_title, inbuffer + index + 2, 64);
         slashed = 1;
      }

      index = 0;
      while(inbuffer[index] != ' ' && inbuffer[index] != '\0')
        index++;
      if(inbuffer[index] == '\0')
        return -1;
      inbuffer += index + 1;
      index = 0;
      while(inbuffer[index] != ' ' && inbuffer[index] != '\0')
        index++;
      if(inbuffer[index] == '\0')
        return -1;
      inbuffer[index] = '\0';
      query->query_list[0].list_genre = cddb_genre_value(inbuffer);
      inbuffer += index + 1;
      index = 0;
      while(inbuffer[index] != ' ' && inbuffer[index] != '\0')
        index++;
      if(inbuffer[index] == '\0')
        return -1;
      inbuffer[index] = '\0';
      query->query_list[0].list_id = strtoul(inbuffer, NULL, 16);
      inbuffer += index + 1;
      if(slashed)
         strncpy(query->query_list[0].list_artist, inbuffer, 64);
      else {
         strncpy(query->query_list[0].list_title, inbuffer, 64);
         strncpy(query->query_list[0].list_artist, "", 64);
      }
      inbuffer = origchunk;
   } else if(token[1] == 1) {
      if(token[2] == 0)
        query->query_match = QUERY_EXACT;
      else if(token[2] == 1)
        query->query_match = QUERY_INEXACT;
      else {
        query->query_match = QUERY_NOMATCH;
        free(inbuffer);
        return 0;
      }

      query->query_matches = 0;
      while(!cddb_read_line(sock, inbuffer, 256)) {
         slashed = 0;
         if(strchr(inbuffer, '/') != NULL && parse_disc_artist) {
            index = 0;
            while(inbuffer[index] != '/' && inbuffer[index] != '\0')
              index++;
            inbuffer[index - 1] = '\0';
            strncpy(query->query_list[query->query_matches].list_title, inbuffer + index + 2, 64);
            slashed = 1;
         }

         index = 0;
         while(inbuffer[index] != ' ' && inbuffer[index] != '\0')
           index++;
         if(inbuffer[index] == '\0')
           return -1;
         inbuffer[index] = '\0';
         query->query_list[query->query_matches].list_genre = cddb_genre_value(inbuffer);
         inbuffer += index + 1;
         index = 0;
         while(inbuffer[index] != ' ' && inbuffer[index] != '\0')
           index++;
         if(inbuffer[index] == '\0')
           return -1;
         inbuffer[index] = '\0';
         query->query_list[query->query_matches].list_id = strtoul(inbuffer, NULL, 16);
         inbuffer += index + 1;
         if(slashed)
            strncpy(query->query_list[query->query_matches++].list_artist, inbuffer, 64);
         else {
            strncpy(query->query_list[query->query_matches].list_title, inbuffer, 64);
            strncpy(query->query_list[query->query_matches++].list_artist, "", 64);
         }
         inbuffer = origchunk;
      }
   } else {
      query->query_match = QUERY_NOMATCH;
      delete inbuffer;
      return 0;
   }

   delete inbuffer;
   return 0;
}

int CDDB::cddb_process_url(struct cddb_host *host, const char *url)
{
   int index = 0;
   char *procbuffer;

   host->host_addressing[0] = '\0';

   if (strchr(url, ':') == NULL)
       return -1;

   while (url[index++] != ':' && index < 527)
       if (index > 5)
           return -1;

   if (!strncmp(url, "http", index - 1)) {
       host->host_protocol = CDDB_MODE_HTTP;
       host->host_server.server_port = HTTP_DEFAULT_PORT;
   } 
   else if (!strncmp(url, "cddbp", index - 1)) {
       host->host_protocol = CDDB_MODE_CDDBP;
       host->host_server.server_port = CDDBP_DEFAULT_PORT;
   } 
   else
       return -1;

   url += (index - 1);

   if (strncmp(url, "://", 3) != 0)
       return -1;

   url += 3;

   index = 0;
   while (url[index] != ':' && url[index] != '\0' && url[index] != '/' && 
          index < 527) {
       index++;
       if (index > 256)
           return -1;
   }

   memset(host->host_server.server_name, '\0', 256);
   strncpy(host->host_server.server_name, url, (index < 256) ? index : 256);


   if (url[index] == ':') {
       url += (index + 1);
       index = 0;
       while (url[index] != '\0' && url[index] != '/' && index < 527) {
           index++;
           if (index > 5)
               return -1;
       }

       procbuffer = new char[index + 1];

       memset(procbuffer, '\0', index + 1);
       strncpy(procbuffer, url, index);
       host->host_server.server_port = strtol(procbuffer, NULL, 10);
       delete procbuffer;
   }

   if (url[index] == '/') {
       url += (index + 1);
       if (url[0] == '\0')
           return 0;
       index = 0;
       while (url[index++] != '\0')
           if (index > 256)
               return -1;
       strncpy(host->host_addressing, url, index);

       return 0;
   }

   return 0;
}


int CDDB::cddb_read_serverlist(struct cddb_conf *conf, 
                               struct cddb_serverlist *list, 
                               struct cddb_server *proxy)
{
    conf->conf_access = CDDB_ACCESS_REMOTE;    

    char *host = new char[1024];
    uint32 length = 1024;
    struct cddb_host proxy_host;
    bool useproxy;

    m_context->prefs->GetUseProxyServer(&useproxy);
    conf->conf_proxy = CDDB_PROXY_DISABLED;
    if (useproxy) {
        conf->conf_proxy = CDDB_PROXY_ENABLED;
        if (IsError(m_context->prefs->GetProxyServerAddress(host, &length))) {
            delete [] host;
            host = new char[length];

            if (IsError(m_context->prefs->GetProxyServerAddress(host, &length)))
            {
                conf->conf_proxy = CDDB_PROXY_DISABLED;
            }
            else {
                cddb_process_url(&proxy_host, host);
                memcpy(proxy, &proxy_host.host_server, 
                       sizeof(struct cddb_server));
            }
        }
        else {
            cddb_process_url(&proxy_host, host);
            memcpy(proxy, &proxy_host.host_server, sizeof(struct cddb_server));
        }
    }

    if (IsError(m_context->prefs->GetCDDBServerAddress(host, &length))) {
        delete [] host;
        host = new char[length];
        if (IsError(m_context->prefs->GetCDDBServerAddress(host, &length))) {
            delete [] host;
            return -1;
        }
    }
    cddb_process_url(&list->list_host[0], host);
    list->list_len++;

    return 0;
}
    
int CDDB::cddb_connect(struct cddb_server *server)
{
   int sock;
   struct sockaddr_in sin;
   struct hostent *host;

   sin.sin_family = AF_INET;
   sin.sin_port = htons(server->server_port);

   if ((sin.sin_addr.s_addr = inet_addr(server->server_name)) == INADDR_NONE) {
       if ((host = gethostbyname(server->server_name)) == NULL) {
           if (use_cddb_message)
               strncpy(cddb_message, strerror(errno), 256);
         return -1;
      }

      memcpy(&sin.sin_addr, host->h_addr, host->h_length);
   }

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      if (use_cddb_message)
          strncpy(cddb_message, strerror(errno), 256);
      return -1;
   }

   if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
      if (use_cddb_message)
          strncpy(cddb_message, strerror(errno), 256);
      return -1;
   }

   return sock;
}

int CDDB::cddb_connect_server(struct cddb_host host, struct cddb_server *proxy,
                              struct cddb_hello hello, ...)
{
   int sock, token[3], http_string_len;
   char *outbuffer, *http_string;
   va_list arglist;

   va_start(arglist, hello);

   if (proxy != NULL) {
       if ((sock = cddb_connect(proxy)) < 0)
           return -1;
   } 
   else {
       if ((sock = cddb_connect(&host.host_server)) < 0)
           return -1;
   }

   if (host.host_protocol == CDDB_MODE_HTTP) {
       http_string = va_arg(arglist, char *);
       http_string_len = va_arg(arglist, int);
       if (proxy != NULL)
           snprintf(http_string, http_string_len, "GET http://%s:%d/%s?hello=anonymous+anonymous+%s+%s&proto=%d HTTP/1.0\n\n", host.host_server.server_name, host.host_server.server_port, host.host_addressing, hello.hello_program, hello.hello_version, CDDB_PROTOCOL_LEVEL);
       else
           snprintf(http_string, http_string_len, "GET /%s?hello=anonymous+anonymous+%s+%s&proto=%d HTTP/1.0\n\n", host.host_addressing, hello.hello_program, hello.hello_version, CDDB_PROTOCOL_LEVEL);
   } 
   else {
       if (cddb_read_token(sock, token) < 0) {
           va_end(arglist);
           return -1;
       }

       if (token[0] != 2) {
           va_end(arglist);
           return -1;
       }

       outbuffer = new char[256];

       snprintf(outbuffer, 256, "cddb hello anonymous anonymous %s %s\n", hello.hello_program, hello.hello_version);

       if (write(sock, outbuffer, strlen(outbuffer)) < 0) {
           delete outbuffer;
           va_end(arglist);
           return -1;
       }

       if (cddb_read_token(sock, token) < 0) {
           delete outbuffer;
           va_end(arglist);
           return -1;
       }

       if (token[0] != 2) {
           delete outbuffer;
           va_end(arglist);
           return -1;
       }

       snprintf(outbuffer, 256, "proto %d\n", CDDB_PROTOCOL_LEVEL);
       if (write(sock, outbuffer, strlen(outbuffer)) < 0) {
           delete outbuffer;
           va_end(arglist);
           return -1;
       }

       delete outbuffer;

       if (cddb_read_token(sock, token) < 0) {
           va_end(arglist);
           return -1;
       }
   }

   va_end(arglist);
   return sock;
}

int CDDB::cddb_generate_http_request(char *outbuffer, const char *cmd, 
                                     char *http_string, int outbuffer_len)
{
    int index = 0;
    char *reqstring;

    if (!strchr(http_string, '?'))   
        return -1;

    while (http_string[index] != '?' && http_string[index] != '\0')
        index++;

    http_string[index] = '\0';
    reqstring = http_string + index + 1;

    snprintf(outbuffer, outbuffer_len, "%s?cmd=%s&%s\n", http_string, cmd, 
             reqstring);
    http_string[index] = '\?';

    return 0;
}

int CDDB::cddb_skip_http_header(int sock)
{
   char inchar;
   int len;

   do {
      len = 0;
      do {
         if (read(sock, &inchar, 1) < 1) {
            if (use_cddb_message)
                strncpy(cddb_message, "Unexpected socket closure", 256);
            return -1;
         }
         len++;
      } while (inchar != '\n');
   } while (len > 2);

   return 0;
}

int CDDB::cddb_read_token(int sock, int token[3])
{
    char *inbuffer;

    inbuffer = new char[512];
    if (cddb_read_line(sock, inbuffer, 512) < 0) {
        delete inbuffer;
        return -1;
    }
    
    if (!strncmp(inbuffer, "<!DOC", 5)) {
        if (use_cddb_message)
            strncpy(cddb_message, "404 CDDB CGI not found", 256);
        delete inbuffer;
    }

    token[0] = inbuffer[0] - 48;
    token[1] = inbuffer[1] - 48;
    token[2] = inbuffer[2] - 48;

    if (use_cddb_message)
        strncpy(cddb_message, (char *)inbuffer + 4, 256);
    delete inbuffer;

    return 0;
}

int CDDB::cddb_read_line(int sock, char *inbuffer, int len)
{
    int index;
    char inchar;

    for (index = 0; index < len; index++) {
        read(sock, &inchar, 1);
        if (inchar == '\n') {
            inbuffer[index] = '\0';
            if (inbuffer[0] == '.')
                return 1;
            return 0;
        }
        inbuffer[index] = inchar;
    }

    return index;
}

int CDDB::cddb_vread(int sock, int mode, struct cddb_entry *entry, 
                     struct disc_data *data, va_list arglist)
{
   int index, token[3];
   char *outbuffer, *proc, *http_string;
   struct __unprocessed_disc_data indata;

   indata.data_id = m_discid;
   outbuffer = new char[512];
   proc = new char[512];

   indata.data_genre = entry->entry_genre;
   indata.data_title_index = 0;
   indata.data_extended_index = 0;
   for (index = 0; index < m_total_tracks; index++) {
       indata.data_track[index].track_name_index = 0;
       indata.data_track[index].track_extended_index = 0;
   }

   if (mode == CDDB_MODE_HTTP) {
       http_string = va_arg(arglist, char *);
       snprintf(proc, 512, "cddb+read+%s+%08lx", 
                cddb_genre(entry->entry_genre), entry->entry_id);
       cddb_generate_http_request(outbuffer, proc, http_string, 512);
   } 
   else
      snprintf(outbuffer, 512, "cddb read %s %08lx\n", 
               cddb_genre(entry->entry_genre), entry->entry_id);

   write(sock, outbuffer, strlen(outbuffer));

   delete [] outbuffer;

   if (mode == CDDB_MODE_HTTP)
       cddb_skip_http_header(sock);

   if (cddb_read_token(sock, token) < 0)
       return -1;

   if (token[0] != 2 && token[1] != 1)
       return -1;

   while (!cddb_read_line(sock, proc, 512))
       cddb_process_line(proc, &indata);

   data_format_input(data, &indata);
   data->data_revision++;

   delete [] proc;

   return 0;
}

int CDDB::cddb_read(int sock, int mode, struct cddb_entry entry, 
                    struct disc_data *data, ...)
{
    int ret;
    va_list arglist;

    va_start(arglist, data);
    ret = cddb_vread(sock, mode, &entry, data, arglist);
    va_end(arglist);

    return ret;
}

int CDDB::cddb_quit(int sock)
{
    char outbuffer[8];

    strcpy(outbuffer, "quit\n");
    write(sock, outbuffer, strlen(outbuffer));
 
    shutdown(sock, 2);
    close(sock);
 
    return 0;
}

int CDDB::cddb_read_data(struct disc_data *data)
{
   int sock = -1, index;
   char *http_string;
   struct cddb_entry entry;
   struct cddb_hello hello;
   struct cddb_query_t query;
   struct cddb_conf conf;
   struct cddb_server *proxy_ptr;
   struct cddb_serverlist list;

   http_string = new char[512];
   proxy_ptr = new struct cddb_server;
   
   cddb_read_serverlist(&conf, &list, proxy_ptr);
   if (!conf.conf_access) {
       delete [] http_string;
       delete proxy_ptr;
       return -1;
   }
   if (!conf.conf_proxy) {
       delete proxy_ptr;
       proxy_ptr = NULL;
   }

   if (list.list_len < 1) {
       delete [] http_string;
       return -1;
   }

   strncpy(hello.hello_program, "libcdaudio", 256);
   strncpy(hello.hello_version, "0.99.4", 256);

   index = 0;

   /* Connect to a server */
   do {
      switch(list.list_host[index].host_protocol) {
          case CDDB_MODE_CDDBP:
              sock = cddb_connect_server(list.list_host[index++], proxy_ptr, 
                                         hello);
              break;
          case CDDB_MODE_HTTP:
              sock = cddb_connect_server(list.list_host[index++], proxy_ptr, 
                                         hello, http_string, 512);
              break;
      }
   } while (index < list.list_len && sock == -1);

   if (sock < 0) {
       if (conf.conf_proxy)
           delete proxy_ptr;
       delete [] http_string;
       return -1;
   }
    
   index--;

   /* CDDB Query, not nessecary for CD Index operations */
   switch (list.list_host[index].host_protocol) {
       case CDDB_MODE_CDDBP: {
           if (cddb_query(sock, CDDB_MODE_CDDBP, &query) < 0) {
              if (conf.conf_proxy) 
                  delete proxy_ptr;
              delete [] http_string;
              return -1;
           } 
           break; }
       case CDDB_MODE_HTTP: {
           if (cddb_query(sock, CDDB_MODE_HTTP, &query, http_string) < 0) {
              if (conf.conf_proxy) 
                  delete proxy_ptr;
              delete http_string;
              return -1;
           }
           shutdown(sock, 2);
           close(sock);

           if ((sock = cddb_connect_server(list.list_host[index], proxy_ptr, 
                                           hello, http_string, 512)) < 0) {
               if (conf.conf_proxy) 
                   delete proxy_ptr;
               delete http_string;
               return -1;
            }
            break; }
   }

   if (conf.conf_proxy) 
       delete proxy_ptr;
   
   /* Since this is an automated operation, we'll assume inexact matches are
      correct. */

   entry.entry_id = query.query_list[0].list_id;
   entry.entry_genre = query.query_list[0].list_genre;

   /* Read operation */
   switch (list.list_host[index].host_protocol) {
       case CDDB_MODE_CDDBP: {
           if (cddb_read(sock, CDDB_MODE_CDDBP, entry, data) < 0) {
               delete http_string;
               return -1;
           }

           cddb_quit(sock);
           break; }
       case CDDB_MODE_HTTP: {
           if (cddb_read(sock, CDDB_MODE_HTTP, entry, data, http_string) < 0) {
               delete http_string;
               return -1;
           }

           shutdown(sock, 2);
           close(sock);
           break; }
   }

   delete http_string;
   return 0;
}

int CDDB::cddb_process_line(char *line, struct __unprocessed_disc_data *data)
{
    int index = 0;
    char *var, *value;

    line[strlen(line) - 1] = '\0';
    if (strstr(line, "Revision") != NULL) {
        while (line[index] != ':' && line[index] != '\0')
            index++;
        data->data_revision = strtol(line + index + 2, NULL, 10);
        return 0;
    }

    if (strchr(line, '=') == NULL)
        return 0;

    while (line[index] != '=' && line[index] != '\0')
        index++;

    line[index] = '\0';
    var = line;
    value = line + index + 1;

    if (value == NULL)
        value = "";

   if (strcmp(var, "DTITLE") == 0) {
       if (data->data_title_index >= MAX_EXTEMPORANEOUS_LINES)
           return 0;
       strncpy (data->data_title[data->data_title_index++], value, 80);
   } 
   else if (strncmp(var, "TTITLE", 6) == 0) {
       if (data->data_track[strtol((char *)var + 6, NULL, 10)].track_name_index 
           >= MAX_EXTEMPORANEOUS_LINES)
           return 0;
       strncpy(data->data_track[strtol((char *)var + 6, NULL, 10)].track_name[data->data_track[strtol((char *)var + 6, NULL, 10)].track_name_index++], value, 80);
   } 
   else if (strcmp(var, "EXTD") == 0) {
       if (data->data_extended_index >= MAX_EXTENDED_LINES)
           return 0;
       strncpy(data->data_extended[data->data_extended_index++], value, 80);
   } 
   else if(strncmp(var, "EXTT", 4) == 0) {
       if (data->data_track[strtol((char *)var + 4, NULL, 10)].track_extended_index >= MAX_EXTENDED_LINES)
           return 0;
       strncpy(data->data_track[strtol((char *)var + 4, NULL, 10)].track_extended[data->data_track[strtol((char *)var + 4, NULL, 10)].track_extended_index++], value, 80);
   }

   return 0;
}

int CDDB::cddb_generate_unknown_entry(struct disc_data *data)
{
    int index;

    data->data_id = m_discid;
    strcpy(data->data_title, "");
    strcpy(data->data_artist, "");
    data->data_genre = CDDB_UNKNOWN;
    for (index = 0; index < m_total_tracks; index++)
        strcpy(data->data_track[index].track_name, "");
    return 0;
}

int CDDB::cddb_read_disc_data(struct disc_data *outdata)
{
    int index;
    struct __unprocessed_disc_data *data;

    char *tempDir = new char[_MAX_PATH];
    uint32 length = _MAX_PATH;
    m_context->prefs->GetPrefString(kDatabaseDirPref, tempDir, &length);

    string database = string(tempDir) + string(DIR_MARKER_STR) + 
                      string("cddb");
    
    Database *cddb_database = new Database(database.c_str());

    delete [] tempDir;

    data = new struct __unprocessed_disc_data;

    data->data_id = m_discid;
    data->data_title_index = 0;
    data->data_extended_index = 0;
    for (index = 0; index < m_total_tracks; index++) {
        data->data_track[index].track_name_index = 0;
        data->data_track[index].track_extended_index = 0;
    }

    for (index = 0; index < 12; index++) {
        char *key = new char[256];
        snprintf(key, 256, "%s/%08lx", cddb_genre(index), data->data_id);
        if (cddb_database->Exists(key)) {
            char *cddbdatabasedata = cddb_database->Value(key);
            char *starts = cddbdatabasedata, *ends;

            for (;;) {
                if (starts[0] == '\0')
                    break;
                if ((ends = strchr(starts, '\n')) == NULL) 
                    break;
                char *tempbuffer = new char[ends - starts + 2];
                strncpy(tempbuffer, starts, ends - starts + 1);
                tempbuffer[ends - starts + 1] = '\0';
                cddb_process_line(tempbuffer, data);
                starts = ends + 1;
                delete [] tempbuffer;
            }
            delete cddb_database;
            delete [] cddbdatabasedata;
            delete [] key;

            data_format_input(outdata, data);

            delete data;
            return 0;
        }
        delete [] key;
    } 

    delete cddb_database;
    delete data;

    if (cddb_read_data(outdata) < 0) 
        cddb_generate_unknown_entry(outdata);
    cddb_write_data(outdata);

    return 0;
}

int CDDB::cddb_write_data(struct disc_data *indata)
{
   struct disc_info disc = m_discinfo;
   struct __unprocessed_disc_data *data;
   int index, tracks;

   data = new struct __unprocessed_disc_data;
   data_format_output(data, indata);

   char *tempDir = new char[_MAX_PATH];
   uint32 length = _MAX_PATH;
   m_context->prefs->GetPrefString(kDatabaseDirPref, tempDir, &length);

   string database = string(tempDir) + string(DIR_MARKER_STR) +
                     string("cddb");

   Database *cddb_database = new Database(database.c_str());

   delete [] tempDir;

   char *key = new char[256];
   snprintf(key, 256, "%s/%08lx", cddb_genre(data->data_genre), data->data_id);

   string cdata;
   char *temps = new char[1024];
   snprintf(temps, 1024, "# xmcd CD database file generated by %s %s\n", "libcdaudio", "0.99.4");
   cdata.append(temps);
   cdata.append("# \n# Track frame offsets:\n");
   for (index = 0; index < disc.disc_total_tracks; index++) {
       snprintf(temps, 1024, "#       %d\n", (disc.disc_track[index].track_pos.minutes * 60 + disc.disc_track[index].track_pos.seconds) * 75 + disc.disc_track[index].track_pos.frames);
       cdata.append(temps);
   }
   cdata.append("# \n");
   snprintf(temps, 1024, "# Disc length: %d seconds\n", disc.disc_length.minutes * 60 + disc.disc_length.seconds);
   cdata.append(temps);
   cdata.append("# \n");
   snprintf(temps, 1024, "# Revision: %d\n", data->data_revision);
   cdata.append(temps);
   snprintf(temps, 1024, "# Submitted via: %s %s\n", "libcdaudio", "0.99.4");
   cdata.append(temps);
   cdata.append("# \n");
   snprintf(temps, 1024, "DISCID=%08lx\n", data->data_id);
   cdata.append(temps);
   for (index = 0; index < data->data_title_index; index++) {
       snprintf(temps, 1024, "DTITLE=%s\n", data->data_title[index]);
       cdata.append(temps);
   }
   for(tracks = 0; tracks < disc.disc_total_tracks; tracks++) {
      for(index = 0; index < data->data_track[tracks].track_name_index; index++)
      {
        snprintf(temps, 1024, "TTITLE%d=%s\n", tracks, data->data_track[tracks].track_name[index]);
        cdata.append(temps);
      }
   }
   if (data->data_extended_index == 0) {
       cdata.append("EXTD=\n");
   }
   else {
      for (index = 0; index < data->data_extended_index; index++) {
          snprintf(temps, 1024, "EXTD=%s\n", data->data_extended[index]);
          cdata.append(temps);
      }
   }

   for(tracks = 0; tracks < disc.disc_total_tracks; tracks++) {
      if(data->data_track[tracks].track_extended_index == 0) {
        snprintf(temps, 1024, "EXTT%d=\n", tracks);
        cdata.append(temps);
      }
      else {
         for(index = 0; index < data->data_track[tracks].track_extended_index; index++) {
           snprintf(temps, 1024, "EXTT%d=%s\n", tracks, data->data_track[tracks].track_extended[index]);
           cdata.append(temps);
        }
      }
   }

   cdata.append("PLAYORDER=\n");

   cddb_database->Insert(key, (char *)cdata.c_str());

   delete data;
   delete cddb_database;
   delete temps;
   delete key;

   return 0;
}



int CDDB::data_process_control_codes(char *outbuffer, char *inbuffer, int len)
{
    int index;
    int outbufferindex = 0;

    for (index = 0; outbufferindex < len; index++) {
        if (inbuffer[index] == '\\') {
            switch(inbuffer[++index]) {
                case 'n': outbuffer[outbufferindex++] = '\n';
                          break;
                case 't': outbuffer[outbufferindex++] = '\t';
                          break;
                case '\\': outbuffer[outbufferindex++] = '\\';
                           break;
            }
        } 
        else if (inbuffer[index] == '\0') {
            outbuffer[outbufferindex] = '\0';
            return 0;
        } 
        else
            outbuffer[outbufferindex++] = inbuffer[index];
    }
    return 0;
}

int CDDB::data_process_block(char *outbuffer, int len, char (*block)[80], 
                             int bounds)
{
   int index, copyindex;
   int outbufferindex = 0;
   char *procbuffer;
 
   procbuffer = new char[256];
   memset(outbuffer, '\0', len);

   for (index = 0; index < bounds; index++) {
       data_process_control_codes(procbuffer, block[index], 256);
       for (copyindex = 0; copyindex < 256; copyindex++) {
           if (procbuffer[copyindex] == '\0')
               break;
           else {
               outbuffer[outbufferindex++] = procbuffer[copyindex];
               if (outbufferindex >= len) {
                   outbuffer[len - 1] = '\0';
                   delete [] procbuffer;
                   return 0;
               }
           }
       }

       outbuffer[outbufferindex] = '\0';
   }

   delete [] procbuffer;
   return 0;
}

int CDDB::data_format_input(struct disc_data *outdata, 
                            struct __unprocessed_disc_data *indata)
{
   int index;
   char *trackbuffer, *procbuffer;

   trackbuffer = new char[256];
   procbuffer = new char[EXTENDED_DATA_SIZE];

   outdata->data_id = indata->data_id;
   outdata->data_revision = indata->data_revision;
   data_process_block(procbuffer, EXTENDED_DATA_SIZE, indata->data_title,                             indata->data_title_index);
   memset(outdata->data_artist, '\0', 256);
   memset(outdata->data_title, '\0', 256);
   if (strstr(procbuffer, " / ") != NULL) {
       index = 0;
       while (strncmp(procbuffer + index, " / ", 3) != 0)
           index++;
       strncpy(outdata->data_artist, procbuffer, index);
       strncpy(outdata->data_title, (char *)procbuffer + index + 3, 256);
   } 
   else {
      strncpy(outdata->data_artist, "", 256);
      strncpy(outdata->data_title, procbuffer, 256);
   }
   data_process_block(outdata->data_extended, EXTENDED_DATA_SIZE, 
                      indata->data_extended, indata->data_extended_index);
   outdata->data_genre = indata->data_genre;

   for (index = 0; index < m_total_tracks; index++) {
       memset(trackbuffer, '\0', 256);
       data_process_block(trackbuffer, 256, 
                          indata->data_track[index].track_name, 
                          indata->data_track[index].track_name_index);
       if (strchr(trackbuffer, '/') != NULL && parse_track_artist) {
           strtok(trackbuffer, "/");
           strncpy(outdata->data_track[index].track_artist, trackbuffer, 
                   strlen(trackbuffer) - 1);
           strncpy(outdata->data_track[index].track_name, 
                   (char *)strtok(NULL, "/") + 1, 256);
       } 
       else {
           strncpy(outdata->data_track[index].track_artist, "", 256);
           strncpy(outdata->data_track[index].track_name, trackbuffer, 256);
       }

       data_process_block(outdata->data_track[index].track_extended, 
                          EXTENDED_DATA_SIZE, 
                          indata->data_track[index].track_extended, 
                          indata->data_track[index].track_extended_index);
   }

   delete [] trackbuffer;
   delete [] procbuffer;

   return 0;
}

int CDDB::data_format_line(char *outbuffer, char *inbuffer, int len)
{
   int index;
   int outbufferindex = 0;

   for(index = 0; (outbufferindex < len) && (index < 64); index++) {
      switch(inbuffer[index]) {
       case '\t':
         outbuffer[outbufferindex++] = '\\';
         outbuffer[outbufferindex++] = 't';
         break;
       case '\n':
         outbuffer[outbufferindex++] = '\\';
         outbuffer[outbufferindex++] = 'n';
         break;
       case '\0':
         outbuffer[outbufferindex] = '\0';
         return 0;
       default:
         outbuffer[outbufferindex++] = inbuffer[index];
         break;
      }
   }

   outbuffer[outbufferindex] = '\0';
   return 0;
}

int CDDB::data_format_block(char (*block)[80], int arraylen, char *inbuffer)
{
   int index;
   char *procbuffer;

   procbuffer = new char[80];

   for(index = 0; index < arraylen; index++) {
      strncpy(procbuffer, inbuffer, 64);
      data_format_line(block[index], procbuffer, 80);
      inbuffer += 64;
   }

   delete [] procbuffer;
   return 0;
}

int CDDB::data_format_output(struct __unprocessed_disc_data *outdata, 
                             struct disc_data *indata)
{
   int index;
   char *trackbuffer, *procbuffer;

   trackbuffer = new char[256];
   procbuffer = new char[EXTENDED_DATA_SIZE];

   outdata->data_id = indata->data_id;
   outdata->data_revision = indata->data_revision;
   outdata->data_genre = indata->data_genre;

   memset(procbuffer, '\0', EXTENDED_DATA_SIZE);
   if(strlen(indata->data_artist))
     snprintf(procbuffer, EXTENDED_DATA_SIZE, "%s / %s", indata->data_artist, indata->data_title);
   else
     strncpy(procbuffer, indata->data_title, EXTENDED_DATA_SIZE);
   data_format_block(outdata->data_title, MAX_EXTEMPORANEOUS_LINES, procbuffer);
   for(outdata->data_title_index = 0; outdata->data_title_index < MAX_EXTEMPORANEOUS_LINES; outdata->data_title_index++) {
      if(strlen(outdata->data_title[outdata->data_title_index]) <= 0)
        break;
   }

   data_format_block(outdata->data_extended, MAX_EXTENDED_LINES, indata->data_extended);
   for(outdata->data_extended_index = 0; outdata->data_extended_index < MAX_EXTENDED_LINES; outdata->data_extended_index++) {
      if(strlen(outdata->data_extended[outdata->data_extended_index]) <= 0)
        break;
   }

   for(index = 0; index < m_total_tracks; index++) {
      memset(trackbuffer, '\0', 256);
      if(strlen(indata->data_track[index].track_artist) > 0)
        snprintf(trackbuffer, 256, "%s / %s", indata->data_track[index].track_artist, indata->data_track[index].track_name);
      else
        strncpy(trackbuffer, indata->data_track[index].track_name, 256);
      data_format_block(outdata->data_track[index].track_name, MAX_EXTEMPORANEOUS_LINES, trackbuffer);
      for(outdata->data_track[index].track_name_index = 0; outdata->data_track[index].track_name_index < MAX_EXTEMPORANEOUS_LINES; outdata->data_track[index].track_name_index++) {
         if(strlen(outdata->data_track[index].track_name[outdata->data_track[index].track_name_index]) <= 0)
           break;
      }

      data_format_block(outdata->data_track[index].track_extended, MAX_EXTENDED_LINES, indata->data_track[index].track_extended);
      for(outdata->data_track[index].track_extended_index = 0; outdata->data_track[index].track_extended_index < MAX_EXTENDED_LINES; outdata->data_track[index].track_extended_index++) {
         if(strlen(outdata->data_track[index].track_extended[outdata->data_track[index].track_extended_index]) <= 0)
           break;
      }
   }

   delete [] trackbuffer;
   delete [] procbuffer;

   return 0;
}

