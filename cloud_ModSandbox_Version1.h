#pragma once
#include <string>

class ModSandbox {
public:
    void handleModUpload(const std::string& modPayload, std::string& response);
    bool validateMod(const std::string& modData);
    // Restrict mod capabilities for security
};