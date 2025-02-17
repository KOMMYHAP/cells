#pragma once
#include "generated/auto_cell_position.h"

inline auto operator<=>(const CellPosition& lhs, const CellPosition& rhs) const = default;

inline constexpr CellPosition InvalidCellPosition { CellPosition::Invalid, CellPosition::Invalid };