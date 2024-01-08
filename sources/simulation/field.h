#pragma once
#include "cell.h"
#include "cell_search_proxy.h"

class Field {
public:
    Field(uint32_t cellRows, uint32_t cellColumns, const Cell& empty);

    CellId Create(const Cell& cell);
    void Move(CellId id, const Cell& cell);
    const Cell& Get(CellId id) const;
    std::vector<CellId> Find(const sf::Vector2u& position) const;
    void Remove(CellId id);

public:
    auto begin() { return MakeSpan().begin(); }
    auto end() { return MakeSpan().end(); }
    auto begin() const { return MakeSpan().begin(); }
    auto end() const { return MakeSpan().end(); }

private:
    CellId MakeNextId();

    std::span<Cell> MakeSpan() { return std::span { _cells.begin(), _cells.end() }; }
    std::span<const Cell> MakeSpan() const { return std::span { _cells.begin(), _cells.end() }; }

private:
    std::vector<Cell> _cells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
    CellSearchProxy _searchProxy;
    uint32_t _cellRows { 0 };
    uint32_t _cellColumns { 0 };
    const Cell _emptyCell;
};