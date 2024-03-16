#ifndef _WIN32
#define SOCKET_VALID(fd) (fd != -1)
#else
#define SOCKET_VALID(fd) (fd != INVALID_SOCKET)
#endif

#include "../../include/Network/NetworkManager.h"

/** CONSTRUCTORS **/

NetworkManager::NetworkManager() {
    tcpClient.setDisconnectCallback([this] {
        Mediator::handleServerDisconnect();
        udpClient.stop();
    });
}


/** ACCESSORS **/

bool NetworkManager::isServer() const {
    return SOCKET_VALID(tcpServer.getSocketFileDescriptor()) && SOCKET_VALID(udpServer.getSocketFileDescriptor());
}


/** METHODS **/

void NetworkManager::startServers() {
    try {
        tcpServer.initialize(8080);
        std::cout << "TCPServer: Server initialized and listening on port 8080" << std::endl;

        // Start the server in a separate thread
        serverTCPThreadPtr = std::make_unique<std::jthread>([this](TCPServer *serverPtr) {
            serverPtr->start(clientAddresses, clientAddressesMutex);
        }, &tcpServer);

        udpServer.initialize(8080);
        std::cout << "UDPServer: Server initialized and listening on port 8080" << std::endl;

        // Start the server in a separate thread
        serverUDPThreadPtr = std::make_unique<std::jthread>([this](UDPServer *serverPtr) {
            serverPtr->start(clientAddresses, clientAddressesMutex);
        }, &udpServer);
    } catch (const NetworkError &) {
        stopServers();
        throw;
    }
}

void NetworkManager::startClients() {
    unsigned short clientPort;
    try {
        tcpClient.connect("127.0.0.1", 8080, clientPort);
        std::cout << "TCPClient: Connected to server" << std::endl;
        clientTCPThreadPtr = std::make_unique<std::jthread>(&TCPClient::start, &tcpClient);

        udpClient.initialize("127.0.0.1", 8080, clientPort);
        std::cout << "UDPClient: Client initialized and running on port " << clientPort << std::endl;
        clientUDPThreadPtr = std::make_unique<std::jthread>(&UDPClient::start, &udpClient);
    } catch (const NetworkError &) {
        stopClients();
        throw;
    }
}

void NetworkManager::stopServers() {
    if (SOCKET_VALID(tcpServer.getSocketFileDescriptor())) tcpServer.stop();
    if (SOCKET_VALID(udpServer.getSocketFileDescriptor())) udpServer.stop();

    if (serverTCPThreadPtr && serverTCPThreadPtr->joinable()) {
        serverTCPThreadPtr->request_stop();
        serverTCPThreadPtr->join();
    }
    serverTCPThreadPtr.reset();

    if (serverUDPThreadPtr && serverUDPThreadPtr->joinable()) {
        serverUDPThreadPtr->request_stop();
        serverUDPThreadPtr->join();
    }
    serverUDPThreadPtr.reset();
}

void NetworkManager::stopClients() {
    if (SOCKET_VALID(tcpClient.getSocketFileDescriptor())) tcpClient.stop();
    if (SOCKET_VALID(udpClient.getSocketFileDescriptor())) udpClient.stop();

    if (clientTCPThreadPtr && clientTCPThreadPtr->joinable()) {
        clientTCPThreadPtr->request_stop();
        clientTCPThreadPtr->join();
    }
    clientTCPThreadPtr.reset();

    if (clientUDPThreadPtr && clientUDPThreadPtr->joinable()) {
        clientUDPThreadPtr->request_stop();
        clientUDPThreadPtr->join();
    }
    clientUDPThreadPtr.reset();
}

void NetworkManager::broadcastMessage(int protocol, const std::string &message, int socketIgnored) const {
    if (message.empty()) {
        std::cerr << "Message is empty" << std::endl;
        return;
    }

    if (protocol == 0) {
        if (SOCKET_VALID(tcpServer.getSocketFileDescriptor())) {
            bool success = tcpServer.broadcast(message, socketIgnored);
            if (success) {
                std::cout << "Message sent to all clients" << std::endl;
            } else {
                std::cerr << "Failed to send message to all clients" << std::endl;
            }
        }
    } else if (protocol == 1) {
        if (SOCKET_VALID(udpServer.getSocketFileDescriptor())) {
            bool success = udpServer.broadcast(message, socketIgnored);
            if (success) {
                std::cout << "Message sent to all clients" << std::endl;
            } else {
                std::cerr << "Failed to send message to all clients" << std::endl;
            }
        }
    } else {
        std::cerr << "Invalid protocol" << std::endl;
    }
}

void NetworkManager::temporarySendMethod(const std::string &message) const {
    if (message.empty()) {
        std::cerr << "Message is empty" << std::endl;
        return;
    }

    if (SOCKET_VALID(tcpClient.getSocketFileDescriptor())) {
        tcpClient.send(message);
    }

    if (SOCKET_VALID(udpClient.getSocketFileDescriptor())) {
        udpClient.send(message);
    }

    if (SOCKET_VALID(tcpServer.getSocketFileDescriptor())) {
        bool success = tcpServer.broadcast("Hello, World!", 0);
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }

    if (SOCKET_VALID(udpServer.getSocketFileDescriptor())) {
        bool success = udpServer.broadcast("Hello, World!", 0);
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }
}

void NetworkManager::sendPlayerUpdate(uint16_t keyboardStateMask) const {

    // Create a message with the player update
    using json = nlohmann::json;
    json message;

    message["messageType"] = "playerUpdate";
    message["keyboardStateMask"] = keyboardStateMask;

    if (isServer()) {
        udpServer.broadcast(message.dump(), 0);
    } else {
        udpClient.send(message.dump());
    }
}