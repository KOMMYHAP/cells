#include "spawner.h"

#include "components/cell_type.h"

Spawner::Spawner(EcsWorld& world, CellLocator& locator)
    : _world(&world)
    , _locator(&locator)
{
}

bool Spawner::CanSpawnAtPosition(CellPosition position) const
{
    const CellId targetId = _locator->Find(position);
    return targetId != CellId::Invalid;
}

void Spawner::Spawn(const CellBrain& brain, CellPosition position)
{
    const CellId child = _world->create();
    _world->emplace<CellBrain>(child, brain);
    _world->emplace<CellPosition>(child, position);
    _world->emplace<CellType>(child, CellType::Unit);
    _locator->Set(position, child);
}