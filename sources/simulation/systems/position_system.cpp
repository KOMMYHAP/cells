#include "position_system.h"

PositionSystem::PositionSystem(uint32_t width, uint32_t height)
    : _grid(width, height)
    , _positions(width * height, InvalidCellPosition)
{
}

void PositionSystem::Move(CellId id, const CellPosition& position)
{
    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());

    const auto oldPosition = _positions[index];
    if (oldPosition != InvalidCellPosition) {
        auto positionMoveVector = oldPosition - position;
        assert(positionMoveVector.x * positionMoveVector.x + positionMoveVector.y * positionMoveVector.y == 1);
        _grid.Remove(id, oldPosition);
    }

    _positions[index] = position;
    _grid.Add(id, position);
}

const CellPosition& PositionSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _positions.size());
    return _positions[index];
}

CellId PositionSystem::Find(const CellPosition& position) const
{
    return _grid.Find(position);
}
