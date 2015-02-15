// router.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _WINSOCKAPI_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> 

#define RECV_IP_ADDR      "234.5.6.7"    
#define DEST_PORT         4567            

int _tmain(int argc, _TCHAR* argv[])
{
	int index = 0,                      // Integer index
		iRecvLen;                       // Length of recv_sin
	char szMessageA[100];               // ASCII string 
	TCHAR szMessageW[100];              // Unicode string
	TCHAR szError[100];                 // Error message string

	SOCKET Sock = INVALID_SOCKET;       // Datagram window socket
	SOCKET tmpSock = INVALID_SOCKET;       // Datagram window socket
	SOCKET *pSock = &tmpSock;

	BOOL bOptVal = TRUE;
	struct hostent *pHost = NULL;
	struct ip_mreq mreq;                // Used in adding or dropping 
	// multicasting addresses
	SOCKADDR_IN local_sin,              // Local socket's address
		recv_sin;               // Holds the source address upon 
	// recvfrom function returns
	WSADATA WSAData;                    // Contains details of the 
	// Winsock implementation

	// Initialize Winsock. 
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		wsprintf(szError, TEXT("WSAStartup failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	// Identify local machine
	char hostName[100];
	gethostname(hostName, 100);
	MessageBox(NULL, szError, TEXT("Host Name"), MB_OK);
	pHost = gethostbyname(hostName);
	while (pHost->h_addr_list[index])
	{
		memcpy(&local_sin.sin_addr, pHost->h_addr_list[index++], pHost->h_length);
		strcpy(hostName, inet_ntoa(local_sin.sin_addr));
	}

	// Create a datagram socket, Sock.
	if ((*pSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		wsprintf(szError, TEXT("Allocating socket failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}
	Sock = *pSock;
	// Fill out the local socket's address information.
	local_sin.sin_family = AF_INET;
	local_sin.sin_port = htons(DEST_PORT);
	local_sin.sin_addr.s_addr = htonl(INADDR_ANY);

	if (setsockopt(Sock,
		SOL_SOCKET,
		SO_BROADCAST,
		(char*)&bOptVal,
		sizeof(bOptVal)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("setsockopt2 failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}

	if (setsockopt(Sock,
		SOL_SOCKET,
		SO_REUSEADDR,
		(char*)&bOptVal,
		sizeof(bOptVal)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("setsockopt3 failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}


	// Associate the local address with Sock.
	if (bind(Sock,
		(struct sockaddr FAR *) &local_sin,
		sizeof(local_sin)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("Binding socket failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}


	// Join the multicast group from which to receive datagrams.
	mreq.imr_multiaddr.s_addr = inet_addr(RECV_IP_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(Sock,
		IPPROTO_IP,
		IP_ADD_MEMBERSHIP,
		(char FAR *)&mreq,
		sizeof(mreq)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("setsockopt failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}

	iRecvLen = sizeof(recv_sin);

	// Receive data from the multicasting group server.
	if (recvfrom(Sock,
		szMessageA,
		100,
		0,
		(struct sockaddr FAR *) &recv_sin,
		&iRecvLen) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("recvfrom failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}
	else
	{
		// Convert the ASCII string to a Unicode string.
		for (index = 0; index <= 50; index++)
			szMessageW[index] = szMessageA[index];

		MessageBox(NULL, szMessageW, TEXT("Info"), MB_OK);
	}

	// Disable receiving on Sock before closing it.
	shutdown(Sock, 0x00);

	// Close Sock.
	closesocket(Sock);

	WSACleanup();

	return TRUE;
}

