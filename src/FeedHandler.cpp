#include "FeedHandler.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

namespace market {

FeedHandler::FeedHandler(uint16_t port) : port(port) {}

FeedHandler::~FeedHandler() {
    stop();
    if (socket_fd >= 0) {
        close(socket_fd);
    }
}

bool FeedHandler::initialize_socket() {
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Failed to bind socket\n";
        close(socket_fd);
        return false;
    }

    return true;
}

void FeedHandler::start() {
    if (!initialize_socket()) {
        return;
    }

    running = true;
    receive_thread = std::thread(&FeedHandler::receive_loop, this);
    process_thread = std::thread(&FeedHandler::process_loop, this);
}

void FeedHandler::stop() {
    running = false;
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    if (process_thread.joinable()) {
        process_thread.join();
    }
}

void FeedHandler::receive_loop() {
    char buffer[MAX_MESSAGE_SIZE];
    sockaddr_in sender_addr{};
    socklen_t sender_len = sizeof(sender_addr);

    while (running) {
        ssize_t bytes_received = recvfrom(socket_fd, buffer, MAX_MESSAGE_SIZE, 0,
                                        (struct sockaddr*)&sender_addr, &sender_len);
        
        if (bytes_received == sizeof(MarketMessage)) {
            MarketMessage* msg = reinterpret_cast<MarketMessage*>(buffer);
            if (!message_buffer.push(*msg)) {
                std::cerr << "Buffer full, message dropped\n";
            }
        }
    }
}

void FeedHandler::process_loop() {
    MarketMessage msg;
    while (running) {
        if (message_buffer.pop(msg)) {
            // Process the message
            auto now = std::chrono::high_resolution_clock::now();
            auto msg_time = std::chrono::nanoseconds(msg.timestamp);
            auto latency = now.time_since_epoch() - msg_time;
            
            std::cout << "Processed message - Symbol: " << msg.symbol 
                      << ", Latency: " << std::chrono::duration_cast<std::chrono::microseconds>(latency).count() 
                      << "µs\n";
        }
    }
}

} // namespace market