#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> //  для IPv6 и getaddrinfo()
#include<string>

#include <thread>

#include <ctime>


#include<fstream>
#include<map>
#pragma comment(lib, "Ws2_32.lib") 


int main()
{
	WSAData wsaData;

	SOCKET  clientSocket;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
		std::cerr << "socket not initializated\n";
		return EXIT_FAILURE;
	}

	clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "error to create socket\n";
		WSACleanup();
		return EXIT_FAILURE;
	}

	return 0;
}

