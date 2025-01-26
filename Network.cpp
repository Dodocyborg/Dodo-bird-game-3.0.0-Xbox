#include "network.h"
#include <iostream>
#include <winsock2.h>

void NetworkManager::connectToServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "Failed to connect to server." << std::endl;
    } else {
        std::cout << "Connected to the server!" << std::endl;
    }
}

void NetworkManager::sendMessage(const std::string& message) {
    send(clientSocket, message.c_str(), message.length(), 0);
}

void NetworkManager::cleanup() {
    closesocket(clientSocket);
    WSACleanup();
}