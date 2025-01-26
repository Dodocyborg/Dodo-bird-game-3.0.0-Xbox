#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <openssl/sha.h>
#include <bcrypt/BCrypt.hpp> // Hashing emails and credentials

// User data storage (In practice, this would be a database)
std::unordered_map<std::string, std::string> userEmails;  // {hashed_email -> username}
std::unordered_map<std::string, std::string> userSessions; // {session_token -> user_email}

// Simulate sending an email (use an email service like SendGrid, Mailgun, etc.)
void sendVerificationEmail(const std::string& email, const std::string& code) {
    std::cout << "Sending verification code to: " << email << std::endl;
    std::cout << "Verification Code: " << code << std::endl; // This would be sent to email
}

// Generate a random 6-digit code for verification
std::string generateVerificationCode() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(100000, 999999);
    return std::to_string(dist(rd));
}

// Hash an email address for secure storage (using bcrypt)
std::string hashEmail(const std::string& email) {
    return BCrypt::generateHash(email);
}

// Create a session token using a combination of email and username
std::string createSessionToken(const std::string& email, const std::string& username) {
    return BCrypt::generateHash(email + username);  // Simple hash of email and username
}

// Authenticate the user using the session token
bool authenticateUser(const std::string& sessionToken) {
    // Check if session token exists and is valid
    if (userSessions.find(sessionToken) != userSessions.end()) {
        std::string storedEmail = userSessions[sessionToken];
        std::cout << "User with email: " << storedEmail << " authenticated successfully!" << std::endl;
        return true;
    }
    return false;
}

int main() {
    std::string email, username, code, enteredCode, sessionToken;

    // Step 1: User enters their email
    std::cout << "Enter your email to log in: ";
    std::getline(std::cin, email);

    // Hash the email to store securely
    std::string hashedEmail = hashEmail(email);

    // Step 2: Generate and send verification code to user's email
    code = generateVerificationCode();
    sendVerificationEmail(email, code);

    // Step 3: User enters the verification code they received in their email
    std::cout << "Enter the verification code sent to your email: ";
    std::getline(std::cin, enteredCode);

    // Check if entered code matches
    if (enteredCode == code) {
        std::cout << "Verification successful! Proceeding to username creation." << std::endl;

        // Step 4: Create username (prompt user)
        std::cout << "Create your username: ";
        std::getline(std::cin, username);

        // Store the user data (this is simplified, normally stored in a database)
        userEmails[hashedEmail] = username;

        // Step 5: Create session token
        sessionToken = createSessionToken(email, username);

        // Store session token in session map
        userSessions[sessionToken] = hashedEmail;

        std::cout << "Account successfully created and logged in!" << std::endl;
        std::cout << "Your username is: " << username << std::endl;

        // Step 6: Authentication (simulating session validation)
        std::cout << "Enter your session token to authenticate: ";
        std::getline(std::cin, enteredCode);

        if (authenticateUser(sessionToken)) {
            std::cout << "Welcome, " << username << "!" << std::endl;
            // User is now logged in and can proceed with the game
        } else {
            std::cout << "Authentication failed. Please try again." << std::endl;
        }
    } else {
        std::cout << "Invalid verification code. Please check your email and try again." << std::endl;
    }

    return 0;
}