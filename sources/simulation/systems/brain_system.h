#pragma once

#include "brain/cell.h"

class BrainSystem {
public:
    BrainSystem(uint32_t capacity);

    void Create(CellId id, const Cell& cell);
    Cell& Modify(CellId id);
    const Cell& Get(CellId id) const;

private:
    std::vector<Cell> _cells;
};