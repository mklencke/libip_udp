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

#ifndef __IP_H
#define __IP_H

#include "compat.h"
#include "inet.h"

typedef struct {
	char blob[20];
} iphdr_t;

int ip_init();

int ip_send( ipaddr_t dst, unsigned short proto, unsigned short id, void *data,
             int len );

int ip_receive( ipaddr_t *srcp, ipaddr_t *dstp, unsigned short *protop,
                unsigned short *idp, char **data );

extern ipaddr_t my_ipaddr;

#endif

