#pragma once

#include "components/cell_id.h"

class IdSystem {
public:
    IdSystem(uint32_t capacity);

    uint32_t GetCellsCount() const { return _capacity - _freeIds.size(); }
    uint32_t GetCellsCountLimit() const { return _capacity; }

    CellId Create();
    void Remove(CellId id);

    template <class Func>
        requires std::invocable<Func, CellId>
    void Iterate(Func&& func)
    {
        for (uint32_t idx { 0 }; idx < _capacity; ++idx) {
            if (!_validCells[idx]) {
                continue;
            }
            const CellId id = static_cast<CellId>(idx);
            func(id);
        }
    }

private:
    CellId MakeNextId();

    uint32_t _capacity { 0 };
    std::vector<bool> _validCells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
};