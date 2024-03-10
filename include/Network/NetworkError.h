#ifndef PLAY_TOGETHER_NETWORKERROR_H
#define PLAY_TOGETHER_NETWORKERROR_H

#include <stdexcept>

// Base class for network errors, inheriting from std::runtime_error
class NetworkError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

#endif //PLAY_TOGETHER_NETWORKERROR_H
