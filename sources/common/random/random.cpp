#include "random.h"

namespace Random {

Engine MakeEngine(std::string_view seed)
{
    auto seedSequence = std::seed_seq { seed.begin(), seed.end() };
    return Engine { seedSequence };
}

Accessor::Accessor(Engine& engine)
    : _engine(&engine)
{
}

float Accessor::GetNextValue()
{
    std::uniform_real_distribution dist { 0.0f, 1.0f };
    return dist(*_engine);
}

uint32_t Accessor::GetValue(const uint32_t start, const uint32_t end)
{
    std::uniform_int_distribution dist { start, end };
    return dist(*_engine);
}

}
