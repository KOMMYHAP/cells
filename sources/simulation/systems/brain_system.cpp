#include "brain_system.h"

BrainSystem::BrainSystem(uint32_t capacity)
    : _cells(capacity)
{
}

CellBrain& BrainSystem::Access(CellId id)
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    return _cells[index];
}

const CellBrain& BrainSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    return _cells[index];
}

ProcessorMemory BrainSystem::AccessMemory(CellId id)
{
    CellBrain& cell = Access(id);
    return ProcessorMemory(std::span(cell.data));
}

ProcessorConstMemory BrainSystem::GetMemory(CellId id) const
{
    const CellBrain& cell = Get(id);
    return ProcessorConstMemory(std::span(cell.data));
}
