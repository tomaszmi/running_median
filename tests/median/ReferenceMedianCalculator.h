#pragma once

// NOTE: STL headers are used here.

#include <algorithm>
#include <vector>

namespace tplx
{

/**
    \brief Straightforward but inefficient median algorithm used as a reference implementation.
 */
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

} // namespace tplx
