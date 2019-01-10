#pragma once

#include "Comparators.h"
#include "HeapStorage.h"

namespace tplx
{

template <typename T, typename Compare>
class Heap : private details::HeapStorage<T>
{
private:
    using Storage = details::HeapStorage<T>;

public:
    using value_type = typename Storage::value_type;
    using pointer = typename Storage::pointer;
    using const_pointer = typename Storage::const_pointer;
    using reference = typename Storage::reference;
    using const_reference = typename Storage::const_reference;
    using iterator = typename Storage::iterator;
    using const_iterator = typename Storage::const_iterator;
    using size_type = typename Storage::size_type;
    using difference_type = typename Storage::difference_type;

    Heap() = default;
    explicit Heap(Compare cmp);

    void insert(T item);
    void pop();
    T top() const noexcept;

    using Storage::reserve;
    using Storage::clear;

    using Storage::empty;
    using Storage::size;

    using Storage::begin;
    using Storage::cbegin;
    using Storage::end;
    using Storage::cend;

private:
    void buildHeap() noexcept;
    void heapify() noexcept;

    Compare cmp_;
};

template <typename T = int>
using MaxHeap = Heap<T, Less<T>>;

template <typename T = int>
using MinHeap = Heap<T, Greater<T>>;

// -------------------------------------------------------------------------- //

template <typename T, typename Compare>
Heap<T, Compare>::Heap(Compare cmp) : cmp_{cmp}
{
}

template <typename T, typename Compare>
void Heap<T, Compare>::insert(T item)
{
    this->push_back(item);
}

template <typename T, typename Compare>
void Heap<T, Compare>::pop()
{
}

template <typename T, typename Compare>
T Heap<T, Compare>::top() const noexcept
{
}

template <typename T, typename Compare>
void Heap<T, Compare>::buildHeap() noexcept
{
        // TODO
}

template <typename T, typename Compare>
void Heap<T, Compare>::heapify() noexcept
{
        // TODO
}

} // namespace tplx
