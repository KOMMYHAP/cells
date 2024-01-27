#pragma once

#include "components/cell_health.h"
#include "components/cell_id.h"

class GraveyardSystem;

class HealthSystem {
public:
    HealthSystem(uint32_t capacity, GraveyardSystem& graveyardSystem);

    void Set(CellId id, CellHealth health);
    CellHealth Get(CellId id) const;

private:
    std::vector<CellHealth> _healthList;
    GraveyardSystem& _graveyardSystem;
};