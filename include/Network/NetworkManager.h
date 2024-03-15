#ifndef PLAY_TOGETHER_NETWORKMANAGER_H
#define PLAY_TOGETHER_NETWORKMANAGER_H

#include <mutex>
#include <map>
#include <thread>

#include "../Game/Game.h"
#include "../Utils/Mediator.h"
#include "../../dependencies/json.hpp"

#ifdef _WIN32
#include "../Network/WIN32/TCPServer.h"
#include "../Network/WIN32/TCPClient.h"
#include "../Network/WIN32/UDPServer.h"
#include "../Network/WIN32/UDPClient.h"
#else
#include "../Network/Unix/TCPServer.h"
#include "../Network/Unix/TCPClient.h"
#include "../Network/Unix/UDPServer.h"
#include "../Network/Unix/UDPClient.h"
#endif

enum class MessageType {
    INPUT,
    PLAYER_UPDATE,
    GAME_EVENT,
    ITEM_PICKUP
};

/**
 * @class NetworkManager
 * @brief Class that manages the network communication.
 */
class NetworkManager {
public:
    /** CONSTRUCTORS **/

    NetworkManager();


    /** PUBLIC METHODS **/

    /**
     * @brief Starts the TCP and UDP servers.
     */
    void startServers();

    /**
     * @brief Starts the TCP and UDP clients.
     */
    void startClients();

    /**
     * @brief Stops the TCP and UDP servers.
     */
    void stopServers();

    /**
     * @brief Stops the TCP and UDP clients.
     */
    void stopClients();

    /**
     * @brief Sends a message to the server if the client is connected, or to all clients if the server is connected. (TCP and UDP)
     * @param message The message to send.
     */
     void temporarySendMethod(const std::string &message) const;

     void sendPlayerUpdate(uint16_t keyboardStateMask) const;

private:
    /** ATTRIBUTES **/

    TCPServer tcpServer; /**< TCP server instance for network communication. */
    TCPClient tcpClient; /**< TCP client instance for network communication. */
    UDPServer udpServer; /**< UDP server instance for network communication. */
    UDPClient udpClient; /**< UDP client instance for network communication. */
    std::unique_ptr<std::jthread> serverTCPThreadPtr; /**< Pointer to the TCP server thread. */
    std::unique_ptr<std::jthread> clientTCPThreadPtr; /**< Pointer to the TCP client thread. */
    std::unique_ptr<std::jthread> serverUDPThreadPtr; /**< Pointer to the UDP server thread. */
    std::unique_ptr<std::jthread> clientUDPThreadPtr; /**< Pointer to the UDP client thread. */
    std::mutex clientAddressesMutex = {}; /**< Mutex to protect the client addresses map. */

#ifdef _WIN32
    std::map<SOCKET, sockaddr_in> clientAddresses; /**< Map storing client addresses. */
#else
    std::map<int, sockaddr_in> clientAddresses; /**< Map storing client addresses. */
#endif
};

#endif //PLAY_TOGETHER_NETWORKMANAGER_H
