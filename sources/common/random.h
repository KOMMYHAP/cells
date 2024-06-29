#pragma once
#include "application_storage.h"
#include <xutility>

namespace common {

struct RandomData;

using RandomEngine = std::default_random_engine;

void InitRandom(ApplicationStorage& storage, std::string_view seed);
void TermRandom();

float GetNextValue();

class RandomAccessor {
public:
    RandomAccessor();
    RandomAccessor(const RandomAccessor&) = delete;
    RandomAccessor(RandomAccessor&&) = delete;
    RandomAccessor& operator=(const RandomAccessor&) = delete;
    RandomAccessor& operator=(RandomAccessor&&) = delete;
    ~RandomAccessor();

    RandomEngine& AccessEngine();
};

template <std::ranges::random_access_range Ranges>
    requires std::permutable<std::ranges::iterator_t<Ranges>>
void Shuffle(Ranges&& ranges)
{
    std::ranges::shuffle(std::forward<Ranges>(ranges), RandomAccessor().AccessEngine());
}

}