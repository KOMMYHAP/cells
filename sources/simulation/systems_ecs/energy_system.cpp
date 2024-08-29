#include "energy_system.h"

#include "components/graveyard_tag.h"

EnergySystem::EnergySystem(EcsWorld& ecsWorld)
    : SimulationEcsSystem(ecsWorld)
{
}

void EnergySystem::DoProcessComponents(const CellId id, CellEnergy& energy, const CellEnergyChange change)
{
    const int16_t startEnergy = energy.value;
    const int16_t diffEnergy = change.value;
    static constexpr int16_t LimitEnergy { 255 };

    const int16_t endEnergy = startEnergy + diffEnergy;
    energy.value = static_cast<uint8_t>(std::clamp(endEnergy, static_cast<int16_t>(0), LimitEnergy));

    if (energy.value == 0) {
        EcsWorld& world = AccessEcsWorld();
        world.emplace_or_replace<GraveyardTag>(id);
    }
}