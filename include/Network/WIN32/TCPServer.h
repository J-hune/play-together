#ifdef _WIN32

#ifndef PLAY_TOGETHER_TCPSERVER_H
#define PLAY_TOGETHER_TCPSERVER_H

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

#include "../TCPError.h"
#include "../../Utils/Mediator.h"
#include "../../../dependencies/json.hpp"

/**
 * @brief The TCPServer class provides functionality to create and manage a TCP server.
 */
class TCPServer {
public:
    /** CONSTRUCTORS **/

    TCPServer();


    /** ACCESSORS **/

    /**
     * @brief Returns the server socket file descriptor.
     * @return The server socket file descriptor.
     */
    [[nodiscard]] SOCKET getSocketFileDescriptor() const;


    /** PUBLIC METHODS **/

    /**
     * @brief Initializes the TCP server with the given port.
     * @param port The port number to listen on.
     * @return True if initialization is successful, false otherwise.
     */
    void initialize(short port);

    /**
     * @brief Starts the TCP server to accept incoming connections.
     * @param clientAddresses The map of client IDs and their addresses.
     * @param clientAddressesMutex The mutex to protect the client addresses map.
     */
    void start(std::map<SOCKET, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex);

    /**
     * @brief Sends a message to the specified client.
     * @param clientSocket The client socket file descriptor.
     * @param message The message to send.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool send(SOCKET clientSocket, const std::string &message) const;

    /**
     * @brief Broadcasts a message to all connected clients.
     * @param message The message to broadcast.
     * @return True if the message is sent successfully to all clients, false otherwise.
     */
    bool broadcast(const std::string &message, SOCKET socketIgnored) const;

    /**
     * @brief Receives a message from the specified client.
     * @param clientSocket The client socket file descriptor.
     * @return The received message.
     */
    [[nodiscard]] std::string receive(SOCKET clientSocket) const;

    /**
     * @brief Sends the game properties to the specified client.
     * @param clientSocket The client socket file descriptor.
     * @return
     */
    bool sendGameProperties(SOCKET clientSocket) const;

    /**
     * @brief Sends the list of players to the specified client.
     * @param clientSocket The client socket file descriptor.
     * @return
     */
    bool sendPlayerList(SOCKET clientSocket) const;

    /**
     * Relay the client connection to all clients.
     * @param clientSocket The client socket file descriptor
     * @return
     */
    bool relayClientConnection(SOCKET clientSocket) const;

    /**
     * @brief Relay the client disconnection to all clients.
     * @param clientSocket The client socket file descriptor.
     * @return
     */
    bool relayClientDisconnection(SOCKET clientSocket) const;

    /**
     * @brief Shuts down the server, closing all client connections.
     */
    void stop();

private:
    /** ATTRIBUTES **/

    SOCKET socketFileDescriptor = INVALID_SOCKET; /**< The server socket file descriptor. */
    unsigned int maxClients = 3; /**< Maximum number of clients that can connect to the server. */
    std::vector<std::jthread> clientThreads; /**< Threads to handle client connections. */
    std::atomic<bool> stopRequested = false; /**< Flag to indicate if the server should stop. (exit the accept loop) */
    std::map<SOCKET, sockaddr_in> *clientAddressesPtr{}; /**< Pointer to the map of client IDs and their addresses. (file descriptor, address) */
    std::mutex *clientAddressesMutexPtr; /**< Pointer to the mutex to protect the client addresses map. */


    /** PRIVATE METHODS **/

    /**
     * @brief Waits for a new client connection.
     * @return The client socket file descriptor if successful, -1 if an error occurs.
     */
    SOCKET waitForConnection();

    /**
     * @brief Accepts incoming client connections.
     */
    void acceptConnections();

    /**
     * @brief Handles messages from a client.
     * @param clientSocket The client socket file descriptor.
     */
    void handleMessage(SOCKET clientSocket);

    /**
     * @brief Close all client connections and clear resources.
     */
    void clearResources();
};

#endif //PLAY_TOGETHER_TCPSERVER_H
#endif // _WIN32
