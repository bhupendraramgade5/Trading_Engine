add_executable(
    basic_tests
    basic_test.cpp
)

target_link_libraries(
    basic_tests
    GTest::gtest_main
    matching_engine
)

add_executable(
    orderbook_tests
    unit/orderbook_tests.cpp
)

target_link_libraries(
    orderbook_tests
    matching_engine
    GTest::gtest_main
)


add_test(
    NAME MatchingTest
    COMMAND bash -c 
    "cat ${CMAKE_SOURCE_DIR}/tests/functional/input1.txt | $<TARGET_FILE:trading_engine>"
)

include(GoogleTest)

gtest_discover_tests(basic_tests)
gtest_discover_tests(orderbook_tests)
# gtest_discover_tests(MatchingTest)