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

#include "../Utils/TCPError.h"

/**
 * @brief The TCPServer class provides functionality to create and manage a TCP server.
 */
class TCPServer {
public:
    /**
     * @brief Constructs a TCPServer object.
     */
    TCPServer();

    /**
     * @brief Destroys the TCPServer object and shuts down the server.
     */
    ~TCPServer();

    /**
     * @brief Returns the server socket file descriptor.
     * @return The server socket file descriptor.
     */
    [[nodiscard]] int getSocketFileDescriptor() const;

    /**
     * @brief Initializes the TCP server with the given port.
     * @param port The port number to listen on.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize(short port);

    /**
     * @brief Starts the TCP server to accept incoming connections.
     */
    void start();

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
    bool broadcast(const std::string &message) const;

    /**
     * @brief Receives a message from the specified client.
     * @param clientSocket The client socket file descriptor.
     * @return The received message.
     */
    [[nodiscard]] std::string receive(int clientSocket) const;

    /**
     * @brief Shuts down the server, closing all client connections.
     */
    void shutdown();

    /**
     * @brief Closes the server socket.
     */
    void destroy();

private:
    int socketFileDescriptor = -1; /**< The server socket file descriptor. */
    unsigned int maxClients = 3; /**< Maximum number of clients that can connect to the server. */
    std::vector<std::jthread> clientThreads; /**< Threads to handle client connections. */
    std::vector<int> clientsFileDescriptors; /**< File descriptors for connected clients. */
    bool stopRequested = false; /**< Flag to indicate if the server should stop. (exit the accept loop) */

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
};

#endif //PLAY_TOGETHER_TCPSERVER_H
