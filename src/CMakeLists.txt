add_library(matching_engine
    MatchingEngine/OrderBook.cpp
    MatchingEngine/MatchingEngine.cpp
)

target_include_directories(matching_engine PUBLIC ../include)

add_executable(TradingEngine main.cpp)

target_link_libraries(TradingEngine matching_engine)