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

#include "uuid.h"

void UUID::uuid_clear(uuid_t uu)
{
  memset(uu, 0, 16);
}

int UUID::uuid_compare(uuid_t uu1, uuid_t uu2)
{
  #define UUCMP(u1,u2) if (u1 != u2) return((u1 < u2) ? -1 : 1);

  struct uuid	uuid1, uuid2;

  uuid_unpack(uu1, &uuid1);
  uuid_unpack(uu2, &uuid2);

  UUCMP(uuid1.time_low, uuid2.time_low);
  UUCMP(uuid1.time_mid, uuid2.time_mid);
  UUCMP(uuid1.time_hi_and_version, uuid2.time_hi_and_version);
  UUCMP(uuid1.clock_seq, uuid2.clock_seq);
  return memcmp(uuid1.node, uuid2.node, 6);
}

void UUID::uuid_copy(uuid_t uu1, uuid_t uu2)
{
  unsigned char 	*cp1, *cp2;
  int		i;

  for (i=0, cp1 = uu1, cp2 = uu2; i < 16; i++)
    *cp1++ = *cp2++;
}

int UUID::get_clock(__u32 *clock_high, __u32 *clock_low, __u16 *ret_clock_seq)
{
	static int			adjustment = 0;
	static struct timeval		last = {0, 0};
	static __u16			clock_seq;
	struct timeval 			tv;
	unsigned long long		clock_reg;
	
try_again:
	gettimeofday(&tv, 0);
	if ((last.tv_sec == 0) && (last.tv_usec == 0)) {
		get_random_bytes(&clock_seq, sizeof(clock_seq));
		clock_seq &= 0x1FFF;
		last = tv;
		last.tv_sec--;
	}
	if ((tv.tv_sec < last.tv_sec) ||
	    ((tv.tv_sec == last.tv_sec) &&
	     (tv.tv_usec < last.tv_usec))) {
		clock_seq = (clock_seq+1) & 0x1FFF;
		adjustment = 0;
	} else if ((tv.tv_sec == last.tv_sec) &&
	    (tv.tv_usec == last.tv_usec)) {
		if (adjustment >= MAX_ADJUSTMENT)
			goto try_again;
		adjustment++;
	} else
		adjustment = 0;
	
	clock_reg = tv.tv_usec*10 + adjustment;
	clock_reg += ((unsigned long long) tv.tv_sec)*10000000;
	clock_reg += (((unsigned long long) 0x01B21DD2) << 32) + 0x13814000;

	*clock_high = clock_reg >> 32;
	*clock_low = clock_reg;
	*ret_clock_seq = clock_seq;
	return 0;
}

void UUID::get_random_bytes(void *buf, int nbytes)
{
	static int fd = -2;
	int i;
	char *cp = (char *) buf;

	if (fd == -2) {
		fd = open("/dev/urandom", O_RDONLY);
		srand((getpid() << 16) ^ getuid() ^ time(0));
	}
	
	if (fd >= 0) {
		while (nbytes > 0) {
			i = read(fd, cp, nbytes);
			if (i < 0) {
				if ((errno == EINTR) || (errno == EAGAIN))
					continue;
				break;
			}
			nbytes -= i;
			cp += i;
		}
	}
	if (nbytes == 0)
		return;

	/* XXX put something better here if no /dev/random! */
	for (i=0; i < nbytes; i++)
		*cp++ = rand() & 0xFF;
	return;

}

int UUID::get_node_id(unsigned char *node_id)
{
#ifdef HAVE_NET_IF_H
	int 		sd;
	struct ifreq 	ifr, *ifrp;
	struct ifconf 	ifc;
	char buf[1024];
	int		n, i;
	unsigned char 	*a;
	
/*
 * BSD 4.4 defines the size of an ifreq to be
 * max(sizeof(ifreq), sizeof(ifreq.ifr_name)+ifreq.ifr_addr.sa_len
 * However, under earlier systems, sa_len isn't present, so the size is 
 * just sizeof(struct ifreq)
 */
#ifdef HAVE_SA_LEN
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#define ifreq_size(i) max(sizeof(struct ifreq),\
     sizeof((i).ifr_name)+(i).ifr_addr.sa_len)
#else
#define ifreq_size(i) sizeof(struct ifreq)
#endif /* HAVE_SA_LEN*/

	sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sd < 0) {
		return -1;
	}
	memset(buf, 0, sizeof(buf));
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl (sd, SIOCGIFCONF, (char *)&ifc) < 0) {
		close(sd);
		return -1;
	}
	n = ifc.ifc_len;
	for (i = 0; i < n; i+= ifreq_size(*ifr) ) {
		ifrp = (struct ifreq *)((char *) ifc.ifc_buf+i);
		strncpy(ifr.ifr_name, ifrp->ifr_name, IFNAMSIZ);
#ifdef SIOCGIFHWADDR
		if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)
			continue;
		a = (unsigned char *) &ifr.ifr_hwaddr.sa_data;
#else
#ifdef SIOCGENADDR
		if (ioctl(sd, SIOCGENADDR, &ifr) < 0)
			continue;
		a = (unsigned char *) ifr.ifr_enaddr;
#else
		/*
		 * XXX we don't have a way of getting the hardware
		 * address
		 */
		close(sd);
		return 0;
#endif /* SIOCGENADDR */
#endif /* SIOCGIFHWADDR */
		if (!a[0] && !a[1] && !a[2] && !a[3] && !a[4] && !a[5])
			continue;
		if (node_id) {
			memcpy(node_id, a, 6);
			close(sd);
			return 1;
		}
	}
	close(sd);
#endif
	return 0;

}

void UUID::uuid_generate(uuid_t out)
{
  static int	has_random = -1;

  if (has_random < 0) 
  {
    if (access("/dev/urandom", R_OK) == 0)
      has_random = 1;
    else
      has_random = 0;
  }

  if (has_random)
    uuid_generate_random(out);
  else
    uuid_generate_time(out);

}

void UUID::uuid_generate_random(uuid_t out)
{
  uuid_t	buf;
  struct uuid uu;

  get_random_bytes(buf, sizeof(buf));
  uuid_unpack(buf, &uu);

  uu.clock_seq = (uu.clock_seq & 0x3FFF) | 0x8000;
  uu.time_hi_and_version = (uu.time_hi_and_version & 0x0FFF) | 0x4000;
  uuid_pack(&uu, out);
}

void UUID::uuid_generate_time(uuid_t out)
{
  static unsigned char node_id[6];
  static int has_init = 0;
  struct uuid uu;
  __u32	clock_mid;

  if (!has_init) 
  {
    if (get_node_id(node_id) <= 0) 
    {
      get_random_bytes(node_id, 6);

      // Set multicast bit, to prevent conflicts
      // with IEEE 802 addresses obtained from
      // network cards

      node_id[0] |= 0x80;
    }
    has_init = 1;
  }

  get_clock(&clock_mid, &uu.time_low, &uu.clock_seq);
  uu.clock_seq |= 0x8000;
  uu.time_mid = (__u16) clock_mid;
  uu.time_hi_and_version = (clock_mid >> 16) | 0x1000;
  memcpy(uu.node, node_id, 6);
  uuid_pack(&uu, out);
}

int UUID::uuid_is_null(uuid_t uu)
{
// Returns 1 if the uuid is the NULL uuid
	unsigned char 	*cp;
	int		i;

	for (i=0, cp = uu; i < 16; i++)
		if (*cp++)
			return 0;
	return 1;

}

int UUID::uuid_parse(char *in, uuid_t uu)
{
	struct uuid uuid;
	int i;
	char *cp, buf[3];

	if (strlen(in) != 36)
		return -1;
	for (i=0, cp = in; i <= 36; i++,cp++) {
		if ((i == 8) || (i == 13) || (i == 18) ||
		    (i == 23))
			if (*cp == '-')
				continue;
		if (i== 36)
			if (*cp == 0)
				continue;
		if (!isxdigit(*cp))
			return -1;
	}
	uuid.time_low = strtoul(in, NULL, 16);
	uuid.time_mid = strtoul(in+9, NULL, 16);
	uuid.time_hi_and_version = strtoul(in+14, NULL, 16);
	uuid.clock_seq = strtoul(in+19, NULL, 16);
	cp = in+24;
	buf[2] = 0;
	for (i=0; i < 6; i++) {
		buf[0] = *cp++;
		buf[1] = *cp++;
		uuid.node[i] = strtoul(buf, NULL, 16);
	}
	
	uuid_pack(&uuid, uu);
	return 0;

}

void UUID::uuid_unparse(uuid_t uu, char *out)
{
	struct uuid uuid;

	uuid_unpack(uu, &uuid);
	sprintf(out,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
		uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
		uuid.node[0], uuid.node[1], uuid.node[2],
		uuid.node[3], uuid.node[4], uuid.node[5]);

}

time_t UUID::uuid_time(uuid_t uu, struct timeval *ret_tv)
{
	struct uuid		uuid;
	__u32			high;
	struct timeval		tv;
	unsigned long long	clock_reg;

	uuid_unpack(uu, &uuid);
	
	high = uuid.time_mid | ((uuid.time_hi_and_version & 0xFFF) << 16);
	clock_reg = uuid.time_low | ((unsigned long long) high << 32);

	clock_reg -= (((unsigned long long) 0x01B21DD2) << 32) + 0x13814000;
	tv.tv_sec = clock_reg / 10000000;
	tv.tv_usec = (clock_reg % 10000000) / 10;

	if (ret_tv)
		*ret_tv = tv;

	return tv.tv_sec;

}

int UUID::uuid_type(uuid_t uu)
{
	struct uuid		uuid;

	uuid_unpack(uu, &uuid);	
	return ((uuid.time_hi_and_version >> 12) & 0xF);

}

int UUID::uuid_variant(uuid_t uu)
{
	struct uuid		uuid;
	int			var;

	uuid_unpack(uu, &uuid);	
	var = uuid.clock_seq;

	if ((var & 0x8000) == 0)
		return UUID_VARIANT_NCS;
	if ((var & 0x4000) == 0)
		return UUID_VARIANT_DCE;
	if ((var & 0x2000) == 0)
		return UUID_VARIANT_MICROSOFT;
	return UUID_VARIANT_OTHER;

}

void UUID::uuid_pack(struct uuid *uu, uuid_t ptr)
{
	__u32	tmp;
	unsigned char	*out = ptr;

	tmp = uu->time_low;
	out[3] = (unsigned char) tmp;
	tmp >>= 8;
	out[2] = (unsigned char) tmp;
	tmp >>= 8;
	out[1] = (unsigned char) tmp;
	tmp >>= 8;
	out[0] = (unsigned char) tmp;
	
	tmp = uu->time_mid;
	out[5] = (unsigned char) tmp;
	tmp >>= 8;
	out[4] = (unsigned char) tmp;

	tmp = uu->time_hi_and_version;
	out[7] = (unsigned char) tmp;
	tmp >>= 8;
	out[6] = (unsigned char) tmp;

	tmp = uu->clock_seq;
	out[9] = (unsigned char) tmp;
	tmp >>= 8;
	out[8] = (unsigned char) tmp;

	memcpy(out+10, uu->node, 6);

}
void UUID::uuid_unpack(uuid_t in, struct uuid *uu)
{
  __u8	*ptr = in;
  __u32	tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  tmp = (tmp << 8) | *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->time_low = tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->time_mid = tmp;
	
  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->time_hi_and_version = tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->clock_seq = tmp;

  memcpy(uu->node, ptr, 6);
}

void UUID::uuid_ascii(uuid_t in, char ascii[37])
{
    uuid foo;
    uuid_unpack(in, &foo);

    sprintf(ascii,
            "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            foo.time_low, foo.time_mid, foo.time_hi_and_version,
            foo.clock_seq >> 8, foo.clock_seq & 0xFF,
            foo.node[0], foo.node[1], foo.node[2],
            foo.node[3], foo.node[4], foo.node[5]);
}
