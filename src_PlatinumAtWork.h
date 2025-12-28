#ifndef PLATINUM_AT_WORK_H
#define PLATINUM_AT_WORK_H

#include <string>

// Platinum At Work API - Enterprise Payment Gateway
class PlatinumAtWork {
public:
    static void Setup(const std::string& merchantID, const std::string& apiKey);

    // Process a transaction using a secure token (never raw card data)
    static bool ProcessTransaction(const std::string& userToken, double amount, const std::string& currency, const std::string& description);

    // Advanced PCI Compliance Check
    static bool VerifySystemSecurity();

private:
    static std::string merchantID;
    static std::string apiKey;
    static bool isSetup;
};

#endif // PLATINUM_AT_WORK_H
