#pragma once

#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "processor/memory.h"

class BrainSystem {
public:
    BrainSystem(uint32_t capacity);

    void Create(CellId id, const Brain& cell);
    Brain& Access(CellId id);
    const Brain& Get(CellId id) const;

    Memory AccessMemory(CellId id);
    ConstMemory GetMemory(CellId id) const;

private:
    std::vector<Brain> _cells;
};