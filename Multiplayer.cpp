#include <iostream>
#include <vector>
#include <string>
// #include "photon/Photon.h"  // Remove the incorrect path

#include "Photon.h" // Use the correct path (assuming the Photon SDK was installed correctly)

// Photon Networking class to handle Photon setup
class PhotonNetwork {
public:
    PhotonNetwork() {
        // Initialize Photon client
        photonClient = new ExitGames::LoadBalancing::Client();
    }

    ~PhotonNetwork() {
        // Clean up Photon client
        delete photonClient;
    }

    // Connect to Photon
    void Connect() {
        ExitGames::LoadBalancing::Client::setAppId("YOUR_APP_ID");  // Set your Photon App ID here
        photonClient->connectToRegionMaster("usw");  // Connect to the region master server
        std::cout << "Connecting to Photon..." << std::endl;
    }

    // Create a new room
    void CreateRoom() {
        ExitGames::LoadBalancing::RoomOptions roomOptions;
        roomOptions.setMaxPlayers(4);  // Example: Max 4 players
        photonClient->createRoom("MyRoom", roomOptions);
        std::cout << "Room 'MyRoom' created." << std::endl;
    }

    // Join an existing room
    void JoinRoom() {
        photonClient->joinRoom("MyRoom");
        std::cout << "Joined 'MyRoom'." << std::endl;
    }

    // Send custom data (e.g., player position) to all other players
    void SendDataToPlayers(const std::vector<float>& data) {
        photonClient->sendDataToOthers(data);
        std::cout << "Sent data to other players." << std::endl;
    }

    // Get list of available rooms (for the lobby)
    std::vector<std::string> GetAvailableRooms() {
        // Simulate room retrieval, replace with actual API call
        return {"Room1", "Room2", "Room3"};
    }

private:
    ExitGames::LoadBalancing::Client* photonClient;
};

// Player class to handle player synchronization
class Player {
public:
    Player(const std::string& name) : playerName(name) {
        position = {0.0f, 0.0f, 0.0f}; // Initial position (x, y, z)
    }

    void SetPosition(float x, float y, float z) {
        position = {x, y, z};
    }

    std::vector<float> GetPosition() {
        return position;
    }

    void SendPositionToOthers() {
        photonNetwork.SendDataToPlayers(position);
    }

    void ReceivePositionFromOtherPlayer(std::vector<float> receivedPosition) {
        SetPosition(receivedPosition[0], receivedPosition[1], receivedPosition[2]);
        std::cout << "Received position from another player: "
                  << "x: " << position[0] << ", y: " << position[1] << ", z: " << position[2] << std::endl;
    }

private:
    std::string playerName;
    std::vector<float> position;  // [x, y, z] for player position
    PhotonNetwork photonNetwork;  // Network instance to communicate with Photon
};

// Main multiplayer class to handle room creation, joining, and synchronization
class MultiplayerGame {
public:
    MultiplayerGame() {
        photonNetwork = new PhotonNetwork();
    }

    ~MultiplayerGame() {
        delete photonNetwork;
    }

    void Start() {
        photonNetwork->Connect();  // Connect to Photon server
        ShowLobbyMenu();           // Display lobby for room selection or creation
    }

    void ShowLobbyMenu() {
        std::cout << "Available rooms:" << std::endl;

        // Display available rooms
        std::vector<std::string> availableRooms = photonNetwork->GetAvailableRooms();
        for (const auto& room : availableRooms) {
            std::cout << room << std::endl;
        }

        // Here, we'll simulate joining a room
        JoinRoom("Room1");
    }

    void JoinRoom(const std::string& roomName) {
        photonNetwork->JoinRoom();  // Join the room
        std::cout << "Joining room: " << roomName << std::endl;

        // Create a new player and sync its position
        player = new Player("Player1");
        player->SetPosition(10.0f, 5.0f, 0.0f);
        player->SendPositionToOthers();  // Send initial position to others
    }

    void Update() {
        // Simulate player movement and sync position to others
        player->SetPosition(player->GetPosition()[0] + 1.0f, player->GetPosition()[1], player->GetPosition()[2]);
        player->SendPositionToOthers();  // Send updated position to all players
    }

private:
    PhotonNetwork* photonNetwork;
    Player* player;
};

// Main function to start the game and run multiplayer networking
int main() {
    MultiplayerGame multiplayerGame;

    multiplayerGame.Start();  // Start multiplayer game
    multiplayerGame.Update(); // Simulate player movement and network updates

    // Game loop or further logic can go here

    return 0;
}