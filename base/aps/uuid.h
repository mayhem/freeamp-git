/*_______________________________________________________________________        
        FreeAmp - The Free MP3 Player

        Portions Copyright (C) 2000 Relatable

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, Write to the Free Software
        Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
        
        $Id$
____________________________________________________________________________*/

#ifndef _UUID_H_
#define _UUID_H_

#include <iostream>
#include <asm/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <linux/ext2_fs.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>

using namespace std;

class UUID
{
public:
  typedef unsigned char uuid_t[16];

  struct uuid {
	__u32	time_low;
	__u16	time_mid;
	__u16	time_hi_and_version;
	__u16	clock_seq;
	__u8	node[6];
  };


  void uuid_clear(uuid_t uu);
  int uuid_compare(uuid_t uu1, uuid_t uu2);
  void uuid_copy(uuid_t uu1, uuid_t uu2);
  int get_clock(__u32 *clock_high, __u32 *clock_low, __u16 *ret_clock_seq);
  void get_random_bytes(void *buf, int nbytes);
  int get_node_id(unsigned char *node_id);
  void uuid_generate(uuid_t out);
  void uuid_generate_random(uuid_t out);
  void uuid_generate_time(uuid_t out);
  int uuid_is_null(uuid_t uu);
  int uuid_parse(char *in, uuid_t uu);
  void uuid_unparse(uuid_t uu, char *out);
  time_t uuid_time(uuid_t uu, struct timeval *ret_tv);
  int uuid_type(uuid_t uu);
  int uuid_variant(uuid_t uu);
  void uuid_pack(struct uuid *uu, uuid_t ptr);
  void uuid_unpack(uuid_t in, struct uuid *uu);
  void uuid_ascii(uuid_t in, char ascii[37]);
  
private:
  const static int UUID_VARIANT_NCS = 0;
  const static int UUID_VARIANT_DCE = 1;
  const static int UUID_VARIANT_MICROSOFT = 2;
  const static int UUID_VARIANT_OTHER = 3;
  
  // Assume that the gettimeofday() has microsecond granularity
  const static int MAX_ADJUSTMENT = 10;
  

  // Offset between 15-Oct-1582 and 1-Jan-70

  const static long int TIME_OFFSET_HIGH = 0x01B21DD2;
  const static long int TIME_OFFSET_LOW = 0x13814000;

};

#endif


