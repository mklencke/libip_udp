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


/*** inet_checksum() *********************
 *
 * Copied from inet.c as part of libcn,
 * used for the computer networks course at the VU
 * http://www.cs.vu.nl/~cn/
 *
 * Written by Leendert van Doorn
 */

/*
 * 16-bit one complement check sum
 */
unsigned short
inet_checksum(void *dp, int counter)
{
    register unsigned short *sp;
    unsigned long sum, oneword = 0x00010000;
    register int count;

    count = counter >> 1;
    for (sum = 0, sp = (unsigned short *)dp; count--; ) {
	sum += *sp++;
	if (sum >= oneword) { /* wrap carry into low bit */
	    sum -= oneword;
	    sum++;
	}
    }
    if (counter & 1) {
#ifdef LITTLE_ENDIAN
	sum += ((unsigned short) *((char *) sp)) &  0x00ff; 
#else
	sum += ((unsigned short) *((char *) sp)) << 8;
#endif
	if (sum >= oneword) { /* wrap carry into low bit */
	    sum -= oneword;
	    sum++;
	}
    }
    return ~sum;
}

