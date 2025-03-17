set(SOURCES
    main.cpp
    FeedHandler.cpp
)

add_library(market_data_lib STATIC
    FeedHandler.cpp
)

target_link_libraries(market_data_lib
    PUBLIC
    Threads::Threads
)