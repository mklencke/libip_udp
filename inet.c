#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

unsigned short inet_checksum( void *data, int maybe_size )
{
	return 0;
}

