#ifndef _WIN32

#include "../../../include/Network/Unix/TCPClient.h"

/** CONSTRUCTORS **/

TCPClient::TCPClient() = default;


/** ACCESSORS **/

int TCPClient::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}


/** MODIFIERS **/

void TCPClient::setDisconnectCallback(std::function<void()> callback) {
    disconnectCallback = std::move(callback);
}


/** METHODS **/

void TCPClient::connect(const std::string &serverAddress, short port, unsigned short& clientPort) {
    // Create temporary file descriptor for the socket
    // (We don't want to modify the socketFileDescriptor if the connection fails)
    int socketFileDescriptorTest = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptorTest == -1) {
        throw TCPSocketCreationError("TCPClient: Error during socket creation");
    }

    // Server address structures
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        throw TCPSocketCreationError("TCPClient: Invalid address/ Address not supported");
    }

    // Connect to the server
    if (::connect(socketFileDescriptorTest, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        throw TCPConnectionError("TCPClient: Unable to connect to server");
    }

    // Get the local port that was chosen for the connection
    struct sockaddr_in localAddr = {};
    if (socklen_t addrLen = sizeof(localAddr); getsockname(socketFileDescriptorTest, (struct sockaddr *) &localAddr, &addrLen) == -1) {
        throw TCPConnectionError("TCPClient: Unable to get local port");
    }

    // Store the chosen port
    clientPort = ntohs(localAddr.sin_port);

    // If the connection is successful, update the socketFileDescriptor
    socketFileDescriptor = socketFileDescriptorTest;
}

void TCPClient::start() {
    stopRequested = false;
    shouldSendDisconnect = true; // Reset the flag to send a disconnect message
    handleMessages(); // Start handling incoming messages (blocking)
    std::cout << "TCPClient: Client shutdown" << std::endl;
}

void TCPClient::handleMessages() {
    std::cout << "TCPClient: Handling incoming messages..." << std::endl;
    while (!stopRequested) {
        std::string receivedMessage = receive();
        if (!receivedMessage.empty()) {
            if (receivedMessage == "DISCONNECT") {
                std::cout << "TCPClient: Server asked to disconnect" << std::endl;
                shouldSendDisconnect = false; // Do not send a disconnect message
                stop(); // Stop the client
                disconnectCallback(); // Invoke the disconnect callback
            } else {
                // Handle received message
                Mediator::handleMessages(0, receivedMessage, 0);
            }
        } else {
            // Handle disconnection or receive error cases
            if (!stopRequested) std::cerr << "TCPClient: Error receiving message or server disconnected." << std::endl;
            break;
        }
    }

    std::cout << "TCPClient: Stopping message handling" << std::endl;
}

bool TCPClient::send(const std::string &message) const {
#ifdef DEVELOPMENT_MODE
    std::cout << "TCPClient: Sending message: " << message << " (" << message.length() << " bytes) to server (socket " << socketFileDescriptor << ")" << std::endl;
#endif

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

void TCPClient::stop() {
    stopRequested = true; // Set the stop flag to terminate the message handling loop

    // Send a message to the server to initiate disconnection
    if (socketFileDescriptor != -1) {
        if (shouldSendDisconnect) send("DISCONNECT");
        ::shutdown(socketFileDescriptor, SHUT_RDWR);
        close(socketFileDescriptor);
        socketFileDescriptor = -1;

        std::cout << "TCPClient: Socket closed" << std::endl;
    }
}

#endif // !_WIN32