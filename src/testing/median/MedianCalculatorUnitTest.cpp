#include "MedianCalculator.h"
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <random>

namespace
{

using namespace ::tplx;
using namespace ::testing;

TEST(MedianCalculator, lowerHalf_and_upperHalf_selection)
{
    MaxHeap<int> lowerHalf;
    MinHeap<int> upperHalf;

    details::addValue(10, lowerHalf, upperHalf);
    EXPECT_FALSE(lowerHalf.empty()) << "lowerHalf gets the value because is empty";
    EXPECT_TRUE(upperHalf.empty());

    details::addValue(5, lowerHalf, upperHalf);
    EXPECT_EQ(2U, lowerHalf.size()) << "lowerHalf gets the value because is less than current top";
    EXPECT_TRUE(upperHalf.empty());
    EXPECT_EQ(10, lowerHalf.top());

    details::addValue(10, lowerHalf, upperHalf);
    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(1U, upperHalf.size()) << "upperHalf gets the value if is equal to the lowerHalf's top";
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(10, upperHalf.top());

    details::addValue(20, lowerHalf, upperHalf);
    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(2U, upperHalf.size()) << "upperHalf gets the value if is greater than the lowerHalf's top";
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(10, upperHalf.top());
}

TEST(MedianCalculator, balance_does_nothing_if_sizes_are_equal)
{
    MaxHeap<int> lowerHalf;
    MinHeap<int> upperHalf;

    details::addValue(10, lowerHalf, upperHalf);
    details::addValue(20, lowerHalf, upperHalf);

    EXPECT_EQ(1U, lowerHalf.size());
    EXPECT_EQ(1U, upperHalf.size());
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(20, upperHalf.top());

    details::balance(lowerHalf, upperHalf);

    EXPECT_EQ(1U, lowerHalf.size());
    EXPECT_EQ(1U, upperHalf.size());
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(20, upperHalf.top());
}

TEST(MedianCalculator, balance_does_nothing_if_sizes_differs_in_less_than_2)
{
    MaxHeap<int> lowerHalf;
    MinHeap<int> upperHalf;

    details::addValue(10, lowerHalf, upperHalf);
    details::addValue(20, lowerHalf, upperHalf);
    details::addValue(5, lowerHalf, upperHalf);

    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(1U, upperHalf.size());
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(20, upperHalf.top());

    details::balance(lowerHalf, upperHalf);

    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(1U, upperHalf.size());
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(20, upperHalf.top());

    details::addValue(30, lowerHalf, upperHalf);
    details::addValue(40, lowerHalf, upperHalf);

    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(3U, upperHalf.size());
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(20, upperHalf.top());

    details::balance(lowerHalf, upperHalf);

    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(3U, upperHalf.size());
    EXPECT_EQ(10, lowerHalf.top());
    EXPECT_EQ(20, upperHalf.top());
}

TEST(MedianCalculator, balance_moves_values_from_lowerHalf_to_upperHalf_if_lowerHalf_is_bigger)
{
    MaxHeap<int> lowerHalf;
    MinHeap<int> upperHalf;

    details::addValue(10, lowerHalf, upperHalf);
    details::addValue(5, lowerHalf, upperHalf);

    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(0U, upperHalf.size());

    details::balance(lowerHalf, upperHalf);

    EXPECT_EQ(1U, lowerHalf.size());
    EXPECT_EQ(1U, upperHalf.size());
    EXPECT_EQ(5, lowerHalf.top());
    EXPECT_EQ(10, upperHalf.top());
}

TEST(MedianCalculator, balance_moves_values_from_upperHalf_to_lowerHalf_if_upperHalf_is_bigger)
{
    MaxHeap<int> lowerHalf;
    MinHeap<int> upperHalf;

    details::addValue(10, lowerHalf, upperHalf);
    details::addValue(20, lowerHalf, upperHalf);
    details::addValue(30, lowerHalf, upperHalf);
    details::addValue(40, lowerHalf, upperHalf);

    EXPECT_EQ(1U, lowerHalf.size());
    EXPECT_EQ(3U, upperHalf.size());

    details::balance(lowerHalf, upperHalf);

    EXPECT_EQ(2U, lowerHalf.size());
    EXPECT_EQ(2U, upperHalf.size());
    EXPECT_EQ(20, lowerHalf.top());
    EXPECT_EQ(30, upperHalf.top());
}

TEST(MedianCalculator, median_of_empty_set_is_nan)
{
    EXPECT_TRUE(std::isnan(MedianCalculator<int>{}.calculate()));
}

TEST(MedianCalculator, median_of_equal_sets_is_a_mean_of_tops)
{
    MedianCalculator<int> c;
    c.add(10);
    c.add(5);
    EXPECT_THAT(c.calculate(), DoubleEq(7.5));
}

TEST(MedianCalculator, running_median_for_fixed_values)
{
    MedianCalculator<int> c;

    c.add(10);
    EXPECT_THAT(c.calculate(), DoubleEq(10));
    c.add(5);
    EXPECT_THAT(c.calculate(), DoubleEq(7.5));
    c.add(20);
    EXPECT_THAT(c.calculate(), DoubleEq(10));
    c.add(3);
    EXPECT_THAT(c.calculate(), DoubleEq(7.5));
    c.add(4);
    EXPECT_THAT(c.calculate(), DoubleEq(5));
    c.add(-4);
    EXPECT_THAT(c.calculate(), DoubleEq(4.5));
    c.add(-4);
    EXPECT_THAT(c.calculate(), DoubleEq(4));
}

template <typename T>
double calculateReferenceMedianValue(std::vector<T>& values)
{
    if (values.empty())
    {
        return NAN;
    }
    std::sort(values.begin(), values.end());
    auto middle = values.size() / 2;
    if (values.size() % 2 != 0)
    {
        return values[middle];
    }
    return (static_cast<double>(values[middle]) + values[middle - 1]) / 2;
}

TEST(MedianCalculator, test_calculateReferenceMedianValue)
{
    std::vector<int> v;
    EXPECT_TRUE(std::isnan(calculateReferenceMedianValue(v)));
    v.push_back(3);
    EXPECT_THAT(calculateReferenceMedianValue(v), DoubleEq(3));
    v.push_back(7);
    EXPECT_THAT(calculateReferenceMedianValue(v), DoubleEq(5));
    v.push_back(10);
    EXPECT_THAT(calculateReferenceMedianValue(v), DoubleEq(7));
    v.push_back(11);
    EXPECT_THAT(calculateReferenceMedianValue(v), DoubleEq((7. + 10) / 2));

    {
        std::vector<int> dd{-337, 4110, 6922, 7963, 7978, 6568};
        EXPECT_THAT(calculateReferenceMedianValue(dd), DoubleEq(6745.0));
    }
}

TEST(MedianCalculator, running_median_for_randomly_generated_numbers)
{
    MedianCalculator<int> c;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(-10000, 10000);

    std::vector<int> collected;
    for (std::size_t i = 0; i < 1000; i++)
    {
        const auto randomizedValue = distribution(generator);
        c.add(randomizedValue);
        collected.push_back(randomizedValue);
        const auto reference = calculateReferenceMedianValue(collected);
        EXPECT_THAT(c.calculate(), DoubleEq(reference));
    }
}

} // anonymous namespace
