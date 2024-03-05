#ifdef _WIN32

#ifndef PLAY_TOGETHER_UDPCLIENT_H
#define PLAY_TOGETHER_UDPCLIENT_H

#include <string>
#include <iostream>
#include <cstring>
#include <map>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../../Utils/UDPError.h"

/**
 * @brief The UDPClient class provides functionality to create and manage a UDP client.
 */
class UDPClient {
public:
    /**
     * @brief Constructs a UDPClient object.
     */
    UDPClient();

    /**
     * @brief Destroys the UDPClient object.
     */
    ~UDPClient();

    /**
     * @brief Gets the socket file descriptor.
     * @return The socket file descriptor.
     */
    [[nodiscard]] SOCKET getSocketFileDescriptor() const;

    /**
     * @brief Initializes the UDP client with the given server address and port.
     * @param serverHostname The IP address or hostname of the server.
     * @param serverPort The port number of the server.
     */
    void initialize(const std::string &serverHostname, short serverPort, unsigned short clientPort);

    /**
     * @brief Starts the client to handle incoming messages.
     */
    void start();

    /**
     * @brief Handles incoming messages from the server.
     */
    void handleMessages();

    /**
     * @brief Sends a message to the server.
     * @param message The message to send.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool send(const std::string &message) const;

    /**
     * @brief Receives a message from the server.
     * @return The received message.
     */
    [[nodiscard]] std::string receive(int timeoutMilliseconds) const;

    /**
     * @brief Stops the client.
     */
    void stop();

private:
    SOCKET socketFileDescriptor = INVALID_SOCKET; /**< The client socket file descriptor. */
    bool stopRequested = false; /**< Flag to indicate if the client should stop. */
    struct sockaddr_in serverAddress{}; /**< The server address structure. */
};

#endif //PLAY_TOGETHER_UDPCLIENT_H
#endif // _WIN32