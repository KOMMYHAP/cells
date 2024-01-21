#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"

class PositionGrid {
public:
    PositionGrid(uint16_t width, uint16_t height);

    CellId Find(const CellPosition& position) const;

    void Add(CellId id, const CellPosition& position);
    void Remove(CellId id, const CellPosition& position);

    uint16_t GetWidth() const { return _width; }
    uint16_t GetHeight() const { return _height; }

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

    const uint16_t _width;
    const uint16_t _height;
    std::vector<CellId> _grid;
};
