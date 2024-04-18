#ifndef _WIN32

#include "../../../include/Network/Unix/UDPServer.h"

/** CONSTRUCTORS **/

UDPServer::UDPServer() = default;


/** ACCESSORS **/

int UDPServer::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}


/** METHODS **/

void UDPServer::initialize(short port) {
    // Create socket
    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFileDescriptor == -1) {
        throw UDPSocketCreationError("UDPServer: Error during socket creation");
    }

    // Bind socket to address
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    if (bind(socketFileDescriptor, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        throw UDPSocketBindError("UDPServer: Error during bind");
    }
}

void UDPServer::start(std::map<int, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex) {
    clientAddressesPtr = &clientAddresses;
    clientAddressesMutexPtr = &clientAddressesMutex;

    stopRequested = false;
    handleMessage();
    std::cout << "UDPServer: Server shutdown" << std::endl;
}

bool UDPServer::send(const sockaddr_in& clientAddress, const std::string &message) const {
#ifdef DEVELOPMENT_MODE
    std::cout << "UDPServer: Sending message: " << message << " (" << message.length() << " bytes)" << std::endl;
#endif

    if (sendto(socketFileDescriptor, message.c_str(), message.length(), 0, (const sockaddr*)&clientAddress, sizeof(clientAddress)) == -1) {
        perror("UDPServer: Error sending message");
        return false;
    }

    return true;
}

bool UDPServer::broadcast(const std::string &message, int socketIgnored) const {
#ifdef DEVELOPMENT_MODE
    std::cout << "UDPServer: Broadcasting message: " << message << " (" << message.length() << " bytes)" << std::endl;
#endif

    clientAddressesMutexPtr->lock();
    for (const auto& [id, address] : *clientAddressesPtr) {
        if (id == socketIgnored) continue;
        if (!send(address, message)) {
            clientAddressesMutexPtr->unlock();

            return false;
        }
    }
    clientAddressesMutexPtr->unlock();

    return true; // Placeholder
}

std::string UDPServer::receive(sockaddr_in& clientAddress, int timeoutMilliseconds) const {
    char buffer[1024];
    socklen_t clientLen = sizeof(clientAddress);

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(socketFileDescriptor, &readSet);

    // Specify timeout
    struct timeval timeout = {};
    timeout.tv_sec = timeoutMilliseconds / 1000;
    timeout.tv_usec = (timeoutMilliseconds % 1000) * 1000;

    // Wait until data is available or timeout expires
    int ready = select(socketFileDescriptor + 1, &readSet, nullptr, nullptr, &timeout);
    if (ready == -1) {
        perror("UDPServer: Error in select()");
        return "";
    } else if (ready == 0) {
        // No data available within the specified timeout, return an empty string
        return "";
    } else {
        // Data is available for reading, receive the data
        ssize_t bytesRead = recvfrom(socketFileDescriptor, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddress, &clientLen);
        if (bytesRead == -1) {
            if(!stopRequested) perror("UDPServer: Error receiving message");
            return "";
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data
        return buffer;
    }
}

void UDPServer::handleMessage() {
    std::cout << "UDPServer: Handling incoming messages..." << std::endl;

    while (!stopRequested && socketFileDescriptor != -1) {
        // Receive message with timeout
        struct sockaddr_in clientAddress;
        std::string message = receive(clientAddress, 200); // 1 second timeout

        // Handle the received message
        if (!message.empty()) {
            // Identify the client with the address (find)
            clientAddressesMutexPtr->lock();
            int clientID = -1;
            for (const auto& [id, address] : *clientAddressesPtr) {
                if (address.sin_addr.s_addr == clientAddress.sin_addr.s_addr && address.sin_port == clientAddress.sin_port) {
                    clientID = id;
                    break;
                }
            }
            clientAddressesMutexPtr->unlock();

            if (clientID != -1) {
                // Handle received message
                Mediator::handleMessages(1, message, clientID);
            } else {
            #ifdef DEVELOPMENT_MODE
                std::cout << "UDPServer: Received message: " << message << " from unknown client" << std::endl;
            #endif
            }
        }
    }

    std::cout << "UDPServer: Stopping message handling" << std::endl;
}

bool UDPServer::sendSyncCorrection(int clientSocket, sockaddr_in address, nlohmann::json &message) const {
    using json = nlohmann::json;

    // Add each player's position and movement to the message
    message["players"] = json::array();

    for (const Player &player : Mediator::getAlivePlayers()) {
        json playerData;

        int playerID = player.getPlayerID();
        if (playerID == -1) playerID = 0; // The server player has ID 0
        if (playerID == clientSocket) playerID = -1; // The client itself has ID -1

        playerData["playerID"] = playerID;
        playerData["x"] = player.getX();
        playerData["y"] = player.getY();
        playerData["moveX"] = player.getMoveX();
        playerData["moveY"] = player.getMoveY();
        message["players"].push_back(playerData);
    }

    return send(address, message.dump());
}

// Stop the server
void UDPServer::stop() {
    stopRequested = true;
    if (socketFileDescriptor != -1) {
        ::shutdown(socketFileDescriptor, SHUT_RDWR);
        close(socketFileDescriptor);
        socketFileDescriptor = -1;

        std::cout << "UDPServer: Server socket closed" << std::endl;
    }
}

#endif // _WIN32