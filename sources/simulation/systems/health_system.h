#pragma once

#include "components/cell_health.h"
#include "components/cell_id.h"

class GraveyardSystem;

class HealthSystem {
public:
    HealthSystem(uint32_t capacity, GraveyardSystem& graveyardSystem);

    /// Tries to increment health of cell on operand and returns the old value.
    CellHealth Increment(CellId id, CellHealth health, CellHealth limit);

    /// Tries to decrement health of cell on operand and returns the old value.
    CellHealth Decrement(CellId id, CellHealth health);

    void Set(CellId id, CellHealth health);
    CellHealth Get(CellId id) const;
    bool IsZero(CellId id) const;

private:
    void BuryOnGraveyardIfNeeded(CellId id, CellHealth health);

    std::vector<CellHealth> _healthList;
    GraveyardSystem& _graveyardSystem;
};