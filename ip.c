/*
 * libip_udp -- Computer Networks Practicum IP Layer Emulation
 *
 * Copyright (C) 2004 Marten Klencke & Erik Bosman
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

typedef struct {
	unsigned char protocol;
	unsigned char checksum;
	ipaddr_t      source;
	ipaddr_t      destination;
} not_quite_ip_header_t;


static int sending_socket;
static int listening_socket;

static int log_packets;

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
	my_port = BASE_PORT + atoi( eth ) - 1;
	if ( my_port < BASE_PORT ) {
		printf( "Invalid value for ETH.\n" );
		exit( EXIT_FAILURE );
	}

	if ( my_port >= BASE_PORT + MAX_PORTS ) {
		printf( "ETH value to high. Max is %d.\n", MAX_PORTS );
		exit( EXIT_FAILURE );
	}

	my_ipaddr = htonl( 192<<24 | 168<<16 | atoi( eth ) );
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
	int result;

	listening_socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( listening_socket < 0 ) {
		printf( "Could not create listening UDP socket.\n" );
		exit( EXIT_FAILURE );
	}
	
	bzero( &sin, sizeof( sin ) );
	sin.sin_family = AF_INET;
	sin.sin_port = htons( my_port );
	sin.sin_addr.s_addr = INADDR_ANY;

	result = bind( listening_socket, (struct sockaddr *)&sin, sizeof( sin ) );
	if ( result < 0 ) {
		printf( "Could not bind to virtual IP %s (UDP port %d).\n",
		        fake_inet_ntoa(my_ipaddr), my_port );
		exit( EXIT_FAILURE );
	}
}


static int send_udp_packet( ipaddr_t dst, void *data, int len )
{
	struct sockaddr_in to;
	int port, result;

	port = BASE_PORT + ( ntohl(dst) & 0xffff ) - 1;

	bzero( &to, sizeof( to ) );
	to.sin_family = AF_INET;
	inet_aton( LOCALHOST, &to.sin_addr );

	/* Must log first to avoid wrong order with logged packets */
	if ( log_packets ) {
		to.sin_port = htons( LOG_PORT );

		sendto( sending_socket, data, len, 0, (struct sockaddr *)&to,
		        sizeof( to ) );
	}

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


static void check_packet_logging()
{
	char *log;

	log = getenv( "LOG_PACKETS" );
	if ( log && ( strcmp( log, "1" ) == 0 ) )
		log_packets = 1;
	else
		log_packets = 0;
}


int ip_init()
{
	set_my_ipaddr();
	create_sending_socket();
	create_listening_socket();
	check_packet_logging();

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
	
	not_quite_ip_header_t header;
	header.protocol = proto;
	header.checksum = 0; /* TODO */
	header.source = my_ipaddr;
	header.destination = dst;

	header_size = sizeof( not_quite_ip_header_t );
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
	char buf[UDP_RECEIVE_BUFFER_SIZE];
	not_quite_ip_header_t header;
	int headerlen, result;

	if ( my_ipaddr == 0 ) {
		ip_init();
	}

	headerlen = sizeof( not_quite_ip_header_t );

	do {
		header.destination = 0;

		result = receive_udp_packet( buf, UDP_RECEIVE_BUFFER_SIZE );
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

