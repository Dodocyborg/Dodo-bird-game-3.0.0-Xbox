#include "src_EconomySystem.h"
#include <iostream>
#include <vector>

void EconomySystem::addCredits(const std::string& user, int amount) {
    balances[user] += amount;
    std::cout << "[Economy] Added " << amount << " Dodo Credits to " << user << ". New Balance: " << balances[user] << "\n";
}

bool EconomySystem::spendCredits(const std::string& user, int amount) {
    if (balances[user] >= amount) {
        balances[user] -= amount;
        std::cout << "[Economy] User " << user << " spent " << amount << " credits. Remaining: " << balances[user] << "\n";
        return true;
    }
    std::cout << "[Economy] Insufficient funds for " << user << ".\n";
    return false;
}

int EconomySystem::getBalance(const std::string& user) {
    return balances[user];
}

void EconomySystem::purchaseIvyJersey(const std::string& user) {
    if (spendCredits(user, 500)) { // Cost 500
        inventory[user].push_back("Ivy Jersey");
        std::cout << "[Shop] You purchased the Ivy Jersey! Features: Enhanced style, +5 Charisma.\n";
    }
}

void EconomySystem::purchaseEndgameItem(const std::string& user, const std::string& itemName) {
    int cost = 10000;
    if (spendCredits(user, cost)) {
        inventory[user].push_back(itemName);
        std::cout << "[Shop] Purchased Endgame Item: " << itemName << ".\n";
    }
}
