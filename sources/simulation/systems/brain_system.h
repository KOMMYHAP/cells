#pragma once

#include "components/cell_brain.h"
#include "components/cell_id.h"
#include "processor/processor_memory.h"

class BrainSystem {
public:
    BrainSystem(uint32_t capacity);

    CellBrain& Access(CellId id);
    const CellBrain& Get(CellId id) const;

    ProcessorMemory AccessMemory(CellId id);
    ProcessorConstMemory GetMemory(CellId id) const;

    void DumpCellBrain(std::ostream& stream, CellId id);

private:
    std::vector<CellBrain> _cells;
};