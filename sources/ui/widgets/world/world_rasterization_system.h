#pragma once

#include "simulation/simulation_ecs_system.h"

#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_cell_type.h"

class WorldRasterizationTarget;
struct SDL_Color;

class WorldRasterizationSystem final : public SimulationEcsSystem<WorldRasterizationSystem, const CellType, const CellPosition> {
public:
    WorldRasterizationSystem(EcsWorld& ecsWorld, WorldRasterizationTarget& target, uint16_t cellSize);

    void DoProcessComponents(EcsEntity id, CellType type, CellPosition position);

private:
    SDL_Color GetColor(CellType type) const;

    int16_t _cellSizeInPixels { 0 };
    gsl::not_null<WorldRasterizationTarget*> _target;
};
