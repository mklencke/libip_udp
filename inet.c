#include <stdlib.h>

#include "inet.h"

ipaddr_t fake_inet_aton( const char *cp )
{
	return atoi( cp );
}

unsigned short inet_checksum( void *data, int maybe_size )
{
	return 0;
}

