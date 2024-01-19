#include "field_search_proxy.h"
#include "brain/brain.h"
#include "field.h"

/// Maybe try quadtree later this: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

FieldGrid::FieldGrid(uint32_t cellsInRow, uint32_t cellsInColumn)
    : _cellsInRow(cellsInRow)
    , _cellsInColumn(cellsInColumn)
{
    _grid.resize(_cellsInRow * _cellsInColumn, CellId::Invalid);
}

std::span<const CellId> FieldGrid::FindAll(const CellPosition& position, uint32_t /*searchSizeLimit*/) const
{
    static std::vector<CellId> stub { 1, CellId::Invalid };
    stub[0] = Find(position);
    return std::span(stub);
}

void FieldGrid::Add(const CellPosition& position, CellId id)
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return;
    }
    assert(_grid[index] == CellId::Invalid && id != CellId::Invalid);
    _grid[index] = id;
}

void FieldGrid::Remove(const CellPosition& position, CellId id)
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return;
    }
    assert(_grid[index] == id && id != CellId::Invalid);
    _grid[index] = CellId::Invalid;
}

CellId FieldGrid::Find(const CellPosition& position) const
{
    const uint32_t index = TryGetGridIndex(position);
    if (index != InvalidGridIndex) {
        return _grid[index];
    }
    return CellId::Invalid;
}

uint32_t FieldGrid::TryGetGridIndex(const CellPosition& position) const
{
    const uint32_t index = GetGridIndex(position);
    if (index < _grid.size()) {
        return index;
    }
    return InvalidGridIndex;
}

CellPosition FieldGrid::GetCellPosition(uint32_t gridIndex) const
{
    const uint16_t x = gridIndex % _cellsInRow;
    const uint16_t y = gridIndex / _cellsInRow;
    return { x, y };
}

uint32_t FieldGrid::GetGridIndex(const CellPosition& position) const
{
    return position.y * _cellsInRow + position.x;
}
