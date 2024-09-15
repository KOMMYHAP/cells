#pragma once

#include "simulation/simulation_ecs_system.h"
#include "world_rasterization_data.h"

#include "components/cell_position.h"
#include "components/cell_type.h"

class WorldRasterizationSystem final : public SimulationEcsSystem<WorldRasterizationSystem, const CellType, const CellPosition> {
public:
    explicit WorldRasterizationSystem(EcsWorld& ecsWorld, uint16_t cellSize);

    void SetDestination(const WorldRasterizationData& rasterizationData);
    void ResetDestination();

    void DoProcessComponents(EcsEntity id, CellType type, CellPosition position);

private:
    uint32_t GetColor(CellType type) const;

    const WorldRasterizationData* _rasterizationData { nullptr };
    uint16_t _cellSizeInPixels { 0 };
};
