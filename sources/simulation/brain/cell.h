#pragma once

enum class CellId : uint32_t {
    Invalid = std::numeric_limits<uint32_t>::max()
};

using CellPosition = sf::Vector2<int16_t>;

constexpr auto CellIdToInt(CellId id) { return static_cast<std::underlying_type_t<CellId>>(id); }

struct Cell {
    static constexpr uint8_t brainSize = 255;
    std::byte brain[brainSize] {};
};
