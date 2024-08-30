#include "spawner.h"

#include "components/spawn_place_tag.h"

Spawner::Spawner(EcsWorld& world)
    : _world(&world)
{
}

EcsEntity Spawner::ScheduleSpawn(CellPosition position)
{
    const EcsEntity id = _world->create();
    _world->emplace<SpawnPlaceTag>(id);
    _world->emplace<CellPosition>(id, position);
    return id;
}