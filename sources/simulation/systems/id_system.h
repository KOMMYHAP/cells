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
    void Iterate(Func&& func);

private:
    CellId MakeNextId();

    uint32_t _capacity { 0 };
    std::vector<bool> _validCells;
    std::vector<CellId> _freeIds;
    CellId _nextId { 0 };
};

#include "id_system.hpp"