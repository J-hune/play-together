#ifndef PLAY_TOGETHER_MESSAGEQUEUE_H
#define PLAY_TOGETHER_MESSAGEQUEUE_H

#include <string>
#include <queue>
#include <mutex>

class MessageQueue {
private:
    std::queue<std::pair<std::string, std::vector<std::string>>> queue;
    mutable std::mutex mutex;

    
public:
    MessageQueue() = default;

    /**
     * @brief Push a message to the queue.
     * @param message The message to push.
     * @param parameters The parameters to push.
     */
    void push(const std::string &message, const std::vector<std::string>& parameters = {});

    /**
     * @brief Pop a message from the queue.
     * @return A tuple containing the message and the parameter.
     */
    std::pair<std::string, std::vector<std::string>> pop();

    /**
     * @brief Check if the queue is empty.
     * @return True if the queue is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const;

};

#endif //PLAY_TOGETHER_MESSAGEQUEUE_H
