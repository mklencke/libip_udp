#include <stdlib.h>
#include <stdio.h>

#include "inet.h"

#include <netinet/in.h>

static char ascii_ipaddr_buf[16];

ipaddr_t fake_inet_aton( const char *cp )
{
	return atoi( cp );
}

char *fake_inet_ntoa( ipaddr_t addr )
{
	snprintf(ascii_ipaddr_buf, 16, "0.0.0.%d", ntohl(addr));
	return ascii_ipaddr_buf;
}

unsigned short inet_checksum( void *data, int maybe_size )
{
	return 0;
}

