#include "reproduction_system.h"

#include "cell_factories/patrol_cell.h"
#include "components/cell_type.h"
#include "procedures/procedure_context.h"

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
    const CellPosition childPosition = _locator->TryApplyDirection(position, direction.value);
    if (childPosition == InvalidCellPosition) {
        procedure.context.AbortProcedure();
        return;
    }

    if (const CellId targetId = _locator->Find(childPosition); targetId != CellId::Invalid) {
        procedure.context.AbortProcedure();
        return;
    }

    const ICellFactory::Parent parent { id, &brain, position };
    const auto [childBrain, success] = _factory->Make(parent);
    if (!success) {
        procedure.context.AbortProcedure();
        return;
    }

    EcsWorld& world = AccessEcsWorld();
    const CellId child = world.create();
    world.emplace<CellBrain>(child, childBrain);
    world.emplace<CellPosition>(child, childPosition);
    world.emplace<CellType>(child, CellType::Unit);

    _vm->CompletePendingProcedure(id, brain, procedure.context);
}