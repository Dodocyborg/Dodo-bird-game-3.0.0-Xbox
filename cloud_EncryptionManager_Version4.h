#pragma once
#include <string>

class EncryptionManager {
public:
    std::string encrypt(const std::string& data);
    std::string decrypt(const std::string& encryptedData);
};