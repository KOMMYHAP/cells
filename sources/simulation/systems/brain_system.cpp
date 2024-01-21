#include "brain_system.h"

BrainSystem::BrainSystem(uint32_t capacity)
    : _cells(capacity)
{
}

void BrainSystem::Create(CellId id, const Brain& cell)
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    _cells[index] = cell;
}

Brain& BrainSystem::Access(CellId id)
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    return _cells[index];
}

const Brain& BrainSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    return _cells[index];
}

Memory BrainSystem::AccessMemory(CellId id)
{
    Brain& cell = Access(id);
    return Memory(std::span(cell.data));
}

ConstMemory BrainSystem::GetMemory(CellId id) const
{
    const Brain& cell = Get(id);
    return ConstMemory(std::span(cell.data));
}
