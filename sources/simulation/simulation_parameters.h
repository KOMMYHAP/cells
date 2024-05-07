#pragma once

#include "processor/processor_state.h"

struct SimulationParameters {
    sf::Time targetSimulationTime;
    uint8_t cellsCountPercentOfLimit { 0 };
    uint8_t systemInstructionPerStep { 0 };
    ProcessorStateWatcher processorStateWatcher;
};