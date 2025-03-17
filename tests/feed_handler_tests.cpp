#include "FeedHandler.hpp"
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

void test_feed_handler_creation() {
    market::FeedHandler handler(8888);
    std::cout << "Feed handler creation test passed\n";
}

void test_feed_handler_start_stop() {
    market::FeedHandler handler(8889);
    handler.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    handler.stop();
    std::cout << "Feed handler start/stop test passed\n";
}

int main() {
    try {
        test_feed_handler_creation();
        test_feed_handler_start_stop();
        std::cout << "All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}