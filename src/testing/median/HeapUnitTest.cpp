#include "median/Heap.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace ::testing;
using namespace ::tplx;

TEST(MaxHeap, default_constructed)
{
    MaxHeap<unsigned> heap;
}

TEST(MaxHeap, insert)
{
    MaxHeap<int> h;
    h.insert(21);
    h.insert(63);
    h.insert(75);
    h.insert(15);
    h.insert(89);
    h.insert(13);
    h.insert(73);
    h.insert(100);
    h.insert(2);
    h.insert(13);
    h.insert(73);
    h.insert(65);
    h.insert(22);
    h.insert(21);
    h.insert(74);

    EXPECT_THAT(
        h, ElementsAreArray(std::initializer_list<short>{100, 89, 74, 75, 73, 65, 73, 15, 2, 13, 21, 13, 22, 21, 63}));
}

TEST(MaxHeap, top)
{
    MaxHeap<int> h;
    h.insert(21);
    EXPECT_EQ(21, h.top());
    h.insert(63);
    EXPECT_EQ(63, h.top());
    h.insert(75);
    EXPECT_EQ(75, h.top());
    h.insert(15);
    EXPECT_EQ(75, h.top());
    h.insert(74);
    EXPECT_EQ(75, h.top());
}

TEST(MaxHeap, pop)
{
    MaxHeap<int> h;
    h.insert(2);
    h.insert(4);
    h.insert(10);
    h.insert(3);
    h.insert(5);
    h.insert(15);
    h.insert(11);
    h.insert(3);
    h.insert(4);
    h.insert(9);

    EXPECT_EQ(15, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{15, 9, 11, 4, 5, 4, 10, 2, 3, 3}));
    h.pop();

    EXPECT_EQ(11, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{11, 9, 10, 4, 5, 4, 3, 2, 3}));
    h.pop();

    EXPECT_EQ(10, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{10, 9, 4, 4, 5, 3, 3, 2}));
    h.pop();

    EXPECT_EQ(9, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{9, 5, 4, 4, 2, 3, 3}));
    h.pop();

    EXPECT_EQ(5, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{5, 4, 4, 3, 2, 3}));
    h.pop();

    EXPECT_EQ(4, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{4, 3, 4, 3, 2}));
    h.pop();

    EXPECT_EQ(4, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{4, 3, 2, 3}));
    h.pop();

    EXPECT_EQ(3, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{3, 3, 2}));
    h.pop();

    EXPECT_EQ(3, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{3, 2}));
    h.pop();

    EXPECT_EQ(2, h.top());
    EXPECT_EQ(1, h.size());
    h.pop();

    EXPECT_TRUE(h.empty());
}

TEST(MinHeap, insert)
{
    MinHeap<int> h;
    h.insert(21);
    h.insert(63);
    h.insert(75);
    h.insert(15);
    h.insert(89);
    h.insert(13);
    h.insert(73);
    h.insert(100);
    h.insert(2);
    h.insert(13);
    h.insert(73);
    h.insert(65);
    h.insert(22);
    h.insert(21);
    h.insert(74);

    EXPECT_THAT(
        h, ElementsAreArray(std::initializer_list<short>{2, 13, 15, 21, 13, 22, 21, 100, 63, 89, 73, 75, 65, 73, 74}));
}

TEST(MinHeap, top)
{
    MinHeap<int> h;
    h.insert(21);
    EXPECT_EQ(21, h.top());
    h.insert(63);
    EXPECT_EQ(21, h.top());
    h.insert(75);
    EXPECT_EQ(21, h.top());
    h.insert(15);
    EXPECT_EQ(15, h.top());
    h.insert(10);
    EXPECT_EQ(10, h.top());
}

TEST(MinHeap, pop)
{
    MinHeap<int> h;
    h.insert(2);
    h.insert(4);
    h.insert(10);
    h.insert(3);
    h.insert(5);
    h.insert(15);
    h.insert(11);
    h.insert(1);
    h.insert(1);
    h.insert(-4);
    h.insert(9);

    EXPECT_EQ(-4, h.top());

    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{-4, 1, 10, 2, 1, 15, 11, 4, 3, 5, 9}));
    h.pop();

    EXPECT_EQ(1, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{1, 1, 10, 2, 5, 15, 11, 4, 3, 9}));
    h.pop();

    EXPECT_EQ(1, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{1, 2, 10, 3, 5, 15, 11, 4, 9}));
    h.pop();

    EXPECT_EQ(2, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{2, 3, 10, 4, 5, 15, 11, 9}));
    h.pop();

    EXPECT_EQ(3, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{3, 4, 10, 9, 5, 15, 11}));
    h.pop();

    EXPECT_EQ(4, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{4, 5, 10, 9, 11, 15}));
    h.pop();

    EXPECT_EQ(5, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{5, 9, 10, 15, 11}));
    h.pop();

    EXPECT_EQ(9, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{9, 11, 10, 15}));
    h.pop();

    EXPECT_EQ(10, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{10, 11, 15}));
    h.pop();

    EXPECT_EQ(11, h.top());
    EXPECT_THAT(h, ElementsAreArray(std::initializer_list<short>{11, 15}));
    h.pop();

    EXPECT_EQ(15, h.top());
    EXPECT_EQ(1, h.size());
    h.pop();

    EXPECT_TRUE(h.empty());
}

} // anonymous namespace
