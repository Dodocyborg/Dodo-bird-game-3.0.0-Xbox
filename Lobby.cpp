{// lobby.cpp
#include "lobby.h"
#include "glm.hpp

Lobby::Lobby(int id, const std::string& name) : lobbyId(id), lobbyName(name), gameStarted(false) {}

void Lobby::addPlayer(int playerId) {
    if (players.size() < 4) {  // Example: max 4 players per lobby
        players.push_back(playerId);
        std::cout << "Player " << playerId << " added to lobby: " << lobbyName << std::endl;
    } else {
        std::cout << "Lobby is full." << std::endl;
    }
}

void Lobby::removePlayer(int playerId) {
    auto it = std::find(players.begin(), players.end(), playerId);
    if (it != players.end()) {
        players.erase(it);
        std::cout << "Player " << playerId << " removed from lobby: " << lobbyName << std::endl;
    }
}

void Lobby::generateMap() {
    // Simple procedural generation: For now, just a random string as a map identifier
    srand(time(0));
    generatedMap = "Map" + std::to_string(rand() % 1000);  // Random map ID
    std::cout << "Generated map for lobby " << lobbyName << ": " << generatedMap << std::endl;
}

void Lobby::startGame() {
    if (players.size() >= 2) {  // Start game if enough players
        gameStarted = true;
        std::cout << "Game started in lobby: " << lobbyName << std::endl;
    } else {
        std::cout << "Not enough players to start the game." << std::endl;
    }
}

int Lobby::getId() const {
    return lobbyId;
}

const std::string& Lobby::getName() const {
    return lobbyName;
}

bool Lobby::isFull() const {
    return players.size() >= 4;  // Example limit
}