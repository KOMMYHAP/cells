#include "look_procedure_system.h"

#include "components/cell_type.h"
#include "procedures/procedure_context.h"
#include "simulation/cell_locator.h"

LookProcedureSystem::LookProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, const CellLocator& locator)
    : _world(&world)
    , _vm(&vm)
    , _locator(&locator)
{
}

auto LookProcedureSystem::ExecuteProcedure(EcsEntity /*id*/, ProcedureContext& context, CellBrain& /*brain*/, CellPosition position) -> ExecutionStatus
{
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return ExecutionStatus::Error;
    }

    Direction direction;
    if (!TryParseDirection(rawDirection, direction)) {
        return ExecutionStatus::Error;
    }

    const CellPosition lookPosition = _locator->TryApplyDirection(position, direction);
    if (lookPosition == InvalidCellPosition) {
        context.TryPushResult(CellType::Wall);
        return ExecutionStatus::Success;
    }

    if (const EcsEntity anotherCell = _locator->Find(lookPosition); anotherCell == InvalidEcsEntity) {
        context.TryPushResult(CellType::Dummy);
        return ExecutionStatus::Success;
    }

    context.TryPushResult(CellType::Unit);
    return ExecutionStatus::Success;
}
