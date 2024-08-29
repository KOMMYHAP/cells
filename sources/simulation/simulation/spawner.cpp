#include "spawner.h"

#include "components/cell_age.h"
#include "components/cell_energy.h"
#include "components/cell_energy_change.h"
#include "components/cell_type.h"

Spawner::Spawner(EcsWorld& world, CellLocator& locator)
    : _world(&world)
    , _locator(&locator)
{
}

bool Spawner::CanSpawnAtPosition(CellPosition position) const
{
    const CellId targetId = _locator->Find(position);
    return targetId == CellId::Invalid;
}

void Spawner::Spawn(const CellBrain& brain, CellPosition position)
{
    const CellId id = _world->create();
    _world->emplace<CellBrain>(id, brain);
    _world->emplace<CellPosition>(id, position);
    _world->emplace<CellType>(id, CellType::Unit);
    _world->emplace<CellUnitTag>(id);
    _world->emplace<CellEnergy>(id, CellEnergy { 100 });
    _world->emplace<CellEnergyChange>(id, CellEnergyChange { 0 });
    _world->emplace<CellAge>(id, CellAge { 0 });
    _locator->Set(position, id);
}