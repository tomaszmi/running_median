// NOTE: STL headers are used here for generating random numbers used for regression test

#include "median/MedianCalculator.h"
#include "ReferenceMedianCalculator.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <random>

namespace
{

using namespace ::tplx;
using namespace ::testing;

TEST(MedianCalculator, test_calculateReferenceMedianValue_sort_based)
{
    {
        std::vector<int> v;
        EXPECT_TRUE(std::isnan(calculateReferenceMedianValue_sort_based(v)));
        v.push_back(3);
        EXPECT_THAT(calculateReferenceMedianValue_sort_based(v), DoubleEq(3));
        v.push_back(7);
        EXPECT_THAT(calculateReferenceMedianValue_sort_based(v), DoubleEq(5));
        v.push_back(10);
        EXPECT_THAT(calculateReferenceMedianValue_sort_based(v), DoubleEq(7));
        v.push_back(11);
        EXPECT_THAT(calculateReferenceMedianValue_sort_based(v), DoubleEq((7. + 10) / 2));
    }
    {
        std::vector<int> v{-337, 4110, 6922, 7963, 7978, 6568};
        EXPECT_THAT(calculateReferenceMedianValue_sort_based(v), DoubleEq(6745.0));
    }
}

TEST(MedianCalculator, test_calculateReferenceMedianValue_nth_element_based)
{
    {
        std::vector<int> v;
        EXPECT_TRUE(std::isnan(calculateReferenceMedianValue_nth_element_based(v)));
        v.push_back(3);
        EXPECT_THAT(calculateReferenceMedianValue_nth_element_based(v), DoubleEq(3));
        v.push_back(7);
        EXPECT_THAT(calculateReferenceMedianValue_nth_element_based(v), DoubleEq(5));
        v.push_back(10);
        EXPECT_THAT(calculateReferenceMedianValue_nth_element_based(v), DoubleEq(7));
        v.push_back(11);
        EXPECT_THAT(calculateReferenceMedianValue_nth_element_based(v), DoubleEq((7. + 10) / 2));
    }
    {
        std::vector<int> v{-337, 4110, 6922, 7963, 7978, 6568};
        EXPECT_THAT(calculateReferenceMedianValue_nth_element_based(v), DoubleEq(6745.0));
    }
}

TEST(MedianCalculatorRegression, running_median_for_randomly_generated_numbers)
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
        const auto reference = calculateReferenceMedianValue_sort_based(collected);
        EXPECT_THAT(c.calculate(), DoubleEq(reference));
    }
}

} // anonymous namespace
