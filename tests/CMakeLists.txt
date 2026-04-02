enable_testing()

add_executable(orderbook_tests orderbook_tests.cpp)

target_link_libraries(orderbook_tests
    matching_engine
    gtest_main
)

add_test(NAME OrderBookTest COMMAND orderbook_tests)