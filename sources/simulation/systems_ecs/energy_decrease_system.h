#pragma once

#include "components/generated/auto_cell_energy.h"
#include "components/generated/auto_cell_energy_change.h"
#include "simulation/simulation_ecs_system.h"

class EnergyDecreaseSystem final : public SimulationEcsSystem<EnergyDecreaseSystem, CellEnergy, CellEnergyDecrease> {
public:
    explicit EnergyDecreaseSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(EcsEntity id, CellEnergy& energy, CellEnergyDecrease& change);
};
