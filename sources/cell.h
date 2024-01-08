#pragma once

enum class CellId : uint32_t {
    Invalid = std::numeric_limits<uint32_t>::max()
};

constexpr auto CellIdToInt(CellId id) { return static_cast<std::underlying_type_t<CellId>>(id); }

enum class Type : uint8_t {
    Unit,
    Food,
    Wall,
    Dummy
};

std::string_view TypeToStr(Type type);

struct Cell {
    Type type { Type::Dummy };
    sf::Vector2u position;
};

struct Unit {
    CellId id;
};

struct Food {
    CellId id;
};

struct Wall {
    CellId id;
};
