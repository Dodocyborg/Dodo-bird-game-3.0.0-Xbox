#ifndef NETWORKING_H
#define NETWORKING_H

class Networking {
public:
    void initialize();    // Initializes the networking system
    void syncState();     // Syncs game state with server
    void cleanup();       // Cleans up the networking resources
};

#endif // NETWORKING_H