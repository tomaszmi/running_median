#include "median/MedianCalculator.h"
#include "../tests/median/ReferenceMedianCalculator.h"
#include <benchmark/benchmark.h>
#include <vector>
#include <random>

template<typename T>
std::vector<T> generate_random_numbers(T from, T to, std::size_t amount)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(from, to);

    std::vector<T> generated;
    generated.reserve(amount);
    for (std::size_t i = 0; i < amount; i++)
    {
        generated.push_back(distribution(generator));
    }
    return generated;
}

const auto _10_numbers = generate_random_numbers(-10000, 10000, 10);
const auto _100_numbers = generate_random_numbers(-10000, 10000, 100);
const auto _1000_numbers = generate_random_numbers(-10000, 10000, 1000);
const auto _10000_numbers = generate_random_numbers(-10000, 10000, 10000);
const auto _100000_numbers = generate_random_numbers(-10000, 100000, 100000);

template<typename T>
tplx::MedianCalculator<T> prepareMedianCalculator(const std::vector<T>& numbers)
{
    tplx::MedianCalculator<T> c;
    c.reserve(numbers.size());
    for(auto number : numbers)
    {
        c.add(number);
    }
    return c;
}

template<typename ... Args>
void BM_MedianCalc_heap(benchmark::State& state, Args ... args)
{
    auto calculator = prepareMedianCalculator(args...);
    for (auto _ : state)
    {
        calculator.add(10);
        benchmark::DoNotOptimize(calculator.calculate());
    }
}

template<typename T>
std::vector<T> prepareRefCalcInput(const std::vector<T>& numbers)
{
    return numbers;
}

template<typename ... Args>
void BM_MedianCalc_sort(benchmark::State& state, Args ... args)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto numbers = prepareRefCalcInput(args...);
        state.ResumeTiming();
        numbers.push_back(10);
        tplx::calculateReferenceMedianValue_sort_based(numbers);
    }
}

template<typename ... Args>
void BM_MedianCalc_nth_element(benchmark::State& state, Args ... args)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        auto numbers = prepareRefCalcInput(args...);
        state.ResumeTiming();
        numbers.push_back(10);
        tplx::calculateReferenceMedianValue_nth_element_based(numbers);
    }
}

BENCHMARK_CAPTURE(BM_MedianCalc_heap, 10, _10_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_heap, 100, _100_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_heap, 1000, _1000_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_heap, 10000, _10000_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_heap, 100000, _100000_numbers);

BENCHMARK_CAPTURE(BM_MedianCalc_sort, 10, _10_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_sort, 100, _100_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_sort, 1000, _1000_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_sort, 10000, _10000_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_sort, 100000, _100000_numbers);

BENCHMARK_CAPTURE(BM_MedianCalc_nth_element, 10, _10_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_nth_element, 100, _100_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_nth_element, 1000, _1000_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_nth_element, 10000, _10000_numbers);
BENCHMARK_CAPTURE(BM_MedianCalc_nth_element, 100000, _100000_numbers);

BENCHMARK_MAIN();
