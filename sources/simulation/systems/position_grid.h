#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"

class PositionGrid {
public:
    using PositionType = decltype(CellPosition::x);

public:
    PositionGrid(uint16_t width, uint16_t height);

    CellId Find(CellPosition position) const;

    void Add(CellId id, CellPosition position);
    void Remove(CellId id, CellPosition position);

    PositionType GetWidth() const { return _width; }
    PositionType GetHeight() const { return _height; }

    template <class Func>
        requires std::invocable<Func, CellPosition>
    void Iterate(Func&& func);

private:
    uint32_t TryGetGridIndex(CellPosition position) const;

    const PositionType _width;
    const PositionType _height;
    std::vector<CellId> _grid;
};

#include "position_grid.hpp"