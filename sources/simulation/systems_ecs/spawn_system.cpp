#include "spawn_system.h"

#include "components/generated/auto_cell_age.h"
#include "components/generated/auto_cell_energy.h"
#include "components/generated/auto_cell_energy_decrease.h"
#include "components/generated/auto_cell_type.h"
#include "components/generated/auto_cell_unit_tag.h"

SpawnSystem::SpawnSystem(EcsWorld& world, CellLocator& locator)
    : SimulationEcsSystem(world)
    , _locator(&locator)
{
}

void SpawnSystem::DoProcessComponents(EcsEntity id, CellPosition position)
{
    EcsWorld& world = AccessEcsWorld();
    const EcsEntity targetId = _locator->Find(position);
    if (const bool canSpawnAtPosition = targetId == InvalidEcsEntity; !canSpawnAtPosition) {
        world.destroy(id);
        return;
    }
    world.remove<SpawnPlaceTag>(id);
    world.emplace<CellType>(id, CellType::Unit);
    world.emplace<CellUnitTag>(id);
    static constexpr uint8_t StartupEnergy { 255 };
    world.emplace<CellEnergy>(id, CellEnergy { StartupEnergy });
    world.emplace<CellEnergyDecrease>(id, CellEnergyDecrease { 0 });
    world.emplace<CellAge>(id, CellAge { 0 });
    _locator->Set(position, id);
}
