#include "HeapStorage.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace ::testing;
using namespace ::tplx::details;

TEST(HeapStorage, default_constructed_storage_is_empty)
{
    HeapStorage<int> storage;
    EXPECT_TRUE(storage.empty());
    EXPECT_EQ(0U, storage.size());
    EXPECT_EQ(0U, storage.capacity());
    EXPECT_TRUE(storage.begin() == storage.end());
    EXPECT_TRUE(storage.cbegin() == storage.cend());
}

TEST(HeapStorage, inserting_and_removing_elements_change_content)
{
    HeapStorage<short> storage;
    storage.push_back(1);
    storage.push_back(2);
    storage.push_back(3);
    EXPECT_FALSE(storage.empty());
    EXPECT_EQ(3U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{1, 2, 3}));

    storage.pop_back();
    EXPECT_FALSE(storage.empty());
    EXPECT_EQ(2U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{1, 2}));

    storage.pop_back();
    EXPECT_FALSE(storage.empty());
    EXPECT_EQ(1U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{1}));

    storage.push_back(200);
    EXPECT_FALSE(storage.empty());
    EXPECT_EQ(2U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{1, 200}));

    storage.pop_back();
    EXPECT_FALSE(storage.empty());
    EXPECT_EQ(1U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{1}));

    storage.pop_back();
    EXPECT_TRUE(storage.empty());
    EXPECT_EQ(0U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{}));

    storage.push_back(33);
    EXPECT_FALSE(storage.empty());
    EXPECT_EQ(1U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<short>{33}));
}

TEST(HeapStorage, reserve_reallocates_and_preserves_the_content)
{
    HeapStorage<int> storage;
    storage.reserve(50);
    EXPECT_EQ(0U, storage.size());
    EXPECT_EQ(50U, storage.capacity());

    for (int i = 0; i < 7; i++)
    {
        storage.push_back(i);
    }

    EXPECT_EQ(7U, storage.size());
    EXPECT_EQ(50U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<int>{0, 1, 2, 3, 4, 5, 6}));

    auto oldStorageBegin = storage.begin();
    storage.reserve(345);
    auto newStorageBegin = storage.begin();

    EXPECT_NE(oldStorageBegin, newStorageBegin);
    EXPECT_EQ(7U, storage.size());
    EXPECT_EQ(345U, storage.capacity());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<int>{0, 1, 2, 3, 4, 5, 6}));
}

TEST(HeapStorage, reserve_does_nothing_if_new_capacity_is_less_than_current_size)
{
    HeapStorage<int> storage;
    storage.push_back(10);
    storage.push_back(20);
    storage.push_back(30);

    EXPECT_EQ(3U, storage.size());
    EXPECT_EQ(4U, storage.capacity());

    auto oldStorageBegin = storage.begin();
    storage.reserve(2);
    auto newStorageBegin = storage.begin();

    EXPECT_EQ(oldStorageBegin, newStorageBegin);
    EXPECT_EQ(3U, storage.size());
    EXPECT_EQ(4U, storage.capacity());
}

TEST(HeapStorage, new_element_doubles_capacity_if_is_already_full)
{
    HeapStorage<int> storage;
    storage.reserve(10);
    EXPECT_EQ(0U, storage.size());
    EXPECT_EQ(10U, storage.capacity());

    for (int i = 0; i < 10; i++)
    {
        storage.push_back(i);
    }
    EXPECT_EQ(10U, storage.size());
    EXPECT_EQ(10U, storage.capacity());

    storage.push_back(11);
    EXPECT_EQ(11U, storage.size());
    EXPECT_EQ(20U, storage.capacity());
}

TEST(HeapStorage, clear_changes_size_but_capacity)
{
    HeapStorage<int> storage;
    storage.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        storage.push_back(i);
    }
    EXPECT_EQ(10U, storage.size());
    EXPECT_EQ(10U, storage.capacity());
    storage.clear();
    EXPECT_TRUE(storage.empty());
    EXPECT_EQ(10U, storage.capacity());
}

TEST(HeapStorage, shrink_to_fit_does_nothing_if_size_is_already_equal_to_capacity)
{
    HeapStorage<int> storage;
    storage.reserve(10);
    for (int i = 0; i < 10; i++)
    {
        storage.push_back(i);
    }
    EXPECT_EQ(storage.capacity(), storage.size());
    storage.shrink_to_fit();
    EXPECT_EQ(storage.capacity(), storage.size());
}

TEST(HeapStorage, shrink_to_fit_allocates_new_memory_with_size_equal_to_storage_size_and_preserves_the_content)
{
    HeapStorage<int> storage;
    storage.reserve(10);
    for (int i = 0; i < 5; i++)
    {
        storage.push_back(i);
    }
    EXPECT_EQ(5U, storage.size());
    EXPECT_EQ(10U, storage.capacity());

    auto oldStorageBegin = storage.begin();
    storage.shrink_to_fit();
    auto newStorageBegin = storage.begin();

    EXPECT_NE(oldStorageBegin, newStorageBegin);
    EXPECT_EQ(5U, storage.capacity());
    EXPECT_EQ(storage.capacity(), storage.size());
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<int>{0, 1, 2, 3, 4}));
}

/// \brief The following helper function guarantees calling move ctor
template <typename T>
HeapStorage<T> moveAround(HeapStorage<T>&& src)
{
    return std::move(src);
}

TEST(HeapStorage, move_ctor_empty_src)
{
    HeapStorage<int> src;
    auto srcBegin = src.begin();
    auto srcAddr = &src;
    auto dst = moveAround(std::move(src));
    EXPECT_NE(srcAddr, &dst);
    EXPECT_EQ(srcBegin, dst.begin());
    EXPECT_EQ(0U, dst.size());
    EXPECT_EQ(0U, dst.capacity());
}

TEST(HeapStorage, move_ctor_non_empty_src)
{
    HeapStorage<int> src;
    src.push_back(1);
    src.push_back(2);
    src.push_back(3);
    auto srcBegin = src.begin();
    auto dst = moveAround(std::move(src));
    EXPECT_EQ(srcBegin, dst.begin());
    EXPECT_EQ(3U, dst.size());
    EXPECT_EQ(4U, dst.capacity());
    EXPECT_THAT(dst, ElementsAreArray(std::initializer_list<int>{1, 2, 3}));
}

TEST(HeapStorage, move_assignment_empty_src_and_dst)
{
    HeapStorage<int> first;
    HeapStorage<int> second;
    second = std::move(first);
    EXPECT_EQ(0U, second.size());
    EXPECT_EQ(0U, second.capacity());
}

TEST(HeapStorage, move_assignment_non_empty_src_and_dst)
{
    HeapStorage<int> first;
    first.push_back(123);
    first.push_back(456);
    HeapStorage<int> second;
    second.push_back(444);
    second.push_back(333);
    second.push_back(1024);
    second = std::move(first);
    EXPECT_EQ(2U, second.size());
    EXPECT_EQ(2U, second.capacity());
}

/// \brief The following helper function guarantees calling copy ctor
template <typename T>
HeapStorage<T> returnCopy(HeapStorage<T>&& src)
{
    HeapStorage<T> copy1 = src;
    HeapStorage<T> copy2 = src;
    return &src > &copy1 ? copy1 : copy2;
}

TEST(HeapStorage, copy_ctor_empty_src)
{
    HeapStorage<int> src;
    auto srcBegin = src.begin();
    auto srcAddr = &src;
    auto dst = returnCopy(std::move(src));
    EXPECT_NE(srcAddr, &dst);
    EXPECT_EQ(srcBegin, dst.begin());
    EXPECT_EQ(0U, dst.size());
    EXPECT_EQ(0U, dst.capacity());
}

TEST(HeapStorage, copy_ctor_non_empty_src)
{
    HeapStorage<int> src;
    src.push_back(1);
    src.push_back(2);
    src.push_back(3);
    auto srcBegin = src.begin();
    auto dst = returnCopy(std::move(src));
    EXPECT_NE(srcBegin, dst.begin());
    EXPECT_EQ(3U, dst.size());
    EXPECT_EQ(3U, dst.capacity()); // capacity is not copied
    EXPECT_THAT(dst, ElementsAreArray(std::initializer_list<int>{1, 2, 3}));
}

TEST(HeapStorage, copy_assignment_empty_src_and_dst)
{
    HeapStorage<int> first;
    HeapStorage<int> second;
    second = first;
    EXPECT_EQ(0U, second.size());
    EXPECT_EQ(0U, second.capacity());
}

TEST(HeapStorage, copy_assignment_non_empty_src_and_dst)
{
    HeapStorage<int> first;
    first.push_back(123);
    first.push_back(456);
    HeapStorage<int> second;
    second.push_back(444);
    second.push_back(333);
    second.push_back(1024);
    second = first;
    EXPECT_EQ(2U, second.size());
    EXPECT_EQ(2U, second.capacity());
}

TEST(HeapStorage, accessors)
{
    constexpr std::size_t storageSize = 10;
    HeapStorage<int> storage;
    storage.reserve(storageSize);
    for (int i = 0; i < static_cast<int>(storageSize); i++)
    {
        storage.push_back(i);
    }

    for (std::size_t i = 0; i < storageSize; i++)
    {
        EXPECT_EQ(i, storage[i]);
    }
    storage[0] = 123;
    storage[9] = 321;
    EXPECT_EQ(123, storage.front());
    EXPECT_EQ(321, storage.back());

    EXPECT_EQ(123, *storage.data());
}

TEST(HeapStorage, swap)
{
    constexpr std::size_t storageSize = 10;
    HeapStorage<int> storage;
    for (int i = 0; i < static_cast<int>(storageSize); i++)
    {
        storage.push_back(i);
    }
    storage.swap(0, 9);
    storage.swap(1, 8);
    storage.swap(2, 7);
    EXPECT_THAT(storage, ElementsAreArray(std::initializer_list<int>{9, 8, 7, 3, 4, 5, 6, 2, 1, 0}));
}

} // anonymous namespace
