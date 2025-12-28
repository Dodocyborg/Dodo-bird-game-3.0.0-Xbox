#ifndef EMAIL_SYSTEM_H
#define EMAIL_SYSTEM_H

#include <string>
#include <vector>
#include <iostream>

struct Email {
    std::string to;
    std::string from;
    std::string subject;
    std::string body;
};

class EmailSystem {
public:
    static void sendSupportEmail(const std::string& userEmail, const std::string& issue);
    static void submitBanAppeal(const std::string& userEmail, const std::string& justification);
    static void sendReceipt(const std::string& userEmail, const std::string& item, double amount);

    // Internal use
    static void send(const Email& email);
};

#endif // EMAIL_SYSTEM_H
