#pragma once

#include "EventListener.h"
#include "Event.h"
#include <istream>
#include <cassert>

namespace tplx
{

namespace details
{

constexpr char calc_median_marker = 'm';
constexpr char seq_end_marker = 'q';
constexpr char separator_marker = ' ';

inline void consumeNextAsMarker(std::istream& source)
{
    char ch;
    source.get(ch);
    assert(not source.eof() and not source.fail());
}

template<typename T>
T consumeNextAsNewValue(std::istream& source)
{
    T newValue{};
    source >> newValue;
    if(source.fail())
    {
        throw std::invalid_argument("Not-a-number");
    }
    return newValue;
}

} // namespace details

template<typename T>
void runEventLoop(std::istream& source, EventListener<T>& listener)
{
    using namespace details;

    char next;
    bool separatorExpected = false;
    T newValue{};
    EventType type;

    while(not source.eof())
    {
        next = source.peek();
        if(source.good())
        {
            if(next == '\n' or next == '\r')
            {
                break;
            }
            else if(separatorExpected and next != separator_marker)
            {
                throw std::invalid_argument("missing separator");
            }
            else if(next == calc_median_marker)
            {
                consumeNextAsMarker(source);
                separatorExpected = true;
                type = EventType::calculate_median;
            }
            else if(next == seq_end_marker)
            {
                consumeNextAsMarker(source);
                separatorExpected = true;
                type = EventType::sequence_end;
            }
            else if(next >= '1' and next <= '9')
            {
                newValue = consumeNextAsNewValue<T>(source);
                separatorExpected = true;
                type = EventType::new_value;
            }
            else if(next == separator_marker)
            {
                if(not separatorExpected)
                {
                    throw std::invalid_argument("unexpected separator");
                }
                consumeNextAsMarker(source);
                separatorExpected = false;
            }
            else
            {
                throw std::invalid_argument("unexpected character");
            }

            if(separatorExpected)
            {
                if(not listener.newEvent(Event<T>{type, newValue}))
                {
                    break;
                }
                newValue = {};
            }
        }
    }
}

} // namespace tplx
