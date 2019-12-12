
#include <stdio.h>
#include <iostream>

#include "mysocket.h"
#include "HttpParser.h"

#pragma comment(lib, "Ws2_32.lib")

// maxsize for our recv buffer
constexpr auto MAX_BUFFER = 65536;

// receive buffer
static char recvBuf[MAX_BUFFER];

HttpParser* httpParser;

int main() {

	// start winsock version 2.2
	startSocketProgram(2, 2);
	// set up TCP protocol, create socket, bind and listen to it
	SOCKET serverSock = initServerSocket();

	// receive buffer length
	int recvLen = 1;
	// client socket we are going to accept
	SOCKET clientSock = INVALID_SOCKET;

	std::cout << std::endl << std::endl;

	do {
		clientSock = acceptSocket(serverSock);

		recvLen = recv(clientSock, recvBuf, MAX_BUFFER, 0);

		if (recvLen > 0) {

			//printf("+++++++++++++++++++++++++++++\n");
			//printf("%s\n", recvBuf);
			//printf("+++++++++++++++++++++++++++++\n");

			httpParser = new HttpParser();
			httpParser->parse(recvBuf);
			
			//printf("-----------------------------\n");
			//httpParser->printRequestLine();
			//httpParser->printHeaderLines();
			//httpParser->printRequestBody();
			//printf("-----------------------------\n");

			delete httpParser;
		}
		else if (recvLen == 0)
			printf("Connection closing...\n");
		else
			printf("recv failed %d\n", WSAGetLastError());

		Sleep(200);

	} while (recvLen > 0);
		
	// close server socket 
	closeTCPsocket(serverSock);

	// close winsock program, release resources
	endSocketProgram();
}