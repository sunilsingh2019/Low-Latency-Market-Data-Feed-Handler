#pragma once
#include "MarketMessage.hpp"
#include "RingBuffer.hpp"
#include <atomic>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

namespace market {

class FeedHandler {
public:
    FeedHandler(uint16_t port);
    ~FeedHandler();

    void start();
    void stop();

private:
    static constexpr size_t BUFFER_SIZE = 1024; // Must be power of 2
    static constexpr size_t MAX_MESSAGE_SIZE = 1024;

    void receive_loop();
    void process_loop();
    bool initialize_socket();

    RingBuffer<MarketMessage, BUFFER_SIZE> message_buffer;
    std::atomic<bool> running{false};
    int socket_fd{-1};
    uint16_t port;
    std::thread receive_thread;
    std::thread process_thread;
};

} // namespace market