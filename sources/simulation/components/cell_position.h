#pragma once

struct CellPosition {
    int16_t x;
    int16_t y;

    auto operator<=>(const CellPosition& other) const = default;
};

inline constexpr static CellPosition InvalidCellPosition { std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::max() };