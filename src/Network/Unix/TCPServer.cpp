#ifndef _WIN32

#include "../../../include/Network/Unix/TCPServer.h"

/*
    The TCP server listens on a specified port and accepts incoming connections from clients. For each
    connected client, a new thread is created to handle incoming and outgoing messages.

    The server can receive variable-sized messages from clients. Each message starts with the message size
    (an integer) followed by the message content. This structure allows for reliable message handling and
    determining the length of the received message.

    When a client disconnects, the server closes the associated connection and removes the client from its
    list of connected clients.

    The server uses exceptions to handle errors during socket creation, binding, and read/write operations.

    Thread Operation:
    - The server creates a dedicated thread for each connected client to handle incoming and outgoing
    messages asynchronously.
    - Each client thread constantly monitors incoming messages and processes the received data according
    to the application's needs.
 */

/** CONSTRUCTORS **/

TCPServer::TCPServer() = default;


/** ACCESSORS **/

// Get the socket file descriptor
int TCPServer::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}


/** METHODS **/

// Initialize the server with a specified port
void TCPServer::initialize(short port) {
    // Create socket
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1) {
        throw TCPSocketCreationError("TCPServer: Error during socket creation");
    }

    // Bind socket to address
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    if (bind(socketFileDescriptor, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        throw TCPSocketBindError("TCPServer: Error during bind");
    }

    // Listen for incoming connections
    if (listen(socketFileDescriptor, 5) == -1) {
        throw TCPSocketListenError("TCPServer: Error during listen");
    }
}

// Start the server
void TCPServer::start(std::map<int, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex) {
    clientAddressesPtr = &clientAddresses;
    clientAddressesMutexPtr = &clientAddressesMutex;

    stopRequested = false;
    acceptConnections();
    clearResources();
    std::cout << "TCPServer: Server shutdown" << std::endl;
}

// Accept incoming client connections
void TCPServer::acceptConnections() {
    std::cout << "TCPServer: Handling incoming connections..." << std::endl;

    while (!stopRequested.load()) {
        int clientSocket = waitForConnection();
        if (clientSocket != -1) {
            // Create a new thread to handle messages from the client
            std::jthread clientThread(&TCPServer::handleMessage, this, clientSocket);
            clientThreads.push_back(std::move(clientThread));
        }
    }

    std::cout << "TCPServer: Stopping connection handling" << std::endl;
}

// Wait for a connection from a client
int TCPServer::waitForConnection() {
    struct sockaddr_in clientAddr = {};
    socklen_t clientLen = sizeof(clientAddr);

    std::cout << "TCPServer: Waiting for new client connection" << std::endl;
    int clientSocket = accept(socketFileDescriptor, (struct sockaddr *) &clientAddr, &clientLen);
    if (clientSocket == -1) return -1;

    // Check if the maximum number of clients has been reached, if so, close the connection
    clientAddressesMutexPtr->lock();
    if (clientAddressesPtr->size() >= maxClients) {
        clientAddressesMutexPtr->unlock();

        send(clientSocket, "DISCONNECT");
        close(clientSocket);
        std::cout << "TCPServer: Maximum number of clients reached" << std::endl;
        return -1;
    }
    clientAddressesMutexPtr->unlock();

    std::string clientIp = inet_ntoa(clientAddr.sin_addr);
    std::cout << "TCPServer: New client connected from " << clientIp << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // Add the client to the list of connected clients
    clientAddressesMutexPtr->lock();
    clientAddressesPtr->insert({clientSocket, clientAddr});
    clientAddressesMutexPtr->unlock();
    std::cout << "TCPServer: New client connected with ID: " << clientSocket << std::endl;

    // Notify the mediator of the new client connection
    Mediator::handleClientConnect(clientSocket);
    sendGameProperties(clientSocket);
    relayClientConnection(clientSocket);

    return clientSocket;
}

// Handle messages from a client
void TCPServer::handleMessage(int clientSocket) {
    bool clientConnected = true;
    while (clientConnected) {
        try {
            // Check if the client is still connected (if the clientFileDescriptor is still in the list of connected clients)
            clientAddressesMutexPtr->lock();
            clientConnected = clientAddressesPtr->contains(clientSocket);
            clientAddressesMutexPtr->unlock();

            if (clientConnected) {
                std::cout << "TCPServer: Waiting for message from client " << clientSocket << std::endl;
                std::string message = receive(clientSocket);

                if (message == "DISCONNECT") {
                    std::cout << "TCPServer: Client " << clientSocket << " asked to disconnect" << std::endl;
                    clientConnected = false;
                } else if (!message.empty()) {
                    // Handle received message
                    Mediator::handleMessages(0, message, clientSocket);
                } else {
                    std::cout << "TCPServer: Client " << clientSocket << " disconnected" << std::endl;
                    clientConnected = false;
                }
            }
        } catch (const TCPSocketReceiveError& e) {
            std::cerr << e.what() << std::endl;
            // Exit the loop on error
            clientConnected = false;
        }
    }

    std::cout << "TCPServer: Client " << clientSocket << " disconnected" << std::endl;

    // Notify the mediator of the client disconnection
    Mediator::handleClientDisconnect(clientSocket);
    relayClientDisconnection(clientSocket);

    close(clientSocket);

    // Remove the client from the list of connected clients
    clientAddressesMutexPtr->lock();
    clientAddressesPtr->erase(clientSocket);
    clientAddressesMutexPtr->unlock();
}

// Send a message to a client
bool TCPServer::send(int clientSocket, const std::string &message) const {
#ifdef DEVELOPMENT_MODE
    std::cout << "TCPServer: Sending message: " << message << " (" << message.length() << " bytes) to client " << clientSocket << std::endl;
#endif

    // First, send the size of the message
    auto messageSize = static_cast<int>(message.length()); // Message size including null terminator
    if (::send(clientSocket, &messageSize, sizeof(int), 0) == -1) {
        perror("TCPServer: Error sending message size");
        return false;
    }

    // Then, send the message itself
    const char *messagePtr = message.c_str();
    ssize_t totalBytesSent = 0;
    while (totalBytesSent < messageSize) {
        ssize_t bytesSent = ::send(clientSocket, messagePtr + totalBytesSent, messageSize - totalBytesSent, 0);
        if (bytesSent == -1) {
            perror("TCPServer: Error sending message");
            return false;
        }
        totalBytesSent += bytesSent;
    }

    return true;
}

// Broadcast a message to all connected clients
bool TCPServer::broadcast(const std::string &message, int socketIgnored) const {
    clientAddressesMutexPtr->lock();
#ifdef DEVELOPMENT_MODE
    std::cout << "TCPServer: Broadcasting message: " << message << " (" << message.length() << " bytes) to " << clientAddressesPtr->size() << " clients" << std::endl;
#endif

    auto send_successful = std::ranges::all_of(*clientAddressesPtr, [&](const auto& client) {
        if (client.first == socketIgnored) return true;
        return send(client.first, message);
    });

    clientAddressesMutexPtr->unlock();
    return send_successful;
}

// Receive a message from a client
std::string TCPServer::receive(int clientSocket) const {
    char sizeBuffer[sizeof(int)];
    ssize_t bytesRead;

    // Receive the size of the message
    bytesRead = recv(clientSocket, sizeBuffer, sizeof(sizeBuffer), MSG_WAITALL);
    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            // No data available, return empty string
            return "";
        } else {
            // Error or connection closed by client
            throw TCPSocketReceiveError("TCPServer: Error receiving message size, client disconnected");
        }
    }

    int messageSize;
    memcpy(&messageSize, sizeBuffer, sizeof(int));

    std::string receivedData;
    receivedData.resize(messageSize);

    size_t totalBytesReceived = 0;
    while (totalBytesReceived < static_cast<size_t>(messageSize)) {
        bytesRead = recv(clientSocket, &receivedData[totalBytesReceived], messageSize - totalBytesReceived, 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                // No data available, return empty string
                return "";
            } else {
                // Error or connection closed by client
                throw TCPSocketReceiveError("TCPServer: Error receiving message data, client disconnected");
            }
        }
        totalBytesReceived += bytesRead;
    }

    return receivedData;
}

bool TCPServer::sendGameProperties(int clientSocket) const {
    using json = nlohmann::json;

    // Create JSON message
    json message;
    message["messageType"] = "gameProperties";
    Mediator::getGameProperties(message);

    // Add all connected clients to the player list
    message["players"] = json::array();
    for (std::vector<Player> players = Mediator::getAlivePlayers(); const auto &player : players) {
        int playerID = player.getPlayerID();
        if (playerID == -1) playerID = 0; // The server player has ID 0
        if (playerID == clientSocket) playerID = -1; // The client itself has ID -1

        // Replace the client socket with -1 for the client itself
        json playerInfo;
        playerInfo["playerID"] = playerID;
        playerInfo["x"] = player.getX();
        playerInfo["y"] = player.getY();
        playerInfo["moveX"] = player.getMoveX();
        playerInfo["moveY"] = player.getMoveY();

        message["players"].push_back(playerInfo);
    }

    return send(clientSocket, message.dump());
}

bool TCPServer::relayClientConnection(int clientSocket) const {
    using json = nlohmann::json;

    json message;
    message["messageType"] = "playerConnect";
    message["playerID"] = clientSocket;

    return broadcast(message.dump(), clientSocket);
}

bool TCPServer::relayClientDisconnection(int clientSocket) const {
    using json = nlohmann::json;

    json message;
    message["messageType"] = "playerDisconnect";
    message["playerID"] = clientSocket;

    return broadcast(message.dump(), clientSocket);
}

// Stop the server
void TCPServer::stop() {
    // Send a disconnect message to all connected clients
    if (clientAddressesMutexPtr && clientAddressesMutexPtr->try_lock()) {
        if (!clientAddressesPtr->empty()) {
            clientAddressesMutexPtr->unlock();
            std::cout << "TCPServer: Sending disconnect message to all clients" << std::endl;
            broadcast("DISCONNECT", 0);
        } else {
            clientAddressesMutexPtr->unlock();
        }
    }

    // Stop message handling
    stopRequested = true;
    if (socketFileDescriptor != -1) {
        ::shutdown(socketFileDescriptor, SHUT_RDWR);
        close(socketFileDescriptor);
        socketFileDescriptor = -1;

        std::cout << "TCPServer: Server socket closed" << std::endl;
    }
}

void TCPServer::clearResources() {
    // Close all client connections (clientAddressPtr)
    clientAddressesMutexPtr->lock();
    for (const auto& [clientSocket, _]: *clientAddressesPtr) {
        ::shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
    }
    clientAddressesPtr->clear();
    clientAddressesMutexPtr->unlock();

    std::cout << "TCPServer: Closing all client connections..." << std::endl;
    for (std::jthread& clientThread: clientThreads) {
        if (clientThread.joinable()) {
            clientThread.request_stop();
            clientThread.join();
        }
    }
    std::cout << "TCPServer: " << clientThreads.size() << " client threads stopped" << std::endl;

    clientThreads.clear();
}

#endif // !_WIN32