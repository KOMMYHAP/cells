#include "move_procedure.h"
#include "systems/position_system.h"
#include "systems/simulation_virtual_machine.h"

MoveProcedure::MoveProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem)
    : _positionSystem(positionSystem)
    , _vm(vm)
{
}

void MoveProcedure::Execute(ProcedureContext& context)
{
    const auto [readArgs, direction] = context.TryPopArgs<Direction>();
    if (!readArgs) {
        return;
    }

    const CellId id = _vm.GetRunningCellId();
    const CellPosition position = _positionSystem.Get(id);
    const CellPosition newPosition = _positionSystem.TryApplyDirection(position, direction);
    if (newPosition == InvalidCellPosition) {
        return;
    }

    const CellId anotherCell = _positionSystem.Find(newPosition);
    if (anotherCell != CellId::Invalid) {
        return;
    }

    _positionSystem.Set(id, newPosition);
}