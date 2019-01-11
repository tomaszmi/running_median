add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest" "${PROJECT_BINARY_DIR}/third_party/googletest")

add_executable(
    running_median_ut
    median/HeapStorageUnitTest
    median/HeapStorageTypeTraitsUnitTest
    median/HeapUnitTest
    median/MedianCalculatorUnitTest
    median/MedianCalculatorRegressionTest
    events/EventLoopUnitTest
)

target_link_libraries(running_median_ut
    median
    events
    gtest_main
    gmock
)

add_test(
    NAME running_median_ut
    COMMAND running_median_ut
)
