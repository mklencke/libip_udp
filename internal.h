#ifndef __LIBIP_UDP_INTERNAL_H
#define __LIBIP_UDP_INTERNAL_H

/* Used to detect if real functions need to be wrapped */
#define __DONT_WRAP_FUNCTIONS

#include "inet.h"

/* Define base port as my birthday :-) */
#define BASE_PORT 2809

#define MAX_PORTS 20

#define LOCALHOST "127.0.0.1"

ipaddr_t my_ipaddr  = 0;
unsigned int my_port;

#endif

