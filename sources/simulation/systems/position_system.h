#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "procedures/direction.h"

class PositionSystem {
    using PositionType = std::decay_t<decltype(CellPosition::x)>;

public:
    PositionSystem(uint32_t width, uint32_t height);

    void Set(CellId id, CellPosition);

    CellPosition Get(CellId id) const;
    CellId Find(CellPosition position) const;

    template <class Func>
        requires std::invocable<Func, CellPosition>
    void Iterate(Func&& func) const;

    std::vector<CellPosition> CollectFreePositions() const;

    auto GetWidth() const { return _width; }
    auto GetHeight() const { return _height; }

    bool IsNeighbourFor(CellId lhs, CellId rhs) const;
    bool IsNeighbourFor(CellPosition lhs, CellPosition rhs) const;

    bool IsValidPosition(CellPosition position) const;
    CellPosition TryApplyDirection(CellPosition position, Direction direction) const;

private:
    void Move(CellId id, CellPosition nextPosition);
    void Reset(CellId id);

    uint32_t ToGridIndex(CellPosition position) const;
    uint32_t TryGetGridIndex(CellPosition position) const;

    const PositionType _width;
    const PositionType _height;

    std::vector<CellPosition> _positions;
    std::vector<CellId> _grid;
};

#include "position_system.hpp"