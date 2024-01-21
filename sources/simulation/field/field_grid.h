#pragma once

#include "brain/cell.h"

class Field;

class FieldGrid {
public:
    FieldGrid(uint32_t cellsInRow, uint32_t cellsInColumn);

    CellId Find(const CellPosition& position) const;

    void Add(const CellPosition& position, CellId id);
    void Remove(const CellPosition& position, CellId id);

private:
    inline static constexpr uint32_t InvalidGridIndex = std::numeric_limits<uint32_t>::max();

    uint32_t TryGetGridIndex(const CellPosition& position) const;

    const uint32_t _cellsInRow;
    const uint32_t _cellsInColumn;
    std::vector<CellId> _grid;
};
