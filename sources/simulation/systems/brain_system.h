#pragma once

#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "processor/memory.h"

class BrainSystem {
public:
    BrainSystem(uint32_t capacity);

    void Create(CellId id, const CellBrain& cell);
    CellBrain& Access(CellId id);
    const CellBrain& Get(CellId id) const;

    Memory AccessMemory(CellId id);
    ConstMemory GetMemory(CellId id) const;

private:
    std::vector<CellBrain> _cells;
};