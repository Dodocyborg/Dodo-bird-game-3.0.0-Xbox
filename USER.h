#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_map>

class User {
public:
    // Method to hash an email
    static std::string hashEmail(const std::string& email);

    // Method to generate a random verification code
    static std::string generateVerificationCode();

    // Simulate sending an email
    static void sendVerificationEmail(const std::string& email, const std::string& code);

    // Method to authenticate the user with session token
    static bool authenticateUser(const std::string& sessionToken);

    // Method to create a session token based on email and username
    static std::string createSessionToken(const std::string& email, const std::string& username);

    // Method to create a new user account
    static bool createUser(const std::string& email, const std::string& username);

private:
    static std::unordered_map<std::string, std::string> userEmails; // {hashed_email -> username}
    static std::unordered_map<std::string, std::string> userSessions; // {session_token -> user_email}
};

#endif // USER_H