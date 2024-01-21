#pragma once

#include "components/cell_id.h"
#include "components/cell_position.h"
#include "position_grid.h"

class PositionSystem {
public:
    PositionSystem(uint32_t width, uint32_t height);

    void Move(CellId id, const CellPosition& position);

    const CellPosition& Get(CellId id) const;
    CellId Find(const CellPosition& position) const;

    template <class Func>
    void Iterate(Func&& func)
    {
        _grid.Iterate(std::forward<Func>(func));
    }

    uint16_t GetWidth() const { return _grid.GetWidth(); }
    uint16_t GetHeight() const { return _grid.GetHeight(); }

private:
    std::vector<CellPosition> _positions;
    PositionGrid _grid;
};