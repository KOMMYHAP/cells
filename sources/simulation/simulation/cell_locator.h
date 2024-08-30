#pragma once

#include "simulation/simulation_ecs_config.h"
#include "components/cell_position.h"
#include "components/direction.h"

class CellLocator {
public:
    CellLocator(uint32_t width, uint32_t height);

    void Set(CellPosition position, EcsEntity id);
    void Replace(CellPosition oldPosition, CellPosition newPosition);
    void Reset(CellPosition position);

    EcsEntity Find(CellPosition position) const;

    uint32_t GetWidth() const { return _width; }
    uint32_t GetHeight() const { return _height; }

    bool IsNeighbourFor(CellPosition lhs, CellPosition rhs) const;

    bool IsValidPosition(CellPosition position) const;
    CellPosition TryApplyDirection(CellPosition position, Direction direction) const;

private:
    uint32_t ToGridIndex(CellPosition position) const;

    uint32_t _width { 0 };
    uint32_t _height { 0 };
    std::vector<EcsEntity> _grid;
};
