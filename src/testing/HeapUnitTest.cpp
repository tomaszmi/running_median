#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Heap.h"

namespace
{

using namespace ::testing;
using namespace ::tplx;

TEST(MaxHeap, default_constructed)
{
    MaxHeap<unsigned> heap;
}

} // anonymous namespace
