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

u16_t inet_checksum(void *buf, int buflen)
{
	u32_t sum=0;
	u16_t *u16_buf = (u16_t *)buf;
	int u16_buflen = buflen >> 1;

	if (buflen % 2)
	{
		sum += ntohs(htons(u16_buf[u16_buflen])&0xff00);
	}

	while (u16_buflen--)
	{
		sum += u16_buf[u16_buflen];
		if (sum >= 0x00010000)
			sum -= 0x0000ffff;
	}

	return ~sum;
}

