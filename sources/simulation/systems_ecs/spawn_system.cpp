#include "generated/auto_spawn_system.h"

#include "components/generated/auto_cell_age.h"
#include "components/generated/auto_cell_energy.h"
#include "components/generated/auto_cell_energy_decrease.h"
#include "components/generated/auto_cell_type.h"
#include "components/generated/auto_cell_unit_tag.h"
#include "simulation/cell_locator.h"

#include <components/generated/auto_spawn_place_tag.h>

void SpawnSystem::DoProcessComponents(EcsEntity id, const CellPosition& cellPosition)
{
    EcsWorld& world = *_ecsWorld;
    const EcsEntity targetId = _cellLocator->Find(cellPosition);
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
    _cellLocator->Set(cellPosition, id);
}
