
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "")
add_subdirectory("${PROJECT_SOURCE_DIR}/third_party/benchmark" "${PROJECT_BINARY_DIR}/third_party/benchmark")

add_executable(running_median_benchmark
    RunningMedianBenchmarkMain
)

target_link_libraries(running_median_benchmark
    benchmark
)
