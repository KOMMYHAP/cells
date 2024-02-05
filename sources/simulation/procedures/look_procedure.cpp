#include "look_procedure.h"

#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"
#include "systems/type_system.h"

LookProcedure::LookProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem, TypeSystem& typeSystem)
    : _vm(vm)
    , _positionSystem(positionSystem)
    , _typeSystem(typeSystem)
{
}

void LookProcedure::Execute(ProcedureContext& context)
{
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return;
    }
    Direction direction;
    if (!TryParse(rawDirection, direction)) {
        context.MarkProcedureAsInvalid();
        return;
    }

    const CellId id = _vm.GetRunningCellId();
    const CellPosition position = _positionSystem.Get(id);
    const CellPosition lookPosition = _positionSystem.TryApplyDirection(position, direction);
    if (lookPosition == InvalidCellPosition) {
        context.TryPushResult(CellType::Wall);
        return;
    }

    const CellId anotherCell = _positionSystem.Find(lookPosition);
    const CellType anotherCellType = _typeSystem.Get(anotherCell);
    context.TryPushResult(anotherCellType);
}
