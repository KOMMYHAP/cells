#pragma once

#include "components/cell_health.h"
#include "components/cell_id.h"

class GraveyardSystem;

class HealthSystem {
public:
    HealthSystem(uint32_t capacity, GraveyardSystem& graveyardSystem);

    CellHealth Increment(CellId id, CellHealth health, CellHealth limit);
    CellHealth Decrement(CellId id, CellHealth health);

    void Set(CellId id, CellHealth health);
    CellHealth Get(CellId id) const;
    bool IsZero(CellId id) const;

private:
    void BuryOnGraveyardIfNeeded(CellId id, CellHealth health);

    std::vector<CellHealth> _healthList;
    GraveyardSystem& _graveyardSystem;
};