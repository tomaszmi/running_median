#include <gtest/gtest.h>
#include "HeapStorageTypeTraits.h"

namespace
{

using namespace ::tplx::details;

TEST(HeapStorageTypeTraits, isTypeInTheList)
{
    EXPECT_FALSE(isTypeInTheList<int>());
    EXPECT_TRUE((isTypeInTheList<int, int, char>()));
    EXPECT_TRUE((isTypeInTheList<int, char, int>()));
    EXPECT_FALSE((isTypeInTheList<short, int, long, double>()));
    EXPECT_TRUE((isTypeInTheList<volatile int, int>()));
    EXPECT_TRUE((isTypeInTheList<const int, int>()));
    EXPECT_TRUE((isTypeInTheList<const volatile int, int>()));
}

} // anonymous namespace
