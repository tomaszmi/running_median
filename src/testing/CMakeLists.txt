add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest" "${PROJECT_BINARY_DIR}/third_party/googletest")

add_executable(
    running_median_ut
    HeapStorageTests.cpp
)

target_include_directories(running_median_ut SYSTEM
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/googletest/googletest/include
    PRIVATE ${PROJECT_SOURCE_DIR}/third_party/googletest/googlemock/include
)

target_link_libraries(running_median_ut
    running_median
    gtest
    gtest_main
    gmock
    gmock_main
)

add_test(
    NAME running_median_ut
    COMMAND running_median_ut
)
