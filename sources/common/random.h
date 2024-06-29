#pragma once
#include "application_storage.h"

namespace common {

using RandomEngine = std::default_random_engine;

void InitRandom(ApplicationStorage& storage, std::string_view seed);
void TermRandom();

float GetNextValue();

}