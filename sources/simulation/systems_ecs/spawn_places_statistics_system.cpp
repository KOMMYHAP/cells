#include "spawn_places_statistics_system.h"

#include "components/spawn_place_tag.h"

SpawnPlacesStatisticsSystem::SpawnPlacesStatisticsSystem(EcsWorld& ecsWorld)
    : _world(&ecsWorld)
{
}

void SpawnPlacesStatisticsSystem::DoSystemUpdate()
{
    _spawnPlaces = _world->view<const SpawnPlaceTag>(ExcludeGraveyardedCells).size_hint();
}