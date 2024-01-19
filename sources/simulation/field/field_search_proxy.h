#pragma once

#include "brain/cell.h"

class Field;

class FieldGrid {
public:
    FieldGrid(uint32_t cellsInRow, uint32_t cellsInColumn);

    std::span<const CellId> FindAll(const CellPosition& position, uint32_t searchSizeLimit) const;
    CellId Find(const CellPosition& position) const;

    void Add(const CellPosition& position, CellId id);
    void Remove(const CellPosition& position, CellId id);

    template <class Func>
        requires std::invocable<Func, const CellId&, const CellPosition&>
    void ViewAllCell(Func&& func) const
    {
        uint32_t rawGridId { 0 };
        for (uint16_t y { 0 }; y < _cellsInColumn; ++y) {
            for (uint16_t x { 0 }; x < _cellsInRow; ++x) {
                const CellId id = _grid[rawGridId];
                func(id, CellPosition { x, y });
                ++rawGridId;
            }
        }
    }

private:
    inline static constexpr uint32_t InvalidGridIndex = std::numeric_limits<uint32_t>::max();

    uint32_t TryGetGridIndex(const CellPosition& position) const;

    const uint32_t _cellsInRow;
    const uint32_t _cellsInColumn;
    std::vector<CellId> _grid;
};
