#include "reproduction_system.h"

#include "cell_factories/patrol_cell_factory.h"

ReproductionSystem::ReproductionSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm, const CellLocator& locator, RandomCellFactory& factory, Spawner& spawner)
    : SimulationEcsSystem(ecsWorld)
    , _spawner(&spawner)
    , _factory(&factory)
    , _locator(&locator)
    , _vm(&vm)
{
}

void ReproductionSystem::DoProcessComponents(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure)
{
    MakeClone(id, position, direction, brain, procedure);
    AccessEcsWorld().remove<ReproductionDirection, DeferredProcedureExecution>(id);
}

void ReproductionSystem::MakeClone(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure)
{
    // Make brain of parent cell more stable before making a child.
    _vm->CompletePendingProcedure(id, brain, procedure.context);

    _spawner->TrySpawn(position, direction.value, [this](CellBrain& childBrain) { return _factory->Make(childBrain); });
}