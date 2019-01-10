#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace tplx
{
namespace details
{

struct TrueType
{
    static constexpr bool value = true;
};

struct FalseType
{
    static constexpr bool value = false;
};

template <typename T>
struct SupportedType : FalseType
{
};

// Fundamental types (https://en.cppreference.com/w/cpp/language/types)
template <>
struct SupportedType<short> : TrueType
{
};
template <>
struct SupportedType<unsigned short> : TrueType
{
};
template <>
struct SupportedType<int> : TrueType
{
};
template <>
struct SupportedType<unsigned> : TrueType
{
};
template <>
struct SupportedType<long> : TrueType
{
};
template <>
struct SupportedType<unsigned long> : TrueType
{
};
template <>
struct SupportedType<long long> : TrueType
{
};
template <>
struct SupportedType<unsigned long long> : TrueType
{
};

template <typename T>
class HeapStorage
{
public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = T*;
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

    void reserve(size_type capacity);
    void push_back(value_type item);
    void pop_back();
    void clear();
    void shrink_to_fit();

    size_type size() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;

protected:
    T* storage_;
    size_type storage_size_;

private:
    void adjustCapacity(size_type length);

    size_type storage_capacity_;
};

template <typename T>
HeapStorage<T>::HeapStorage() noexcept : storage_{nullptr}, storage_size_{0}, storage_capacity_{0}
{
    static_assert(SupportedType<T>::value, "type is not supported by HeapStorage");
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
    adjustCapacity(other.storage_size_);
    std::memcpy(storage_, other.storage_, other.storage_size_ * sizeof(value_type));
    storage_size_ = other.storage_size_;
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
    adjustCapacity(0);
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
void HeapStorage<T>::reserve(size_type capacity)
{
    if(capacity > storage_capacity_)
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
void HeapStorage<T>::pop_back()
{
    assert(storage_size_ > 0);
    --storage_size_;
}

template <typename T>
void HeapStorage<T>::clear()
{
    storage_size_ = 0;
}

template <typename T>
void HeapStorage<T>::shrink_to_fit()
{
    adjustCapacity(storage_size_);
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
void HeapStorage<T>::adjustCapacity(std::size_t length)
{
    if (length == storage_capacity_)
    {
        return;
    }
    if (length == 0)
    {
        if (storage_)
        {
            delete[] storage_;
            storage_ = nullptr;
            storage_size_ = 0;
            storage_capacity_ = 0;
        }
        return;
    }

    value_type* new_storage = new value_type[length];
    std::memcpy(new_storage, storage_, storage_size_ * sizeof(value_type));
    delete[] storage_;
    storage_ = new_storage;
    storage_capacity_ = length;
    if (length < storage_size_)
    {
        storage_size_ = length;
    }
}

} // namespace details
} // namespace tplx
