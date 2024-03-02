#include <stdexcept>
#include <string>

// Base class for TCP errors, inheriting from std::runtime_error
class TCPError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Error for issues during socket creation
class SocketCreationError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during socket binding to an address and port
class SocketBindError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during socket listening
class SocketListenError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during accepting new connections
class SocketAcceptError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during connection to a remote server
class ConnectionError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during sending data on the socket
class SocketSendError : public TCPError {
public:
    using TCPError::TCPError;
};

// Error for issues during receiving data on the socket
class SocketReceiveError : public TCPError {
public:
    using TCPError::TCPError;
};
