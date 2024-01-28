#include "random.h"

static std::unique_ptr<common::RandomEngine> _randomEngine;

namespace common {

RandomEngine& GetRandomEngine()
{
    return *_randomEngine;
}

void InitRandom(std::string_view seed)
{
    auto seedSequence = std::seed_seq { seed.begin(), seed.end() };
    _randomEngine = std::make_unique<RandomEngine>(seedSequence);
}

void TermRandom()
{
    _randomEngine.reset();
}

}
