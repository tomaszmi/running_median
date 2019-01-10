#pragma once

namespace tplx
{

template <typename T = void>
struct Less
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs < rhs;
    }
};

template <typename T = void>
struct Greater
{
    constexpr bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs > rhs;
    }
};

} // namespace tplx
