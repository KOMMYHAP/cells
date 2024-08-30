#include "energy_leak_system.h"

EnergyLeakSystem::EnergyLeakSystem(EcsWorld& ecsWorld)
    : SimulationEcsSystem(ecsWorld)
{
}

void EnergyLeakSystem::DoProcessComponents(const CellId /*id*/, CellEnergyDecrease& change)
{
    static constexpr uint8_t LeakValue = 1;
    if (std::numeric_limits<uint8_t>::max() - LeakValue < change.value) {
        change.value = std::numeric_limits<uint8_t>::max();
        return;
    }
    change.value += LeakValue;
}