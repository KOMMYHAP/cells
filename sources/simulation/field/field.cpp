#include "field.h"
#include "brain/brain.h"

Field::Field(uint16_t cellsInColumn, uint16_t cellsInRow)
    : _grid(cellsInRow, cellsInColumn)
    , _cellRows(cellsInColumn)
    , _cellColumns(cellsInRow)
{
    const uint32_t cellsCount = cellsInColumn * cellsInRow;
    assert(cellsCount > 0);

    // preallocate cellsPerPoint to avoid allocations during the game
    _freeIds.resize(cellsCount);
    for (auto it = _freeIds.rbegin(); it != _freeIds.rend(); ++it) {
        *it = MakeNextId();
    }

    Cell emptyCell;
    Brain brain { emptyCell };
    brain.AccessInfo().type = CellType::Dummy;
    _cells.resize(cellsCount, emptyCell);
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
    CellId id;
    if (!_freeIds.empty()) {
        id = _freeIds.back();
        _freeIds.pop_back();
    } else {
        const CellId nextId = MakeNextId();
        id = nextId;
    }

    const uint32_t index = CellIdToInt(id);
    _cells[index] = cell;

    ConstBrain brain { cell };
    _grid.Add(brain.GetInfo().position, id);

    return id;
}

void Field::Move(CellId id, const CellPosition& position)
{
    const auto index = CellIdToInt(id);
    Cell& cell = _cells[index];
    Brain brain(cell);
    const CellPosition& oldPosition = brain.GetInfo().position;

    auto positionMoveVector = oldPosition - position;
    assert(positionMoveVector.x * positionMoveVector.x + positionMoveVector.y * positionMoveVector.y == 1);

    _grid.Remove(oldPosition, id);
    brain.AccessInfo().position = position;
    _grid.Add(position, id);
}

const Cell& Field::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    return _cells[index];
}

Cell& Field::Modify(CellId id)
{
    const auto index = CellIdToInt(id);
    return _cells[index];
}

void Field::Remove(CellId id)
{
    const auto index = CellIdToInt(id);
    Cell& cell = _cells[index];
    Brain brain(cell);
    _grid.Remove(brain.GetInfo().position, id);
    brain.AccessInfo().type = CellType::Dummy;
    _freeIds.push_back(id);
}

std::span<const CellId> Field::FindAll(const CellPosition& position, uint32_t searchSizeLimit /*= std::numeric_limits<uint32_t>::max()*/) const
{
    return _grid.FindAll(position, searchSizeLimit);
}

uint32_t Field::GetCellsCount() const
{
    assert(_cells.size() >= _freeIds.size());
    return _cells.size() - _freeIds.size();
}

CellId Field::Find(const CellPosition& position) const
{
    return _grid.Find(position);
}
