#pragma once

namespace common {

template <class Mask, class Flag>
constexpr bool HasFlag(Mask value, Flag flag)
{
    static_assert(std::is_integral_v<Mask>, "You should manipulate mask as integer");
    static_assert(std::is_same_v<std::underlying_type_t<Flag>, Mask>, "Wrong type of flag");
    const auto f = static_cast<std::underlying_type_t<Flag>>(flag);
    return (value & f) == f;
}

template <class Mask, class Flag>
constexpr void SetFlag(Mask& value, Flag flag)
{
    static_assert(std::is_integral_v<Mask>, "You should manipulate mask as integer");

    const auto f = static_cast<std::underlying_type_t<Flag>>(flag);
    value = value | f;
}

template <class Mask, class Flag>
constexpr void ResetFlag(Mask& value, Flag flag)
{
    static_assert(std::is_integral_v<Mask>, "You should manipulate mask as integer");

    const auto f = static_cast<std::underlying_type_t<Flag>>(flag);
    value = value & (!f);
}

}