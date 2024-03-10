#include "../../include/Network/NetworkManager.h"

NetworkManager::NetworkManager(Mediator *mediator) : mediatorPtr(mediator) {
    tcpClient.setDisconnectCallback([this] {
        mediatorPtr->handleServerDisconnect();
        udpClient.stop();
    });
};

NetworkManager::~NetworkManager() {
    stopServers();
    stopClients();
}

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
    if (tcpServer.getSocketFileDescriptor() != -1) tcpServer.stop();
    if (serverTCPThreadPtr && serverTCPThreadPtr->joinable()) {
        serverTCPThreadPtr->request_stop();
        serverTCPThreadPtr->join();
    }
    serverTCPThreadPtr.reset();

    if (udpServer.getSocketFileDescriptor() != -1) udpServer.stop();
    if (serverUDPThreadPtr && serverUDPThreadPtr->joinable()) {
        serverUDPThreadPtr->request_stop();
        serverUDPThreadPtr->join();
    }
    serverUDPThreadPtr.reset();
}

void NetworkManager::stopClients() {
    if (tcpClient.getSocketFileDescriptor() != -1) tcpClient.stop();
    if (clientTCPThreadPtr && clientTCPThreadPtr->joinable()) {
        clientTCPThreadPtr->request_stop();
        clientTCPThreadPtr->join();
    }
    clientTCPThreadPtr.reset();

    if (udpClient.getSocketFileDescriptor() != -1) udpClient.stop();
    if (clientUDPThreadPtr && clientUDPThreadPtr->joinable()) {
        clientUDPThreadPtr->request_stop();
        clientUDPThreadPtr->join();
    }
    clientUDPThreadPtr.reset();
}

void NetworkManager::temporarySendMethod(const std::string &message) const {
    if (message.empty()) {
        std::cerr << "Message is empty" << std::endl;
        return;
    }

#ifdef _WIN32
    if (tcpClient.getSocketFileDescriptor() != INVALID_SOCKET) {
        tcpClient.send("Hello, World from TCP!");
    }

    if (udpClient.getSocketFileDescriptor() != INVALID_SOCKET) {
        udpClient.send("Hello, World from UDP!");
    }

    if (tcpServer.getSocketFileDescriptor() != INVALID_SOCKET) {
        bool success = tcpServer.broadcast("Hello, World!");
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }

    if (udpServer.getSocketFileDescriptor() != INVALID_SOCKET) {
        bool success = udpServer.broadcast("Hello, World!");
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }
#else
    if (tcpClient.getSocketFileDescriptor() != -1) {
        tcpClient.send(message);
    }

    if (udpClient.getSocketFileDescriptor() != -1) {
        udpClient.send(message);
    }

    if (tcpServer.getSocketFileDescriptor() != -1) {
        bool success = tcpServer.broadcast("Hello, World!");
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }

    if (udpServer.getSocketFileDescriptor() != -1) {
        bool success = udpServer.broadcast("Hello, World!");
        if (success) {
            std::cout << "Message sent to all clients" << std::endl;
        } else {
            std::cerr << "Failed to send message to all clients" << std::endl;
        }
    }
#endif
}
