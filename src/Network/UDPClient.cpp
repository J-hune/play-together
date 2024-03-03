#include "../../include/Network/UDPClient.h"

UDPClient::UDPClient() = default;

UDPClient::~UDPClient() {
    stop(); // Stop the client
}

int UDPClient::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}

bool UDPClient::initialize(const std::string &serverHostname, short serverPort, unsigned short clientPort) {
    // Create UDP socket
    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFileDescriptor == -1) {
        perror("UDPClient: Error during socket creation");
        return false;
    }

    // Client address structure
    struct sockaddr_in clientAddr = {};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available local address
    clientAddr.sin_port = htons(clientPort); // Set the client port

    // Bind the socket to the client port
    if (bind(socketFileDescriptor, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
        perror("UDPClient: Error during binding");
        return false;
    }

    // Server address structure
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverHostname.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("UDPClient: Invalid address/ Address not supported");
        return false;
    }

    // No need to connect in UDP, just remember the server address for sending
    this->serverAddress = serverAddr;
    return true;
}

void UDPClient::start() {
    handleMessages(); // Start handling incoming messages (blocking)
    std::cout << "UDPClient: Client shutdown" << std::endl;
}

void UDPClient::handleMessages() {
    std::cout << "UDPClient: Handling incoming messages..." << std::endl;
    while (!stopRequested) {
        std::string receivedMessage = receive(200);
        if (!receivedMessage.empty()) {
            std::cout << "UDPClient: Received message: " << receivedMessage << std::endl;
        }
    }

    std::cout << "UDPClient: Stopping message handling" << std::endl;
}

bool UDPClient::send(const std::string &message) const {
    std::cout << "UDPClient: Sending message: " << message << " (" << message.length() << " bytes)" << std::endl;

    std::cout << "UDPClient: Sending message to " << inet_ntoa(serverAddress.sin_addr) << ":" << ntohs(serverAddress.sin_port) << std::endl;
    if (sendto(socketFileDescriptor, message.c_str(), message.length(), 0, (const sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("UDPClient: Error sending message");
        return false;
    }

    return true;
}

std::string UDPClient::receive(int timeoutMilliseconds) const {
    char buffer[1024];
    socklen_t serverLen = sizeof(sockaddr_in);

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
        perror("UDPClient: Error in select()");
        return "";
    } else if (ready == 0) {
        // No data available within the specified timeout, return an empty string
        return "";
    } else {
        // Data is available for reading, receive the data
        ssize_t bytesRead = recvfrom(socketFileDescriptor, buffer, sizeof(buffer), 0, nullptr, &serverLen);
        if (bytesRead == -1) {
            if (!stopRequested) perror("UDPClient: Error receiving message");
            return "";
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data
        return std::string(buffer);
    }
}

void UDPClient::stop() {
    // No need to disconnect in UDP
    if (socketFileDescriptor != -1) {
        close(socketFileDescriptor);
        socketFileDescriptor = -1;
    }

    stopRequested = true; // Set the stop flag to terminate the message handling loop
}

