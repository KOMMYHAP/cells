#include "field.h"

Field::Field(int32_t sizeX, int32_t sizeY)
    : _cells(sizeX * sizeY, false)
    , _sizeX(sizeX)
    , _sizeY(sizeY)
{
}

void Field::Set(FieldPosition position, bool value)
{
    _cells[GetCellIndex(position)] = value;
}

bool Field::Has(FieldPosition position) const
{
    if (position.x < 0 || position.y < 0) [[unlikely]] {
        return false;
    }
    if (position.x >= _sizeX || position.y >= _sizeY) [[unlikely]] {
        return false;
    }

    return _cells[GetCellIndex(position)];
}

int32_t Field::GetNeighboursCount(FieldPosition position) const
{
    int32_t count = 0;

    count += Has({ position.x - 1, position.y - 1 }) ? 1 : 0;
    count += Has({ position.x + 0, position.y - 1 }) ? 1 : 0;
    count += Has({ position.x + 1, position.y - 1 }) ? 1 : 0;

    count += Has({ position.x - 1, position.y + 1 }) ? 1 : 0;
    count += Has({ position.x + 0, position.y + 1 }) ? 1 : 0;
    count += Has({ position.x + 1, position.y + 1 }) ? 1 : 0;

    count += Has({ position.x - 1, position.y + 0 }) ? 1 : 0;
    count += Has({ position.x + 1, position.y + 0 }) ? 1 : 0;

    return count;
}

int32_t Field::GetCellIndex(FieldPosition position) const
{
    return position.y * _sizeX + position.x;
}