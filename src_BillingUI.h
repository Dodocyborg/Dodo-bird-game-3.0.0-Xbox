#ifndef BILLING_UI_H
#define BILLING_UI_H

#include <string>

struct PaymentDetails {
    std::string cardNumber;
    std::string expiry;
    std::string cvv;
    bool cancelled;
};

class BillingUI {
public:
    // Pauses the game to prompt user for payment info in the console
    static PaymentDetails ShowCheckoutForm(const std::string& itemName, double price);
};

#endif // BILLING_UI_H
