#include "health_system.h"
#include "graveyard_system.h"

HealthSystem::HealthSystem(uint32_t capacity, GraveyardSystem& graveyardSystem)
    : _healthList(capacity, CellHealth::Zero)
    , _graveyardSystem(graveyardSystem)
{
}

void HealthSystem::Set(CellId id, CellHealth health)
{
    const auto index = CellIdToInt(id);
    assert(index <= _healthList.size());
    _healthList[index] = health;

    if (health == CellHealth::Zero) {
        _graveyardSystem.Bury(id);
    }
}

CellHealth HealthSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    assert(index <= _healthList.size());
    return _healthList[index];
}
