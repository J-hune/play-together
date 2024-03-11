#ifndef PLAY_TOGETHER_TCPERROR_H
#define PLAY_TOGETHER_TCPERROR_H

#include <string>
#include "NetworkError.h"

// Base class for TCP errors, inheriting from NetworkError
class TCPError : public NetworkError {
public:
    using NetworkError::NetworkError;
};

// Error for issues during socket creation
class TCPSocketCreationError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during socket binding to an address and port
class TCPSocketBindError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during socket listening
class TCPSocketListenError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during accepting new connections
class TCPSocketAcceptError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during connection to a remote server
class TCPConnectionError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during sending data on the socket
class TCPSocketSendError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during receiving data on the socket
class TCPSocketReceiveError : public TCPError {
public:
    using TCPError::TCPError;
};

#endif //PLAY_TOGETHER_TCPERROR_H