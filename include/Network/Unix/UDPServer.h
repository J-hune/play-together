#ifndef _WIN32

#ifndef PLAY_TOGETHER_UDPSERVER_H
#define PLAY_TOGETHER_UDPSERVER_H

#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <ranges>
#include <algorithm>
#include <cstring>

#include "../UDPError.h"
#include "../../Utils/Mediator.h"

/**
 * @brief The UDPServer class provides functionality to create and manage a UDP server.
 */
class UDPServer {
private:
    /* ATTRIBUTES */

    int socketFileDescriptor = -1; /**< The server socket file descriptor. */
    bool stopRequested = false; /**< Flag to indicate if the server should stop. */
    std::map<int, sockaddr_in> *clientAddressesPtr = nullptr; /**< Pointer to the map of client IDs and their addresses. */
    std::mutex *clientAddressesMutexPtr = nullptr; /**< Pointer to the mutex to protect the client addresses map. */


public:
    /* CONSTRUCTORS */

    UDPServer();


    /* ACCESSORS */

    /**
     * @brief Returns the server socket file descriptor.
     * @return The server socket file descriptor.
     */
    [[nodiscard]] int getSocketFileDescriptor() const;


    /* METHODS */

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
    void start(std::map<int, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex);

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
     * @param socketIgnored The socket to ignore when broadcasting. (0 to broadcast to all clients)
     * @return True if the message is sent successfully to all clients, false otherwise.
     */
    bool broadcast(const std::string &message, int socketIgnored) const;

    /**
     * @brief Receives a message from a client.
     * @param clientAddress The client address structure.
     * @return The received message.
     */
    [[nodiscard]] std::string receive(sockaddr_in& clientAddress, int timeout) const;

    /**
     * @brief Sends a synchronous correction message to the client.
     * @param clientSocket The client socket file descriptor.
     * @param message The message to send.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool sendSyncCorrection(int clientSocket, sockaddr_in address, nlohmann::json &message) const;

    /**
     * @brief Shuts down the server.
     */
    void stop();

private:

    /**
     * @brief Waits for incoming messages.
     */
    void handleMessage();
};

#endif //PLAY_TOGETHER_UDPSERVER_H
#endif // !_WIN32