#include "world.h"

World::World(uint32_t cellsCount)
{
    // preallocate cells to avoid allocations during the game
    _freeIds.resize(cellsCount);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }
    _cells.reserve(cellsCount);
}

CellId World::MakeNextId()
{
    if (_nextId == CellId::Invalid) {
        assert(false);
        return _nextId;
    }

    CellId id = _nextId;
    _nextId = static_cast<CellId>(CellIdToInt(_nextId) + 1);
    return id;
}

CellId World::Create(Cell cell)
{
    CellId nextId;
    if (!_freeIds.empty()) {
        nextId = _freeIds.back();
        _freeIds.pop_back();
    } else {
        const auto id = MakeNextId();
        nextId = id;
    }

    const auto index = CellIdToInt(nextId);
    _cells.resize(index + 1);

    Update(nextId, cell);
    return nextId;
}

void World::Update(CellId id, const Cell& cell)
{
    const auto index = CellIdToInt(id);
    _cells[index] = cell;
}

const Cell& World::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    return _cells[index];
}

void World::Remove(CellId id)
{
    const auto index = CellIdToInt(id);
    _cells[index].type = Type::Dummy;
    _freeIds.push_back(id);
}

std::string_view TypeToStr(Type type)
{
    switch (type) {
    case Type::Unit:
        return "Unit";
    case Type::Food:
        return "Food";
    case Type::Wall:
        return "Wall";
    case Type::Dummy:
        return "Dummy";
    default:
        assert(false);
        return "unknown";
    }
}
