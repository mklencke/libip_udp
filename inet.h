#ifndef __INET_H
#define __INET_H

#include "compat.h"

#define IP_PROTO_UDP 17

typedef unsigned long ipaddr_t;

#ifndef __INTERNAL
#define inet_aton fake_inet_aton
#endif

ipaddr_t fake_inet_aton( const char *cp );

unsigned short inet_checksum(void *, int);

#endif

