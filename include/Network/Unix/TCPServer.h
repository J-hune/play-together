#ifndef _WIN32

#ifndef PLAY_TOGETHER_TCPSERVER_H
#define PLAY_TOGETHER_TCPSERVER_H

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

#include "../TCPError.h"
#include "../../Utils/Mediator.h"

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
    [[nodiscard]] int getSocketFileDescriptor() const;


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
    void start(std::map<int, sockaddr_in> &clientAddresses, std::mutex &clientAddressesMutex);

    /**
     * @brief Sends a message to the specified client.
     * @param clientSocket The client socket file descriptor.
     * @param message The message to send.
     * @return True if the message is sent successfully, false otherwise.
     */
    bool send(int clientSocket, const std::string &message) const;

    /**
     * @brief Broadcasts a message to all connected clients.
     * @param message The message to broadcast.
     * @return True if the message is sent successfully to all clients, false otherwise.
     */
    bool broadcast(const std::string &message, int socketIgnored) const;

    /**
     * @brief Receives a message from the specified client.
     * @param clientSocket The client socket file descriptor.
     * @return The received message.
     */
    [[nodiscard]] std::string receive(int clientSocket) const;

    /**
     * @brief Shuts down the server, closing all client connections.
     */
    void stop();

private:
    /** ATTRIBUTES **/

    int socketFileDescriptor = -1; /**< The server socket file descriptor. */
    unsigned int maxClients = 3; /**< Maximum number of clients that can connect to the server. */
    std::vector<std::jthread> clientThreads; /**< Threads to handle client connections. */
    std::atomic<bool> stopRequested = false; /**< Flag to indicate if the server should stop. (exit the accept loop) */
    std::map<int, sockaddr_in> *clientAddressesPtr = nullptr; /**< Pointer to the map of client IDs and their addresses. (file descriptor, address) */
    std::mutex *clientAddressesMutexPtr = nullptr; /**< Pointer to the mutex to protect the client addresses map. */


    /** PRIVATE METHODS **/

    /**
     * @brief Waits for a new client connection.
     * @return The client socket file descriptor if successful, -1 if an error occurs.
     */
    int waitForConnection();

    /**
     * @brief Accepts incoming client connections.
     */
    void acceptConnections();

    /**
     * @brief Handles messages from a client.
     * @param clientSocket The client socket file descriptor.
     */
    void handleMessage(int clientSocket);

    /**
     * @brief Close all client connections and clear resources.
     */
    void clearResources();
};

#endif //PLAY_TOGETHER_TCPSERVER_H
#endif // !_WIN32