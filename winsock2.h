{#include "winsock2.h"

int main() {
    Winsock winsock;

    if (!winsock.initialize()) {
        return 1;  // Initialization failed
    }

    SOCKET sock = winsock.createSocket();
    if (sock == INVALID_SOCKET) {
        return 1;  // Socket creation failed
    }

    if (!winsock.connectToServer("127.0.0.1", 8080)) {
        return 1;  // Connection failed
    }

    if (!winsock.sendData("Hello Server!")) {
        return 1;  // Sending data failed
    }

    std::string response = winsock.receiveData();
    std::cout << "Received from server: " << response << std::endl;

    winsock.cleanup();  // Clean up resources
    return 0;
}