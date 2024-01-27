#pragma once

template <class Func>
    requires std::invocable<Func, CellId>
void IdSystem::Iterate(Func&& func)
{
    for (uint32_t idx { 0 }; idx < _capacity; ++idx) {
        if (!_validCells[idx]) {
            continue;
        }
        const CellId id = static_cast<CellId>(idx);
        func(id);
    }
}
