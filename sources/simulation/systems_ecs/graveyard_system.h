#pragma once

#include "simulation/cell_locator.h"
#include "simulation/simulation_ecs_system.h"

// todo: reuse SimulationEcsSystem, when it can get exclude components outside
class GraveyardSystem final : public SimulationSystem {
public:
    explicit GraveyardSystem(EcsWorld& world, CellLocator& locator);

    void DoSystemUpdate() override;

private:
    gsl::not_null<EcsWorld*> _world;
    gsl::not_null<CellLocator*> _locator;
};
