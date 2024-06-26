#pragma once

struct CellPosition {
    int16_t x;
    int16_t y;

    auto operator<=>(const CellPosition& other) const = default;
};

struct CurrentPosition {
    int16_t x;
    int16_t y;
};

struct NextPosition {
    int16_t x;
    int16_t y;
};

inline constexpr static CellPosition InvalidCellPosition { std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::max() };