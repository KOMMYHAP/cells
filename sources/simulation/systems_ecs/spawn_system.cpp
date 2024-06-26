#include "spawn_system.h"

#include "cell_factories/cell_factory_interface.h"
#include "components/cell_age.h"
#include "components/cell_brain.h"
#include "components/cell_health.h"
#include "components/cell_type.h"
#include "entt/entt.hpp"

#include "components/spawn_place.h"

SpawnSystem::SpawnSystem(EcsWorld& ecsWorld, ICellFactory& cellFactory)
    : EcsSimulationSystem(ecsWorld)
    , _cellFactory(&cellFactory)
{
}

void SpawnSystem::DoProcessComponents(CellId id, CellPosition position)
{
    EcsWorld& ecsWorld = AccessEcsWorld();

    const bool hasPrototypeComponents = ecsWorld.any_of<CellBrain, CellType, CellHealth, CellAge>(id);
    ASSERT(!hasPrototypeComponents);
    auto [status, brain] = _cellFactory->Make();
    if (status != ICellFactory::Status::Success) {
        ASSERT(status == ICellFactory::Status::TryLater);
        return;
    }

    constexpr CellHealth initialHealth { 100 };
    ecsWorld.emplace<CellBrain>(id, brain);
    ecsWorld.emplace<CellType>(id, CellType::Unit);
    ecsWorld.emplace<CellHealth>(id, initialHealth);
    ecsWorld.emplace<CellAge>(id, CellAge::Zero);
    ecsWorld.emplace<CellPosition>(id, position);

    ecsWorld.remove<SpawnPlace>(id);
}
