#include "generated/auto_energy_leak_system.h"

void EnergyLeakSystem::DoProcessComponents(const EcsEntity /*id*/, CellEnergyDecrease& change)
{
    static constexpr uint8_t LeakValue = 1;
    if (std::numeric_limits<uint8_t>::max() - LeakValue < change.value) {
        change.value = std::numeric_limits<uint8_t>::max();
        return;
    }
    change.value += LeakValue;
}