#ifndef PHOTON_H
#define PHOTON_H

#include <iostream>
#include <string>

class Photon {
public:
    // Constructor
    Photon(const std::string& appId);

    // Destructor
    ~Photon();

    // Connect to the Photon server
    bool connectToServer(const std::string& serverUrl);

    // Disconnect from the Photon server
    void disconnect();

    // Send a message to other players
    void sendMessage(const std::string& message);

    // Receive a message from other players
    std::string receiveMessage() const;

private:
    std::string appId;     // Application ID for Photon
    bool isConnected;      // Connection status
    std::string serverUrl; // URL of the Photon server
};

#endif // PHOTON_H