#pragma once

#include "simulation_ecs_system.h"
#include "simulation/simulation_virtual_machine.h"

#include "components/cell_brain.h"

class BrainSimulationSystem final : public SimulationEcsSystem<BrainSimulationSystem, CellBrain> {
public:
    explicit BrainSimulationSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm);

    void DoProcessComponents(CellId id, CellBrain& brain);

private:
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
