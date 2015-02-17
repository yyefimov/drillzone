// router.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _WINSOCKAPI_

#include "quickfix/Utility.h"

//#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> 

#include <iostream>

#define RECV_IP_ADDR      "234.5.6.7"    
#define DEST_PORT         4567            

int _tmain(int argc, _TCHAR* argv[])
{
	int index = 0,                      // Integer index
		iRecvLen;                       // Length of recv_sin
	TCHAR szError[100];                 // Error message string

	SOCKET sock = INVALID_SOCKET;       // Datagram window socket

	//struct hostent *pHost = NULL;	
	SOCKADDR_IN recv_sin;               // Holds the source address upon recvfrom function returns

	FIX::socket_init();

	// Create a datagram socket, Sock.
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	FIX::socket_setsockopt(sock, SO_BROADCAST);
	FIX::socket_setsockopt(sock, SO_REUSEADDR);

	struct ip_mreq mreq;                // Used in adding or dropping multicasting addresses
	mreq.imr_multiaddr.s_addr = inet_addr(RECV_IP_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&mreq, sizeof(mreq));

	// Fill out the local socket's address information.
	SOCKADDR_IN local_sin;              // Local socket's address
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons(DEST_PORT);
	local_sin.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associate the local address with Sock.
	bind(sock, (struct sockaddr FAR *) &local_sin, sizeof(local_sin));

	// Join the multicast group from which to receive datagrams.
	
	char szMessageA[100];               // ASCII string 

	// Receive data from the multicasting group server.
	//iRecvLen = sizeof(recv_sin);
	//while (recvfrom(Sock, szMessageA, sizeof(szMessageA), 0, (struct sockaddr FAR *) &recv_sin, &iRecvLen) != SOCKET_ERROR)
	while(ssize_t size = recv(sock, szMessageA, sizeof(szMessageA), 0) != SOCKET_ERROR)
	{
		std::cout << szMessageA << std::endl;
	}

	FIX::socket_close(sock);

	FIX::socket_term();

	return TRUE;
}

