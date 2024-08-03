#include "look_procedure.h"

#include "components/deferred_procedure_execution.h"
#include "components/look_direction.h"
#include "procedures/procedure_context.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_procedure_context.h"

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
        procedureContext.AbortProcedure();
        return;
    }

    const CellId id = procedureContext.GetUserData().Get<SimulationProcedureContext>().id;
    _world->emplace<LookDirection>(id, direction);
    _world->emplace<DeferredProcedureExecution>(id, procedureContext);
}
