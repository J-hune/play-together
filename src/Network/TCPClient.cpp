#include "../../include/Network/TCPClient.h"

TCPClient::TCPClient() = default;

TCPClient::~TCPClient() {
    disconnect(); // Disconnect from the server
    stop(); // Stop the client
}

int TCPClient::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}

void TCPClient::setDisconnectCallback(std::function<void()> callback) {
    disconnectCallback = std::move(callback);
}

bool TCPClient::connect(const std::string &serverAddress, short port) {
    // Create TCP socket
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1) {
        perror("TCPClient: Error during socket creation");
        return false;
    }

    // Server address structure
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("TCPClient: Invalid address/ Address not supported");
        return false;
    }

    // Connect to the server
    if (::connect(socketFileDescriptor, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("TCPClient: Connection failed");
        return false;
    }

    return true;
}

void TCPClient::start() {
    handleMessages(); // Start handling incoming messages (blocking)
    std::cout << "TCPClient: Stopping client" << std::endl;
}


void TCPClient::handleMessages() {
    std::cout << "TCPClient: Handling incoming messages..." << std::endl;
    while (!stopRequested) {
        std::string receivedMessage = receive();
        if (!receivedMessage.empty()) {
            std::cout << "TCPClient: Received message: " << receivedMessage << std::endl;

            if (receivedMessage == "DISCONNECT") {
                std::cout << "TCPClient: Server asked to disconnect" << std::endl;
                if (socketFileDescriptor != -1) {
                    close(socketFileDescriptor);
                    socketFileDescriptor = -1;
                }
                disconnectCallback(); // Invoke the disconnect callback
            }

        } else {
            // Handle disconnection or receive error cases
            std::cerr << "Error receiving message or server disconnected." << std::endl;
            break;
        }
    }

    std::cout << "TCPClient: Stopping message handling" << std::endl;
}

bool TCPClient::send(const std::string &message) const {
    std::cout << "TCPClient: Sending message: " << message << " (" << message.length() << " bytes) to server (socket " << socketFileDescriptor << ")" << std::endl;

    // First, send the size of the message
    auto messageSize = static_cast<int>(message.length()); // Message size including null terminator
    if (::send(socketFileDescriptor, &messageSize, sizeof(int), 0) == -1) {
        perror("TCPClient: Error sending message size");
        return false;
    }

    // Then, send the message itself
    const char *messagePtr = message.c_str();
    ssize_t totalBytesSent = 0;
    while (totalBytesSent < messageSize) {
        ssize_t bytesSent = ::send(socketFileDescriptor, messagePtr + totalBytesSent, messageSize - totalBytesSent, 0);
        if (bytesSent == -1) {
            perror("TCPClient: Error sending message");
            return false;
        }
        totalBytesSent += bytesSent;
    }

    return true;
}

std::string TCPClient::receive() const {
    char sizeBuffer[sizeof(int)];
    ssize_t bytesRead;

    // Receive the size of the message
    bytesRead = recv(socketFileDescriptor, sizeBuffer, sizeof(sizeBuffer), 0);
    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            return ""; // No data available, return empty string
        } else {
            // Error or connection closed by server
            perror("TCPClient: Error receiving message size");
            return "";
        }
    }

    int messageSize;
    memcpy(&messageSize, sizeBuffer, sizeof(int));

    std::string receivedData;
    receivedData.resize(messageSize);

    size_t totalBytesReceived = 0;
    while (totalBytesReceived < static_cast<size_t>(messageSize)) {
        bytesRead = recv(socketFileDescriptor, &receivedData[totalBytesReceived], messageSize - totalBytesReceived, 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                return ""; // No data available, return empty string
            } else {
                // Error or connection closed by server
                perror("TCPClient: Error receiving message data");
                return "";
            }
        }
        totalBytesReceived += bytesRead;
    }

    return receivedData;
}

void TCPClient::disconnect() {
    // Send a message to the server to initiate disconnection
    if (socketFileDescriptor != -1) {
        send("DISCONNECT");
        close(socketFileDescriptor);
        socketFileDescriptor = -1;
    }
}

void TCPClient::stop() {
    stopRequested = true; // Set the stop flag to terminate the message handling loop
}
