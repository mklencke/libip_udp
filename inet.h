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

#ifndef __INET_H
#define __INET_H

#include "compat.h"

#define IP_PROTO_UDP 17
#define IP_PROTO_TCP 6

typedef unsigned long ipaddr_t;

#ifndef __DONT_WRAP_FUNCTIONS
#define inet_aton fake_inet_aton
#define inet_ntoa fake_inet_ntoa
#endif

ipaddr_t fake_inet_aton( const char *cp );
char *fake_inet_ntoa( ipaddr_t addr );

unsigned short inet_checksum( void *, int );

/*
 * Some big endian platforms define htons(x) and ntohs(x) as (x)
 * But they still have these functions in the standard library
 *
 */

#ifndef htons
u16_t htons( u16_t );
#endif

#ifndef ntohs
u16_t ntohs( u16_t );
#endif

#ifndef htonl
u32_t htonl( u32_t );
#endif

#ifndef ntohl
u32_t ntohl( u32_t );
#endif

#endif

