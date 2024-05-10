#pragma once

#include "components/cell_age.h"

enum class SpawnPolicy {
    Random,
    Patrol
};

struct SimulationParameters {
    CellAge limitCellAge { 100 };
    uint16_t bestCellSelectionSize { 100 };
    uint16_t selectionEpochTicks { 1000 };
    SpawnPolicy spawnPolicy { SpawnPolicy::Random };
    sf::Time targetSimulationTime;
};
