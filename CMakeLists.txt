cmake_minimum_required(VERSION 3.10)
project(MarketDataFeedHandler VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Find required packages
find_package(Threads REQUIRED)

# Main executable
add_executable(market_data_feed src/main.cpp)
target_link_libraries(market_data_feed PRIVATE Threads::Threads)