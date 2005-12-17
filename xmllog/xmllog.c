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
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>

#include "internal.h"
#include "xmllog.h"

static int listening_socket;


static void create_listening_socket()
{
	struct sockaddr_in sin;
	int result;

	listening_socket = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( listening_socket < 0 ) {
		fprintf( stderr, "Could not create listening UDP socket.\n" );
		exit( EXIT_FAILURE );
	}
	
	bzero( &sin, sizeof( sin ) );
	sin.sin_family = AF_INET;
	sin.sin_port = htons( LOG_PORT );
	sin.sin_addr.s_addr = INADDR_ANY;

	result = bind( listening_socket, (struct sockaddr *)&sin, sizeof( sin ) );
	if ( result < 0 ) {
		fprintf( stderr, "Could not bind to logging UDP port %d.\n", LOG_PORT );
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
	printf( "\t\t<time>%s</time>\n", buf );
}


static void log_tcp_packet( char *buf, int len )
{
	/* This contains some pointer arithmetic because we probably are not
	 * allowed to publish a tcp_header_t structure */
	unsigned int ack = 0, seq;

	if ( len < TCP_HEADER_SIZE ) {
		printf( "\t\t\t<error>Packet Too Small</error>\n" );
		return;
	}

	seq = ntohl( *(u32_t*)(buf+4) );
	ack = ntohl( *(u32_t*)(buf+8) );

	printf( "\t\t\t<seq>%u</seq>\n\t\t\t<ack>%u</ack>\n", seq, ack );
			
	printf( "\t\t\t<flags>\n" );

	if ( (u8_t)*(buf+13) & 0x08 )
		printf( "\t\t\t\t<psh/>\n" );

	if ( (u8_t)*(buf+13) & 0x02 )
		printf( "\t\t\t\t<syn/>\n" );

	if ( (u8_t)*(buf+13) & 0x10 )
		printf( "\t\t\t\t<ack/>\n" );

	if ( (u8_t)*(buf+13) & 0x01 )
		printf( "\t\t\t\t<fin/>\n" );

	if ( (u8_t)*(buf+13) & 0x20 )
		printf( "\t\t\t\t<urg/>\n" );

	if ( (u8_t)*(buf+13) & 0x04 )
		printf( "\t\t\t\t<rst/>\n" );

	printf( "\t\t\t</flags>\n" );

	if ( len > TCP_HEADER_SIZE )
	{
		printf( "\t\t\t<data>\n" );
		printf( "\t\t\t\t<size>%d</size>\n", ( len - TCP_HEADER_SIZE ) );
		printf( "\t\t\t</data>\n" );
	}
}


static void log_packet( char *buf, int len )
{
	not_quite_ip_header_t *header;

	print_time();

	if ( len < sizeof( not_quite_ip_header_t ) )
	{
		printf( "\t\t<error>Packet Too Small</error>\n" );
		return;
	}

	header = (not_quite_ip_header_t *)buf;

	printf("\t\t<source>%s</source>\n",
	       fake_inet_ntoa(header->source));

	printf("\t\t<destination>%s</destination>\n",
	       fake_inet_ntoa(header->destination));

	if ( header->flags & DROP_PACKET )
		printf("\t\t<dropped/>\n");
	else if ( header->flags & CORRUPT_PACKET )
		printf("\t\t<corrupted/>\n");

	switch ( header->protocol ) {
		case IP_PROTO_UDP:
			printf( "\t\t<udp/>]\n");
			break;
		case IP_PROTO_TCP:
			printf( "\t\t<tcp>\n" );
			log_tcp_packet( buf + sizeof( not_quite_ip_header_t ),
			                len - sizeof( not_quite_ip_header_t ) );
			printf( "\t\t</tcp>\n" );
			break;
		default:
			printf( "\t\t<protocol>%d</protocol>\n",
			        ntohs( header->protocol ) );
	}
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
	{
		printf("\t<packet>\n");
		log_packet( buf, len );
		printf("\t</packet>\n\n");
		fflush( stdout );
	}
}


int main()
{
	fd_set read_fds;

	create_listening_socket();
	printf("<?xml version='1.0' encoding='UTF-8'?>\n" );
	printf( "<log>\n\n" );

	/* Logging loop */
	while( 1 )
	{
		FD_ZERO(&read_fds);
		FD_SET(listening_socket, &read_fds);
		FD_SET(0, &read_fds);

		select(listening_socket+1, &read_fds, NULL, NULL, NULL);

		if (FD_ISSET(listening_socket, &read_fds))
			receive_and_log_packet();
		else
			break;
	}

	printf( "</log>\n" );

	return EXIT_SUCCESS;
}

