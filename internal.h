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

#ifndef __LIBIP_UDP_INTERNAL_H
#define __LIBIP_UDP_INTERNAL_H

/* Used to detect if real functions need to be wrapped */
#define __DONT_WRAP_FUNCTIONS

#include "inet.h"

/* Define base port as my birthday :-) */
#define BASE_PORT 2809

/* Define logging port as Erik's birthday :-) */
#define LOG_PORT 1211

#define MAX_PORTS 20

/* More than 8192 just to be safe */
#define UDP_RECEIVE_BUFFER_SIZE 9216

#define LOCALHOST "127.0.0.1"

#define DROP_PACKET 1
#define CORRUPT_PACKET 2

ipaddr_t my_ipaddr  = 0;
unsigned int my_port;

typedef struct {
	unsigned char protocol;
	unsigned char flags;
	ipaddr_t      source;
	ipaddr_t      destination;
} not_quite_ip_header_t;


#endif

