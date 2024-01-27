#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "position_grid.h"
#include "procedures/direction.h"

class PositionSystem {
public:
    PositionSystem(uint32_t width, uint32_t height);

    void Set(CellId id, CellPosition);

    CellPosition Get(CellId id) const;
    CellId Find(CellPosition position) const;

    template <class Func>
    void Iterate(Func&& func);

    auto GetWidth() const { return _grid.GetWidth(); }
    auto GetHeight() const { return _grid.GetHeight(); }

    bool IsNeighbourFor(CellId lhs, CellId rhs) const;
    bool IsNeighbourFor(CellPosition lhs, CellPosition rhs) const;
    CellPosition TryApplyDirection(CellPosition position, Direction direction) const;

private:
    void Move(CellId id, CellPosition nextPosition);
    void Reset(CellId id);

    std::vector<CellPosition> _positions;
    PositionGrid _grid;
};

#include "position_system.hpp"