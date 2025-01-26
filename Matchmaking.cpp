{#include "matchmaking.h"
#include <algorithm>
#include <iostream>

std::vector<PlayerScore> leaderboard;

void updateLeaderboard(const PlayerScore& playerScore) {
    leaderboard.push_back(playerScore);
    // Sort leaderboard by score in descending order
    std::sort(leaderboard.begin(), leaderboard.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score;
    });
}

void matchmaking(std::vector<PlayerScore>& players) {
    // Match players based on scores, here we just sort them by score.
    std::sort(players.begin(), players.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score;
    });

    // For example, after sorting we can match the first player with the second, third with fourth, etc.
    for (size_t i = 0; i < players.size(); i += 2) {
        if (i + 1 < players.size()) {
            std::cout << "Matching: " << players[i].username << " vs " << players[i + 1].username << std::endl;
        }
    }
}

void displayLeaderboard() {
    std::cout << "Leaderboard:" << std::endl;
    for (const auto& player : leaderboard) {
        std::cout << player.username << ": " << player.score << std::endl;
    }
}#include "Matchmaking.h"
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
        std::vector<Player> match(players.begin() + i,
                                  players.begin() + std::min(i + MAX_PLAYERS_PER_MATCH, players.size()));
        matches.push_back(match);
        std::cout << "Match created with " << match.size() << " players!" << std::endl;
    }
}