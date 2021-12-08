#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "SerialClass.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)

int main(int argc, char* argv[])
{
    Serial* port = new Serial("COM7");
    
    if (port->IsConnected()) std::cout << "Connected!" << std::endl;
    if (!port->IsConnected()) std::cout << "not Connected!" << std::endl;
    
    std::cout << "test test" << std::endl;
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    const char* serverIp = "localhost";
    const int porter = 12010;
    char msg[1500];
    
    hostent* host = gethostbyname(serverIp);

    sockaddr_in sendSockAddr;
    memset((char*)&sendSockAddr, '\0', sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
        inet_addr(inet_ntoa(*(in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(porter);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    int status = connect(clientSd,
        (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
    if (status == INVALID_SOCKET)
    {
        std::cout << "Error connecting to socket!" << std::endl;
        return -1;
    }
    std::cout << "Connected to the server!" << std::endl;

    while (true)
    {
        char data[13] = "";
        char command[2] = "1";
        int msglen = strlen(command);
        int n;
        
        for (int i = 0; i < 12; ++i) { data[i] = 0; }

        while ((n = port->ReadData(data, 12)) < 11) { data[n] = 0; }
        memset(&msg, 0, sizeof(msg));
        strcpy_s(msg, data);
        send(clientSd, (char*)&msg, strlen(msg), 0);
        if (data == "exit")
        {
            break;
        }

        std::cout << "Awaiting server response..." << std::endl;
        memset(&msg, 0, sizeof(msg));
        recv(clientSd, (char*)&msg, sizeof(msg), 0);
        if (!strcmp(msg, "1"))
        {
            if (port->WriteData(command, msglen))
            {
                printf("\n(writing success)\n");
            }
        }
        if (!strcmp(msg, "exit"))
        {
            std::cout << "Server has quit the session" << std::endl;
            break;
        }
        std::cout << "Server: " << msg << std::endl;
    }

    closesocket(clientSd);

    std::cout << "Connection closed" << std::endl;
    return 0;
}
