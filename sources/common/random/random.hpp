#pragma once
#include "random.h"

namespace Random {

template <std::ranges::random_access_range Ranges>
    requires std::permutable<std::ranges::iterator_t<Ranges>>
void Accessor::Shuffle(Ranges&& ranges)
{
    std::ranges::shuffle(std::forward<Ranges>(ranges), *_engine);
}

template <std::ranges::constant_range Range>
    requires std::ranges::sized_range<Range>
auto Accessor::Select(Range range)
{
    ASSERT(!std::ranges::empty(range));
    const size_t index = GetValue(0, std::ranges::size(range) - 1);
    return range[index];
}
}