enable_testing()

add_executable(market_data_tests
    feed_handler_tests.cpp
)

target_link_libraries(market_data_tests
    PRIVATE
    market_data_lib
    Threads::Threads
)