#include "Matchmaking.h"
#include <algorithm>
#include <iostream>

void Matchmaking::initialize() {
    std::cout << "Matchmaking system initialized!" << std::endl;
}

void Matchmaking::matchPlayers(std::vector<Player>& players) {
    // Sort players by rank (ascending order)
    std::sort(players.begin(), players.end(), [](Player& a, Player& b) {
        return a.rank < b.rank;
    });

    // Group players into matches
    for (size_t i = 0; i < players.size(); i += MAX_PLAYERS_PER_MATCH) {
        std::vector<Player> match;
        for (size_t j = 0; j < MAX_PLAYERS_PER_MATCH && i + j < players.size(); ++j) {
            match.push_back(players[i + j]);
        }
        matches.push_back(match);
        std::cout << "Match created with " << match.size() << " players!" << std::endl;
    }
}
