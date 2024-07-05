#include "look_procedure.h"

#include "components/look_direction.h"
#include "components/procedure_result.h"
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
    const auto [readArgs, rawDirection] = procedureContext.TryPopArgs<uint8_t>();
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
    _world->emplace<ProcedureResult>(id, procedureContext);
}
