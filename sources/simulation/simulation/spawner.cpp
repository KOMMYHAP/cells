#include "spawner.h"

#include "components/generated/auto_spawn_place_tag.h"

Spawner::Spawner(EcsWorld& world, const CellLocator& locator)
    : _world(&world)
    , _locator(&locator)
{
}

EcsEntity Spawner::ScheduleSpawn(CellPosition position)
{
    ASSERT(_locator->IsValidPosition(position), "Invalid position");
    const EcsEntity id = _world->create();
    _world->emplace<SpawnPlaceTag>(id);
    _world->emplace<CellPosition>(id, position);
    return id;
}