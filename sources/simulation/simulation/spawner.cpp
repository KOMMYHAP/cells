#include "spawner.h"

#include "components/spawn_place_tag.h"

Spawner::Spawner(EcsWorld& world)
    : _world(&world)
{
}

CellId Spawner::ScheduleSpawn(CellPosition position)
{
    const CellId id = _world->create();
    _world->emplace<SpawnPlaceTag>(id);
    _world->emplace<CellPosition>(id, position);
    return id;
}