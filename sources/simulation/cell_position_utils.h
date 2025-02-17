#pragma once
#include "components/generated/auto_cell_position.h"

inline bool operator==(const CellPosition& lhs, const CellPosition& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator<(const CellPosition& lhs, const CellPosition& rhs)
{
    if (lhs.x == rhs.x) {
        return lhs.y < rhs.y;
    }
    return lhs.x < rhs.x;
}

inline constexpr CellPosition InvalidCellPosition { CellPosition::Invalid, CellPosition::Invalid };