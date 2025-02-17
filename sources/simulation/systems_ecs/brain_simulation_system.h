#pragma once

#include "simulation/simulation_ecs_system.h"
#include "simulation/simulation_virtual_machine.h"

#include "components/generated/auto_cell_brain.h"

class BrainSimulationSystem final : public SimulationEcsSystem<BrainSimulationSystem, CellBrain> {
public:
    explicit BrainSimulationSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm);

    void DoProcessComponents(EcsEntity id, CellBrain& brain);

private:
    gsl::not_null<SimulationVirtualMachine*> _vm;
};
