#include "position_system.h"

PositionSystem::PositionSystem(uint32_t width, uint32_t height)
    : _grid(width, height)
    , _positions(width * height, InvalidCellPosition)
{
}

void PositionSystem::Move(CellId id, CellPosition nextPosition)
{
    assert(nextPosition != InvalidCellPosition);
    assert(_grid.Find(nextPosition) == CellId::Invalid);

    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());

    const auto currentPosition = _positions[index];
    if (currentPosition != InvalidCellPosition) {
        assert(IsNeighbourFor(currentPosition, nextPosition));
        _grid.Remove(id, currentPosition);
    }

    _positions[index] = nextPosition;
    _grid.Add(id, nextPosition);
}

CellPosition PositionSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());
    return _positions[index];
}

CellId PositionSystem::Find(CellPosition position) const
{
    return _grid.Find(position);
}

bool PositionSystem::IsNeighbourFor(CellId lhs, CellId rhs) const
{
    const CellPosition lhsPosition = Get(lhs);
    assert(lhsPosition != InvalidCellPosition);

    const CellPosition rhsPosition = Get(rhs);
    assert(rhsPosition != InvalidCellPosition);

    return IsNeighbourFor(lhsPosition, rhsPosition);
}

CellPosition PositionSystem::TryApplyDirection(CellPosition position, Direction direction) const
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
    default:
        assert(false);
        return InvalidCellPosition;
    }
    return InvalidCellPosition;
}

void PositionSystem::Reset(CellId id)
{
    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());

    const auto position = _positions[index];
    if (position == InvalidCellPosition) {
        return;
    }
    _positions[index] = InvalidCellPosition;
    _grid.Remove(id, position);
}

bool PositionSystem::IsNeighbourFor(CellPosition lhs, CellPosition rhs) const
{
    auto positionMoveVector = rhs - lhs;
    const uint16_t distanceSqr = positionMoveVector.x * positionMoveVector.x + positionMoveVector.y * positionMoveVector.y;
    return distanceSqr == 1;
}

void PositionSystem::Set(CellId id, CellPosition position)
{
    if (position != InvalidCellPosition) {
        Move(id, position);
    } else {
        Reset(id);
    }
}
