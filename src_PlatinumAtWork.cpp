#include "src_PlatinumAtWork.h"
#include "src_PasswordBank.h"
#include <iostream>
#include <thread>
#include <chrono>

std::string PlatinumAtWork::merchantID = "";
std::string PlatinumAtWork::apiKey = "";
bool PlatinumAtWork::isSetup = false;

void PlatinumAtWork::Setup(const std::string& mID, const std::string& key) {
    merchantID = mID;
    // We store the API key in the Password Bank, not just locally
    PasswordBank::StoreAPIKey("PlatinumAtWork", key);
    apiKey = key; // Keep local ref for simulation
    isSetup = true;
    std::cout << "[PlatinumAtWork] Gateway initialized for Merchant ID: " << merchantID << ".\n";
}

bool PlatinumAtWork::VerifySystemSecurity() {
    std::cout << "[PlatinumAtWork] Performing PCI-DSS Compliance Handshake...\n";
    // Simulation of checking TLS 1.3, Firewall status, etc.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "[PlatinumAtWork] Security Check: PASSED. Connection is End-to-End Encrypted.\n";
    return true;
}

bool PlatinumAtWork::ProcessTransaction(const std::string& userToken, double amount, const std::string& currency, const std::string& description) {
    if (!isSetup) {
        std::cout << "[PlatinumAtWork] Error: Gateway not configured. Please contact administrator.\n";
        return false;
    }

    if (!PasswordBank::ValidateToken(userToken)) {
        std::cout << "[PlatinumAtWork] Error: Invalid or expired payment token.\n";
        return false;
    }

    std::cout << "[PlatinumAtWork] Initiating Transaction...\n";
    std::cout << "   Merchant: " << merchantID << "\n";
    std::cout << "   Amount:   " << amount << " " << currency << "\n";
    std::cout << "   Desc:     " << description << "\n";

    // Simulate Network Latency to API Endpoint
    std::cout << "[PlatinumAtWork] Connecting to Banking Network...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Simulate API Response
    std::cout << "[PlatinumAtWork] Response: 200 OK. Transaction ID: trans_" << std::rand() << "\n";
    return true;
}
