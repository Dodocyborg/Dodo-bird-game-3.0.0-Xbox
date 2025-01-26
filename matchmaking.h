#ifndef MATCHMAKING_H
#define MATCHMAKING_H

#include <vector>
#include <string>

struct PlayerScore {
    std::string username;
    int score;
};

extern std::vector<PlayerScore> leaderboard;

void updateLeaderboard(const PlayerScore& playerScore);
void matchmaking(std::vector<PlayerScore>& players);
void displayLeaderboard();

#endif