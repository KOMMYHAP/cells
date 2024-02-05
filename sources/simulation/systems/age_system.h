#pragma once

#include "components/cell_age.h"
#include "components/cell_id.h"

class HealthSystem;

class AgeSystem {
public:
    AgeSystem(uint32_t capacity, HealthSystem& healthSystem);

    void Increment(CellId id, CellAge limit);
    CellAge Get(CellId id) const;

private:
    std::vector<CellAge> _ageList;
    HealthSystem& _healthSystem;
};
