#ifndef __INET_H
#define __INET_H

#include "compat.h"

#define IP_PROTO_UDP 17

typedef unsigned long ipaddr_t;

#ifndef __DONT_WRAP_FUNCTIONS
#define inet_aton fake_inet_aton
#define inet_ntoa fake_inet_ntoa
#endif

ipaddr_t fake_inet_aton( const char *cp );
char *fake_inet_ntoa(ipaddr_t addr);

unsigned short inet_checksum(void *, int);

#endif

