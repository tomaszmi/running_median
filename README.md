# Running Median

## Problem

TODO

# Running Median Application (solution)

## Building

Requirements:

 * OS: recommended is Linux (but there is a chance that it works on IOS as well)
 * C++ compiler supporting C++11 (tested with gcc 8.2.0 and clang 7.0.0)
 * Ninja-build or GNU Makefile
 * git

```
$ cd ~/
$ git clone --recurse-submodules https://github.com/tomaszmi/running_median.git
$ mkdir ~/build
$ cd ~/build
$ cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ~/running_median
$ ninja
```

## Usage

Passing a sequence from file:
```
$ cd ~/build
$ cat src/app/example_input.txt
3 5 m 8 m 6 m q
$ ./src/app/running_median < src/app/example_input.txt
4 5 5.5
$
```

# Algorithm (solution description)

TODO (how the problem has been solved, rationale, etc...)

# Code Structure

## Heap

 * running_median/src/median/HeapStorage.h

   : Implements extensible buffer of contiguous memory (pseudo vector but limited to some POD types only).

 * running_median/src/median/Heap.h

   : Implements basic Heap data structure (MaxHeap and MinHeap)

## MedianCalculator

 * running_median/src/median/MedianCalculator.h

   : Fast heap-based median calculator implementation

## Events

 * running_median/src/events/Event.h

   : Represents new value, median calculation request or calculator reset.

 * running_median/src/events/EventListener.h

   : Abstracted receiver of an Event

 * running_median/src/events/EventLoop.h

   : Main loop (the hearth) processesing standard input and builds Event objects out of it and notifies provided EventListener

## Application

 * running_median/src/app/RunningMedianMain.cpp

   : Implements EventListener which translates received Event objects to calls to MedianCalculator object. Runs main event loop passing this custom EventListener implementation.

# Running Tests

```
$ ctest -V
UpdateCTestConfiguration  from :/home/tmikolajczyk/build/DartConfiguration.tcl
UpdateCTestConfiguration  from :/home/tmikolajczyk/build/DartConfiguration.tcl
Test project /home/tmikolajczyk/build
Constructing a list of tests
Done constructing a list of tests
Updating test list for fixtures
Added 0 tests to meet fixture requirements
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: running_median_ut

1: Test command: /home/tmikolajczyk/build/tests/running_median_ut
1: Test timeout computed to be: 10000000
1: Running main() from /home/tmikolajczyk/running_median/third_party/googletest/googletest/src/gtest_main.cc
1: [==========] Running 46 tests from 8 test cases.
1: [----------] Global test environment set-up.
1: [----------] 18 tests from HeapStorage
1: [ RUN      ] HeapStorage.default_constructed_storage_is_empty
1: [       OK ] HeapStorage.default_constructed_storage_is_empty (0 ms)
1: [ RUN      ] HeapStorage.inserting_and_removing_elements_change_content
1: [       OK ] HeapStorage.inserting_and_removing_elements_change_content (0 ms)
1: [ RUN      ] HeapStorage.reserve_reallocates_and_preserves_the_content
1: [       OK ] HeapStorage.reserve_reallocates_and_preserves_the_content (0 ms)
1: [ RUN      ] HeapStorage.reserve_does_nothing_if_new_capacity_is_less_than_current_size
1: [       OK ] HeapStorage.reserve_does_nothing_if_new_capacity_is_less_than_current_size (0 ms)
1: [ RUN      ] HeapStorage.new_element_doubles_capacity_if_is_already_full
1: [       OK ] HeapStorage.new_element_doubles_capacity_if_is_already_full (0 ms)
1: [ RUN      ] HeapStorage.clear_changes_size_but_capacity
1: [       OK ] HeapStorage.clear_changes_size_but_capacity (0 ms)
1: [ RUN      ] HeapStorage.shrink_to_fit_does_nothing_if_size_is_already_equal_to_capacity
1: [       OK ] HeapStorage.shrink_to_fit_does_nothing_if_size_is_already_equal_to_capacity (0 ms)
1: [ RUN      ] HeapStorage.shrink_to_fit_allocates_new_memory_with_size_equal_to_storage_size_and_preserves_the_content
1: [       OK ] HeapStorage.shrink_to_fit_allocates_new_memory_with_size_equal_to_storage_size_and_preserves_the_content (0 ms)
1: [ RUN      ] HeapStorage.move_ctor_empty_src
1: [       OK ] HeapStorage.move_ctor_empty_src (0 ms)
1: [ RUN      ] HeapStorage.move_ctor_non_empty_src
1: [       OK ] HeapStorage.move_ctor_non_empty_src (0 ms)
1: [ RUN      ] HeapStorage.move_assignment_empty_src_and_dst
1: [       OK ] HeapStorage.move_assignment_empty_src_and_dst (0 ms)
1: [ RUN      ] HeapStorage.move_assignment_non_empty_src_and_dst
1: [       OK ] HeapStorage.move_assignment_non_empty_src_and_dst (0 ms)
1: [ RUN      ] HeapStorage.copy_ctor_empty_src
1: [       OK ] HeapStorage.copy_ctor_empty_src (0 ms)
1: [ RUN      ] HeapStorage.copy_ctor_non_empty_src
1: [       OK ] HeapStorage.copy_ctor_non_empty_src (0 ms)
1: [ RUN      ] HeapStorage.copy_assignment_empty_src_and_dst
1: [       OK ] HeapStorage.copy_assignment_empty_src_and_dst (0 ms)
1: [ RUN      ] HeapStorage.copy_assignment_non_empty_src_and_dst
1: [       OK ] HeapStorage.copy_assignment_non_empty_src_and_dst (0 ms)
1: [ RUN      ] HeapStorage.accessors
1: [       OK ] HeapStorage.accessors (0 ms)
1: [ RUN      ] HeapStorage.swap
1: [       OK ] HeapStorage.swap (0 ms)
1: [----------] 18 tests from HeapStorage (0 ms total)
1: 
1: [----------] 1 test from HeapStorageTypeTraits
1: [ RUN      ] HeapStorageTypeTraits.isTypeInTheList
1: [       OK ] HeapStorageTypeTraits.isTypeInTheList (0 ms)
1: [----------] 1 test from HeapStorageTypeTraits (0 ms total)
1: 
1: [----------] 4 tests from MaxHeap
1: [ RUN      ] MaxHeap.default_constructed
1: [       OK ] MaxHeap.default_constructed (0 ms)
1: [ RUN      ] MaxHeap.insert
1: [       OK ] MaxHeap.insert (0 ms)
1: [ RUN      ] MaxHeap.top
1: [       OK ] MaxHeap.top (0 ms)
1: [ RUN      ] MaxHeap.pop
1: [       OK ] MaxHeap.pop (1 ms)
1: [----------] 4 tests from MaxHeap (1 ms total)
1: 
1: [----------] 3 tests from MinHeap
1: [ RUN      ] MinHeap.insert
1: [       OK ] MinHeap.insert (0 ms)
1: [ RUN      ] MinHeap.top
1: [       OK ] MinHeap.top (0 ms)
1: [ RUN      ] MinHeap.pop
1: [       OK ] MinHeap.pop (0 ms)
1: [----------] 3 tests from MinHeap (0 ms total)
1: 
1: [----------] 10 tests from MedianCalculator
1: [ RUN      ] MedianCalculator.lowerHalf_and_upperHalf_selection
1: [       OK ] MedianCalculator.lowerHalf_and_upperHalf_selection (0 ms)
1: [ RUN      ] MedianCalculator.balance_does_nothing_if_sizes_are_equal
1: [       OK ] MedianCalculator.balance_does_nothing_if_sizes_are_equal (0 ms)
1: [ RUN      ] MedianCalculator.balance_does_nothing_if_sizes_differs_in_less_than_2
1: [       OK ] MedianCalculator.balance_does_nothing_if_sizes_differs_in_less_than_2 (0 ms)
1: [ RUN      ] MedianCalculator.balance_moves_values_from_lowerHalf_to_upperHalf_if_lowerHalf_is_bigger
1: [       OK ] MedianCalculator.balance_moves_values_from_lowerHalf_to_upperHalf_if_lowerHalf_is_bigger (0 ms)
1: [ RUN      ] MedianCalculator.balance_moves_values_from_upperHalf_to_lowerHalf_if_upperHalf_is_bigger
1: [       OK ] MedianCalculator.balance_moves_values_from_upperHalf_to_lowerHalf_if_upperHalf_is_bigger (0 ms)
1: [ RUN      ] MedianCalculator.median_of_empty_set_is_nan
1: [       OK ] MedianCalculator.median_of_empty_set_is_nan (0 ms)
1: [ RUN      ] MedianCalculator.median_of_equal_sets_is_a_mean_of_tops
1: [       OK ] MedianCalculator.median_of_equal_sets_is_a_mean_of_tops (0 ms)
1: [ RUN      ] MedianCalculator.running_median_for_fixed_values
1: [       OK ] MedianCalculator.running_median_for_fixed_values (0 ms)
1: [ RUN      ] MedianCalculator.reset_restarts_calculation
1: [       OK ] MedianCalculator.reset_restarts_calculation (0 ms)
1: [ RUN      ] MedianCalculator.test_calculateReferenceMedianValue
1: [       OK ] MedianCalculator.test_calculateReferenceMedianValue (0 ms)
1: [----------] 10 tests from MedianCalculator (0 ms total)
1: 
1: [----------] 1 test from MedianCalculatorRegression
1: [ RUN      ] MedianCalculatorRegression.running_median_for_randomly_generated_numbers
1: [       OK ] MedianCalculatorRegression.running_median_for_randomly_generated_numbers (11 ms)
1: [----------] 1 test from MedianCalculatorRegression (11 ms total)
1: 
1: [----------] 3 tests from EventLoop
1: [ RUN      ] EventLoop.empty_input
1: [       OK ] EventLoop.empty_input (0 ms)
1: [ RUN      ] EventLoop.eol_input
1: [       OK ] EventLoop.eol_input (0 ms)
1: [ RUN      ] EventLoop.valid_input
1: [       OK ] EventLoop.valid_input (0 ms)
1: [----------] 3 tests from EventLoop (0 ms total)
1: 
1: [----------] 6 tests from EventLoopTestParametrized/EventLoopTest
1: [ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/0
1: [       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/0 (0 ms)
1: [ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/1
1: [       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/1 (0 ms)
1: [ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/2
1: [       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/2 (0 ms)
1: [ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/3
1: [       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/3 (0 ms)
1: [ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/4
1: [       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/4 (0 ms)
1: [ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/5
1: [       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/5 (0 ms)
1: [----------] 6 tests from EventLoopTestParametrized/EventLoopTest (0 ms total)
1: 
1: [----------] Global test environment tear-down
1: [==========] 46 tests from 8 test cases ran. (12 ms total)
1: [  PASSED  ] 46 tests.
1/1 Test #1: running_median_ut ................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

# Running Benchmark Tests

```
$ ./benchmark/running_median_benchmark 
2019-01-11 13:04:26
Running ./benchmark/running_median_benchmark
Run on (12 X 4100 MHz CPU s)
CPU Caches:
  L1 Data 32K (x6)
  L1 Instruction 32K (x6)
  L2 Unified 256K (x6)
  L3 Unified 9216K (x1)
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                                    Time           CPU Iterations
---------------------------------------------------------------------------
BM_FastMedianCalculator/10                   9 ns          9 ns   64990408
BM_FastMedianCalculator/100                 10 ns         10 ns   78497324
BM_FastMedianCalculator/1000                10 ns         10 ns   77333556
BM_FastMedianCalculator/10000               10 ns         10 ns   74878329
BM_FastMedianCalculator/100000              10 ns         10 ns   56262959
BM_ReferenceMedianCalculator/10          75459 ns      75422 ns      23472
BM_ReferenceMedianCalculator/100         75784 ns      75745 ns      23020
BM_ReferenceMedianCalculator/1000        63185 ns      63154 ns      14002
BM_ReferenceMedianCalculator/10000      301567 ns     301268 ns       2364
BM_ReferenceMedianCalculator/100000    5198249 ns    5195875 ns        133
```
