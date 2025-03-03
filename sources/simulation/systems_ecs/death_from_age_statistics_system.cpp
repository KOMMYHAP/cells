#include "components/generated/auto_death_from_age_tag.h"
#include "generated/auto_death_from_age_statistics_system.h"
#include "simulation/simulation_statistics_provider.h"

#include "components/generated/auto_graveyard_tag.h"

void DeathFromAgeStatisticsSystem::DoSystemUpdate()
{
    const size_t cellsCount = _ecsWorld->view<DeathFromAgeTag>(entt::exclude_t<GraveyardTag> {}).size_hint();
    _simulationStatisticsProvider->SetDeathFromAge(static_cast<int64_t>(cellsCount));
}