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

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>

#include "internal.h"
#include "simplelog.h"

static int listening_socket;


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
	sin.sin_port = htons( LOG_PORT );
	sin.sin_addr.s_addr = INADDR_ANY;

	result = bind( listening_socket, (struct sockaddr *)&sin, sizeof( sin ) );
	if ( result < 0 ) {
		printf( "Could not bind to logging UDP port %d.\n", LOG_PORT );
		exit( EXIT_FAILURE );
	}
}


static void print_time()
{
	char buf[16];
	time_t current_time;
	struct tm *tm_ptr;

	time( &current_time );
	tm_ptr = localtime( &current_time );
	strftime( buf, 16, "%H:%M:%S", tm_ptr );
	printf( "%s ", buf );
}


static void log_tcp_packet( char *buf, int len )
{
	/* This contains some pointer arithmetic because we probably are not
	 * allowed to publish a tcp_header_t structure */
	unsigned int ack = 0, seq;

	if ( len < TCP_HEADER_SIZE ) {
		printf( "            (packet too small to be a TCP packet)" );
		return;
	}

	seq = ntohl( *(u32_t*)(buf+4) );
	ack = ntohl( *(u32_t*)(buf+8) );

	printf( "            [Seq: %10u] [Ack: %10u]", seq, ack );
			
	printf( " [Flags:" );
	if ( (u8_t)*(buf+13) & 0x08 )
		printf( " PSH" );
	if ( (u8_t)*(buf+13) & 0x02 )
		printf( " SYN" );
	if ( (u8_t)*(buf+13) & 0x10 )
		printf( " ACK" );
	if ( (u8_t)*(buf+13) & 0x01 )
		printf( " FIN" );
	printf( "]" );

	if ( len > TCP_HEADER_SIZE )
		printf( " + DATA (%u)", ( len - TCP_HEADER_SIZE ) );

	printf( "\n" );
	
}


static void log_packet( char *buf, int len )
{
	not_quite_ip_header_t *header;
	char *source, *destination;

	if ( len < sizeof( not_quite_ip_header_t ) )
	{
		print_time();
		printf( "[Tiny packet (<IP header) received]\n\n" );
		return;
	}

	header = (not_quite_ip_header_t *)buf;
	source = (char *)strdup( fake_inet_ntoa( header->source ) );
	destination = (char *)strdup( fake_inet_ntoa( header->destination ) );

	if ( header->flags & DROP_PACKET ) printf("%c[1;30;40m", 27);
	else if ( header->flags & CORRUPT_PACKET ) printf("%c[1;31;40m", 27);

	print_time();

	switch ( header->protocol ) {
		case IP_PROTO_UDP:
			printf( "[UDP Packet from %s to %s]\n", source, destination );
			break;
		case IP_PROTO_TCP:
			printf( "[TCP Packet from %s to %s]\n", source, destination);
			log_tcp_packet( buf + sizeof( not_quite_ip_header_t ),
			                len - sizeof( not_quite_ip_header_t ) );
			break;
		default:
			printf( "[Unidentified IP Packet from %s to %s (protocol: %d)]\n",
					source, destination,
			        ntohs( header->protocol ) );
	}

	if ( header->flags ) printf("%c[0m", 27);
	printf( "\n" );

	free( source );
	free( destination );

	fflush( stdout );
}


static void receive_and_log_packet()
{
	char buf[UDP_RECEIVE_BUFFER_SIZE];
	struct sockaddr from;
	socklen_t fromlen;
	ssize_t len;

	fromlen = sizeof( from );

	len = recvfrom( listening_socket, buf, UDP_RECEIVE_BUFFER_SIZE, 0, &from, &fromlen );
	
	if ( len != -1 )
		log_packet( buf, len );
}


int main()
{
	create_listening_socket();

	printf( "Starting packet logging...\n" );

	/* Logging loop */
	while( 1 )
		receive_and_log_packet();

	return EXIT_SUCCESS;
}

