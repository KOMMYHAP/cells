#include "reproduction_system.h"

#include "cell_factories/patrol_cell.h"
#include "components/deferred_procedure_execution.h"
#include "procedures/procedure_context.h"

ReproductionSystem::ReproductionSystem(EcsWorld& ecsWorld, SimulationVirtualMachine& vm, const CellLocator& locator, Random::Accessor random)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
    , _vm(&vm)
    , _random(random)
{
}

void ReproductionSystem::DoProcessComponents(CellId id, CellPosition position, ReproductionDirection direction, CellBrain& brain, DeferredProcedureExecution)
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
}