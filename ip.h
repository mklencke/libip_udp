#ifndef __IP_H
#define __IP_H

#include "compat.h"
#include "inet.h"

int ip_init();

int ip_send( ipaddr_t dst, unsigned short proto, unsigned short id, void *data,
             int len );

int ip_receive( ipaddr_t *srcp, ipaddr_t *dstp, unsigned short *protop,
                unsigned short *idp, char **data );

extern ipaddr_t my_ipaddr;

#endif

