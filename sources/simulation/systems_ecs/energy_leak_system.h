#pragma once

#include "components/generated/auto_cell_energy_change.h"
#include "simulation/simulation_ecs_system.h"

class EnergyLeakSystem final : public SimulationEcsSystem<EnergyLeakSystem, CellEnergyDecrease> {
public:
    explicit EnergyLeakSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(EcsEntity id, CellEnergyDecrease& change);
};
