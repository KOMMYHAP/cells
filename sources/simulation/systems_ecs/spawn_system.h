#pragma once

#include "simulation/simulation_ecs_system.h"

#include "components/cell_position.h"
#include "components/spawn_place_tag.h"
#include "simulation/cell_locator.h"

class SpawnSystem final : public SimulationEcsSystem<SpawnSystem, SpawnPlaceTag, const CellPosition> {
public:
    SpawnSystem(EcsWorld& world, CellLocator& locator);
    void DoProcessComponents(CellId id, CellPosition position);

private:
    gsl::not_null<CellLocator*> _locator;
};