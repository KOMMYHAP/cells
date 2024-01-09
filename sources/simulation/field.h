#pragma once
#include "cell.h"
#include "cell_search_proxy.h"

class Field {
    friend class FieldIterator;
    friend class ConstFieldIterator;

public:
    Field(uint32_t cellRows, uint32_t cellColumns);

    CellId Create(const Cell& cell);
    void Move(CellId id, const sf::Vector2<uint16_t>& position);
    const Cell& Get(CellId id) const;
    std::vector<CellId> Find(const sf::Vector2u& position) const;
    void Remove(CellId id);

    sf::Vector2u GetPositionLimits() const { return { GetColumnsCount() - 1, GetRowsCount() - 1 }; }
    uint32_t GetRowsCount() const { return _cellRows; }
    uint32_t GetColumnsCount() const { return _cellColumns; }

private:
    CellId MakeNextId();

private:
    std::vector<Cell> _cells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
    CellSearchProxy _searchProxy;
    uint32_t _cellRows { 0 };
    uint32_t _cellColumns { 0 };
    Cell _emptyCell;
};