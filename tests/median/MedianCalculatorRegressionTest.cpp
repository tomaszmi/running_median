// NOTE: STL headers are used in this test to implement straighforward but inefficient median
// algorithm used as a source of reference data for regression

#include "median/MedianCalculator.h"
#include <algorithm>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <random>

namespace
{

using namespace ::tplx;
using namespace ::testing;

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
        const auto reference = calculateReferenceMedianValue(collected);
        EXPECT_THAT(c.calculate(), DoubleEq(reference));
    }
}

} // anonymous namespace
