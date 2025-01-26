#include "Photon.h"

Photon::Photon(const std::string& id) : appId(id), isConnected(false) {}

Photon::~Photon() {
    disconnect();
}

bool Photon::connectToServer(const std::string& serverUrl) {
    this->serverUrl = serverUrl;
    isConnected = true; // Simulate a successful connection
    std::cout << "Connected to Photon server at " << serverUrl << std::endl;
    return isConnected;
}

void Photon::disconnect() {
    isConnected = false;
    std::cout << "Disconnected from Photon server." << std::endl;
}

void Photon::sendMessage(const std::string& message) {
    if (isConnected) {
        std::cout << "Sending message: " << message << std::endl;
    } else {
        std::cout << "Not connected to Photon server!" << std::endl;
    }
}

std::string Photon::receiveMessage() const {
    if (isConnected) {
        return "Message received from another player!";
    }
    return "No connection to server.";
}