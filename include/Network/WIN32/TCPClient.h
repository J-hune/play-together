#ifdef _WIN32

#ifndef PLAY_TOGETHER_TCPCLIENT_H
#define PLAY_TOGETHER_TCPCLIENT_H

#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <functional>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../../Utils/TCPError.h"

/**
 * @brief The TCPClient class provides functionality to create and manage a TCP client.
 */
class TCPClient {
public:
    /**
     * @brief Constructs a TCPClient object.
     */
    TCPClient();

    /**
     * @brief Destroys the TCPClient object.
     */
    ~TCPClient();

    /**
     * @brief Gets the socket file descriptor.
     * @return The socket file descriptor.
     */
    [[nodiscard]] SOCKET getSocketFileDescriptor() const;

    /**
     * @brief Set a callback function to notify menu on server disconnect.
     * @param callback The callback function.
     */
    void setDisconnectCallback(std::function<void()> callback);

    /**
     * @brief Connects to the specified server.
     * @param serverAddress The IP address or hostname of the server.
     * @param port The port number of the server.
     */
    void connect(const std::string &serverAddress, short port, unsigned short& clientPort);

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
    [[nodiscard]] std::string receive() const;

    /**
     * @brief Stops the client.
     */
    void stop();

private:
    SOCKET socketFileDescriptor = INVALID_SOCKET; /**< The client socket file descriptor. */
    bool stopRequested = false; /**< Flag to indicate if the client should stop. */
    bool shouldSendDisconnect = true; /**< Flag to indicate if the client should send a disconnect message. */
    std::function<void()> disconnectCallback; /**< Callback function to notify menu on server disconnect. */
};

#endif //PLAY_TOGETHER_TCPCLIENT_H
#endif // _WIN32