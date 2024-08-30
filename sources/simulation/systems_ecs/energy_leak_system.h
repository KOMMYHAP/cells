#pragma once

#include "components/cell_energy_change.h"
#include "simulation/simulation_ecs_system.h"

class EnergyLeakSystem final : public SimulationEcsSystem<EnergyLeakSystem, CellEnergyDecrease> {
public:
    explicit EnergyLeakSystem(EcsWorld& ecsWorld);

    void DoProcessComponents(CellId id, CellEnergyDecrease& change);
};
