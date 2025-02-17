#pragma once

#include "simulation/simulation_ecs_config.h"
#include "components/generated/cell_position.h"
#include "components/generated/direction.h"

class CellLocator {
public:
    CellLocator(int32_t width, int32_t height);

    void Set(CellPosition position, EcsEntity id);
    void Replace(CellPosition oldPosition, CellPosition newPosition);
    void Reset(CellPosition position);

    EcsEntity Find(CellPosition position) const;

    int32_t GetWidth() const { return _width; }
    int32_t GetHeight() const { return _height; }

    bool IsNeighbourFor(CellPosition lhs, CellPosition rhs) const;

    bool IsValidPosition(CellPosition position) const;
    CellPosition TryApplyDirection(CellPosition position, Direction direction) const;

private:
    int32_t ToGridIndex(CellPosition position) const;

    int32_t _width { 0 };
    int32_t _height { 0 };
    std::vector<EcsEntity> _grid;
};
