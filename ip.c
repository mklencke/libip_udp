/*
 * Computer Networks Practicum IP Layer Emulation
 *
 * This library emulates the CN IP Layer by sending fake IP packets
 * through UDP. This eliminates Minix during development time and thus
 * makes the whole process much easier.
 *
 * It is being developed on Slackware Linux 10.0.
 *
 * Copyright (C) 2004 Marten Klencke
 * This code is licensed under the GNU LGPL
 *
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "internal.h"
#include "ip.h"

static int sending_socket;
static int listening_socket;


static void set_my_ipaddr()
{
	char *eth;
	/* ETH environment variable must be set so we can
	 * choose a fake IP.
	 */
	eth = getenv( "ETH" );
	if ( eth == NULL ) {
		printf( "ETH must be set.\n" );
		exit( EXIT_FAILURE );
	}
	
	my_ipaddr = atoi( eth );
	if ( my_ipaddr <= 0 ) {
		printf( "Invalid value for ETH.\n" );
		exit( EXIT_FAILURE );
	}

	if ( my_ipaddr > MAX_PORTS ) {
		printf( "ETH value to high. Max is %d.\n", MAX_PORTS );
		exit( EXIT_FAILURE );
	}
}


static void create_sending_socket()
{
	sending_socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( sending_socket < 0 ) {
		printf( "Could not create sending UDP socket.\n" );
		exit( EXIT_FAILURE );
	}
}


static void create_listening_socket()
{
	struct sockaddr_in sin;
	int result, port;

	listening_socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( listening_socket < 0 ) {
		printf( "Could not create listening UDP socket.\n" );
		exit( EXIT_FAILURE );
	}

	port = BASE_PORT + my_ipaddr - 1;
	
	bzero( &sin, sizeof( sin ) );
	sin.sin_family = AF_INET;
	sin.sin_port = htons( port );
	sin.sin_addr.s_addr = INADDR_ANY;

	result = bind( listening_socket, (struct sockaddr *)&sin, sizeof( sin ) );
	if ( result < 0 ) {
		printf( "Could not bind to virtual IP %d (UDP port %d).\n",
		        (int)my_ipaddr, port );
		exit( EXIT_FAILURE );
	}
}


static int send_udp_packet( ipaddr_t dst, void *data, int len )
{
	struct sockaddr_in to;
	int port, result;

	port = BASE_PORT + dst - 1;

	bzero( &to, sizeof( to ) );
	to.sin_family = AF_INET;
	inet_aton( LOCALHOST, &to.sin_addr );
	to.sin_port = htons( port );

	result = sendto( sending_socket, data, len, 0, (struct sockaddr *)&to,
	                 sizeof( to ) );
	return result;
}


static int receive_udp_packet( void *buf, int maxlen )
{
	struct sockaddr from;
	int fromlen, result;

	fromlen = sizeof( from );

	result = recvfrom( listening_socket, buf, maxlen, 0, &from, &fromlen );

	if ( result == -1 ) {
		if ( errno != EINTR ) {
			printf( "recvfrom() returned an error which was not EINTR but %d\n", errno );
			exit( EXIT_FAILURE );
		}
	}
	
	return result;
}


int ip_init()
{
	set_my_ipaddr();
	create_sending_socket();
	create_listening_socket();

	return 0;
}

int ip_send( ipaddr_t dst, unsigned short proto, unsigned short id, void *data,
             int len )
{
	char *buf;
	size_t header_size;
	int result;

	if ( my_ipaddr == 0 ) {
		ip_init();
	}
	
	ip_header_t header;
	header.protocol = proto;
	header.checksum = 0; /* TODO */
	header.source = my_ipaddr;
	header.destination = dst;

	header_size = sizeof( ip_header_t );
	buf = malloc( header_size + len );

	memcpy( buf, &header, header_size );
	memcpy( buf + header_size, data, len );

	result = send_udp_packet( dst, buf, header_size + len );

	free( buf );

	if ( result < 0 )
		return result;
	else {
		if ( result < header_size )
			return -1;
		else
			return ( result - header_size );
	}
}

int ip_receive( ipaddr_t *srcp, ipaddr_t *dstp, unsigned short *protop,
                unsigned short *idp, char **data )
{
	char buf[8192]; /* FIXME maxlen */
	ip_header_t header;
	int headerlen, result;

	if ( my_ipaddr == 0 ) {
		ip_init();
	}

	headerlen = sizeof( ip_header_t );

	do {
		header.destination = 0;

		result = receive_udp_packet( buf, 8192 ); /* FIXME maxlen */
		if ( result < 0 )
			return result;
		if ( result < headerlen )
			continue;
		
		memcpy( &header, buf, headerlen );
	} while ( header.destination != my_ipaddr );
		
	*srcp = header.source;
	*dstp = header.destination;
	*protop = header.protocol;
	*data = malloc( result );
	memcpy( *data, ( buf + headerlen ), ( result - headerlen ) );
	
	return ( result - headerlen );
}

