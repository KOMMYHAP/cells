#pragma once

#include "asserts/assert.h"

template <class T, class U>
    requires std::is_arithmetic_v<std::decay_t<T>> && std::is_arithmetic_v<std::decay_t<U>>
constexpr T NarrowCast(U u) noexcept
{
#if defined(COMMON_ASSERTS_ENABLED)
    const auto uCurrent = static_cast<std::common_type_t<T, U>>(u);
    const auto uMax = static_cast<std::common_type_t<T, U>>(std::numeric_limits<T>::max());
    ASSERT(uCurrent <= uMax, "Numeric limit is reached!");
#endif
    return static_cast<T>(u);
}