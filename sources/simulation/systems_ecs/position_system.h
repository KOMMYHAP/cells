#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "components/move_direction.h"

class PositionSystem {
    using PositionType = std::decay_t<decltype(CellPosition::x)>;

public:
    PositionSystem(uint32_t width, uint32_t height);

    void Set(CellId id, CellPosition position);

    CellPosition Get(CellId id) const;
    CellId Find(CellPosition position) const;

    std::vector<CellPosition> CollectFreePositions() const;

    auto GetWidth() const { return _width; }
    auto GetHeight() const { return _height; }

    bool IsNeighbourFor(CellId lhs, CellId rhs) const;
    bool IsNeighbourFor(CellPosition lhs, CellPosition rhs) const;

    bool IsValidPosition(CellPosition position) const;
    CellPosition TryApplyDirection(CellPosition position, MoveDirection direction) const;

private:
    void Move(CellId id, CellPosition nextPosition);
    void Reset(CellId id);

    uint32_t ToGridIndex(CellPosition position) const;
    uint32_t TryGetGridIndex(CellPosition position) const;

    uint32_t _width{0};
    uint32_t _height{0};

    std::vector<CellPosition> _positions;
    std::vector<CellId> _grid;
};
