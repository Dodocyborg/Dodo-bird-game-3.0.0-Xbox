#include "src_EmailSystem.h"

void EmailSystem::sendSupportEmail(const std::string& userEmail, const std::string& issue) {
    Email e = { "support@dodogame.com", userEmail, "Support Request", issue };
    send(e);
    std::cout << "[EmailSystem] Support ticket created for " << userEmail << ".\n";
}

void EmailSystem::submitBanAppeal(const std::string& userEmail, const std::string& justification) {
    Email e = { "moderation@dodogame.com", userEmail, "Ban Appeal", justification };
    send(e);
    std::cout << "[EmailSystem] Ban appeal submitted. Please allow 24-48 hours for review.\n";
}

void EmailSystem::sendReceipt(const std::string& userEmail, const std::string& item, double amount) {
    Email e = { userEmail, "noreply@dodogame.com", "Purchase Receipt", "Thank you for buying " + item + " for $" + std::to_string(amount) };
    send(e);
}

void EmailSystem::send(const Email& email) {
    std::cout << ">>> EMAIL SENT >>>\n";
    std::cout << "To: " << email.to << "\n";
    std::cout << "From: " << email.from << "\n";
    std::cout << "Subject: " << email.subject << "\n";
    std::cout << "Body: " << email.body << "\n";
    std::cout << "<<< END EMAIL <<<\n";
}
