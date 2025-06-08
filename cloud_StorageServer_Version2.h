#pragma once
#include <string>

class StorageServer {
public:
    StorageServer(const std::string& storagePath);
    void saveData(const std::string& payload, std::string& response);
    void loadData(const std::string& payload, std::string& response
