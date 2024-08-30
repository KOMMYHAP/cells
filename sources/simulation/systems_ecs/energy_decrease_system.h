#pragma once

#include "components/cell_energy.h"
#include "components/cell_energy_change.h"
#include "simulation/simulation_ecs_system.h"

class EnergyDecreaseSystem final : public SimulationEcsSystem<EnergyDecreaseSystem, CellEnergy, CellEnergyDecrease> {
public:
    explicit EnergyDecreaseSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(CellId id, CellEnergy& energy, CellEnergyDecrease& change);
};
