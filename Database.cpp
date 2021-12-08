#include <iostream>
#include <string>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
    char bruker[13] = "119212209202";
    char command[2] = "1";
    char error[2] = "2";
    int msglen = strlen(command);
    int msglen_2 = strlen(error);

    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }


    const int port = 12010;
    char msg[1500];
    sockaddr_in servAddr;
    memset((char*)&servAddr, '\0', sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd == INVALID_SOCKET)
    {
        std::cerr << "Error establishing the server socket" << std::endl;
        return 1;
    }
    int bindStatus = bind(serverSd, (struct sockaddr*)&servAddr,
        sizeof(servAddr));
    if (bindStatus < 0)
    {
        std::cerr << "Error binding socket to local address" << std::endl;
        return 1;
    }
    std::cout << "Waiting for a client to connect..." << std::endl;
    listen(serverSd, 1);
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr*)&newSockAddr, &newSockAddrSize);
    if (newSd < 0)
    {
        std::cerr << "Error accepting request from client!" << std::endl;
        return 1;
    }
    std::cout << "Connected with client!" << std::endl;

    while (true)
    {
        std::cout << "Awaiting client response..." << std::endl;
        memset(&msg, 0, sizeof(msg));
        recv(newSd, (char*)&msg, sizeof(msg), 0);
        std::cout << "Client: " << msg << std::endl;
        if (!strcmp(msg, bruker))
        {
            std::cout << command << std::endl;
            memset(&msg, 0, sizeof(msg));
            strcpy_s(msg, command);
            send(newSd, (char*)&msg, strlen(msg), 0);
        }

    }

    closesocket(newSd);
    closesocket(serverSd);

    std::cout << "Connection closed..." << std::endl;
    return 0;
}
