#pragma once

#include "HeapStorageTypeTraits.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace tplx
{
namespace details
{

template <typename T>
class HeapStorage
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = T*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    HeapStorage() noexcept;
    ~HeapStorage();

    HeapStorage(const HeapStorage& other);
    HeapStorage(HeapStorage&& other) noexcept;
    HeapStorage& operator=(const HeapStorage& other);
    HeapStorage& operator=(HeapStorage&& other) noexcept;

    // element access
    reference operator[](size_type pos) noexcept;
    const_reference operator[](size_type pos) const noexcept;
    reference front() noexcept;
    const_reference front() const noexcept;
    reference back() noexcept;
    const_reference back() const noexcept;
    T* data() noexcept;
    const T* data() const noexcept;

    // modifiers
    void push_back(value_type item);
    void pop_back() noexcept;
    void clear() noexcept;
    void swap(size_type a, size_type b) noexcept;

    // capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    void reserve(size_type capacity);
    size_type capacity() const noexcept;
    void shrink_to_fit();

    // iterators
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;

private:
    void adjustCapacity(size_type length, bool force_shrink = false);

    T* storage_;
    size_type storage_size_;
    size_type storage_capacity_;
};

template <typename T>
HeapStorage<T>::HeapStorage() noexcept : storage_{nullptr}, storage_size_{0}, storage_capacity_{0}
{
    // Fundamental types (https://en.cppreference.com/w/cpp/language/types)
    static_assert(
        isTypeInTheList<T, short, unsigned short, int, unsigned, long, unsigned long, long long, unsigned long long>(),
        "Provided type T is not supported by HeapStorage");
}

template <typename T>
HeapStorage<T>::~HeapStorage()
{
    if (storage_)
    {
        delete[] storage_;
    }
}

template <typename T>
HeapStorage<T>::HeapStorage(const HeapStorage& other) : storage_{nullptr}, storage_size_{0}, storage_capacity_{0}
{
    if (other.storage_size_ > 0)
    {
        adjustCapacity(other.storage_size_);
        std::memcpy(storage_, other.storage_, other.storage_size_ * sizeof(value_type));
        storage_size_ = other.storage_size_;
    }
}

template <typename T>
HeapStorage<T>::HeapStorage(HeapStorage&& other) noexcept
    : storage_{other.storage_}, storage_size_{other.storage_size_}, storage_capacity_{other.storage_capacity_}
{
    other.storage_ = nullptr;
    other.storage_size_ = 0;
    other.storage_capacity_ = 0;
}

template <typename T>
HeapStorage<T>& HeapStorage<T>::operator=(const HeapStorage& other)
{
    adjustCapacity(other.storage_size_);
    std::memcpy(storage_, other.storage_, other.storage_size_ * sizeof(value_type));
    storage_size_ = other.storage_size_;
    return *this;
}

template <typename T>
HeapStorage<T>& HeapStorage<T>::operator=(HeapStorage&& other) noexcept
{
    adjustCapacity(0);
    storage_ = other.storage_;
    storage_size_ = other.storage_size_;
    storage_capacity_ = other.storage_capacity_;

    other.storage_ = nullptr;
    other.storage_size_ = 0;
    other.storage_capacity_ = 0;

    return *this;
}

template <typename T>
typename HeapStorage<T>::reference HeapStorage<T>::operator[](size_type pos) noexcept
{
    assert(pos <= storage_size_);
    return *(storage_ + pos);
}

template <typename T>
typename HeapStorage<T>::const_reference HeapStorage<T>::operator[](size_type pos) const noexcept
{
    assert(pos <= storage_size_);
    return *(storage_ + pos);
}

template <typename T>
typename HeapStorage<T>::reference HeapStorage<T>::front() noexcept
{
    assert(storage_size_ > 0);
    return *storage_;
}

template <typename T>
typename HeapStorage<T>::const_reference HeapStorage<T>::front() const noexcept
{
    assert(storage_size_ > 0);
    return *storage_;
}

template <typename T>
typename HeapStorage<T>::reference HeapStorage<T>::back() noexcept
{
    assert(storage_size_ > 0);
    return *(storage_ + storage_size_ - 1);
}

template <typename T>
typename HeapStorage<T>::const_reference HeapStorage<T>::back() const noexcept
{
    assert(storage_size_ > 0);
    return *(storage_ + storage_size_ - 1);
}

template <typename T>
T* HeapStorage<T>::data() noexcept
{
    return storage_;
}

template <typename T>
const T* HeapStorage<T>::data() const noexcept
{
    return storage_;
}

template <typename T>
void HeapStorage<T>::reserve(size_type capacity)
{
    if (capacity > storage_capacity_)
    {
        adjustCapacity(capacity);
        assert(storage_capacity_ == capacity);
    }
}

template <typename T>
void HeapStorage<T>::push_back(value_type item)
{
    assert(storage_capacity_ >= storage_size_);
    if (storage_capacity_ == storage_size_)
    {
        adjustCapacity(storage_capacity_ == 0 ? 1 : storage_capacity_ * 2);
    }
    assert(storage_capacity_ >= storage_size_);
    assert(storage_);
    storage_[storage_size_++] = item;
}

template <typename T>
void HeapStorage<T>::pop_back() noexcept
{
    assert(storage_size_ > 0);
    --storage_size_;
}

template <typename T>
void HeapStorage<T>::clear() noexcept
{
    storage_size_ = 0;
}

template <typename T>
void HeapStorage<T>::swap(size_type a, size_type b) noexcept
{
    const auto tmp = *(storage_ + b);
    *(storage_ + b) = *(storage_ + a);
    *(storage_ + a) = tmp;
}

template <typename T>
void HeapStorage<T>::shrink_to_fit()
{
    adjustCapacity(storage_size_, true);
    assert(storage_size_ == storage_capacity_);
}

template <typename T>
typename HeapStorage<T>::size_type HeapStorage<T>::size() const noexcept
{
    return storage_size_;
}

template <typename T>
typename HeapStorage<T>::size_type HeapStorage<T>::capacity() const noexcept
{
    return storage_capacity_;
}

template <typename T>
bool HeapStorage<T>::empty() const noexcept
{
    return storage_size_ == 0;
}

template <typename T>
typename HeapStorage<T>::const_iterator HeapStorage<T>::cbegin() const noexcept
{
    return storage_;
}

template <typename T>
typename HeapStorage<T>::const_iterator HeapStorage<T>::cend() const noexcept
{
    return storage_ + storage_size_;
}

template <typename T>
typename HeapStorage<T>::const_iterator HeapStorage<T>::begin() const noexcept
{
    return cbegin();
}

template <typename T>
typename HeapStorage<T>::const_iterator HeapStorage<T>::end() const noexcept
{
    return cend();
}

template <typename T>
typename HeapStorage<T>::iterator HeapStorage<T>::begin() noexcept
{
    return storage_;
}

template <typename T>
typename HeapStorage<T>::iterator HeapStorage<T>::end() noexcept
{
    return storage_ + storage_size_;
}

template <typename T>
void HeapStorage<T>::adjustCapacity(size_type length, bool force_shrink)
{
    /*
        Possible cases:

        1) storage_capacity_ = 0, length = 0
        2) storage_capacity_ = 0, length > 0
        3) storage_capacity_ > 0, length = 0
        4) storage_capacity_ > 0, length > 0 and storage_capacity_ > length
        5) storage_capacity_ > 0, length > 0 and storage_capacity_ < length
        6) storage_capacity_ > 0, length > 0 and storage_capacity_ = length
    */

    auto realloc = [this](size_type length) {
        value_type* new_storage = new value_type[length];
        std::memcpy(new_storage, storage_, storage_size_ * sizeof(value_type));
        delete[] storage_;
        storage_ = new_storage;
        storage_capacity_ = length;
    };

    if (length == storage_capacity_) // case #1 and #6
    {
        return;
    }
    if (storage_capacity_ == 0) // case #2
    {
        assert(not storage_);
        assert(length > 0);
        // allocate
        storage_ = new value_type[length];
        storage_capacity_ = length;
    }
    else if (length == 0) // case #3
    {
        assert(storage_capacity_ > 0);
        assert(storage_);
        delete[] storage_;
        storage_ = nullptr;
        storage_capacity_ = 0;
    }
    else if (storage_capacity_ > length) // case #4
    {
        assert(storage_capacity_ > 0);
        assert(storage_);
        if (force_shrink)
        {
            realloc(length);
            storage_size_ = length;
        }
    }
    else // case #5
    {
        assert(storage_capacity_ < length);
        assert(storage_capacity_ > 0);
        assert(storage_);
        realloc(length);
    }
}

} // namespace details
} // namespace tplx
