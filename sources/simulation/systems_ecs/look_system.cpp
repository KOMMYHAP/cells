#include "look_system.h"

#include "components/cell_type.h"
#include "procedures/procedure_context.h"

LookSystem::LookSystem(EcsWorld& ecsWorld, CellLocator& locator, SimulationVirtualMachine& vm)
    : SimulationEcsSystem(ecsWorld)
    , _locator(&locator)
    , _vm(&vm)
{
}

void LookSystem::DoProcessComponents(CellId id, CellPosition position, LookDirection direction, CellBrain& brain, DeferredProcedureExecution& procedure)
{
    Look(position, direction, procedure);
    _vm->CompletePendingProcedure(id, brain, procedure.context);
    AccessEcsWorld().remove<LookDirection, DeferredProcedureExecution>(id);
}

void LookSystem::Look(CellPosition position, LookDirection direction, DeferredProcedureExecution& procedure)
{
    const CellPosition lookPosition = _locator->TryApplyDirection(position, direction.value);
    if (lookPosition == InvalidCellPosition) {
        procedure.context.TryPushResult(CellType::Wall);
        return;
    }

    if (const CellId anotherCell = _locator->Find(lookPosition); anotherCell == CellId::Invalid) {
        procedure.context.TryPushResult(CellType::Dummy);
        return;
    }

    procedure.context.TryPushResult(CellType::Unit);
}