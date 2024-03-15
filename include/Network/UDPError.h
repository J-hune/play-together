#ifndef PLAY_TOGETHER_UDPERROR_H
#define PLAY_TOGETHER_UDPERROR_H

#include <string>
#include "NetworkError.h"

// Base class for UDP errors, inheriting from NetworkError
class UDPError : public NetworkError {
public:
    using NetworkError::NetworkError;
};

// Error for issues during socket creation
class UDPSocketCreationError : public UDPError {
public:
    using UDPError::UDPError;
};

// Error for issues during socket binding to an address and port
class UDPSocketBindError : public UDPError {
public:
    using UDPError::UDPError;
};

// Error for issues during sending data on the socket
class UDPSocketSendError : public UDPError {
public:
    using UDPError::UDPError;
};

// Error for issues during receiving data on the socket
class UDPSocketReceiveError : public UDPError {
public:
    using UDPError::UDPError;
};

#endif //PLAY_TOGETHER_UDPERROR_H