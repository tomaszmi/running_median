add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/googletest" "${PROJECT_BINARY_DIR}/third_party/googletest")

add_executable(
    running_median_tests
    median/HeapStorageUnitTest
    median/HeapStorageTypeTraitsUnitTest
    median/HeapUnitTest
    median/MedianCalculatorUnitTest
    median/MedianCalculatorRegressionTest
    events/EventLoopUnitTest
)

target_link_libraries(running_median_tests
    median
    events
    gtest_main
    gmock
)

add_test(
    NAME running_median_tests
    COMMAND running_median_tests
)
