#include "position_system.h"

constexpr uint32_t InvalidGridIndex = std::numeric_limits<uint32_t>::max();

PositionSystem::PositionSystem(uint32_t width, uint32_t height)
    : _width(width)
    , _height(height)
    , _positions(width * height, InvalidCellPosition)
    , _grid(width * height, CellId::Invalid)
{
}

void PositionSystem::Move(CellId id, CellPosition nextPosition)
{
    assert(nextPosition != InvalidCellPosition);
    assert(Find(nextPosition) == CellId::Invalid);

    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());

    const auto currentPosition = _positions[index];
    if (currentPosition != InvalidCellPosition) {
        assert(IsNeighbourFor(currentPosition, nextPosition));
        const uint32_t currentGridIndex = ToGridIndex(currentPosition);
        _grid[currentGridIndex] = CellId::Invalid;
    }

    _positions[index] = nextPosition;
    const uint32_t nextGridIndex = ToGridIndex(nextPosition);
    _grid[nextGridIndex] = id;
}

CellPosition PositionSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());
    return _positions[index];
}

CellId PositionSystem::Find(CellPosition position) const
{
    const uint32_t index = TryGetGridIndex(position);
    if (index == InvalidGridIndex) {
        return CellId::Invalid;
    }
    return _grid[index];
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
    const auto positionIndex = CellIdToInt(id);
    assert(positionIndex <= _positions.size());

    const auto position = _positions[positionIndex];
    if (position == InvalidCellPosition) {
        return;
    }
    _positions[positionIndex] = InvalidCellPosition;

    const uint32_t gridIndex = ToGridIndex(position);
    _grid[gridIndex] = CellId::Invalid;
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

std::vector<CellPosition> PositionSystem::CollectFreePositions() const
{
    std::vector<CellPosition> freePosition;
    freePosition.reserve(_grid.size());
    Iterate([&](const CellPosition position) {
        const uint32_t index = ToGridIndex(position);
        const CellId id = _grid[index];
        const bool isBusy = id != CellId::Invalid;
        if (isBusy) {
            return;
        }
        freePosition.emplace_back(position);
    });
    return freePosition;
}

uint32_t PositionSystem::ToGridIndex(CellPosition position) const
{
    return position.y * _width + position.x;
}

uint32_t PositionSystem::TryGetGridIndex(CellPosition position) const
{
    const uint32_t index = ToGridIndex(position);
    if (index < _grid.size()) {
        return index;
    }
    return InvalidGridIndex;
}