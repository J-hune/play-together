#ifdef _WIN32

#include "../../../include/Network/WIN32/TCPServer.h"

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
SOCKET TCPServer::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}


/** METHODS **/

// Initialize the server with a specified port
void TCPServer::initialize(short port) {
    // Create socket
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == INVALID_SOCKET) {
        std::cerr << "TCPServer: Error during socket creation - " << WSAGetLastError() << std::endl;
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
void TCPServer::start(std::map<SOCKET, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex) {
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
        SOCKET clientSocket = waitForConnection();
        if (clientSocket != INVALID_SOCKET) {
            // Create a new thread to handle messages from the client
            std::jthread clientThread(&TCPServer::handleMessage, this, clientSocket);
            clientThreads.push_back(std::move(clientThread));
        }
    }

    std::cout << "TCPServer: Stopping connection handling" << std::endl;
}

// Wait for a connection from a client
SOCKET TCPServer::waitForConnection() {
    struct sockaddr_in clientAddr = {};
    socklen_t clientLen = sizeof(clientAddr);

    // Check if the maximum number of clients has been reached
    clientAddressesMutexPtr->lock();
    if (clientAddressesPtr->size() >= maxClients) {
        std::cout << "TCPServer: Maximum number of clients reached" << std::endl;
        return INVALID_SOCKET;
    }
    clientAddressesMutexPtr->unlock();

    std::cout << "TCPServer: Waiting for new client connection" << std::endl;
    SOCKET clientSocket = accept(socketFileDescriptor, (struct sockaddr *) &clientAddr, &clientLen);

    if (clientSocket == INVALID_SOCKET) return INVALID_SOCKET;

    std::string clientIp = inet_ntoa(clientAddr.sin_addr);
    std::cout << "TCPServer: New client connected from " << clientIp << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // Add the client to the list of connected clients
    clientAddressesMutexPtr->lock();
    clientAddressesPtr->insert({clientSocket, clientAddr});
    clientAddressesMutexPtr->unlock();
    std::cout << "TCPServer: New client connected with ID: " << clientSocket << std::endl;

    // Notify the mediator of the new client connection
    Mediator::handleClientConnect((int)clientSocket);
    sendGameProperties(clientSocket);
    sendPlayerList(clientSocket);
    relayClientConnection(clientSocket);

    return clientSocket;
}

// Handle messages from a client
void TCPServer::handleMessage(SOCKET clientSocket) {
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

    closesocket(clientSocket);

    // Remove the client from the list of connected clients
    clientAddressesMutexPtr->lock();
    clientAddressesPtr->erase(clientSocket);
    clientAddressesMutexPtr->unlock();
}

// Send a message to a client
bool TCPServer::send(SOCKET clientSocket, const std::string &message) const {
    std::cout << "TCPServer: Sending message: " << message << " (" << message.length() << " bytes) to client " << clientSocket << std::endl;

    // First, send the size of the message
    int messageSize = message.length(); // Message size including null terminator
    if (::send(clientSocket, reinterpret_cast<const char *>(&messageSize), sizeof(int), 0) == -1) {
        std::cerr << "TCPServer: Error sending message size: " << WSAGetLastError() << std::endl;
        return false;
    }

    // Then, send the message itself
    const char *messagePtr = message.c_str();
    int totalBytesSent = 0;
    while (totalBytesSent < messageSize) {
        int bytesSent = ::send(clientSocket, messagePtr + totalBytesSent, static_cast<int>(messageSize - totalBytesSent), 0);
        if (bytesSent == -1) {
            std::cerr << "TCPServer: Error sending message: " << WSAGetLastError() << std::endl;
            return false;
        }
        totalBytesSent += bytesSent;
    }

    return true;
}

// Broadcast a message to all connected clients
bool TCPServer::broadcast(const std::string &message, SOCKET socketIgnored) const {
    clientAddressesMutexPtr->lock();
    std::cout << "TCPServer: Broadcasting message: " << message << " (" << message.length() << " bytes) to " << clientAddressesPtr->size() << " clients" << std::endl;

    auto send_successful = std::ranges::all_of(*clientAddressesPtr, [&](const auto& client) {
        if (client.first == socketIgnored) return true;
        return send(client.first, message);
    });

    clientAddressesMutexPtr->unlock();
    return send_successful;
}

// Receive a message from a client
std::string TCPServer::receive(SOCKET clientSocket) const {
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
            // Ignore if the client is not in the list of connected clients
            clientAddressesMutexPtr->lock();
            bool clientConnected = clientAddressesPtr->contains(clientSocket);
            clientAddressesMutexPtr->unlock();
            if (clientConnected) {
                throw TCPSocketReceiveError("TCPServer: Error receiving message size, client disconnected");
            } else {
                return "";
            }
        }
    }

    int messageSize;
    memcpy(&messageSize, sizeBuffer, sizeof(int));

    std::string receivedData;
    receivedData.resize(messageSize);

    size_t totalBytesReceived = 0;
    while (totalBytesReceived < static_cast<size_t>(messageSize)) {
        bytesRead = recv(clientSocket, &receivedData[totalBytesReceived], static_cast<int>(messageSize - totalBytesReceived), 0);
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

bool TCPServer::sendGameProperties(SOCKET clientSocket) const {
    using json = nlohmann::json;

    // Create JSON message
    json message;
    message["messageType"] = "gameProperties";
    Mediator::getGameProperties(message);

    return send(clientSocket, message.dump());
}

bool TCPServer::sendPlayerList(SOCKET clientSocket) const {
    using json = nlohmann::json;

    // Create JSON message
    json message;
    message["messageType"] = "playerList";
    message["players"] = json::array();

    // Add the server to the player list (ID 0)
    message["players"].push_back(0);

    // Lock the mutex to safely access client addresses
    clientAddressesMutexPtr->lock();

    // Add all connected clients to the player list
    for (const auto& [socket, _]: *clientAddressesPtr) {
        // Replace the client socket with -1 for the client itself
        message["players"].push_back((socket == clientSocket) ? -1 : socket);
    }

    // Unlock the mutex after accessing client addresses
    clientAddressesMutexPtr->unlock();

    return send(clientSocket, message.dump());
}

bool TCPServer::relayClientConnection(SOCKET clientSocket) const {
    using json = nlohmann::json;

    json message;
    message["messageType"] = "playerConnect";
    message["playerID"] = clientSocket;

    return broadcast(message.dump(), clientSocket);
}

bool TCPServer::relayClientDisconnection(SOCKET clientSocket) const {
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

    if (socketFileDescriptor != INVALID_SOCKET) {
        ::shutdown(socketFileDescriptor, SD_BOTH);
        closesocket(socketFileDescriptor);
        socketFileDescriptor = INVALID_SOCKET;

        std::cout << "TCPServer: Server socket closed" << std::endl;
    }
}

void TCPServer::clearResources() {
    // Close all client connections (clientAddressPtr)
    clientAddressesMutexPtr->lock();
    for (const auto& [clientSocket, _]: *clientAddressesPtr) {
        ::shutdown(clientSocket, SD_BOTH);
        closesocket(clientSocket);
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

#endif // _WIN32