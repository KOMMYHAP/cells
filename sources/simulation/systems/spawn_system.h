#pragma once

#include "components/cell_id.h"

class SimulationVirtualMachine;
class BrainSystem;
class ProcessorMemory;

class SpawnSystem {
public:
    SpawnSystem(SimulationVirtualMachine& vm, BrainSystem& brainSystem);

    bool MakePatrolUnit(CellId id, uint16_t length);
    bool MakeRandomUnit(CellId id, uint16_t lengthBytes);

private:
    bool InitMemory(ProcessorMemory& memory);

    SimulationVirtualMachine& _vm;
    BrainSystem& _brainSystem;
};
