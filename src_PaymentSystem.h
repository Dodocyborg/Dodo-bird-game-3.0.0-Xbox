#ifndef PAYMENT_SYSTEM_H
#define PAYMENT_SYSTEM_H

#include <string>
#include <iostream>

enum class PaymentMethod {
    CREDIT_CARD,
    DEBIT_CARD,
    ACH,
    GIFT_CARD,
    CASH_IN_STORE,
    REAL_MONEY_TRANSACTION
};

enum class Currency {
    USD,
    EUR,
    GBP,
    JPY
};

class PaymentSystem {
public:
    static bool processPayment(const std::string& user, double amount, PaymentMethod method, const std::string& details);
    static bool redeemGiftCard(const std::string& user, const std::string& code);

    // E-Gift Card System
    static std::string generateEGiftCard(double amount, Currency currency);
    static void emailGiftCard(const std::string& email, const std::string& code);

    // Real Money / Endgame Purchases
    // Now accepts raw card details passed from the UI
    static bool processRealMoneyPurchase(const std::string& user, const std::string& itemID, double price, Currency currency, const std::string& cardNum, const std::string& cvv);

    // Secure billing form (mock)
    static void openSecureBillingForm(const std::string& user);
};

#endif // PAYMENT_SYSTEM_H
