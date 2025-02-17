#include "move_procedure_system.h"
#include "cell_position_utils.h"
#include "direction_utils.h"

MoveProcedureSystem::MoveProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, CellLocator& locator)
    : _world(&world)
    , _vm(&vm)
    , _locator(&locator)
{
}

MoveProcedureSystem::ExecutionStatus MoveProcedureSystem::ExecuteProcedure(EcsEntity /*id*/, ProcedureContext& context, CellBrain& /*brain*/, CellPosition& position)
{
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return ExecutionStatus::Error;
    }
    Direction direction;
    if (!TryParseDirection(rawDirection, direction)) {
        return ExecutionStatus::Error;
    }

    const CellPosition nextPosition = _locator->TryApplyDirection(position, direction);
    if (nextPosition == InvalidCellPosition) {
        return ExecutionStatus::Success;
    }
    if (const EcsEntity targetCell = _locator->Find(nextPosition); targetCell != InvalidEcsEntity) {
        return ExecutionStatus::Success;
    }

    _locator->Replace(position, nextPosition);
    position = nextPosition;
    return ExecutionStatus::Success;
}