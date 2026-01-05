#include "field.h"

#include "asserts/assert.h"

static constexpr bool SwizzledPositionEnabled = false; //< It seems like swizzling has no performance effect. Should look at PIX later.
static constexpr int32_t SwizzledBlockSide = 64;

Field::Field(int32_t sizeX, int32_t sizeY)
    : _cells(sizeX * sizeY, false)
    , _sizeX(sizeX)
    , _sizeY(sizeY)
    , _blocksInRow(sizeX / SwizzledBlockSide)
{
    if constexpr (SwizzledPositionEnabled) {
        ASSERT(sizeX % SwizzledBlockSide == 0, "Size x must be multiple to swizzled block size!");
        ASSERT(sizeY % SwizzledBlockSide == 0, "Size x must be multiple to swizzled block size!");
    }
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

    return _cells.at(GetCellIndex(position));
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
    if constexpr (SwizzledPositionEnabled) {
        const int32_t blockIndexX = position.x / SwizzledBlockSide;
        const int32_t blockIndexY = position.y / SwizzledBlockSide;

        const int32_t blockOffsetX = position.x % SwizzledBlockSide;
        const int32_t blockOffsetY = position.y % SwizzledBlockSide;

        return _blocksInRow * blockIndexY * SwizzledBlockSide * SwizzledBlockSide + blockIndexX * SwizzledBlockSide * SwizzledBlockSide + blockOffsetY * SwizzledBlockSide + blockOffsetX;

    } else {
        return position.y * _sizeX + position.x;
    }
}