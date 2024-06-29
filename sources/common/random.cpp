#include "random.h"

#include "mt/spin_lock.h"

static common::RandomData* g_randomData { nullptr };

namespace common {

struct RandomData {
    common::RandomEngine engine;
    std::mutex lock;
};

float GetNextValue()
{
    std::scoped_lock _ { g_randomData->lock };
    std::uniform_real_distribution dist { 0.0f, 1.0f };
    return dist(g_randomData->engine);
}

RandomAccessor::RandomAccessor()
{
    g_randomData->lock.lock();
}

RandomAccessor::~RandomAccessor()
{
    g_randomData->lock.unlock();
}

RandomEngine& RandomAccessor::AccessEngine()
{
    return g_randomData->engine;
}

void InitRandom(ApplicationStorage& storage, std::string_view seed)
{
    auto seedSequence = std::seed_seq { seed.begin(), seed.end() };
    g_randomData = &storage.Store<RandomData>(RandomEngine(seedSequence), std::mutex {});
}

void TermRandom()
{
    g_randomData = nullptr;
}

}
