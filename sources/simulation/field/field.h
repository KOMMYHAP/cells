#pragma once
#include "brain/cell.h"
#include "field_search_proxy.h"

class Field {
    friend class FieldIterator;
    friend class ConstFieldIterator;

public:
    Field(uint32_t cellRows, uint32_t cellColumns, uint32_t cellsPerPoint);

    CellId Create(const Cell& cell);
    void Move(CellId id, const sf::Vector2<uint16_t>& position);
    const Cell& Get(CellId id) const;
    std::span<const CellId> Find(const sf::Vector2u& position, uint32_t searchSizeLimit = std::numeric_limits<uint32_t>::max()) const;
    void Remove(CellId id);

    sf::Vector2u GetPositionLimits() const { return { GetColumnsCount() - 1, GetRowsCount() - 1 }; }
    uint32_t GetRowsCount() const { return _cellRows; }
    uint32_t GetColumnsCount() const { return _cellColumns; }

    uint32_t GetCellsCount() const;

private:
    CellId MakeNextId();

private:
    std::vector<Cell> _cells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
    FieldSearchProxy _searchProxy;
    uint32_t _cellRows { 0 };
    uint32_t _cellColumns { 0 };
    Cell _emptyCell;
};