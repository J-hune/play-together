#ifdef _WIN32

#ifndef PLAY_TOGETHER_UDPSERVER_H
#define PLAY_TOGETHER_UDPSERVER_H

#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <ranges>
#include <algorithm>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../UDPError.h"
#include "../../Utils/Mediator.h"

/**
 * @brief The UDPServer class provides functionality to create and manage a UDP server.
 */
class UDPServer {
public:
    /** CONSTRUCTORS **/

    UDPServer();


    /** ACCESSORS **/

    /**
     * @brief Returns the server socket file descriptor.
     * @return The server socket file descriptor.
     */
    [[nodiscard]] SOCKET getSocketFileDescriptor() const;


    /** PUBLIC METHODS **/

    /**
     * @brief Initializes the UDP server with the given port.
     * @param port The port number to listen on.
     */
    void initialize(short port);

    /**
     * @brief Starts the UDP server to handle incoming messages.
     * @param clientAddresses The map of client IDs and their addresses.
     * @param clientAddressesMutex The mutex to protect the client addresses map.
     */
    void start(std::map<SOCKET, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex);

    /**
     * @brief Sends a message to the specified client.
     * @param clientAddress The client address structure.
     * @param message The message to send.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool send(const sockaddr_in& clientAddress, const std::string &message) const;

    /**
     * @brief Broadcasts a message to all connected clients.
     * @param message The message to broadcast.
     * @return True if the message is sent successfully to all clients, false otherwise.
     */
    bool broadcast(const std::string &message) const;

    /**
     * @brief Receives a message from a client.
     * @param clientAddress The client address structure.
     * @return The received message.
     */
    [[nodiscard]] std::string receive(sockaddr_in& clientAddress, int timeout) const;

    /**
     * @brief Shuts down the server.
     */
    void stop();

private:
    /** ATTRIBUTES **/

    SOCKET socketFileDescriptor = INVALID_SOCKET; /**< The server socket file descriptor. */
    bool stopRequested = false; /**< Flag to indicate if the server should stop. */
    std::map<SOCKET, sockaddr_in> *clientAddressesPtr = nullptr; /**< Pointer to the map of client IDs and their addresses. */
    std::mutex *clientAddressesMutexPtr = nullptr; /**< Pointer to the mutex to protect the client addresses map. */


    /** PRIVATE METHODS **/

    /**
     * @brief Waits for incoming messages.
     */
    void handleMessage();
};

#endif //PLAY_TOGETHER_UDPSERVER_H
#endif // _WIN32