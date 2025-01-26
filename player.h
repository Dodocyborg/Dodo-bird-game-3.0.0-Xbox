#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    // Constructors
    Player(std::string username, std::string sessionToken);  // Initialize a player with a username and session token
    Player();  // Default constructor

    // Setters
    void setUsername(const std::string& username);  // Set the player's username
    void setSessionToken(const std::string& sessionToken);  // Set the player's session token
    void setScore(int score);  // Set the player's score

    // Getters
    std::string getUsername() const;  // Get the player's username
    std::string getSessionToken() const;  // Get the player's session token
    int getScore() const;  // Get the player's score

    // Methods
    void increaseScore(int amount);  // Increase the player's score by a certain amount
    void decreaseScore(int amount);  // Decrease the player's score by a certain amount
    void resetScore();  // Reset the player's score to zero
    void displayPlayerInfo() const;  // Display player's information (for debugging or game management)

private:
    std::string username;  // The player's username
    std::string sessionToken;  // The session token associated with the player
    int score;  // The player's score in the game
};

#endif // PLAYER_H