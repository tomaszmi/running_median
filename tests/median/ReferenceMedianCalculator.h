#pragma once

// NOTE: STL headers are used here.

#include <algorithm>
#include <vector>

namespace tplx
{

/**
    \brief Straightforward but inefficient median algorithm used as a reference implementation.
        Uses std::sort algorithm underneath.
 */
template <typename T>
double calculateReferenceMedianValue_sort_based(std::vector<T>& values)
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

/**
    \brief Straightforward but inefficient median algorithm used as a reference implementation.
        Uses std::nth_element algorithm underneath.
 */
template <typename T>
double calculateReferenceMedianValue_nth_element_based(std::vector<T>& values)
{
    if (values.empty())
    {
        return NAN;
    }
    if( values.size() == 1)
    {
        return values.front();
    }
    auto middle = std::next(values.begin(), values.size()/2);
    std::nth_element(values.begin(), middle, values.end());
    if(values.size()%2 != 0)
    {
        return *middle;
    }
    auto beforeMiddle = std::prev(middle);
    std::nth_element(values.begin(), beforeMiddle, values.end());
    return (static_cast<double>(*beforeMiddle) + *middle)/2;
}

} // namespace tplx
