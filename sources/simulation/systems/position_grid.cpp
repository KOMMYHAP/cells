#include "position_grid.h"

/// Maybe try quadtree later this: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

constexpr uint32_t InvalidGridIndex = std::numeric_limits<uint32_t>::max();

PositionGrid::PositionGrid(uint16_t width, uint16_t height)
    : _width(width)
    , _height(height)
{
    _grid.resize(_width * _height, CellId::Invalid);
}

void PositionGrid::Add(CellId id, CellPosition position)
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return;
    }
    assert(_grid[index] == CellId::Invalid && id != CellId::Invalid);
    _grid[index] = id;
}

void PositionGrid::Remove(CellId id, CellPosition position)
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return;
    }
    assert(_grid[index] == id && id != CellId::Invalid);
    _grid[index] = CellId::Invalid;
}

CellId PositionGrid::Find(CellPosition position) const
{
    const uint32_t index = TryGetGridIndex(position);
    if (index != InvalidGridIndex) {
        return _grid[index];
    }
    return CellId::Invalid;
}

uint32_t PositionGrid::TryGetGridIndex(CellPosition position) const
{
    const uint32_t index = position.y * _width + position.x;
    if (index < _grid.size()) {
        return index;
    }
    return InvalidGridIndex;
}
