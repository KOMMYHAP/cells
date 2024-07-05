#include "look_procedure.h"

#include "components/cell_type.h"
#include "components/look_direction.h"
#include "components/procedure_result_arg.h"
#include "simulation_procedure_context.h"
#include "systems_ecs/cell_locator.h"
#include "systems_ecs/simulation_virtual_machine.h"

LookProcedure::LookProcedure(EcsWorld& world, const CellLocator& locator)
    : _world(&world)
    , _locator(&locator)
{
}

void LookProcedure::Execute(ProcedureContext& procedureContext)
{
    ASSERT_FAIL("Not implemented!");

    const auto [readArgs, rawDirection, resultAddress] = procedureContext.TryPopArgs<uint8_t, uint8_t>();
    if (!readArgs) {
        return;
    }
    Direction direction;
    if (!TryParseDirection(rawDirection, direction)) {
        procedureContext.MarkProcedureAsInvalid();
        return;
    }

    auto&& [id] = procedureContext.GetExternalContext().Get<SimulationProcedureContext>();

    _world->emplace<LookDirection>(id, direction);
    _world->emplace<ProcedureResultAddress>(id, resultAddress);

    // const CellPosition lookPosition = _locator->TryApplyDirection(position, direction);
    // if (lookPosition == InvalidCellPosition) {
    //     procedureContext.TryPushResult(CellType::Wall);
    //     return;
    // }

    //
    // const CellId anotherCell = _locator.Find(lookPosition);
    // if (anotherCell == CellId::Invalid) {
    //     procedureContext.TryPushResult(CellType::Dummy);
    //     return;
    // }

    // const CellType anotherCellType = _typeSystem.Get(anotherCell);
    // procedureContext.TryPushResult(anotherCellType);
}
