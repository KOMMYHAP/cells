#pragma once
#include "brain/cell.h"
#include "field_grid.h"

class Field {
public:
    Field(uint16_t cellsInColumn, uint16_t cellsInRow);

    CellId Create(const Cell& cell);
    void Move(CellId id, const CellPosition& position);
    const Cell& Get(CellId id) const;
    Cell& Modify(CellId id);
    CellId Find(const CellPosition& position) const;
    void Remove(CellId id);

    CellPosition GetPositionLimits() const { return { static_cast<int16_t>(GetColumnsCount() - 1), static_cast<int16_t>(GetRowsCount() - 1) }; }
    uint16_t GetRowsCount() const { return _cellRows; }
    uint16_t GetColumnsCount() const { return _cellColumns; }

    uint32_t GetCellsCount() const;

    template <class Func>
        requires std::invocable<Func, CellId, const Cell&>
    void IterateByData(Func&& func) const
    {
        const uint32_t cellsCount = _cells.size();
        for (uint32_t idx { 0 }; idx < cellsCount; ++idx) {
            const CellId id = static_cast<CellId>(idx);
            const Cell& cell = _cells[idx];
            func(id, cell);
        }
    }

    template <class Func>
        requires std::invocable<Func, CellId, Cell&>
    void IterateByData(Func&& func)
    {
        const uint32_t cellsCount = _cells.size();
        for (uint32_t idx { 0 }; idx < cellsCount; ++idx) {
            const CellId id = static_cast<CellId>(idx);
            Cell& cell = _cells[idx];
            func(id, cell);
        }
    }

private:
    CellId MakeNextId();

private:
    std::vector<Cell> _cells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
    FieldGrid _grid;
    uint16_t _cellRows { 0 };
    uint16_t _cellColumns { 0 };
};