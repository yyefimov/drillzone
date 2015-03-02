// seq.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "SocketWrapper.h"

#define _WINSOCKAPI_

//#include <winsock.h>
#include <ws2tcpip.h>
#include <windows.h> 

#include <iostream>

#define DEST_MCAST        "234.5.6.7"     
#define DESTINATION_PORT  4567            
#define SOURCE_PORT       0               


int _tmain(int argc, _TCHAR* argv[])
{
	stream::SocketWrapper sw;

	std::cout << sw.test() << std::endl;

	int iOptVal = 64;
	char szMessage[] = "Multicasting message!";
	// Sent message string
	TCHAR szError[100];               // Error message string
	SOCKET Sock = INVALID_SOCKET;     // Datagram window socket

	SOCKADDR_IN source_sin,           // Source socket address
		dest_sin;             // Destination socket address

	WSADATA WSAData;                  // Contains details of the 
	// Winsock implementation

	// Initialize Winsock. 
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		wsprintf(szError, TEXT("WSAStartup failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	// Create a datagram window socket, Sock.
	if ((Sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		wsprintf(szError, TEXT("Allocating socket failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		return FALSE;
	}

	// Fill out source socket's address information.
	source_sin.sin_family = AF_INET;
	source_sin.sin_port = htons(SOURCE_PORT);
	source_sin.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associate the source socket's address with the socket, Sock.
	if (bind(Sock,
		(struct sockaddr FAR *) &source_sin,
		sizeof(source_sin)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("Binding socket failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}

	// Set the Time-to-Live of the multicast.
	if (setsockopt(Sock,
		IPPROTO_IP,
		IP_MULTICAST_TTL,
		(char FAR *)&iOptVal,
		sizeof(int)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("setsockopt failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}

	// Fill out the desination socket's address information.
	dest_sin.sin_family = AF_INET;
	dest_sin.sin_port = htons(DESTINATION_PORT);
	dest_sin.sin_addr.s_addr = inet_addr(DEST_MCAST);

	// Send a message to the multicasting address.
	if (sendto(Sock,
		szMessage,
		strlen(szMessage) + 1,
		0,
		(struct sockaddr FAR *) &dest_sin,
		sizeof(dest_sin)) == SOCKET_ERROR)
	{
		wsprintf(szError, TEXT("sendto failed! Error: %d"),
			WSAGetLastError());
		MessageBox(NULL, szError, TEXT("Error"), MB_OK);
		closesocket(Sock);
		return FALSE;
	}
	else
		MessageBox(NULL, TEXT("Sending data succeeded!"), TEXT("Info"), MB_OK);

	// Disable sending on Sock before closing it.
	shutdown(Sock, 0x01);

	// Close Sock.
	closesocket(Sock);

	WSACleanup();

	return TRUE;
}

