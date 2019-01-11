cmake_minimum_required (VERSION 3.0.0)
cmake_policy(SET CMP0048 NEW)
project(running_median VERSION 0.0.1)

option(ENABLE_SANITIZERS "Enables/disables various sanitizers" OFF)
option(ENABLE_TESTS "Build tests" ON)
option(ENABLE_BENCHMARK "Build benchmark" ON)

set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
if(ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
endif()

include_directories(src)

add_subdirectory(src/median)
add_subdirectory(src/events)
add_subdirectory(src/app)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ENABLE_BENCHMARK)
    add_subdirectory(benchmark)
endif()
