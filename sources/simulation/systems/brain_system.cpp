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

Memory BrainSystem::AccessMemory(CellId id)
{
    CellBrain& cell = Access(id);
    return Memory(std::span(cell.data));
}

ConstMemory BrainSystem::GetMemory(CellId id) const
{
    const CellBrain& cell = Get(id);
    return ConstMemory(std::span(cell.data));
}
