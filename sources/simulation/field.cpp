#include "field.h"
#include "brain.h"
#include "brain_view.h"

Field::Field(uint32_t cellRows, uint32_t cellColumns, const Cell& empty)
    : _searchProxy(*this, cellColumns, cellRows)
    , _cellRows(cellRows)
    , _cellColumns(cellColumns)
    , _emptyCell(empty)
{
    const uint32_t cellsCount = cellRows * cellColumns;

    // preallocate cells to avoid allocations during the game
    _freeIds.resize(cellsCount);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }
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

void Field::Move(CellId id, const Cell& cell)
{
    assert(_cellRows > cell.GetBrain().GetPosition().y);
    assert(_cellColumns > cell.GetBrain().GetPosition().x);

    const auto index = CellIdToInt(id);

    _searchProxy.Remove(id);
    _cells[index] = cell;
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
    _cells[index].GetBrain().AccessType() = CellType::Dummy;
    _freeIds.push_back(id);
}

std::vector<CellId> Field::Find(const sf::Vector2u& position) const
{
    return _searchProxy.Find(position);
}
