cmake_minimum_required(VERSION 3.16)

project(TradingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

enable_testing()

FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(tests)
