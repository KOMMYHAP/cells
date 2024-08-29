#pragma once

#include "components/cell_energy.h"
#include "components/cell_energy_change.h"
#include "simulation/simulation_ecs_system.h"

class EnergySystem final : public SimulationEcsSystem<EnergySystem, CellEnergy, const CellEnergyChange> {
public:
    explicit EnergySystem(EcsWorld& ecsWorld);

    void DoProcessComponents(CellId id, CellEnergy& energy, CellEnergyChange change);
};
