#include "death_from_empty_energy_statistics_system.h"

#include "components/generated/auto_death_from_empty_energy_tag.h"

DeathFromEmptyEnergySystem::DeathFromEmptyEnergySystem(EcsWorld& world, SimulationStatisticsProvider& stats)
    : _world(&world)
    , _stats(&stats)
{
}
void DeathFromEmptyEnergySystem::DoSystemUpdate()
{
    const size_t count = _world->view<const DeathFromEmptyEnergyTag>().size();
    _stats->SetDeathFromEmptyEnergy(count);
}