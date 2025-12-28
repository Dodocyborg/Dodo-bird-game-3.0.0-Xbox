#include "src_PaymentSystem.h"
#include "src_PlatinumAtWork.h"
#include "src_PasswordBank.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

bool PaymentSystem::processPayment(const std::string& user, double amount, PaymentMethod method, const std::string& details) {
    std::cout << "[Payment] Processing payment of $" << amount << " for user " << user << "...\n";

    // Mock security check
    if (details.empty()) {
        std::cout << "[Payment] Error: Invalid payment details.\n";
        return false;
    }

    // 5.0.0 Update: Use Platinum At Work for Credit/Debit
    if (method == PaymentMethod::CREDIT_CARD || method == PaymentMethod::DEBIT_CARD) {
        // 'details' is assumed to be "CardNumber|CVV" for simulation
        // In real app, this data comes from a secure form
        std::string cardNum = details.substr(0, 16);
        std::string cvv = "123"; // Mock extraction

        // 1. Tokenize via PasswordBank
        std::string token = PasswordBank::TokenizeCard(cardNum, cvv);

        // 2. Process via PlatinumAtWork
        if (PlatinumAtWork::ProcessTransaction(token, amount, "USD", "In-Game Purchase")) {
             return true;
        } else {
             std::cout << "[Payment] Transaction Declined by Bank.\n";
             return false;
        }
    }

    switch (method) {
        // Platinum At Work handles cards now (above)
        case PaymentMethod::ACH:
            std::cout << "[Payment] Verifying routing number for direct bank transfer...\n";
            break;
        case PaymentMethod::CASH_IN_STORE:
            std::cout << "[Payment] Please visit a local retailer to complete transaction.\n";
            return true;
        default:
            break;
    }

    std::cout << "[Payment] Transaction Successful. Receipt sent to email.\n";
    return true;
}

bool PaymentSystem::redeemGiftCard(const std::string& user, const std::string& code) {
    std::cout << "[GiftCard] Redeeming code: " << code << " for user " << user << "...\n";
    // Mock validation
    if (code.length() >= 16) {
        std::cout << "[GiftCard] Success! Funds added to account balance.\n";
        return true;
    }
    std::cout << "[GiftCard] Invalid code.\n";
    return false;
}

std::string PaymentSystem::generateEGiftCard(double amount, Currency currency) {
    // Generate a random 16-character alphanumeric code
    const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string code = "";
    for (int i = 0; i < 16; ++i) {
        code += charset[rand() % 36];
        if ((i + 1) % 4 == 0 && i != 15) code += "-";
    }
    std::string currStr = (currency == Currency::USD) ? "USD" : "International";
    std::cout << "[Payment] Generated E-Gift Card (" << currStr << " " << amount << "): " << code << "\n";
    return code;
}

void PaymentSystem::emailGiftCard(const std::string& email, const std::string& code) {
    std::cout << "[Email] Sending E-Gift Card to " << email << "... SENT.\n";
}

bool PaymentSystem::processRealMoneyPurchase(const std::string& user, const std::string& itemID, double price, Currency currency) {
    std::cout << "[Store] User " << user << " is purchasing " << itemID << " with Real Money.\n";

    // Simulate currency conversion or region check
    if (currency != Currency::USD) {
        std::cout << "[Store] Converting foreign currency to USD for processing...\n";
    }

    return processPayment(user, price, PaymentMethod::CREDIT_CARD, "StoredCard-Token-123");
}

void PaymentSystem::openSecureBillingForm(const std::string& user) {
    std::cout << "[Billing] Opening secure 256-bit encrypted billing form for " << user << "...\n";
}
