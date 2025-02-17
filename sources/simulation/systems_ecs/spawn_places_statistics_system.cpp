#include "spawn_places_statistics_system.h"

#include "components/generated/spawn_place_tag.h"

SpawnPlacesStatisticsSystem::SpawnPlacesStatisticsSystem(EcsWorld& world, SimulationStatisticsProvider& stats)
    : _world(&world)
    , _stats(&stats)
{
}

void SpawnPlacesStatisticsSystem::DoSystemUpdate()
{
    const size_t count = _world->view<const SpawnPlaceTag>(ExcludeGraveyardedCells).size_hint();
    _stats->SetSpawnPlacesCount(count);
}