#include "move_procedure_system.h"

MoveProcedureSystem::MoveProcedureSystem(EcsWorld& world, SimulationVirtualMachine& vm, CellLocator& locator)
    : _world(&world)
    , _vm(&vm)
    , _locator(&locator)
{
}

MoveProcedureSystem::ExecutionStatus MoveProcedureSystem::ExecuteProcedure(CellId id, ProcedureContext& context, CellBrain& brain, CellPosition& position)
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
    if (const CellId targetCell = _locator->Find(nextPosition); targetCell != CellId::Invalid) {
        return ExecutionStatus::Success;
    }

    _locator->Replace(position, nextPosition);
    position = nextPosition;
    return ExecutionStatus::Success;
}