#include "reproduction_system.h"

#include "cell_factories/patrol_cell.h"
#include "procedures/procedure_context.h"

ReproductionSystem::ReproductionSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm, const CellLocator& locator)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
    , _vm(&vm)
{
}

void ReproductionSystem::DoProcessComponents(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain)
{
    ProcedureContext context = _vm->RestoreDeferredExecution(id, brain);

    const CellPosition childPosition = _locator->TryApplyDirection(position, direction.value);
    if (childPosition == InvalidCellPosition) {
        context.AbortProcedure();
        return;
    }

    if (const CellId targetId = _locator->Find(childPosition); targetId != CellId::Invalid) {
        context.AbortProcedure();
        return;
    }

    EcsWorld& world = AccessEcsWorld();
    const CellId child = world.create();
    world.emplace<CellBrain>(child, MakePatrolCell(*_vm));
    world.emplace<CellPosition>(child, childPosition);
    context.CompleteProcedure();
}