add_library(
    matching_engine
    Matching_Engine/MatchingEngine.cpp
    Matching_Engine/OrderBook.cpp
    Matching_Engine/Command_Parser.cpp
    Position_Engine/PostionEngine.cpp
)

target_include_directories(
    matching_engine
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(
    trading_engine
    main.cpp
)

target_link_libraries(
    trading_engine
    matching_engine
)