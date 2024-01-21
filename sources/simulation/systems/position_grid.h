#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"

class PositionGrid {
public:
    PositionGrid(uint32_t width, uint32_t height);

    CellId Find(const CellPosition& position) const;

    void Add(CellId id, const CellPosition& position);
    void Remove(CellId id, const CellPosition& position);

    template <class Func>
        requires std::invocable<Func, CellPosition>
    void Iterate(Func&& func)
    {
        for (int16_t y { 0 }; y < _height; ++y) {
            for (int16_t x { 0 }; x < _width; ++x) {
                func(CellPosition { x, y });
            }
        }
    }

private:
    inline static constexpr uint32_t InvalidGridIndex = std::numeric_limits<uint32_t>::max();

    uint32_t TryGetGridIndex(const CellPosition& position) const;

    const uint32_t _width;
    const uint32_t _height;
    std::vector<CellId> _grid;
};
