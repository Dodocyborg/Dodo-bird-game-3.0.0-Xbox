// server.cpp
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <map>
#include "lobby.h"
#include "winsock2.h"

class MultiplayerServer {
public:
    void startServer();
    void handleClient(SOCKET clientSocket);
    void listLobbies(SOCKET clientSocket);
    void createLobby(SOCKET clientSocket);
    void joinLobby(SOCKET clientSocket, int lobbyId);

private:
    SOCKET serverSocket;
    std::vector<SOCKET> clients;
    std::map<int, Lobby> lobbies;  // Map of lobbies by ID
    int lobbyCounter = 0;
};

void MultiplayerServer::startServer() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // Create the server socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Listen on port 12345
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Bind to any IP

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 10);  // Listen for up to 10 incoming connections

    std::cout << "Server listening on port 12345..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);  // Accept a new client
        std::cout << "Client connected!" << std::endl;
        clients.push_back(clientSocket);  // Add the client socket to the list

        std::thread(&MultiplayerServer::handleClient, this, clientSocket).detach();
    }
}

void MultiplayerServer::handleClient(SOCKET clientSocket) {
    char buffer[512];
    int lobbyId = -1;
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';  // Null-terminate the message
            std::string message(buffer);

            if (message == "list_lobbies") {
                listLobbies(clientSocket);  // Send a list of available lobbies
            } else if (message == "create_lobby") {
                createLobby(clientSocket);  // Create a new lobby
            } else if (message.substr(0, 10) == "join_lobby") {
                // Join a lobby (message format: join_lobby <lobbyId>)
                lobbyId = std::stoi(message.substr(11));
                joinLobby(clientSocket, lobbyId);
            } else if (message == "start_game") {
                if (lobbyId != -1) {
                    lobbies[lobbyId].startGame();  // Start game in the chosen lobby
                }
            }
        }
    }
}

void MultiplayerServer::listLobbies(SOCKET clientSocket) {
    std::string response = "Available lobbies:\n";
    for (auto& pair : lobbies) {
        response += "Lobby " + std::to_string(pair.first) + ": " + pair.second.getName() + "\n";
    }
    send(clientSocket, response.c_str(), response.length(), 0);
}

void MultiplayerServer::createLobby(SOCKET clientSocket) {
    lobbyCounter++;
    Lobby newLobby(lobbyCounter, "Lobby " + std::to_string(lobbyCounter));
    newLobby.addPlayer(1);  // The creator automatically joins
    lobbies[lobbyCounter] = newLobby;

    newLobby.generateMap();  // Generate a map for the new lobby

    std::string response = "Created Lobby " + std::to_string(lobbyCounter) + ". You are the first player.";
    send(clientSocket, response.c_str(), response.length(), 0);
}

void MultiplayerServer::joinLobby(SOCKET clientSocket, int lobbyId) {
    if (lobbies.find(lobbyId) != lobbies.end() && !lobbies[lobbyId].isFull()) {
        lobbies[lobbyId].addPlayer(1);  // Player joins the lobby
        std::string response = "Joined Lobby " + std::to_string(lobbyId);
        send(clientSocket, response.c_str(), response.length(), 0);
    } else {
        std::string response = "Lobby is full or doesn't exist.";
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

int main() {
    MultiplayerServer server;
    server.startServer();
    return 0;
}