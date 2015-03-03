#include "stdafx.h"

#include "SocketUtil.h"
#include "SocketWrapper.h"

#include <iostream>

#define DEST_MCAST        "234.5.6.7"     
#define DESTINATION_PORT  4567            
#define SOURCE_PORT       0               

int main(int argc, char** argv)
{
	char szMessage[] = "Multicasting message!";

	stream::socket_init();
	int sock = stream::socket_create_udp();

	stream::socket_bind(sock, SOURCE_PORT);

	// Fill out the desination socket's address information.
	SOCKADDR_IN dest_sin;             // Destination socket address
	dest_sin.sin_family = AF_INET;
	dest_sin.sin_port = htons(DESTINATION_PORT);
	dest_sin.sin_addr.s_addr = inet_addr(DEST_MCAST);

	// Send a message to the multicasting address.
	sendto(sock, szMessage, strlen(szMessage) + 1, 0, (struct sockaddr FAR *) &dest_sin, sizeof(dest_sin));

	stream::socket_close(sock);
	stream::socket_term();

	return TRUE;
}

