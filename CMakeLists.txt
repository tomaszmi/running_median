cmake_minimum_required (VERSION 2.8.11)
cmake_policy(SET CMP0048 NEW)
project(running_median VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

option(ENABLE_TESTS "Build tests" ON)

add_subdirectory(src/running_median)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(src/testing)
endif()
