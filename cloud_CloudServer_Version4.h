#pragma once
#include <string>
#include <memory>
#include "AuthManager.h"
#include "EncryptionManager.h"
#include "AntiCheatManager.h"
#include "ModSandbox.h"
#include "StorageServer.h"

class CloudServer {
public:
    CloudServer(int port);
    void start();
    void stop();
    void handleRequest(const std::string& endpoint, const std::string& payload, std::string& response);

private:
    int port;
    AuthManager authManager;
    EncryptionManager encryptionManager;
    AntiCheatManager antiCheatManager;
    ModSandbox modSandbox;
    std::unique_ptr<StorageServer> storageServer;

    void routeAPI(const std::string& endpoint, const std::string& payload, std::string& response);
};