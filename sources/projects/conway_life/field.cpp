#include "field.h"

Field::Field(int32_t sizeX, int32_t sizeY)
    : _cells(sizeX * sizeY, false)
    , _sizeX(sizeX)
    , _sizeY(sizeY)
{
}

void Field::Set(CellPosition position, bool value)
{
    _cells[GetCellIndex(position)] = value;
}

bool Field::Has(CellPosition position) const
{
    if (position.x < 0 || position.y < 0) [[unlikely]] {
        return false;
    }
    if (position.x >= _sizeX || position.y >= _sizeY) [[unlikely]] {
        return false;
    }

    return _cells.at(GetCellIndex(position));
}

int32_t Field::GetCellIndex(CellPosition position) const
{
    return position.y * _sizeX + position.x;
}