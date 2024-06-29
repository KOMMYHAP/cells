#include "consume_procedure.h"
#include "simulation_procedure_context.h"
#include "systems_ecs/position_system.h"
#include "systems_ecs/simulation_virtual_machine.h"

ConsumeProcedure::ConsumeProcedure(const SimulationVirtualMachine& vm, PositionSystem& positionSystem, HealthSystem& healthSystem, TypeSystem& typeSystem)
    : _vm(vm)
    , _positionSystem(positionSystem)
    , _healthSystem(healthSystem)
    , _typeSystem(typeSystem)
{
}

void ConsumeProcedure::Execute(ProcedureContext& context)
{
    ASSERT_FAIL("Not implemented!");
    const auto [readArgs, rawDirection] = context.TryPopArgs<uint8_t>();
    if (!readArgs) {
        return;
    }
    MoveDirection direction;
    if (!TryParseMoveDirection(rawDirection, direction)) {
        context.MarkProcedureAsInvalid();
        return;
    }

    const CellId id = *context.GetExternalContext().Get<SimulationProcedureContext>().id;

    // constexpr CellHealth healthPerAction { 5 };
    // if (_healthSystem.Decrement(id, healthPerAction) == CellHealth::Zero) {
    //     return;
    // }

    const CellPosition position = _positionSystem.Get(id);
    const CellPosition newPosition = _positionSystem.TryApplyDirection(position, direction);
    if (newPosition == InvalidCellPosition) {
        context.MarkProcedureAsInvalid();
        return;
    }
    const CellId anotherCell = _positionSystem.Find(newPosition);
    if (anotherCell == CellId::Invalid) {
        return;
    }

    // const CellType anotherCellType = _typeSystem.Get(anotherCell);
    // if (anotherCellType == CellType::Unit) {
    //     constexpr CellHealth damagePerHit { 25 };
    //     _healthSystem.Decrement(anotherCell, damagePerHit);
    // } else if (anotherCellType == CellType::Food) {
    //     constexpr CellHealth healthPerFood { 15 };
    //     constexpr CellHealth limitHealthAfterFood { 100 };
    //     _healthSystem.Increment(anotherCell, healthPerFood, limitHealthAfterFood);
    // }
}