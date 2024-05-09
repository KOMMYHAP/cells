#pragma once

#include "processor/processor_state.h"

struct SimulationParameters {
    common::Storage systems;
    sf::Time targetSimulationTime;
    uint8_t cellsCountPercentOfLimit { 0 };

    // virtual machine
    uint8_t systemInstructionPerStep { 0 };
    ProcessorStateWatcher processorStateWatcher;
};