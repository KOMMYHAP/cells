#include "world.h"

World::World(uint32_t cellRows, uint32_t cellColumns)
    : _searchProxy(*this, cellColumns, cellRows)
    , _cellRows(cellRows)
    , _cellColumns(cellColumns)
{
    const uint32_t cellsCount = cellRows * cellColumns;

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
    assert(_cellRows > cell.position.y);
    assert(_cellColumns > cell.position.x);

    const auto index = CellIdToInt(id);

    const Cell& oldCell = _cells[index];
    if (oldCell.type != Type::Dummy) {
        _searchProxy.Remove(id);
    }
    _cells[index] = cell;
    _searchProxy.Add(id);
}

const Cell& World::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    return _cells[index];
}

void World::Remove(CellId id)
{
    const auto index = CellIdToInt(id);
    _searchProxy.Remove(id);
    _cells[index].type = Type::Dummy;
    _freeIds.push_back(id);
}

std::vector<CellId> World::Find(const sf::Vector2u& position) const
{
    return _searchProxy.Find(position);
}
