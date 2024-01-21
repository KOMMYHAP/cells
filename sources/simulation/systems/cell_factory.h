#pragma once

#include "brain_system.h"
#include "components/cell_id.h"

class SimulationVirtualMachine;
class BrainSystem;

class CellFactory {
public:
    CellFactory(SimulationVirtualMachine& vm, BrainSystem& brainSystem);

    bool MakePatrolUnit(CellId id, uint16_t length);

private:
    bool InitMemory(Memory& memory);

    SimulationVirtualMachine& _vm;
    BrainSystem& _brainSystem;
};
