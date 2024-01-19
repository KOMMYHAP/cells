#pragma once
#include "brain/cell.h"
#include "field_search_proxy.h"

class Field {
    friend class FieldIterator;
    friend class ConstFieldIterator;

public:
    Field(uint16_t cellsInColumn, uint16_t cellsInRow);

    CellId Create(const Cell& cell);
    void Move(CellId id, const CellPosition& position);
    const Cell& Get(CellId id) const;
    Cell& Modify(CellId id);
    std::span<const CellId> FindAll(const CellPosition& position, uint32_t searchSizeLimit = std::numeric_limits<uint32_t>::max()) const;
    CellId Find(const CellPosition& position) const;
    void Remove(CellId id);

    CellPosition GetPositionLimits() const { return { static_cast<uint16_t>(GetColumnsCount() - 1), static_cast<uint16_t>(GetRowsCount() - 1) }; }
    uint16_t GetRowsCount() const { return _cellRows; }
    uint16_t GetColumnsCount() const { return _cellColumns; }

    uint32_t GetCellsCount() const;

    template <class Func>
    void IterateAllCells(Func&& func) const
    {
        _searchProxy.ViewAllCell(std::forward<Func>(func));
    }

private:
    CellId MakeNextId();

private:
    std::vector<Cell> _cells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
    FieldGrid _searchProxy;
    uint16_t _cellRows { 0 };
    uint16_t _cellColumns { 0 };
};