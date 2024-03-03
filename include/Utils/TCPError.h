#include <stdexcept>
#include <string>

// Base class for TCP errors, inheriting from std::runtime_error
class TCPError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
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
