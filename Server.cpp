#include <iostream>
#include <winsock2.h> 
#include <ws2tcpip.h> //  для IPv6 и getaddrinfo()

#include <thread>

#include <ctime>

#pragma comment(lib, "Ws2_32.lib") 

 void message(SOCKET clientSocket) {
    try {
        std::cout << "Client sucsesfull coneccted\n";

        while (true) {

            char buffer[1024];

            short response = recv(clientSocket,buffer,sizeof(buffer),0);
            
            
            if (response <= 0) { 
                break;
            }


            buffer[response] = '\0';

            std::string message(buffer);

            



        }
    }

    catch (const std::exception& e) {
        std::cerr << "error to connection\n";
    }
    closesocket(clientSocket);
}

SOCKET AcceptConnect(SOCKET serverSocket) {
    
    sockaddr_in clientAddres;
    socklen_t addrSize = sizeof(clientAddres);

    //create client
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddres), &addrSize);

    
    if (clientSocket == INVALID_SOCKET)std::cerr << "error to accept connectr\n";
    else { std::cout << "cleint has been connected\n"; }


    return clientSocket;
}



int main()
{
    srand(time(NULL));


    WSAData wsaData;
    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddr, clientAddr;

    unsigned short addrLen = sizeof(clientAddr);


    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR) {
        std::cerr << "socket not initializated\n";
        return EXIT_FAILURE;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /*
    AF_INET: Используется для IPv4. Самый распространённый тип.
    AF_INET6: Использовать IPv6 протокол.
    AF_UNIX: Локальные Unix-домены (только для межпроцессного взаимодействия внутри одной машины).

    SOCK_STREAM: Используется для TCP (Transmission Control Protocol). Потоковые сокеты гарантируют надёжность передачи данных.
    SOCK_DGRAM: Использует UDP (User Datagram Protocol). Непротоколируемые датаграммы, быстрые, но менее надежные.
    SOCK_RAW: Доступ к низкоуровневым пакетам без использования стандартного уровня протоколов (используется редко, требует специальных прав).

    IPPROTO_TCP: Точно соответствует использованию TCP (наряду с SOCK_STREAM).
    IPPROTO_UDP: Соответствует использованию UDP (для SOCK_DGRAM).
    значение 0, чтобы система сама выбрала подходящий протокол по умолчанию
    */

    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "error to create socket\n";
        WSACleanup();
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < 10;i++) {
        unsigned short addr = 1024 + rand() % 10001;
        serverAddr.sin_port = htons(addr);

        if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) != SOCKET_ERROR) {
            std::cout<< ntohs(serverAddr.sin_port) << std::endl;
            
        }
        else {
            Sleep(100); 
            std::cout << "wait to binding port\n";
        }
    }

    if (listen(serverSocket,SOMAXCONN)==SOCKET_ERROR) {
        std::cerr << "error listening\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    else {
        std::cout << "server started:\n";
    }

    //unsigned long long* client_ptr = clientSocket;
    //main
    while(true)
    {
     
        SOCKET clientSocket = AcceptConnect(serverSocket);
        if (clientSocket != INVALID_SOCKET) {
            std::thread clientT(message, clientSocket);
            clientT.detach();
        }


    }
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

