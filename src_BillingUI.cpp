#include "src_BillingUI.h"
#include <iostream>
#include <limits>

PaymentDetails BillingUI::ShowCheckoutForm(const std::string& itemName, double price) {
    PaymentDetails details;
    details.cancelled = false;

    std::cout << "\n=========================================\n";
    std::cout << "        PLATINUM SECURE CHECKOUT         \n";
    std::cout << "=========================================\n";
    std::cout << " Item:  " << itemName << "\n";
    std::cout << " Price: $" << price << "\n";
    std::cout << "=========================================\n";
    std::cout << "Please enter your payment details below.\n";
    std::cout << "-----------------------------------------\n";

    // Card Number
    std::cout << "Card Number (16 digits): ";
    std::cin >> details.cardNumber;

    // Basic validation check (simulated UI validation)
    if (details.cardNumber.length() < 16) {
        std::cout << "[UI] Error: Invalid card number format.\n";
        details.cancelled = true;
        return details;
    }

    // Expiry
    std::cout << "Expiry (MM/YY): ";
    std::cin >> details.expiry;

    // CVV
    std::cout << "CVV (3 digits): ";
    std::cin >> details.cvv;

    std::cout << "\n[UI] Encrypting data...";
    // In a real app, we would clear the console or mask inputs
    std::cout << " Done.\n";
    std::cout << "Processing Transaction...\n";

    // Clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return details;
}
