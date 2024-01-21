#pragma once

#include "brain/cell.h"

class PositionGrid {
public:
    PositionGrid(uint32_t cellsInRow, uint32_t cellsInColumn);

    CellId Find(const CellPosition& position) const;

    void Add(CellId id, const CellPosition& position);
    void Remove(CellId id, const CellPosition& position);

private:
    inline static constexpr uint32_t InvalidGridIndex = std::numeric_limits<uint32_t>::max();

    uint32_t TryGetGridIndex(const CellPosition& position) const;

    const uint32_t _cellsInRow;
    const uint32_t _cellsInColumn;
    std::vector<CellId> _grid;
};
