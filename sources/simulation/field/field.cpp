#include "field.h"
#include "brain/brain.h"

Field::Field(uint32_t cellRows, uint32_t cellColumns, uint32_t cellsPerPoint)
    : _searchProxy(*this, cellColumns, cellRows, cellsPerPoint)
    , _cellRows(cellRows)
    , _cellColumns(cellColumns)
{
    const uint32_t cellsCount = cellRows * cellColumns;
    assert(cellsCount > 0);

    // preallocate cellsPerPoint to avoid allocations during the game
    _freeIds.resize(cellsCount);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }

    Brain brain { _emptyCell };
    brain.AccessInfo().type = CellType::Dummy;
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
    Cell& cell = _cells[index];
    Brain brain(cell);
    brain.AccessInfo().position = position;
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
    Cell& cell = _cells[index];
    Brain brain(cell);
    brain.AccessInfo().type = CellType::Dummy;
    _freeIds.push_back(id);
}

std::span<const CellId> Field::Find(const sf::Vector2u& position, uint32_t searchSizeLimit /*= std::numeric_limits<uint32_t>::max()*/) const
{
    return _searchProxy.Find(position, searchSizeLimit);
}

uint32_t Field::GetCellsCount() const
{
    assert(_cells.size() >= _freeIds.size());
    return _cells.size() - _freeIds.size();
}
