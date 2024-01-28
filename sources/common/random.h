#pragma once

namespace common {

using RandomEngine = std::default_random_engine;

void InitRandom(std::string_view seed);
void TermRandom();

RandomEngine& GetRandomEngine();

}