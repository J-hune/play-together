#include "../../include/Utils/MessageQueue.h"

void MessageQueue::push(const std::string &message, const std::vector<std::string> &parameters) {
    std::scoped_lock lock(mutex);
    queue.emplace(message, parameters);
}

std::pair<std::string, std::vector<std::string>> MessageQueue::pop() {
    std::scoped_lock lock(mutex);
    if (queue.empty()) {
        return {"", {}};
    }
    auto message = queue.front();
    queue.pop();
    return message;
}

bool MessageQueue::empty() const {
    std::scoped_lock lock(mutex);
    return queue.empty();
}
