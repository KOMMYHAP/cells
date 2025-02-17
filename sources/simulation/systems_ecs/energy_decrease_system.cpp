#include "energy_decrease_system.h"

#include "components/generated/auto_death_from_empty_energy_tag.h"
#include "components/generated/auto_graveyard_tag.h"

EnergyDecreaseSystem::EnergyDecreaseSystem(EcsWorld& ecsWorld)
    : SimulationEcsSystem(ecsWorld)
{
}

void EnergyDecreaseSystem::DoProcessComponents(const EcsEntity id, CellEnergy& energy, CellEnergyDecrease& change)
{
    if (change.value == 0) {
        return;
    }

    const int endEnergy = NarrowCast<int>(energy.value) - NarrowCast<int>(change.value);
    energy.value = NarrowCast<uint8_t>(std::max(0, endEnergy));
    change.value = 0;

    if (energy.value == 0) {
        EcsWorld& world = AccessEcsWorld();
        world.emplace_or_replace<GraveyardTag>(id);
        world.emplace<DeathFromEmptyEnergyTag>(id);
    }
}