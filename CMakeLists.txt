cmake_minimum_required (VERSION 2.8.11)
cmake_policy(SET CMP0048 NEW)
project(running_median VERSION 0.0.1)

option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_SANITIZERS "Enables/disables various sanitizers" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
if(ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
endif()

add_subdirectory(src/running_median)
add_subdirectory(src/events)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(src/testing)
endif()
