#include "cell_locator.h"

CellLocator::CellLocator(uint32_t width, uint32_t height)
    : _width(width)
    , _height(height)
    , _grid(NarrowCast<size_t>(width * height), CellId::Invalid)
{
}

void CellLocator::Move(const CellPosition position, const CellPosition newPosition)
{
    const uint32_t index = ToGridIndex(position);
    const uint32_t newIndex = ToGridIndex(newPosition);
    ASSERT(_grid[newIndex] == CellId::Invalid, "Two cells placed on the same position!");
    _grid[newIndex] = std::exchange(_grid[index], CellId::Invalid);
}

void CellLocator::Reset(const CellPosition position)
{
    const uint32_t index = ToGridIndex(position);
    ASSERT(_grid[index] != CellId::Invalid, "Specified position contains noone!");
    _grid[index] = CellId::Invalid;
}

CellId CellLocator::Find(CellPosition position) const
{
    const uint32_t index = ToGridIndex(position);
    return _grid[index];
}

CellPosition CellLocator::TryApplyDirection(CellPosition position, Direction direction) const
{
    switch (direction) {
    case Direction::Left:
        if (position.x >= 1) {
            position.x -= 1;
            return position;
        }
        break;
    case Direction::Right:
        if (position.x + 1 < GetWidth()) {
            position.x += 1;
            return position;
        }
        break;
    case Direction::Up:
        if (position.y >= 1) {
            position.y -= 1;
            return position;
        }
        break;
    case Direction::Down:
        if (position.y + 1 < GetHeight()) {
            position.y += 1;
            return position;
        }
        break;
    }

    return InvalidCellPosition;
}

bool CellLocator::IsNeighbourFor(CellPosition lhs, CellPosition rhs) const
{
    CellPosition delta = rhs;
    delta.x -= lhs.x;
    delta.y -= lhs.y;
    const uint16_t distanceSqr = delta.x * delta.x + delta.y * delta.y;
    return distanceSqr == 1;
}

uint32_t CellLocator::ToGridIndex(CellPosition position) const
{
    const uint32_t index = position.y * _width + position.x;
    ASSERT(index < _grid.size(), "Invalid cell position!");
    return index;
}

bool CellLocator::IsValidPosition(CellPosition position) const
{
    return position.x >= 0 && position.x < GetWidth() && position.y >= 0 && position.y < GetHeight();
}