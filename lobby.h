#ifndef LOBBY_H
#define LOBBY_H

#include <vector>
#include <string>
#include <iostream>
#include "Player.h"
#include "Photon.h"  // Assuming a Photon class exists to handle multiplayer connections

class Lobby {
public:
    // Constructor
    Lobby(const std::string& lobbyName);

    // Destructor
    ~Lobby();

    // Add player to the lobby
    void addPlayer(const Player& player);

    // Remove player from the lobby
    void removePlayer(const std::string& playerName);

    // Start the game (move players to a game room)
    void startGame();

    // List all players in the lobby
    void listPlayers() const;

    // Get the current lobby status
    std::string getLobbyStatus() const;

    // Connect the lobby to Photon
    void connectToPhoton(const Photon& photon);

private:
    std::string lobbyName;
    std::vector<Player> players;  // List of players currently in the lobby
    Photon* photonConnection;     // Pointer to the Photon connection object
};

#endif // LOBBY_H