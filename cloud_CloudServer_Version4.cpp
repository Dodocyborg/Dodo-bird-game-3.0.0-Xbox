#include "CloudServer.h"

CloudServer::CloudServer(int port)
    : port(port),
      storageServer(std::make_unique<StorageServer>("data/")) {}

void CloudServer::start() {
    // Start listening for requests (stub)
}

void CloudServer::stop() {
    // Stop server (stub)
}

void CloudServer::handleRequest(const std::string& endpoint, const std::string& payload, std::string& response) {
    routeAPI(endpoint, payload, response);
}

void CloudServer::routeAPI(const std::string& endpoint, const std::string& payload, std::string& response) {
    if (endpoint == "/api/v1/auth/login") {
        authManager.login(payload, response);
    } else if (endpoint == "/api/v1/game/join") {
        std::string decrypted = encryptionManager.decrypt(payload);
        if (antiCheatManager.validate(decrypted)) {
            response = "Joined game!";
        } else {
            response = "Cheat detected!";
        }
    } else if (endpoint == "/api/v1/storage/save") {
        storageServer->saveData(payload, response);
    } else if (endpoint == "/api/v1/mods/upload") {
        modSandbox.handleModUpload(payload, response);
    } else {
        response = "Unknown endpoint";
    }
}