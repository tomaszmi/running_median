# Running Median

## Problem Statement

Calculate the median of unsorted integers being received from a stream of unknown upfront length. The task should rely on designing and implementing an efficient algorithm finding at any time the median of already received integers.

### Input

The stream sequence may contain any of the following items:

 * an arbitrary number of integers denoting subsequently added elements to the set
 * letter 'm' denoting a request to calculate and return the median of elements currently available in the set
 * letter 'c' denoting an end of the steam
 
The first element is always a number and the last is always the letter 'q'.

### Output

A sequence of floating point numbers separated by a single space, where each number represents a response for the calculate median request (letter 'm').

### Example

| Input           | Output  |
|-----------------|---------|
| 3 5 m 8 m 6 m q | 4 5 5.5 |

### Additional Notes

The implementation must be written in C/C++ and in case of C++ is allowed to use only C++ Standard I/O Library. In particular must not use Standard Library algorithms and data structures. The designed algorithm must be optimized in terms of time. Its complexity should be estimated and justified. The code should be clean, well structured and tested using any testing framework. 

# Solution

The code of application solving the problem statement is available [here](https://github.com/tomaszmi/running_median/tree/master/src/app "src/app"). The application processes sequence from the standard input stream and writes output to the standard output stream. Any error is logged to the standard error stream.

## Building

### Requirements:

 * OS: Linux recommended (tested on [Ubuntu](https://www.ubuntu.com/) 18.10), but there is a chance that it works on IOS and Windows as well
 * C++ compiler supporting C++11 (tested with [gcc](https://gcc.gnu.org/) 8.2.0 and [clang](http://clang.llvm.org/) 7.0.0)
 * [cmake](https://cmake.org/)
 * [Ninja-build](https://ninja-build.org/) or [GNU Makefile](https://www.gnu.org/software/make/manual/make.html)
 * [git](https://git-scm.com/)

### Building steps:

```console
$ cd ~/
$ git clone --recurse-submodules https://github.com/tomaszmi/running_median.git
$ mkdir ~/build
$ cd ~/build
$ cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ~/running_median
$ ninja
```

The above command builds all available targets, in particular:
 * main application (target **running_median**)
 * all available tests (target **ninja running_median_tests**)
 * benchmarking application (target **ninja running_median_benchmark**)

In order to use clang compiler it is necessary to set CC and CXX environment variables to accordingly clang and clang++.
On linux cmake uses GNU Makefile as the default generator if -G option is not provided:

```console
$ CC=clang CXX=clang++ cmake -DCMAKE_BUILD_TYPE=Release ~/running_median
$ make -j $(nproc)
```

### Additional Options

The following [cmake options](https://cmake.org/cmake/help/latest/command/option.html) are available:
 * ENABLE_SANITIZERS (default: OFF)  
   Enables/disables [address sanitizer](https://en.wikipedia.org/wiki/AddressSanitizer) and [undefined behavior](https://en.wikipedia.org/wiki/Undefined_behavior) sanitizer
 * ENABLE_TESTS (default: ON)  
   Enables/disables building all available tests.
 * ENABLE_BENCHMARK (default: ON)  
   Enables/disables building benchmarking application

## Usage

The following example usage processes sequence defined in a file by redirecting it to the standard input stream:

```console
$ cd ~/build
$ cat src/app/example_input.txt
3 5 m 8 m 6 m q
$ ./src/app/running_median < src/app/example_input.txt
4 5 5.5
$
```

# Algorithm Calculating Median

## Concept

Algorithm maintains two balanced buckets of numbers:
 * lower half - keeping numbers less than or equal to the current median value
 * upper half - keeping numbers greater than or equal to the current median value

Calculator has access to the **top** elements from both halfs. The top element from the lower half has the biggest value from all values located in that bucket. The top element from the upper half has the lowest value from all values placed there.    
When a new number comes in the algorithm determines whether it should be placed to the lower half or the uper half by comparing it with both tops. If the number is less than lower half's top it goes to the lower half bucket, otherwise goes to the upper half bucket. The buckets may differ in size by up to two elements. If they differ by exactly two elements they are balanced. The balancing is performed right after inserting the new value and relies on moving the top element from the bigger bucket to the smaller bucket. After balancing bucket are equal in size.

Calculating median:
If the bucket sizes differ (by one) the median is equal to the top value of the bigger bucket, otherwise (sizes are equal) the median is equal to the arithmetic mean of two tops. If buckets are empty the median is **NAN** (https://en.wikipedia.org/wiki/NaN). Puting the same into a pseudo code:

```
calculate_median(lower, upper):
   if size(lower) > size(upper)
      return top(lower)
   if size(upper) > size(lower)
      return top(upper)
   if size(lower) == size(upper) and size(lower) > 0
      return (top(lower) + top(upper))/2
   return NAN

```

In order to make the above algorithm efficient the following condition must be met:
 * access to the top element must be fast
 * inserting and deleting top element must be fast and efficient

[Heap](https://en.wikipedia.org/wiki/Heap_(data_structure)) and in particular [Binary Heap](https://en.wikipedia.org/wiki/Binary_heap) has important properties which make that structure a good candidate to use as the buckets.

## Implementation

The lower and upper buckets are respectively Max Heap and Min Heap. The Heap has been implemented according to the book "Introduction to Algorithms" by Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford Stein (ISBN-10  9780262033848).

## Time Complexity

The most crucial heap operations are: 
 * Getting the top element:   
   Relies on reading the first element from the underlying array so is as fast as it could be
 * Inserting a new element:   
   Relies on inserting the new element at the end of the underlying array and traversing the Binary Tree bottom-up up to the "log n" times. At each step there is a single comparison and swap of two elements performed.
 * Deleting top element:   
   Relies on swapping first and last element in the underlying array, deleting the last element and then traversing the Binary Tree top-down (AKA. heapify operation) "log n" times. At each step there are three comparisons and a single swap of two elements performed.

Summarizing:

|              | Get top | Delete top |   Insert   | Calc median |
|--------------|---------|------------|------------|-------------|
| average case |   O(1)  |  O(log n)  |    O(1)    |     O(1)    |
| worst case   |   O(1)  |  O(log n)  |  O(log n)  |     O(1)    |


# Code Structure 

The [median](https://github.com/tomaszmi/running_median/tree/master/src/median) directory contains an implementation of the Heap data structure and the MedianCalculator, in particular:
 * [HeapStorage](https://github.com/tomaszmi/running_median/blob/master/src/median/HeapStorage.h) implements extensible buffer of contiguous memory limited to some POD types only used as a storage for Heap implementation.
 * [HeapStorageTypeTraits](https://github.com/tomaszmi/running_median/blob/master/src/median/HeapStorageTypeTraits.h) is a utility used in order to limit HeapStorage implementation for a subset of POD types.
 * [Heap](https://github.com/tomaszmi/running_median/blob/master/src/median/Heap.h) implements on top of HeapStorage the Binary Heap data structure.
 * [Comparators](https://github.com/tomaszmi/running_median/blob/master/src/median/Comparators.h) defines comparison functors used to implement MaxHeap and MinHeap by specializing Heap template class.
 * [MedianCalculator](https://github.com/tomaszmi/running_median/blob/master/src/median/MedianCalculator.h) implements heap-based median calculator.

The [events](https://github.com/tomaszmi/running_median/tree/master/src/events) directory contains an implementation of the event loop function reading input sequence and generating a set of corresponding events received by the provided listener, in particular:
 * [Event](https://github.com/tomaszmi/running_median/blob/master/src/events/Event.h) represents either:
   - new value being received
   - request to calculate median
   - sequence end
 * [EventListener](https://github.com/tomaszmi/running_median/blob/master/src/events/EventListener.h) abstracts and represents receiver of the generated Event objects
 * [EventLoop](https://github.com/tomaszmi/running_median/blob/master/src/events/EventLoop.h) input sequence reader building a set of Event objects and notifying EventListener about each of them. 

The [app](https://github.com/tomaszmi/running_median/tree/master/src/app) directory contains code of the main application, in particular:
 * [RunningMedianMain](https://github.com/tomaszmi/running_median/blob/master/src/app/RunningMedianMain.cpp implements a dedicated EventListener called MedianCalculatingEventListener which translates each received Event object to appropriate call to the MedianCalculator object. It defines the "main" routine creating MedianCalculatingEventListener object and passing it to the started event loop.  

The [tests](https://github.com/tomaszmi/running_median/tree/master/tests) directory contains unit tests covering functionality of median and events.

The [performance](https://github.com/tomaszmi/running_median/tree/master/performance) directory contains benchmarking code, comparing various implementation of median calculators. 

# Running Tests

```
$ cd ~/build/
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

Test command: /home/tmikolajczyk/build/tests/running_median_ut
Test timeout computed to be: 10000000
Running main() from /home/tmikolajczyk/running_median/third_party/googletest/googletest/src/gtest_main.cc
[==========] Running 46 tests from 8 test cases.
[----------] Global test environment set-up.
[----------] 18 tests from HeapStorage
[ RUN      ] HeapStorage.default_constructed_storage_is_empty
[       OK ] HeapStorage.default_constructed_storage_is_empty (0 ms)
[ RUN      ] HeapStorage.inserting_and_removing_elements_change_content
[       OK ] HeapStorage.inserting_and_removing_elements_change_content (0 ms)
[ RUN      ] HeapStorage.reserve_reallocates_and_preserves_the_content
[       OK ] HeapStorage.reserve_reallocates_and_preserves_the_content (0 ms)
[ RUN      ] HeapStorage.reserve_does_nothing_if_new_capacity_is_less_than_current_size
[       OK ] HeapStorage.reserve_does_nothing_if_new_capacity_is_less_than_current_size (0 ms)
[ RUN      ] HeapStorage.new_element_doubles_capacity_if_is_already_full
[       OK ] HeapStorage.new_element_doubles_capacity_if_is_already_full (0 ms)
[ RUN      ] HeapStorage.clear_changes_size_but_capacity
[       OK ] HeapStorage.clear_changes_size_but_capacity (0 ms)
[ RUN      ] HeapStorage.shrink_to_fit_does_nothing_if_size_is_already_equal_to_capacity
[       OK ] HeapStorage.shrink_to_fit_does_nothing_if_size_is_already_equal_to_capacity (0 ms)
[ RUN      ] HeapStorage.shrink_to_fit_allocates_new_memory_with_size_equal_to_storage_size_and_preserves_the_content
[       OK ] HeapStorage.shrink_to_fit_allocates_new_memory_with_size_equal_to_storage_size_and_preserves_the_content (0 ms)
[ RUN      ] HeapStorage.move_ctor_empty_src
[       OK ] HeapStorage.move_ctor_empty_src (0 ms)
[ RUN      ] HeapStorage.move_ctor_non_empty_src
[       OK ] HeapStorage.move_ctor_non_empty_src (0 ms)
[ RUN      ] HeapStorage.move_assignment_empty_src_and_dst
[       OK ] HeapStorage.move_assignment_empty_src_and_dst (0 ms)
[ RUN      ] HeapStorage.move_assignment_non_empty_src_and_dst
[       OK ] HeapStorage.move_assignment_non_empty_src_and_dst (0 ms)
[ RUN      ] HeapStorage.copy_ctor_empty_src
[       OK ] HeapStorage.copy_ctor_empty_src (0 ms)
[ RUN      ] HeapStorage.copy_ctor_non_empty_src
[       OK ] HeapStorage.copy_ctor_non_empty_src (0 ms)
[ RUN      ] HeapStorage.copy_assignment_empty_src_and_dst
[       OK ] HeapStorage.copy_assignment_empty_src_and_dst (0 ms)
[ RUN      ] HeapStorage.copy_assignment_non_empty_src_and_dst
[       OK ] HeapStorage.copy_assignment_non_empty_src_and_dst (0 ms)
[ RUN      ] HeapStorage.accessors
[       OK ] HeapStorage.accessors (0 ms)
[ RUN      ] HeapStorage.swap
[       OK ] HeapStorage.swap (0 ms)
[----------] 18 tests from HeapStorage (0 ms total)

[----------] 1 test from HeapStorageTypeTraits
[ RUN      ] HeapStorageTypeTraits.isTypeInTheList
[       OK ] HeapStorageTypeTraits.isTypeInTheList (0 ms)
[----------] 1 test from HeapStorageTypeTraits (0 ms total)

[----------] 4 tests from MaxHeap
[ RUN      ] MaxHeap.default_constructed
[       OK ] MaxHeap.default_constructed (0 ms)
[ RUN      ] MaxHeap.insert
[       OK ] MaxHeap.insert (0 ms)
[ RUN      ] MaxHeap.top
[       OK ] MaxHeap.top (0 ms)
[ RUN      ] MaxHeap.pop
[       OK ] MaxHeap.pop (1 ms)
[----------] 4 tests from MaxHeap (1 ms total)

[----------] 3 tests from MinHeap
[ RUN      ] MinHeap.insert
[       OK ] MinHeap.insert (0 ms)
[ RUN      ] MinHeap.top
[       OK ] MinHeap.top (0 ms)
[ RUN      ] MinHeap.pop
[       OK ] MinHeap.pop (0 ms)
[----------] 3 tests from MinHeap (0 ms total)

[----------] 10 tests from MedianCalculator
[ RUN      ] MedianCalculator.lowerHalf_and_upperHalf_selection
[       OK ] MedianCalculator.lowerHalf_and_upperHalf_selection (0 ms)
[ RUN      ] MedianCalculator.balance_does_nothing_if_sizes_are_equal
[       OK ] MedianCalculator.balance_does_nothing_if_sizes_are_equal (0 ms)
[ RUN      ] MedianCalculator.balance_does_nothing_if_sizes_differs_in_less_than_2
[       OK ] MedianCalculator.balance_does_nothing_if_sizes_differs_in_less_than_2 (0 ms)
[ RUN      ] MedianCalculator.balance_moves_values_from_lowerHalf_to_upperHalf_if_lowerHalf_is_bigger
[       OK ] MedianCalculator.balance_moves_values_from_lowerHalf_to_upperHalf_if_lowerHalf_is_bigger (0 ms)
[ RUN      ] MedianCalculator.balance_moves_values_from_upperHalf_to_lowerHalf_if_upperHalf_is_bigger
[       OK ] MedianCalculator.balance_moves_values_from_upperHalf_to_lowerHalf_if_upperHalf_is_bigger (0 ms)
[ RUN      ] MedianCalculator.median_of_empty_set_is_nan
[       OK ] MedianCalculator.median_of_empty_set_is_nan (0 ms)
[ RUN      ] MedianCalculator.median_of_equal_sets_is_a_mean_of_tops
[       OK ] MedianCalculator.median_of_equal_sets_is_a_mean_of_tops (0 ms)
[ RUN      ] MedianCalculator.running_median_for_fixed_values
[       OK ] MedianCalculator.running_median_for_fixed_values (0 ms)
[ RUN      ] MedianCalculator.reset_restarts_calculation
[       OK ] MedianCalculator.reset_restarts_calculation (0 ms)
[ RUN      ] MedianCalculator.test_calculateReferenceMedianValue_sort_based
[       OK ] MedianCalculator.test_calculateReferenceMedianValue_sort_based (0 ms)
[ RUN      ] MedianCalculator.test_calculateReferenceMedianValue_nth_element_based
[       OK ] MedianCalculator.test_calculateReferenceMedianValue_nth_element_based (0 ms)
[----------] 10 tests from MedianCalculator (0 ms total)

[----------] 1 test from MedianCalculatorRegression
[ RUN      ] MedianCalculatorRegression.running_median_for_randomly_generated_numbers
[       OK ] MedianCalculatorRegression.running_median_for_randomly_generated_numbers (11 ms)
[----------] 1 test from MedianCalculatorRegression (11 ms total)

[----------] 3 tests from EventLoop
[ RUN      ] EventLoop.empty_input
[       OK ] EventLoop.empty_input (0 ms)
[ RUN      ] EventLoop.eol_input
[       OK ] EventLoop.eol_input (0 ms)
[ RUN      ] EventLoop.valid_input
[       OK ] EventLoop.valid_input (0 ms)
[----------] 3 tests from EventLoop (0 ms total)

[----------] 6 tests from EventLoopTestParametrized/EventLoopTest
[ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/0
[       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/0 (0 ms)
[ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/1
[       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/1 (0 ms)
[ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/2
[       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/2 (0 ms)
[ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/3
[       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/3 (0 ms)
[ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/4
[       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/4 (0 ms)
[ RUN      ] EventLoopTestParametrized/EventLoopTest.invalid_input/5
[       OK ] EventLoopTestParametrized/EventLoopTest.invalid_input/5 (0 ms)
[----------] 6 tests from EventLoopTestParametrized/EventLoopTest (0 ms total)

[----------] Global test environment tear-down
[==========] 46 tests from 8 test cases ran. (12 ms total)
[  PASSED  ] 46 tests.
1/1 Test #1: running_median_ut ................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

# Running Benchmark Tests

Benchmarking application checks and compares a few implementations of median calculators (heap-based, std::sort-based and std::nth_element based) measuring time needed to insert a single integer value to the set of respectively 10, 100, 1000, 10000 and 100000 elements.

```
$ cd ~/build/
$ ./performance/running_median_benchmark 
2019-01-11 13:04:26
Running ./performance/running_median_benchmark
Run on (12 X 4100 MHz CPU s)
CPU Caches:
  L1 Data 32K (x6)
  L1 Instruction 32K (x6)
  L2 Unified 256K (x6)
  L3 Unified 9216K (x1)
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark                                 Time           CPU Iterations
------------------------------------------------------------------------
BM_MedianCalc_heap/10                     8 ns          8 ns   83863948
BM_MedianCalc_heap/100                    8 ns          8 ns   93458373
BM_MedianCalc_heap/1000                   8 ns          8 ns   85859602
BM_MedianCalc_heap/10000                  9 ns          9 ns   91475462
BM_MedianCalc_heap/100000                 9 ns          9 ns   64679835
BM_MedianCalc_sort/10                   363 ns        365 ns    1909804
BM_MedianCalc_sort/100                  791 ns        793 ns     875498
BM_MedianCalc_sort/1000               15773 ns      15783 ns      46406
BM_MedianCalc_sort/10000             534112 ns     534195 ns       1000
BM_MedianCalc_sort/100000           7281180 ns    7281929 ns        101
BM_MedianCalc_nth_element/10            628 ns        631 ns    1195197
BM_MedianCalc_nth_element/100           810 ns        814 ns     859960
BM_MedianCalc_nth_element/1000         3096 ns       3100 ns     224450
BM_MedianCalc_nth_element/10000       58976 ns      58994 ns      11340
BM_MedianCalc_nth_element/100000     989380 ns     989529 ns        684
```
