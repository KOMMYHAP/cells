#include "reproduction_system.h"

#include "cell_factories/patrol_cell_factory.h"
#include "components/cell_type.h"

ReproductionSystem::ReproductionSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm, const CellLocator& locator, ICellFactory& factory)
    : SimulationEcsSystem(ecsWorld)
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

    const CellPosition childPosition = _locator->TryApplyDirection(position, direction.value);
    if (childPosition == InvalidCellPosition) {
        return;
    }

    if (const CellId targetId = _locator->Find(childPosition); targetId != CellId::Invalid) {
        return;
    }

    const ICellFactory::Parent parent { id, &brain, position };
    const auto [childBrain, success] = _factory->Make(parent);
    if (!success) {
        return;
    }

    EcsWorld& world = AccessEcsWorld();
    const CellId child = world.create();
    world.emplace<CellBrain>(child, childBrain);
    world.emplace<CellPosition>(child, childPosition);
    world.emplace<CellType>(child, CellType::Unit);
}