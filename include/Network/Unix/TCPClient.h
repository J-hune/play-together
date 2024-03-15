#ifndef _WIN32

#ifndef PLAY_TOGETHER_TCPCLIENT_H
#define PLAY_TOGETHER_TCPCLIENT_H

#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <functional>

#include "../TCPError.h"
#include "../../Utils/Mediator.h"

/**
 * @brief The TCPClient class provides functionality to create and manage a TCP client.
 */
class TCPClient {
public:
    /** CONSTRUCTORS **/

    TCPClient();


    /** ACCESSORS **/

    /**
     * @brief Gets the socket file descriptor.
     * @return The socket file descriptor.
     */
    [[nodiscard]] int getSocketFileDescriptor() const;


    /** MODIFIERS **/

    /**
     * @brief Set a callback function to notify menu on server disconnect.
     * @param callback The callback function.
     */
    void setDisconnectCallback(std::function<void()> callback);


    /** PUBLIC METHODS **/

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
    /** ATTRIBUTES **/

    int socketFileDescriptor = -1; /**< The client socket file descriptor. */
    bool stopRequested = false; /**< Flag to indicate if the client should stop. */
    bool shouldSendDisconnect = true; /**< Flag to indicate if the client should send a disconnect message. */
    std::function<void()> disconnectCallback; /**< Callback function to notify menu on server disconnect. */
};

#endif //PLAY_TOGETHER_TCPCLIENT_H
#endif // !_WIN32