/*
 * libip_udp -- Computer Networks Practicum IP Layer Emulation
 *
 * Copyright (C) 2004 Marten Klencke & Erik Bosman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define __DONT_WRAP_FUNCTIONS
#include "inet.h"

ipaddr_t fake_inet_aton( const char *cp )
{
	struct in_addr addrstruct;
	inet_aton(cp, &addrstruct);
	return (ipaddr_t)addrstruct.s_addr;
}

char *fake_inet_ntoa( ipaddr_t addr )
{
	struct in_addr addrstruct;
	addrstruct.s_addr = (unsigned long int)addr;
	return inet_ntoa(addrstruct);
}

u16_t inet_checksum(void *buf, int buflen)
{
	u32_t sum=0;
	u16_t u16_end=0;
	u16_t *u16_buf = (u16_t *)buf;
	int u16_buflen = buflen >> 1;

	if (buflen % 2)
	{
		*((u8_t *)&u16_end) = ((u8_t *)buf)[buflen-1];
		sum += u16_end;
	}

	while (u16_buflen--)
	{
		sum += u16_buf[u16_buflen];
		if (sum >= 0x00010000)
			sum -= 0x0000ffff;
	}

	return ~sum;
}

/*
 * Enforce System V behaviour of the signal
 * This implementation is just a quick hack.
 * The GNU libc provides a sysv_signal call,
 * but this is not available on OSX (and BSD?)
 */
sighandler_t fake_signal(int signum, sighandler_t handler)
{
	siginterrupt(signum, handler?1:0);
	return signal(signum, handler);
}

