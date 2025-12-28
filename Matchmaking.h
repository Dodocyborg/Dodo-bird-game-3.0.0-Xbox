#ifndef MATCHMAKING_H
#define MATCHMAKING_H

#include <vector>
#include <iostream>

struct Player {
    int rank;
    std::string username;
};

class Matchmaking {
public:
    static const int MAX_PLAYERS_PER_MATCH = 4;
    std::vector<std::vector<Player>> matches;

    void initialize();
    void matchPlayers(std::vector<Player>& players);
};

#endif // MATCHMAKING_H
