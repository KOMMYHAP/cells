#include "brain_system.h"

BrainSystem::BrainSystem(uint32_t capacity)
    : _cells(capacity)
{
}

void BrainSystem::Create(CellId id, const Cell& cell)
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    _cells[index] = cell;
}

Cell& BrainSystem::Modify(CellId id)
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    return _cells[index];
}

const Cell& BrainSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _cells.size());
    return _cells[index];
}
