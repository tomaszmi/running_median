#pragma once

#include "Heap.h"
#include <cmath>
#include <cassert>

namespace tplx
{

template <typename T>
class MedianCalculator
{
public:
    /**
        \brief Adds a new value to the set.
        \param value - value to add.
     */
    void add(T value);

    /**
        \brief Calculates median from all so far collected values.
            NOTE: Median of empty set is NAN.

        \return median value
     */
    double calculate() const noexcept;

    /**
        \brief Reset internal state, so median will be calculated from scratch starting with the
            nearest added new value.
     */
    void reset() noexcept;

    void reserve(std::size_t capacity);

private:
    MaxHeap<T> lowerHalf_;
    MinHeap<T> upperHalf_;
};

template <typename T>
double MedianCalculator<T>::calculate() const noexcept
{
    if (lowerHalf_.empty() and upperHalf_.empty())
    {
        return NAN;
    }
    if (lowerHalf_.size() == upperHalf_.size())
    {
        return (static_cast<double>(lowerHalf_.top()) + upperHalf_.top()) / 2;
    }
    if (lowerHalf_.size() > upperHalf_.size())
    {
        return lowerHalf_.top();
    }
    return upperHalf_.top();
}

namespace details
{

template <typename T>
void addValue(T value, MaxHeap<T>& lowerHalf, MinHeap<T>& upperHalf)
{
    if (lowerHalf.empty() or (value < lowerHalf.top()))
    {
        lowerHalf.insert(value);
    }
    else
    {
        upperHalf.insert(value);
    }
}

template <typename T>
void balance(MaxHeap<T>& lowerHalf, MinHeap<T>& upperHalf)
{
    if (lowerHalf.size() > upperHalf.size())
    {
        const auto diff = lowerHalf.size() - upperHalf.size();
        assert(diff <= 2);
        if (diff == 2)
        {
            upperHalf.insert(lowerHalf.top());
            lowerHalf.pop();
        }
    }
    else if (upperHalf.size() > lowerHalf.size())
    {
        const auto diff = upperHalf.size() - lowerHalf.size();
        assert(diff <= 2);
        if (diff == 2)
        {
            lowerHalf.insert(upperHalf.top());
            upperHalf.pop();
        }
    }
}

} // namespace details

template <typename T>
void MedianCalculator<T>::add(T value)
{
    addValue(value, lowerHalf_, upperHalf_);
    balance(lowerHalf_, upperHalf_);
}

template <typename T>
void MedianCalculator<T>::reset() noexcept
{
    lowerHalf_.clear();
    upperHalf_.clear();
}

template<typename T>
void MedianCalculator<T>::reserve(std::size_t capacity)
{
    lowerHalf_.reserve(capacity/2);
    upperHalf_.reserve(capacity/2);
}

} // namespace tplx
