#include "spawn_system.h"

#include "components/cell_age.h"
#include "components/cell_energy.h"
#include "components/cell_energy_change.h"
#include "components/cell_type.h"
#include "components/cell_unit_tag.h"

SpawnSystem::SpawnSystem(EcsWorld& world, CellLocator& locator)
    : SimulationEcsSystem(world)
    , _locator(&locator)
{
}

void SpawnSystem::DoProcessComponents(CellId id, CellPosition position)
{
    EcsWorld& world = AccessEcsWorld();
    const CellId targetId = _locator->Find(position);
    if (const bool canSpawnAtPosition = targetId == CellId::Invalid; !canSpawnAtPosition) {
        world.destroy(id);
        return;
    }
    world.remove<SpawnPlaceTag>(id);
    world.emplace<CellType>(id, CellType::Unit);
    world.emplace<CellUnitTag>(id);
    world.emplace<CellEnergy>(id, CellEnergy { 100 });
    world.emplace<CellEnergyChange>(id, CellEnergyChange { 0 });
    world.emplace<CellAge>(id, CellAge { 0 });
    _locator->Set(position, id);
}
