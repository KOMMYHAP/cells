#include "move_procedure.h"
#include "systems/position_system.h"

MoveProcedure::MoveProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem)
    : _positionSystem(positionSystem)
    , _vm(vm)
{
}

void MoveProcedure::Execute(ProcedureContext& context)
{
    const auto [readArgs, direction] = context.TryPopArgs<MoveDirection>();
    if (!readArgs) {
        return;
    }

    const CellId id = _vm.GetRunningCellId();
    const CellPosition position = _positionSystem.Get(id);
    const CellPosition newPosition = TryApplyDirection(position, direction);
    if (newPosition == InvalidCellPosition) {
        return;
    }

    const CellId anotherCell = _positionSystem.Find(newPosition);
    if (anotherCell != CellId::Invalid) {
        return;
    }

    _positionSystem.Move(id, newPosition);
}

CellPosition MoveProcedure::TryApplyDirection(CellPosition position, MoveDirection direction)
{
    switch (direction) {
    case MoveDirection::Left:
        if (position.x >= 1) {
            position.x -= 1;
            return position;
        }
        break;
    case MoveDirection::Right:
        if (position.x + 1 < _positionSystem.GetWidth()) {
            position.x += 1;
            return position;
        }
        break;
    case MoveDirection::Up:
        if (position.y >= 1) {
            position.y -= 1;
            return position;
        }
        break;
    case MoveDirection::Down:
        if (position.y + 1 < _positionSystem.GetHeight()) {
            position.y += 1;
            return position;
        }
        break;
    default:
        return InvalidCellPosition;
    }
    return InvalidCellPosition;
}