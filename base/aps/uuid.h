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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uuid_t[17];

typedef unsigned int __u32;
typedef unsigned short __u16;
typedef unsigned char __u8;

struct uuid {
   __u32 time_low;
   __u16 time_mid;
   __u16 time_hi_and_version;
   __u16 clock_seq;
   __u8  node[6];
};

void uuid_clear(uuid_t uu);
int uuid_parse(char *in, uuid_t uu);
void uuid_pack(struct uuid *uu, uuid_t ptr);
void uuid_unpack(uuid_t in, struct uuid *uu);
void uuid_ascii(uuid_t in, char ascii[37]);
  
#endif


