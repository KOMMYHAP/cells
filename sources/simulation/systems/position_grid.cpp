#include "position_grid.h"
#include "brain/brain.h"

/// Maybe try quadtree later this: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

PositionGrid::PositionGrid(uint32_t cellsInRow, uint32_t cellsInColumn)
    : _cellsInRow(cellsInRow)
    , _cellsInColumn(cellsInColumn)
{
    _grid.resize(_cellsInRow * _cellsInColumn, CellId::Invalid);
}

void PositionGrid::Add(CellId id, const CellPosition& position)
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return;
    }
    assert(_grid[index] == CellId::Invalid && id != CellId::Invalid);
    _grid[index] = id;
}

void PositionGrid::Remove(CellId id, const CellPosition& position)
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return;
    }
    assert(_grid[index] == id && id != CellId::Invalid);
    _grid[index] = CellId::Invalid;
}

CellId PositionGrid::Find(const CellPosition& position) const
{
    const uint32_t index = TryGetGridIndex(position);
    if (index != InvalidGridIndex) {
        return _grid[index];
    }
    return CellId::Invalid;
}

uint32_t PositionGrid::TryGetGridIndex(const CellPosition& position) const
{
    const uint32_t index = position.y * _cellsInRow + position.x;
    if (index < _grid.size()) {
        return index;
    }
    return InvalidGridIndex;
}
