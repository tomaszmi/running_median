add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest" "${PROJECT_BINARY_DIR}/third_party/googletest")

add_executable(
    running_median_ut
    HeapStorageUnitTest
    HeapUnitTest
)

target_link_libraries(running_median_ut
    running_median
    gtest_main
    gmock
)

add_test(
    NAME running_median_ut
    COMMAND running_median_ut
)
