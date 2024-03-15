#ifdef _WIN32

#include "../../../include/Network/WIN32/UDPClient.h"

/** CONSTRUCTORS **/

UDPClient::UDPClient() = default;


/** ACCESSORS **/

SOCKET UDPClient::getSocketFileDescriptor() const {
    return socketFileDescriptor;
}


/** METHODS **/

void UDPClient::initialize(const std::string &serverHostname, short serverPort, unsigned short clientPort) {
    // Create UDP socket
    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFileDescriptor == INVALID_SOCKET) {
        throw UDPSocketCreationError("UDPClient: Error during socket creation");
    }

    // Client address structure
    struct sockaddr_in clientAddr = {};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available local address
    clientAddr.sin_port = htons(clientPort); // Set the client port

    // Bind the socket to the client port
    if (bind(socketFileDescriptor, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
        std::cerr << "UDPClient: Error during bind: " << WSAGetLastError() << std::endl;
        throw UDPSocketBindError("UDPClient: Error during bind");
    }

    // Server address structure
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverHostname.c_str(), &serverAddr.sin_addr) <= 0) {
        throw UDPSocketCreationError("UDPClient: Invalid address/ Address not supported");
    }

    // No need to connect in UDP, just remember the server address for sending
    this->serverAddress = serverAddr;
}

void UDPClient::start() {
    handleMessages(); // Start handling incoming messages (blocking)
    std::cout << "UDPClient: Client shutdown" << std::endl;
}

void UDPClient::handleMessages() const {
    std::cout << "UDPClient: Handling incoming messages..." << std::endl;
    while (!stopRequested) {
        std::string receivedMessage = receive(200);

        if (!receivedMessage.empty()) {
            // Handle received message
            Mediator::handleMessages(1, receivedMessage, 0);
        }
    }

    std::cout << "UDPClient: Stopping message handling" << std::endl;
}

bool UDPClient::send(const std::string &message) const {
    std::cout << "UDPClient: Sending message: " << message << " (" << message.length() << " bytes)" << std::endl;

    std::cout << "UDPClient: Sending message to " << inet_ntoa(serverAddress.sin_addr) << ":" << ntohs(serverAddress.sin_port) << std::endl;
    if (sendto(socketFileDescriptor, message.c_str(), static_cast<int>(message.length()), 0, (const sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "UDPClient: Error sending message: " << WSAGetLastError() << std::endl;
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
    int ready = select(0, &readSet, nullptr, nullptr, &timeout);
    if (ready == -1) {
        std::cerr << "UDPClient: Error in select: " << WSAGetLastError() << std::endl;
        return "";
    } else if (ready == 0) {
        // No data available within the specified timeout, return an empty string
        return "";
    } else {
        // Data is available for reading, receive the data
        int bytesRead = recvfrom(socketFileDescriptor, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddress, &serverLen);
        if (bytesRead == -1) {
            if (!stopRequested) std::cerr << "UDPClient: Error receiving message: " << WSAGetLastError() << std::endl;
            return "";
        }

        buffer[bytesRead] = '\0'; // Null-terminate the received data
        return buffer;
    }
}

void UDPClient::stop() {
    // No need to disconnect in UDP
    if (socketFileDescriptor != INVALID_SOCKET) {
        ::shutdown(socketFileDescriptor, SD_BOTH);
        closesocket(socketFileDescriptor); // Close socket on Windows
        socketFileDescriptor = INVALID_SOCKET;

        std::cout << "UDPClient: Socket closed" << std::endl;
    }

    stopRequested = true; // Set the stop flag to terminate the message handling loop
}

#endif // _WIN32