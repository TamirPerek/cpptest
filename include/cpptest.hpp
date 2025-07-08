#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace std
{
std::string to_string(const std::vector<std::string> &in)
{
    std::string result;
    for (const auto &i : in)
    {
        if (!result.empty())
        {
            result += ", ";
        }
        result += i;
    }

    return result;
}
// Only for numeric types
template <typename T>
typename std::enable_if_t<std::is_arithmetic_v<T>, std::string> to_string(const std::vector<T> &in)
{
    std::string result;
    for (const auto &i : in)
    {
        if (!result.empty())
        {
            result += ", ";
        }
        result += std::to_string(i);
    }

    return result;
}
} // namespace std

template <typename T> struct is_string_type : std::false_type
{
};

// Spezialisierung für std::string
template <> struct is_string_type<std::string> : std::true_type
{
};

// Spezialisierung für std::string_view
template <> struct is_string_type<std::string_view> : std::true_type
{
};

// Spezialisierung für const char*
template <> struct is_string_type<const char *> : std::true_type
{
};

// Spezialisierung für char*
template <> struct is_string_type<char *> : std::true_type
{
};

// Eine Hilfsfunktion für bessere Lesbarkeit
template <typename T> constexpr bool is_string_type_v = is_string_type<T>::value;

// More generic approach - detects any container with begin()/end()
template <typename T, typename = void> struct is_container : std::false_type
{
};

template <typename T>
struct is_container<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>())),
                                   typename T::value_type>> : std::true_type
{
};

template <typename T> constexpr bool is_container_v = is_container<T>::value;

// Exclude strings from being treated as containers
template <typename T>
struct is_non_string_container : std::conjunction<is_container<T>, std::negation<is_string_type<T>>>
{
};

template <typename T> constexpr bool is_non_string_container_v = is_non_string_container<T>::value;

inline std::tuple<std::string, std::function<bool(std::string)>> Equals(const char *xIn)
{
    return {xIn, [xIn](std::string In) { return xIn == In; }};
}

template <typename T> constexpr inline std::tuple<T, std::function<bool(T)>> Equals(T &&xIn)
{
    if constexpr (is_non_string_container_v<T>)
    {
        return {xIn, [xIn](auto In) -> bool { return std::equal(In.begin(), In.end(), xIn.begin(), xIn.end()); }};
    }
    else
    {
        return {xIn, [xIn](auto In) -> bool { return xIn == In; }};
    }
}

namespace Assert
{
template <typename T1, typename T2>
constexpr static inline void That(const T1 &xResult, std::tuple<T2, std::function<bool(T2)>> &&xFunc) noexcept
{
    const auto [tExpected, tFunc] = xFunc;
    if constexpr (is_string_type_v<T1>)
    {
        if (!tFunc(xResult))
        {
            std::cerr << "Assertion failed:\nExpected: " << tExpected << "\nActual: " << xResult << std::endl;
            // std::abort();
        }
    }
    else
    {
        if (!tFunc(xResult))
        {
            std::cerr << "Assertion failed:\nExpected: " << std::to_string(tExpected)
                      << "\nActual: " << std::to_string(xResult) << std::endl;
            // std::abort();
        }
    }
}
} // namespace Assert

void printDuration(const std::chrono::steady_clock::time_point &start, const std::chrono::steady_clock::time_point &end)
{
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    if (duration.count() < 1000)
    {
        std::cout << "Duration: " << duration.count() << " ns" << std::endl;
    }
    else if (duration.count() >= 1000000000)
    {
        std::cout << "Duration: " << duration.count() / static_cast<double>(1000000000) << " s" << std::endl;
    }
    else if (duration.count() >= 1000000)
    {
        std::cout << "Duration: " << duration.count() / static_cast<double>(1000000) << " ms" << std::endl;
    }
    else if (duration.count() >= 1000)
    {
        std::cout << "Duration: " << duration.count() / static_cast<double>(1000) << " µs" << std::endl;
    }
}
