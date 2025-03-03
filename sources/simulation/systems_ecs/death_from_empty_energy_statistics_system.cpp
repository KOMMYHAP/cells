#include "generated/auto_death_from_empty_energy_statistics_system.h"
#include "simulation/simulation_statistics_provider.h"

#include "components/generated/auto_death_from_empty_energy_tag.h"
#include "components/generated/auto_graveyard_tag.h"

void DeathFromEmptyEnergyStatisticsSystem::DoSystemUpdate()
{
    const size_t cellsCount = _ecsWorld->view<DeathFromEmptyEnergyTag>(entt::exclude_t<GraveyardTag> {}).size_hint();
    _simulationStatisticsProvider->SetDeathFromEmptyEnergy(static_cast<int64_t>(cellsCount));
}