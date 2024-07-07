#include "look_system.h"

#include "components/cell_type.h"
#include "procedures/procedure_context.h"

LookSystem::LookSystem(EcsWorld& ecsWorld, CellLocator& locator, SimulationVirtualMachine& vm)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
    , _vm(&vm)
{
}

void LookSystem::DoProcessComponents(CellId id, CellPosition position, LookDirection direction, CellBrain& brain)
{
    ProcedureContext context = _vm->RestoreDeferredExecution(id, brain);

    const CellPosition lookPosition = _locator->TryApplyDirection(position, direction.value);
    if (lookPosition == InvalidCellPosition) {
        context.TryPushResult(CellType::Wall);
        return;
    }

    if (const CellId anotherCell = _locator->Find(lookPosition); anotherCell == CellId::Invalid) {
        context.TryPushResult(CellType::Dummy);
        return;
    }

    context.TryPushResult(CellType::Unit);
    // context.CompleteProcedure();
}