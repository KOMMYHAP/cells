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
    Type type;
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

class World {
public:
    World() = default;
    World(uint32_t cellsCount);

    CellId Create(Cell cell);
    void Update(CellId id, const Cell& cell);
    const Cell& Get(CellId id) const;
    void Remove(CellId id);

public:
    auto begin() { return MakeSpan().begin(); }
    auto end() { return MakeSpan().end(); }
    auto begin() const { return MakeSpan().begin(); }
    auto end() const { return MakeSpan().end(); }
    auto cbegin() const { return MakeSpan().begin(); }
    auto cend() const { return MakeSpan().end(); }

private:
    CellId MakeNextId();

    std::span<Cell> MakeSpan() { return std::span { _cells.begin(), _cells.end() }; }
    std::span<const Cell> MakeSpan() const { return std::span { _cells.begin(), _cells.end() }; }

    std::vector<Cell> _cells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
};