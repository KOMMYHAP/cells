#include "generated/auto_spawn_places_statistics_system.h"
#include "graveyarded_cell_exclude.h"

#include "components/generated/auto_spawn_place_tag.h"
#include "simulation/simulation_statistics_provider.h"

void SpawnPlacesStatisticsSystem::DoSystemUpdate()
{
    const size_t count = _ecsWorld->view<const SpawnPlaceTag>(ExcludeGraveyardedCells).size_hint();
    _simulationStatisticsProvider->SetSpawnPlacesCount(count);
}