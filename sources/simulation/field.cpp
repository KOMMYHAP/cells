#include "field.h"
#include "brain.h"
#include "brain_view.h"

Field::Field(uint32_t cellRows, uint32_t cellColumns)
    : _searchProxy(*this, cellColumns, cellRows)
    , _cellRows(cellRows)
    , _cellColumns(cellColumns)
{
    const uint32_t cellsCount = cellRows * cellColumns;
    assert(cellsCount > 0);

    // preallocate cells to avoid allocations during the game
    _freeIds.resize(cellsCount);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }

    _emptyCell.GetBrain().AccessInfo().type = CellType::Dummy;
    _cells.resize(cellsCount, _emptyCell);
}

CellId Field::MakeNextId()
{
    if (_nextId == CellId::Invalid) {
        assert(false);
        return _nextId;
    }

    CellId id = _nextId;
    _nextId = static_cast<CellId>(CellIdToInt(_nextId) + 1);
    return id;
}

CellId Field::Create(const Cell& cell)
{
    CellId nextId;
    if (!_freeIds.empty()) {
        nextId = _freeIds.back();
        _freeIds.pop_back();
    } else {
        const auto id = MakeNextId();
        nextId = id;
    }

    const uint32_t index = CellIdToInt(nextId);
    _cells[index] = cell;
    _searchProxy.Add(nextId);

    return nextId;
}

void Field::Move(CellId id, const sf::Vector2<uint16_t>& position)
{
    _searchProxy.Remove(id);
    const auto index = CellIdToInt(id);
    _cells[index].GetBrain().AccessInfo().position = position;
    _searchProxy.Add(id);
}

const Cell& Field::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    return _cells[index];
}

void Field::Remove(CellId id)
{
    _searchProxy.Remove(id);
    const auto index = CellIdToInt(id);
    _cells[index].GetBrain().AccessInfo().type = CellType::Dummy;
    _freeIds.push_back(id);
}

std::vector<CellId> Field::Find(const sf::Vector2u& position) const
{
    return _searchProxy.Find(position);
}
