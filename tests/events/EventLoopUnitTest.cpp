#include "events/EventLoop.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>

namespace tplx
{

template<typename T>
bool operator==(const Event<T>& lhs, const Event<T>& rhs)
{
    return lhs.value == rhs.value and lhs.type == rhs.type;
}

} // namespace tplx

namespace
{

using namespace ::tplx;
using namespace ::testing;

template<typename T>
class MockEventListener : public EventListener<T>
{
public:
    bool newEvent(Event<T>&& e) override
    {
        return newEvent(e);
    }
    MOCK_METHOD1_T(newEvent, bool(const Event<T>&));
};

template<typename T>
Event<T> newValueEvent(T value)
{
    return Event<T>(value);
}

template<typename T>
Event<T> calcMedianEvent(T)
{
    return Event<T>(EventType::calculate_median);
}

template<typename T>
Event<T> seqEndEvent(T)
{
    return Event<T>(EventType::sequence_end);
}

TEST(EventLoop, empty_input)
{
    std::istringstream input;
    StrictMock<MockEventListener<int>> listener;
    runEventLoop(input, listener);
}

TEST(EventLoop, eol_input)
{
    std::istringstream input{"\n"};
    StrictMock<MockEventListener<int>> listener;
    runEventLoop(input, listener);
}

TEST(EventLoop, valid_input)
{
    InSequence s;

    std::istringstream input{"3 5 m 8 m 6 m q 1000 m"};
    StrictMock<MockEventListener<int>> listener;
    EXPECT_CALL(listener, newEvent(newValueEvent(3))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(newValueEvent(5))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(calcMedianEvent(0))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(newValueEvent(8))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(calcMedianEvent(0))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(newValueEvent(6))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(calcMedianEvent(0))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(seqEndEvent(0))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(newValueEvent(1000))).WillOnce(Return(true));
    EXPECT_CALL(listener, newEvent(calcMedianEvent(0))).WillOnce(Return(true));
    runEventLoop(input, listener);
}

class EventLoopTest : public TestWithParam<const char*>
{
};

TEST_P(EventLoopTest, invalid_input)
{
    std::istringstream input{GetParam()};
    NiceMock<MockEventListener<int>> listener;
    EXPECT_CALL(listener, newEvent(_)).WillRepeatedly(Return(true));
    EXPECT_THROW(runEventLoop(input, listener), std::invalid_argument) << GetParam();
}

} // anonymous namespace

INSTANTIATE_TEST_SUITE_P(EventLoopTestParametrized, EventLoopTest,
                         ::testing::Values(" ", "1m", "o", "mm", "1  ", " 2"));
