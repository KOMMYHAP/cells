#include "look_procedure.h"

#include "components/cell_type.h"
#include "simulation_procedure_context.h"
#include "systems_ecs/cell_locator.h"
#include "systems_ecs/simulation_virtual_machine.h"

LookProcedure::LookProcedure(const SimulationVirtualMachine& vm, CellLocator& positionSystem, TypeSystem& typeSystem)
    : _vm(vm)
    , _positionSystem(positionSystem)
    , _typeSystem(typeSystem)
{
}

void LookProcedure::Execute(ProcedureContext& context)
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

    // const CellId id = *context.GetExternalContext().Get<SimulationProcedureContext>().id;
    // const CellPosition position = _positionSystem.Get(id);
    // const CellPosition lookPosition = _positionSystem.TryApplyDirection(position, direction);
    // if (lookPosition == InvalidCellPosition) {
    //     context.TryPushResult(CellType::Wall);
    //     return;
    // }
    //
    // const CellId anotherCell = _positionSystem.Find(lookPosition);
    // if (anotherCell == CellId::Invalid) {
    //     context.TryPushResult(CellType::Dummy);
    //     return;
    // }

    // const CellType anotherCellType = _typeSystem.Get(anotherCell);
    // context.TryPushResult(anotherCellType);
}
