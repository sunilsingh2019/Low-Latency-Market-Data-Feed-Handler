#include "FeedHandler.hpp"
#include <iostream>
#include <csignal>

market::FeedHandler* handler = nullptr;

void signal_handler(int signal) {
    if (handler) {
        std::cout << "\nShutting down...\n";
        handler->stop();
    }
}

int main(int argc, char* argv[]) {
    // Default port 8888 if not specified
    uint16_t port = argc > 1 ? static_cast<uint16_t>(std::stoi(argv[1])) : 8888;
    
    // Set up signal handling
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    try {
        handler = new market::FeedHandler(port);
        std::cout << "Starting Market Data Feed Handler on port " << port << std::endl;
        handler->start();
        
        // Wait for signal
        while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    delete handler;
    return 0;
}