#include "random.h"

#include "mt/spin_lock.h"

namespace {

struct RandomData {
    common::RandomEngine engine;
    SpinLock spinlock;
};
RandomData* g_randomData { nullptr };

}

namespace common {

float GetNextValue()
{
    SpinLocker _ { g_randomData->spinlock };
    std::uniform_real_distribution dist { 0.0f, 1.0f };
    return dist(g_randomData->engine);
}

void InitRandom(ApplicationStorage& storage, std::string_view seed)
{
    auto seedSequence = std::seed_seq { seed.begin(), seed.end() };
    g_randomData = &storage.Store<RandomData>(RandomEngine(seedSequence), SpinLock {});
}

void TermRandom()
{
    g_randomData = nullptr;
}

}
