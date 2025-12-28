#ifndef ECONOMY_SYSTEM_H
#define ECONOMY_SYSTEM_H

#include <string>
#include <unordered_map>

class EconomySystem {
public:
    void addCredits(const std::string& user, int amount);
    bool spendCredits(const std::string& user, int amount);
    int getBalance(const std::string& user);

    // In-game purchases
    void purchaseIvyJersey(const std::string& user);
    void purchaseEndgameItem(const std::string& user, const std::string& itemName);

private:
    std::unordered_map<std::string, int> balances;
    std::unordered_map<std::string, std::vector<std::string>> inventory;
};

#endif // ECONOMY_SYSTEM_H
