#pragma once

namespace Random {

using Engine = std::default_random_engine;

Engine MakeEngine(std::string_view seed);

class Accessor {
public:
    explicit Accessor(Engine& engine);

    // Returns value from range [0.0f; 1.0f]
    float GetNextValue();

    // Returns value from range [start; end]
    uint32_t GetValue(uint32_t start = 0, uint32_t end = std::numeric_limits<uint32_t>::max());

    template <std::ranges::random_access_range Ranges>
        requires std::permutable<std::ranges::iterator_t<Ranges>>
    void Shuffle(Ranges&& ranges);

    template <std::ranges::constant_range Range>
        requires std::ranges::sized_range<Range>
    auto Select(Range range);

private:
    gsl::not_null<Engine*> _engine;
};

}

#include "random.hpp"