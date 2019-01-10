#pragma once

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
struct RemoveConst
{
    using type = T;
};

template <typename T>
struct RemoveConst<const T>
{
    using type = T;
};

template <typename T>
struct RemoveVolatile
{
    using type = T;
};

template <typename T>
struct RemoveVolatile<volatile T>
{
    using type = T;
};

template <typename T>
struct RemoveCV
{
    using type = typename RemoveVolatile<typename RemoveConst<T>::type>::type;
};

template <typename T, typename U>
struct IsSame : FalseType
{
};

template <typename T>
struct IsSame<T, T> : TrueType
{
};

template <typename T>
constexpr bool findTypeImpl()
{
    return false;
}

template <typename T, typename Head, typename... Ts>
constexpr bool findTypeImpl()
{
    // short-circuit evaluation
    return IsSame<typename RemoveCV<T>::type, typename RemoveCV<Head>::type>::value or findTypeImpl<T, Ts...>();
}

/**
    \brief Checks whether type T exists in the type list provided by Ts variadic template.

    \return true if T exists in the Ts type list, false otherwise
 */
template <typename T, typename... Ts>
constexpr bool isTypeInTheList()
{
    return findTypeImpl<T, Ts...>();
}

}} // namespace tplx::details
