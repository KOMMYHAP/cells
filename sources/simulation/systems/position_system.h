#pragma once

#include "cell_id.h"
#include "cell_position.h"
#include "position_grid.h"

class PositionSystem {
public:
    PositionSystem(uint32_t width, uint32_t height);

    void Move(CellId id, const CellPosition& position);

    inline const static CellPosition InvalidPosition { std::numeric_limits<int16_t>::max(), std::numeric_limits<int16_t>::max() };

    const CellPosition& Get(CellId id) const;
    CellId Find(const CellPosition& position) const;

    template <class Func>
    void Iterate(Func&& func)
    {
        _grid.Iterate(std::forward<Func>(func));
    }

private:
    std::vector<CellPosition> _positions;
    PositionGrid _grid;
};