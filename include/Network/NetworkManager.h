#ifndef PLAY_TOGETHER_NETWORKMANAGER_H
#define PLAY_TOGETHER_NETWORKMANAGER_H

#include <mutex>
#include <map>
#include <thread>

#include "../Game/Game.h"
#include "../Utils/Mediator.h"

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

/**
 * @class NetworkManager
 * @brief Class that manages the network communication.
 */
class NetworkManager {
public:
    /** CONSTRUCTORS **/

    explicit NetworkManager(Mediator *mediator);


    /** DESTRUCTORS **/
    
    ~NetworkManager();


    /** PUBLIC METHODS **/

    void startServers();
    void startClients();

    void stopServers();
    void stopClients();

    void temporarySendMethod(const std::string &message) const;

private:
    /** ATTRIBUTES **/

    Mediator *mediatorPtr; /**< Pointer to the network manager mediator. */
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
