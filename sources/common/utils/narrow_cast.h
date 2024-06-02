#pragma once

#include "asserts/assert.h"

template <class T, class U>
    requires std::is_arithmetic_v<std::decay_t<T>> && std::is_arithmetic_v<std::decay_t<U>>
constexpr T NarrowCast(U&& u) noexcept
{
    ASSERT(u <= std::numeric_limits<T>::max());
    return static_cast<T>(std::forward<U>(u));
}