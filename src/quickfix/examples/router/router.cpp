#include "stdafx.h"

#include "SocketUtil.h"
#include "SocketWrapper.h"

#include <iostream>

#define RECV_IP_ADDR      "234.5.6.7"    
#define DEST_PORT         4567            

int main(int argc, char** argv)
{
	stream::socket_init();
	int sock = stream::socket_create_udp();

	stream::socket_setsockopt(sock, SO_BROADCAST);
	stream::socket_setsockopt(sock, SO_REUSEADDR);

	stream::socket_add_membership(sock, RECV_IP_ADDR);

	// Join the multicast group from which to receive datagrams.
	stream::socket_bind(sock, DEST_PORT);
	
	char szMessageA[100];

	// Receive data from the multicasting group server.
	while(int size = recv(sock, szMessageA, sizeof(szMessageA), 0) != SOCKET_ERROR)
	{
		std::cout << szMessageA << std::endl;
	}

	stream::socket_close(sock);
	stream::socket_term();

	return TRUE;
}

