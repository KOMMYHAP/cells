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
    ASSERT(index <= _healthList.size());

    _healthList[index] = health;
    BuryOnGraveyardIfNeeded(id, health);
}

CellHealth HealthSystem::Get(CellId id) const
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _healthList.size());
    return _healthList[index];
}

CellHealth HealthSystem::Increment(CellId id, CellHealth health, CellHealth limit)
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _healthList.size());

    if (_healthList[index] == CellHealth::Zero) {
        return CellHealth::Zero;
    }

    const auto healthCurrent = static_cast<int32_t>(_healthList[index]);
    auto healthDiff = static_cast<int32_t>(health);
    const auto healthLimit = static_cast<int32_t>(limit);

    if (healthCurrent + healthDiff > healthLimit) {
        healthDiff = healthLimit - healthCurrent;
    }

    const auto healthNew = static_cast<CellHealth>(healthCurrent + healthDiff);
    _healthList[index] = healthNew;

    BuryOnGraveyardIfNeeded(id, healthNew);
    return static_cast<CellHealth>(healthCurrent);
}

CellHealth HealthSystem::Decrement(CellId id, CellHealth health)
{
    const auto index = CellIdToInt(id);
    ASSERT(index <= _healthList.size());

    if (_healthList[index] == CellHealth::Zero) {
        return CellHealth::Zero;
    }

    const auto healthCurrent = static_cast<int32_t>(_healthList[index]);
    auto healthDiff = static_cast<int32_t>(health);
    constexpr auto healthLimit = static_cast<int32_t>(CellHealth::Zero);

    if (healthCurrent - healthDiff < healthLimit) {
        healthDiff = healthCurrent;
    }

    const auto healthNew = static_cast<CellHealth>(healthCurrent - healthDiff);
    _healthList[index] = healthNew;

    BuryOnGraveyardIfNeeded(id, healthNew);
    return healthNew;
}

void HealthSystem::BuryOnGraveyardIfNeeded(CellId id, CellHealth health)
{
    if (health != CellHealth::Zero) {
        return;
    }

    _graveyardSystem.Bury(id);
}

bool HealthSystem::IsZero(CellId id) const
{
    return Get(id) == CellHealth::Zero;
}