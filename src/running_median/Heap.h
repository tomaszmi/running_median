#pragma once

#include "Comparators.h"
#include "HeapStorage.h"

namespace tplx
{

/**
    \brief Implementation of the HEAP data structure
        inspired by "Introduction To Algorithms" (Thomas H. Cormen et. all) 
 */
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

    /**
        \brief Inserts new element to the heap.
        \throws std::bad_alloc if there is not enough memory available
        \param item new element to be inserted
     */
    void insert(T item);

    /**
        \brief Removes top element from the heap.
     */
    void pop() noexcept;

    /**
        \brief Returns top element of the heap.
        \return top element.
     */
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
    size_type parent(size_type index) const noexcept
    {
        return (index-1)/2;
    }
    size_type left(size_type index) const noexcept
    {
        return 2*index + 1;
    }
    size_type right(size_type index) const noexcept
    {
        return 2*index + 2;
    }

    /// \brief Implements MAX-HEAPIFY/MIN-HEAPIFY procedure
    void heapify(size_type index) noexcept;

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

/// \brief Implements MAX-HEAP-INSERT/MIN-HEAP-INSERT procedure
template <typename T, typename Compare>
void Heap<T, Compare>::insert(T item)
{
    this->push_back(item);
    auto i = this->size()-1;
    while(i > 0 and cmp_(this->operator[](parent(i)), this->operator[](i)))
    {
        this->swap(i, parent(i));
        i = parent(i);
    }
}

/// \brief Implements HEAP-EXTRACT-MAX/HEAP-EXTRACT-MIN procedure
template <typename T, typename Compare>
void Heap<T, Compare>::pop() noexcept
{
    this->swap(0, this->size()-1);
    this->pop_back();
    if(not this->empty())
    {
        heapify(0);
    }
}

/// \brief Implements HEAP-MAXIMUM/HEAP-MINIMUM procedure
template <typename T, typename Compare>
T Heap<T, Compare>::top() const noexcept
{
    return this->front();
}

/// \brief Implements MAX-HEAPIFY/MIN-HEAPIFY procedure
template <typename T, typename Compare>
void Heap<T, Compare>::heapify(size_type index) noexcept
{
    while(true)
    {
        auto l = left(index);
        auto r = right(index);
        auto selected = index;
        if(l <= (this->size() - 1) and cmp_(this->operator[](selected), this->operator[](l)))
        {
            selected = l;
        }
        if(r <= (this->size() -1) and cmp_(this->operator[](selected), this->operator[](r)))
        {
            selected = r;
        }
        if(selected != index)
        {
            this->swap(selected, index);
            index = selected;
        }
        else
        {
            break;
        }
    }
}

} // namespace tplx
