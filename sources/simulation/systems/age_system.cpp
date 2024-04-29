#include "age_system.h"
#include "health_system.h"

AgeSystem::AgeSystem(uint32_t capacity, HealthSystem& healthSystem)
    : _ageList(capacity, CellAge::Zero)
    , _healthSystem(healthSystem)
{
}

void AgeSystem::Increment(CellId id, CellAge limitAge)
{
     if (_healthSystem.IsZero(id)) {
        // cell is dead
        return;
    }

    const auto index = CellIdToInt(id);
    ASSERT(index <= _ageList.size());
    const CellAge currentAge = _ageList[index];

    const uint16_t current = static_cast<uint16_t>(currentAge) + 1;
    const uint16_t limit = static_cast<uint16_t>(limitAge);
    if (current >= limit) {
        _healthSystem.Set(id, CellHealth::Zero);
        return;
    }

    _ageList[index] = static_cast<CellAge>(current);
}

CellAge AgeSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _ageList.size());
    return _ageList[index];
}

void AgeSystem::Set(CellId id, CellAge age)
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _ageList.size());
    _ageList[index] = age;
}
